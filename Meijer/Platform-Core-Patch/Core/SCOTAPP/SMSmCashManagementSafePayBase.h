////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCashManagementSafePayBase.h
//
// TITLE: Class header for store mode CashManagement state
//
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _SmCashManagementSafePayBaseSTATE
#define _SmCashManagementSafePayBaseSTATE

#define SPMAX_COLUMNS 8
#define SPHEIGHTOFBOTTOMDIAMETER 13				// TAR 360882
#define SPHEIGHTOFCASSETTEBOTTOMDIAMETER 20		// TAR 360882

#include "SMSmCashManagement.h"
#include "SafePayApi.h"			// Need SP_TransType_XXX enumerations.
#include "SafePayC.h"

#define SFPAY_AERR    _T("-%s ERROR:  Button should not be enabled!  m_bBill: %d; m_bfAuthenticated: %d")		
#define SFPAY_CFG_ERR _T("Index [%d]: Option \"%s\" or \"CashValueList\" is configured incorrectly.")

typedef CMap<int, int, int, int> currency_map;
typedef CArray<int, int> currency_array;

// Class that represents a coordinate of x, y, w, h
// Used for cylinder positioning.
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
CSPCoordinate : public CObject
{
public:
	CSPCoordinate();
	explicit CSPCoordinate(const CString &csPosition);
	CSPCoordinate(int x, int y, int w, int h);
	CSPCoordinate(const CSPCoordinate &rhs);

	operator LPCTSTR();
	void operator=(const CString &rhs);
	void operator=(const CSPCoordinate &rhs);

	inline int x(void) const { return _x; }
	inline int y(void) const { return _y; }
	inline int w(void) const { return _w; }
	inline int h(void) const { return _h; }

	inline void x(int nx) { _x = nx; }
	inline void y(int ny) { _y = ny; }
	inline void w(int nw) { _w = nw; }
	inline void h(int nh) { _h = nh; }
private:
	int _x;
	int _y;
	int _w;
	int _h;
	CString csStr;		// Holds the string representation of coordinates.
};

typedef struct 
{
	long s_amount;		// Total amount of currency in inventory location S.
	long a_count;		// Total count of notes in cassette A (front cassette).
	long a_amount;		// Total value of notes in cassette A.
	long b_count;		// Total count of notes in cassette B (rear cassette).
	long b_amount;		// Total value of notes in cassette B.
	long f_amount;		// Total amount of currency in inventory location F.
} SPInventory;


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
SMSmCashManagementSafePayBase : public STATE(SmCashManagement)
{
public:
	SMSmCashManagementSafePayBase();			 // constructor
	virtual SMStateBase  *Initialize(void);      // required override
	virtual void UnInitialize();
	virtual SMStateBase  *TimedOut(void);

	// State definitions for inventory list and cash count parsers.
	const enum ParseState
	{
		SFPAY_START, SFPAY_LOCATION, SFPAY_CCODE, SFPAY_DENOM, 
		SFPAY_COUNT, SFPAY_ERROR,    SFPAY_IGNORE
	};

protected :
	virtual bool DataInitialize(void);	
	virtual void showScreen();

	virtual SMStateBase  *PSButton1(void); 
	virtual SMStateBase  *PSButton2(void); // Remove Cash / Coins
	virtual SMStateBase  *PSButton3(void); // Unlatch Note Cover
	virtual SMStateBase  *PSButton4(void); // Unlatch Note Cassette
	virtual SMStateBase  *PSButton6(void); // Transfer notes for pickup.
	virtual SMStateBase  *PSButton7(void); // Transfer all notes to cassette.
	virtual SMStateBase  *PSButton8(void); // Go Back button 
	virtual SMStateBase *PSKeystroke(const long wParam, const long lParam);
	virtual SMStateBase *DMAcceptor(void); // Process inserted currency.

	virtual bool GetNewCylinderPosition(int i, bool bVisible, 
			CString& csImage, CSPCoordinate &pos);

	
	virtual bool OneTimeInit(void);			// One-time initialization.

	// Wrapper methods for accessing array data.
	virtual int Count(int index) const;
	virtual int GetDenomCount(void) const;
	virtual int DenomValue(int index) const;
	virtual int ChangeInCount(int index) const;

	virtual int GetRemoveAmountLimit(int index) const;
	virtual int GetCapacity(int index=-1) const;
	virtual int GetHighList(int index=-1) const;
	virtual int GetLowList(int index) const;			

    virtual bool BeginDeposit(void);
	virtual void EndDeposit(bool finalize=false);
	virtual long SetTransactionType(long type);

	virtual void ShowColumn(long nIndex, bool bVisible, 
							const CString &csImage, const CString &csPosition);
							
	
	virtual void ShowCassette(void);
	virtual void DisposeAllData();

	// Retrieve inventory counts from the device.
	virtual long GetInventoryCounts(CString &csInventory);

	virtual int  ParseCashCounts(const CString &csCounts, bool unfixed=true);
	virtual bool ParseInventoryCounts(SPInventory &inv,
									  const CString &csInventory);

	// Utility method to latch / unlatch note cassette.  
	// Default is to latch cassette.
	virtual bool NoteCassette(bool bLatch=true);

	// Utility method to transfer notes to cassette.
	virtual long TransferNotes(int mode = 0);

	// Returns a System error message for cassette latch error.
	SMStateBase *NoteCassetteUnlatchNotify(void);

	// Stores initial coordinates of denomination cylinders.  
	// Last position is for the cassette cylinder.
	static CSPCoordinate cylPositions[SPMAX_COLUMNS+1];	

	static int offset;
	static currency_array m_caUnfixCount;
	static currency_array m_caDenomList;
	static bool m_bNoteCassetteLatched;

	// Hash tables to record the initial and final currency levels in
	// the front and rear cassettes (locations A and B) and non-dispensable
	// (location F).  Hash tables are used because it is possible that some
	// denominations might be accepted, but not dispensed.
	// The maps will contain the counts for each denomination in locations
	// A, B and F.  InvStart stores the counts when cash management is initially
	// entered.  InvEnd stores the counts when cash management is exited
	// (by PSButton8()).
	static currency_map InvStart;	
	static currency_map InvEnd;		

	// MFC Runtime class/object information
	DECLARE_DYNCREATE(SMSmCashManagementSafePayBase)
};

#endif  // _SmCashManagementSafePayBaseSTATE
