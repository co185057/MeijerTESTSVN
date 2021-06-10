// dftransView.cpp : implementation of the CDftransView class
//

#include "stdafx.h"
#include "dftrans.h"

#include "dftransDoc.h"
#include "dftransView.h"
#include "LoadingDlg.h"
#include "GotoDialog.h"

#include "TransBroker.h"
#include "tbStateListType.h"
#include "tbStateList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define LV_MY_STYLES     (LVS_REPORT | LVS_OWNERDATA)
#define LV_MY_EXT_STYLES (LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES )


int __cdecl dfCompare(const void *vp1, const void *vp2)
{
  TBSLISTTYPE *p1 = (TBSLISTTYPE*) vp1;
  TBSLISTTYPE *p2 = (TBSLISTTYPE*) vp2;

  if (p1->id < p2->id)
    return -1;
  else if (p1->id == p2->id)
    return 0;
  else
    return 1;
}


/////////////////////////////////////////////////////////////////////////////
// CDftransView

IMPLEMENT_DYNCREATE(CDftransView, CListView)

BEGIN_MESSAGE_MAP(CDftransView, CListView)
	//{{AFX_MSG_MAP(CDftransView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_EDIT_FINDNEXT, OnEditFindnext)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FINDNEXT, OnUpdateEditFindnext)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_COMMAND(ID_EDIT_FINDPREV, OnEditFindprev)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FINDPREV, OnUpdateEditFindprev)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(ID_POPUP_FIND_PREV, OnPopupFindPrev)
	ON_UPDATE_COMMAND_UI(ID_POPUP_FIND_PREV, OnUpdatePopupFindPrev)
	ON_COMMAND(ID_POPUP_FIND_NEXT, OnPopupFindNext)
	ON_UPDATE_COMMAND_UI(ID_POPUP_FIND_NEXT, OnUpdatePopupFindNext)
	ON_COMMAND(ID_POPUP_STATS, OnPopupStats)
	ON_UPDATE_COMMAND_UI(ID_POPUP_STATS, OnUpdatePopupStats)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_COMMAND(ID_EDIT_GOTOINDEX, OnEditGotoindex)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GOTOINDEX, OnUpdateEditGotoindex)
	ON_COMMAND(ID_VIEW_RAW, OnViewRaw)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RAW, OnUpdateViewRaw)
	ON_COMMAND(ID_POPUP_FIND_NEXT_TB, OnPopupFindNextTB)
	ON_COMMAND(ID_POPUP_FIND_PREV_TB, OnPopupFindPrevTB)
	ON_UPDATE_COMMAND_UI(ID_POPUP_FIND_PREV_TB, OnUpdatePopupFindPrevTB)
	ON_UPDATE_COMMAND_UI(ID_POPUP_FIND_NEXT_TB, OnUpdatePopupFindNextTB)
	ON_COMMAND(ID_POPUP_FINDNEXTTYPE_APPLICATIONDEATH, OnPopupFindnexttypeApplicationdeath)
	ON_COMMAND(ID_POPUP_FINDNEXTTYPE_APPLICATIONRESTART, OnPopupFindnexttypeApplicationrestart)
	ON_COMMAND(ID_POPUP_FINDNEXTTYPE_TRANSACTION, OnPopupFindnexttypeTransaction)
	ON_COMMAND(ID_POPUP_FINDPREVTYPE_APPLICATIONDEATH, OnPopupFindprevtypeApplicationdeath)
	ON_COMMAND(ID_POPUP_FINDPREVTYPE_APPLICATIONRESTART, OnPopupFindprevtypeApplicationrestart)
	ON_COMMAND(ID_POPUP_FINDPREVTYPE_TRANSACTIONEND, OnPopupFindprevtypeTransactionend)
	ON_UPDATE_COMMAND_UI(ID_POPUP_FINDNEXTTYPE_APPLICATIONDEATH, OnUpdatePopupFindnexttypeApplicationdeath)
	ON_UPDATE_COMMAND_UI(ID_POPUP_FINDNEXTTYPE_APPLICATIONRESTART, OnUpdatePopupFindnexttypeApplicationrestart)
	ON_UPDATE_COMMAND_UI(ID_POPUP_FINDNEXTTYPE_TRANSACTION, OnUpdatePopupFindnexttypeTransaction)
	ON_UPDATE_COMMAND_UI(ID_POPUP_FINDPREVTYPE_APPLICATIONDEATH, OnUpdatePopupFindprevtypeApplicationdeath)
	ON_UPDATE_COMMAND_UI(ID_POPUP_FINDPREVTYPE_APPLICATIONRESTART, OnUpdatePopupFindprevtypeApplicationrestart)
	ON_UPDATE_COMMAND_UI(ID_POPUP_FINDPREVTYPE_TRANSACTIONEND, OnUpdatePopupFindprevtypeTransactionend)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
  ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDftransView construction/destruction

CDftransView::CDftransView() : clc(GetListCtrl())
{
  qsort(&tbsListArray, tbsListArraySize, sizeof(TBSLISTTYPE), dfCompare);


  m_nMaxTBStateLength = 0;
  int ndx = -1;
  for (int i = 0; i < tbsListArraySize; i++)
  {
    if (strlen(tbsListArray[i].name) > m_nMaxTBStateLength)
    {
      ndx = i;
      m_nMaxTBStateLength = strlen(tbsListArray[i].name);
    }
  }

  ASSERT(ndx != -1);

  if (ndx != -1)
    m_csLongestTBState = tbsListArray[ndx].name;

  memset(&selItem, 0, sizeof(LV_ITEM));
  selItemIndex = -1;
  selTransPtr = NULL;
  m_FindData = "";
  m_FindType = 0;
  m_FindDirection = 1;

}

