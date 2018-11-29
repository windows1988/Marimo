#ifndef _GRAPX_RENDERTARGET_H_
#define _GRAPX_RENDERTARGET_H_

namespace GrapX
{
  class RenderTarget : public GResource
  {
  public:
    RenderTarget() : GResource(0, RESTYPE_RENDERTEXTURE) {}

    GXSTDINTERFACE(GXHRESULT    AddRef            ());
    GXSTDINTERFACE(GXHRESULT    Release           ());

  public:
    GXSTDINTERFACE(GXBOOL     GetRatio              (GXSizeRatio* pWidth, GXSizeRatio* pHeight));
    GXSTDINTERFACE(GXSIZE*    GetDimension          (GXSIZE* pDimension));
    GXSTDINTERFACE(GXHRESULT  GetColorTexture       (Texture** ppColorTexture, GXResUsage eUsage)); // ֻ���� GXResUsage::Default ���� GXResUsage::Read
    GXSTDINTERFACE(Texture*   GetColorTextureUnsafe (GXResUsage eUsage)); // ֻ���� GXResUsage::Default ���� GXResUsage::Read
    GXSTDINTERFACE(GXHRESULT  GetDepthStencilTexture(Texture** ppDepthStencilTexture));
    GXSTDINTERFACE(GXBOOL     StretchRect           (Texture* pSrc, GXLPCRECT lpDest, GXLPCRECT lpSrc, GXTextureFilterType eFilter));
    GXSTDINTERFACE(GXBOOL     SaveToFile            (GXLPCWSTR szFilePath, GXLPCSTR pImageFormat));
    GXSTDINTERFACE(GXBOOL     SaveToMemory          (clstd::MemBuffer* pBuffer, GXLPCSTR pImageFormat));
  };
} // namespace GrapX


#endif // _GRAPX_RENDERTARGET_H_