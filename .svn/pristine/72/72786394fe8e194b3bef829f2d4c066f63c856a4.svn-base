// NextGenUISigCap.cpp: implementation of the CNextGenUISigCap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifndef _CPPUNIT
#include "Common.h"                 // common includes
#endif // _CPPUNIT

#include "NextGenUISigCap.h"
#include "LaneStateData.h"

extern "C" int compress(char *circ_start,
	     char *circ_end,
	     CO_ORD_BUF  *sig_ptr,
	     COMP_STROKE_BUF  *dest_buff,
	     char  *end_dest_buff,
	     unsigned int *count_ptr );
extern "C" char  compress_alg ;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define COMP_ID ID_OD
#define T_ID    _T("NextGenUI SigCap")

CNextGenUISigCap::CNextGenUISigCap()
{
}


CNextGenUISigCap::~CNextGenUISigCap()
{

}


long CNextGenUISigCap::Enable()
{
  long rc = 0;
  trace(L6,_T("+CNextGenUISigCap::Enable"));
	
	
  trace(L6,_T("-CNextGenUISigCap::Enable %d"),rc);
  return rc;
}

long CNextGenUISigCap::Disable()
{
  long rc = 0;
  trace(L6,_T("+CNextGenUISigCap::Disable"));
	
	
  trace(L6,_T("-CNextGenUISigCap::Disable %d"),rc);
  return rc;
}


long CNextGenUISigCap::Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID)
{
  long rc = 0;

  m_csProfile = profile;

  return rc;
}

long CNextGenUISigCap::UnInitialize()
{
  long rc = 0;
  
  // All of our uninitialization is done by DMInterface, so this
  // is a no-brainer.

  return rc;
}


long CNextGenUISigCap::BeginCapture(LPCTSTR szCaptureForm, HWND hDrawWnd)
{
  long rc = 0;
  m_csSigData.Empty();

  COleVariant v;
  v = m_csSigData;
  ps.GetPSXObject()->SetTransactionVariable(_T("NextGenUISignatureData"), v);

  CLaneStateData laneStateData(ps.GetPSXObject());
  laneStateData.SendNextGenUIData(_T("NextGenUIShowSigCapture"), (bool)true);
  return rc;
}

long CNextGenUISigCap::EndCapture()
{
  long rc = 0 ;

  return rc;
}



const TCHAR delimBar= _T('|');
const TCHAR delimSpace= _T(' ');

int CNextGenUISigCap::ParseStroke(const CString &csStroke, short PointArray[])
{
	int stopIndex, startIndex=0;
	bool bLast=false;
	int count = 0;
	short sValue;

	do {
		stopIndex= csStroke.Find(delimSpace, startIndex);
		if (stopIndex == startIndex) { // if starting with a space
			startIndex ++;
			continue;
		}
		bLast = stopIndex == -1;
		if (bLast)
			sValue = _ttoi(csStroke.Mid(startIndex));
		else
			sValue = _ttoi(csStroke.Mid(startIndex, stopIndex - startIndex));

		startIndex = stopIndex +1;
		
		if (count % 2 == 0) {
			// x value. scale as necessary
			PointArray[count] = sValue + 1;
			//pData [ (int)(count/2) ].X_VAL = sValue;
		} else {
			// y value. scale as necessary
			PointArray[count] = sValue + 1;
			//pData [ (int)(count/2) ].Y_VAL = sValue;
		}
		count++;
	} while (!bLast);
	PointArray[count++] = -1;
	PointArray[count++] = -1;
	return count;
}

