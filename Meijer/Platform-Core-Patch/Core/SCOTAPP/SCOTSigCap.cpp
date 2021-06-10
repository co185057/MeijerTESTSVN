// SCOTSigCap.cpp: implementation of the CSCOTSigCap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "opos.h"
#include <malloc.h>


// Include stuff needed to do 5991 Compressed format.
// Since no one really understands this stuff, we will just use
// as much provided code as possible.

extern "C" 
{
  #include "Comp.c"
}

#include "Common.h"                 // common includes
#include "SCOTSigCap.h"
#include "DMProcedures.h"           // DM procedures

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define COMP_ID ID_OD
#define T_ID    _T("SCOT SigCap")

#define SIG_VALID_PTS_THRESHOLD   10 // This is what DM uses currently also

// Compression Code

char  compress_alg = 0x01;      /*  defines compression algorithm  */

typedef struct 
{
  BYTE b05;      // Always 0x05
  BYTE bVli;     // Determined at the end
  BYTE bFc;      // Function Code
  BYTE bStatus;  // 0x80, 0x00 or error
  BYTE bSeq;     // Sequence Number
} MSG_HEADER_5991;

typedef struct 
{
  BYTE bFormat;
  BYTE bReserved1;
  BYTE bReserved2;
  BYTE bRecordType : 4;
  BYTE bResolution : 4;
} SIG_HEADER_5991;

static int FindNumPoints(const BSTR bstrStrokeData, int nLength )
{
  const CO_ORD_DATA*   pPoints = (const CO_ORD_DATA*)bstrStrokeData;
  int nIndex;

  for ( nIndex = 0; nIndex < nLength; nIndex++ )
  {
    if ( pPoints[ nIndex ].Y_VAL == -1
       && pPoints[ nIndex ].X_VAL == -1 )
       break;
  }
  return nIndex; 
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSCOTSigCap::CSCOTSigCap()
{
  m_bHaveValidSig = FALSE;
  m_hDispWnd = NULL;
}

CSCOTSigCap::~CSCOTSigCap()
{

}

//////////////////////////////////////////////////////////////////////
// CSCOTOPOSSigCap methods - these use an OPOS SigCap control
//////////////////////////////////////////////////////////////////////


CSCOTOPOSSigCap::CSCOTOPOSSigCap()
{
  m_bInSigcap = false;
}


CSCOTOPOSSigCap::~CSCOTOPOSSigCap()
{
  DetachDispWnd();
}


long CSCOTOPOSSigCap::Enable()
{
  long rc = 0;
  
  m_Sigcap.SetDeviceEnabled(TRUE);
  rc = m_Sigcap.GetResultCode();
  return TranslateOPOSRetToSCOTRet(rc);
}

long CSCOTOPOSSigCap::Disable()
{
  long rc = 0;
  
  m_Sigcap.SetDeviceEnabled(FALSE);
  rc = m_Sigcap.GetResultCode();
  return TranslateOPOSRetToSCOTRet(rc);
}

long CSCOTOPOSSigCap::Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID)
{
  long rc = 0;

  m_csProfile = profile;

  m_pMainWnd = pMainWnd;

  if (!m_Sigcap.Initialize(profile, pParentWnd, nID))
  {
    CString csErr = m_Sigcap.GetErrorFunc();
    ScotError(RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_OPOS_CTRL_INIT_FAILURE, _T("%s|%s"), 
          profile, csErr);
    rc = -1;
  }
  
  return rc;
}

long CSCOTOPOSSigCap::UnInitialize()
{
  long rc = 0;
  
  m_Sigcap.UnInitialize();
  return rc;
}

void CSCOTOPOSSigCap::DataEvent(long Status)
{
  // The only data event that the OPOS sig cap
  // spec supports is SigcapEnded.  So if we get any
  // data event, we know we can tell the core that it ended.

  long DMStatus = DM_SIGCAP_ENDED;

  m_bInSigcap = false;

  mo.PostDM(DM_STATUS, DMCLASS_SIGCAP, 0, DMStatus, NULL);

  m_Sigcap.SetDataEventEnabled(TRUE);
}


