#ifndef _STATE_BLOCK_H_
#define _STATE_BLOCK_H_

//#include <Include/GUnknown.H>




#define RENDERSTATECOUNT  103    // ö�����������
#define LASTRENDERSTATEENUM  209    // ���һ��ö��ֵ

#define SAMPLERCOUNT      16
#define SAMPSTATECOUNT    13
#define LASTSAMPSTSTEENUM 13

//#include <vector>
//using namespace std;

class GXGraphics;

namespace D3D9
{
  class GXGraphicsImpl;
  //class GRenderStateImpl : public GUnknown
  //{
  //  friend class GXGraphicsImpl;
  //  friend class GXCanvasCoreImpl;
  //  friend class GXCanvas3DImpl;
  //private:
  //  struct GROUPMASK  // �������� 
  //  {
  //    union{
  //      GXDWORD  dw;
  //      GXWORD  w[2];
  //      GXBYTE  b[4];
  //    };
  //  };
  //private:
  //  GRenderStateImpl(GXGraphics* pGraphics);
  //  static GXBOOL     InitializeStatic  ();
  //  GXBOOL            Update            (GRenderStateImpl* pPrevState);
  //  GXBOOL            ResetToDefault    ();
  //public:
  //  virtual GXLRESULT AddRef            ();
  //  virtual GXLRESULT Release           ();

  //  GXBOOL            Set               (GXRenderStateType eType, GXDWORD dwValue);
  //  GXDWORD           Get               (GXRenderStateType eType);
  //  GXBOOL            SetBlock          (GXLPCRENDERSTATE lpBlock);
  //private:
  //  static GXINT                s_aEnumToIdx           [LASTRENDERSTATEENUM];
  //  static GXDWORD              s_aRenderStateValue    [RENDERSTATECOUNT];
  //  static GXRenderStateType    s_aRenderStateTypeList [RENDERSTATECOUNT + 1];
  //  GXGraphicsImpl*   m_pGraphics;
  //  GROUPMASK         m_aChanged[(RENDERSTATECOUNT + 31) / 32];
  //  GXDWORD           m_aRenderStateValue[RENDERSTATECOUNT];
  //  GXBOOL            m_bOnDevice;
  //};

  class GRasterizerStateImpl : public GRasterizerState
  {
  private:
    GXGraphicsImpl*   m_pGraphicsImpl;
    GXRASTERIZERDESC  m_RasterizerDesc;
  public:
#ifdef ENABLE_VIRTUALIZE_ADDREF_RELEASE
    virtual GXHRESULT AddRef        ();
    virtual GXHRESULT Release       ();
#endif // #ifdef ENABLE_VIRTUALIZE_ADDREF_RELEASE
    virtual GXHRESULT Invoke        (GRESCRIPTDESC* pDesc){ return GX_OK; }

  public:
    GRasterizerStateImpl(GXGraphicsImpl*        pGraphicsImpl);
    virtual ~GRasterizerStateImpl();
    GXBOOL  Initialize  (GXRASTERIZERDESC*      pDesc);
    GXBOOL  Activate    (GRasterizerStateImpl*  pPrevState);
  };

  class GBlendStateImpl : public GBlendState
  {
  private:
    GXGraphicsImpl* m_pGraphicsImpl;
    GXDWORD         m_BlendFactor;
    GXBLENDDESC     m_BlendDesc;

  public:
#ifdef ENABLE_VIRTUALIZE_ADDREF_RELEASE
    virtual GXHRESULT AddRef            ();
    virtual GXHRESULT Release           ();
#endif // #ifdef ENABLE_VIRTUALIZE_ADDREF_RELEASE
    virtual GXHRESULT Invoke        (GRESCRIPTDESC* pDesc) { return GX_OK; }

  public:
    GBlendStateImpl(GXGraphicsImpl* pGraphicsImpl);
    virtual ~GBlendStateImpl();
    GXBOOL  Initialize  (GXBLENDDESC* pDesc, GXUINT nNum);
    GXBOOL  Activate    (GBlendStateImpl* pPrevState);

    virtual GXDWORD SetBlendFactor  (GXDWORD dwBlendFactor);

    inline  GXBOOL IsInvalidBlend (GXBlend eBlend);
  };

  inline GXBOOL GBlendStateImpl::IsInvalidBlend(GXBlend eBlend)
  {
    return eBlend < 1 || eBlend > 17;
  }
  //////////////////////////////////////////////////////////////////////////
  class GDepthStencilStateImpl : public GDepthStencilState
  {
    friend class GXGraphicsImpl;
  private:
    GXGraphicsImpl*     m_pGraphicsImpl;
    GXDWORD             m_StencilRef;
    GXDEPTHSTENCILDESC  m_Desc;

