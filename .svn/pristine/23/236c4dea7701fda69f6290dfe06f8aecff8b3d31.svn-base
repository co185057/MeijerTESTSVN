//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCashManagementBase.CPP
//
// TITLE: Class implementation for store mode CashManagement state
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSmCashManagement.h"      
#include "SmSmCashierPassword.h"
#include "SMSmMediaStatus.h"

#include "DMCashCount.h"
DLLEXPORT extern DMCashCount  dmcc;    // Global DM object


#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SmCashManagement")

bool SMSmCashManagementBase::m_bGlory = false;
bool SMSmCashManagementBase::m_bfFirstEntry = true;
bool SMSmCashManagementBase::m_bfAuthenticated = false;
int SMSmCashManagementBase::nSavedPurgeCnt = 0;
int SMSmCashManagementBase::nCoinCashSeparator = 0;
bool SMSmCashManagementBase::m_bBill = true; //default to bill screen

CArray<int,int> SMSmCashManagementBase::m_caSavedOriginalCountList; // not used. maybe we can use for future use
CStringArray SMSmCashManagementBase::m_caSavedOriginalColumnPositions;
CArray<int,int> SMSmCashManagementBase::m_caChangeInCount;         // we store all the change in counts
CArray<CString,CString> SMSmCashManagementBase::caDispenserDenomList;
CArray<int,int> SMSmCashManagementBase::caValueList;
CArray<int,int> SMSmCashManagementBase::caCountList;

CArray<CString,CString> SMSmCashManagementBase::m_caCashAcceptorDenomList;
CArray<CString,CString> SMSmCashManagementBase::m_caCoinAcceptorDenomList;

CArray<int,int> SMSmCashManagementBase::m_caCashAcceptorCountList;
CArray<int,int> SMSmCashManagementBase::m_caCoinAcceptorCountList;
CArray<int,int> SMSmCashManagementBase::m_caCashAcceptorChangeInCountList;
CArray<int,int> SMSmCashManagementBase::m_caCoinAcceptorChangeInCountList;

#define SHOWMINIMUMCOUNT 20
#define MAXCOLUMNS 6
#define GENERICMAXCOLUMNS 8
#define ACCEPTORDEVICECYLINDERINDEX  GENERICMAXCOLUMNS
#define SEPARATOR _T(":")
#define HEIGHTOFBOTTOMDIAMETER 20
#define GENERICHEIGHTOFBOTTOMDISPENSERDIAMETER 13
#define GENERICHEIGHTOFBOTTOMACCEPTORDIAMETER 17