void CSCOTOPOSSigCap::ErrorEvent(LONG ResultCode, 
                                 LONG ResultCodeExtended, 
                                 LONG ErrorLocus, 
                                 LONG* pErrorResponse)
{
  long DMStatus = DM_SIGCAP_FAILURE;
  ScotError(RETURN, SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_SC_BEGCAP_FAILED, _T("%s|%d|%s|%s"), _T("OPOSSigCap::ErrorEvent"), ResultCode, _T("assumed to be Sig Capture."), _T("Signature capture failed while signing signature pad."));
  if(ResultCode == OPOS_E_FAILURE)
  {
    mo.PostDM(DM_STATUS, DMCLASS_SIGCAP, 0, DMStatus, NULL);
  }
}


long CSCOTOPOSSigCap::BeginCapture(LPCTSTR szCaptureForm, HWND hDrawWnd)
{
  trace(L6, _T("+BeginCapture"));
  m_bHaveValidSig = FALSE;

  // Subclass the display window in preparation for painting it after
  // the capture is complete.

  AttachDispWnd(hDrawWnd);

  CString func = _T("Enable");

  long rc = Enable();
  if (rc == 0)
  {
    if (szCaptureForm == NULL)
      szCaptureForm = SCOT_DEFAULT_SIGFORM;

    func = _T("BeginCapture");

    try
    {
      rc = m_Sigcap.BeginCapture(szCaptureForm);
    }
    catch(...)
    {
      rc = -1;
      trace(L6, _T("Got an exception in BeginCapture"));
    }
  }

  if (rc != 0)
  {
    ScotError(RETURN, SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_SC_BEGCAP_FAILED, _T("%s|%d|%s"), 
          func, rc, szCaptureForm);
  }
  else
  {
    m_bInSigcap = true;
  }

  trace(L6, _T("-BeginCapture"));
  return rc;
}

long CSCOTOPOSSigCap::EndCapture()
{
  trace(L6, _T("+EndCapture"));
  long rc = 0;
  m_lTotalPoints = 0; //TAR 338816

  if (m_bInSigcap)
  {
    try
    {
      m_bInSigcap = false;
      rc = m_Sigcap.EndCapture();
    }
    catch(...)
    {
      rc = -1;
      trace(L6, _T("Got an exception in EndCapture"));
    }
  }

  if (rc != 0)
  {
    ScotError(RETURN, SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_SC_ENDCAP_FAILED, _T("%d"), rc);
  }
  else if (m_Sigcap.GetTotalPoints() > SIG_VALID_PTS_THRESHOLD)
  {
    m_bHaveValidSig = TRUE;
    rc = DisplaySigOnWindow();
  }

  ClearDisplay();
  trace(L6, _T("-EndCapture"));
  return rc;
}

// This code was copied from DM to get an OPOS
// signature Point Array into 5992 compressed format