  public:
#ifdef ENABLE_VIRTUALIZE_ADDREF_RELEASE
    virtual GXHRESULT AddRef            ();
    virtual GXHRESULT Release           ();
#endif // #ifdef ENABLE_VIRTUALIZE_ADDREF_RELEASE
    virtual GXHRESULT Invoke        (GRESCRIPTDESC* pDesc) { return GX_OK; }


  public:
    GDepthStencilStateImpl(GXGraphicsImpl* pGraphicsImpl);
    virtual ~GDepthStencilStateImpl();
    GXBOOL  Initialize      (GXDEPTHSTENCILDESC* pDesc);
    GXBOOL  Activate        (GDepthStencilStateImpl* pPrevState);
    GXBOOL  CheckStencilOp  (GXDEPTHSTENCILOP* pStencilOp, GXLPCSTR szPrefix);

    virtual GXDWORD SetStencilRef  (GXDWORD dwStencilRef);
  };
  //////////////////////////////////////////////////////////////////////////

  //struct GXSAMPLERSTATE
  //{
  //  D3DSAMPLERSTATETYPE eType;
  //  GXDWORD             dwValue;
  //};

  //struct GXSAMPLERSTAGE
  //{
  //  union {
  //    struct
  //    {
  //      GXDWORD dwMask;     // ���State��¼��Ĭ��ֵ���,���Ӧλ��"0"
  //      GXDWORD dwAddressU;
  //      GXDWORD dwAddressV;
  //      GXDWORD dwAddressW;
  //      GXDWORD dwBorderColor;
  //      GXDWORD dwMagFilter;
  //      GXDWORD dwMinFilter;
  //      GXDWORD dwMipFilter;
  //      GXDWORD dwMipmapLodBias;
  //      GXDWORD dwMaxMipLevel;
  //      GXDWORD dwMaxAnisotropy;
  //      GXDWORD dwSRGBTexture;
  //      GXDWORD dwElementIndex;
  //      GXDWORD dwDMapOffset;
  //    };
  //    //STATIC_ASSERT(sizeof(DETAIL) == sizeof(GXDWORD) * 13);
  //    GXDWORD m[14];
  //  };
  //};

  //typedef GXSAMPLERSTATE* LPGXSAMPLERSTATE;
  //typedef GXSAMPLERSTATE* GXLPSAMPLERSTATE;
  //typedef GXSAMPLERSTAGE* GXLPSAMPLERSTAGE;
  //typedef GXCONST GXSAMPLERSTAGE* GXLPCSAMPLERSTAGE;
  //void IntSetSamplerToDefault(GXLPSAMPLERSTAGE lpSampStage);

  class GSamplerStateImpl : public GSamplerState
  {
    friend class GXGraphicsImpl;
    friend class GXCanvas;
  private:
    GXGraphicsImpl*       m_pGraphicsImpl;
    //GXSAMPLERSTAGE        m_SamplerStage[SAMPLERCOUNT];
    GXSAMPLERDESC         m_SamplerDesc[SAMPLERCOUNT];
    GXDWORD               m_dwDefaultMask[SAMPLERCOUNT];  // �����Ĭ��ֵ,��Ӧλ��0
    //static GXSAMPLERSTAGE s_DefaultSamplerState;
  private:
    GSamplerStateImpl(GXGraphics* pGraphics);
    virtual ~GSamplerStateImpl();

    static GXBOOL     InitializeStatic  ();
    GXBOOL            Activate          (GSamplerStateImpl* pPrevSamplerState);  // ���ֻ�ܱ�Graphics����!
    void              SetStageToDevice  (GXUINT Stage, const GXSAMPLERDESC* pPrevSampDesc);
    void              IntUpdateStates   (GXUINT Sampler, const GXSAMPLERDESC* pSamplerDesc, const GXSAMPLERDESC* pDefault);
    //void              SetStateToDevice  (DWORD dwStage, D3DSAMPLERSTATETYPE eType);
  public:
#ifdef ENABLE_VIRTUALIZE_ADDREF_RELEASE
    virtual GXHRESULT AddRef          ();
    virtual GXHRESULT Release         ();
#endif // #ifdef ENABLE_VIRTUALIZE_ADDREF_RELEASE
    virtual GXHRESULT Invoke          (GRESCRIPTDESC* pDesc) { return GX_OK; }
    virtual GXHRESULT SetState        (GXUINT nSamplerSlot, GXSAMPLERDESC* pSamplerDesc);
    virtual GXHRESULT SetStateArray   (GXUINT nStartSlot, GXSAMPLERDESC* pSamplerDesc, int nCount);
    virtual GXHRESULT ResetToDefault  ();
    //GXBOOL            ResetToDefault    ();

    GXBOOL            Set               (GXDWORD Sampler, D3DSAMPLERSTATETYPE eType, GXDWORD dwValue);
    GXDWORD           Get               (GXDWORD Sampler, D3DSAMPLERSTATETYPE eType);
  };
} // namespace D3D9

#endif // _STATE_BLOCK_H_