﻿#ifndef _CLSTD_IMAGE_H_
#define _CLSTD_IMAGE_H_

// 这个是clstd下的设备无关Image处理类
// Image数据完全在内存中，不与显卡等硬件设备关联，处理操作也依赖于cpu进行

namespace clstd
{
  // 通道含义：
  // R = 红色
  // G = 绿色
  // B = 蓝色
  // A = Alpha
  // X = 占位
  // 未来考虑支持'Y','U','V'通道

  struct IMAGEDESC
  {
    void* ptr;        // 数据地址
    int   width;      // 像素宽度
    int   height;     // 像素高度
    int   channel;    // 通道数, 1, 2, 3, 4
    int   pitch;      // 一行像素占的字节长度
    int   depth;      // 通道位数，8bit/16bit/32bits/64bits
    union {
      u8  name[4];    // 通道格式 'R','G','B','A','X'
      u32 code;
    }format;
  };

  enum ImageFilter
  {
    ImageFilter_Nearest = 0,
  };

  class Image
  {
    // 这里面包含了基本数据结构和方法，所有成员方法应当与数据类型无关
  protected:
    CLLPBYTE m_ptr;   // 数据地址
    int m_width;      // 像素宽度
    int m_height;     // 像素高度
    int m_channel;    // 通道数, 1, 2, 3, 4
    int m_pitch;      // 一行像素占的字节长度
    int m_depth;      // 通道位数，8bit/16bit/32bits
    union
    {
      u8  name[4];    // 通道格式 'R','G','B','A','X'
      u32 code;
    }m_format;

    const static int s_nMaxChannel = 4;

    static b32 IntParseFormat(const char* fmt, u32* pFmtCode, int* pChannel);

  public:
    Image();
    virtual ~Image();

  public:
    b32       CompareFormat (const char* fmt) const;
    b32       Set           (int nWidth, int nHeight, const char* fmt, const void* pData);
    b32       Set           (int nWidth, int nHeight, const char* fmt, int nPitch, int nDepth, const void* pData);
    int       GetWidth      () const;
    int       GetHeight     () const;
/*没实现*/int       Inflate       (int left, int top, int right, int bottom); // 调整Image尺寸，参数是四个边缘扩展的像素数，可以是负数
    const void* GetPixelPtr   (int x, int y) const;
    void*       GetPixelPtr   (int x, int y);
    const void* GetLine       (int y) const;
    void*       GetLine       (int y);
    int       GetChannelOffset(char chChannel);   // 通道在像素中的偏移量
    b32       GetChannelPlane (Image* pDestImage, char chChannel); // 获得通道平面，pDest将被清空
    b32       ScaleNearest    (Image* pDestImage, int nWidth, int nHeight); // 点采样缩放，这个不需要计算像素
    const char* GetFormat     () const;
  private:
    template<typename _Ty>
    void IntCopyChannel( Image* pDestImage, int nOffset, const int nPixelSize );

    template<typename _Ty>
    void IntStretchCopy( Image* pDestImage, int nWidth, int nHeight );

    template<typename _Ty>
    void IntStretchCopyMulti( Image* pDestImage, int nWidth, int nHeight, int nCount );

    //template<typename _Ty>
    //void IntCopyChannel( Image* pDestImage, int nOffset, const int nPixelSize );

  public:
    static void BlockTransfer(IMAGEDESC* pDest, int x, int y, int nCopyWidth, int nCopyHeight, const IMAGEDESC* pSrc);
  };

  //
  // Filter 还是 Sampler
  //
  class ImageFilterI8 : public Image
  {
  public:
    b32 SetAsInteger(Image* pSrcImage); // 按照整数像素设置
  protected:
  private:
  };

  class ImageFilterI16 : public Image
  {
  public:
    b32 SetAsInteger(Image* pSrcImage);
  protected:
  private:
  };

  class ImageFilterI32 : public Image
  {
  public:
    b32 SetAsInteger(Image* pSrcImage);
  protected:
  private:
  };

  class ImageFilterF : public Image
  {
  public:
    b32 Set(ImageFilterI8* pSrcImage, float fLevel = (1.0f / 255.0f));
    b32 Set(ImageFilterI16* pSrcImage, float fLevel = (1.0f / 65535.0f));
    b32 Set(ImageFilterI32* pSrcImage, float fLevel = (1.0f / 255.0f));
    b32 Set (int nWidth, int nHeight, const char* fmt, const void* pData);
    b32 Clear(float value, char chChannel);
  };

  // ImageFilterXXX只是Image的方法扩展，不做数据记录
  STATIC_ASSERT(sizeof(Image) == sizeof(ImageFilterF));
  STATIC_ASSERT(sizeof(Image) == sizeof(ImageFilterI8));
  STATIC_ASSERT(sizeof(Image) == sizeof(ImageFilterI16));
  STATIC_ASSERT(sizeof(Image) == sizeof(ImageFilterI32));
} // namespace clstd

#endif // _CLSTD_IMAGE_H_