CDftransView::~CDftransView()
{
}

BOOL CDftransView::PreCreateWindow(CREATESTRUCT& cs)
{
  cs.style |= LV_MY_STYLES;
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDftransView drawing
void CDftransView::OnInitialUpdate()
{
	m_pDoc      = (CDftransDoc *) GetDocument();
  m_pDataList = m_pDoc->GetTransactionList();

  const CString& pathName = m_pDoc->GetPathName();
  if (pathName.GetLength() > 0)
    m_pDoc->SetTitle(pathName);

  LV_COLUMN lv;
  int pixPerChar;
  static BOOL bDoneThisAlready = FALSE;

  pixPerChar = clc.GetStringWidth("w");
  if (!bDoneThisAlready)
  {
    bDoneThisAlready = TRUE;

    // Load up the image list for severities
    m_SeverityImageList.Create(16, 16, ILC_COLOR, DF_NUM_SEV_BMPS, 0);

    CBitmap* pBmp;
  
    pBmp = new CBitmap;
    pBmp->LoadBitmap(IDB_BITMAP_ERROR);
    m_SeverityImageList.Add(pBmp, RGB(0, 255, 0));
  
    pBmp = new CBitmap;
    pBmp->LoadBitmap(IDB_BITMAP_WARNING);
    m_SeverityImageList.Add(pBmp, RGB(0, 255, 0));
  
    pBmp = new CBitmap;
    pBmp->LoadBitmap(IDB_BITMAP_INFO);
    m_SeverityImageList.Add(pBmp, RGB(0, 255, 0));
  
    pBmp = new CBitmap;
    pBmp->LoadBitmap(IDB_BITMAP_AUDITSUCCESS);
    m_SeverityImageList.Add(pBmp, RGB(0, 255, 0));
  
    pBmp = new CBitmap;
    pBmp->LoadBitmap(IDB_BITMAP_AUDITFAILURE);
    m_SeverityImageList.Add(pBmp, RGB(0, 255, 0));
  
    clc.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, LV_MY_EXT_STYLES, LV_MY_EXT_STYLES);
    //clc.SetImageList(&m_SeverityImageList, LVSIL_SMALL);
    //clc.SetImageList(&m_SeverityImageList, LVSIL_STATE);


    lv.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;

    CString colName = "Index";
    lv.fmt = LVCFMT_CENTER;
    lv.cx = pixPerChar * 6;
    lv.pszText = (char *) (const char *) colName;
    clc.InsertColumn(DF_COL_INDEX, &lv);
    
    colName = "Date/Time";
    lv.fmt = LVCFMT_LEFT;
    lv.cx = pixPerChar * 13;
    lv.pszText = (char *) (const char *) colName;
    clc.InsertColumn(DF_COL_TIME, &lv);

    colName = "SCOT State";
    lv.fmt = LVCFMT_LEFT;
    lv.cx = pixPerChar * 25;
    lv.pszText = (char *) (const char *) colName;
    clc.InsertColumn(DF_COL_STATE, &lv);

    colName = "Last TB State";
    lv.fmt = LVCFMT_LEFT;
    lv.cx = (clc.GetDC()->GetTextExtent(m_csLongestTBState)).cx;
    lv.pszText = (char *) (const char *) colName;
    clc.InsertColumn(DF_COL_TB_STATE, &lv);

    colName = "TB State#";
    lv.fmt = LVCFMT_RIGHT;
    lv.cx = pixPerChar * 10;
    lv.pszText = (char *) (const char *) colName;
    clc.InsertColumn(DF_COL_TB_ID, &lv);

#if 0
    colName = "Term#";
    lv.fmt = LVCFMT_CENTER;
    lv.cx = pixPerChar * 6;
    lv.pszText = (char *) (const char *) colName;
    clc.InsertColumn(DF_COL_TERMNUM, &lv);

    colName = "Store#";
    lv.fmt = LVCFMT_CENTER;
    lv.cx = pixPerChar * 6;
    lv.pszText = (char *) (const char *) colName;
    clc.InsertColumn(DF_COL_STORENUM, &lv);
#endif
#if 0
    lv.mask = LVCF_FMT | LVCF_IMAGE | LVCF_WIDTH;

    lv.fmt = LVCFMT_LEFT;
    lv.cx = 40;
    lv.iImage = 0;
    //colName = "Sev";
    //lv.pszText = (char *) (const char *) colName;
    clc.InsertColumn(DF_COL_STORENUM+1, &lv);
#endif
    m_MinPrintWarnItems = ((CDftransApp*) (AfxGetApp()))->m_MinPrintWarnItems;
  }

	CListView::OnInitialUpdate();

  CWaitCursor waitCursor;

	FreeList();

  // set the number of items we will be adding
  clc.SendMessage(LVM_SETITEMCOUNT, m_pDataList->GetSize(), LVSICF_NOINVALIDATEALL);
  theApp.SetListCtrl(&clc);
}

/////////////////////////////////////////////////////////////////////////////
// CDftransView printing

