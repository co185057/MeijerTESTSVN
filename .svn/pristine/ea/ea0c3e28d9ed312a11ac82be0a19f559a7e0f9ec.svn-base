//////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCashManagementSafePayBase.CPP
//
// TITLE: Class implementation for store mode SafePay CashManagement state
//
//////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>                 // MS MFC always needed first
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSmCashManagementSafePay.h"     
#include "SmSmCashierPassword.h"
#include "SMSmMediaStatus.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SmCashManagementSP")

// MFC Runtime class/object information
IMPLEMENT_DYNCREATE(SMSmCashManagementSafePayBase, CObject)  //lint !e1774

DEFINE_TIMESTAMP

// Static variables.
int SMSmCashManagementSafePayBase::offset = 0;
bool SMSmCashManagementSafePayBase::m_bNoteCassetteLatched = true;
CSPCoordinate SMSmCashManagementSafePayBase::cylPositions[SPMAX_COLUMNS+1];
currency_array SMSmCashManagementSafePayBase::m_caUnfixCount;
currency_array SMSmCashManagementSafePayBase::m_caDenomList;
currency_map SMSmCashManagementSafePayBase::InvStart;
currency_map SMSmCashManagementSafePayBase::InvEnd;

/*lint --esym( 534,TraceObject::Trace,PSProceduresBase::Echo, 
                   PSProceduresBase::Set*,CPSX::Set*,SCOTAPPErrorObject::Error) */
SMSmCashManagementSafePayBase::SMSmCashManagementSafePayBase()
{
  IMPLEMENT_TIMESTAMP  
}

