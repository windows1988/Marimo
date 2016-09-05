//
// Default Rich list adapter
//

namespace GXUI
{
  class CDefRichListAdapter : public IListDataAdapter
  {
  private:
    enum CtrlClass
    {
      CC_None,
      CC_Button,        //#define GXWC_BUTTONW            _T("Button")
      CC_Edit,          //#define GXWC_EDITW              _T("Edit")
      CC_Edit_1_3_30,   //#define GXWC_EDITW_1_3_30       _T("Edit_1_3_30")
      CC_ListBox,       //#define GXWC_LISTBOXW           _T("ListBox")
      CC_ScrollBar,     //#define GXWC_SCROLLBARW         _T("ScrollBar")
      CC_Static,        //#define GXWC_STATICW            _T("Static")
      CC_UIEdit,        //#define GXUICLASSNAME_EDIT      _T("GXUIEdit")   
      CC_UIStatic,      //#define GXUICLASSNAME_STATIC    _T("GXUIStatic")
      CC_UISlider,      //#define GXUICLASSNAME_SLIDER    _T("GXUISlider")
      CC_UIButton,      //#define GXUICLASSNAME_BUTTON    _T("GXUIButton")
      CC_UIList,        //#define GXUICLASSNAME_LIST      _T("GXUIList")
      CC_UIToolbar,     //#define GXUICLASSNAME_TOOLBAR   _T("GXUIToolbar")
    };

    struct ELEMENT
    {
      clStringA strName;
      CtrlClass eClass;
      GXDWORD   dwStyle;
      GXDWORD   dwExStyle;
    };
    typedef clvector<ELEMENT> ElementArray;
  private:
    MODataPool*   m_pDataPool;
    MOVariable    m_DynArray;
    clStringA     m_ArrayName;
    ElementArray  m_aElements;
    int           m_nDefault;   // LB_ADDSTRING ��ϢĬ�����ӵ��ַ���,m_aElements������
    GXINT         m_nItemHeight;

  public:
    CDefRichListAdapter(GXHWND hWnd)
      : GXUI::IListDataAdapter(hWnd)
      , m_pDataPool (NULL)
      , m_nDefault  (-1)
      , m_nItemHeight(10)
    {
    }

    ~CDefRichListAdapter()
    {
      m_pDataPool->Ignore(&m_DynArray, m_hWnd);
      SAFE_RELEASE(m_pDataPool);
    }

    virtual GXHRESULT AddRef()
    {
      GXLONG nRefCount = gxInterlockedIncrement(&m_nRefCount);
      return nRefCount;
    }

    virtual GXHRESULT Release()
    {
      GXLONG nRefCount = gxInterlockedDecrement(&m_nRefCount);
      if(nRefCount == 0) {
        delete this;
        return GX_OK;
      }
      return nRefCount;
    }

    //GXBOOL Initialize(const ItemElementArray& aItemElements, MOVariable* pVariable)
    //{
    //  if( ! pVariable) {
    //    return Initialize(aItemElements);
    //  }
    //  return FALSE;
    //}