IMPLEMENT_DYNCREATE(SMSmCashManagementBase, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////
SMSmCashManagementBase::SMSmCashManagementBase()
:nPurgeCnt(0), 
m_bCoinHasMadeChanges(false),m_bBillHasMadeChanges(false),
nCashAcceptorTotalCount(0),nCashAcceptorTotalValue(0), nCoinAcceptorTotalChangeInCount(0),
nCoinAcceptorTotalCount(0),nCoinAcceptorTotalValue(0),nCashAcceptorTotalChangeInCount(0),
nCashAcceptorTotalChangeInValue(0), nCoinAcceptorTotalChangeInValue(0), //tar 299529
nCoinDispenserTotalCount(0), nCashDispenserTotalCount(0),
nCoinDispenserTotalValue(0), nCashDispenserTotalValue(0),
nLoadIndex(0)
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMSmCashManagementBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmCashManagement") );	// RFC 330050
  ps.Echo( _T("") );
  !dm.m_csCashRecyclerModel.IsEmpty() ? m_bGlory = true : m_bGlory = false;
  trace( L6, _T("Is Glory device? %d, %s"), m_bGlory, dm.m_csCashRecyclerModel );
  if(DataInitialize())   //If return true, refresh the screen.
  {
    showScreen();
  }
  SetDevices();
  nTimeOut = 0;

  // Do not preserve recycler values in cash management.
  if(dm.m_csCashRecyclerModel == _CASHDISPENSER_GLORY)
  {
	  dm.CashRecyclerPauseCounts(true);			// TAR 311471
  }

  return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    GenericDataInitialize                                        
// Description: Initializes the data based on the DMCashCounts class
///////////////////////////////////////////////////////////////////////////////////
void SMSmCashManagementBase::GenericDataInitialize()
{
  trace(L6,_T("+GenericDataInitialize()"));
  CArray<CString,CString> caCashAcceptorDenomList, caCoinAcceptorDenomList, caCashChangerDenomList, caCoinChangerDenomList;
  CArray<int,int> caCashAcceptorCountList, caCoinAcceptorCountList, caCashChangerCountList, caCoinChangerCountList;

  trace(L9,_T("Getting Currency Denominations."));
  dmcc.GetCashDenoms(&caCashAcceptorDenomList, &caCoinAcceptorDenomList, &caCashChangerDenomList, &caCoinChangerDenomList);
  trace(L6,_T("Denomination Sizes : Cash Acceptor : %d, Coin Acceptor : %d, CashChanger : %d, CoinChanger : %d."),
    caCashAcceptorDenomList.GetSize(), caCoinAcceptorDenomList.GetSize(), 
    caCashChangerDenomList.GetSize(), caCoinChangerDenomList.GetSize());
  m_caCashAcceptorDenomList.Copy( caCashAcceptorDenomList );
  m_caCoinAcceptorDenomList.Copy( caCoinAcceptorDenomList );

  trace(L9,_T("Getting Currency Counts."));
  dmcc.GetCashCounts(&caCashAcceptorCountList, &caCoinAcceptorCountList, &caCashChangerCountList, &caCoinChangerCountList);
  trace(L6,_T("Count Sizes : Cash Acceptor : %d, Coin Acceptor : %d, CashChanger : %d, CoinChanger : %d."),
    caCashAcceptorCountList.GetSize(), caCoinAcceptorCountList.GetSize(), 
    caCashChangerCountList.GetSize(), caCoinChangerCountList.GetSize());
  m_caCashAcceptorCountList.Copy( caCashAcceptorCountList ); 
  m_caCoinAcceptorCountList.Copy( caCoinAcceptorCountList ); 

  m_caCashAcceptorChangeInCountList.RemoveAll();
  m_caCoinAcceptorChangeInCountList.RemoveAll();
        

  m_caCashAcceptorChangeInCountList.SetSize( m_caCashAcceptorCountList.GetSize() );
  m_caCoinAcceptorChangeInCountList.SetSize( m_caCoinAcceptorCountList.GetSize() );

  trace(L6,_T("Initializing Change In Count."));
  for( int i = 0; i < m_caCashAcceptorChangeInCountList.GetSize(); i++ )
    {m_caCashAcceptorChangeInCountList[i] = 0;}
  for( int j = 0; j < m_caCoinAcceptorChangeInCountList.GetSize(); j++ )
    {m_caCoinAcceptorChangeInCountList[j] = 0;}

  trace(L9,_T("Setting Cash/Coin Changer Denomination List."));
  caValueList.RemoveAll();
  caDispenserDenomList.RemoveAll();

  //since the 
  for( int k = 0; k < caCoinChangerDenomList.GetSize(); k++ )
  {
    caValueList.Add( _ttol(caCoinChangerDenomList[k]));
    caDispenserDenomList.Add( caCoinChangerDenomList[k] );
  }
  for( int l = 0; l < caCashChangerDenomList.GetSize(); l++ )
  {
    caValueList.Add( _ttol(caCashChangerDenomList[l]));
    caDispenserDenomList.Add( caCashChangerDenomList[l] );
  }

  nCoinCashSeparator = caCoinChangerDenomList.GetSize();

  trace(L9,_T("Setting Cash/Coin Changer Count List."));
  caCountList.RemoveAll();
  m_caSavedOriginalCountList.RemoveAll();
  for( int a = 0; a < caCoinChangerCountList.GetSize(); a++ )
    {caCountList.Add( caCoinChangerCountList[a] );}
  for( int b = 0; b < caCashChangerCountList.GetSize(); b++ )
    {caCountList.Add( caCashChangerCountList[b] );}
  m_caSavedOriginalCountList.Copy( caCountList );
  trace(L6,_T("CountList Size %d"), caCountList.GetSize());

  trace(L6,_T("-GenericDataInitialize()"));
}

///////////////////////////////////////////////////////////////////////////////
// Function:    ComputeTotals
// Description: based on CashManagementDisplayCount config option, we compute the
//              total count or value. We also compute the total change in count 
//              for each cash/coin acceptor/dispenser
///////////////////////////////////////////////////////////////////////////////////
void SMSmCashManagementBase::ComputeTotals()
{
  
  trace( L6, _T("+ComputeTotals") );

  nCashAcceptorTotalValue = nCoinAcceptorTotalValue = 0;
  nCashAcceptorTotalCount = nCoinAcceptorTotalCount = 0;
  nCashAcceptorTotalChangeInCount = nCoinAcceptorTotalChangeInCount = 0;
  nCoinDispenserTotalCount = nCashDispenserTotalCount = 0;
  nCoinDispenserTotalValue = nCashDispenserTotalValue = 0;
  nCashAcceptorTotalChangeInValue = nCoinAcceptorTotalChangeInValue = 0; //tar 299529

  int nIndexCheck = 0;

  try
  {
    for( int nAcceptorCount = 0; nAcceptorCount < m_caCashAcceptorChangeInCountList.GetSize(); nAcceptorCount++ )
    {
        nCashAcceptorTotalCount += m_caCashAcceptorCountList.GetAt( nAcceptorCount );
        nCashAcceptorTotalValue += ( m_caCashAcceptorCountList.GetAt( nAcceptorCount ) * 
                                            _ttol( m_caCashAcceptorDenomList.GetAt( nAcceptorCount ) ) );
      nCashAcceptorTotalChangeInCount += m_caCashAcceptorChangeInCountList.GetAt( nAcceptorCount );

      //tar 299529
      nCashAcceptorTotalChangeInValue  += ( m_caCashAcceptorChangeInCountList.GetAt( nAcceptorCount ) *
                                            _ttol( m_caCashAcceptorDenomList.GetAt( nAcceptorCount ) ) );
    }
    nIndexCheck++;
    for( int nAcceptorCount1 = 0; nAcceptorCount1 < m_caCoinAcceptorChangeInCountList.GetSize(); nAcceptorCount1++ )
    {
        nCoinAcceptorTotalCount += m_caCoinAcceptorCountList.GetAt( nAcceptorCount1 );
        nCoinAcceptorTotalValue += abs( m_caCoinAcceptorCountList.GetAt( nAcceptorCount1 ) *
                                            _ttol( m_caCoinAcceptorDenomList.GetAt( nAcceptorCount1 ) ) );
      nCoinAcceptorTotalChangeInCount += m_caCoinAcceptorChangeInCountList.GetAt( nAcceptorCount1 );

      //tar 299529
      nCoinAcceptorTotalChangeInValue  += ( m_caCoinAcceptorChangeInCountList.GetAt( nAcceptorCount1 ) *
                                            _ttol( m_caCoinAcceptorDenomList.GetAt( nAcceptorCount1 ) ) );

    }
    nIndexCheck++;
    for( int nAcceptorCount2 = 0; nAcceptorCount2 < nCoinCashSeparator; nAcceptorCount2++ )
    {
        nCoinDispenserTotalCount += caCountList.GetAt( nAcceptorCount2 );
        nCoinDispenserTotalValue += caCountList.GetAt( nAcceptorCount2 ) *
                                            abs( _ttol( caDispenserDenomList.GetAt( nAcceptorCount2 ) ) );
    }
    nIndexCheck++;
    for( int nAcceptorCount3 = nCoinCashSeparator; nAcceptorCount3 < caCountList.GetSize(); nAcceptorCount3++ )
    {
        nCashDispenserTotalCount += caCountList.GetAt( nAcceptorCount3 );
        nCashDispenserTotalValue += caCountList.GetAt( nAcceptorCount3 ) *
                                            abs( _ttol( caDispenserDenomList.GetAt( nAcceptorCount3 ) ) );
    }
    nIndexCheck++;
  }
  catch(...)
  {
    trace( L6, _T("UnExpected computation error : %d."), nIndexCheck );
  }
 trace( L6, _T("Total Counts and Value: CashAcceptor %d %d %d, CoinAcceptor %d %d %d, CashChanger %d %d, CoinChanger %d %d"),
    nCashAcceptorTotalCount, nCashAcceptorTotalChangeInCount, nCashAcceptorTotalChangeInValue, nCoinAcceptorTotalCount, nCoinAcceptorTotalChangeInCount,
    nCoinAcceptorTotalChangeInValue, nCashDispenserTotalCount, nCashDispenserTotalValue, nCoinDispenserTotalCount, nCoinDispenserTotalValue );


 trace( L6, _T("-ComputeTotals.") );
}

///////////////////////////////////////////////////////////////////////////////
// Function:    DataInitialize()
// Description: This is the main function that gets and processes all the data 
//              needed for display
// Returns:     true - if data has changed, and need repaint
///////////////////////////////////////////////////////////////////////////////////
bool SMSmCashManagementBase::DataInitialize()
{
  trace(L6,_T("+DataInitialize %d, %d, %d."), 
    m_bfFirstEntry, m_bGlory, co.fLocaleCashManagementAPPTrackCashCount );
  CString csCurrentCashCountsString;
  bool rc = true;


  if(m_bfFirstEntry && !m_bGlory && co.fLocaleCashManagementAPPTrackCashCount)
  {
        GenericDataInitialize();
        ComputeTotals();

  }
  else if( !m_bfFirstEntry && !m_bGlory && co.fLocaleCashManagementAPPTrackCashCount )
  {
//  	    trace(L6,_T("Nothing much to do."));
        //compare saved countlists to current count lists...
        ComputeTotals();
  }
  else if( !m_bGlory && !co.fLocaleCashManagementAPPTrackCashCount )
  {
  	    trace(L6,_T("Not supported"));
        //let's just switch first to CashManagementAppTrackCashCount=Y
        co.fLocaleCashManagementAPPTrackCashCount = true;
        DataInitialize();
  }
  else if( m_bGlory )
  {
    BSTR pCashCounts = SysAllocStringByteLen(NULL,200);
    BOOL pDiscrepancy = true;

    // we get BOTH cash and coin counts
    int n=0;
    while(dm.CashChangerGetCashCounts( &pCashCounts, &pDiscrepancy )!= OPOS_SUCCESS && n<30)  //device is not ready to give count, wait for 30 second
    {
	    trace(L0,_T("dm.CashChangerGetCashCounts() not ready n=%d."),n);
	    Sleep(1000);
	    n++;
    }
    if (n==30)
    {
	    trace(L0,_T("dm.CashChangerGetCashCounts() failed. "));
    }

    csCurrentCashCountsString = pCashCounts;

    CArray<int,int> caSavedCountList;
    caSavedCountList.Copy( caCountList );

    //collect Glory balance data
	csBalance = CString( pCashCounts );
	csBalance = _T("Coin=") + csBalance;
	int Index = csBalance.Find(_T(";"));
	if (Index >=0 )
	{
		try
		{
			csBalance.Insert(Index + 1, _T("Cash="));
		}
		catch (CMemoryException *e)
		{
			trace(L6, _T("ERROR:  CMemoryException() thrown."));
			e->Delete();
		}
	}

	//parse the pCashCounts
    ParseCashCount( CString( pCashCounts ) );
 
    for( int i = 0; i < caSavedCountList.GetSize(); i++ )
    {
      if( caCountList[i] != caSavedCountList[i] )
      {
        int nCountDelta = caCountList[i] - caSavedCountList[i];
        trace( L6, _T("Bill/Coin Inserted. Adding at index %d, counts : %d."), i, nCountDelta );
        m_caChangeInCount[i] += nCountDelta;
      }
    }

    n = 0;
    while(dm.CashAcceptorReadBillCounts( &pCashCounts) != OPOS_SUCCESS && n<30)  //device is not ready to give count, wait for 30 second
    {
	    trace(L0,_T("dm.CashAcceptorReadBillCounts() not ready n=%d."),n);
	    Sleep(1000);
	    n++;
    }
    if (n==30)
    {
	    trace(L0,_T("dm.CashAcceptorReadBillCounts() failed. "));
    }

    CString csCashCounts = pCashCounts;
    SysFreeString(pCashCounts);             // free the BSTR  //FZ160000

    trace(L6,_T("dm.CashAcceptorReadBillCounts() %s."), csCashCounts );

    nPurgeCnt = 0; //Reset it 

    while( !csCashCounts.IsEmpty() )
    {
      int nPosition = csCashCounts.Find( SEPARATOR );

      if( nPosition != -1 )
      {
        csCashCounts = csCashCounts.Mid( nPosition + ((CString)SEPARATOR).GetLength()  );
        nPurgeCnt += _ttol( csCashCounts );
      }
      else
      {
        csCashCounts.Empty();
      }
    }

    trace(L6,_T("Counts, Purge, Bill %s.. %s, %d, %d, %d, %d"), csCurrentCashCountsString, csSavedCashCountsString, 
      nPurgeCnt, nSavedPurgeCnt, m_bBill, m_bSavedBill );
    if(csCurrentCashCountsString == csSavedCashCountsString && nPurgeCnt == nSavedPurgeCnt && m_bBill == m_bSavedBill)  //Don't need reflesh screen since all counts are same
    {
	    rc= false;
	    trace(L6,_T("-DataInitialize rc= %d"), rc);
	    return rc;
    }
    else
    {
	    csSavedCashCountsString = csCurrentCashCountsString; //Should reset PurgeCnt?
	    m_bSavedBill = m_bBill;
    }

    if( m_bfFirstEntry )
    {
      nSavedPurgeCnt = nPurgeCnt;
    }
    else  //update the current lock box ChangeInCount
    {
      m_caChangeInCount[m_caChangeInCount.GetSize()-1] = nPurgeCnt - nSavedPurgeCnt;
    }

  }

  if( m_bfFirstEntry )
  {
    trace(L6,_T("Save Position of every column cylinder area")); // because we reset this to shift the image
    SaveOriginalCylinderPosition();
    m_caChangeInCount.RemoveAll(); // make sure we remove all elements
    int nSize = caCountList.GetSize();
    if( m_bGlory )
      {nSize++;}
    for( int i = 0; i < nSize; i++ ) // if glory, then let's add the change in count for purge bin
      {m_caChangeInCount.Add( 0 );}
    m_bfFirstEntry = false;
  }


  trace(L6,_T("-DataInitialize rc= %d"), rc);

  return rc;

}

///////////////////////////////////////////////////////////////////////////////
// Function:    SaveOriginalCylinderPosition()
// Description: Save the original Cylinder Area Position of each cylinder area
//              from the fastlane3.1.xml layout file. Saving these positions will
//              only be done once by scotapp. These position will be the basis
//              on how we compute the correct level of each bin.
///////////////////////////////////////////////////////////////////////////////////
void SMSmCashManagementBase::SaveOriginalCylinderPosition()
{
    trace(L6,_T("+SaveOriginalCylinderPosition %d."), m_caSavedOriginalColumnPositions.GetSize());

    if( m_caSavedOriginalColumnPositions.GetSize() == 0 )
    { // if there is already a saved cylinder position, then use that data
      int nColumnSize = MAXCOLUMNS;
      if( !m_bGlory )
      {
        nColumnSize = GENERICMAXCOLUMNS + 1;  // save the acceptor cylinder area
      }
      for( int i = 1; i <= nColumnSize; i++ )
      {
        //+ get cylinder data
        CString csChange = _T("SmCashManagementCylinderArea%d");

        if( !m_bGlory )
          {csChange = _T("SmCashManagementGenericCylinderArea%d");}
        csChange.Format( (CString)csChange,i);

        COleVariant vReturn;
        m_pAppPSX->GetConfigProperty( csChange, _T(""), UI::PROPERTYPOSITION, vReturn);

        CString csRet = vReturn.bstrVal;
        trace(L9,_T("Column %d, Saved Cylinder position : %s."), i, csRet);
        //-get cylinder data
        m_caSavedOriginalColumnPositions.Add( csRet );

      }
    }
    trace(L6,_T("-SaveOriginalCylinderPosition. Number of saved positions : %d"), 
      m_caSavedOriginalColumnPositions.GetSize());
}

///////////////////////////////////////////////////////////////////////////////
// Function:    ParseCashCounts                                        
// Description: Parses the CashCountList we got from cashchangergetcashcounts(),
//              and assigns the parse denomination and counts to caValueList,
//              and caCountList class members
// Parameters : csCashCounts - the CString that contains the cashcounts data
//              e.g. _T("1:200,5:160,10:150,25:20,50:0;100:100,500:150,1000:20")
///////////////////////////////////////////////////////////////////////////////////
void SMSmCashManagementBase::ParseCashCount( CString csCashCounts )
{
  trace(L6,_T("+ParseCashCount %s"), csCashCounts);
//  csCashCounts = _T("1:200,5:160,10:150,25:20,50:0;100:100,500:150,1000:20");

  int nTimes = -1;

  //make sure the value list and count list variables to store the csCashCounts is empty
  caValueList.RemoveAll();
  caCountList.RemoveAll();

//  csCashCounts = "1:200,5:160,10:150,25:20,50:0;100:200,500:150,1000:100";

  nCoinCashSeparator = 0;

  while( !csCashCounts.IsEmpty() )
  {
    if( csCashCounts.GetAt(0) == ';' )
    {
      nTimes = 1;
      
    }
  
    int nStrIndex = csCashCounts.Find( ':' );
    if( nStrIndex<=0) break;

    caValueList.Add( _ttol(csCashCounts) * nTimes );
  
    caCountList.Add( _ttol(csCashCounts.Mid(nStrIndex+1)) );

    if( m_bfFirstEntry )
      {m_caSavedOriginalCountList.Add( _ttol(csCashCounts.Mid(nStrIndex+1)) );}
    nStrIndex = csCashCounts.Find(',');

    if (nTimes == -1)
    {
          nCoinCashSeparator++;
          int nSemiColonIndex = csCashCounts.Find(';');
          if (nSemiColonIndex >= 0 && nSemiColonIndex < nStrIndex)
          {
              nStrIndex = nSemiColonIndex;
              nTimes = 1;   // if past the semi-colon the rest are positive bills
          }
      }

    if( nStrIndex != -1 )
    {
      csCashCounts = csCashCounts.Mid( nStrIndex + 1 );
    }
    else
    { // if we did not find the ',', then this is the last token
      csCashCounts.Empty();
    }
  }
  trace(L6,_T("-ParseCashCount(). Sizes. caValueList %d, CountList %d, OriginalCountList %d, Separator %d."), 
    caValueList.GetSize(), caCountList.GetSize(), m_caSavedOriginalCountList.GetSize(), nCoinCashSeparator);
}

///////////////////////////////////////////////////////////////////////////////
// Function:    ChangeAndShowContext
// Description: Handles context changes of SmLoadLift, SmIgnoreChanges, and the cash management contexts
///////////////////////////////////////////////////////////////////////////////////
void SMSmCashManagementBase::ChangeAndShowContext( CString csContext )
{
  trace(L6, _T("+ChangeAndShowContext() %s"), csContext );

  int nValueListSize = caValueList.GetSize();
  int nValue = 0;

  if( csContext == _T("SmIgnoreChanges") )
  {
  	ps.SetCMFrame(_T("SmIgnoreChanges"));

    if( m_bBill )
    {
      ps.SetLeadthruText( ps.GetPSText( MSG_CONFIRM_IGNORE_CHANGES_BILL ) );
    }
    else
    {
      ps.SetLeadthruText( ps.GetPSText( MSG_CONFIRM_IGNORE_CHANGES ) );
    }

    // be sure we enable the 'No' button
    m_pAppPSX->SetConfigProperty( _T("SMButton2"), ps.GetCurrentContext(), UI::PROPERTYSTATE, ( long ) UI::Normal);
    ps.ShowFrame();
  }
  else if( csContext == _T("SmShowPickupDumpFailure") )
  {
  	ps.SetCMFrame(_T("SmShowPickupDumpFailure"));

    ps.ShowFrame();
  }
  
  else if( csContext == _T("SmLoadLift") )
  {
      CString csWork, csRet, csDenom;
      int nCoinOrBill = 0;
      if( m_bBill )
        {nCoinOrBill = nCoinCashSeparator;}

      if ( (nLoadIndex + nCoinOrBill) < nValueListSize )
	  {
		  //+get denomination
		  if( caValueList[ nLoadIndex + nCoinOrBill] == 0 )
		  {
			csDenom = caDispenserDenomList[ nLoadIndex + nCoinOrBill ];
		  }
		  else
		  {
			csDenom = SMStateBase::ConvertValueToString( abs(caValueList[ nLoadIndex + nCoinOrBill ]), false );
		  }
		  //-get denomination
	  }
	  else
	  {
		  trace(L6, _T("SmLoadLift - Invalid index for caValueList"));
	  }

      if( ps.GetCurrentContext() == _T("SmLoadLift") )
      { // if current context is still loadlift, then input value was not accepted
        trace(L9, _T("Show error lead thru text"));
        PSClearKey();
        if( !co.fLocaleCashManagementDisplayCount )
          {csRet = ps.GetPSText( MSG_REENTER_VALUE );}
        else
          {csRet = ps.GetPSText( MSG_ENTER_LOAD_COUNT );}
      }
      else
      {
        trace(L6, _T("Shifting to loadlift context"));

        ps.SetCMFrame(_T("SmLoadLift"));
        ps.SetTitle(ps.GetPSText( MSG_LOAD ), _T("") );

        if ( (nLoadIndex + nCoinOrBill) < nValueListSize )
		{
		    nValue = caValueList[ nLoadIndex + nCoinOrBill];
		}
		else
		{
			trace(L6, _T("Invalid index for caValueList"));
		}

        if( co.fLocaleCashManagementDisplayCount || nValue == 0 )
        {
          ps.AllowInput(9,false,false);
          csRet = ps.GetPSText( MSG_ENTER_LOAD_COUNT );
        }
        else
        {
          ps.AllowInput(9,false,true);
          csRet = ps.GetPSText( MSG_ENTER_LOAD_VALUE );
        }
        ps.SetButtonText(_T("SMButton1"),MSG_FULL_REPLACEMENT);

      }
      csWork.Format( csRet, csDenom );
      ps.SetLeadthruText( csWork );
      ps.ShowFrame();
  
  }
  //+TAR323136
  else if( csContext == _T("SmExcessLoadAmount") )
  {
	  int nCoinOrBill = 0;
      if( m_bBill )
	  {nCoinOrBill = nCoinCashSeparator;}
	  
	  CString csWork, csRet;
	  ps.SetCMFrame(_T("SmExcessLoadAmount"));
	  if(co.fLocaleCashManagementDisplayCount)
	  {
		  csRet = ps.GetPSText( MSG_EXCESS_LOAD_COUNT );
		  csWork.Format(csRet, m_nMaxAmtNeeded, m_nMaxAmtNeeded);
	  }
	  else
	  {
		  if ( (nLoadIndex + nCoinOrBill) < nValueListSize )
		  {
			  nValue = caValueList[ nLoadIndex + nCoinOrBill];
		  }
		  else
		  {
			  trace(L6, _T("SmExcessLoadAmount - Invalid index for caValueList"));
		  }
		  int n = m_nMaxAmtNeeded * nValue;
		  CString csAmt = SMStateBase::ConvertValueToString(n,false);

		  csRet = ps.GetPSText( MSG_EXCESS_LOAD_AMT );
		  csWork.Format(csRet, (LPCTSTR)csAmt, (LPCTSTR)csAmt);
	  }
	  ps.SetLeadthruText(csWork);
	  ps.ShowFrame();
  }
  //-TAR323136
  else
  {
	  if(DataInitialize())   //If return true, lrefresh the screen.
	  {
		  showScreen();
	  }
    SetDevices();
  }
  trace(L6, _T("-ChangeAndShowContext()") );
}

///////////////////////////////////////////////////////////////////////////////
// Function:    SetDevices()
// Description: After ShowScreen, this function will be called to set correct
//              states of the devices
///////////////////////////////////////////////////////////////////////////////////
bool SMSmCashManagementBase::SetDevices()
{
  trace( L6, _T("+SetDevices") );
  bool brc = true;


/*
  commented - don't do this here
  CString csHigh, csCoinDispenserAlert, csCashDispenserAlert;
  CString csLow = SMStateBase::DMCheckCashCounts( false, csHigh, csCoinDispenserAlert, csCashDispenserAlert );

  //be sure only green light is on 
  dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                      DM_TRICOLORLIGHT_OFF,
                      DM_TRICOLORLIGHT_NORMALREQUEST);
  dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                      DM_TRICOLORLIGHT_OFF,
                      DM_TRICOLORLIGHT_NORMALREQUEST);
  
  DMTriColorLightState dmGreenState = DM_TRICOLORLIGHT_ON;
  if( dm.GetCoinsOrBillsVeryLow() )
  {
    dmGreenState = DM_TRICOLORLIGHT_BLINK_1HZ;
  }
  dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
                      dmGreenState,
                      DM_TRICOLORLIGHT_NORMALREQUEST);
*/
  if( m_bGlory )
  {
    DMCashAcceptorEnable(co.csTenderDenominations);
    DMCoinAcceptorEnable();
  }

  trace( L6, _T("-SetDevices %d."), brc );
  return brc;
}


///////////////////////////////////////////////////////////////////////////////
// Function:    showScreen()
// Description: This is the main function that sets the UI layout
///////////////////////////////////////////////////////////////////////////////////
void SMSmCashManagementBase::showScreen()
{
   trace(L6,_T("+showScreen %d. %d. %d. %d. %d."), m_bBill, caValueList.GetSize(),
     caCountList.GetSize(), m_caChangeInCount.GetSize(), caDispenserDenomList.GetSize() );

   // let's set to the correct context first
  
   if( m_bGlory )
    {ps.SetCMFrame(_T("SmCashManagement"));}
   else
    {ps.SetCMFrame(_T("SmGenericCashManagement"));}
   if( m_bBill )
   {
      //set coin device UI
     ps.SetTitle(ps.GetPSText( MSG_CASH_MANAGEMENT ), _T("") );
     ps.SetLeadthruText(MSG_INSERT_OR_UNLOAD_CASH);

//     ps.SetButtonText(_T("SMButton1"),MSG_COIN_STATUS);
     ps.SetButtonText(_T("SMButton1"),MSG_COIN_MANAGEMENT);

   }
   else
   {
     //set cash device UI
     ps.SetTitle(ps.GetPSText( MSG_COIN_MANAGEMENT ), _T("") );
     ps.SetLeadthruText(MSG_INSERT_OR_UNLOAD_COIN);

//     ps.SetButtonText(_T("SMButton1"),MSG_BILL_STATUS);
     ps.SetButtonText(_T("SMButton1"),MSG_CASH_MANAGEMENT);

   }
   if( m_bGlory )
   {

     if( m_bBill )
     {
       ps.SetButtonText(_T("SMButton2"), MSG_AUTHENTICATE);
     }
     else
     {
       ps.SetButtonText(_T("SMButton2"), MSG_REMOVE_COINS);
     }

     if( m_bfAuthenticated || ( !co.fLocaleAllowSoftwareRemoval && !co.fLocaleAllowSoftwareUnload ) )
     { // if not authenticated, or allow removal And unload through the cash management screen
        m_pAppPSX->SetConfigProperty( _T("SMButton2"), ps.GetCurrentContext(), UI::PROPERTYSTATE, ( long ) UI::Disabled);
     }
     else
     { // enable the button
        m_pAppPSX->SetConfigProperty( _T("SMButton2"), ps.GetCurrentContext(), UI::PROPERTYSTATE, ( long ) UI::Normal);
     }
   }
   else
   {
     ps.SetLeadthruText(MSG_BILL_COIN_MANAGEMENT_LOAD_PICKUP);
   }


  COleVariant v1, vColumnVisible, vReturn;
  COleVariant vRemoveButtonVisible, vUnloadButtonVisible, vLoadButtonVisible;
  CString vDenom, vChangeInCount, vCountOrValue;
  long vUnloadButtonState, vRemoveButtonState, vLoadButtonState;

  int nCoinOrBill = 0; //use this to process the correct index
                       //if coin, then 0, but if bill, then
                       //correct index is chosen index+nCoinCashSeparator
  if( m_bBill )
    nCoinOrBill = nCoinCashSeparator;

  //+ show dispenser column data

  int nDispenserCylinders = MAXCOLUMNS;

  if( !m_bGlory ) // if not glory, then maximum cylinder columns is 8
    {nDispenserCylinders = GENERICMAXCOLUMNS;}

  for(long i = 0; i < nDispenserCylinders; i++ ) 
	{
    v1 = i;

    int iChangeInCount = 0, nRemoveAmount = 0;
    vChangeInCount = _T("%d");
    vDenom = _T("$%d");
    vUnloadButtonState = (long)UI::Normal;
    vRemoveButtonState = (long)UI::Normal;
    vLoadButtonState = (long)UI::Normal;
    vRemoveButtonVisible = VARIANT_TRUE;
    vUnloadButtonVisible = VARIANT_TRUE;
    vLoadButtonVisible = VARIANT_TRUE;
    vColumnVisible = VARIANT_TRUE;
    

    trace(L6,_T("Column Number : %d"), i);

    if( m_bBill && m_bGlory && ( i + nCoinOrBill == caValueList.GetSize() ) ) // special case for lock boxs
      { // if we are at lock box, do specific processing( should lock box always be present?)
        trace(L6,_T("We are at lock box cylinder."));

        //iChangeInCount = nPurgeCnt;
        iChangeInCount = m_caChangeInCount[m_caChangeInCount.GetSize() - 1]; // gets the last element
        vRemoveButtonState = vUnloadButtonState = (long) UI::Disabled;
        vUnloadButtonVisible = VARIANT_FALSE;
        vRemoveButtonVisible = VARIANT_FALSE;
        vDenom = ps.GetPSText( MSG_LOCKBOX );
        
      }
      else if( i + nCoinOrBill >= caValueList.GetSize() )
      { //if computed index is greater or equal to size of valuelist, then 
        //do not display this area
        trace(L6,_T("Not in the list, button at cylinder %d must not be visible"), i );
        vUnloadButtonVisible = VARIANT_FALSE;
        vRemoveButtonVisible = VARIANT_FALSE;
        vLoadButtonVisible = VARIANT_FALSE;
        vColumnVisible = VARIANT_FALSE;
      }
      else if( !m_bBill && ( i >= nCoinCashSeparator || i >= caValueList.GetSize() ) )
      { // additional checking
        trace(L6,_T("Expected Coin, not bill. button at cylinder %d must not be visible"), i );
        vUnloadButtonVisible = VARIANT_FALSE;
        vRemoveButtonVisible = VARIANT_FALSE;
        vLoadButtonVisible = VARIANT_FALSE;
        vColumnVisible = VARIANT_FALSE;
      }
      else if( caValueList[ i + nCoinOrBill ] == 0 )
      {
        //for tesco feature, non-denomination denomination is possible
        if( !m_bGlory && caDispenserDenomList[ i + nCoinOrBill ] != EMPTY_STRING )
        { // check to see if we have a non-numeric denomination
          trace(L6,_T("Cylinder %d has non-numeric denomination"), i );
          vDenom = caDispenserDenomList[ i + nCoinOrBill ];
          iChangeInCount = m_caChangeInCount[i + nCoinOrBill];
        }
        else
        { 
          trace(L6,_T("no denomination at this column : %d"), i );
          vUnloadButtonVisible = VARIANT_FALSE;
          vRemoveButtonVisible = VARIANT_FALSE;
          vLoadButtonVisible = VARIANT_FALSE;
          vColumnVisible = VARIANT_FALSE;
        }
      }
      else if( caCountList[ i + nCoinOrBill ] == 0 ) // if zero count or empty, disable the remove, unload button
      {
        trace(L6,_T("Cylinder %d empty. disable buttons"), i );
        vUnloadButtonState = (long) UI::Disabled;
        vRemoveButtonState = (long) UI::Disabled;
        nRemoveAmount = co.nCashRemoveAmount[ i + nCoinOrBill];
        vDenom = SMStateBase::ConvertValueToString( abs(caValueList[ i+ nCoinOrBill ]), !m_bGlory && m_bBill );
        iChangeInCount = m_caChangeInCount[i+nCoinOrBill];
      }
      else
      { //everything's ok
        nRemoveAmount = co.nCashRemoveAmount[ i + nCoinOrBill];
        vDenom = SMStateBase::ConvertValueToString( abs(caValueList[ i+ nCoinOrBill ]), !m_bGlory && m_bBill );
        iChangeInCount = m_caChangeInCount[i+nCoinOrBill];
      }


    if (!SMSmCashManagementBase::m_bfAuthenticated)
    { //if not authenticated, then disable the unload, and remove button
      vUnloadButtonState = vRemoveButtonState = vLoadButtonState = (long) UI::Disabled;
    }

    if( !co.fLocaleAllowSoftwareRemoval )
    {
      vRemoveButtonState = (long) UI::Disabled;
    }
    if( !co.fLocaleAllowSoftwareUnload )
    {
      vUnloadButtonState = (long) UI::Disabled;
    }
 
    caLastCountList.Copy( caCountList ); // lets save the last count

    CString csCylinderImagePath;

    CString csNewCylinderPosition = GetNewCylinderPosition( i, vColumnVisible, nCoinOrBill, NULL, csCylinderImagePath );

    //+set dispenser count or value field
    int iValue;
    if( !m_bGlory )
    {
      if( ( vColumnVisible.boolVal == VARIANT_TRUE ) && !m_bGlory && !co.fLocaleCashManagementDisplayCount )
      { // display by value
        iValue = ( abs( caValueList[i+nCoinOrBill] )  ) * caCountList[i+nCoinOrBill];
        vCountOrValue = SMStateBase::ConvertValueToString( iValue, !m_bGlory && m_bBill );
        int nChangeInValue = abs(caValueList[ i+ nCoinOrBill ]) * iChangeInCount;
        vChangeInCount = SMStateBase::ConvertValueToString( nChangeInValue, !m_bGlory && m_bBill );
      }
      else if( vColumnVisible.boolVal == VARIANT_TRUE )
      { //display by count
        iValue = caCountList[i+nCoinOrBill];
        vCountOrValue.Format( _T("%d"), iValue );
        vChangeInCount.Format((CString) vChangeInCount, iChangeInCount );
      }
    }
    else
    {
      vChangeInCount.Format((CString) vChangeInCount, iChangeInCount );
    }
    if( iChangeInCount > 0 )
    {
      vChangeInCount = _T("+") + vChangeInCount;
    }
    //-set dispenser count or value field
    
    ShowColumn( i+ 1, vColumnVisible.boolVal, vCountOrValue, vChangeInCount, 
                csCylinderImagePath, csNewCylinderPosition, vDenom, 
                vUnloadButtonState,vRemoveButtonState, nRemoveAmount, 
                vLoadButtonState,vUnloadButtonVisible.boolVal, 
                vRemoveButtonVisible.boolVal, vLoadButtonVisible.boolVal);
    //- assign new data to cylinder
  }
  //- show dispenser column data
  ShowAcceptorAndGenericData();

  m_pAppPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_TRUE );

  ps.ShowFrame();

  trace(L6,_T("-showScreen"));

}