long CSCOTOPOSSigCap::Get5991Format(BSTR* pData)
{
    long rc = 0;
	long totalPts=0;
	CString sigData;
  
  totalPts = m_Sigcap.GetTotalPoints();
  sigData  = m_Sigcap.GetPointArray();


  ASSERT( totalPts == sigData.GetLength() / 4 );
  ASSERT( sizeof( CO_ORD_DATA ) == 4 ); 

  char * cDest = (char * )_alloca( totalPts * sizeof( CO_ORD_BUF ) ); 
  // for each stroke               
  BSTR bstrAllStrokes = (BSTR)(LPCTSTR)sigData;
  // Set up the sig header, one per signature
  SIG_HEADER_5991* pSigHeader = (SIG_HEADER_5991*)( cDest );
  pSigHeader->bFormat = SIG_HEAD_BYTE1_VAL;/*  load the header             */
  pSigHeader->bReserved1 = SIG_HEAD_BYTE2_VAL;
  pSigHeader->bReserved2 = SIG_HEAD_BYTE3_VAL;
  pSigHeader->bRecordType = compress_alg;     // Signature
  pSigHeader->bResolution = 0;   // Why?

  BSTR bstrThisStroke = bstrAllStrokes;
  // Find the number of points in this stroke, but not more than are left
  // May not be EOS terminated.
  UINT nDestIndex = sizeof( SIG_HEADER_5991 );  
  while ( bstrThisStroke < bstrAllStrokes + totalPts * 2 )
  {
    UINT numPointsThisStroke = FindNumPoints( bstrThisStroke, 
    totalPts - ( bstrThisStroke - bstrAllStrokes ) / 2 );
    // If stroke is too large, (pcoOrdBuf->vli must be less than 
    // 32768/4 points - see multiplication below).  If the stroke
    // is too long, break it up and treat it as multiple strokes. 
#define MAX_STROKE_LENGTH (32768/4-1)
    BOOL bSplitStroke = numPointsThisStroke > MAX_STROKE_LENGTH;
    if ( bSplitStroke != FALSE )
       numPointsThisStroke = MAX_STROKE_LENGTH;
    // Allocate memory and swap 
    CO_ORD_BUF* pcoOrdBuf = (CO_ORD_BUF*)_alloca( sizeof( CO_ORD_BUF ) 
    + numPointsThisStroke * sizeof( CO_ORD_DATA ) );
    pcoOrdBuf->vli = numPointsThisStroke * sizeof( CO_ORD_DATA );
    for ( UINT nIndex = 0; nIndex < numPointsThisStroke; nIndex++ )
    {     // Reverse the x-y pairs
      CO_ORD_DATA *pData = &pcoOrdBuf->co_ord;
      pData [ nIndex ].Y_VAL = bstrThisStroke[ nIndex * 2 + 1];
      pData [ nIndex ].X_VAL = bstrThisStroke[ nIndex * 2 ];
    }
    {
    UINT count_ptr = 0;
    /*  go compress the stroke data */
    nDestIndex += compress(
                        (char *)pcoOrdBuf,                               // Constant
                        (char *)pcoOrdBuf + sizeof( CO_ORD_BUF ) 
                        + numPointsThisStroke * sizeof( CO_ORD_DATA ),   // Constant
                        pcoOrdBuf,
                        (COMP_STROKE_BUF*)(cDest + nDestIndex),
                        cDest + totalPts * sizeof( CO_ORD_BUF ),
                        &count_ptr );
    ASSERT( count_ptr == numPointsThisStroke );
    ASSERT( nDestIndex < totalPts * sizeof( CO_ORD_BUF ) );
    // If stroke was too large, then repeat last points
    // into the new stroke so that there are no gaps. 
    if ( bSplitStroke != FALSE )
       numPointsThisStroke -=2;
    bstrThisStroke += ( numPointsThisStroke + 1 ) * 2;
    }
  }

  

  SysFreeString( *pData );

  *pData = SysAllocStringByteLen( cDest, nDestIndex );

  return rc;
}


long CSCOTOPOSSigCap::DisplaySigOnWindow()
{
  long rc = 0;

  CString sigData = m_Sigcap.GetPointArray();
  DrawSignature(m_Sigcap.GetTotalPoints(), sigData);
  return rc;
}



CSCOTOPOSSigCap* CSCOTOPOSSigCap::m_pThis = NULL;


// This function subclasses the SigDisplay window.
// It is called by BeginCapture to get things ready
// for an eventual display.
//
// Subclassing (in this context) means that our windows
// message handler will get all the messages destined for 
// that window.  Then we can either do something new with the
// message, or pass it to the original handler for the window.
//
// In this case will be intercepting WM_PAINT messages and 
// calling our OnDraw for these.  In all other cases, we will
// call the window's handler.  Note that since DPS creates
// windows on another thread from the core app, our 
// SubClassSigCapWindowFunc() and OnDraw() will be called on
// a different thread than this control was created on.
//
// Note also when subclassing across threads, you can't use CWnd
// objects from another thread, you have to use Window Handles.
// That is because MFC GUI objects are not valid across threads.

void CSCOTOPOSSigCap::AttachDispWnd(HWND hWnd)
{
  if (hWnd)
  {
    if (m_hDispWnd != NULL)
      DetachDispWnd();

    m_pThis = this;
    m_hDispWnd = hWnd;
    m_lpfnOldWndProc = (WNDPROC)::SetWindowLong(m_hDispWnd,
                                              GWL_WNDPROC, 
                                              (long)SubClassSigCapWindowFunc);
    ASSERT(m_lpfnOldWndProc != NULL);
  }
  else
  {
     ScotError(RETURN, SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_SC_DISPWND_NULL);
  }
}

void CSCOTOPOSSigCap::DetachDispWnd()
{
  if (m_hDispWnd)
  {
    ASSERT(m_lpfnOldWndProc != NULL);
    ::SetWindowLong( m_hDispWnd, GWL_WNDPROC, (long)m_lpfnOldWndProc );
    m_lpfnOldWndProc = NULL;
    m_hDispWnd       = NULL;
    m_pThis          = NULL;
  }
}

