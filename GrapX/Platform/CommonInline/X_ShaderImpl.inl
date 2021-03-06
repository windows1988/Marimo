﻿const int c_D3D_INCLUDE_LOCAL  = 0;
const int c_D3D_INCLUDE_SYSTEM = 1;

#define GRAPX_hModule NULL

template<class _ID3DIncludeT, typename _IncludeTypeT>
class IHLSLIncludeT : public _ID3DIncludeT
{
private:
  typedef clhash_map<clStringA, clBuffer*> IncDict;
  Graphics* m_pGraphics;
  clStringA   m_strBaseDir;
  IncDict     m_IncFiles;
  const GXSHADER_SOURCE_DESC* m_pDesc = NULL;

public:
  void SetDesc(const GXSHADER_SOURCE_DESC* pDesc)
  {
    m_pDesc = pDesc;
  }

public:
  IHLSLIncludeT(Graphics* pGraphics, GXLPCSTR szBaseDir)
    : m_pGraphics(pGraphics)
    , m_strBaseDir(szBaseDir != NULL ? szBaseDir : "")
  {
  }

  virtual ~IHLSLIncludeT()
  {
    for(IncDict::iterator it = m_IncFiles.begin();
      it != m_IncFiles.end(); ++it)
    {
      clBuffer* pBuffer = it->second;
      delete pBuffer;
    }
    m_IncFiles.clear();
  }