    GXBOOL Initialize(const ItemElementArray& aItemElements, MOVariable* pVariable)
    {
      ELEMENT sElement;
      typedef clvector<Marimo::VARIABLE_DECLARATION> VarDeclArray;

      VarDeclArray                  aStructMember;
      Marimo::VARIABLE_DECLARATION  sVarDecl;

      static GXLPCSTR szStringTypeName  = "string";
      static GXLPCSTR szFloatTypeName   = "float";
      static GXLPCSTR szIntegerTypeName = "int";
      static GXLPCSTR szDWordTypeName   = "DWORD";
      static GXLPCSTR szObjectTypeName  = "object";

      clstd::StringSetA VarNameSet;   // ��������VARIABLE_DECLARATION�е��ַ���ʵ��

      aStructMember.reserve(aItemElements.size());

      for(ItemElementArray::const_iterator it = aItemElements.begin(); it != aItemElements.end(); ++it) 
      {
        if(it->strClass != GXUICLASSNAME_STATIC && ! Marimo::DataPool::IsIllegalName(sElement.strName)) {
          continue;
        }

        sElement.strName   = it->strName;
        sElement.eClass    = CC_None;
        sElement.dwStyle   = it->dwStyle;
        sElement.dwExStyle = it->dwExStyle;

        sVarDecl.Count     = 1;
        sVarDecl.Init      = NULL;

        // Wine Button   => string
        // Wine Edit     => string
        // Wine ListBox  => ûʵ��
        // Wine Scroll Bar => <None>
        // GrapX Slider => float/int

        if(it->strClass == GXWE_BUTTONW)
        {
          sElement.eClass    = CC_Button;
          sVarDecl.Type      = szStringTypeName;
          sVarDecl.Name      = VarNameSet.add(sElement.strName);
          aStructMember.push_back(sVarDecl);
        }
        else if(it->strClass == GXWE_EDITW)
        {
          sElement.eClass    = CC_Edit;
          sVarDecl.Type      = szStringTypeName;
          sVarDecl.Name      = VarNameSet.add(sElement.strName);
          aStructMember.push_back(sVarDecl);

          //if(m_strDefault.IsEmpty()) {
          //  m_strDefault = sVarDecl.Name;
          //}
          if(m_nDefault < 0) {
            m_nDefault = (int)m_aElements.size();
          }
        }
        else if(it->strClass == GXWE_EDITW_1_3_30)
        {
          sElement.eClass    = CC_Edit_1_3_30;
          sVarDecl.Type      = szStringTypeName;
          sVarDecl.Name      = VarNameSet.add(sElement.strName);
          aStructMember.push_back(sVarDecl);

          if(m_nDefault < 0) {
            m_nDefault = (int)m_aElements.size();
          }
        }          
        else if(it->strClass == GXWE_LISTBOXW)
        {
          CLBREAK; // ûʵ��!
          //sElement.eClass    = CC_ListBox;
          //sVarDecl.Type      = szStringTypeName;
          //sVarDecl.Name      = sElement.strName;
          //aStructMember.push_back(sVarDecl);
        }
        else if(it->strClass == GXWE_SCROLLBARW)
        {
          //sElement.eClass = CC_ScrollBar;
          sElement.eClass = CC_None;
        }
        else if(it->strClass == GXWE_STATICW)
        {
          CLBREAK; // ûʵ��!
          sElement.eClass = CC_Static;
        }
        else if(it->strClass == GXUICLASSNAME_EDIT)
        {
          sElement.eClass    = CC_UIEdit;
          sVarDecl.Type      = szStringTypeName;
          sVarDecl.Name      = VarNameSet.add(sElement.strName);
          aStructMember.push_back(sVarDecl);

          //if(m_strDefault.IsEmpty()) {
          //  m_strDefault = sVarDecl.Name;
          //}
          if(m_nDefault < 0) {
            m_nDefault = (int)m_aElements.size();
          }
        }
        else if(it->strClass == GXUICLASSNAME_STATIC)
        {
          sElement.eClass    = CC_UIStatic;
          sVarDecl.Name      = VarNameSet.add(sElement.strName);

          switch(it->dwStyle & GXUISS_TYPE_MASK)
          {
          case GXUISS_TYPE_LABEL:
          case GXUISS_TYPE_RECT:
            if( ! Marimo::DataPool::IsIllegalName(sElement.strName)) {
              continue;
            }

            if((it->dwStyle & GXUISS_TYPE_MASK) == GXUISS_TYPE_LABEL) {
              sVarDecl.Type = szStringTypeName;
            }
            else {
              sVarDecl.Type = szDWordTypeName;
            }
            aStructMember.push_back(sVarDecl);

            //if(m_strDefault.IsEmpty()) {
            //  m_strDefault = sVarDecl.Name;
            //}
            if(m_nDefault < 0) {
              m_nDefault = (int)m_aElements.size();
            }
            break;
          case GXUISS_TYPE_SPRITE:
            {
              clStringA strSprite;
              clStringA strUniformIndex;

              // Sprite ���͵�staticӦ���ǡ�uniform_index@sprite_name����ʽ;

              sElement.strName.DivideBy('@', strUniformIndex, strSprite);

              if(Marimo::DataPool::IsIllegalName(strSprite) && 
                Marimo::DataPool::IsIllegalName(strUniformIndex))
              {
                //NameList.push_back(strSprite);
                sVarDecl.Type = szObjectTypeName;
                sVarDecl.Name = VarNameSet.add(strSprite);
                aStructMember.push_back(sVarDecl);

                //NameList.push_back(strUniformIndex);
                sVarDecl.Type = szIntegerTypeName;
                sVarDecl.Name = VarNameSet.add(strUniformIndex);
                aStructMember.push_back(sVarDecl);
              }
            }
            break;
          } // switch
        }
        else if(it->strClass == GXUICLASSNAME_SLIDER)
        {
          sElement.eClass = CC_UISlider;
          if(TEST_FLAG(it->dwStyle, GXUISLDS_FLOAT))
          {
            sVarDecl.Type = szFloatTypeName;
          }
          else
          {
            sVarDecl.Type = szIntegerTypeName;
          }
          sVarDecl.Name = VarNameSet.add(sElement.strName);
          aStructMember.push_back(sVarDecl);
        }
        else if(it->strClass == GXUICLASSNAME_BUTTON)
        {
          sElement.eClass    = CC_UIButton;
          sVarDecl.Type      = szStringTypeName;
          sVarDecl.Name      = VarNameSet.add(sElement.strName);
          aStructMember.push_back(sVarDecl);

          //if(m_strDefault.IsEmpty()) {
          //  m_strDefault = sVarDecl.Name;
          //}
          if(m_nDefault < 0) {
            m_nDefault = (int)m_aElements.size();
          }
        }
        else if(it->strClass == GXUICLASSNAME_LIST)
        {
          CLBREAK; // ûʵ��!
          sElement.eClass = CC_UIList;
        }
        else if(it->strClass == GXUICLASSNAME_TOOLBAR)
        {
          CLBREAK; // ûʵ��!
          sElement.eClass = CC_UIToolbar;
        }

        if(sElement.eClass != CC_None)
        {
          ASSERT(sElement.strName.IsNotEmpty());
          m_aElements.push_back(sElement);
        }
      } // for

      if( ! pVariable)
      {
        // ��β��־
        sVarDecl.Type = NULL;
        sVarDecl.Name = NULL;
        sVarDecl.Count = 0;
        sVarDecl.Init = NULL;
        aStructMember.push_back(sVarDecl);

        static GXLPCSTR szItemType = "ITEM";
        Marimo::TYPE_DECLARATION sStructType[2];
        Marimo::VARIABLE_DECLARATION aGlobalVar[2];

        sStructType[0].Cate       = Marimo::T_STRUCT;
        sStructType[0].Name       = szItemType;
        sStructType[0].as.Struct  = &aStructMember.front();
        sStructType[1].Cate       = Marimo::T_UNDEFINE;
        sStructType[1].Name       = NULL;
        sStructType[1].as.Struct  = NULL;


        aGlobalVar[0].Type = szItemType;
        aGlobalVar[0].Name = "Array";
        aGlobalVar[0].Count = -1;
        aGlobalVar[0].Init = NULL;
        aGlobalVar[1].Type = NULL;
        aGlobalVar[1].Name = NULL;
        aGlobalVar[1].Count = 0;
        aGlobalVar[1].Init = NULL;

        GXHRESULT result = Marimo::DataPool::CreateDataPool(&m_pDataPool, NULL, sStructType, aGlobalVar);

        if(GXSUCCEEDED(result))
        {
          m_ArrayName = "Array";
          m_pDataPool->QueryByName(m_ArrayName, &m_DynArray);
          m_pDataPool->Watch(&m_DynArray, m_hWnd);
        }
      }
      else
      {
        pVariable->GetPool(&m_pDataPool);
        m_ArrayName = pVariable->GetName();
        m_DynArray = *pVariable;
        m_pDataPool->Watch(&m_DynArray, m_hWnd);
      }
      return TRUE;
    }

//#ifdef ENABLE_DATAPOOL_WATCHER
//    virtual GXBOOL IsAutoKnock() const
//    {
//      return m_pDataPool->IsAutoKnock();
//    }
//#endif // #ifdef ENABLE_DATAPOOL_WATCHER