LRESULT CALLBACK CSCOTOPOSSigCap::SubClassSigCapWindowFunc(HWND hWnd, 
	                                                         UINT wMessage,
	                                                         WPARAM wParam, 
	                                                         LPARAM lParam)
{
  ASSERT( m_pThis != NULL );

  // Since this is a static function, we can't call our methods
  // directly, we have to call thru the static m_pThis pointer that
  // was setup previously.

  LONG lResult = m_pThis->CallWindowProc( wMessage, wParam, lParam);
  if ( wMessage == WM_PAINT )
    m_pThis->OnDraw();
  return lResult;
}


LRESULT CSCOTOPOSSigCap::CallWindowProc(UINT     Msg, 
                                        WPARAM   wParam, 
                                        LPARAM   lParam ) const 
{
  return ::CallWindowProc( m_lpfnOldWndProc, m_hDispWnd, Msg, wParam, lParam );
}


// This function is called by EndCapture to "Draw" the signature on
// the display window.  However, it is really Windows issuing a WM_PAINT
// message that actually draws.  This function just sets things up so that
// when the OnDraw handler is called, it is ready to go.

void CSCOTOPOSSigCap::DrawSignature(long lTotalPoints, LPCTSTR pRawData)
{
  m_lTotalPoints = lTotalPoints;
  //+338816
  if(m_lTotalPoints > 4000) //if exceeds max points for PA array
  {
   	trace(L6,_T("Max number of points exceeded; setting to maximum"));
   	m_lTotalPoints = 4000;
  }
  //-338816
  unsigned char* pPoints = (unsigned char*) (pRawData);


  // Don't start w/ an Initial Stroke End marker - skip past any:

  while ((*((int*) pPoints) == 0xFFFFFFFF) && (m_lTotalPoints))
  {
    pPoints += 4;
    m_lTotalPoints--;
  }

  memcpy(PA, pPoints, m_lTotalPoints*4);
  int pa_index = (m_lTotalPoints*4);
	PA[pa_index] = 0xFF;
	pa_index++;
	PA[pa_index] = 0xFF;
	pa_index++;
	PA[pa_index] = 0xFF;
	pa_index++;
	PA[pa_index] = 0xFF;
	pa_index++;

	m_lTotalPoints++;
  FindMaxMins(m_SigRect);
}

// This function is called by Windows in response to a WM_PAINT being
// issued for the SigCap display window.  The whole point of subclassing
// that window is so that this OnDraw function will get called and the sig
// will be drawn in the window.

void CSCOTOPOSSigCap::OnDraw( void )
{
  ASSERT( m_hDispWnd != NULL );
  CWnd* pWnd= CWnd::FromHandle( m_hDispWnd );
  CDC& dc = *pWnd->GetDC();

  // Get the coordinates of the Rectangle into which the signature 
  // will be placed ....
  CRect rDisp;
  pWnd->GetClientRect(rDisp);

	// Paint the rectangle white
	CBrush BkgdBr;
	BkgdBr.CreateSolidBrush(RGB(255,255,255));
	dc.FillRect(rDisp, &BkgdBr);
	BkgdBr.DeleteObject();

	// Fill the rectangle with the signature, if the signature exists ...
	if (m_lTotalPoints != 0)
	{
		CPen	NewPen;
		CPen*	pOldPen;

		NewPen.CreatePen(0,1,RGB(0,0,0));
		pOldPen = (CPen*) dc.SelectObject(&NewPen);

    CalcNewCoordinates(rDisp);

		// First, set up window mapping so that the signature will be
    // scaled from the size of the device to the size of the window,
    // but Aspect ratio will be maintained.

		dc.SetMapMode(MM_ISOTROPIC);

    SetWindowExtEx(dc.m_hDC, 
                   m_WindowExtent.x, 
                   m_WindowExtent.y, 
                   NULL);

 		SetViewportExtEx(dc.m_hDC, 
                     m_ViewportExtent.x,
                     m_ViewportExtent.y,
                     NULL);

    SetViewportOrgEx(dc.m_hDC, 
                     m_ViewportOrigin.x,
                     m_ViewportOrigin.y,
                     NULL); 

   
		//
		// Perform MoveTo/LineTo calls until signature is painted ...
		//

		unsigned char x_lsb = PA[0];
		unsigned char x_msb = PA[1];
		unsigned char y_lsb = PA[2];
		unsigned char y_msb = PA[3];

		long x = (x_msb * 256) + x_lsb;
		long y = (y_msb * 256) + y_lsb;

		dc.MoveTo(x, y);

		for (int i=1; i<=m_lTotalPoints - 1; i++)
		{
			x_lsb = PA[(i*4)+0];
			x_msb = PA[(i*4)+1];
			y_lsb = PA[(i*4)+2];
			y_msb = PA[(i*4)+3];

			if ((x_msb == 255) && (x_lsb == 255))
			{
				// Pen-Lifted ... get next start pt ....
				if (! (i==(m_lTotalPoints - 1)))
				{
					i = i + 1;

					x_lsb = PA[(i*4)+0];
					x_msb = PA[(i*4)+1];
					y_lsb = PA[(i*4)+2];
					y_msb = PA[(i*4)+3];

					x = (x_msb * 256) + x_lsb;
					y = (y_msb * 256) + y_lsb;
							
					dc.MoveTo(x, y);
				}
			}
			else
			{
				x = (x_msb * 256) + x_lsb;
				y = (y_msb * 256) + y_lsb;
			
				dc.LineTo(x, y);
			}
		}

 		dc.SelectObject(pOldPen);
		NewPen.DeleteObject();
	}
  return;
}