///////////////////////////////////////////////////////////////////////////////////
// Function:    ShowAcceptorAndGenericData()
// Description: This function displays the acceptor datas, additional labels,
//              and Totals, and also sets the correct states of the buttons of
//              the generic screen
// Calls :      ShowAcceptorColumns
//              ShowAcceptorCylinder
//              ShowTotals
///////////////////////////////////////////////////////////////////////////////////
void SMSmCashManagementBase::ShowAcceptorAndGenericData()
{
  trace( L6, _T("+ShowAcceptorAndGenericData %d"), m_bBill );
  //+show more display  for tesco
  int *nAcceptorTotalCount, *nAcceptorTotalValue, *nAcceptorTotalChangeInCount, *nDispenserTotalCount;
  int *nAcceptorTotalChangeInValue; //tar 299529
  int nCoinOrBill,*nDispenserTotalValue;

  if( !m_bGlory )
  {
    //+show Acceptor Columns
    CArray<int,int> *caAcceptorCount, *caAcceptorChangeInCount;
    CArray<CString,CString> * caAcceptorDenom;
    
    //set correct data for current view
    if( m_bBill )
    {
      caAcceptorCount = &m_caCashAcceptorCountList;
      caAcceptorDenom = &m_caCashAcceptorDenomList;
      caAcceptorChangeInCount = &m_caCashAcceptorChangeInCountList;
      nAcceptorTotalCount = &nCashAcceptorTotalCount;
      nAcceptorTotalValue = &nCashAcceptorTotalValue;
      nAcceptorTotalChangeInCount = &nCashAcceptorTotalChangeInCount;
      nAcceptorTotalChangeInValue = &nCashAcceptorTotalChangeInValue; //tar 299529
      nDispenserTotalCount = &nCashDispenserTotalCount;
      nDispenserTotalValue = &nCashDispenserTotalValue;
      nCoinOrBill = nCoinCashSeparator;
    }
    else
    {
      caAcceptorCount = &m_caCoinAcceptorCountList;
      caAcceptorDenom = &m_caCoinAcceptorDenomList;
      caAcceptorChangeInCount = &m_caCoinAcceptorChangeInCountList;
      nAcceptorTotalCount = &nCoinAcceptorTotalCount;
      nAcceptorTotalValue = &nCoinAcceptorTotalValue;
      nAcceptorTotalChangeInCount = &nCoinAcceptorTotalChangeInCount;
      nAcceptorTotalChangeInValue = &nCoinAcceptorTotalChangeInValue; //tar 299529
      nDispenserTotalCount = &nCoinDispenserTotalCount;
      nDispenserTotalValue = &nCoinDispenserTotalValue;
      nCoinOrBill = 0;
    }

    VARIANT_BOOL vColumnVisible;
    CString csCountOrValue, csDenom;
    for( int nCount = 0; nCount < GENERICMAXCOLUMNS; nCount++ )
    {
      trace(L6,_T("AcceptorColumn : %d AcceptorDenom %d AcceptorCount %d "), 
        nCount, caAcceptorDenom->GetSize(), caAcceptorCount->GetSize() );
      
      vColumnVisible = VARIANT_FALSE;
      csCountOrValue = EMPTY_STRING;
      csDenom = EMPTY_STRING;
      if( nCount < caAcceptorCount->GetSize() )
      {
        vColumnVisible = VARIANT_TRUE;
        if( _ttol( caAcceptorDenom->GetAt( nCount ) ) == 0 )
        {
          trace(L6,_T("Non-Numeric denomination"));
          csDenom = caAcceptorDenom->GetAt( nCount );
        }
        else
        { 
          csDenom = SMStateBase::ConvertValueToString( abs(_ttol( caAcceptorDenom->GetAt( nCount ) )), !m_bGlory && m_bBill );
        }

        if( !co.fLocaleCashManagementDisplayCount )
        { // display by value
          int dValue = ( abs( _ttol( caAcceptorDenom->GetAt( nCount ) ) ) ) * caAcceptorCount->GetAt(nCount);
          trace(L6,_T("Show Value : %d"), dValue);
          csCountOrValue = SMStateBase::ConvertValueToString( dValue, !m_bGlory && m_bBill );

        }
        else
        { // display by count
          csCountOrValue.Format( _T("%d"), caAcceptorCount->GetAt(nCount) );
        }
      }
      ShowAcceptorColumns( nCount+1, vColumnVisible, csCountOrValue, csDenom );
    }
    //-show Acceptor Columns

    //+show totals, and acceptor cylinder
    CString csAcceptorTotal, csDispenserTotal, csGrandTotal;

    trace( L6, _T("AcceptorTotal : %d. DispenserTotal : %d. GrandTotal : %d."), 
      *nAcceptorTotalCount, *nDispenserTotalCount, *nAcceptorTotalCount + *nDispenserTotalCount );
    if( co.fLocaleCashManagementDisplayCount )
    { //display by count
      csAcceptorTotal.Format( _T("%d"), *nAcceptorTotalCount );
      csDispenserTotal.Format( _T("%d"), *nDispenserTotalCount );
      csGrandTotal.Format( _T("%d"), *nAcceptorTotalCount + *nDispenserTotalCount );
    }
    else
    { // display by value
      csAcceptorTotal = SMStateBase::ConvertValueToString( *nAcceptorTotalValue, !m_bGlory && m_bBill );
      csDispenserTotal = SMStateBase::ConvertValueToString( *nDispenserTotalValue, !m_bGlory && m_bBill );
      csGrandTotal = SMStateBase::ConvertValueToString( *nAcceptorTotalValue + *nDispenserTotalValue, !m_bGlory && m_bBill );
    }
    CString csAcceptorTotalChangeInCount, csCylinderImagePath, csNewCylinderPosition;


    if( co.fLocaleCashManagementDisplayCount )
    {
      csAcceptorTotalChangeInCount.Format(_T("%d"), *nAcceptorTotalChangeInCount);
    }
    else
    {
      //tar 299529
      csAcceptorTotalChangeInCount = SMStateBase::ConvertValueToString( *nAcceptorTotalChangeInValue, !m_bGlory && m_bBill );
    }
    
    csNewCylinderPosition = GetNewCylinderPosition( ACCEPTORDEVICECYLINDERINDEX, VARIANT_TRUE, nCoinOrBill, NULL, csCylinderImagePath );
    trace( L9, _T("Getting cylinder data for Acceptor Cylinder"));

    //show acceptor cylinder
    ShowAcceptorCylinder( csAcceptorTotal, csAcceptorTotalChangeInCount, csCylinderImagePath, csNewCylinderPosition );
    
    ShowTotals( csAcceptorTotal, csDispenserTotal, csGrandTotal );
    //-show totals and acceptor cylinder

    //set correct button state for tesco screen
    if( ps.GetCurrentContext() == _T("SmGenericCashManagement") )
    {
    //+disable/enable pickup buttons
      if( *nAcceptorTotalCount == 0 )
		 //Tar344136
        {m_pAppPSX->SetConfigProperty( _T("SMButton2"), ps.GetCurrentContext(), UI::PROPERTYSTATE, ( long ) UI::Disabled);}
	  else
		//Tar344136
        {m_pAppPSX->SetConfigProperty( _T("SMButton2"), ps.GetCurrentContext(), UI::PROPERTYSTATE, ( long ) UI::Normal);}

      if( *nDispenserTotalCount == 0 )
		  //Tar344136
        {m_pAppPSX->SetConfigProperty( _T("SMButton3"), ps.GetCurrentContext(), UI::PROPERTYSTATE, ( long ) UI::Disabled);}
	  else
		  //Tar344136
        {m_pAppPSX->SetConfigProperty( _T("SMButton3"), ps.GetCurrentContext(), UI::PROPERTYSTATE, ( long ) UI::Normal);}
      //-disable/enable pickup buttons

      //+disable/enable ignore/cancel changes button
      bool *bHasMadeChanges;
      if( m_bBill )
        {bHasMadeChanges = &m_bBillHasMadeChanges;}
      else
        {bHasMadeChanges = &m_bCoinHasMadeChanges;}

      if( *bHasMadeChanges )
		  //Tar344136
        {m_pAppPSX->SetConfigProperty( _T("SMButton4"), ps.GetCurrentContext(), UI::PROPERTYSTATE, ( long ) UI::Normal);}
      else
		  //Tar344136
        {m_pAppPSX->SetConfigProperty( _T("SMButton4"), ps.GetCurrentContext(), UI::PROPERTYSTATE, ( long ) UI::Disabled);}
      //-disable/enable ignore/cancel changes button
    }
  }
  //-show more display  for tesco
  trace( L6, _T("-ShowAcceptorAndGenericData") );
}