    virtual GXSIZE_T GetCount() const
    {
      if( ! m_DynArray.IsValid() || ! m_pDataPool) {
        return 0;
      }
      return (GXSIZE_T)m_DynArray.GetLength();
    }

    //virtual GXINT GetItemHeight(GXINT nIdx) const
    //{
    //  return -1;
    //}

    //virtual GXINT SetReferenceHeight(GXINT nItemHeight)
    //{
    //  GXINT nPrev = m_nItemHeight;
    //  m_nItemHeight = nItemHeight;
    //  return nPrev;
    //}

    //virtual GXINT GetItemBottoms(GXINT* bottoms, const GXINT* indices, int count) GXCONST
    //{
    //  // �����Ϸ���Ҫ�е����߱�֤,����ֻ��У��
    //  ASSERT(count >= 1);
    //  ASSERT(bottoms && indices);

    //  int i = 0;
    //  do {
    //    bottoms[i] = m_nItemHeight + m_nItemHeight * indices[i];
    //  } while (++i < count);
    //  return i;
    //}

    //virtual GXDWORD GetStatus(GXINT item) GXCONST
    //{
    //  return 0;
    //}

    //virtual GXDWORD SetStatus(GXINT item, GXDWORD dwNewStatus)
    //{
    //  return 0;
    //}