// Find the max and min X and Y points in our signature.
// These are used to scale the sig from sigcap size to 
// display size

void CSCOTOPOSSigCap::FindMaxMins(CRect &cornerRect)
{
  short minX = (short) USHRT_MAX, maxX = 0, minY = (short) USHRT_MAX, maxY = 0;

  int numPoints = m_lTotalPoints;

  short* pVal = (short*) PA;
  // scan for max, min y
  while (numPoints--)
  {
    maxX = max(*pVal, maxX);
    minX = min(*pVal, minX);

    pVal++;

    maxY = max(*pVal, maxY);
    minY = min(*pVal, minY);

    pVal++;
  }

  cornerRect.SetRect(minX, minY, maxX, maxY);
}

// The intent of this function is to set things up so that no matter
// what size the sig is on the device, and no matter what amount of space
// it takes up on the device, the displayed window will have the sig
// centered horizontally and vertically, taking up the whole window.

void CSCOTOPOSSigCap::FindNewOrigin(CRect &wnd, CRect &dev, CPoint &newOrigin)
{
  int devX = dev.right  - dev.left;
  int devY = dev.bottom - dev.top;

  int wndX = wnd.right  - wnd.left;
  int wndY = wnd.bottom - wnd.top;

  int devXwndY = devX * wndY;
  int wndXdevY = wndX * devY;

  if (devXwndY == 0)
    devXwndY++;

  if (wndXdevY == 0)
    wndXdevY++;


  if (devXwndY > wndXdevY)
  {
    // We need to shift device's y origin
    float ratioY = (float)(devXwndY) / (float)(wndXdevY);
    float flOrgY = ((float) wndY - ((float) wndY / ratioY)) / (float) 2.0;

    newOrigin.y = int(flOrgY);
    newOrigin.x = 0;
  }
  else if (devXwndY < wndXdevY)
  {
    // We need to shift device's x origin
    float ratioX = (float)(wndXdevY) / (float)(devXwndY);
    float flOrgX = ((float) wndX - ((float) wndX / ratioX)) / (float) 2.0;

    newOrigin.x = int(flOrgX);
    newOrigin.y = 0;
  }
  else // do nothing
  {
    newOrigin.x = 0;
    newOrigin.y = 0;
  }
}