///////////////////////////////////////////////////////////////////////////////
// Function:    GetNewCylinderPosition
// Description: Gets the new cylinder position and image based on the current count
//              of the bin relative to that bin's capacity
// Parameters : i - the bin index
//              vColumnVisible - column will be shown, or not
//              nCoinOrBill - the coin and cash changer separator for the caCountList
//              csImage - out parameter
// Return     : returns the new cylinder position as CString
//              assigns the csImage parameter with the correct color of the cylinder
///////////////////////////////////////////////////////////////////////////////////
CString SMSmCashManagementBase::GetNewCylinderPosition( int i, COleVariant vColumnVisible, int nCoinOrBill, VARIANT_BOOL fVisible, CString& csImage )
{

    trace(L6,_T("+GetNewCylinderPosition %d, %d, %d, %d, %d, %d, %d, %d, %d"), i, vColumnVisible.boolVal, nCoinOrBill, 
      caValueList.GetSize(), caCountList.GetSize(), co.nCashCapacity.GetSize(), co.nCashRemoveAmount.GetSize(),
      co.nCashLowList.GetSize(), co.nCashHighList.GetSize() );
    COleVariant vReturn;

    //+get cylinder data
    CString csRet = m_caSavedOriginalColumnPositions[i]; //retrieve the saved position


    int nX[2] = {0}, nY[2] = {0};

#ifdef _UNICODE
    swscanf(	csRet, _T("%d,%d,%d,%d"), &nX[0], &nY[0], &nX[1], &nY[1] );
#else
    _stscanf(	csRet, _T("%d,%d,%d,%d"), &nX[0], &nY[0], &nX[1], &nY[1] );

#endif

    int nHeightOfBottom = HEIGHTOFBOTTOMDIAMETER;
    if( !m_bGlory )
    {
      if( i == ACCEPTORDEVICECYLINDERINDEX ) // if acceptor cylinder
        {nHeightOfBottom = GENERICHEIGHTOFBOTTOMACCEPTORDIAMETER;}
      else
        {nHeightOfBottom = GENERICHEIGHTOFBOTTOMDISPENSERDIAMETER;}  
    }

    int nBottomPosition = nY[0]+nY[1] - nHeightOfBottom; //compute our limit
    int nLengthOfCylinder = nY[1];
    //-get cylinder data

    //+assign new data to cylinder
    CString csNewCylinderPosition = _T("%d,%d,%d,%d");
    CString csCylinderImagePath;

    int nCount;

    int nCapacity,nHighThreshold, nLowThreshold;

    if( i+nCoinOrBill > caCountList.GetSize() || i+nCoinOrBill > co.nCashCapacity.GetSize() ||
        i+nCoinOrBill > co.nCashHighList.GetSize() || i+nCoinOrBill > co.nCashLowList.GetSize() )
    { //missing value in scotopts.
      trace( L6, _T("Missing value in scotopts. %d, %d, %d, %d, %d, %d."), i, nCoinOrBill,
        caCountList.GetSize(), co.nCashCapacity.GetSize(), co.nCashHighList.GetSize(),
        co.nCashLowList.GetSize());
    }
    if( ( vColumnVisible.boolVal == VARIANT_TRUE ) )
    {
//        trace(L6,_T("caCountList.GetSize() : %d. co.nCashCapacity.GetSize() : %d."), caCountList.GetSize(), co.nCashCapacity.GetSize() );
        if( i == ACCEPTORDEVICECYLINDERINDEX ) // we expect this to be the acceptor cylinder
        {
          if( m_bBill )
          {
            nCount = nCashAcceptorTotalCount;
            nCapacity = co.nLocaleCashAcceptorCapacity;
            nHighThreshold = co.nLocaleCashAcceptorHighThreshold;
            nLowThreshold = 0;  
          }
          else
          {
            nCount = nCoinAcceptorTotalCount;
            nCapacity = co.nLocaleCoinAcceptorCapacity;
            nHighThreshold = co.nLocaleCoinAcceptorHighThreshold;
            nLowThreshold = 0;  
          }
        }
        else if( m_bGlory && m_bBill && ( i + nCoinOrBill >= caValueList.GetSize() ) )
        { // expect to be purge
          nCount = nPurgeCnt;
          nCapacity = co.nCashCapacity[co.nCashCapacity.GetSize()-1];
          nHighThreshold = co.nCashHighList[co.nCashHighList.GetSize()-1];
          nLowThreshold = co.nCashLowList[co.nCashLowList.GetSize()-1];  // won't be evaluated
        }
        else
        { //dispenser data
          nCount = caCountList[i+nCoinOrBill];
          nCapacity = co.nCashCapacity[i+nCoinOrBill];
          nHighThreshold = co.nCashHighList[i+nCoinOrBill];
          nLowThreshold = co.nCashLowList[i+nCoinOrBill];
        }
        trace(L6,_T("Count %d, capacity %d, high %d, low %d."), nCount, nCapacity,
          nHighThreshold,nLowThreshold );

        double dPercentage;
        if( nCount == 0 || 
          nCount > nCapacity )
        {
          dPercentage = 1;
        }
        else
        {
          dPercentage = ((double)nCount / nCapacity); //percentage compute, count / capacity
        }
        trace(L9,_T("Count Percentage %f"), dPercentage);

        int nSupposedLengthOfCylinder = ( nLengthOfCylinder - nHeightOfBottom) * dPercentage; // ok to disregard to decimals. 

        int nBottomLimit = nBottomPosition ;//- SHOWMINIMUMCOUNT;

        int nNewYStartingPosition = nBottomPosition - nSupposedLengthOfCylinder; // the new vertical position starting point.

        trace(L9,_T("Bottom Limit : %d, new starting point : %d"), nBottomLimit, nNewYStartingPosition);

        if( nNewYStartingPosition > nBottomLimit )
        { //if the new starting position is greater than the limit, reset the new position to the limit
          nNewYStartingPosition = nBottomLimit;
        }
        trace(L9,_T("length of cylinder : %d, new starting point : %d"), nLengthOfCylinder, nNewYStartingPosition);


        if( m_bGlory )
        { // let's not touch this
          if( nCount == 0 )
          { // if empty, don't show full red cylinder, but only bottom. same for purge
            csNewCylinderPosition.Format( (CString) csNewCylinderPosition, nX[0], nBottomLimit, nX[1], nLengthOfCylinder );
          }
          else
          { 
            csNewCylinderPosition.Format( (CString) csNewCylinderPosition, nX[0], nNewYStartingPosition, nX[1], nLengthOfCylinder );
          }
          //+ assign color
          if( (!(m_bBill && ( i + nCoinOrBill >= caValueList.GetSize() ) ) && ( nCount == 0 || nCount >= nCapacity) ) || // if full or empty, not purge
                ( (m_bBill && ( i + nCoinOrBill >= caValueList.GetSize() ) ) && ( nCount >= nCapacity) ) ) //if purge and full( empty purge is ok?)
          { 
            m_pAppPSX->GetConfigProperty( _T("SmCashManagementRedFill"), _T(""), UI::PROPERTYPICTURE, vReturn);
          }
          else if( (!(m_bBill && ( i + nCoinOrBill >= caValueList.GetSize() ) ) && ( nCount > 0 &&         // if not purge &&
                     nCount <= nLowThreshold ) ||     // if low ||
                   ( nCount < nCapacity &&      // or high 
                     nCount > nHighThreshold ) ) ||  
                   ((m_bBill && ( i + nCoinOrBill >= caValueList.GetSize() ) ) &&                            // if purge &&
                   ( nCount < nCapacity &&      // if high
                     nCount >= nHighThreshold ) ) ) 
          { 
            m_pAppPSX->GetConfigProperty( _T("SmCashManagementYellowFill"), _T(""), UI::PROPERTYPICTURE, vReturn);
          }
          else
          { 
            m_pAppPSX->GetConfigProperty( _T("SmCashManagementGreenFill"), _T(""), UI::PROPERTYPICTURE, vReturn);
          }
        }
        else
        {
          if( nCount <= 0 )
          { // if empty, don't show full red cylinder, but only bottom. same for purge
            csNewCylinderPosition.Format( (CString) csNewCylinderPosition, nX[0], nBottomLimit, nX[1], nLengthOfCylinder );
          }
          else
          { 
            csNewCylinderPosition.Format( (CString) csNewCylinderPosition, nX[0], nNewYStartingPosition, nX[1], nLengthOfCylinder );
          }
          //+ assign color
          
          if( i == ACCEPTORDEVICECYLINDERINDEX )
          { // acceptor cylinder
            if( nCount >= nCapacity )
            {
              m_pAppPSX->GetConfigProperty( _T("SmCashManagementAcceptorRedFill"), _T(""), UI::PROPERTYPICTURE, vReturn);
            }
            else if( nCount < nCapacity && nCount >= nHighThreshold )
            {
              m_pAppPSX->GetConfigProperty( _T("SmCashManagementAcceptorYellowFill"), _T(""), UI::PROPERTYPICTURE, vReturn);
            }
            else
            {
              m_pAppPSX->GetConfigProperty( _T("SmCashManagementAcceptorGreenFill"), _T(""), UI::PROPERTYPICTURE, vReturn);
            }
          }
          else if( nCount <= 0 ) // if empty
          { 
            m_pAppPSX->GetConfigProperty( _T("SmCashManagementRedFill"), _T(""), UI::PROPERTYPICTURE, vReturn);
          }
          else if( nCount > 0 && nCount <= nLowThreshold ) 
          { 
            m_pAppPSX->GetConfigProperty( _T("SmCashManagementYellowFill"), _T(""), UI::PROPERTYPICTURE, vReturn);
          }
          else
          { 
            m_pAppPSX->GetConfigProperty( _T("SmCashManagementGreenFill"), _T(""), UI::PROPERTYPICTURE, vReturn);
          }
        }
    }
    else
    {
      //overlap the background image
      csNewCylinderPosition.Format( (CString) csNewCylinderPosition, nX[0]-5, nY[0]-5, nX[1]+10, nY[1]+10 );
      m_pAppPSX->GetConfigProperty( _T("SmCashManagementWhiteMask"), _T(""), UI::PROPERTYPICTURE, vReturn);
    }

  csImage = vReturn.bstrVal;
  trace(L6,_T("-GetNewCylinderPosition() %s, %s"), csNewCylinderPosition, csImage);

  return csNewCylinderPosition;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    PSButton1                                        
// Description: if current context is :
//              SmLoadLift      - do a full replace
//              SmIgnoreChanges - Ignores the changes made to current screen
//              SmGenericCashManagement, SmCashManagement - switch to coin/cash screen
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSmCashManagementBase::PSButton1(void)
{
  trace(L6,_T("+PSButton1"));

  if( ps.GetCurrentContext() == _T("SmLoadLift") )
  { 
    LoadAmount( nLoadIndex, 0, true );

    ChangeAndShowContext();

    return STATE_NULL;
  }
  else if(ps.GetCurrentContext() == _T("SmShowPickupDumpFailure") )
  {
      return PSButton3();
  }
  else if( ps.GetCurrentContext() == _T("SmIgnoreChanges") )
  {
    IgnoreChanges();
    if( m_bBill )
      {m_bBillHasMadeChanges = false;}
    else
      {m_bCoinHasMadeChanges = false;}

    if( DataInitialize() )
    {
      showScreen();
    }
    SetDevices();

    return STATE_NULL;
  }
  //+TAR323136
  else if( ps.GetCurrentContext() == _T("SmExcessLoadAmount") )
  {
	  ChangeAndShowContext();
	  return STATE_NULL;
  }
  //-TAR323136

  m_bBill = !m_bBill;

  if( DataInitialize() )    //If return true, refresh the screen.
  {
    showScreen();
  }
  SetDevices();

  trace(L6,_T("-PSButton1 %d"), m_bBill );

  return STATE_NULL;

}

///////////////////////////////////////////////////////////////////////////////
// Function:    PSButton2                                        
// Description: if current context is :
//              SmIgnoreChanges - just go back to cashmanagement context
//              SmGenericCashManagement - acceptor pickup
//              SmCashManagement - Authenticate screen
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSmCashManagementBase::PSButton2(void)
{
  trace(L6,_T("+PSButton2"));

  if( ps.GetCurrentContext() == _T("SmIgnoreChanges") )
  { // just go back to cashmanagement context

    ChangeAndShowContext();

    return STATE_NULL;
  }

  if( !m_bGlory )
  {
    CArray<int,int> *caAcceptorCountList, *caAcceptorChangeInCountList;

    if( m_bBill )
    {
      caAcceptorCountList = &m_caCashAcceptorCountList;
      caAcceptorChangeInCountList = &m_caCashAcceptorChangeInCountList;
      m_bBillHasMadeChanges = true;
    }
    else
    {
      caAcceptorCountList = &m_caCoinAcceptorCountList;
      caAcceptorChangeInCountList = &m_caCoinAcceptorChangeInCountList;
      m_bCoinHasMadeChanges = true;
    }

    for( int i = 0; i < caAcceptorCountList->GetSize(); i++ )
    { //reset the target device's count list to 0.
      int nAcceptorChangeInCount = caAcceptorChangeInCountList->GetAt(i) - caAcceptorCountList->GetAt(i);
      caAcceptorChangeInCountList->SetAt(i,nAcceptorChangeInCount);
      caAcceptorCountList->SetAt(i,0);
    }
    ChangeAndShowContext();

    return STATE_NULL;
  }  

  trace(L6,_T("-PSButton2"));
  RETURNSTATE(SmCashierPassword);
}

///////////////////////////////////////////////////////////////////////////////
// Function:    PSButton3                                        
// Description: Dispenser pickup - sets all counts of current dispenser view to 0
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSmCashManagementBase::PSButton3(void)
{
  trace(L6,_T("+PSButton3() %d."), m_bBill);

  //Dump coins left in Coin Dispenser
  if(!m_bBill && dm.m_csCoinDispenserModel == _COINDISPENSER_TELEQUIP_HOPPER)
  { 
	long rc;
	CString changerStatusList = dm.CashChangerGetLowIndicators();

	  trace(L6,_T("dm.CashChangerGetLowIndicators() %s."), changerStatusList);

    int n = changerStatusList.Find(_T(";"));
	if(n> -1)
		changerStatusList = changerStatusList.Left(n);

	if(changerStatusList.Find(_T("0")) != -1)
	{
		  trace(L6,_T("SmShowPickupDumpFailure since not low"));	
		  ChangeAndShowContext( _T("SmShowPickupDumpFailure") );
		  return STATE_NULL;
	}
    else
	{
			long lDenum = 0;
			CString csDump = _T("AUDITDUMP");
			BSTR bstrDump = csDump.AllocSysString();
			rc = dm.CashChangerReplenishCoins(&lDenum, &bstrDump);
			trace(L6,_T("dm.CashChangerReplenishCoins rc = 0x%x"), rc);
			if(rc == OPOS_E_EXTENDED)
			{
				trace(L6,_T("SmShowPickupDumpFailure since rc = OPOS_E_EXTENDED "));
				ChangeAndShowContext( _T("SmShowPickupDumpFailure") );
				return STATE_NULL;
			}
			SysFreeString(bstrDump);


	trace(L6,_T("Successfully dump coins"));

	}

  }


  if( m_bBill )
  {
    for( int i = nCoinCashSeparator; i < caCountList.GetSize(); i++ )
    {
      m_caChangeInCount[i] -= caCountList[i];
      caCountList[i] = 0;
    }
    m_bBillHasMadeChanges = true;
  }
  else
  {
    for( int i = 0; i < nCoinCashSeparator; i++ )
    {
      m_caChangeInCount[i] -= caCountList[i];
      caCountList[i] = 0;
    }
    m_bCoinHasMadeChanges = true;
  }
  ChangeAndShowContext();

  trace(L6,_T("-PSButton3 %d, %d"), m_bBillHasMadeChanges, m_bCoinHasMadeChanges );

  return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    Ignore Changes
// Description: discards all changes that we made only on current cash, or coin device,
//              e.g. if we load/acceptor pickup/dispenser pickup at cash and coin screen,
//              then pressed the ignored changes button on the coin screen, then it will
//              only ignore the changes on the coin acceptor and dispenser.
///////////////////////////////////////////////////////////////////////////////////
void SMSmCashManagementBase::IgnoreChanges()
{
  trace( L6, _T("+IgnoreChanges()") );

  CArray<int,int> caCashAcceptorCountList, caCoinAcceptorCountList, caCashChangerCountList, caCoinChangerCountList;

  dmcc.GetCashCounts(&caCashAcceptorCountList, &caCoinAcceptorCountList, &caCashChangerCountList, &caCoinChangerCountList);

  if( m_bBill )
  {
    trace(L6,_T("Ignoring changes for cash acceptor"));

    m_caCashAcceptorCountList.Copy( caCashAcceptorCountList ); 
    m_caCashAcceptorChangeInCountList.RemoveAll();
    m_caCashAcceptorChangeInCountList.SetSize( m_caCashAcceptorCountList.GetSize() );

    trace(L9,_T("Re-Initializing cash acceptor Change In Count."));
    for( int i = 0; i < m_caCashAcceptorChangeInCountList.GetSize(); i++ )
      {m_caCashAcceptorChangeInCountList[i] = 0;}


    trace(L9,_T("Setting Cash Changer Count List."));
    for( int b = 0, nCash = nCoinCashSeparator; b < caCashChangerCountList.GetSize(); b++, nCash++ )
    {
      caCountList.SetAt( nCash, caCashChangerCountList[b] );
      m_caSavedOriginalCountList.SetAt( nCash, caCashChangerCountList[b] );
      m_caChangeInCount.SetAt( nCash, 0 );
    }
  }
  else
  {
    trace(L6,_T("Ignoring changes for coin acceptor"));

    m_caCoinAcceptorCountList.Copy( caCoinAcceptorCountList ); 

    m_caCoinAcceptorChangeInCountList.RemoveAll();
    m_caCoinAcceptorChangeInCountList.SetSize( m_caCoinAcceptorCountList.GetSize() );

    trace(L9,_T("Re-Initializing coin acceptor Change In Count."));
    for( int i = 0; i < m_caCoinAcceptorChangeInCountList.GetSize(); i++ )
      {m_caCoinAcceptorChangeInCountList[i] = 0;}


    trace(L9,_T("Setting Coin Changer Count List. %d"), caCoinChangerCountList.GetSize() );
    for( int b = 0; b < caCoinChangerCountList.GetSize(); b++ )
    {
      caCountList.SetAt( b, caCoinChangerCountList[b] );
      m_caSavedOriginalCountList.SetAt( b, caCoinChangerCountList[b] );   
      m_caChangeInCount.SetAt( b, 0 );
    }
  }
  trace( L6, _T("-IgnoreChanges()") );
}

///////////////////////////////////////////////////////////////////////////////
// Function:    PSButton4                                        
// Description: Ignore Changes button. Changes to SmIgnoreChange context
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSmCashManagementBase::PSButton4(void)
{
  trace( L6, _T("+PSButton4()") );

  if( ps.GetCurrentContext() != _T( "SmIgnoreChanges" ) )
  { //ignore the changes
    ChangeAndShowContext( _T("SmIgnoreChanges") );
    return STATE_NULL;
  }

  trace( L6, _T("-PSButton4()") );
  return STATE_NULL;
}


///////////////////////////////////////////////////////////////////////////////
// Function:    DMAcceptor                                        
// Description: resets the timeout
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSmCashManagementBase::DMAcceptor()
{
  
  //For glory device, DMAcceptor() will be called for each coin deposited. If 50 coins are 
  //deposited, this API will be called 50 times! We can't refresh the screen every time we
  //we recevie DMAcceptor(). It will take a long time to refresh the screen 50 times!

 /* if(DataInitialize())   //If return true, refresh the screen.
	  showScreen();


  DMCashAcceptorEnable(co.csTenderDenominations);
  DMCoinAcceptorEnable();
*/
  trace( L6, _T("DMAcceptor()") );

	if(!nTimeOut)   //If nTimeOut== 0, refresh screen 1 second later
		nTimeOut = 1;

  return STATE_NULL;
}

/*long SMSmCashManagementBase::DMCashAcceptorAccept()   
{
  trace(L6,_T("+SMSmCashManagementBase::DMCashAcceptorAccept"));

  //m_bBill = true; // we accepted cash

  trace(L6,_T("-SMSmCashManagementBase::DMCashAcceptorAccept %d"), m_bBill);
  return SMStateBase::DMCashAcceptorAccept();
}

long SMSmCashManagementBase::DMCoinAcceptorAccept()   
{
  trace(L6,_T("+SMSmCashManagementBase::DMCoinAcceptorAccept"));
  
  //m_bBill = false; // we accepted cash

  trace(L6,_T("-SMSmCashManagementBase::DMCoinAcceptorAccept %d"), m_bBill);
  return SMStateBase::DMCoinAcceptorAccept();
}
*/
void SMSmCashManagementBase::UnInitialize()
{
  trace(L6, _T("+Uninitialize"));

  if(dm.m_csCashRecyclerModel.IsEmpty())  // This is not a cash recycler.	
  {
	  dm.CoinAcceptorDisable();
	  dm.CashAcceptorDisable();
  }

  // TAR 335219 - Disabling of the Glory cash recycler is no longer done at
  //              UnInitialize.  It is now done in PSButton8.
  //              If currency is inserted when in an error 
  //              screen, then the currency will not be rejected.

   if(co.fLocaleCashManagementAPPTrackCashCount && dm.m_csCashRecyclerModel != _CASHDISPENSER_GLORY )
   {
      DMHandleAcceptorHigh();
   }

  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmCashManagement") );	// RFC 330050
  trace(L6, _T("-Uninitialize"));
}

///////////////////////////////////////////////////////////////////////////////
// Function:    PSKeystroke
// Description: if current context is :
//              SmGenericCashManagement - processes the Load command
//              SmCashManagement - processes the Remove nn, and Unload commands
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSmCashManagementBase::PSKeystroke(const long wParam, const long lParam) 
{ 

    //  based on the VALUE, we do some processing to be compatible with the CashChangerDispenseChange format.
    trace(L6, _T("+PSKeyStroke %d"), wParam);

    CString csChange = _T("%d:%d"); // important to tell cash changer what to dispense

    if( ps.GetCurrentContext() == _T("SmLoadLift") )
    { //we need this so we can properly process the data from loadlift context
      return SMStateBase::PSKeystroke( wParam, lParam );
    }
    //TAR 367095, Re-mapped the Key Stroke for the Cash Management screen for the fix

    if( wParam < 601 || wParam > ( m_bGlory ? 612 : 620 ) ) // ignore event
    {                                     //612 for glory, 620 for generic
      return STATE_NULL;
    }

    if( !m_bGlory && ( wParam >= 613 && wParam <= 620 ) )
    { //the load button of the generic cash management 
      nLoadIndex = wParam - 613;
      ChangeAndShowContext( _T("SmLoadLift") );

      return STATE_NULL;
    }

    int nRemoveOrUnload = 0;
    if( wParam >= 607 && wParam <= 612 ) // if this is unload
      nRemoveOrUnload = MAXCOLUMNS;   // the number of columns is also the number of remove nn buttons


    int nCoinOrBill = 0; //use this to process the correct index


    if( !m_bBill )
    { // if coin
      ASSERT( wParam-nRemoveOrUnload-600 <= nCoinCashSeparator );  // check that the coin in list exist
      // wParam -606, because the unload button list id starts from 607, then we can get the
      // target denomination by wParam - 606. e.g. if <Unload> button of the first denomination 
      // was pressed, then wParam is 607.
      csChange += _T(";");
    }
    else
    { // if bills
      nCoinOrBill = nCoinCashSeparator;

      ASSERT( ((wParam-nRemoveOrUnload-1) + nCoinOrBill) <= caValueList.GetSize() );
      csChange = _T(";") + csChange;
    }

    trace(L6, _T("nRemoveOrUnload : %d, nCoinOrBill : %d. ValueList %d. CountList %d."), 
      nRemoveOrUnload, nCoinOrBill, caValueList.GetSize(), caCountList.GetSize() );

    ASSERT( ((wParam-nRemoveOrUnload-1) + nCoinOrBill) <= caValueList.GetSize() );
           
    trace(L6, _T("Denomination selected : %d"), caValueList[((wParam-nRemoveOrUnload-1)+nCoinOrBill)]);
    ASSERT( abs(caValueList[((wParam-nRemoveOrUnload-1)+nCoinOrBill)]) > 0 ); // on the clicked button, the column must have a denomination 

    trace(L6, _T("Current count at the denomination %d is : %d"), 
      caValueList[((wParam-nRemoveOrUnload-1)+nCoinOrBill)], caCountList[wParam-nRemoveOrUnload-1+nCoinOrBill]);
    ASSERT( caCountList[((wParam-nRemoveOrUnload-1)+nCoinOrBill)] > 0 ); // on the clicked button, the column must not be empty 

    trace(L6,_T("nRemoveOrUnload : %d,  %d:%d"), nRemoveOrUnload, 
      caValueList[((wParam-nRemoveOrUnload-1)+nCoinOrBill)], 
      caCountList[((wParam-nRemoveOrUnload-1)+nCoinOrBill)] );

    int nDispenseAmount = 0;
    if( nRemoveOrUnload == 0 ) // if remove nn
    {
      if( caCountList[(wParam-1)+nCoinOrBill] < co.nCashRemoveAmount[(wParam-1)+nCoinOrBill] )
      { // in scotopts.dat, we specify the remove nn amount. if current count of chosen denomination is less than nn, then
        // dispense current count
        trace(L6,_T("at index %d, amount to dispense : %d"),wParam-1+nCoinOrBill, nRemoveOrUnload, caCountList[wParam-1+nCoinOrBill] );
        nDispenseAmount = caCountList[(wParam-1)+nCoinOrBill];
        csChange.Format( (CString)csChange, abs(caValueList[wParam-1+nCoinOrBill]), caCountList[wParam-1+nCoinOrBill]);
      }
      else
      {
        trace(L6,_T("at index %d, amount to dispense : %d"),(wParam-1)+nCoinOrBill, nRemoveOrUnload, co.nCashRemoveAmount[(wParam-1)+nCoinOrBill] );
        nDispenseAmount = co.nCashRemoveAmount[(wParam-1)+nCoinOrBill];
        csChange.Format( (CString)csChange, abs(caValueList[(wParam-1)+nCoinOrBill]), co.nCashRemoveAmount[(wParam-1)+nCoinOrBill]);
      }
    }
    else
    {           
      nDispenseAmount = caCountList[(wParam-nRemoveOrUnload-1)+nCoinOrBill];
      csChange.Format( (CString)csChange, abs(caValueList[(wParam-nRemoveOrUnload-1)+nCoinOrBill]), caCountList[(wParam-nRemoveOrUnload-1)+nCoinOrBill]);
    }
   
    trace(L6,_T("csChange : %s"), csChange );

	// +TAR 348739
	if (TrainingModeObj.IsTMOn() && !co.nOperationsAllowDispenseInTrainingMode) 
	{
		// in training mode and option set to not dispense
		trace(L4, _T("-PSKeyStroke:  In training mode.  No currency can be dispensed."));
		return STATE_NULL;
	}
	// -TAR 348739

    //m_caChangeInCount[wParam-nRemoveOrUnload-1+nCoinOrBill] = nDispenseAmount;
    long rc;
    if(m_bGlory) 
       rc = DMDispenseChange( csChange ); //dp185016 Issue #9 - Currency Dispensed Reporting issue


    if(DataInitialize())   //If return true, refresh the screen.
    {
      showScreen();
    }
    SetDevices();
    trace(L6,_T("-PSKeyStroke") );

  return STATE_NULL; 
} 

///////////////////////////////////////////////////////////////////////////////
// Function:    ShowAcceptorColumns
// Description: Shows the acceptor denomination, and count or value, depending on
//              the cashmanagementdisplaycount config value
///////////////////////////////////////////////////////////////////////////////////
void SMSmCashManagementBase::ShowAcceptorColumns( long nColumnNumber, 
        VARIANT_BOOL vColumnVisible, 
        CString vCountOrValue,
        CString vDenomination )
{
  trace(L6,_T("+ShowAcceptorColumns %d, %s, %s"), nColumnNumber, vCountOrValue, vDenomination );

  COleVariant v1 = (long)(nColumnNumber - 1), vReturn;

  CString csCountOrValue = _T("SmCashManagementAcceptorCountOrValue%d");
  CString csDenomID = _T("SmCashManagementAcceptorDenom%d");

  csCountOrValue.Format( (CString) csCountOrValue, nColumnNumber);
  csDenomID.Format( (CString) csDenomID, nColumnNumber);

  m_pAppPSX->SetConfigProperty(csDenomID, _T(""), UI::PROPERTYTEXTFORMAT, vDenomination); //SWR740
  m_pAppPSX->SetConfigProperty(csCountOrValue, _T(""), UI::PROPERTYTEXTFORMAT, vCountOrValue); //SWR740


  m_pAppPSX->SetConfigProperty( csDenomID, _T(""), UI::PROPERTYVISIBLE, vColumnVisible);
  m_pAppPSX->SetConfigProperty( csCountOrValue, _T(""), UI::PROPERTYVISIBLE, vColumnVisible);

  trace(L6,_T("-ShowAcceptorColumns"));
}

///////////////////////////////////////////////////////////////////////////////
// Function:    ShowAcceptorCylinder
// Description: Shows the current view's acceptor cylinder
///////////////////////////////////////////////////////////////////////////////////
void SMSmCashManagementBase::ShowAcceptorCylinder(  
        CString csAcceptorTotal,
        CString csChangeInCount,
        CString CylinderImagePath,
        CString CylinderPosition )
{
  trace(L6, _T("+ShowAcceptorCylinder %s, %s, %s, %s."), csAcceptorTotal, 
    csChangeInCount, CylinderImagePath, CylinderPosition );
  CString csCylinderAreaID = _T("SmCashManagementGenericCylinderArea9");

  COleVariant vOrder;
  m_pAppPSX->GetConfigProperty( _T("SmCashManagementPictureBackground"), _T(""), UI::PROPERTYZORDER, vOrder );

  //+set cylinder image path and position here
  m_pAppPSX->SetConfigProperty( csCylinderAreaID, _T(""), UI::PROPERTYPOSITION, CylinderPosition);
  m_pAppPSX->SetConfigProperty( csCylinderAreaID, _T(""), UI::PROPERTYPICTURE, CylinderImagePath);
  //-set cylinder image path and position here


  //+assign and set CountOrValue and Change In Count fields
  m_pAppPSX->SetConfigProperty(_T("SmCashManagementAcceptorChangeInCount"), _T(""), UI::PROPERTYTEXTFORMAT, csChangeInCount); //SWR740  
  m_pAppPSX->SetConfigProperty(_T("SmCashManagementAcceptorCountOrValue9"), _T(""), UI::PROPERTYTEXTFORMAT, csAcceptorTotal); //SWR740  

  //-assign and set CountOrValue and Change In Count fields

  trace(L6, _T("-ShowAcceptorCylinder") );
}

///////////////////////////////////////////////////////////////////////////////
// Function:    ShowTotals
// Description: Shows the totals fields and the labels fields of the Bill/Coin
//              Management UI
///////////////////////////////////////////////////////////////////////////////////
void SMSmCashManagementBase::ShowTotals(  
        CString csAcceptorTotal,
        CString csDispenserTotal,
        CString csGrandTotal )
{
  trace( L6, _T("+ShowTotals %s, %s, %s."),
    csAcceptorTotal, csDispenserTotal, csGrandTotal );

  m_pAppPSX->SetConfigProperty(_T("SmCashManagementAcceptorTotalText"), _T(""), UI::PROPERTYTEXTFORMAT, csAcceptorTotal); //SWR740  
  m_pAppPSX->SetConfigProperty(_T("SmCashManagementDispenserTotalText"), _T(""), UI::PROPERTYTEXTFORMAT, csDispenserTotal); //SWR740  
  m_pAppPSX->SetConfigProperty(_T("SmCashManagementDispenserTotalText"), _T(""), UI::PROPERTYTEXTFORMAT, csGrandTotal); //SWR740  


  trace( L6, _T("-ShowTotals") );
}

///////////////////////////////////////////////////////////////////////////////
// Function:    ShowColumn
// Description: If current context is :
//              SmCashManagement - glory device. shows the dispenser cylinders, change in count,
//                                 denomination, remove nn, and unload buttons
//              SmGenericCashManagement - non-glory. shows the dispenser cylinders, 
//                                 change in count, count or value, denomination, and
//                                 load buttons
///////////////////////////////////////////////////////////////////////////////////
void SMSmCashManagementBase::ShowColumn( long nColumnNumber, 
        VARIANT_BOOL vColumnfVisible, 
        CString vCountOrValue,
        CString vChangeInCount, 
        CString CylinderImagePath,
        CString CylinderPosition,
        CString vDenomination,
        long vUnloadButtonfEnable, 
        long vRemoveButtonfEnable,
        int nRemoveAmount,
        long vLoadButtonfEnable,
        VARIANT_BOOL vUnloadButtonfVisible,
        VARIANT_BOOL vRemoveButtonfVisible,
        VARIANT_BOOL vLoadButtonfVisible
        )
{
  trace(L6,_T("+ShowColumn() %d, %s, %s, %s, %s, %s, Enables/Visibility : %d, %d, %d, %d, %d, %d, %d"),
    vColumnfVisible, vCountOrValue, vChangeInCount, CylinderImagePath, CylinderPosition, vDenomination,
    vUnloadButtonfEnable, vRemoveButtonfEnable, nRemoveAmount, vLoadButtonfEnable,
    vUnloadButtonfVisible, vRemoveButtonfVisible, vLoadButtonfVisible );

  COleVariant v1 = (long)(nColumnNumber - 1), vReturn;

  CString csChangeInCountID = _T("SmCashManagementChangeInCount%d");
  CString csDenomID = _T("SmCashManagementDenom%d");
  CString csCylinderAreaID = _T("SmCashManagementCylinderArea%d");
  if( !m_bGlory )
    {csCylinderAreaID = _T("SmCashManagementGenericCylinderArea%d");}

  csChangeInCountID.Format( (CString) csChangeInCountID, nColumnNumber);
  csDenomID.Format( (CString) csDenomID, nColumnNumber);

  csCylinderAreaID.Format((CString) csCylinderAreaID, nColumnNumber);

  COleVariant vOrder; // get the order, this will be used when setting the white mask if column is not visible
  m_pAppPSX->GetConfigProperty( _T("SmCashManagementPictureBackground"), _T(""), UI::PROPERTYZORDER, vOrder );

  CString csCountOrValue;
  csCountOrValue.Format(_T("SmCashManagementDispenserCountOrValue%d"), nColumnNumber);

  if( vColumnfVisible == VARIANT_FALSE )
  { //then we set all fields for that column to not visible, and set the white mask
    m_pAppPSX->SendCommand(_T("SmCashManagementRemoveList"), UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, vColumnfVisible );
    m_pAppPSX->SendCommand(_T("SmCashManagementUnloadList"), UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, vColumnfVisible );
    m_pAppPSX->SendCommand(_T("SmCashManagementLoadList"), UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, vColumnfVisible );
    m_pAppPSX->SetConfigProperty( csChangeInCountID, _T(""), UI::PROPERTYVISIBLE, vColumnfVisible);
    m_pAppPSX->SetConfigProperty( csDenomID, _T(""), UI::PROPERTYVISIBLE, vColumnfVisible);
    m_pAppPSX->SetConfigProperty( csCountOrValue, _T(""), UI::PROPERTYVISIBLE, vColumnfVisible);

    vOrder = (long)(vOrder.lVal + 1);
    m_pAppPSX->SetConfigProperty( csCylinderAreaID, _T(""), UI::PROPERTYPOSITION, CylinderPosition);
    m_pAppPSX->SetConfigProperty( csCylinderAreaID, _T(""), UI::PROPERTYPICTURE, CylinderImagePath);
    m_pAppPSX->SetConfigProperty( csCylinderAreaID, _T(""), UI::PROPERTYZORDER, vOrder );

    trace(L6,_T("Done setting column to not visible") );
    return;
  }

  //+assign and set visible the change in count and denomination
  vDenomination = (CString) vDenomination;
  vChangeInCount= (CString) vChangeInCount;
  
  m_pAppPSX->SetConfigProperty(csDenomID, _T(""), UI::PROPERTYTEXTFORMAT, vDenomination); //SWR740
  m_pAppPSX->SetConfigProperty(csChangeInCountID, _T(""), UI::PROPERTYTEXTFORMAT, vChangeInCount); //SWR740


  m_pAppPSX->SetConfigProperty( csChangeInCountID, _T(""), UI::PROPERTYVISIBLE, vColumnfVisible);
  m_pAppPSX->SetConfigProperty( csDenomID, _T(""), UI::PROPERTYVISIBLE, vColumnfVisible);
  //-assign and set visible the change in count and denomination

  //+set visible or not, enabled or not, and data of the remove and unload button
  COleVariant cVariant = (long)vRemoveButtonfEnable;
  m_pAppPSX->SendCommand(_T("SmCashManagementRemoveList"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, v1, cVariant );
  cVariant = (long)vUnloadButtonfEnable;
  m_pAppPSX->SendCommand(_T("SmCashManagementUnloadList"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, v1, cVariant );

  COleVariant cVisible = vRemoveButtonfVisible;
  m_pAppPSX->SendCommand(_T("SmCashManagementRemoveList"), UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, cVisible );
  cVisible = vUnloadButtonfVisible;
  m_pAppPSX->SendCommand(_T("SmCashManagementUnloadList"), UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, cVisible );
  
  CString csStr = _T("StringIdButton"), csRet = _T(""); 
	m_pAppPSX->GetCustomDataVar( csStr, csRet, _T("SmCashManagementUnloadList") );
  long lStr = _ttol( csRet );
  csRet = ps.GetPSText( lStr );
  cVariant = (CString)csRet;
  m_pAppPSX->SendCommand(_T("SmCashManagementUnloadList"), UI::COMMANDSETBUTTONTEXT, vReturn, 2, v1, cVariant );

  //set the correct nn in remove nn button
  m_pAppPSX->GetCustomDataVar( csStr, csRet, _T("SmCashManagementRemoveList") );
  lStr = _ttol( csRet );
  csRet = ps.GetPSText( lStr );
  csRet += _T("\n%d");
  csRet.Format( (CString) csRet, nRemoveAmount );
  trace(L6,_T("Setting Remove Text to, %s : %s"), csStr, csRet );
  cVariant = (CString)csRet;
  m_pAppPSX->SendCommand(_T("SmCashManagementRemoveList"), UI::COMMANDSETBUTTONTEXT, vReturn, 2, v1, cVariant );
  //-set visible or not, enabled or not, and data of the remove and unload button

  //+set cylinder image path and position here
  m_pAppPSX->SetConfigProperty( csCylinderAreaID, _T(""), UI::PROPERTYPOSITION, CylinderPosition);
  m_pAppPSX->SetConfigProperty( csCylinderAreaID, _T(""), UI::PROPERTYPICTURE, CylinderImagePath);
  vOrder = (long)(vOrder.lVal - 1); // set to the correct order, must be painted first before the picturebackground 
  m_pAppPSX->SetConfigProperty( csCylinderAreaID, _T(""), UI::PROPERTYZORDER, vOrder );
  //-set cylinder image path and position here

  //+for generic, set visible or not, enabled or not, the load button
  cVariant = (long)vLoadButtonfEnable;
  m_pAppPSX->SendCommand(_T("SmCashManagementLoadList"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, v1, cVariant );

  cVisible = vLoadButtonfVisible;
  m_pAppPSX->SendCommand(_T("SmCashManagementLoadList"), UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, cVisible );
  //-set visible or not, enabled or not, the load button

  //+set text of load button
  csStr = _T("StringIdButton");
  csRet = _T(""); 
	m_pAppPSX->GetCustomDataVar( csStr, csRet, _T("SmCashManagementLoadList") );
  lStr = _ttol( csRet );
  csRet = ps.GetPSText( lStr );
  //trace(L6,_T("Setting Load Text, %s : %s"), csStr, csRet );
  cVariant = (CString)csRet;
  m_pAppPSX->SendCommand(_T("SmCashManagementLoadList"), UI::COMMANDSETBUTTONTEXT, vReturn, 2, v1, cVariant );
  //-set text of load button

  //+set value, and visible or not, the dispenser count or value field
  m_pAppPSX->SetConfigProperty(csCountOrValue, _T(""), UI::PROPERTYTEXTFORMAT, vCountOrValue); //SWR740
  m_pAppPSX->SetConfigProperty( csCountOrValue, _T(""), UI::PROPERTYVISIBLE, vColumnfVisible);
   //-set value, and visible or not, the dispenser count or value field

  trace(L6,_T("-ShowColumn"));

}

///////////////////////////////////////////////////////////////////////////////
// Function:    PSButton8
// Description: If current context is :
//              SmLoadLift - go to SmGenericCashManagement context
//              SmCashManagement - glory device. prints receipt
//              SmGenericCashManagement - Save the Changes made to DMCashCounts class
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSmCashManagementBase::PSButton8() // Go Back button
{
  // from cash management screen, we can go to authenticate, or maybe to error screens,
  // but from these next states, it should go back to this Cash Management screen, nowhere else.

  CString csDeposit;

  trace(L6,_T("+PSButton8"));

  if( ps.GetCurrentContext() == _T("SmLoadLift") )
  {
    ChangeAndShowContext(); //reset to cash management context

    return STATE_NULL;
  }
  else if(ps.GetCurrentContext() == _T("SmShowPickupDumpFailure") )
  {
    ChangeAndShowContext(); //reset to cash management context

    return STATE_NULL;
  }

  if(m_csPrinterCharSet != _T("437"))//TAR 410168  
    SendNewEscSeq2Printer(); //TAR381298

  if( !m_bGlory )
  {
    //let's get first the unmodified cashcounts list from DMCashCounts, then print it out in trace.

    CArray<int,int> caCashAcceptorCountList, caCoinAcceptorCountList,caCashChangerCountList, caCoinChangerCountList;

    dmcc.GetCashCounts( &caCashAcceptorCountList, &caCoinAcceptorCountList, &caCashChangerCountList, &caCoinChangerCountList );

    CString csCashAcceptorList, csCoinAcceptorList, csCashChangerList, csCoinChangerList;
    trace(L6,_T("Showing old counts"));
    for( int i = 0; i < caCashAcceptorCountList.GetSize(); i++ )
    {
      csCashAcceptorList.Format( _T("%s, %d"), (CString)csCashAcceptorList, caCashAcceptorCountList[i] );
    }
    for( i = 0; i < caCoinAcceptorCountList.GetSize(); i++ )
    {
      csCoinAcceptorList.Format( _T("%s, %d"), (CString)csCoinAcceptorList, caCoinAcceptorCountList[i] );
    }
    for( i = 0; i < caCashChangerCountList.GetSize(); i++ )
    {
      csCashChangerList.Format( _T("%s, %d"), (CString)csCashChangerList, caCashChangerCountList[i] );
    }
    for( i = 0; i < caCoinChangerCountList.GetSize(); i++ )
    {
      csCoinChangerList.Format( _T("%s, %d"), (CString)csCoinChangerList, caCoinChangerCountList[i] );
    }

    //set cash/coin changer counts;
    trace(L9,_T("Setting Cash/Coin Changer Count List."));
    caCashChangerCountList.RemoveAll();
    caCoinChangerCountList.RemoveAll();
  
    for( int a = 0; a < nCoinCashSeparator; a++ )
    {
      caCoinChangerCountList.Add( caCountList[a] );
    }
    for( ; a < caCountList.GetSize(); a++ )
    {
      caCashChangerCountList.Add( caCountList[a] );
    }
  
    CString csNewCashAcceptorList, csNewCoinAcceptorList, csNewCashChangerList, csNewCoinChangerList;
    for( i = 0; i < m_caCashAcceptorCountList.GetSize(); i++ )
    {
      csNewCashAcceptorList.Format( _T("%s, %d"), (CString)csNewCashAcceptorList, m_caCashAcceptorCountList[i] );
    }
    for( i = 0; i < m_caCoinAcceptorCountList.GetSize(); i++ )
    {
      csNewCoinAcceptorList.Format( _T("%s, %d"), (CString)csNewCoinAcceptorList, m_caCoinAcceptorCountList[i] );
    }
    for( i = nCoinCashSeparator; i < caCountList.GetSize(); i++ )
    {
      csNewCashChangerList.Format( _T("%s, %d"), (CString)csNewCashChangerList, caCountList[i] );
    }
    for( i = 0; i < nCoinCashSeparator; i++ )
    {
      csNewCoinChangerList.Format( _T("%s, %d"), (CString)csNewCoinChangerList, caCountList[i] );
    }
    if (!TrainingModeObj.IsTMOn())
	{
		dmcc.SetCashCounts(&m_caCashAcceptorCountList, &m_caCoinAcceptorCountList, &caCashChangerCountList, &caCoinChangerCountList);

		trace( L6, _T("Cash device Old Counts : Acceptor : %s. Changer : %s."), csCashAcceptorList, csCashChangerList );
		trace( L6, _T("Cash device New Counts : Acceptor : %s. Changer : %s."), csNewCashAcceptorList, csNewCashChangerList );

		trace( L6, _T("Coin Device Old Counts : Acceptor : %s. Changer : %s."), csCoinAcceptorList, csCoinChangerList );
		trace( L6, _T("Coin Device New Counts : Acceptor : %s. Changer : %s."), csNewCoinAcceptorList, csNewCoinChangerList );
    
		CArray<CString,CString> m_caCashChangerDenomList, m_caCoinChangerDenomList;
		CArray<int,int> m_caCashChangerChangeInCountList,m_caCoinChangerChangeInCountList;

		//+print receipt for generic
		for( i = 0; i < nCoinCashSeparator; i++ )
		{
		  m_caCoinChangerDenomList.Add( caDispenserDenomList[i] );
		  m_caCoinChangerChangeInCountList.Add( m_caChangeInCount[i] );
		}
		for( i = nCoinCashSeparator; i < caCountList.GetSize(); i++ )
		{
		  m_caCashChangerDenomList.Add( caDispenserDenomList[i] );
		  m_caCashChangerChangeInCountList.Add( m_caChangeInCount[i] );
		}
 
    rp.CurrencyCashManagement( m_caCashAcceptorDenomList, m_caCashAcceptorCountList, m_caCashAcceptorChangeInCountList,
                  m_caCashChangerDenomList, caCashChangerCountList, m_caCashChangerChangeInCountList,
                  m_caCoinAcceptorDenomList, m_caCoinAcceptorCountList, m_caCoinAcceptorChangeInCountList,
                  m_caCoinChangerDenomList, caCoinChangerCountList, m_caCoinChangerChangeInCountList, 
                  SMStateBase::csOperator, SMStateBase::csCashierID );

		DMPrintCashManagementReceipt( 
		  m_caCashAcceptorDenomList, 
		  m_caCoinAcceptorDenomList,  
		  m_caCashChangerDenomList, 
		  m_caCoinChangerDenomList,
		  m_caCashAcceptorCountList, 
		  m_caCoinAcceptorCountList,  
		  caCashChangerCountList, 
		  caCoinChangerCountList,
		  m_caCashAcceptorChangeInCountList,
		  m_caCoinAcceptorChangeInCountList,
		  m_caCashChangerChangeInCountList,
		  m_caCoinChangerChangeInCountList );
		//-print receipt for generic

	}
  }
  else	// This is a Glory cash recycler.
  {
    long rc = 1;

    trace(L6,_T("Size of value and change in count before passing: %d,   %d"), 
      caValueList.GetSize(), m_caChangeInCount.GetSize() );
    rc = DMPrintCashManagementReceipt( caValueList, m_caChangeInCount ); //print the glory receipt
    if( rc ) // if in error. rc is zero when success
    {
  //    ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_CASH_MANAGEMENT_INVALID_RECEIPT_DATA );
      trace(L6,_T("Failed to print receipt")); 
    }

    trace(L6,_T("Generate report. User ID : %s  Cashier ID : %s"), SMStateBase::csOperator, SMStateBase::csCashierID ); 

    rp.CurrencyCashManagement( caValueList, m_caChangeInCount, SMStateBase::csOperator, SMStateBase::csCashierID );
	
	// TAR 335219 
	// Only disable the cash recycler when exiting cash management.
	
	// TAR 319896:  We don't want any time lag for disabling the Glory 
	// recycler.
	// If FixDeposit() is called after dm.CashRecyclerPauseCounts(false),
	// then currency deposited in cash management may be stored
	// in CashRecyclerWrapper::m_nDeposited.	  
	dm.CashRecyclerStopDeposit(csDeposit);	// TAR 319896  
	dm.CashRecyclerPauseCounts(false);		// TAR 311471
  }

  if(m_csPrinterCharSet != _T("437"))//TAR 410168
    SendOrigEscSeq2Printer(); //TAR381298

  if( !fSAInTransaction )
  { // enables this to generate the xml file, and not wait till a transaction finishes
    rp.m_bForceEnd = true;
    rp.TransactionEnd();  
    rp.m_bForceEnd = false;  
  }

  SendTBCashManagementData();

  DisposeAllData(); // clear all data
  ps.Echo( _T("") );
  
  trace(L6,_T("-PSButton8"));

  RETURNSTATE(SmMediaStatus);
  //return STATE_RETURN;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    DisposeAllData                                        
// Description: resets all data upon exit of this state
///////////////////////////////////////////////////////////////////////////////////
void SMSmCashManagementBase::DisposeAllData()
{
  trace(L6,_T("+DisposeAllData()"));
  //let's clear all data
  m_caSavedOriginalCountList.RemoveAll();
  caValueList.RemoveAll();
  caDispenserDenomList.RemoveAll();
  caCountList.RemoveAll();
  m_caCashAcceptorCountList.RemoveAll();
  m_caCoinAcceptorCountList.RemoveAll();
  m_caCashAcceptorChangeInCountList.RemoveAll();
  m_caCoinAcceptorChangeInCountList.RemoveAll();
  m_caCashAcceptorDenomList.RemoveAll();
  m_caCoinAcceptorDenomList.RemoveAll();
  nSavedPurgeCnt = 0;
  m_bfFirstEntry = true; //reset
  m_bfAuthenticated = false;
  csBalance.Empty();
  csLoan.Empty();
  csPickUp.Empty();

  SMStateBase::csCashierID = EMPTY_STRING; //reset cashierid

  trace(L6,_T("-DisposeAllData()"));
}


///////////////////////////////////////////////////////////////////////////////
// Function:    TimedOut                                        
// Description: 
///////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmCashManagementBase::TimedOut(void)
{

  trace( L6, _T("TimedOut()") );
  if(DataInitialize())   //If return true, refresh the screen.
  {
    showScreen();
  }
  SetDevices();

  
  nTimeOut = 0;
   
  return STATE_NULL;
}


// Load screen functionalities

/////////////////////////////////////////////
// PSClearKey - Clear all input (on the pinpad)
//////////////////////////////////////////////
SMStateBase * SMSmCashManagementBase::PSClearKey()
{
  trace( L6, _T("+PSClearKey"));

  PSClearAllInput();

  trace( L6, _T("-PSClearKey"));

  return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    LoadAmount                                        
// Description: resets the count of a specified bin
// Parameters:  nLoadIndex          -the index of the bin chosen
//              nDeltaOrFullChange  -the amount to be added/deducted
//                                  -this will be of no use when bFullReplace is true 
//              bFullReplace        -whether we add/deduct the nDeltaOrFullChange, or 
//                                   do a full replace, setting the count to the bin's capacity.
//                                  -bfullreplace - Obsolete!
///////////////////////////////////////////////////////////////////////////////////
bool SMSmCashManagementBase::LoadAmount( int nLoadIndex, int nDeltaOrFullChange, bool bFullReplace )
{
  trace( L6, _T("+LoadAmount()"));
  bool bret = false;

  if( nDeltaOrFullChange == 0 && !bFullReplace )
    {return bret;}

  int nCoinOrBill = 0;
  bool *bHasMadeChanges;
  if( m_bBill ) //set to correct data
  {
    nCoinOrBill = nCoinCashSeparator;
    bHasMadeChanges = &m_bBillHasMadeChanges;
  }
  else
  {
    bHasMadeChanges = &m_bCoinHasMadeChanges;
  }
  trace( L6, _T("LoadIndex : %d. nCoinOrBill %d. ValueList size : %d"), 
    nLoadIndex, nCoinOrBill, caValueList.GetSize() ); 
  if( !co.fLocaleCashManagementDisplayCount && caValueList[ nLoadIndex + nCoinOrBill ] != 0)
  { // then this must be value
//    trace( L9, _T("Checking divisibility.%d"), caValueList[ nLoadIndex + nCoinOrBill ]); 
    if( nDeltaOrFullChange % caValueList[ nLoadIndex + nCoinOrBill ] != 0 )
    {
      trace( L6, _T("Input Value %d not divisible by %d."), 
        nDeltaOrFullChange, caValueList[ nLoadIndex + nCoinOrBill] );
      return bret;
    }
    else
    {
      nDeltaOrFullChange /= abs( caValueList[ nLoadIndex + nCoinOrBill] );
      trace( L6, _T("Load count for load value of %d is %d."),
        caValueList[ nLoadIndex + nCoinOrBill], nDeltaOrFullChange );
    }
  }

  if( !bFullReplace ) // add the nDeltaOrFullChange to current count
  {
	  //+TAR323136
	  if(nDeltaOrFullChange + caCountList[ nLoadIndex + nCoinOrBill ] > co.nCashCapacity[ nLoadIndex + nCoinOrBill ] )
	  {  //The amount entered will exceed the capacity of the bin for this denomination; using maximum capacity 
		  m_caChangeInCount[nLoadIndex + nCoinOrBill] += co.nCashCapacity[ nLoadIndex + nCoinOrBill ] - caCountList[ nLoadIndex + nCoinOrBill ];
		  nDeltaOrFullChange = co.nCashCapacity[ nLoadIndex + nCoinOrBill ];
		  m_nMaxAmtNeeded = co.nCashCapacity[ nLoadIndex + nCoinOrBill ] - caCountList[ nLoadIndex + nCoinOrBill ];
		  m_bExcessLoadAmtEntered = true;
	  }
	  //-TAR323136
	  else
	  {
		  m_caChangeInCount[nLoadIndex + nCoinOrBill] += nDeltaOrFullChange;
		  nDeltaOrFullChange += caCountList[ nLoadIndex + nCoinOrBill ];
	  }
  }
  else
  {
	  nDeltaOrFullChange = co.nCashCapacity[ nLoadIndex + nCoinOrBill ];
  }
  trace( L6, _T("Old Count  : %d. New Count : %d. ChangeInCount %d."), 
    caCountList[ nLoadIndex + nCoinOrBill ], nDeltaOrFullChange, m_caChangeInCount[nLoadIndex + nCoinOrBill]);
  
  if( caCountList[ nLoadIndex + nCoinOrBill ] != nDeltaOrFullChange )
  {
    *bHasMadeChanges = true;
    bret = true;
  }
  caCountList.SetAt( nLoadIndex + nCoinOrBill, nDeltaOrFullChange );

  trace( L6, _T("-LoadAmount() %d, %d, %d, %d, %d"), bret, m_caChangeInCount[nLoadIndex + nCoinOrBill], 
    nLoadIndex, nCoinOrBill, caCountList[ nLoadIndex + nCoinOrBill ] );

  return bret;
}


//////////////////////////////////////////////
// PSEnterKey
//////////////////////////////////////////////
SMStateBase  *SMSmCashManagementBase::PSEnterKey(void)
{
  trace( L6, _T("+PSEnterKey"));

  CString csInput;
  
  int nEntryDataLength = ps.GetInput(csInput);

  int nAmount = _ttol( csInput );

//  trace( L6, _T("Load count : %d"), nAmount );
//+TAR323136
  m_bExcessLoadAmtEntered = false; 
  m_nMaxAmtNeeded = 0;
//-TAR323136
  if( !LoadAmount( nLoadIndex, nAmount ) )
  { // input was not validated
    ChangeAndShowContext( _T("SmLoadLift") );
    return STATE_NULL;
  }
  //+TAR323136
  if(m_bExcessLoadAmtEntered)
  {
	  ChangeAndShowContext(_T("SmExcessLoadAmount") );
	  m_bExcessLoadAmtEntered = false;
	  m_nMaxAmtNeeded = 0;
	  return STATE_NULL;
  }
  //-TAR323136
  ChangeAndShowContext();

  trace( L6, _T("-PSEnterKey"));

  return STATE_NULL;
}

//////////////////////////////////////////////
// PSNumericKey
//////////////////////////////////////////////
SMStateBase  *SMSmCashManagementBase::PSNumericKey(const long lKey)
{
  trace( L6, _T("+PSNumericKey %d"), lKey );

  PSEchoInput(lKey);

  CString csVal;
  long lLen = ps.GetInput(csVal);
  long lVal = _ttol(csVal);

  if (lVal==0)
    ps.EnterClearKeys(true, false);  // disable Enter, enable Clear key.


  trace( L6, _T("-PSNumericKey"));

  return STATE_NULL;
}

//////////////////////////////////////////////
// PSOtherSpecialKeys - the minus sign
//////////////////////////////////////////////
SMStateBase *SMSmCashManagementBase::PSOtherSpecialKeys(const long lKey)
{
  trace( L6, _T("+PSOtherSpecialKeys()"));

  PSEchoInput(lKey);

  CString csVal;
  ps.GetInput(csVal);
  long lVal = _ttol(csVal); // if csVal is '-', then lVal will be zero

  if (lVal==0)
    ps.EnterClearKeys(true, false);  // disable Enter, enable Clear key.  

  trace( L6, _T("-PSOtherSpecialKeys"));
  return STATE_NULL;
}

void SMSmCashManagementBase::SendTBCashManagementData()
{
	CString csOperator;				// Operator user ID.
  csBalance.TrimRight(_T(";"));
  csLoan.TrimRight(_T(";"));
  csPickUp.TrimRight(_T(";"));
  csLoan.TrimRight(_T(","));
  csPickUp.TrimRight(_T(","));

  if (csLoan.Find(_T(":") ) != -1)//has loan data
  {
	if( dm.m_csCashRecyclerModel.IsEmpty() )  // If this is not a cash recycler.
		csLoan = _T("OperatorID=") + csCashierID + _T(";") + csLoan;
	else
		csLoan = _T("OperatorID=") + GetOperator() + _T(";") + csLoan;
	SendTBCommand(TBCMD_LOAN, csLoan);
  }

  if (csPickUp.Find(_T(":") )!= -1) //has pick up data
  {
	csPickUp = _T("OperatorID=") + csCashierID + _T(";") + csPickUp;
	SendTBCommand(TBCMD_PICKUP, csPickUp);
  }

  if( dm.m_csCashRecyclerModel.IsEmpty() ) // If this is not a cash recycler.
	csBalance = _T("OperatorID=") + csCashierID + _T(";") + csBalance;//Generic Cash Management required head cashier login
  else	// This is a cash recycler.
  {
	  csOperator = m_bfAuthenticated ? csCashierID : GetOperator();
	  csBalance = _T("OperatorID=") + csOperator + _T(";") + csBalance;
  }
	
  SendTBCommand(TBCMD_BALANCE, csBalance);

  trace(L6, _T("csLoan = %s, csPickUp = %s, csBalance = %s"), csLoan, csPickUp, csBalance);
}