BOOL CDftransView::OnPreparePrinting(CPrintInfo* pInfo)
{
  pInfo->m_pPD->m_pd.nFromPage = 1;

  if (clc.GetSelectedCount() > 0)
  {
    // Enable the Selection button.
    pInfo->m_pPD->m_pd.Flags &= ~PD_NOSELECTION;

    if (clc.GetSelectedCount() > 1)
    {
      // Checked the Selection button.
      pInfo->m_pPD->m_pd.Flags |= PD_SELECTION;	
    }
  }
  
  BOOL rc = DoPreparePrinting(pInfo);

  if (rc && !pInfo->m_bPreview)
  {
    int numItems;

    if (((pInfo->m_pPD->PrintAll())       && ((numItems = clc.GetItemCount())     > m_MinPrintWarnItems)) ||
        ((pInfo->m_pPD->PrintSelection()) && ((numItems = clc.GetSelectedCount()) > m_MinPrintWarnItems)) ||
        ((pInfo->m_bDirect)               && ((numItems = clc.GetItemCount())     > m_MinPrintWarnItems)))
    {
      CString msg;
      msg.Format("About to print %d lines (about %d pages). Is this OK?", 
                  numItems, numItems / 50);

      rc = (BOOL) (AfxMessageBox(msg, MB_OKCANCEL | MB_ICONEXCLAMATION | MB_DEFBUTTON2) == IDOK);
    }
  }

  return rc;
}

void CDftransView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
  m_dfPrintInfo.InitDC(pDC);
  ResetNextSelPrintIndex();
}

void CDftransView::OnEndPrinting(CDC* pDC, CPrintInfo* /*pInfo*/)
{
  m_dfPrintInfo.Cleanup();
}

/////////////////////////////////////////////////////////////////////////////
// CDftransView diagnostics

#ifdef _DEBUG
void CDftransView::AssertValid() const
{
	CListView::AssertValid();
}

void CDftransView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CDftransDoc* CDftransView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDftransDoc)));
	return (CDftransDoc*)m_pDocument;
}
#endif //_DEBUG


void CDftransView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
  OnShowStats();  
  *pResult = 0;
}

void CDftransView::UpdateSelectedItem()
{
  if (clc.GetSelectedCount() == 0)
  {
    selItemIndex = -1;
    selTransPtr = NULL;
    return;
  }

  selItemIndex = clc.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);

  if (selItemIndex == -1)
  {
    selTransPtr = NULL;
    return;
  }

  selItem.mask     = LVIF_STATE;
  selItem.state    = LVIS_SELECTED;
  selItem.iSubItem = 0;
  selItem.iItem    = selItemIndex;

  if (!clc.GetItem(&selItem))
  {
    selItemIndex = -1;
    selTransPtr   = NULL;
    return;
  }

  selTransPtr = (*m_pDataList)[selItemIndex];

  return;
}


void CDftransView::OnEditFind() 
{  
  m_FindDlg.m_FindType      = m_FindType;
  m_FindDlg.m_FindDirection = m_FindDirection;

  if (clc.GetSelectedCount() == 1)
  {
    UpdateSelectedItem();
    if (selTransPtr)
    {
      m_FindDlg.SetDynaframeName(CString(selTransPtr->GetDynaframeName()));
      m_FindDlg.SetDynaframeID(CString(selTransPtr->GetTBStateName()));
      CString tmp;
      tmp.Format("%d", selTransPtr->GetTBState());
      m_FindDlg.SetDynaframeTransNum(tmp);
    }
  }
  else if (!m_FindData.IsEmpty())
  {
    if (m_FindType == DF_FIND_BY_DESC) // Find by name
      m_FindDlg.SetDynaframeName(m_FindData);
    else if (m_FindType == DF_FIND_BY_ID) // Find by ID#
      m_FindDlg.SetDynaframeID(m_FindData);
    else if (m_FindType == DF_FIND_BY_TRANSNUM) // Find by Trans#
      m_FindDlg.SetDynaframeTransNum(m_FindData);
  }

  if (m_FindDlg.DoModal() == IDOK)
  {
    m_FindDirection = m_FindDlg.m_FindDirection;
    m_FindType      = m_FindDlg.m_FindType;

    if (m_FindType == DF_FIND_BY_DESC) // Find by name
      m_FindData = m_FindDlg.m_DynaName;
    else if (m_FindType == DF_FIND_BY_ID) // Find by ID#
      m_FindData = m_FindDlg.m_DynaId;
    else if (m_FindType == DF_FIND_BY_TRANSNUM) // Find by Trans#
      m_FindData = m_FindDlg.m_DynaTransNum;

    (m_FindDirection == DF_FIND_DIR_DOWN) ? OnEditFindnext() : OnEditFindprev();
  }
}


void CDftransView::OnEditFindnext() 
{
  FindNextDynaframe(m_FindType, m_FindDirection = DF_FIND_DIR_DOWN, m_FindData);
}

void CDftransView::OnUpdateEditFindnext(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable((m_FindData != "") && (clc.GetItemCount() != 0) ? TRUE : FALSE);	
}

void CDftransView::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable((clc.GetItemCount() != 0) ? TRUE : FALSE);	
}

