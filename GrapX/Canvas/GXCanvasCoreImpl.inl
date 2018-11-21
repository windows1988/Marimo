GXCanvasCoreImpl::GXCanvasCoreImpl(GraphicsImpl* pGraphics, GXUINT nPriority, GXDWORD dwType)
  : GXCanvas            (nPriority, dwType)
  , m_pGraphics         (pGraphics)
  , m_pTargetTex        (NULL)
  //, m_xExt              (0)
  //, m_yExt              (0)
  , m_pBlendState       (NULL)
  , m_pSamplerState     (NULL)
  , m_pDepthStencilState(NULL)
  , m_pEffectImpl       (NULL)
  , m_pCamera           (NULL)
{
  InlSetZeroT(m_sExtent);
  AddRef();
}

GXCanvasCoreImpl::~GXCanvasCoreImpl()
{
  SAFE_RELEASE(m_pTargetTex);
  SAFE_RELEASE(m_pBlendState);
  SAFE_RELEASE(m_pSamplerState);
  SAFE_RELEASE(m_pDepthStencilState);
  SAFE_RELEASE(m_pEffectImpl);
  SAFE_RELEASE(m_pCamera);
}

GXBOOL GXCanvasCoreImpl::Initialize(RenderTarget* pTarget)
{
  //if(pTarget == NULL)
  //{
  //  return FALSE;
  //}

  m_pTargetTex = static_cast<RenderTargetImpl*>(pTarget);
  if(m_pTargetTex != NULL)
  {
    m_pTargetTex->AddRef();
    m_pTargetTex->GetDimension(&m_sExtent);
  }
  else
  {
    GXGRAPHICSDEVICE_DESC GraphDeviceDesc;
    m_pGraphics->GetDesc(&GraphDeviceDesc);
    
    m_sExtent.cx = GraphDeviceDesc.BackBufferWidth;
    m_sExtent.cy = GraphDeviceDesc.BackBufferHeight;
  }

  if(m_pSamplerState == NULL) {
    m_pGraphics->CreateSamplerState(reinterpret_cast<GSamplerState**>(&m_pSamplerState));
  }
  else {
    m_pSamplerState->ResetToDefault();
  }

  m_pEffectImpl = (EffectImpl*)m_pGraphics->IntGetEffect();
  m_pEffectImpl->AddRef();

  return TRUE;
}

GXHRESULT GXCanvasCoreImpl::Invoke(GRESCRIPTDESC* pDesc)
{
  return GX_OK;
}

GXSIZE* GXCanvasCoreImpl::GetTargetDimension(GXSIZE* pSize) const
{
  *pSize = m_sExtent;
  return pSize;
}

RenderTarget* GXCanvasCoreImpl::GetTargetUnsafe() const
{
  return m_pTargetTex;
}

#ifdef ENABLE_VIRTUALIZE_ADDREF_RELEASE
GXHRESULT GXCanvasCoreImpl::AddRef()
{
  return gxInterlockedIncrement(&m_nRefCount);
}
#endif // #ifdef ENABLE_VIRTUALIZE_ADDREF_RELEASE
