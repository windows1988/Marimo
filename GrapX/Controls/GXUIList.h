#ifndef _DEV_DISABLE_UI_CODE
#ifndef _GXUI_LIST_H_
#define _GXUI_LIST_H_

#define SCROLLBAR_WIDTH 10
#define CHECKBOX_SIZE 10

namespace DlgXM
{
  struct DLGBASICPARAM;
} // namespace DlgXM

namespace GXUI
{
  class IDataPool;

  //////////////////////////////////////////////////////////////////////////

  class List : public CtrlBase
  {
  public:
    const static GXUINT IDT_SCROLLUP   = 1000;  // 向上滚动动画的Timer
    const static GXUINT IDT_SCROLLDOWN = 1001;  // 向下滚动动画的Timer
    const static GXUINT IDT_WHEELCHECK = 1002;  // 
    
    struct ITEMSTATUS
    {
      GXINT    nBottom;
      GXHWND   hItem;
      GXLPARAM lParam;
      GXDWORD  bSelected : 1;
      GXDWORD  bValidate : 1;  // Item的有效标志, 如果为0表示要重新从Adapter获取
      ITEMSTATUS() : nBottom(-1), hItem(NULL), bSelected(0), bValidate(0){}
    };

    typedef clvector<ITEMSTATUS>  ItemStatusArray;

    //enum ListType
    //{
    //  LT_Simple,
    //  LT_Custom,
    //};
  protected:
    IListDataAdapter* m_pAdapter;
    GXSIZE_T          m_nTopIndex;      // Item索引
    GXINT             m_nScrolled;      // Item滚动过去的像素,稳定时<=0
    GXSIZE_T          m_nExtend;        // Item整体占用的像素，横向滚动时为宽度，纵向滚动时为高度

    GXINT             m_nColumnCount;
    GXINT             m_nColumnWidth;   // 只有simple list才用??
    GXINT             m_nLastSelected;
    GXINT             m_nPrevScrolled;  // 拖动开始时的Scrolled
    GXINT             m_nItemHeight;
    GXCOLORREF        m_crBackground;
    GXCOLORREF        m_crText;
    GXCOLORREF        m_crHightlight;
    GXCOLORREF        m_crHightlightText;
    ItemStatusArray   m_aItems;
    GXDWORD           m_bShowScrollBar : 1;   // 这个滚动条只用于显示, 不响应操作
    GXDWORD           m_bShowButtonBox : 1;   // TODO: 以后改成List的Style
    GXDWORD           m_bRichList      : 1;

  public:
    List(GXLPCWSTR szIdName, GXDWORD bRichList);
  public:
    static GXLRESULT GXCALLBACK WndProc        (GXHWND hWnd, GXUINT message, GXWPARAM wParam, GXLPARAM lParam);
    static List* Create     (GXHINSTANCE hInst, GXHWND hParent, const DlgXM::DLGBASICPARAMW* pDlgParam, const GXDefinitionArrayW* pDefinitions);
    static List* CreateRich (GXHINSTANCE hInst, GXHWND hParent, const DlgXM::DLGBASICPARAMW* pDlgParam, const GXDefinitionArrayW* pDefinitions);


    virtual int       OnCreate            (GXCREATESTRUCT* pCreateParam);
    virtual GXLRESULT Destroy             ();
    virtual GXBOOL    SolveDefinition     (const GXDefinitionArrayW& aDefinitions);
    virtual GXUINT    SetColumnsWidth     (GXLPCWSTR szString);
    virtual GXUINT    SetColumnsWidth     (const GXUINT* pColumns, GXUINT nCount);
    virtual GXUINT    GetColumnsWidth     (GXUINT* pColumns, GXUINT nCount);
    virtual int       OnLButtonDown       (int fwKeys, int x, int y);
    virtual int       OnLButtonUp         (int fwKeys, int x, int y);
    virtual int       OnLButtonDblClk     (int fwKeys, int x, int y);
    virtual int       OnMouseMove         (int fwKeys, int x, int y);
    virtual int       OnMouseWheel        (int fwKeys, int nDelta);
    virtual int       OnSize              (int cx, int cy) GXPURE;
    virtual int       OnTimer             (GXUINT nIDTimer);
    virtual GXVOID    OnImpulse           (LPCDATAIMPULSE pImpulse);
    virtual GXINT     GetCurSel           () const;
    virtual int       SetCurSel           (int nIndex);
    virtual GXBOOL    IsSelected          (GXSIZE_T index) const;
    virtual GXBOOL    SetAdapter          (IListDataAdapter* pAdapter);
    virtual GXBOOL    GetAdapter          (IListDataAdapter** ppAdapter);
    virtual void      SetScrolledVal      (GXINT nScrolled);
    virtual GXINT     GetItemHeight       (GXSIZE_T nIdx) const;
    virtual GXBOOL    EndScroll           ();
    virtual GXBOOL    GetItemRect         (int nItem, GXDWORD dwStyle, GXLPRECT lprc) const GXPURE;
    virtual GXINT     HitTest             (int fwKeys, int x, int y) const GXPURE;
    virtual GXLRESULT SetItemTemplate     (GXLPCWSTR szTemplate) GXPURE;

  protected:
    void        DrawScrollBar       (GXWndCanvas& canvas, LPGXCRECT lprcClient, GXINT nLastBottom, GXSIZE_T count, GXDWORD dwStyle ) const;
    GXBOOL      SelectItem          (GXINT nItem, GXBOOL bSelected, GXBOOL bNotify);
    //GXBOOL      UpdateItemStatus    (GXSIZE_T nBegin, GXSIZE_T nEnd);
    //GXBOOL      SyncItemStatCount   ();
    void        BottomToHeight      (GXDWORD dwStyle, GXSIZE_T begin); // GXLBS_MULTICOLUMN
    void        HeightToBottom      (GXDWORD dwStyle, const GXRECT& rcClient, GXSIZE_T begin); // GXLBS_MULTICOLUMN
    GXINT       DbgCalcColumnCount  ();
    GXBOOL      OnSyncInsert        (GXSIZE_T begin, GXSIZE_T count);
    GXBOOL      OnSyncRemove        (GXSIZE_T begin, GXSIZE_T count);
    //void        DeleteItemStat      (GXINT nIndex);
    //GXBOOL      ReduceItemStat      (GXINT nCount);
    GXBOOL      UpdateTopIndex      (GXDWORD dwStyle);
    GXBOOL      UpdateTopIndex      ();
    GXSIZE_T    AddStringW          (GXLPCWSTR lpString);
    GXSIZE_T    GetStringW          (GXSIZE_T nIndex, clStringW& str);
    GXLRESULT   SetItemData         (GXSIZE_T index, GXLPARAM lParam);
    GXLPARAM    GetItemData         (GXSIZE_T index);
    GXSIZE_T    DeleteString        (GXSIZE_T nIndex);
    GXVOID      ResetContent        ();
    GXSIZE_T    GetCount            () const;
    GXINT       SetItemHeight       (GXINT nNewHeight);
    GXBOOL      IsItemSelected      (GXINT nItem) const;
    GXCOLORREF  SetColor            (GXUINT nType, GXCOLORREF color);
    //GXBOOL      CheckEndScrollAnim  (GXUINT nIDTimer, bool bForced);
    void        UpdateScrollBarRect (GXDWORD dwStyle, LPGXCRECT lprcClient = NULL);
    GXBOOL      IsEmpty             () const;
  };

  //////////////////////////////////////////////////////////////////////////

} // namespace GXUI
#endif // _GXUI_LIST_H_
#endif // #ifndef _DEV_DISABLE_UI_CODE