void CDftransView::FindNextDynaframe(int findType, int findDirection, CString & findData, int nFindIndex)
{
  CWaitCursor waitCursor;

	m_LastTopVisibleIndex = clc.GetTopIndex();
	m_LastBottomVisibleIndex = m_LastTopVisibleIndex + clc.GetCountPerPage() - 1;

  int lastIndex = clc.GetItemCount() - 1;

  BOOL bWrapNow = FALSE;

  if (nFindIndex == -1)
  {
    if (clc.GetSelectedCount() != 1)
    {
      if (findDirection == DF_FIND_DIR_DOWN)
        m_FindStartIndex = 0;
      else
        m_FindStartIndex = lastIndex;
    }
    else
    {
      m_FindStartIndex = clc.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);

      if ((m_FindStartIndex > 0) && (findDirection == DF_FIND_DIR_UP))
        m_FindStartIndex--;
      else if ((m_FindStartIndex == 0) && (findDirection == DF_FIND_DIR_UP))
        bWrapNow = TRUE;
      else if ((m_FindStartIndex < lastIndex) && (findDirection == DF_FIND_DIR_DOWN))
        m_FindStartIndex++;
      else if ((m_FindStartIndex == lastIndex) && (findDirection == DF_FIND_DIR_DOWN))
        bWrapNow = TRUE;
    }
  }
  else
    m_FindStartIndex = nFindIndex;


  int i = m_FindStartIndex;

  BOOL found = FALSE;
  while (!found  && !bWrapNow)
  {
    if (findType == DF_FIND_BY_DESC)
    {
      found = (BOOL) (ComparePartialNoCase(findData, GetItemText(i, DF_COL_STATE)) == 0);
    }
    else if (findType == DF_FIND_BY_ID)
    {
      found = (BOOL) (ComparePartialNoCase(findData, GetItemText(i, DF_COL_TB_STATE)) == 0);
    }
    else if (findType == DF_FIND_BY_TRANSNUM)
    {
      found = (BOOL) (CompareExact(findData, GetItemText(i, DF_COL_TB_ID)) == 0);
    }
    else if (findType == DF_FIND_BY_INDEX)
    {
      found = (BOOL) (CompareExact(findData, GetItemText(i, DF_COL_INDEX)) == 0);
    }

    if (found)
      break;
    else
    {
      if (findDirection == DF_FIND_DIR_UP)
      {
        if (--i < 0) break;
      }
      else
      {
        if (++i > lastIndex) break;
      }
    }
  }  

  waitCursor.Restore();

  if (found)
  {
    UnselectAllItems();
    SelectItem(i, TRUE);

    if ( clc.GetCountPerPage() <= 3)
      clc.EnsureVisible(i, FALSE);
    else if (i <= m_LastTopVisibleIndex)
      clc.EnsureVisible(max(0, i - 2), FALSE);
    else
      clc.EnsureVisible(min(clc.GetItemCount() - 1, i + 2), FALSE);
  }
  else
  {
    CString msg;
    msg.Format("Item not found - start from %s?", findDirection == DF_FIND_DIR_DOWN ? "beginning" : "end");
    if (AfxMessageBox(msg, MB_YESNO) == IDYES)
    {
      FindNextDynaframe(findType, findDirection, findData, findDirection == DF_FIND_DIR_DOWN ? 0 : lastIndex);
    }
  }
}

int CDftransView::CompareExact(CString & key, CString & data)
{
  return key.Compare(data);
}

int CDftransView::ComparePartialNoCase(CString key, CString data)
{
  key.MakeLower();
  data.MakeLower();

  return (data.Find(key) == -1 ? -1 : 0);
}

void CDftransView::OnEditFindprev() 
{
  FindNextDynaframe(m_FindType, m_FindDirection = DF_FIND_DIR_UP, m_FindData);
}

void CDftransView::OnUpdateEditFindprev(CCmdUI* pCmdUI) 
{
  OnUpdateEditFindnext(pCmdUI);
}

void CDftransView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
    CWnd *mainWnd = AfxGetMainWnd();
    CMenu menu;
    if (!menu.LoadMenu(IDR_POPUP))
        return;
    CMenu* pSubMenu = menu.GetSubMenu(0);
    if (!pSubMenu) 
        return;

    // Display the menu at the current mouse location. There's a "bug"
    // (Microsoft calls it a feature) in Windows 95 that requires calling
    // SetForegroundWindow. To find out more, search for Q135788 in MSDN.
        //
    CPoint mouse;
    GetCursorPos(&mouse);
    ::SetForegroundWindow(mainWnd->GetSafeHwnd());
            
    pSubMenu->TrackPopupMenu(0, mouse.x, mouse.y, mainWnd, NULL);

    ::PostMessage(mainWnd->GetSafeHwnd(), WM_USER, 0, 0);

    *pResult = 0;
}

void CDftransView::OnPopupFindPrev() 
{
  UpdateSelectedItem();
  
  m_FindType      = DF_FIND_BY_DESC;
  m_FindDirection = DF_FIND_DIR_UP;

  if (selTransPtr)
  {
    m_FindData = selTransPtr->GetDynaframeName();
  }
  else
  {
    m_FindData = GetItemText(selItemIndex, DF_COL_STATE);
  }

  FindNextDynaframe(m_FindType, m_FindDirection, m_FindData);
}

void CDftransView::OnUpdatePopupFindPrev(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable((clc.GetSelectedCount() == 1) ? TRUE : FALSE);	
}

void CDftransView::OnPopupFindNext() 
{
  UpdateSelectedItem();
  m_FindDirection = DF_FIND_DIR_DOWN;
  m_FindType      = DF_FIND_BY_DESC;

  if (selTransPtr)
  {
    m_FindData = selTransPtr->GetDynaframeName();
  }
  else
  {
    m_FindData = GetItemText(selItemIndex, DF_COL_STATE);
  }

  FindNextDynaframe(m_FindType, m_FindDirection, m_FindData);
}

void CDftransView::OnUpdatePopupFindNext(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable((clc.GetSelectedCount() == 1) ? TRUE : FALSE);	
}

void CDftransView::OnPopupStats() 
{
  OnShowStats();  
}

void CDftransView::OnUpdatePopupStats(CCmdUI* pCmdUI) 
{
  BOOL canDo = FALSE;

  if (clc.GetSelectedCount() == 1)
  {
    UpdateSelectedItem();
    canDo = (BOOL)(selTransPtr != NULL);
  }

  pCmdUI->Enable(canDo);	
}

