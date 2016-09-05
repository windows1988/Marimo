#ifndef _GRAPHX_PRIMITIVE_H_
#define _GRAPHX_PRIMITIVE_H_

// <��ʱע��>
// 1.ʹ���� Primitive ������� ��Ϊ�˱�����Ժ��Stream����
//    ��Ϊ Stream ���ܻ�� Graphics3D ��������

//////////////////////////////////////////////////////////////////////////
// TODO: ׷�����ݽ�����������һ��Collector��
// ���׷�����ݻ��������,�򷵻� FALSE
//GXBOOL  Append  (GXLPVOID lpPrimitiveData, GXUINT uElementCount);
//GXBOOL  Append  (GXLPVOID lpPrimitiveData, GXUINT uElementCount, WORD* pIndices, GXUINT uIndexCount);
//GXBOOL  Reset  ();
// </��ʱע��>

class GVertexDeclaration;

class GPrimitive : public GResource
{
public:
  enum ResEnum
  {
    ResourceIndices,
    ResourceVertices,
    ResourceAll,
  };

public:
  //enum Type
  //{
  //  VertexOnly = 0,
  //  Indexed = 1,
  //};
  GPrimitive(GXUINT nPriority, GXDWORD dwType) : GResource(nPriority, dwType){}
  GXSTDINTERFACE(GXHRESULT  AddRef  ());
  //GXSTDINTERFACE(Type       GetType ());

  GXSTDINTERFACE(GXLPVOID   GetVerticesBuffer ());
  GXSTDINTERFACE(GXUINT     GetVerticesCount  ());
  GXSTDINTERFACE(GXUINT     GetVertexStride   ());
  GXSTDINTERFACE(GXBOOL     UpdateResouce     (ResEnum eRes));  // ���ڴ����ݸ��µ��豸��
  GXSTDINTERFACE(GXHRESULT  GetVertexDeclaration(GVertexDeclaration** ppDeclaration));
  GXSTDINTERFACE(GXGraphics*GetGraphicsUnsafe ());
  GXSTDINTERFACE(GXINT      GetElementOffset  (GXDeclUsage Usage, GXUINT UsageIndex, LPGXVERTEXELEMENT lpDesc = NULL));
};

//
// Primitive - Vertex 
//
class GPrimitiveV : public GPrimitive
{
public:
  GPrimitiveV() : GPrimitive(0, RESTYPE_PRIMITIVE){}

  GXSTDINTERFACE(GXHRESULT AddRef        ());
  GXSTDINTERFACE(GXHRESULT Release       ());

  GXSTDINTERFACE(GXBOOL    EnableDiscard (GXBOOL bDiscard));
  GXSTDINTERFACE(GXBOOL    IsDiscardable ());
  GXSTDINTERFACE(GXLPVOID  Lock          (GXUINT uElementOffsetToLock, GXUINT uElementCountToLock, 
    GXDWORD dwFlags = (GXLOCK_DISCARD | GXLOCK_NOOVERWRITE)));
  GXSTDINTERFACE(GXBOOL    Unlock        ());
};

//
// Primitive - Vertex & Index
//
class GPrimitiveVI : public GPrimitive
{
public:
  GPrimitiveVI() : GPrimitive(0, RESTYPE_INDEXED_PRIMITIVE){}

  GXSTDINTERFACE(GXHRESULT AddRef        ());
  GXSTDINTERFACE(GXHRESULT Release       ());

  GXSTDINTERFACE(GXBOOL    EnableDiscard (GXBOOL bDiscard));
  GXSTDINTERFACE(GXBOOL    IsDiscardable ());
  GXSTDINTERFACE(GXBOOL    Lock          (GXUINT uElementOffsetToLock, GXUINT uElementCountToLock, 
    GXUINT uIndexOffsetToLock, GXUINT uIndexLengthToLock,
    GXLPVOID* ppVertexData, GXWORD** ppIndexData,
    GXDWORD dwFlags = (GXLOCK_DISCARD | GXLOCK_NOOVERWRITE)));
  GXSTDINTERFACE(GXBOOL    Unlock        ());
  GXSTDINTERFACE(GXUINT    GetIndexCount ());

  GXSTDINTERFACE(GXLPVOID  GetIndicesBuffer());
};
//
// EnableDiscard()
// ȷ���Ƿ������LostDeviceʱ����, Ĭ��ΪTRUE
//
#endif // _GRAPHX_PRIMITIVE_H_