void CSCOTOPOSSigCap::CalcNewCoordinates(CRect &rDisp)
{
  CPoint newOrigin(0,0);

	// First, set up window mapping ...
  m_WindowExtent.x = m_SigRect.right  - m_SigRect.left;
  m_WindowExtent.y = m_SigRect.bottom - m_SigRect.top;

  FindNewOrigin(rDisp, m_SigRect, newOrigin);

  // Calculate 5% of the width and height of our rDisp rect and use it as fudge
  // to keep a little white space around the drawing.

  int fudgeX = (rDisp.right  - rDisp.left) / 20;
  int fudgeY = (rDisp.bottom - rDisp.top)  / 20;

  int viewExtY, viewOrgY;

  // OPOS defines the Y axis origin to be opposite that of
  // how Windows does it, so flip the Y axis.  The weird 
  // if (FALSE) state is just in here for future use if necessary.

  if (FALSE /* m_yAxisFlipped */)
  {
    viewExtY = -((rDisp.bottom - rDisp.top) - fudgeY);
    viewOrgY = rDisp.bottom - ((fudgeY / 2) + newOrigin.y) ;
  }
  else
  {
    viewExtY = (rDisp.bottom - rDisp.top) - fudgeY;
    viewOrgY = rDisp.top  + ((fudgeY / 2) + newOrigin.y);
  }

  m_ViewportExtent.x = (rDisp.right - rDisp.left) - fudgeX; 
  m_ViewportExtent.y = viewExtY;

  m_ViewportOrigin.x = rDisp.left + (fudgeX / 2) + newOrigin.x; 
  m_ViewportOrigin.y = viewOrgY;
}


//////////////////////////////////////////////////////////////////////
// CSCOTDMTopaz methods - these use the SCOT 1.1 DM SigCap interface for Topaz
//////////////////////////////////////////////////////////////////////


CSCOTDMTopaz::CSCOTDMTopaz()
{

}


CSCOTDMTopaz::~CSCOTDMTopaz()
{

}


long CSCOTDMTopaz::Enable()
{
  long rc = 0;
  trace(L6,_T("+CSCOTDMTopaz::Enable"));
	
  rc = dm.SigCapEnable();			  // enable the topaz sig cap
	
  trace(L6,_T("-CSCOTDMTopaz::Enable %d"),rc);
  return rc;
}

long CSCOTDMTopaz::Disable()
{
  long rc = 0;
  trace(L6,_T("+CSCOTDMTopaz::Disable"));
	
  rc = dm.SigCapDiable();			  // disable the encryptor
	
  trace(L6,_T("-CSCOTDMTopaz::Disable %d"),rc);
  return rc;
}


long CSCOTDMTopaz::Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID)
{
  long rc = 0;
  
  m_csProfile = profile;

  // All of our initialization is done by DMInterface, so this
  // is a no-brainer.

  return rc;
}

long CSCOTDMTopaz::UnInitialize()
{
  long rc = 0;
  
  // All of our uninitialization is done by DMInterface, so this
  // is a no-brainer.

  return rc;
}


long CSCOTDMTopaz::BeginCapture(LPCTSTR szCaptureForm, HWND hDrawWnd)
{
  long rc = 0;
  m_bHaveValidSig = FALSE;
  rc = Enable();
  if (rc == 0)
  {
		/******cm150000: Commented Out In Merged Code******/
    //rc = 	dm.SetSigCapNotifyStyle(1);
		/******cm150000: End Comment In Merged Code********/
    if (rc == 0)
    {
      rc = dm.SigCapSetWindowHandle((long*)&hDrawWnd);
      if (rc == 0)
      {
	      rc = dm.SigCapBeginCapture(szCaptureForm);
      }
    }
  }

  return rc;
}

long CSCOTDMTopaz::EndCapture()
{
  long rc = dm.SigCapEndCapture() ;
  if (rc == 0)
  {
    // We will assume that if this works we have a valid sig.  Not
    // sure that is necessarily the case, though
    m_bHaveValidSig = TRUE;
  }

  return rc;
}

long CSCOTDMTopaz::Get5991Format(BSTR* pData)
{
  long rc = dm.SigCapDataTo5991Format(pData);
  return rc;
}


//////////////////////////////////////////////////////////////////////
// CSCOT5992SigCap methods - these use an OPOS 5992 SigCap control
//////////////////////////////////////////////////////////////////////


CSCOT5992SigCap::CSCOT5992SigCap()
{
}


CSCOT5992SigCap::~CSCOT5992SigCap()
{
}

long CSCOT5992SigCap::ClearDisplay()
{
  long rc = 0;
  long myLong = 0;
  _TCHAR ClearCommand[] = {0x05, 0x04, 0x31, 0x30, 0x0 };
  CString string = ClearCommand;
  BSTR bstr = string.AllocSysString();
  rc = m_Sigcap.DirectIO(0, &myLong, &bstr);
  SysFreeString(bstr);
  return rc;
}