void CDftransView::OnShowStats()
{
  if (clc.GetSelectedCount() == 1)
  {
    UpdateSelectedItem(); 
    if (selTransPtr)
      selTransPtr->DisplayDynaframeStats();
  }
}

void CDftransView::UnselectAllItems()
{
  int i = -1;
  do
  {
    i = clc.GetNextItem(i, LVNI_ALL | LVNI_SELECTED);
    if (i != -1)
    {
      SelectItem(i, FALSE);
    }
  }
  while (i != -1);
}


void CDftransView::SelectItem(int index, BOOL bSelected)
{
  LV_ITEM itemInfo;

  itemInfo.mask      = LVIF_STATE;
  itemInfo.iItem     = index;
  itemInfo.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
  itemInfo.state     = bSelected ? LVIS_SELECTED | LVIS_FOCUSED : 0;

  clc.SetItemState(index, &itemInfo);
}


static char EOTDesc[] = "---  End of SCOT Transaction  ---";

void CDftransView::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
  LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
  LVITEM*      pItem     = &(pDispInfo->item);

  if ((pDispInfo->hdr.code == LVN_GETDISPINFO) &&
      (pItem->mask & LVIF_TEXT))
  {
    CDynaTrans*  pTrans;
    
    pTrans = (*m_pDataList)[pItem->iItem];

    if (pTrans->IsSeparator())  // EOT item
    {
      if (pItem->iSubItem == DF_COL_STATE)
      {
        pItem->pszText = (LPTSTR) (LPCTSTR) pTrans->GetDynaframeName();
      }
      else
      {
        strncpy(pItem->pszText, "", pItem->cchTextMax);
      }
    }
    else // Normal item
    {
      switch(pItem->iSubItem)
      {
      case DF_COL_INDEX:
        sprintf(pItem->pszText, "%d", pTrans->GetIndex());
        break;
      case DF_COL_TIME:
        pItem->pszText = (LPTSTR) (LPCTSTR) pTrans->GetDateTime();
        break;
      case DF_COL_STATE:
        pItem->pszText = (LPTSTR) (LPCTSTR) pTrans->GetDynaframeName();
        break;
      case DF_COL_TB_STATE:
        pItem->pszText = (LPTSTR) (LPCTSTR) pTrans->GetTBStateName();
        break;
      case DF_COL_TB_ID:
        sprintf(pItem->pszText, "%d", pTrans->GetTBState());
        break;
#if 0
      case DF_COL_TERMNUM:
        sprintf(pItem->pszText, "%d", pTrans->GetTerminalID());
        break;
      case DF_COL_STORENUM:
        sprintf(pItem->pszText, "%d", pTrans->GetStoreNum());
        break;
      case DF_COL_STORENUM+1:
        strncpy(pItem->pszText, "", pItem->cchTextMax);
        break;
#endif
      }
    }
  }
  else if ((pDispInfo->hdr.code == LVN_GETDISPINFO) &&
      (pItem->mask & LVIF_IMAGE))
  {
    CDynaTrans*  pTrans;
    
    pTrans = (*m_pDataList)[pItem->iItem];

    if (pTrans != NULL)  // EOT item
    {
      pItem->iImage = 1;
    }
  }

	*pResult = 0;
}

void CDftransView::FreeList()
{
  clc.DeleteAllItems();
}

CString CDftransView::GetItemText(int nItem, int nSubItem)
{
  LV_DISPINFO DispInfo;
  memset(&DispInfo, 0, sizeof(LV_DISPINFO));

  LVITEM*      pItem     = &(DispInfo.item);
  TCHAR itemText[256];

  DispInfo.hdr.code   = LVN_GETDISPINFO;
  pItem->mask        |= LVIF_TEXT;
  pItem->iItem        = nItem;
  pItem->iSubItem     = nSubItem;
  pItem->pszText      = itemText;

  LRESULT lResult;

  OnGetdispinfo((NMHDR*) &DispInfo, &lResult);

  if (lResult == 0)
  {
    return (CString) pItem->pszText;
  }
  else
    return (CString) "";

}

void CDftransView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{	
	CListView::OnPrepareDC(pDC, pInfo);
  if (m_dfPrintInfo.IsFirstTime())
    pInfo->m_bContinuePrinting = TRUE;
  else
    pInfo->m_bContinuePrinting = m_dfPrintInfo.ContinuePrinting(pInfo);
}