  STDMETHOD(Open)(_IncludeTypeT IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
  {
    //extern GXLPCWSTR g_szPlatformSourceCodePath;
    //extern GXLPCWSTR g_szGraphicsImplSourceCodePath;
    if(IncludeType == c_D3D_INCLUDE_SYSTEM)
    {
      HRSRC hRsrc = NULL;
      if(clstd::strcmpiT(pFileName, "marimo.cgh") == 0)
      {
        if(m_pGraphics->GetPlatformID() == GXPLATFORM_WIN32_DIRECT3D11)
        {
          hRsrc = FindResource(g_hDLLModule, MAKEINTRESOURCE(IDR_MARIMO_DX11), _T("raw"));
        }
        else {
          CLBREAK;
        }

      }
      else if(clstd::strcmpiT(pFileName, "CommonUniform.h") == 0)
      {
        hRsrc = FindResource(g_hDLLModule, MAKEINTRESOURCE(IDR_COMMON), _T("raw"));
      }

      if(hRsrc)
      {
        HGLOBAL hGlb = LoadResource(g_hDLLModule, hRsrc);
        if(hGlb)
        {
          *ppData = LockResource(hGlb);
          *pBytes = SizeofResource(g_hDLLModule, hRsrc);
          return S_OK;

          // 16位遗留api，不需要调用释放资源
          //FreeResource(hGlb);
          //UnlockResource()
        }
      }
      CLOG_ERROR("Can not find resource %s", pFileName);
    }
    else if(IncludeType == c_D3D_INCLUDE_LOCAL)
    {
      clFile file;
      clStringA strFullPath;

      //if(clpathfile::IsRelative(pFileName))
      //{
      //  if(m_pGraphics != NULL) {
      //    strFullPath = pFileName;
      //    m_pGraphics->ConvertToAbsolutePathA(strFullPath);
      //  }
      //  else if(m_strBaseDir.IsNotEmpty()) {
      //    clpathfile::CombinePath(strFullPath, m_strBaseDir, pFileName);
      //  }
      //  else {
      //    strFullPath = pFileName;
      //  }
      //}
      //else {
      //  strFullPath = pFileName;
      //}
      if(_CL_NOT_(FindLocalFile(strFullPath, pFileName)))
      {
        return E_FAIL;
      }

      // 查找 Include 是否已经存在
      IncDict::iterator it = m_IncFiles.find(strFullPath);
      if(it != m_IncFiles.end()) {
        *ppData = it->second->GetPtr();
        *pBytes = (UINT)it->second->GetSize();
        return S_OK;
      }

      // 打开并映射文件
      if(file.OpenExisting(strFullPath))
      {
        clBuffer* pBuffer = new clBuffer(128);
        if(file.GetSize(NULL) == 0)
        {
          //pBuffer = new clBuffer(128);
        }
        else if( ! file.Read(pBuffer))
        {
          return E_FAIL;
        }

        clStringA strLineInfo;
        strLineInfo.Format("#line 1 \"%s\"\r\n", strFullPath);
        pBuffer->Insert(0, strLineInfo.GetBuffer(), strLineInfo.GetLength());

        *ppData = pBuffer->GetPtr();
        *pBytes = (UINT)pBuffer->GetSize();
        m_IncFiles[strFullPath] = pBuffer;
        return S_OK;
      }
    }
    return E_FAIL;
  }

  STDMETHOD(Close)(LPCVOID pData)
  {
    return S_OK;
  }

  GXBOOL FindLocalFile(clStringA& strFullPath, LPCSTR pFileName)
  {
    GXLPCWSTR* pSearchingDirectiory =  m_pDesc->pHeaderDirectory;
    for(int i = 0; pSearchingDirectiory && pSearchingDirectiory[i] != NULL; i++)
    {
      clStringA strDirectoryA = pSearchingDirectiory[i];
      if(clpathfile::IsPathExist(clpathfile::CombinePath(strFullPath, strDirectoryA, pFileName)))
      {
        return TRUE;
      }
    }

    if (m_pGraphics != NULL) {
      strFullPath = pFileName;
      m_pGraphics->ConvertToAbsolutePathA(strFullPath);
      if(clpathfile::IsPathExist(strFullPath)) {
        return TRUE;
      }
    }
    
    if (m_strBaseDir.IsNotEmpty() && 
      clpathfile::IsPathExist(clpathfile::CombinePath(strFullPath, m_strBaseDir, pFileName))) {
      return TRUE;
    }
    return FALSE;
  }
};

#ifdef _WIN32_DIRECT3D_9_H_
typedef IHLSLIncludeT<ID3DXInclude, D3DXINCLUDE_TYPE> IHLSLInclude;
#elif defined(_WIN32_DIRECT3D_11_H_)
typedef IHLSLIncludeT<ID3D10Include, D3D_INCLUDE_TYPE> IHLSLInclude;
#endif // #ifdef _WIN32_DIRECT3D_9_H_

#ifdef REFACTOR_GRAPX_SHADER
GXHRESULT GShaderImpl::LoadFromFile(MOSHADER_ELEMENT_SOURCE* pSdrElementSrc)
{
  GXDefinitionArray aMacros;
  MOSHADERBUFFERS ShaderBuffers;
  ShaderBuffers.pVertexShader = NULL;
  ShaderBuffers.pPixelShader  = NULL;
  GXDEFINITION* pShaderMacro = NULL;
  IHLSLInclude* pInclude = NULL;
  GXHRESULT hr = GX_OK;

  GXPlatformIdentity pltid;
  m_pGraphicsImpl->GetPlatformID(&pltid);
  ASSERT(pltid == GXPLATFORM_WIN32_DIRECT3D9 || pltid == GXPLATFORM_WIN32_DIRECT3D11);

  if(GShader::ComposeSource(pSdrElementSrc, pltid, &ShaderBuffers, &aMacros))
  {
    clBuffer* const pVertexBuffer = ShaderBuffers.pVertexShader;
    clBuffer* const pPixelBuffer  = ShaderBuffers.pPixelShader;
    const GXBOOL bCompiledVS = ((GXLPBYTE)pVertexBuffer->GetPtr())[0] == '\0';
    const GXBOOL bCompiledPS = ((GXLPBYTE)pPixelBuffer->GetPtr() )[0] == '\0';


    if( ! (bCompiledPS && bCompiledVS))
    {
      if( ! aMacros.empty())
      {
        int i = 0;
        pShaderMacro = new GXDEFINITION[aMacros.size() + 1];

        // 这个是为了把宏数组的结尾填为 NULL, clString 无法做到这一点.
        for(GXDefinitionArray::iterator it = aMacros.begin();
          it != aMacros.end(); ++it, ++i) {
            pShaderMacro[i].szName = it->Name;
            pShaderMacro[i].szValue = it->Value;
        }
        pShaderMacro[i].szName = NULL;
        pShaderMacro[i].szValue = NULL;
      }

      pInclude = new IHLSLInclude(m_pGraphicsImpl, NULL);
    }

    // 注意: 输入的pVertexBuffer和pPixelBuffer会改变
    if( ! bCompiledVS)
    {
      const GXBOOL bComposing = pSdrElementSrc->strVSComposer.IsNotEmpty();
      hr = CompileShader(pVertexBuffer, pInclude, pShaderMacro, bComposing ? CompiledComponentVertexShder : CompiledVertexShder);
      if(GXFAILED(hr)) {
        goto FUNC_RET;
      }
    }

    if( ! bCompiledPS)
    {
      const GXBOOL bComposing = pSdrElementSrc->strPSComposer.IsNotEmpty();
      hr = CompileShader(pPixelBuffer, pInclude, pShaderMacro, bComposing ? CompiledComponentPixelShder : CompiledPixelShder);
      if(GXFAILED(hr)) {
        goto FUNC_RET;
      }
    }

    hr = LoadFromMemory(pVertexBuffer, pPixelBuffer);
  }
  else {
    hr = GX_FAIL;
  }

FUNC_RET:
  SAFE_DELETE(pInclude);
  SAFE_DELETE_ARRAY(pShaderMacro);
  SAFE_DELETE(ShaderBuffers.pVertexShader);
  SAFE_DELETE(ShaderBuffers.pPixelShader);

  return hr;
}
#endif // #ifdef REFACTOR_GRAPX_SHADER