    //virtual GXBOOL IsFixedHeight() const
    //{
    //  return TRUE;
    //}

    virtual GXSIZE_T AddStringW(GXLPCWSTR szName, GXLPCWSTR lpString)
    {
      // FIXME: szName û������
      // ���û��Ĭ�Ͽؼ���ʾLB_ADDSTRING�����ӵ����ݣ����ش���
      //if(m_strDefault.IsEmpty()) {
      //  return GXLB_ERR;
      //}
      if(m_nDefault < 0) {
        return GXLB_ERR;
      }
      ASSERT((int)m_aElements.size() > m_nDefault); // Ĭ��Ԫ�������Ч�Ļ���һ����m_aElements�е�����

      MOVariable varNew = m_DynArray.NewBack();
      MOVariable varDefaultTitle = varNew.MemberOf(m_aElements[m_nDefault].strName);
      varDefaultTitle.Set(lpString);

      int nLength = (int)m_DynArray.GetLength();
      return nLength - 1;
    }

    //GXINT AddSprite(int nSpriteId)
    //{
    //  MOVariable varNew = m_DynArray.NewBack();
    //  int nLength = (int)m_DynArray.GetLength();

    //  MOVariable varDesc     = varNew.GetMember("Desc");
    //  MOVariable varSpriteId = varNew.GetMember("nSpriteId");

    //  clStringA strTemp;
    //  strTemp.Format("%d", nSpriteId);
    //  varDesc.SetAsStringA(strTemp);
    //  //varDesc.SetAsStringA("<noname>");
    //  varSpriteId.SetAsInteger(nSpriteId);

    //  return nLength - 1;
    //}

    virtual MOVariable GetVariable()
    {
      return m_DynArray;
    }