void CDftransView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	CDftransDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  int tabs1[1] = { 0 };
  int tabsForLines[4] = { 300, 1050, 2350, 4180 };
  int tabsForHeader[4] = { 300, 1050, 2350, 4000 };

  static CTime printTime;

  if (m_dfPrintInfo.IsFirstTime())
  {
    // Initialize print info
    m_dfPrintInfo.Initialize(pDC, pInfo, clc);

    // Get a string with date/time of print job.
    printTime = CTime::GetCurrentTime();
  }

  m_dfPrintInfo.PrepareHeaderDC(pDC);

  CString line;

  line = "\tSCOT State Transition Report - " + GetDocument()->GetTitle();

  int yPos = 0;

  /* Print header */

  if(pInfo->m_rectDraw.Width() > pDC->GetTextExtent(line).cx)
  {
    tabs1[0] = (pInfo->m_rectDraw.Width() - pDC->GetTextExtent(line).cx) / 2;
  }
  else
  {
    tabs1[0] = 0;
  }

  pDC->TabbedTextOut(0, m_dfPrintInfo.GetYPos(yPos), line, 1, tabs1, 0);

  m_dfPrintInfo.PrepareDC(pDC);
  // skip a line
  yPos++;
  yPos++;

  line = "Index\tDate/Time\tSCOT State\tLast TB State\tLast TB State#";
  pDC->TabbedTextOut(0, m_dfPrintInfo.GetYPos(yPos), line, 4, tabsForHeader, 0);

  // skip a line
  yPos++;
  yPos++;

  int maxLines = m_dfPrintInfo.m_DataLinesPerPage;

  int ndx;
  if (m_dfPrintInfo.m_bPrintSelectionOnly)
  {
    int i = 0;
    ndx = GetNextSelPrintIndex(pInfo->m_nCurPage);

    while ((i < maxLines) &&
           ((ndx = clc.GetNextItem(ndx, LVNI_ALL | LVNI_SELECTED)) != -1))
           
    {
      pDC->TabbedTextOut(0, m_dfPrintInfo.GetYPos(yPos), GetPrintLine(ndx), 4, tabsForLines, 0);
      yPos++;
      i++;
    }
    SetLastSelPrintIndex(ndx);
  }
  else
  {
    int startNdx = (pInfo->m_nCurPage - 1) * maxLines;
    int endNdx   = (pInfo->m_nCurPage * maxLines) - 1;
    for (ndx = startNdx; (ndx <= endNdx) && (ndx < m_pDataList->GetSize()); ndx++)
    {
      pDC->TabbedTextOut(0, m_dfPrintInfo.GetYPos(yPos), GetPrintLine(ndx), 4, tabsForLines, 0);
      yPos++;
    }
  }
  // Print footer
  CString pageStr;
  pageStr.Format("Page %d of %d", pInfo->m_nCurPage, m_dfPrintInfo.m_nNumPages);

  tabs1[0] = pInfo->m_rectDraw.Width() - (pDC->GetTextExtent(pageStr).cx + 70);

  line.Format("%s\t%s", printTime.Format("%#c"), pageStr);
  pDC->TabbedTextOut(0, m_dfPrintInfo.GetFooterPos(), line, 1, tabs1, 0);

  m_dfPrintInfo.CleanupDC(pDC);
}


const CString& CDftransView::GetPrintLine(int ndx)
{
  static CString line;
  CString Index, TBStateIdStr;

  CDynaTrans* pTrans = (*m_pDataList)[ndx];
  if (!pTrans->IsSeparator())
  {
    Index.Format("%d",pTrans->GetIndex());
    TBStateIdStr.Format("%d",pTrans->GetTBState());

    line = Index                      + "\t" +
           pTrans->GetDateTime()      + "\t" +
           pTrans->GetDynaframeName() + "\t" +
           pTrans->GetTBStateName()   + "\t" +
           TBStateIdStr;
  }
  else   // It's a descriptor line
  {
    line = CString("\t\t") + CString(pTrans->GetDynaframeName());
  }

  return line;
}


CDfPrintInfo::CDfPrintInfo()
{
  m_pDfFont  = NULL;
  m_pOldFont = NULL;
  m_bFirstTime = TRUE;

}

int CDfPrintInfo::GetYPos(int yPos)
{ 
  return  ((yPos * m_lineHeight) + m_topBottomMargin);
}


int CDfPrintInfo::GetFooterPos()
{ 
  return  (m_PageHeight - m_topBottomMargin - m_lineHeight);
}

#define NUM_HEADER_FOOTER_LINES  6

void CDfPrintInfo::Initialize(CDC * pDC, CPrintInfo* pInfo, CListCtrl& clc)
{
  int numSelected    = clc.GetSelectedCount();

  // Compute # of lines that fit on a page.

  m_PageHeight  = pInfo->m_rectDraw.Height();

  m_MaxLinesPerPage  = (m_PageHeight - (m_topBottomMargin * 2)) / m_lineHeight;
  m_DataLinesPerPage = m_MaxLinesPerPage - NUM_HEADER_FOOTER_LINES;

  if (pInfo->m_pPD->PrintSelection())
  {
    m_nNumPages = numSelected / m_DataLinesPerPage;
    if (numSelected % m_DataLinesPerPage)
      m_nNumPages++;
    m_bPrintSelectionOnly = TRUE;
    m_nStartPage = 1;
    m_nEndPage   = m_nNumPages;
  }
  else if (pInfo->m_pPD->PrintRange())
  {
    m_nStartPage = pInfo->GetFromPage();
    m_nEndPage   = pInfo->GetToPage();
    m_bPrintSelectionOnly = FALSE;

    m_nNumPages = pInfo->GetToPage() - pInfo->GetFromPage() + 1;
  }
  else // Print all
  {
    m_nStartPage = 1;
    m_nNumPages = clc.GetItemCount() / m_DataLinesPerPage;
    if (clc.GetItemCount() % m_DataLinesPerPage)
      m_nNumPages++;

    m_nEndPage = m_nNumPages; 
    m_bPrintSelectionOnly = FALSE;
  }

  // set m_bFirstTime FALSE since once this has been called, we are no longer on
  // the first time through OnPrint

  m_bFirstTime = FALSE;
}

void CDfPrintInfo::PrepareDC(CDC * pDC)
{
  pDC->SelectObject(m_pDfFont);
}


void CDfPrintInfo::PrepareHeaderDC(CDC * pDC)
{
  m_pOldFont = pDC->SelectObject(m_pDfHeaderFont);
}


void CDfPrintInfo::CleanupDC(CDC * pDC)
{
  pDC->SelectObject(m_pOldFont);
}