SMStateBase  *SMSmCashManagementSafePayBase::Initialize(void)
{
	const int MAXDENOMS = SPMAX_COLUMNS << 1;  // Mult by 2.  8 coins / 8 notes.

	ps.Echo( _T("") ); 
	trace(L6, _T("+Initialize> Recycler Model: %s"), dm.m_csCashRecyclerModel);

	// +TAR 354761
	if(m_SafePayLastErrorCode != OPOS_SUCCESS)
	{
		// If the coin door is opened while in cash mgmnt, the SafePay is
		// no longer in deposit mode.  Call EndDeposit to insure that FL
		// state is the same as the device.
		EndDeposit();
	}
	// -TAR 354761

	// Reset.  Continue button is OK for errors that occur in this state.
	m_SafePayLastErrorCode = OPOS_SUCCESS;	
	m_bShowMediaStatusButton = false;

	// If cash management is entered while at transaction is in progress,
	// save the value of nDMCashInserted so that currency deposited in
	// cash management will not be credited to the transaction balance.
	// If dm.IsPauseCountsActive == true, then this state is being re-entered
	// after an error or store login. 
	if(! dm.IsPauseCountsActive(DMCLASS_CASHRECYCLER))
	{
		// Do not preserve recycler values in cash management.
		dm.CashRecyclerPauseCounts(true);		// TAR 311471
	}

	// Try to latch cassette. BeginDeposit will fail if cassette is not present.
	// If the cash recycler is enabled, then the cassette must be present.
	// Attempts to latch the cassette while in a deposit mode will fail.
	if(! dm.CashRecyclerGetEnabled() && ! NoteCassette())
	{
		trace(L4, _T("ERROR - FAILED to latch note cassette.  Posting error to DM"));
			
		(void)mo.PostDM(DM_ERROR, DMCLASS_CASHRECYCLER, 0, 
			            OPOS_E_EXTENDED, NULL);
	}

	ps.SetFrame(_T("SmCashManagementSafePay"));

	// Initialize count arrays if necessary.
	if(m_caUnfixCount.GetSize() == 0)
	{
		try
		{
			// Set the size of all arrays to the maximum number of
			// supported denominations.
			m_caUnfixCount.SetSize(MAXDENOMS);
			caCountList.SetSize(MAXDENOMS);
			caValueList.SetSize(MAXDENOMS);
			m_caChangeInCount.SetSize(MAXDENOMS);
			
			// Gunnebo specified that this transaction type should be used
			// for cash management (for tracking purposes at back office).
			// TAR 355536:  Moved setting of transaction type.
			(void)SetTransactionType(SP_TransType_Float);
		}
		catch(CMemoryException *e)  //lint !e1752
		{
			trace(L4, _T("ERROR!  CMemoryException thrown!"));  
			e->Delete();
		}
	}

	// Don't save cylinder positions again if this has already been done.
	// The save stays in effect until the application is loaded again.  The
	// reason for this is that the in-memory PSX settings are changed to show
	// the cylinder levels.  Saving them again would result in different values.
	if(cylPositions[0].w() == -1)
	{
		OneTimeInit();
	}

	// Get initial currency counts and paint screen.
	// It is important that the recycler is not in deposit mode when the
	// initial call to DataInitialize() is made.
	if(DataInitialize())
	{
		showScreen();						
	}

	// Put recycler in deposit mode.
	BeginDeposit();			//lint !e534  Errors are handled in DMParse.
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    DataInitialize()
// Description: This is the main function that gets and processes all the data 
//              needed for display
// Returns:     true - if data has changed, and need repaint
///////////////////////////////////////////////////////////////////////////////
bool SMSmCashManagementSafePayBase::DataInitialize()
{
	BSTR pCashCounts;
    BOOL pDiscrepancy;
	int n;
	bool rc = false;
	CString csTemp;

	trace(L6, _T("+DataInitialize"));  

	if(dm.CashRecyclerGetEnabled())
	{
		// If in a deposit cycle, then DepositAmount property must be used
		// to determine the unfixed currency deposited so far.
		(void)dm.CashRecyclerGetDepositCounts(csTemp);
	}
	else
	{
		// Read the current fixed currency counts.
		pCashCounts = SysAllocStringByteLen(NULL,200);
		pDiscrepancy = true;
		n=0;
		while(dm.CashChangerGetCashCounts( &pCashCounts, 
										   &pDiscrepancy )!= OPOS_SUCCESS &&
			  n< co.nOperationsGloryDispenseRetries)  
		{   
			// device is not ready to give count, wait and try again.
			trace(L0,_T("dm.CashChangerGetCashCounts() not ready n=%d."), n);
			Sleep(co.getTimeOutGloryUnitDispense());  //lint !e732
			n++;
		}

		if (n==co.nOperationsGloryDispenseRetries)
		{
			trace(L0,_T("dm.CashChangerGetCashounts() failed."));
		} 

		csTemp = pCashCounts;
		SysFreeString(pCashCounts);
	}

	// Extract value / counts from cash list.
	if(ParseCashCounts(csTemp, dm.CashRecyclerGetEnabled()==TRUE) != -1)
	{
		rc = true;
	}

    offset = m_bBill ? nCoinCashSeparator : 0;
	return rc;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    showScreen()
// Description: This is the main function that sets the UI layout
//
// Notes:
// From the code review (B. Dawkins comment):
//   SMSmCashManagementSafePayBase::showScreen() - It might be better to 
//   have a Bill context and a coin context.  If you switched between 
//   contexts you could define all the text strings in the XML and 
//   significantly shorten this method.
//
// Decision from code review regarding this suggestion:
//   This is a very good suggestion.  This change would most likely make 
//   future maintenance and modifications easier.
//   Since the code completion date is very near.  It was decided that this 
//   change should be deferred until the next release.
// 
//   The change should not be difficult to make, but there may be unintended 
//   consequences from using separate contexts.
//
////////////////////////////////////////////////////////////////////////////////
void SMSmCashManagementSafePayBase::showScreen()
{
	CPSXRedrawLock crLock(_T("Display"), 
						  _T("SMSmCashManagementSafePayBase::showScreen"));
	long lButtonState = m_bfAuthenticated ? (long)UI::Normal:(long)UI::Disabled;
	bool bColumnVisible;
	CString csImagePath;
	CSPCoordinate cPosition;
	int totalAmnt = 0;			// Total value of currency (coins or notes)
	int totalCnt  = 0;			// Total count of currency (coins or notes)
	CString csTotal;

	ps.SetButtonText(_T("SMButton3"), PS_NOTEXT);
	ps.SetButtonText(_T("SMButton4"), PS_NOTEXT);
	ps.SetButtonText(_T("SMButton6"), PS_NOTEXT);
	ps.SetButtonText(_T("SMButton7"), PS_NOTEXT);
		
	if( m_bBill )
	{
		ps.SetTitle(ps.GetPSText( MSG_CASH_MANAGEMENT ), _T("") );
		ps.SetLeadthruText(MSG_SAFEPAY_INSERT_CASH);
		ps.SetButtonText(_T("SMButton1"),MSG_COIN_MANAGEMENT);
		ps.SetButtonText(_T("SMButton3"), MSG_SAFEPAY_UNLOCK_NOTECOVER);

		if(co.fLocaleAllowNoteCassetteRemoval)
		{
			ps.SetButtonText(_T("SMButton4"), MSG_SAFEPAY_UNLOCK_CASSETTE);
			ps.SetButtonText(_T("SMButton6"), MSG_SAFEPAY_NOTE_PICKUP);
			ps.SetButtonText(_T("SMButton7"), MSG_SAFEPAY_NOTE_TRANSFER);
		}
	}
	else
	{
		ps.SetTitle(ps.GetPSText( MSG_COIN_MANAGEMENT ), _T("") );
		ps.SetLeadthruText(MSG_SAFEPAY_INSERT_UNLOAD_COIN);
		ps.SetButtonText(_T("SMButton1"), MSG_CASH_MANAGEMENT);
	}

	// Disable critical buttons unless authenticated.
	if(! co.fLocaleAllowNoteCoverRemoval)
	{
		// Head cashier login required to remove note cover.if 
		// fLocaleAllowNoteCoverRemoval is false.  Normal store operator can
		// remove if fLocaleAllowNoteCoverRemoval is true.
		m_pAppPSX->SetConfigProperty( _T("SMButton3"), ps.GetCurrentContext(), 
									  UI::PROPERTYSTATE, lButtonState);
	}

	m_pAppPSX->SetConfigProperty( _T("SMButton4"), ps.GetCurrentContext(), 
			UI::PROPERTYSTATE, lButtonState);
	m_pAppPSX->SetConfigProperty( _T("SMButton6"), ps.GetCurrentContext(), 
			UI::PROPERTYSTATE, lButtonState);
	m_pAppPSX->SetConfigProperty( _T("SMButton7"), ps.GetCurrentContext(), 
			UI::PROPERTYSTATE, lButtonState);

	// If already authenticated, then disable the "Remove Cash" button.
	// If config options for software removal, software unload, and
	// note casette removal are not enabled, and the option for note
	// cover removal is enabled, then disable the "Remove Cash" button.
	// TAR: 350885
	lButtonState = (long)UI::Normal;
	if( m_bfAuthenticated || 
		( !co.fLocaleAllowSoftwareRemoval && !co.fLocaleAllowSoftwareUnload && !co.fLocaleAllowNoteCassetteRemoval && co.fLocaleAllowNoteCoverRemoval) )
	{ 
		lButtonState = (long)UI::Disabled;
	}
	m_pAppPSX->SetConfigProperty( _T("SMButton2"), ps.GetCurrentContext(), 
			UI::PROPERTYSTATE, lButtonState);
	
	// TAR 355536:  Moved ShowCassette() before column updates so that inventory
	//				counts are read.  Inventory counts are now used to calculate
	//				the change in counts.
	// Display the cassette cylinder.
	ShowCassette();

	//+ show dispenser column data	
	for(int i=0; i < SPMAX_COLUMNS; i++ ) 
	{		
		bColumnVisible = true;				
		
		// No denominations here.  Hide this cylinder.
		if( i >= GetDenomCount())
		{ 
			bColumnVisible = false;
		}
		else
		{
			totalAmnt += (Count(i) * DenomValue(i));
			totalCnt  += Count(i);
		}
		
		if(GetNewCylinderPosition(i, bColumnVisible, csImagePath, cPosition))
		{
			ShowColumn(i, bColumnVisible, csImagePath, CString((LPCTSTR)cPosition));
		}
	}

	// Update totals field.
	if(co.fLocaleCashManagementDisplayCount)
	{
		csTotal.Format(_T("%d"), totalCnt);
	}
	else
	{
		csTotal = SMStateBase::ConvertValueToString(totalAmnt, m_bBill);
	}
	m_pAppPSX->SetTransactionVariable( _T("SmCashManagementDispenserTotalText"), csTotal);

	(void)ps.ShowFrame();
	trace(L6,_T("-showScreen"));
}

///////////////////////////////////////////////////////////////////////////////
// Function:    GetNewCylinderPosition
// Description: Gets the new cylinder position and image based on the 
// 				current count
//              of the bin relative to that bin's capacity
// Parameters : i - the bin index
// 				bool bVisible - If true the column is visible.  If false, it
// 				    will be hidden.
//              csImage - out parameter.  Will contain the image file that 
//              	will be used to display the level for the cylinder.
//              pos - Contains the display coordinates for csImage.
// Return     : true if successful.
////////////////////////////////////////////////////////////////////////////////
bool SMSmCashManagementSafePayBase::GetNewCylinderPosition(int i, bool bVisible,
														   CString& csImage, 
														   CSPCoordinate &pos)
{
    COleVariant vReturn;
	CString csImageId = _T("SmCashManagementGreenFill");
    int nBottomPosition;
	int nNewCylPosition;
    double dPercentage = 1.0;
	int count = 0;
	int capacity = 0; 
	
    nBottomPosition = 
		cylPositions[i].y() + cylPositions[i].h() - SPHEIGHTOFBOTTOMDIAMETER; 

	// If an attempt to display an invalid column number is made, 
	// then log & return.
	if(i > SPMAX_COLUMNS-1 || i < 0)
	{ 
		trace(L6, _T("ERROR:  Invalid cylinder index: %d"), i);
		return false;
	}

	// If the column is not visible, position the white mask graphic over the 
	// background of this cylinder.
	if(! bVisible)
	{		
		(void)m_pAppPSX->GetConfigProperty( _T("SmCashManagementWhiteMask"), 
											_T(""), UI::PROPERTYPICTURE, vReturn);
		
		pos.x(cylPositions[i].x()-5);
		pos.y(cylPositions[i].y()-5);
		pos.w(cylPositions[i].w()+10);
		pos.h(cylPositions[i].h()+10);
		csImage = vReturn.bstrVal;
		return true;
	}

	count = Count(i);		   // Avoid as many virtual func calls as possible.
	capacity = GetCapacity(i); // Avoid as many virtual func calls as possible.

	// All but one of the coordinates will be the same, so just copy the current
	// cylinder location to start with.
	pos = cylPositions[i];	
	if( count != 0 && count <= capacity )
	{
		dPercentage = ((double)count / capacity); // ok to disregard decimals. 
	}
	
	//lint -e{524}  Calculate new y offset of cylinder.
	nNewCylPosition = (cylPositions[i].h()-SPHEIGHTOFBOTTOMDIAMETER)*dPercentage; 

	// the new vertical position starting point.
	nNewCylPosition = nBottomPosition - nNewCylPosition; 

	// Assign new position and determine which graphic to use.
	pos.y(nNewCylPosition);	

	if( count <= 0 || capacity == -1 )
	{ 
		// Empty.or misconfigured capacity.
		pos.y(nBottomPosition);		// If empty, don't show full red cylinder.
		csImageId = _T("SmCashManagementRedFill");
	}
	else if( count <= GetLowList(i) || (count >= GetHighList(i) && dPercentage < 1.0)) 
	{ 
		// Almost empty or almost full.
		csImageId = _T("SmCashManagementYellowFill");
	}
	else if( dPercentage >= 1.0 )		
	{
		// Full cylinder.
		csImageId = _T("SmCashManagementRedFill");
	}
	
	// Get the image that corresponds to the cylinder graphic.
	(void)m_pAppPSX->GetConfigProperty(csImageId, _T(""),UI::PROPERTYPICTURE,vReturn);
	csImage = vReturn.bstrVal;
	
	trace(L6,_T("-GetNewCylinderPosition()> %s, %s"), (LPCTSTR)pos, csImage);	
	return true;
}

/**
 * Function:		BeginDeposit
 * Purpose:			Enable deposit mode of SafePay cash recycler.
 * Parameters:		NONE
 * Returns:			true if successful, false otherwise.
 * Calls:			DMCashAcceptorEnable
 * Side Effects:
 * Notes:			This method was added to log note cover latching.
 *					This has been removed, so this method is probably not 
 *					required.
 */
bool SMSmCashManagementSafePayBase::BeginDeposit(void)
{
	long rc;
    rc = DMCashAcceptorEnable(co.csTenderDenominations);

	if(rc == OPOS_SUCCESS)
	{
		// If the note cover is tagged as unlatched, but we successfully
		// entered deposit mode, then the cover must now be latched.
		/*
		if(m_bNoteCoverLatched == false)
		{
			m_bNoteCoverLatched = true;
			rp.ModeChange(entryID++, MODE_NOTECOVER, _T("LOCKED"));
		} */
		
		return true;
	}
	
	return false;
}

/**
 * Function:		OneTimeInit
 * Purpose:			Performs one-time initialization.
 * Parameters:		NONE
 * Returns:			true if successful, false otherwise.
 * Calls:			
 * Side Effects:
 * Notes:			
 */
bool SMSmCashManagementSafePayBase::OneTimeInit(void)
{
	trace(L6, _T("+OneTimeInit"));

	CString csCurrent;
	CString csBuff;				// Temporary buffer.
    COleVariant vResult;
	int i;
	int notes = -1;				// Used to distinguish notes from coins.
	
	// Initialize the denomination cylinders and cassette cylinder.
	for(i=1; i <= SPMAX_COLUMNS+1; ++i)
	{
		csCurrent.Format(_T("SmCashManagementGenericCylinderArea%d"), i);
        (void)m_pAppPSX->GetConfigProperty(csCurrent, _T(""), UI::PROPERTYPOSITION, 
									 vResult);

        csCurrent = vResult.bstrVal;
		cylPositions[i-1] = csCurrent;

		trace(L6, _T("Cylinder pos %d: [%s]"),i-1,(LPCTSTR)cylPositions[i-1]);
	}

	// Initialize the denomination list for inventory counts.
	// The ReadCashCounts command (CashCount property) is used to display the 
	// cylinder levels in cash management, but the InventoryList property is 
	// used to determine the actual inventory changes.  The reason for this is 
	// that the counts returned by ReadCashCounts are for the coins and notes 
	// that can be dispensed, while the InventoryList gives a count for all 
	// currency in the recycler.  The first few coins (5) of each 
	// denomination cannot be dispensed reliably, so the counts for these coins 
	// are not included in ReadCashCounts, but they are included in the 
	// Inventory list. 
	// The denominations of the ReadCashCounts command correspond to the
	// ExitCashList property.  This represents all dispensable denominations,
	// but it may not contain all denominations that are accepted by the 
	// recycler.  The CurrencyCashList property is a list of all denominations
	// that are accepted and dispensed.  the m_caDenomList is used to store
	// each of these denominations.
	m_caDenomList.RemoveAll();

	// Obtain the CurrencyCashList.
	csCurrent.Empty();
	csBuff.Empty();
	if(!dm.CashRecyclerGetCurrencyCashList(csCurrent))
	{
		trace(L4, _T("-OneTimeInit ERROR Could not get CurrencyCashList"));
		return false;
	}

	// Parse the string and add values to m_caDenomList.  
	// This array will be passed to rp.CurrencyCashManagement().  This method
	// expects the coin denominations to be negative.
	for(i=0; i<csCurrent.GetLength(); ++i)
	{
		switch(csCurrent[i])
		{
			case _T(';'):
				(void)m_caDenomList.Add(_ttoi(csBuff) * notes);  // Add last coin.
				csBuff.Empty();
				notes = 1;								   // Transition to notes.
				break;
			case _T(','):
				(void)m_caDenomList.Add(_ttoi(csBuff) * notes);
				csBuff.Empty();
				break;
			default:
				if(_istdigit(csCurrent[i]))
				{
					csBuff += csCurrent[i];
				}
				else
				{
					trace(L4, _T("-OneTimeInit> Invalid char %c"), 
							csCurrent[i]); 
					return false;
				}
		}
	}
	(void)m_caDenomList.Add(_ttoi(csBuff) * notes);	// Add the last denomination.

	trace(L6, _T("-OneTimeInit"));
	return true;
}

/**
 * Function:		Count
 * Purpose:			Wrapper for accessing data from count array.
 * Parameters:		index - zero-based index.
 * Returns:			The count for the denomination at location "index" or 0 if 
 *					the index is not within the array bounds.
 * Calls:			
 * Side Effects:
 * Notes:			offset is 0 if coins are currently displayed.  offset 
 *					equals the index of the first note denomination if notes are
 *					currently displayed.
 */
int SMSmCashManagementSafePayBase::Count(int index) const
{
	ASSERT((offset + index) < caCountList.GetSize());
	ASSERT((offset + index) < m_caUnfixCount.GetSize());

	if( (offset + index) >= caCountList.GetSize()  || 
		(offset + index) >= m_caUnfixCount.GetSize() )
	{
		trace(L4, _T("ERROR:  Attempted out-of-bounds array access!!"));
		return 0;
	}

	// Return current count + unfixed count.
	return caCountList[offset + index] + m_caUnfixCount[offset + index];
}

/**
 * Function:	    GetDenomCount
 * Purpose:			Return the number of coin or note denominations.
 * Parameters:		NONE
 * Returns:			The number of coin denominations if offset=0.  The number of
 *					note denominations if offset > 0.
 * Calls:			
 * Side Effects:
 * Notes:			
 */
int SMSmCashManagementSafePayBase::GetDenomCount(void) const
{
	// TAR 355536:  No longer using m_caSavedOriginalCountList.
	return caCountList.GetSize() - offset;
}

/**
 * Function:	    DenomValue
 * Purpose:			Wrapper for accessing data from denomination array.
 * Parameters:		index - zero-based index.
 * Returns:			The numeric value for the denomination at location "index" 
 *                  or 0 if 
 *					the index is not within the array bounds.
 * Calls:			
 * Side Effects:
 * Notes:			offset is 0 if coins are currently displayed.  offset 
 *					equals the index of the first note denomination if notes are
 *					currently displayed.
 */
int SMSmCashManagementSafePayBase::DenomValue(int index) const
{
	ASSERT((offset + index) < caValueList.GetSize());

	if( (offset + index) >= caValueList.GetSize() )
	{
		trace(L4, _T("ERROR:  Attempted out-of-bounds array access!!"));
		return 0;
	}
	return(abs(caValueList[offset + index]));
}

/**
 * Function:		ChangeInCount
 * Purpose:			Report the net change in counts for a denomination.
 * Parameters:		index - zero-based index.
 * Returns:			Returns the change in count (currency deposited/dispensed 
 *                  while in current cash management session) for denomination 
 *					at location "index" or 0 if 
 *					the index is not within the array bounds.
 * Calls:			
 * Side Effects:
 * Notes:			offset is 0 if coins are currently displayed.  offset 
 *					equals the index of the first note denomination if notes are
 *					currently displayed.
 */
int SMSmCashManagementSafePayBase::ChangeInCount(int index) const
{
	ASSERT((offset + index) < caValueList.GetSize());
	ASSERT((offset + index) < m_caUnfixCount.GetSize());

	// +TAR 355536:  Now using start/end inventory counts to determine the change
	//				 in counts.  Previously used start/end values from 
	//				 ReadCashCounts().
	int sfcount = 0;	// Starting fixed count.
	int efcount = 0;	// Ending fixed count.

	if( (offset + index) >= caValueList.GetSize() )
	{
		// m_caUnfixCount and should always have a 
		// size >= caCountList.
		trace(L4, _T("ERROR:  Attempted out-of-bounds array access!!"));
		return 0;
	}

	if(InvStart.Lookup(abs(caValueList[offset + index]), sfcount) == FALSE)
	{
		sfcount = 0;	// This denomination was not found.
	}

	if(InvEnd.IsEmpty())
	{
		// If InvEnd is empty, then this is the initial load of cash management.
		efcount = sfcount;
	}
	else if(InvEnd.Lookup(abs(caValueList[offset + index]), efcount) == FALSE)
	{
		efcount = 0;		// This denomination was not found.
	}

	return efcount + m_caUnfixCount[offset + index] - sfcount;
	// -TAR 355536
}

/**
 * Function:		EndDeposit
 * Purpose:			Disable the deposit cycle of the recycler and update counts.
 * Parameters:		finalize - If true, perform a final count (done at exit of
 *					cash management).  If false, then just update count arrays 
 *					that are used for screen display.
 * Returns:			NOTHING
 * Calls:			
 * Side Effects:
 * Notes:			
 */
void SMSmCashManagementSafePayBase::EndDeposit(bool finalize)
{
	CString csTemp;		// Sacrificial string.
	int i;				// Counter.
	int scount;			// Start count.
	int ecount;			// End count.
	SPInventory sinv;

	trace(L6, _T("+EndDeposit"));
	(void)dm.CashRecyclerStopDeposit(csTemp);

	for(i=0; i < caCountList.GetSize(); i++)
	{
		caCountList[i] += m_caUnfixCount[i]; // Update until next data refresh. 
		m_caUnfixCount[i] = 0;				 // Reset.
	}

	if(!finalize)
	{
		return;
	}

	// If here, then finalize start and end counts so the change in counts
	// (if any) can be determined.
	if( GetInventoryCounts(csTemp) != OPOS_SUCCESS ||
		!ParseInventoryCounts(sinv, csTemp) )		// Get a final inventory count.
	{
		memset(&sinv, 0, sizeof(sinv));
		trace(L4, _T("ERROR:  Could not get / parse inventory counts."));
	}

	m_caChangeInCount.RemoveAll();
	csBalance.Empty();
	csTemp.Empty();

	// Determine the inventory change and build a balance string for use
	// with the TB cash management API.
	for(i=0; i<m_caDenomList.GetSize(); ++i)
	{
		// Potential problem:  There is no distinction between notes and coins
		// in the InventoryList property.  If the system had coins and notes
		// of the same value, then their counts would be lumped together.
		// According to Gunnebo, this is not a valid configuration.
		if(InvStart.Lookup(abs(m_caDenomList[i]), scount) == FALSE)
		{
			scount = 0;		// This denomination was not found.
		}
				
		if(InvEnd.Lookup(abs(m_caDenomList[i]), ecount) == FALSE)
		{
			ecount = 0;		// This denomination was not found.
		}

		// Record the change in inventory for this denomination.
		(void)m_caChangeInCount.Add(ecount - scount);

		// Build csBalance.
		csTemp.Format(_T("%d:%d,"), abs(m_caDenomList[i]), ecount);

		if(m_caDenomList[i]>=0 && i>0 && m_caDenomList[i-1]<0)
		{
			// Transitioned to notes.
			if(csBalance[csBalance.GetLength()-1] == _T(','))
			{
				// Replace comma from last coin with note separator.
				csBalance.SetAt(csBalance.GetLength()-1, _T(';'));
			}
			else
			{
				csBalance += _T(";");	
			}
			csBalance += csTemp; 
		}
		else
		{
			csBalance += csTemp; 
		}

		// As a safeguard, remove the denomination from the hash table so
		// there is no chance of using the counts again.
		(void)InvStart.RemoveKey(abs(m_caDenomList[i]));  
		(void)InvEnd.RemoveKey(abs(m_caDenomList[i]));  
	}

	// Remove trailing comma.
	csBalance.TrimRight(_T(','));

	ASSERT(InvStart.IsEmpty() && InvEnd.IsEmpty());
	trace(L6, _T("-EndDeposit> csBalance: %s"), (LPCTSTR)csBalance);
}

/**
 * Function:		SetTransactionType
 * Purpose:			The SafePay recycler has different categories to track 
 *                  deposit /  dispense activities.  This method sets it.
 * Parameters:		type.- The new transaction mode.  
 *					SP_TransType_Sale is for normal transactions.
 *					SP_TransType_Float is for cash management activities.
 *					
 * Returns:			OPOS_E_ILLEGAL if type is an invalid mode.  rc of 
 *                  dm.DirectIO otherwise.
 * Calls:			
 * Side Effects:
 * Notes:			
 */
long SMSmCashManagementSafePayBase::SetTransactionType(long type)
{
	long lData;
	long lRcEx;
	CString csData;
	long rc;

	// TAR 355536:  Added additional tracing (not related to this TAR)..
	trace(L6, _T("+SetTransactionType(%d)"), type);

	// Sanity check.
	if(type < SP_TransType_Sale || type > SP_TransType_CashBack)
	{
		trace(L6, _T("-SetTransactionType(%d):  ERROR Invalid parameter."), type);
		return OPOS_E_ILLEGAL;
	}
	
	lData = type;

	rc = dm.DirectIO(DMCLASS_CASHRECYCLER, 0,
					 SP_DIO_SETTRANTYPE, &lData, csData, &lRcEx);

	trace(L6, _T("-SetTransactionType(%d): rc=%d"), type, rc);
	return (rc);
}

/**
 * Function:	 	GetRemoveAmountLimit
 * Purpose:			Return the configured amount (ScotOpts) that may be removed
 *					for a denomination.
 * Parameters:		index - zero-based index.
 * Returns:			Returns the remove amount for denomination 
 *					at location "index".
 * Calls:			
 * Side Effects:
 * Notes:			offset is 0 if coins are currently displayed.  offset 
 *					equals the index of the first note denomination if notes are
 *					currently displayed.
 */
int SMSmCashManagementSafePayBase::GetRemoveAmountLimit(int index) const
{
	int remAmnt;

	ASSERT((offset + index) < co.nCashRemoveAmount.GetSize());

	// Check for out-of-bounds array access.
	if( (offset + index) >= co.nCashRemoveAmount.GetSize() )
	{
		ScotError(RETURN,SM_CAT_CONFIGURATION, SCOTOPTS_CONFIG_ERROR,SFPAY_CFG_ERR,
		          offset + index, _T("CashRemoveAmount") );

		return 0;
	}

	remAmnt = co.nCashRemoveAmount[offset + index]; 

	// Check for incorrect configuration.
	return remAmnt < 0 ? 0 : remAmnt; 
}

/**
 * Function:	 	GetCapacity
 * Purpose:			Return the configured capacity (ScotOpts) for cashrecycler
 *					storage of denomination.
 * Parameters:		index - zero-based index of denomination or -1 to get the 
 *                  capacity of the note cassette.
 * Returns:			Returns the storage capacity for denomination 
 *					at location "index" or -1 if the index is not within the 
 *					array bounds.
 * Calls:			
 * Side Effects:
 * Notes:			offset is 0 if coins are currently displayed.  offset 
 *					equals the index of the first note denomination if notes are
 *					currently displayed.
 *
 *					The capacity is used to scale the height of the denomination
 *					cylinders.
 */
int SMSmCashManagementSafePayBase::GetCapacity(int index) const
{
	int cap = -1;

	ASSERT((offset + index) < co.nCashCapacity.GetSize());

	if(index == -1)	
	{
		// Return capacity of the cassette.
		index = co.nCashCapacity.GetSize() - (1 + offset);  // For correct logging.
		cap = co.nCashCapacity[co.nCashCapacity.GetSize() - 1];
	}
	else if((offset + index) < co.nCashCapacity.GetSize())	
	{
		// Valid request
		cap = co.nCashCapacity[offset + index];
	}

	if(cap <= 0)
	{
		ScotError(RETURN,SM_CAT_CONFIGURATION, SCOTOPTS_CONFIG_ERROR,SFPAY_CFG_ERR,
			      offset + index, _T("CashCapacity"));

		cap = -1;			// To avoid divide by zero.
	}
	return cap;
}

/**
 * Function:	 	GetHighList
 * Purpose:			Return the configured high level (ScotOpts) for cashrecycler
 *					storage of denomination.
 * Parameters:		index - zero-based index of denomination..
 * Returns:			Returns the high count for denomination 
 *					at location "index" or 0 if the index is not within the 
 *					array bounds.
 * Calls:			
 * Side Effects:
 * Notes:			offset is 0 if coins are currently displayed.  offset 
 *					equals the index of the first note denomination if notes are
 *					currently displayed.
 *
 *					The high count is used to indicate whether the number of 
 *					coins / notes for a particular denomination is high and 
 *					should be emptied.
 */
int SMSmCashManagementSafePayBase::GetHighList(int index) const
{
	ASSERT((offset + index) < co.nCashHighList.GetSize());

	// Check for out-of-bounds array access.
	if( (offset + index) >= co.nCashHighList.GetSize() )
	{
		ScotError(RETURN,SM_CAT_CONFIGURATION, SCOTOPTS_CONFIG_ERROR,SFPAY_CFG_ERR,
		          offset + index, _T("CashHighList") );

		return 0;
	}

	if(index == -1)		// Return capacity of the cassette.
	{
		return co.nCashHighList[co.nCashHighList.GetSize() - 1];
	}

	return co.nCashHighList[offset + index];
}

/**
 * Function:	 	GetLowList
 * Purpose:			Return the configured low level (ScotOpts) for cashrecycler
 *					storage of denomination.
 * Parameters:		index - zero-based index of denomination..
 * Returns:			Returns the low count for denomination 
 *					at location "index" or 0 if the index is not within the 
 *					array bounds.
 * Calls:			
 * Side Effects:
 * Notes:			offset is 0 if coins are currently displayed.  offset 
 *					equals the index of the first note denomination if notes are
 *					currently displayed.
 *
 *					The low count is used to indicate whether the number of 
 *					coins / notes for a particular denomination is low and 
 *					should be replenished.
 */
int SMSmCashManagementSafePayBase::GetLowList(int index) const			
{
	ASSERT((offset + index) < co.nCashLowList.GetSize());

	// Check for out-of-bounds array access.
	if( (offset + index) >= co.nCashLowList.GetSize() )
	{
		ScotError(RETURN,SM_CAT_CONFIGURATION, SCOTOPTS_CONFIG_ERROR,SFPAY_CFG_ERR,
		          offset + index, _T("CashLowList") );

		return 0;
	}

	return co.nCashLowList[offset + index];
}

/**
 * Function:		PSButton1
 * Purpose:			Toggle between the display of coins or notes.
 * Parameters:		NONE
 * Returns:			STATE_NULL (remain in same state).
 * Calls:			
 * Side Effects:
 * Notes:			
 */
SMStateBase *SMSmCashManagementSafePayBase::PSButton1(void)
{
  trace(L6,_T("+PSButton1"));

  m_bBill = !m_bBill;
  offset = m_bBill ? nCoinCashSeparator : 0;

  if( DataInitialize() )    //If return true, refresh the screen.
  {
    showScreen();
  }

  trace(L6,_T("-PSButton1 %d"), (int)m_bBill );

  return STATE_NULL;

}

/**
 * Function:		PSButton2
 * Purpose:			Authenticate head cashier to allow currency dispense, 
 *                  removal of the note cover and note cassette.
 * Parameters:		NONE
 * Returns:			SmCashierPassword state.
 * Calls:			
 * Side Effects:
 * Notes:			
 */
SMStateBase *SMSmCashManagementSafePayBase::PSButton2(void)
{
  trace(L6,_T("+PSButton2"));

  trace(L6,_T("-PSButton2"));
  RETURNSTATE(SmCashierPassword);  //lint !e527 !e1774
} //lint !e533

/**
 * Function:		PSButton3
 * Purpose:			Unlock the note recycler note cover.
 * Parameters:		NONE
 * Returns:			STATE_NULL
 * Calls:			DMUnlockNoteCover
 * Side Effects:	Note cover is unlocked.
 * Notes:			
 */
SMStateBase *SMSmCashManagementSafePayBase::PSButton3(void)
{
	trace(L6,_T("+PSButton3"));

	// Sanity check.  
	// This action is not relevant in the coin screen.
	// If head cashier access is required, then the user must be authenticated.
	// TAR: 350885
	if(! m_bBill || (! m_bfAuthenticated && ! co.fLocaleAllowNoteCoverRemoval))
	{
		trace(L4, SFPAY_AERR, _T("-PSButton3"),(int)m_bBill, (int)m_bfAuthenticated);
		return STATE_NULL;
	} 

	(void)EndDeposit();  // TAR 355536:  Cleanup variables and end deposit.
	(void)DMUnlockNoteCover();

	trace(L6,_T("-PSButton3"));
	return STATE_NULL;
}

/**
 * Function:		PSButton4
 * Purpose:			Unlock the note recycler note cassette.
 * Parameters:		NONE
 * Returns:			STATE_NULL
 * Calls:			NoteCassette
 * Side Effects:	If authenticated (logged in as head cashier), note cassette 
 *                  is unlocked.
 * Notes:			
 */
SMStateBase *SMSmCashManagementSafePayBase::PSButton4(void)
{
	trace(L6,_T("+PSButton4"));
	bool rc;

	// Sanity check.  
	if(!m_bBill || !m_bfAuthenticated)
	{
		trace(L4, SFPAY_AERR, _T("-PSButton4"), (int)m_bBill, (int)m_bfAuthenticated);
		return STATE_NULL;
	} 

	// Unlatch note cassette.
	rc = NoteCassette(false);
	
	trace(L6,_T("-PSButton4"));
	return rc ? NoteCassetteUnlatchNotify() : STATE_NULL;
}

/**
 * Function:		PSButton6
 * Purpose:			Transfer notes to cassette for pickup.
 * Parameters:		NONE
 * Returns:			STATE_NULL
 * Calls:			TransferNotes
 * Side Effects:	If authenticated (logged in as head cashier), any notes that
 *					are greater than the normal working level of the note 
 *					recycler are transferred to the note cassette for pickup.
 * Notes:			
 */
SMStateBase *SMSmCashManagementSafePayBase::PSButton6(void)
{
	(void)TransferNotes(co.nLocaleXferToCassetteMode);
	return STATE_NULL;
}

/**
 * Function:		PSButton7
 * Purpose:			Transfer all notes to cassette.
 * Parameters:		NONE
 * Returns:			STATE_NULL
 * Calls:			TransferNotes
 * Side Effects:	If authenticated (logged in as head cashier), all notes in 
 *                  the note recycler are transferred to the note cassette.
 * Notes:			
 */
SMStateBase *SMSmCashManagementSafePayBase::PSButton7(void)
{
	(void)TransferNotes();		
	return STATE_NULL;
}

/**
 * Function:		PSButton8
 * Purpose:			Perform final cleanup / counts and exit cash management.
 * Parameters:		NONE
 * Returns:			MediaStatus state if successful, error screen if note 
 *                  cassette is not present.
 * Calls:			
 * Side Effects:	
 * Notes:			
 */
SMStateBase *SMSmCashManagementSafePayBase::PSButton8(void)
{
	CString csErr;
	long rc = 0;

	trace(L6,_T("+PSButton8"));

	// Attempt to latch note cassette.  Show error screen if not successful.
	if( ! NoteCassette() )
	{
		return NoteCassetteUnlatchNotify();
	}

	EndDeposit(true);						// TAR 319896
	dm.CashRecyclerPauseCounts(false);		// TAR 311471

	rc = SetTransactionType(SP_TransType_Sale);	// Restore default type.
	if(rc != OPOS_SUCCESS)
	{
		trace(L4,_T("ERROR: Failed to set transaction type to %d; rc: %d"),
			  SP_TransType_Sale, rc);
	}

	if(DMPrintCashManagementReceipt(m_caDenomList, m_caChangeInCount))
	{
		trace(L6,_T("Failed to print receipt")); 
	}

	trace(L6,_T("User ID: %s; Cashier ID: %s"), 
			SMStateBase::csOperator, SMStateBase::csCashierID ); 

	// Report change in counts to report server.
	rc = rp.CurrencyCashManagement(m_caDenomList, m_caChangeInCount, 
	 						SMStateBase::csOperator, SMStateBase::csCashierID );

	trace(L7, _T("rp.CurrencyCashManagement returned %d"), rc);

	// enables this to generate the xml file, and not wait till a 
	// transaction finishes
	if( !fSAInTransaction )
	{ 
		rp.m_bForceEnd = true;
		(void)rp.TransactionEnd();  
		rp.m_bForceEnd = false;  
	}

	SendTBCashManagementData();		// Notify TB of changes.
	DisposeAllData(); 				// Clear all static data.

	ps.Echo( _T("") );
	trace(L6,_T("-PSButton8"));

	RETURNSTATE(SmMediaStatus);		//lint !e527 !e1774
}  //lint !e533

/**
 * Function:		PSKeystroke
 * Purpose:			Processes the Remove nn, and Unload commands.
 * Parameters:		NONE
 * Returns:			
 * Calls:			
 * Side Effects:	If logged in as the head cashier AND:
 *					If an unload button is pressed for a denomination, all 
 *					currency for that denomination is dispensed.
 *					
 *					If a removeNN button is pressed for a denomination the 
 *					configured (ScotOpts) count for that denomination is 
 *					dispensed.
 * Notes:			The remove and unload button controls are assigned 
 *                  parameters in the range of 501-516.  
 */
SMStateBase *SMSmCashManagementSafePayBase::PSKeystroke(const long wParam, const long lParam) 
{ 
    trace(L6, _T("+PSKeyStroke %d"), wParam);
    CString csChange = _T("%d:%d"); 
	CString csDispense;
    int nDispenseAmount = 0;
	bool bRemoveNN = wParam >= 501 && wParam <= 508;
	int index = bRemoveNN ? wParam - 501 : wParam - 509;

	long rc = OPOS_SUCCESS;			// Result of dispense operation.

    if( wParam < 501 || wParam > 516 ) // ignore event
    {    
		trace(L6, _T("-PSKeyStroke %d: Ignoring"), wParam);
		return STATE_NULL;
    }

	// Extra error checking.  We shouldn't be here if we're not authenticated
	// or if configuration options have not been set, but double check 
	// just in case.
	if(!m_bfAuthenticated)	// Should have logged in first.
	{
		trace(L4, _T("-PSKeyStroke:ERROR - Request to dispense attempted when not authenticated."));
		return STATE_NULL;
	}

	// Coins being removed when configuration option has not been set.
	if(bRemoveNN && !co.fLocaleAllowSoftwareRemoval)
	{
		trace(L4, _T("-PSKeyStroke:ERROR - Removal of currency attempted and \"AllowSoftwareRemoval\" config option is not enabled."));
		return STATE_NULL;
	}

	// Unload attempted when configuration option has not been set.
	if(!bRemoveNN && 
		(!co.fLocaleAllowSoftwareUnload || 
		 (m_bBill && !co.fLocaleAllowBillUnload) ))
	{
		trace(L4, _T("-PSKeyStroke:ERROR - Unload of currency attempted and \"AllowSoftwareUnload\" config option is not enabled."));
		return STATE_NULL;
	}

    ASSERT( index < GetDenomCount() );  
    if( m_bBill )
    { 
		csChange = _T(";") + csChange;      
    }
    else
    { 
		csChange += _T(";");
    }

	if (TrainingModeObj.IsTMOn() && !co.nOperationsAllowDispenseInTrainingMode) 
	{
		// in training mode and option set to not dispense
		trace(L4, _T("-PSKeyStroke:  In training mode.  No currency can be dispensed."));
		return STATE_NULL;
	}

	// End the current deposit.
	EndDeposit();

	// +TAR 355653:  If this is the initial deposit of coins, then the 
	// low-stop levels are included in the counts.  Only counts > the low stop
	// levels can be dispensed.  Refresh the count data before obtaining
	// the number of coins to be dispensed.
	//
	// TAR 361470 - Need to update counts for both coins AND notes.
	//              Previously only updated for coins.
	if(DataInitialize())   
    {
		showScreen();
    }
	// -TAR 355653

	// If the configured removal amount is greater than what is on-hand,
	// then remove everything.  Otherwise remove the configured amount.
	// If bRemoveNN == false, then this is an unload.
	if(!bRemoveNN || Count(index) < GetRemoveAmountLimit(index))
    {
        nDispenseAmount = Count(index);
	}
	else
	{
        nDispenseAmount = GetRemoveAmountLimit(index);
	}
 
	if(nDispenseAmount <= 0)
	{
		trace(L4, _T("ERROR:  Invalid dispense amount of %d was requested>"),
			nDispenseAmount);
	}
	else
	{
		csDispense.Format(csChange, DenomValue(index), nDispenseAmount);
		trace(L6,_T("csDispense : %s"), csDispense);		
		(void)DMDispenseChange( csDispense, &rc); 
	}

	// If dispense failed, then post error message.
	if(rc != OPOS_SUCCESS)
	{
		trace(L4, _T("-PSKeyStroke: ERROR - DMDispenseChange(%s) FAILED. rc: %d. Calling PostDM"), 
			  csDispense, rc);

		// TAR 354761 - Post OPOS_E_EXTENDED instead of rc.
		(void)mo.PostDM(DM_ERROR, DMCLASS_CASHRECYCLER, 0, 
			            OPOS_E_EXTENDED, NULL);
		return STATE_NULL;  // TAR 354761 - Keep recycler disabled.	
	}

    if(DataInitialize())   //If return true, refresh the screen.
    {
      showScreen();
    }
    BeginDeposit();		   //lint !e534  Errors are handled in DMParse.

    trace(L6,_T("-PSKeyStroke") );
	return STATE_NULL; 
}   //lint !e715  lParam is not used.

///////////////////////////////////////////////////////////////////////////////
// Function:    DMAcceptor                                        
// Description: resets the timeout
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSmCashManagementSafePayBase::DMAcceptor()
{  
	trace( L6, _T("+/-DMAcceptor()") );
	
	// Coin or note inserted.  Wait for any additional deposits and repaint
	// screen when the time expires.
	nTimeOut = 1;
	
	return STATE_NULL;
}

void SMSmCashManagementSafePayBase::UnInitialize()
{
	trace(L6, _T("+Uninitialize"));
	
	
	trace(L6, _T("-Uninitialize"));
}


///////////////////////////////////////////////////////////////////////////////
// Function:    TimedOut                                        
// Description: 
///////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmCashManagementSafePayBase::TimedOut(void)
{
	trace( L6, _T("TimedOut()") );
	
	if(DataInitialize())   //If return true, refresh the screen.
	{
		showScreen();
	}
	
	nTimeOut = 0;   
	return STATE_NULL;
}

/**
 * Function:		ShowColumn
 * Purpose:			Update the display of a single denomination column and the
 *					buttons below the cylinder.
 * Parameters:		index    - zero-based index of the denomination cylinder
 *					(lowest denomination to highest).
 *					bVisible - true if the column is visible.  false if it is 
 *					hidden.
 *					csImage  - Name of the image file to use for displaying the 
 *								column level (red column for low, yellow for 
 *								high, green for normal etc.).
 *					csPosition - String containing the coordinates for the 
 *					cylinder.
 * Returns:			NOTHING
 * Calls:			
 * Side Effects:	
 * Notes:			
 */
void SMSmCashManagementSafePayBase::ShowColumn( 
		long nIndex, bool bVisible, const CString &csImage,
        const CString &csPosition)
{
	//lint --e{1058}  PSX argument is const.  non-const ref OK.
	CString csID;					// Holds ID of current element.
	CString csID2;					// Holds ID of current element.
	CString csTmp;
	CString csTmp2;
	CString csCylinderTotals;		// Shows total cylinder count / value.
	long lRemoveButtonState = (long)UI::Disabled;
	long lUnloadButtonState = (long)UI::Disabled;
	COleVariant vOrder; 
	COleVariant vRet;
	COleVariant vArg;;
	const COleVariant vCol = nIndex;
	const COleVariant vShow = bVisible ? VARIANT_TRUE : VARIANT_FALSE;

	// Avoid multiple virtual method calls.
	const int removeAmnt = bVisible ? GetRemoveAmountLimit(nIndex)  : 0;
	const int changeAmnt = bVisible ? ChangeInCount(nIndex) : 0;
	const int totalCnt   = bVisible ? Count(nIndex) 		: 0;
	const int denom		 = bVisible ? DenomValue(nIndex)			: 0;

	if (m_bfAuthenticated && bVisible && Count(nIndex) > 0)
	{
		// If configured to allow removal and there is something to remove,
		// then enable the remove button below the cylinder.
		if(co.fLocaleAllowSoftwareRemoval && removeAmnt > 0)
		{
			lRemoveButtonState = (long) UI::Normal;
		}
		
		// If configured to allow currency unload, then enable the unload
		// button below the cylinder.
		if(co.fLocaleAllowSoftwareUnload && 
				(!m_bBill || co.fLocaleAllowBillUnload))
		{
			lUnloadButtonState = (long) UI::Normal;
		}
	}

	// Make white mask cover cylinder area if not visible.
	csID = _T("SmCashManagementPictureBackground"); 
	(void)m_pAppPSX->GetConfigProperty(csID, _T(""), UI::PROPERTYZORDER, vOrder);
	vOrder = bVisible ? (long)(vOrder.lVal - 1) : (long)(vOrder.lVal + 1);

	// Update cylinder image path, location and order.
	csID.Format(_T("SmCashManagementGenericCylinderArea%d"), nIndex + 1);
	m_pAppPSX->SetConfigProperty(csID, _T(""),
			UI::PROPERTYPOSITION,(COleVariant)csPosition);
	m_pAppPSX->SetConfigProperty(csID, _T(""), 
			UI::PROPERTYPICTURE, (COleVariant)csImage);
	m_pAppPSX->SetConfigProperty(csID, _T(""), UI::PROPERTYZORDER, vOrder);

	// Update denomination text for this cylinder.
	csID.Format(_T("SmCashManagementDenom%d"), nIndex + 1);
	if(bVisible)
	{	
		// Don't call DenomValue() if cylinder is not visible.
		csTmp = SMStateBase::ConvertValueToString(denom, m_bBill);
		m_pAppPSX->SetTransactionVariable(csID, csTmp);
	}
	m_pAppPSX->SetConfigProperty(csID, ps.GetCurrentContext(), 
								 UI::PROPERTYVISIBLE, vShow);

	// Update change-in-count and cylinder totals text.
	csID.Format (_T("SmCashManagementChangeInCount%d"), nIndex + 1);
	csID2.Format(_T("SmCashManagementDispenserCountOrValue%d"), nIndex + 1);

	csTmp = _T("0");
	if(co.fLocaleCashManagementDisplayCount)
	{
		csTmp.Format(_T("%d"), changeAmnt);
		csTmp2.Format(_T("%d"), totalCnt);
	}
	else
	{
		csTmp  = SMStateBase::ConvertValueToString(changeAmnt*denom, m_bBill);
		csTmp2 = SMStateBase::ConvertValueToString(totalCnt*denom,   m_bBill);
	}

	if(changeAmnt > 0)
	{
		csTmp = _T("+") + csTmp;
	}

	m_pAppPSX->SetTransactionVariable(csID, csTmp);
	m_pAppPSX->SetConfigProperty(csID, ps.GetCurrentContext(), 
								 UI::PROPERTYVISIBLE, vShow);

	m_pAppPSX->SetTransactionVariable(csID2, csTmp2);
	m_pAppPSX->SetConfigProperty(csID2, ps.GetCurrentContext(), 
								 UI::PROPERTYVISIBLE, vShow);

	// Configure remove button for this cylinder.
    //lint --e{437} suppress COleVariant args to SendCommand.
	vArg = lRemoveButtonState;
	csID = _T("SmCashManagementRemoveList2"); 
	(void)m_pAppPSX->SendCommand(csID,UI::COMMANDSETBUTTONSTATE,vRet,2,vCol,vArg);
	(void)m_pAppPSX->SendCommand(csID,UI::COMMANDSETBUTTONVISIBLE,vRet,2,vCol,vShow);
	csTmp.Format(ps.GetPSText(BTT_REMOVE) + _T("\n%d"), removeAmnt);			
	vArg = csTmp;
	(void)m_pAppPSX->SendCommand(csID, UI::COMMANDSETBUTTONTEXT, vRet, 2, vCol, vArg);
			
	// Configure unload button for this cylinder.
	vArg = lUnloadButtonState;
	csID = _T("SmCashManagementUnloadList2"), 
	(void)m_pAppPSX->SendCommand(csID,UI::COMMANDSETBUTTONSTATE,vRet,2,vCol,vArg);
	(void)m_pAppPSX->SendCommand(csID,UI::COMMANDSETBUTTONVISIBLE,vRet,2,vCol,vShow);
	vArg = ps.GetPSText(BTT_UNLOAD);
	(void)m_pAppPSX->SendCommand(csID, UI::COMMANDSETBUTTONTEXT, vRet, 2, vCol, vArg);
}

/**
 * Function:		ShowCassette
 * Purpose:			Update the display of the note cassette cylinder.
 * Parameters:		NONE
 * Returns:			NOTHING
 * Calls:			
 * Side Effects:	If coins are displayed, the cassette column is hidden.  
 *                  If notes are displayed, the cassette column is updated 
 *                  with current levels.
 *					
 * Notes:			
 */
void SMSmCashManagementSafePayBase::ShowCassette(void)
{
	//lint --e{1058}  PSX argument is const.  non-const ref OK.
	CString csInventory;
	SPInventory sinv;
	CSPCoordinate pos;	// Position of cylinder.
	CString spos;		// Position as a string.
	int nBasePos;		// Default cylinder base position.
	long lCCount  = 0;	// Total count of notes in cassette A and B.
	long lCAmount = 0;	// Total value of notes in cassette A and B.

	const CString csAreaID = _T("SmCashManagementGenericCylinderArea9");
	const CString csLabel  = ps.GetPSText(MSG_SAFEPAY_CASSETTE);
	CString csAmount;
	CString csImagePath;
	CString csImageId = _T("SmCashManagementWhiteMask");
	COleVariant vOrder;
	const COleVariant vVisible = m_bBill ? VARIANT_TRUE : VARIANT_FALSE;
    COleVariant vReturn;
	int capacity = GetCapacity();

	trace(L6, _T("+ShowCassette"));

    memset(&sinv, 0, sizeof(sinv));	// Zero-out all counts.

	// Get the total count/amount of currency in cassette and non-dispense loc.
	if(GetInventoryCounts(csInventory) == OPOS_SUCCESS &&
	   ParseInventoryCounts(sinv, csInventory))
	{
		lCCount  = sinv.a_count + sinv.b_count;
		lCAmount = sinv.a_amount + sinv.b_amount;
	}

	// FIXME - This should be indicated on the display.  If any currency is in
	// location F, then there is a problem with note(s) or coin(s) in the 
	// recycler.  This may require a service call to fix.  This condition
	// will not interfere with normal operations unless the problem is with
	// coins.  In this case an entire hopper will be marked as non-dipsensable.
	// Trace for now.
	if(sinv.f_amount > 0)
	{
		ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_DM_EVENT,
			      _T("The SafePay device indicates that a currency amount of %d is non-dispensable (Location F).  Please contact SafePay support to correct this error."), 
				  sinv.f_amount);
	}
					
	nBasePos = cylPositions[SPMAX_COLUMNS].y() + 
			   cylPositions[SPMAX_COLUMNS].h() - SPHEIGHTOFCASSETTEBOTTOMDIAMETER;

	// Cassette cylinder is not shown in coin view.
	if(! m_bBill)
	{
		pos.x(cylPositions[SPMAX_COLUMNS].x()-5);
		pos.y(cylPositions[SPMAX_COLUMNS].y()-5);
		pos.w(cylPositions[SPMAX_COLUMNS].w()+10);
		pos.h(cylPositions[SPMAX_COLUMNS].h()+10);
		csImagePath = vReturn.bstrVal;
	}	
	else 	// Default to an empty cylinder.
	{
		pos = cylPositions[SPMAX_COLUMNS];
		csImageId = _T("SmCashManagementAcceptorRedFill");
		if(lCCount <= 0)
		{
			pos.y(nBasePos);
		}
		else
		{
			pos.y((cylPositions[SPMAX_COLUMNS].h() - SPHEIGHTOFCASSETTEBOTTOMDIAMETER) *
				  (lCCount)/capacity);
			pos.y(cylPositions[SPMAX_COLUMNS].y() + 
				  cylPositions[SPMAX_COLUMNS].h() -
				    (SPHEIGHTOFCASSETTEBOTTOMDIAMETER + pos.y()) ); 
		}

		if(lCCount > GetHighList() && lCCount < capacity)
		{
			csImageId = _T("SmCashManagementAcceptorYellowFill");
		}
		else if(lCCount > 0 && lCCount < capacity)	// Normal levels.
		{
			csImageId = _T("SmCashManagementAcceptorGreenFill");
		}
	}

	(void)m_pAppPSX->GetConfigProperty( csImageId, _T(""), 
			UI::PROPERTYPICTURE, vReturn);
	csImagePath = vReturn.bstrVal;

	(void)m_pAppPSX->GetConfigProperty( _T("SmCashManagementPictureBackground"), 
			ps.GetCurrentContext(), UI::PROPERTYZORDER, vOrder );
	vOrder = m_bBill ? (long)(vOrder.lVal - 1) : (long)(vOrder.lVal + 1);
	
	m_pAppPSX->SetConfigProperty( csAreaID, ps.GetCurrentContext(), 
			UI::PROPERTYVISIBLE, VARIANT_TRUE);
	m_pAppPSX->SetConfigProperty( _T("SmCashManagementCassetteLabel"), 
			ps.GetCurrentContext(), UI::PROPERTYVISIBLE, vVisible);
	m_pAppPSX->SetConfigProperty( _T("SmCashManagementCassetteAmount"), 
			ps.GetCurrentContext(), UI::PROPERTYVISIBLE, vVisible);

	spos = pos;
	m_pAppPSX->SetConfigProperty( csAreaID, ps.GetCurrentContext(), 
			UI::PROPERTYPOSITION, (COleVariant)spos);
	m_pAppPSX->SetConfigProperty( csAreaID, ps.GetCurrentContext(), 
			UI::PROPERTYPICTURE, (COleVariant)csImagePath);
	m_pAppPSX->SetConfigProperty( csAreaID, ps.GetCurrentContext(), 
			UI::PROPERTYZORDER, vOrder );
	m_pAppPSX->SetTransactionVariable( _T("SmCashManagementCassetteLabel"), 
			(COleVariant)csLabel );	

	// Set the amount label.
	csAmount = SMStateBase::ConvertValueToString(lCAmount, true);

	m_pAppPSX->SetTransactionVariable(_T("SmCashManagementCassetteAmount"),
			csAmount);

	trace(L6, _T("-ShowCassette Count: %d, Amount: %d"), lCCount, lCAmount);
}

/**
 * Function:		DisposeAllData
 * Purpose:			Resets static variables.
 * Parameters:		NONE
 * Returns:			NOTHING
 * Calls:			SMSmCashManagementBase::DisposeAllData(); 
 * Side Effects:	Called on final exit of cash management (PSButton8).
 * Notes:			
 */
void SMSmCashManagementSafePayBase::DisposeAllData()
{
	InvStart.RemoveAll();
	InvEnd.RemoveAll();

	m_caUnfixCount.RemoveAll();
	m_SafePayLastErrorCode = OPOS_SUCCESS;
	SMSmCashManagementBase::DisposeAllData(); 
}

/**
 * Function:		GetInventoryCounts
 * Purpose:			Obtains the InventoryList property fron the SafePay device.
 *					This property is a string that contains counts for all 
 *					currency in the recycler.
 * Parameters:		csInventory (out) - Receives the string result.
 * Returns:			return code of directIO command.
 * Calls:			
 * Side Effects:	
 * Notes:			
 */
long SMSmCashManagementSafePayBase::GetInventoryCounts(CString &csInventory)
{
	long lData;
	long lRcEx;
	long rc;
	CString csTmp;

	rc = dm.DirectIO(DMCLASS_CASHRECYCLER, 0,
			   		 SP_DIO_GETINVENTORYLIST, &lData, csInventory, &lRcEx);

	if(rc != OPOS_SUCCESS)
	{
		trace(L4, _T("GetInventoryCounts: ERROR> rc: %d; exrc: %d"), rc, lRcEx);
	}
	else
	{
		// For diagnostic purposes, log the inventory list.  This string
		// contains newlines, which messes up tracing.  Replace them with a 
		// different char.
		csTmp = csInventory;
		csTmp.Replace(_T('\n'), _T(' '));  //lint !e534  Done for tracing only.
		trace(L6, _T("GetInventoryCounts>  Raw inventory string: [%s]"), 
			  (LPCTSTR)csTmp);
	}
	return rc;
}

/**
 * Function:		ParseCashCounts
 * Purpose:			Parse the currency string returned from 
 *					dm.CashChangerGetCashCounts() (if not in a deposit cycle) or
 *					dm.CashRecyclerGetDepositCounts() (if in a deposit cycle -
 *					unfixed currency.
 * Parameters:		csCounts - Input cash count string.
 *					unfixed  - If true, the string is from 
 *							   dm.CashRecyclerGetDepositCounts.  If false, the
 *							   string is from dm.CashChangerGetCashCounts().
 *					Each string is handled differently.
 * Returns:			-1 if an error occurs.  The number of denominations processed
 *					if parse was successful.
 * Calls:			
 * Side Effects:	m_caUnfixCount array is updated if unfixed parameter = true.
 *					caCountList is updated if unfixed parameter = false.
 * Notes:			Format of the input string:
 *					[<coinunit>':'<count>[','...]][';'<noteunit>':'<count>[','...]]
 */
int SMSmCashManagementSafePayBase::ParseCashCounts(const CString &csCounts, 
												   bool unfixed)
{
	ParseState cstate = SFPAY_START;
	CString csBuff = _T("");		// Temporary buffer.
	int i;							// Counter
	int j;							// Counter
	int denom;						// Temporary storage for a denomination.
	int notes = -1;					// Used to distinguish notes from coins.

	// Configure references to target arrays.
	currency_array &counts = unfixed ? m_caUnfixCount : caCountList;
	currency_array &denoms = caValueList;

	// If this is a fixed currency string, then calculate the note offset.
	if(! unfixed)
	{
		nCoinCashSeparator = -1;
	}

	trace(L6, _T("+ParseCashCounts: Input:%s; unfixed:%d"), 
		  csCounts, unfixed);   //lint !e730

	// Extract denomination / count data from the input string.
	for(i=0,j=0; i<csCounts.GetLength() && cstate!=SFPAY_ERROR && j<denoms.GetSize(); ++i)
	{
		switch(csCounts[i])
		{
			case _T(';'):			// Note separator.
				switch(cstate)
				{
					case SFPAY_START:
						if(! unfixed)
						{
							nCoinCashSeparator = 0;
						}
						csBuff.Empty();
						cstate = SFPAY_DENOM;
						notes = 1;					// Entered note territory.
						break;
					case SFPAY_COUNT:
						counts[j++] = _ttol( (LPCTSTR)csBuff );
						csBuff.Empty();
						if(! unfixed)
						{
							// nCoinCashSeparator is only calculated on full 
							// (complete) currency strings.
							nCoinCashSeparator = j;
						}
						cstate = SFPAY_DENOM;
						notes = 1;					// Entered note territory.
						break;
					default:
						cstate = SFPAY_ERROR;
				}  //lint !e788
				break;
			case _T(':'):
				switch(cstate)
				{
					case SFPAY_DENOM:
						denom =  _ttol( (LPCTSTR)csBuff ) * notes;

						// If this is an unfixed count, then find the 
						// corresponding denomination array location.
						if(unfixed)
						{
							for(j=0;j<denoms.GetSize() && denom!=denoms[j]; ++j)
							{
								;  // Do nothing.
							}

							// No array location for this denomination was 
							// found.  Abort parse.
							if(j >= denoms.GetSize())
							{
								trace(L4,_T("ERROR: Invalid denom [%d]"),denom);
								cstate = SFPAY_ERROR;
								break;
							}
						}
						else	
						{
							// Record the parsed denomination.
							// Denomination is negative for coins. 
							denoms[j] = denom;
						}
						csBuff.Empty();
						cstate = SFPAY_COUNT;
						break;
					default:
						cstate = SFPAY_ERROR;
				}  //lint !e788
				break;
			case _T(','):	// Move to next denom/count pair.
				switch(cstate)
				{
					case SFPAY_COUNT:
						counts[j++] = _ttol( (LPCTSTR)csBuff );
						csBuff.Empty();
						cstate = SFPAY_DENOM;
						break;
					default:
						cstate = SFPAY_ERROR;
				}  //lint !e788
				break;
			default:
				// If a digit or a minus sign, then add to the buffer.
				if(_istdigit(csCounts[i]) || csCounts[i] == _T('-'))
				{
					switch(cstate)
					{
						case SFPAY_START:
							csBuff.Empty();
							cstate = SFPAY_DENOM;
							//lint -fallthrough
						case SFPAY_COUNT:  
						case SFPAY_DENOM:
							csBuff += csCounts[i];
							break;
						default:
							cstate = SFPAY_ERROR;
					}  //lint !e788
				}
				else
				{
					cstate = SFPAY_ERROR;
				}
		}
	}

	if(cstate == SFPAY_COUNT) // Finish-up last count
	{
		counts[j++] = _ttol( (LPCTSTR)csBuff );
	}

	// If there are no notes in the string, then set the position to 
	// the number of denominations.
	if(nCoinCashSeparator == -1)
	{
		nCoinCashSeparator = j+1;
	}

	if(cstate == SFPAY_ERROR)
	{
		trace(L4, _T("-ParseCashCounts:  ERROR PARSING INPUT"));
		return -1;
	}
	else
	{
		trace(L6, _T("-ParseCashCounts rc: %d; nCoinCashSeparator: %d"), 
				j, nCoinCashSeparator);

		if(! unfixed)
		{
			// Set the size of these arrays to the number of parsed denominations.
			denoms.SetSize(j);
			counts.SetSize(j);
		}
		return j;
	}
}

/**
 * Function:		ParseInventoryCounts
 * Purpose:			Parse the inventory string returned from GetInventoryCounts
 *					Values are extracted when entering and leavingcash 
 *					management (PSButton8).  The differences between the two 
 *					sets of data are used to determine the net change in counts.
 *
 *					Values in this string are also used to determine the counts
 *					for the note cassette.
 * Parameters:		inv - Reference to a SPInventory structure that will receive
 *					      the updated counts.
 *					csInventory - InventoryList string to be parsed.
 * Returns:			true if successful.  false otherwise.
 * Calls:			
 * Side Effects:	
 * Notes:			There is no separator to indicate the transition from coins 
 *					to notes (';') in this string.
 *					Format of inventory string:
 *		inventorylist: {location}‘:’<currency code>‘:’{unitcount}LF[{inventorylist}]
 *		(Note: Currency code is ISO4217. LF is ASCII character 0x0A).
 *		location: ‘F’|‘S’|‘A’|‘B’
 *		(Note: ‘F’ non-dispensable, ‘S’ internal (fixed only), ‘A’ cassette front, ‘B’ cassette rear).
 *		 unitcount: <unit>‘,’<count>[‘:’{unitcount}]
 *
 *		If counts for a particular denomination or location are zero, then no 
 *		entries are present in the string for the location/denom.
 */
bool SMSmCashManagementSafePayBase::ParseInventoryCounts(SPInventory &inv,
												  const CString &csInventory)
{
	ParseState cstate = SFPAY_START;
	CString csBuff = _T("");	// Temporary buffer.
	int denom=0;				// Current denomination in string.
	int count=0;				// Current count in string.
	long null_count;			// Temporary holder for counts of location
								// S and F.
	long *pcnt = &null_count;	// Points to ccount or fcount.
	long *pamt = &null_count;	// Points to camount or famount.

	// Remove any previouse counts from InvEnd.  Only want current counts.
	InvEnd.RemoveAll();

	// Determine the target map for storing inventory counts.
	currency_map &cmap = m_bfFirstEntry ? InvStart : InvEnd;

	m_bfFirstEntry = false;			// Use InvEnd on subsequent calls.
    memset(&inv, 0, sizeof(inv));	// Zero-out all counts.

	// Parse the inventory list.
	for(int i=0; i<csInventory.GetLength() && cstate != SFPAY_ERROR; ++i)
	{
		switch(csInventory[i])
		{
			case _T('A'):
			case _T('B'):
			case _T('S'):				
			case _T('F'):					
				switch(cstate)
				{
					case SFPAY_START:
						cstate = SFPAY_LOCATION;

						// Set locations for storage of totals.
						if(csInventory[i] == _T('A'))	// Front cassette
						{
							pcnt = &inv.a_count;
							pamt = &inv.a_amount;
							break;
						}

						if(csInventory[i] == _T('B'))	// Rear cassette
						{
							pcnt = &inv.b_count;
							pamt = &inv.b_amount;
							break;
						}

						if(csInventory[i] == _T('S'))	// Internal fixed
						{
							pcnt = &null_count;
							pamt = &inv.s_amount;
							break;
						}

						if(csInventory[i] == _T('F'))	// Non-dispensable
						{
							pcnt = &null_count;
							pamt = &inv.f_amount;
							break;
						}

						break;
					case SFPAY_CCODE:		// Don't validate currency code.
						break;
					default:
						cstate = SFPAY_ERROR;
				}  //lint !e788
				break;
			case _T('\n'):
				switch(cstate)
				{
					case SFPAY_COUNT:
						count = _ttoi(csBuff);
						*pamt += denom * count;
						*pcnt += count;
						cmap[denom] += count; // Add this count to the map.
						csBuff.Empty();
						pcnt = pamt = &null_count;
						cstate = SFPAY_START;  // Begin next location.
						break;
					default:
						cstate = SFPAY_ERROR;
				}  //lint !e788
				break;
			case _T(':'):
				switch(cstate)
				{
					case SFPAY_LOCATION:
						cstate = SFPAY_CCODE;
						break;
					case SFPAY_CCODE:
						cstate = SFPAY_DENOM;
						csBuff.Empty();
						break;
					case SFPAY_COUNT:
						count = _ttoi(csBuff);
						*pamt += denom * count;
						*pcnt += count;

						// Add this count to the map.
						cmap[denom] += count;

						csBuff.Empty();
						cstate = SFPAY_DENOM;
						break;
					default:
						cstate = SFPAY_ERROR;
				}  //lint !e788
				break;
			case _T(','):
				switch(cstate)
				{
					case SFPAY_DENOM:
						denom = _ttoi(csBuff);
						csBuff.Empty();
						cstate = SFPAY_COUNT;
						break;
					default:
						cstate = SFPAY_ERROR;
				}  //lint !e788
				break;
			default:
				if(cstate == SFPAY_CCODE)
				{
					continue;
				}

				if(_istdigit(csInventory[i]))
				{
					if(cstate == SFPAY_DENOM || cstate == SFPAY_COUNT)
					{
						csBuff += csInventory[i];
					}
				}
				else
				{
					trace(L4, _T("Invalid char '%c' in state %d"), 
						csInventory[i], cstate);
					cstate = SFPAY_ERROR;
				}
		}
	}

	return cstate != SFPAY_ERROR;
}

/**
 * Function:		NoteCassette
 * Purpose:			Lock or unlock the note recycler cassette.
 * Parameters:		bLatch - If true, latch (lock) the cassette.
 *							 If false, unlock the cassette.
 * Returns:			true if successful.
 * Calls:			
 * Side Effects:	
 * Notes:			
 */
bool SMSmCashManagementSafePayBase::NoteCassette(bool bLatch)
{
	long lData = co.nLocaleNoteCassetteLocation;
	long lRcEx;
	CString csData;
	long rc;
	long iaction = bLatch ? SP_Latch_Cassette : SP_UnLatch_Cassette;

	trace(L6,_T("+NoteCassette(%d); Location: %d"), bLatch, lData); //lint !e730

	// If in deposit mode, then the cassette must already be latched.
	if(bLatch && dm.CashRecyclerGetEnabled())
	{
		trace(L6, _T("-NoteCassette:  Already latched."));
		return true;
	}

	// Don't allow unlatch of note cassette in training mode.
	if(!bLatch && TrainingModeObj.IsTMOn() && !co.nOperationsAllowDispenseInTrainingMode) 
	{
		trace(L6, _T("-NoteCassette:  In training mode.  Note cassette cannot be unlatched."));
		return false;
	}

	if(!bLatch && dm.CashRecyclerGetEnabled())
	{
		// The note cassette cannot be unlatched if in a deposit cycle.
		EndDeposit();
	}

	// Latch command is in the upper word of lData.
	lData |= (iaction << 16);  //lint !e703

	// Latch / Unlatch the cassette.
	rc = dm.DirectIO(DMCLASS_CASHRECYCLER, 0,
			   		 SP_DIO_LATCH_CASSETTE, &lData, csData, &lRcEx);

	if(rc == OPOS_E_ILLEGAL && bLatch)	// Cassette is already latched.
	{
		trace(L6, _T("Latch of note cassette while in deposit mode."));
	}

	if(rc != OPOS_SUCCESS)
	{
		trace(L4, _T("ERROR: NoteCassette(%d) FAILED. rc:%d; exrc:%d"),
				bLatch, rc, lRcEx);  //lint !e730
	}
	else
	{
		// Log latch / unlatch to report server.  Don't log duplicate
		// latch attempts, but always log unlatches.
		if(!bLatch || m_bNoteCassetteLatched != bLatch)  //lint !e731
		{		
			csData = bLatch ? _T("LOCKED") : _T("UNLOCKED");
			(void)rp.ModeChange(rp.m_lEntryID++, MODE_NOTECASSETTE, csData);

			// Trace this activity.
			trace(L4, _T("Note Cassette: %s"), (LPCTSTR)csData);
		}

		m_bNoteCassetteLatched = bLatch; 
	}

	trace(L6,_T("-NoteCassette"));
	return rc == OPOS_SUCCESS;
}

/**
 * Function:		TransferNotes
 * Purpose:			Transfer notes from the note recycler wheel to the note 
 *                  cassette using the specified SafePay transfer mode.
 * Parameters:		mode - SafePay transfer mode.
 * Returns:			OPOS_E_ILLEGAL if not logged-in as head cashier or if in 
 *                  coin display.  Result of SO DirectIO otherwise.
 * Calls:			
 * Side Effects:	
 * Notes:			
 */
long SMSmCashManagementSafePayBase::TransferNotes(int mode)
{
	long lData = (long)(co.nLocaleNoteCassetteLocation | (mode << 16));  //lint !e701
	long lRcEx;
	CString csData;
	long rc;

	ASSERT(m_bBill);		// Shouldn't be able to get here otherwise.

	trace(L6,_T("+TransferNotes: Cassette=%d; Mode=%d"), 
					co.nLocaleNoteCassetteLocation, mode);

	// Sanity check.  
	if(! m_bBill || ! m_bfAuthenticated)
	{
		/*lint -e(730) */
		trace(L4, SFPAY_AERR, _T("-TransferNotes"),m_bBill,m_bfAuthenticated);
		return OPOS_E_ILLEGAL;
	}
	
	if (TrainingModeObj.IsTMOn() && !co.nOperationsAllowDispenseInTrainingMode) 
	{
		trace(L4, _T("-TransferNotes:  Operation not performed while in training mode."));
		return OPOS_SUCCESS;
	}

	// Note transfer cannot occur in a deposit cycle.
	EndDeposit();

	// Transfer the notes.
	rc = dm.DirectIO(DMCLASS_CASHRECYCLER, 0,
			   		 SP_DIO_NOTE_TRANSFER, &lData, csData, &lRcEx);

	if(rc != OPOS_SUCCESS)
	{
		trace(L4, _T("ERROR: Note transfer to cassette FAILED. rc:%d; exrc:%d"),
				rc, lRcEx);  
	}

	(void)DataInitialize();	// Get latest note counts;

	// Update display.  
	showScreen();
	
	// Return to deposit mode.
	BeginDeposit();			//lint !e534  Errors are handled in DMParse.

	trace(L6,_T("-TransferNotes"));  
	return rc;
}

/**
 * Function:		NoteCassetteUnlatchNotify
 * Purpose:			Display an error message if note cassette is not present.
 * Parameters:		NONE.
 * Returns:			SystemMessage state.
 * Calls:			
 * Side Effects:	
 * Notes:			
 */
SMStateBase *SMSmCashManagementSafePayBase::NoteCassetteUnlatchNotify(void)
{
	CString csErr = _T("DeviceErrorCashRecycler.xml:CHAN_ERROR_CASSETTE_UNLATCHED");

	return createLevel1Message(csErr,
				  PS_NOTEXT,        // no canned message
				  PS_NOTEXT,        // no prompt
				  0,                // no value
				  DMCLASS_CASHRECYCLER,// device reporting the error or -1
				  false);            // T/F need to issue CheckHealth.
}

/**
 * CSPCoordinate class implementation
 * Convenience class for specifying and manipulating PSX coordinate strings.
 */
CSPCoordinate::CSPCoordinate() : _x(-1), _y(-1), _w(-1), _h(-1)
{
}

CSPCoordinate::CSPCoordinate(const CString &csPosition)
{
	*this = csPosition;
}

CSPCoordinate::CSPCoordinate(int x, int y, int w, int h) :
_x(x), _y(y), _w(w), _h(h)
{
}

CSPCoordinate::CSPCoordinate(const CSPCoordinate &rhs)  
{   //lint !e1538  CObject default constructor is OK.
	*this = rhs;
}

CSPCoordinate::operator LPCTSTR()
{
	csStr.Format(_T("%d,%d,%d,%d"), _x, _y, _w, _h);
	return (LPCTSTR)csStr;
}

void CSPCoordinate::operator=(const CString &rhs)
{
	csStr = rhs;
	int rc;

	trace(L6, _T("Assigning coordinate string \"%s\""), (LPCTSTR)rhs); 
	rc = _stscanf(csStr, _T("%d,%d,%d,%d"), &_x, &_y, &_w, &_h);

	if(rc == 0)
		trace(L6, _T("Assignment FAILED"));  
}

void CSPCoordinate::operator=(const CSPCoordinate &rhs)
{
	if(this == &rhs)
	{
		return;
	}

	_x = rhs._x;
	_y = rhs._y;
	_w = rhs._w;
	_h = rhs._h;

	csStr = rhs.csStr;
}