    virtual GXBOOL GetStringW(GETSTRW* pItemStrDesc)
    {
      MOVariable var = m_DynArray.IndexOf(pItemStrDesc->item);
      const ELEMENT& element = m_aElements[pItemStrDesc->element < 0
        ? m_nDefault : pItemStrDesc->element];

      ASSERT(element.strName == pItemStrDesc->name || pItemStrDesc->name == NULL);

      switch(element.eClass)
      {
      case CC_Button:
      case CC_Edit:
      case CC_Edit_1_3_30:
      case CC_UIButton:
      case CC_UIEdit:
        pItemStrDesc->VarString = var.MemberOf(element.strName);
        pItemStrDesc->sString = pItemStrDesc->VarString.ToStringW();
        break;

      case CC_UISlider:
        {
          using namespace Marimo;
          MOVariable varPos = var.MemberOf(element.strName);
          TypeCategory eCate = varPos.GetTypeCategory();
          if(eCate == T_SDWORD || eCate == T_DWORD || eCate == T_WORD || 
            eCate == T_SWORD || eCate == T_BYTE || eCate == T_SBYTE) {
              gxSendMessage(pItemStrDesc->hItemWnd, GXSBM_SETPOS, varPos.ToInteger(), FALSE);
          }
        }
        return FALSE;

      case CC_UIStatic:
        {
          switch(element.dwStyle & GXUISS_TYPE_MASK)
          {
          case GXUISS_TYPE_LABEL:
            {
              MOVariable varString = var.MemberOf(element.strName);
              if(varString.IsValid()) {
                pItemStrDesc->sString = varString.ToStringW();
              }
            }
            break;
          case GXUISS_TYPE_SPRITE:
            {
              clStringA strSprite;
              clStringA strIndex;
              element.strName.DivideBy('@', strIndex, strSprite);

              if(strIndex.IsNotEmpty() && strSprite.IsNotEmpty())
              {
                MOVariable varSprite = var.MemberOf(strSprite);
                MOVariable varIndex = var.MemberOf(strIndex);
                ASSERT(varSprite.IsValid() && varIndex.IsValid());

                if(varSprite.IsValid() && varIndex.IsValid())
                {
                  GXSprite* pSprite = NULL;
                  //varSprite.GetData(&pSprite, sizeof(pSprite)); // TODO: ר��ʵ��"MOVariable object"����,�Զ��������ü���
                  varSprite.Query((GUnknown**)&pSprite);
                  if(pSprite)
                  {
                    gxSendMessage(pItemStrDesc->hItemWnd, GXSSM_SETSPRITE, NULL, (GXLPARAM)pSprite);
                    gxSendMessage(pItemStrDesc->hItemWnd, GXSSM_SETMODULEBYINDEX, varIndex.ToInteger(), NULL);
                    pSprite->Release();
                    pSprite = NULL;
                  }
                }
              }
            }
            return FALSE;

          case CC_ScrollBar:
          case CC_ListBox:
          case CC_Static:
          case CC_UIList:
          case CC_UIToolbar:
          case CC_None:
          default:
            CLBREAK; // ûʵ��
            break;
          }
        }
        break;
      } // switch(element.eClass)

      //pItemStrDesc->sString = var.GetMember("Desc").ToStringA();
      //gxSendMessage(pItemStrDesc->hItemWnd, GXSSM_SETSPRITE, NULL, (GXLPARAM)m_pSprite);
      //int nBlockId = var.GetMember("nSpriteId").GetAsInteger();
      //gxSendMessage(pItemStrDesc->hItemWnd, GXSSM_SETMODULEBYINDEX, nBlockId, NULL);
      return TRUE;
    }

    virtual GXHRESULT GetDataPool(MODataPool** ppDataPool)
    {
      *ppDataPool = m_pDataPool;
      if(m_pDataPool != NULL) {
        return m_pDataPool->AddRef();
      }
      return GX_FAIL;
    }
  }; // CDefCustListAdapter
} // namespace GXUI