void CDfPrintInfo::Cleanup()
{
  if (m_pDfFont)
  {
    m_pDfFont->DeleteObject();
    delete m_pDfFont;
  }
  if (m_pDfHeaderFont)
  {
    m_pDfHeaderFont->DeleteObject();
    delete m_pDfHeaderFont;
  }

  m_bFirstTime = TRUE;
}


BOOL CDfPrintInfo::ContinuePrinting(CPrintInfo * pInfo)
{
  return (BOOL) (pInfo->m_nCurPage <= m_nEndPage);
}

void CDfPrintInfo::InitDC(CDC * pDC)
{
  UINT pixelsPerInch = pDC->GetDeviceCaps(LOGPIXELSY);

  m_pDfFont = new CFont;
  m_pDfFont->CreateFont(pixelsPerInch * 10L / 72L, 0, 0, 0, FW_NORMAL,
                                 FALSE, FALSE, FALSE, ANSI_CHARSET,
                                 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                 PROOF_QUALITY, VARIABLE_PITCH | FF_SWISS, NULL);

  m_pDfHeaderFont = new CFont;
  m_pDfHeaderFont->CreateFont(pixelsPerInch * 10L / 60L, 0, 0, 0, FW_BOLD,
                                 FALSE, FALSE, FALSE, ANSI_CHARSET,
                                 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                 PROOF_QUALITY, VARIABLE_PITCH | FF_SWISS, NULL);

  pDC->SetBkMode(TRANSPARENT);

  m_lineHeight  = pDC->GetDeviceCaps(LOGPIXELSY) * 10 / 72;
  m_lineHeight += pDC->GetDeviceCaps(LOGPIXELSY) / 30;

  m_topBottomMargin = pDC->GetDeviceCaps(LOGPIXELSY) / 4;

  // set m_bFirstTime TRUE since this gets called only at beginning of print job.
}

BOOL CDfPrintInfo::IsFirstTime()
{
  return m_bFirstTime;
}


static int lastPage  = INT_MIN;
static int lastIndex = INT_MIN;
static int nextIndex = INT_MIN;

int CDftransView::GetNextSelPrintIndex(int curPage)
{
  int ndx;

  if ((curPage == lastPage) && (lastIndex != INT_MIN))
  {
    ndx = lastIndex;
  }
  else if (curPage == 1)
  {
    ndx = lastIndex = -1; // Always start first page of selection print on -1 (1st item)
  }
  else if ((curPage == (lastPage + 1)) && (lastIndex != INT_MIN))
  {
    lastIndex = ndx = nextIndex;
  }
  else // must compute, can't use any cached values
  {
    int i = 0;
    ndx = -1;
    do
    {
      ndx = clc.GetNextItem(ndx, LVNI_ALL | LVNI_SELECTED);
      i++;
    } while ((i < ((curPage - 1) * m_dfPrintInfo.m_DataLinesPerPage)) && (ndx != -1));

    // If ndx comes out of this loop as -1, then there are no selected items, or
    // we hit the end of the list w/o finding the index we expect.  If that is the
    // case, then return INT_MIN as a failure value.

    if (ndx == -1)
      ndx = INT_MIN;
    
    lastIndex = ndx;
  }

  lastPage = curPage;

  return ndx;
}

void CDftransView::SetLastSelPrintIndex(int ndx)
{
  nextIndex = ndx;
}

void CDftransView::ResetNextSelPrintIndex()
{
  lastPage  = INT_MIN;
  lastIndex = INT_MIN;
  nextIndex = INT_MIN;
}

void CDftransView::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable((clc.GetItemCount() != 0) ? TRUE : FALSE);	
}

void CDftransView::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable((clc.GetItemCount() != 0) ? TRUE : FALSE);	
}

void CDftransView::OnEditGotoindex() 
{
  CGotoDialog gotoDlg;

  if (IDOK == gotoDlg.DoModal())
  {
    UnselectAllItems();
    SelectItem(atoi(gotoDlg.m_GotoIndex) - 2, TRUE);
    FindNextDynaframe(DF_FIND_BY_INDEX, DF_FIND_DIR_DOWN, gotoDlg.m_GotoIndex);
  }
}

void CDftransView::OnUpdateEditGotoindex(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable((clc.GetItemCount() != 0) ? TRUE : FALSE);	
}

void CDftransView::OnViewRaw() 
{
	((CDftransApp*)AfxGetApp())->LaunchDefaultViewer(m_pDoc->GetScreenTransName());
	
}

void CDftransView::OnUpdateViewRaw(CCmdUI* pCmdUI) 
{
  BOOL bEmpty = m_pDoc->GetScreenTransName().IsEmpty();
  pCmdUI->Enable(!bEmpty);
}

void CDftransView::OnFileRefresh()
{
  clc.EnsureVisible(clc.GetItemCount() - 1, FALSE);
}

void CDftransView::OnPopupFindNextTB() 
{
  UpdateSelectedItem();
  m_FindDirection = DF_FIND_DIR_DOWN;
  m_FindType      = DF_FIND_BY_ID;

  if (selTransPtr)
  {
    m_FindData = selTransPtr->GetTBStateName();
  }
  else
  {
    m_FindData = GetItemText(selItemIndex, DF_COL_TB_STATE);
  }

  FindNextDynaframe(m_FindType, m_FindDirection, m_FindData);
}

