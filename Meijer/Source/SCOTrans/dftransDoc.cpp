// dftransDoc.cpp : implementation of the CDftransDoc class
//

#include "stdafx.h"
#include "dftrans.h"
#include "LoadingDlg.h"
#include "dftransDoc.h"
#include "dftransView.h"
#include "tbStateListType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define DF_LINE_LENGTH 27

/////////////////////////////////////////////////////////////////////////////
// CDftransDoc

IMPLEMENT_DYNCREATE(CDftransDoc, CDocument)

BEGIN_MESSAGE_MAP(CDftransDoc, CDocument)
	//{{AFX_MSG_MAP(CDftransDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDftransDoc construction/destruction

CDftransDoc::CDftransDoc()
{
  m_FileLength = -1;
}

CDftransDoc::~CDftransDoc()
{
  FreeDataList();
}


/////////////////////////////////////////////////////////////////////////////
// CDftransDoc serialization

void CDftransDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
    CDynaTrans *pTrans, *pPreviousTrans;
    BOOL bStartTransaction = FALSE;

    int approxNumLines = m_FileLength / DF_LINE_LENGTH;

    CLoadingDlg dlg;
    dlg.m_MsgTxt = "Please wait while SCOT State Transition Data is loaded...";
    dlg.Create(IDD_DFLISTLOAD, NULL);
    dlg.ShowWindow(SW_SHOWNORMAL);

    dlg.InitProgress(approxNumLines);

    // First, initialize the data list;
    POSITION pos = GetFirstViewPosition();
    CDftransView* pView = (CDftransView*) GetNextView(pos);
    pView->FreeList();
    FreeDataList();


    // Next, zero all counters in dynaframe stats array.

    int i,j;
    TBSLISTTYPE *p;

    for (i=0; i < tbsListArraySize; i++)
    {
      p = &(tbsListArray[i]);
      p->numTimes  = 0;
      p->totalTime = 0;
      p->minTime   = 0;
      p->maxTime   = 0;
    }


    m_DataList.RemoveAll();
    m_DataList.SetSize(approxNumLines);

    // Now read it in again
    CString line;
    i = 0;
    j = 1; // j is 1-based index passed to Initialize().
    BOOL bLastTransWasNull = FALSE;

    while (ar.ReadString(line))
    {
      if (i == 0)
      {
        pPreviousTrans = NULL;
      }
      else if (bLastTransWasNull)
      {
        pPreviousTrans = m_DataList[i-2];
      }
      else
      {
        pPreviousTrans = m_DataList[i-1];
      }

      pTrans = new CDynaTrans;
      if (pTrans->Initialize(j, line, pPreviousTrans))
      {
        m_DataList.SetAtGrow(i++, pTrans);
        if (pTrans->IsEndOfTransaction())
        {
          CDynaTrans* pTmp = new CDynaTrans;
          pTmp->InitSeparator(theApp.m_csTransEndText);
          m_DataList.SetAtGrow(i++, pTmp);
          bLastTransWasNull = TRUE;
        }
        else if (pTrans->IsLoadState())
        {
          CDynaTrans* pTmp = new CDynaTrans;
          if (pPreviousTrans && !pPreviousTrans->IsShutdownState())
            pTmp->InitSeparator(theApp.m_csAppDeathText);
          else
            pTmp->InitSeparator(theApp.m_csAppRestartText);
          m_DataList.SetAtGrow(i++, pTmp);
          bLastTransWasNull = TRUE;
        }
        else if (pTrans->IsLaneClosed())
        {
          CDynaTrans* pTmp = new CDynaTrans;
          pTmp->InitSeparator(theApp.m_csLaneClosedText);
          m_DataList.SetAtGrow(i++, pTmp);
          bLastTransWasNull = TRUE;
        }
        else
          bLastTransWasNull = FALSE;

        j++;
      }
      else
      {
        delete pTrans;
        if (i == 0) // first line of file is bad
        {
          dlg.DestroyWindow();
          AfxThrowArchiveException(CArchiveException::badIndex, ar.m_pDocument->GetPathName());
        }
      }

      dlg.StepIt(i);
    }

    m_DataList.SetSize(i);
    m_DataList.FreeExtra();
    dlg.DestroyWindow();
  }
}

/////////////////////////////////////////////////////////////////////////////
// CDftransDoc diagnostics

#ifdef _DEBUG
void CDftransDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDftransDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDftransDoc commands

void CDftransDoc::FreeDataList()
{
  CDynaTrans *pTrans;

  int i, size = m_DataList.GetUpperBound();

  for (i = 0; i <= size; i++)
  {
    pTrans = m_DataList[i];
    if (pTrans)
    {
      delete pTrans;
      pTrans = NULL;
    }
  }

  m_DataList.RemoveAll();
}

CArray <CDynaTrans*, CDynaTrans*> *CDftransDoc::GetTransactionList()
{
  return &m_DataList;
}


BOOL CDftransDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  // This override is a copy of CDocument::OnOpenDocument, instead of calling
  // the base class.  This is because I want to change the open mode from the
  // base class implementation, and there is no way to set it nicely in MFC.

	if (IsModified())
		TRACE0("Warning: OnOpenDocument replaces an unsaved document.\n");

	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
//		CFile::modeRead|CFile::shareDenyWrite, &fe); // original CDocument line
	    CFile::modeRead|CFile::shareDenyNone, &fe);
	if (pFile == NULL)
	{
		ReportSaveLoadException(lpszPathName, &fe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}

  m_FileLength = pFile->GetLength();

	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;
   
	//+TAR247967 - changing all upper case MFC Macro TRY/CATCH to C++ try/catch
	try
	{
		CWaitCursor wait;
		if (pFile->GetLength() != 0)
			Serialize(loadArchive);     // load me
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	catch(CFileException* e)
	{
		ReleaseFile(pFile, TRUE);
		DeleteContents();   // remove failed contents
		
		ReportSaveLoadException(lpszPathName, e,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		e->Delete();
		return FALSE;
	}
	catch(CArchiveException* e)
	{
		ReleaseFile(pFile, TRUE);
		DeleteContents();   // remove failed contents
		
		ReportSaveLoadException(lpszPathName, e,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		e->Delete();
		return FALSE;
	}

    //-TAR247967

	SetModifiedFlag(FALSE);     // start off with unmodified

  return TRUE;
}


CString CDftransDoc::GetScreenTransName()
{
  return GetPathName();
}