// DPIX DPIY|Width Height|x1 y1 x2 y2 x3 y3... | x1 y1 x2 y2
// 96 96|500.0 600.0|100.0 200.0 200.0 200.0 300.0 200.0|500.0 500.0 500.0 600.0 600.0 600.0
int CNextGenUISigCap::ParseWPFSigFormat(const CString &csData, short PointArray[])
{
	int dpiX, dpiY;
	int width, height;
	int startIndex, stopIndex;
	bool bLastStroke;
	int totalPoints=0;

	// dpix
	startIndex= csData.Find(delimSpace);
	if (startIndex > 0) {
		dpiX= _ttoi(csData.Left(startIndex));

		// dpiy 
		startIndex++;
		stopIndex= csData.Find(delimBar, startIndex);
		if (stopIndex > 0) {
			dpiY = _ttoi(csData.Mid(startIndex, stopIndex - startIndex));
		}

		// width 
		startIndex= stopIndex +1;
		stopIndex= csData.Find(delimSpace, startIndex);
		if (stopIndex > 0) {
			width = _ttoi(csData.Mid(startIndex, stopIndex - startIndex));
		}
		// height
		startIndex= stopIndex +1;
		stopIndex= csData.Find(delimBar, startIndex);
		if (stopIndex > 0) {
			height = _ttoi(csData.Mid(startIndex, stopIndex - startIndex));
		}

		CString csStroke;
		// strokes loop
		do {
			startIndex= stopIndex +1;
			stopIndex= csData.Find(delimBar, startIndex);

			bLastStroke = stopIndex == -1;
			if (bLastStroke)
				csStroke = csData.Mid(startIndex);
			else
				csStroke = csData.Mid(startIndex, stopIndex - startIndex);

            csStroke = AdjustStroke(csStroke);
			// y,x values loop
			totalPoints += ParseStroke(csStroke, &PointArray[totalPoints]);

		} while (!bLastStroke);
	}

    SetWidth(width);
    SetHeight(height);
	return totalPoints/2; // divide by 2 to count how many x,y points
}

//If there is only one point per stroke, need to adjust the stroke data by adding the same point into the stroke
CString CNextGenUISigCap::AdjustStroke(const CString &csStroke)
{
    int totalPoint = 0;
    int i = 0;
    int contain = 0;

    CString csAdjustStroke;

    while((contain = csStroke.Find(delimSpace,i)) != -1){
        totalPoint++;
        i = contain + 1;
    }

    if(totalPoint == 1)
    {
        short xValue = _ttoi(csStroke.Left(i)) + 4;
        int nLength = csStroke.GetLength();
        short yValue = _ttoi(csStroke.Right(nLength - i)) + 4;
        csAdjustStroke.Format(_T("%s %d %d"), csStroke, xValue, yValue);
    }
    else
    {
        csAdjustStroke = csStroke;
    }
    return csAdjustStroke;
}

long CNextGenUISigCap::Get5991Format(BSTR* pData)
{
  long rc = 0;
  long totalPts=0;
 
  // instead of getting the exact number of points and parsing twice, 
  // we'll just over-estimate the size for the PointArray
  // each point will be minimum of 3 chars, and typically six chars.  assume 3 char points to be conservative
	
  short *PointArray = new short[m_csSigData.GetLength()/3 + 1024];

  //CString csData= _T("96 96|500.0 600.0|100.0 200.0 200.0 200.0 300.0 200.0|500.0 500.0 500.0 600.0 600.0 600.0");

  totalPts= ParseWPFSigFormat(m_csSigData, PointArray);
  
  char * cDest = (char * )_alloca( totalPts * sizeof( CO_ORD_BUF ) ); 

  // for each stroke               
  BSTR bstrAllStrokes = (BSTR)PointArray;
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
    CSigCapUtility sigCapUtility;
    UINT numPointsThisStroke = sigCapUtility.FindNumPoints( bstrThisStroke, totalPts - ( bstrThisStroke - bstrAllStrokes ) / 2 );
    // If stroke is too large, (pcoOrdBuf->vli must be less than 
    // 32768/4 points - see multiplication below).  If the stroke
    // is too long, break it up and treat it as multiple strokes. 
#define MAX_STROKE_LENGTH (32768/4-1)
    BOOL bSplitStroke = numPointsThisStroke > MAX_STROKE_LENGTH;
    if ( bSplitStroke != FALSE )
       numPointsThisStroke = MAX_STROKE_LENGTH;

    // Allocate memory and swap 
    CO_ORD_BUF* pcoOrdBuf = (CO_ORD_BUF*)_alloca( sizeof( CO_ORD_BUF ) + numPointsThisStroke * sizeof( CO_ORD_DATA ) );

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

  delete [] PointArray;
  SysFreeString( *pData );

  *pData = SysAllocStringByteLen( cDest, nDestIndex );

  return rc;
}


CString CNextGenUISigCap::GetcsSigData() const
{
    return m_csSigData;
}

void CNextGenUISigCap::SetcsSigData(const CString &csData)
{
    m_csSigData = csData;
    COleVariant v;
    v = m_csSigData;
    ps.GetPSXObject()->SetTransactionVariable(_T("NextGenUISignatureData"), v);
}

void CNextGenUISigCap::SetWidth(const long lValue)
{
    m_lWidth = lValue;
}

void CNextGenUISigCap::SetHeight(const long lValue)
{
    m_lHeight = lValue;
}