void CDftransView::OnPopupFindPrevTB() 
{
  UpdateSelectedItem();
  m_FindDirection = DF_FIND_DIR_UP;
  m_FindType      = DF_FIND_BY_ID;

  if (selTransPtr)
  {
    m_FindData = selTransPtr->GetTBStateName();
  }
  else
  {
    m_FindData = GetItemText(selItemIndex, DF_COL_TB_STATE);
  }

  FindNextDynaframe(m_FindType, m_FindDirection, m_FindData);
}

void CDftransView::OnUpdatePopupFindPrevTB(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable((clc.GetSelectedCount() == 1) ? TRUE : FALSE);	
}

void CDftransView::OnUpdatePopupFindNextTB(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable((clc.GetSelectedCount() == 1) ? TRUE : FALSE);	
}

void CDftransView::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
  //for this notification, the structure is actually a
  // NMLVCUSTOMDRAW that tells you what's going on with the custom
  // draw action. So, we'll need to cast the generic pNMHDR pointer.
  LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
  int iRow, iSubItem;

  switch(lplvcd->nmcd.dwDrawStage)
  {

    case CDDS_PREPAINT:
      *pResult = CDRF_NOTIFYITEMDRAW;          // ask for item notifications.
      break;

    case CDDS_ITEMPREPAINT:
      *pResult = CDRF_NOTIFYSUBITEMDRAW;
      break;

    case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
      *pResult = CDRF_DODEFAULT;

      iRow     = lplvcd->nmcd.dwItemSpec;
      iSubItem = lplvcd->iSubItem;
      if (iSubItem == 2)
      {
        CString csText = clc.GetItemText(iRow, iSubItem);
        if(csText.Compare(theApp.m_csAppDeathText) == 0)
        {
          lplvcd->clrText   = theApp.m_AppDeathColor;
          *pResult = CDRF_NEWFONT;
        }
        else if(csText.Compare(theApp.m_csTransEndText) == 0)
        {
          lplvcd->clrText   = theApp.m_TransEndColor;
          *pResult = CDRF_NEWFONT;
        }
        else if(csText.Compare(theApp.m_csAppRestartText) == 0)
        {
          lplvcd->clrText   = theApp.m_AppRestartColor;
          *pResult = CDRF_NEWFONT;
        }
        else if(csText.Compare(theApp.m_csLaneClosedText) == 0)
        {
          lplvcd->clrText   = theApp.m_LaneClosedColor;
          *pResult = CDRF_NEWFONT;
        }
      }
      break;

    default:
      *pResult = CDRF_DODEFAULT;
  }
}

void CDftransView::OnPopupFindnexttypeApplicationdeath() 
{
  UpdateSelectedItem();
  m_FindDirection = DF_FIND_DIR_DOWN;
  m_FindType      = DF_FIND_BY_DESC;
  m_FindData      = theApp.m_csAppDeathText;

  FindNextDynaframe(m_FindType, m_FindDirection, m_FindData);
}


void CDftransView::OnPopupFindnexttypeApplicationrestart() 
{
  UpdateSelectedItem();
  m_FindDirection = DF_FIND_DIR_DOWN;
  m_FindType      = DF_FIND_BY_DESC;
  m_FindData      = theApp.m_csAppRestartText;

  FindNextDynaframe(m_FindType, m_FindDirection, m_FindData);
}


void CDftransView::OnPopupFindnexttypeTransaction() 
{
  UpdateSelectedItem();
  m_FindDirection = DF_FIND_DIR_DOWN;
  m_FindType      = DF_FIND_BY_DESC;
  m_FindData      = theApp.m_csTransEndText;

  FindNextDynaframe(m_FindType, m_FindDirection, m_FindData);
}


void CDftransView::OnPopupFindprevtypeApplicationdeath() 
{
  UpdateSelectedItem();
  m_FindDirection = DF_FIND_DIR_UP;
  m_FindType      = DF_FIND_BY_DESC;
  m_FindData      = theApp.m_csAppDeathText;

  FindNextDynaframe(m_FindType, m_FindDirection, m_FindData);
}


void CDftransView::OnPopupFindprevtypeApplicationrestart() 
{
  UpdateSelectedItem();
  m_FindDirection = DF_FIND_DIR_UP;
  m_FindType      = DF_FIND_BY_DESC;
  m_FindData      = theApp.m_csAppRestartText;

  FindNextDynaframe(m_FindType, m_FindDirection, m_FindData);
}


void CDftransView::OnPopupFindprevtypeTransactionend() 
{
  UpdateSelectedItem();
  m_FindDirection = DF_FIND_DIR_UP;
  m_FindType      = DF_FIND_BY_DESC;
  m_FindData      = theApp.m_csTransEndText;

  FindNextDynaframe(m_FindType, m_FindDirection, m_FindData);
}


void CDftransView::OnUpdatePopupFindnexttypeApplicationdeath(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable((clc.GetItemCount() != 0) ? TRUE : FALSE);	
}

void CDftransView::OnUpdatePopupFindnexttypeApplicationrestart(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable((clc.GetItemCount() != 0) ? TRUE : FALSE);	
}

void CDftransView::OnUpdatePopupFindnexttypeTransaction(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable((clc.GetItemCount() != 0) ? TRUE : FALSE);	
}

void CDftransView::OnUpdatePopupFindprevtypeApplicationdeath(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable((clc.GetItemCount() != 0) ? TRUE : FALSE);	
}

void CDftransView::OnUpdatePopupFindprevtypeApplicationrestart(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable((clc.GetItemCount() != 0) ? TRUE : FALSE);	
}

void CDftransView::OnUpdatePopupFindprevtypeTransactionend(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable((clc.GetItemCount() != 0) ? TRUE : FALSE);	
}
