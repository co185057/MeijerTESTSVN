// SafePayApi.h: interface for the CSafePayApi class.
//
//////////////////////////////////////////////////////////////////////

// This class is an API Wrapper class for CCC.dll 
// as documented in the SafePay Protocol 2.1 


#if !defined(AFX_SAFEPAYAPI_H__B858A64F_6498_4538_BB29_625751863D55__INCLUDED_)
#define AFX_SAFEPAYAPI_H__B858A64F_6498_4538_BB29_625751863D55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// SafePay Cassette Locations

enum
{
	SP_Cassette_Front, 
	SP_Cassette_Rear
};


// SafePay Dispense ExchangeModes
enum 
{
	SP_EM_LeastCurrency,	// Least number of notes / coins
	SP_EM_LeastCurrencyNoTop		// Least number of notes / coins not using highest denomination 
};


// SafePay Transaction Type values - used in Dispense
enum
{
	SP_TransType_Sale,
	SP_TransType_Return,
	SP_TransType_Float,
	SP_TransType_Bank,
	SP_TransType_Exchange,
	SP_TransType_CashBack,
};


// SafePay Property Indexes
// Note: these values must be kept in sync with the 
//		 CCC.dll property indexes as documented in the
//	     Safepay Protocol and API document. Otherwise
//		 properties access may not work.  

enum 
{
	SpPropertyMinProp = 0, 
    SpPropertyCurrencyCode,					// STR	RW
    SpPropertyCurrencyCodeList,				// STR  R	
    SpPropertyDepositCodeList,				// STR  R
    SpPropertyCurrencyCashList,				// STR	R
    SpPropertyExitCashList,					// STR	R
    SpPropertyDepositCashList,				// STR  R
    SpPropertyDepositCashCount,				// STR  R
    SpPropertyDispenseCashCount,			// STR	RW
    SpPropertyCashCount,					// STR  R
    SpPropertyDiscrepancy,					// BOOL R
    SpPropertyDepositStatus,				// LONG - Don't use per SafePay protocol document
    SpPropertyDeviceStatus,					// LONG R
    SpPropertyFullStatus,					// LONG R
    SpPropertyCheckHealthText,				// STR	RW
    SpPropertyDispenseAmount,				// LONG RW
    SpPropertyDepositAmount,				// LONG R
    SpPropertyOperatorId,					// STR  RW
    SpPropertyDateTime,						// STR	RW
    SpPropertySelectCassette,				// SHORT RW
    SpPropertySelectSPUnit,					// SHORT RW
    SpPropertyDumpMode,						// SHORT RW
    SpPropertyBypassWithValue,				// LONG RW
    SpPropertyBaseLevelFill,				// BOOL - Don't use per SafePay protocol document
    SpPropertyTransType,					// SHORT RW
    SpPropertySelectExchangeMode,			// SHORT RW
    SpPropertyEnableEvents,					// BYTE RW
    SpPropertyInventoryList,				// STR	RW
	SpPropertyMaxProp
};


// SafePay Event Id values - These SafePay events are used on SafePay callbacks
enum
{
	SpEventDeviceStatus = 1, 
	SpEventDepositAmount, 
	SpEventDispenseAmount, 
	SpEventInventoryList, 
	SpEventCheckHealthText
};

// Event masks for EnableEvents Property or Register Event Handler Api 
const BYTE SpEventsMaskLow3				= 0x01;		// Enable low 3 events
const BYTE SpEventsDeviceStatusChanged	= 0x02;		// Device status changed mask
const BYTE SpEventsDepositAmountChanged = 0x04;		// Deposit Amount changed 
const BYTE SpEventsDispenseCompleted	= 0x08;		// Dispense Completed 
const BYTE SpEventsInventoryChanged		= 0x10;		// Inventory List Changed 
const BYTE SpEventsCheckHealthText		= 0x20;		// Check Health Text Changed 
const BYTE SpEventsMask = SpEventsDeviceStatusChanged 
							| SpEventsDepositAmountChanged 
							| SpEventsDispenseCompleted;
							// | SpEventsCheckHealthText;

// SafePay Command Results
// Note - 200 (OPOSEXTERR) is added to these values when returned in an extended error

const LONG SP_RESULT_OK					= 0;		// 0, Success 
const LONG SP_RESULT_NOT_INITIALIZED	= -1;		// -1, Not Initialized 
const LONG SP_RESULT_FAILED				= -1;		// -1, Failed 
const LONG SP_RESULT_UNKNOWN			= 1;		// 1, unknown SafePay error 
const LONG SP_RESULT_ACCESS_DENIED		= 2;		// 2, access denied 
const LONG SP_RESULT_BUSY				= 3;		// 3, busy - retry - should not happen via API 
const LONG SP_RESULT_INVALID_VALUE		= 4;		// 4, property can't accept, invalid value
const LONG SP_RESULT_BUSY_OTHER			= 5;		// 5, busy with other client
const LONG SP_RESULT_CERTIFICATE		= 6;		// 6, certificate required
const LONG SP_RESULT_JAM_BASE			= 19;		// 19, - jams will be 20 - 33 
const LONG SP_RESULT_JAM_COIN_DEPOSIT	= 20;		// 20, Coin Deposit Jam 
const LONG SP_RESULT_JAM_NOTE_DEPOSIT	= 21;		// 21, Note Deposit Jam
const LONG SP_RESULT_JAM_COIN_DOOR_OPEN	= 22;		// 22, Coin Unit Door Open
const LONG SP_RESULT_JAM_NOTE_COVER		= 23;		// 23, Note Unit Cover Open
const LONG SP_RESULT_JAM_HOPPER			= 24;		// 24, Coin Hopper Locked 
const LONG SP_RESULT_JAM_CASSETTE		= 25;		// 25, Cassette Jam
const LONG SP_RESULT_JAM_COINIO			= 26;		// 26, Coin IO Error
const LONG SP_RESULT_JAM_NOTE_VAL_IO	= 27;		// 27, Note Validator IO Error
const LONG SP_RESULT_JAM_NOTE_LIB		= 28;		// 28, Note Library Error
const LONG SP_RESULT_JAM_NOTE_GENERAL	= 29;		// 29, Note General Error
const LONG SP_RESULT_JAM_COIN_GENERAL	= 30;		// 30, Coin General Error
const LONG SP_RESULT_JAM_RESOURCE		= 31;		// 31, Resource Error
const LONG SP_RESULT_JAM_OPENSTATE		= 32;		// 32, Open State invalid 
const LONG SP_RESULT_SP_ERROR			= 50;		// 50, Error
const LONG SP_RESULT_PICK_FAIL			= 51;		// 51, Note internal Pickup failed 
const LONG SP_RESULT_EXIT_RETRY			= 52;		// 52, Fail ejecting currency
const LONG SP_RESULT_FATAL_JAM			= 53;		// 53, Fatal Jam
const LONG SP_RESULT_LIMBO_JAM			= 54;		// 54, Limbo Jam
const LONG SP_RESULT_FULL				= 61;		// 61, Full 
const LONG SP_RESULT_DEPOSIT_UNIT		= 201;		// 201, deposit unit busy
const LONG SP_RESULT_UNLATCHED			= 202;		// 202, deposit unit unlatched
const LONG SP_RESULT_DEPOSIT_HW_CONFIG	= 203;		// 203, deposit unit wrong type
const LONG SP_RESULT_COIN_UNIT_ERROR 	= 204;		// 204, coin unit error
const LONG SP_RESULT_UNKNOWN2			= 255;		// 255, Unknown Error
const LONG SP_RESULT_TIMEOUT			= 256;		// 256, Timeout
const LONG SP_RESULT_NO_SPIN			= 257;		// 257, Note Unit wheel won't spin
const LONG SP_RESULT_NOTE_JAM			= 258;		// 258, Note Jam
const LONG SP_RESULT_COIN_JAM			= 260;		// 260, Coin Jam
const LONG SP_RESULT_DEPOSIT_SENSOR		= 401;		// 401, deposit unit sensor blocked
const LONG SP_RESULT_DEPOSIT_MEMORY		= 402;		// 402, deposit unit memory corrupt
const LONG SP_RESULT_DEPOSIT_RESET		= 403;		// 403, deposit unit needs reset
const LONG SP_RESULT_DEPOSIT_UNIT_DYED	= 404;		// 404, deposit unit dyed
const LONG SP_RESULT_INVALID_CASSETTE	= 80;		// 80, invalid cassette id 
const LONG SP_RESULT_INVALID_DUMPMODE	= 81;		// 81, invalid dump mode 
const LONG SP_RESULT_INVALID_LATCHCODE	= 82;		// 82, invalid latch code
const LONG SP_RESULT_LOAD_FAILED		= 83;		// 83, unable to load dll 
const LONG SP_RESULT_OFFLINE1			= 0x80000001; // offline - no protocol	
const LONG SP_RESULT_OFFLINE2			= 0x80000002; // offline - not started 	
const LONG SP_RESULT_OFFLINE3			= 0x80000003; // offline - no response	
const LONG SP_RESULT_OFFLINE4			= 0x80000004; // offline - illegal reentry	
const LONG SP_RESULT_OFFLINE5			= 99;		// 99, offline 


#pragma pack(push,1)

// Safepay Status bits 
/*
			Bit0 SafePay is open.
			Bit1 Dispense active. Set while dispense is performed.
			Bit2 Deposit active. Set while deposit state is active.
			Bit3 Deposit fixed. Set when deposit has been fixed.

			Bit8 Coin feeder running.
			
			Bit12 Urgent message pending.
			Bit13 Non-zero FullStatus
			
			Bit16 Note unit I/O error.
			Bit17 Jamming notes. A problem within the note feeding mechanism.
			Bit18 Jamming notes. A problem in the transition to cassette.
			Bit19 Generel note unit error (e.g. top-cover open).
			Bit20 Note reader unit I/O error.
			Bit21 Note library error.
			Bit23 General note reader unit error.
			Bit24 Coin unit I/O error.
			Bit25 Error detected paying out coins (e.g. hopper error).
			Bit26 Jamming coins. A problem within the coin sorting mechanism.
			Bit27 General coin unit error.
			Bit28 Internal error (e.g. coin-unit door open).
			Bit29 Error on internal resources.

*/

typedef struct SP_Status
{
public: 
	unsigned	Open			: 1;	// 0 SafePay is Open
	unsigned	Dispensing		: 1;	// 1 Dispense is active
	unsigned	Depositing		: 1;	// 2 Deposit is active
	unsigned	DepositFixed	: 1;	// 3 Deposit is fixed
	unsigned	Filler1			: 4;	// 4-7
	unsigned	CoinFeedRun		: 1;	// 8 Coin Feeder Running
	unsigned	Filler2			: 3;	// 9-11
	unsigned	CheckHealth		: 1;	// 12 CheckHealth Message Pending
	unsigned	FullStatus		: 1;	// 13 Full Status is set
	unsigned	Filler3			: 2;	// 14-15
	unsigned	NoteIOError		: 1;	// 16 Note Unit IO Error
	unsigned	JamNoteFeed		: 1;	// 17 Jammed Notes in note feeder
	unsigned	JamNoteCassette : 1;	// 18 Jammed Notes in Cassette
	unsigned	GenNoteError	: 1;	// 19 General Note Error
	unsigned	NoteReadIOError	: 1;	// 20 Note Reader IO Error
	unsigned	NoteLibError	: 1;	// 21 Note Library Error
	unsigned	Filler4			: 1;	// 22 
	unsigned	NoteReadGenError: 1;	// 23 Note Reader Gen Error
	unsigned	CoinUnitError	: 1;	// 24 Coin Unit IO Error
	unsigned	JamCoinHopError	: 1;	// 25 Coin Jam Hopper ( Deposit ) 	
	unsigned	JamCoinSortError: 1;	// 26 Coin Jam Sorter ( Deposit ) 
	unsigned	CoinUnitGenError: 1;	// 27 Coin Unit Gen Error
	unsigned	InternalError	: 1;	// 28 Internal Error
	unsigned	ResourceError	: 1;	// 29 Resource Error 
	unsigned	Filler5			: 2;	// 30-31
	SP_Status()
	{
		*this = 0;
	}
	SP_Status( const long& val )
	{
		*this = val;
	}
	SP_Status& operator=( const long& val )
	{
		if	( this !=  ( SP_Status * ) &val )
		{
			* ( long * ) this = val;
		}
		return *this;
	}
	operator long()
	{
		long l = * ( long * ) this;
		return l;
	}
} SPStatus;


// SafePay FullStatus - as returned by CCC_FullStatus 
/*
		Each element consists of two bits, 
		specifying full status of a particular compartment.
		Each may assume values: 0 = OK, 1 = near full, 2 = overflow.

			Bit0-Bit1 Hopper 1 (smallest coin denomination).
			Bit2-Bit3 Hopper 2.
			Bit4-Bit5 Hopper 3.
			Bit6-Bit7 Hopper 4.
			Bit8-Bit9 Hopper 5.
			Bit10-Bit11 Hopper 6.
			Bit12-Bit13 Hopper 7.
			Bit14-Bit15 Hopper 8.
			Bit16-Bit17 Front deposit cassette unit.
			Bit18-Bit19 Rear deposit cassette unit.
*/
 
typedef struct SP_FullStatus
{
public:
	unsigned	Hopper0			: 2;
	unsigned	Hopper1			: 2;
	unsigned	Hopper2			: 2;
	unsigned	Hopper3			: 2;
	unsigned	Hopper4			: 2;
	unsigned	Hopper5			: 2;
	unsigned	Hopper6			: 2;
	unsigned	Hopper7			: 2;
	unsigned	FrontCassette	: 2;
	unsigned	BackCassette	: 2;
	SP_FullStatus()
	{
		*this = 0;
	}
	SP_FullStatus( const long& val )
	{
		*this = val;
	}
	SP_FullStatus& operator=( const long& val )
	{
		if	( this != ( SP_FullStatus * ) & val )
		{
			* ( long * ) this = val;
		}
		return *this;
	}
	operator long()
	{
		long l = * ( long * ) this;
		return l;
	}
}SPFullStatus;

#pragma pack(pop)


// SafePay bin Full States - bit values for Full States
enum
{
	SpFullOk,
	SpFullNearFull,
	SpFullOverFull,
	SpFullUnknown
};

// SafePay Exception - SafePay API failed. Throw the result code, function and a message

class CSafePay_Exception 
{
public:		
	CSafePay_Exception(){};
	CSafePay_Exception( LPCSTR szErrorMsg, LPCSTR szFunctionName, long nErrorCode )
		: m_ErrorCode( nErrorCode ), 
		  m_ErrorString( szErrorMsg ),
		  m_ErrorFunction( szFunctionName )
	{
	}
	CSafePay_Exception( CString& csErrorMsg, LPCSTR szFunctionName, long nErrorCode )
		: m_ErrorCode( nErrorCode ), 
		  m_ErrorString( csErrorMsg ),
		  m_ErrorFunction( szFunctionName )
	{
	}
	CSafePay_Exception( const CSafePay_Exception& exc)
		: m_ErrorCode( exc.m_ErrorCode ),
		  m_ErrorString( exc.m_ErrorString ),
		  m_ErrorFunction( exc.m_ErrorFunction )
	{

	}

	virtual ~CSafePay_Exception(){};

	const	long	 GetErrorCode()		{ return m_ErrorCode; };
	const	CString& GetErrorString()	{ return m_ErrorString; };
	const   CString& GetFunctionName()  { return m_ErrorFunction; };

	const CSafePay_Exception& operator=( const CSafePay_Exception& exc )
	{
		if	( this != &exc ) 
		{
			m_ErrorCode		= exc.m_ErrorCode;
			m_ErrorString	= exc.m_ErrorString;
			m_ErrorFunction = exc.m_ErrorFunction;
		}
		return *this;
	}

private:
	long	m_ErrorCode;
	CString	m_ErrorString;
	CString m_ErrorFunction;
};

// SafePay Api result string lengths 
const LONG 	 SP_CURRENCY_INFO_MAX			= 65;
const LONG 	 SP_CURRENCY_UNIT_INFO_MAX		= 512;
const LONG 	 SP_DEPOSIT_STRING_MAX			= 512;
const LONG	 SP_INVENTORYLIST_STRING_MAX	= 4096;

// SafePay Note Transfer dump modes

enum
{
	SP_ClaimMode3 = -3,		// Move notes to recycler to reach preconfigured total amount in wheel
	SP_ClaimMode2 = -2,		// Move notes to recycler to reach a preconfigured note distribution base level
	SP_ClaimMode1 = -1,		// Move notes to recycler to reach a preconfigured note distribution work level
	SP_DumpMode0 = 0,		// All notes are dumped to cassette
	SP_DumpMode1 = 1,		// Notes are dumped to cassette, leaving recycler with backoffice defined quanities.  
	SP_DumpMode2 = 2,		// Notes are dumped to cassette, leaving recycler with backoffice defined quanities.  	
	SP_DumpMode3 = 3,		// A Predefined total value of notes remain in recycler, others are dumped 
	SP_DumpMode16 = 16,		// Like 0, but cassette location overrides configuration
	SP_DumpMode17 = 17,		// Like 1, but cassette location overrides configuration
	SP_DumpMode18 = 18,		// Like 2, but cassette location overrides configuration
	SP_DumpMode19 = 19,		// Like 3, but cassette location overrides configuration
};


// SafePay Jam Text 
const LPCTSTR 	Jam_CoinDeposit			= _T( "Jam in coin deposit" );
const LPCTSTR 	Jam_NoteDeposit			= _T( "Jam in note deposit" );
const LPCTSTR	Jam_CoinDoor			= _T( "Jam, coin door open" );
const LPCTSTR	Jam_NoteCover			= _T( "Jam Note cover open" );
const LPCTSTR	Jam_Hopper				= _T( "Jam Hopper locked" );
const LPCTSTR	Jam_Cassette			= _T( "Jam in cassette" );
const LPCTSTR	Jam_CoinIO				= _T( "Jam coin IO" );
const LPCTSTR	Jam_NoteNVIO			= _T( "Jam Note Validator IO" );
const LPCTSTR	Jam_NoteLib				= _T( "Jam Note Lib Error" );
const LPCTSTR	Jam_NoteGeneral			= _T( "Jam Note General Error" );
const LPCTSTR	Jam_CoinGeneral			= _T( "Jam Coin General Error" );
const LPCTSTR	Jam_Resource			= _T( "Jam Resource Error" );
const LPCTSTR	Jam_OpenState			= _T( "Jam Open State Error" );


// SafePay Jam codes 
enum
{
	SP_JamCoinDeposit = 1,
	SP_JamNoteDeposit,
	SP_JamCoinDoor,
	SP_JamNoteCover,
	SP_JamHopper,
	SP_JamCassette,
	SP_JamCoinUnitIO,
	SP_JamNoteValidatorIO,
	SP_JamNoteIO,
	SP_JamNoteLib,
	SP_JamNoteGeneral,
	SP_JamCoinGeneral,
	SP_JamResource,
	SP_JamOpenState
};


// SafePay LatchCassette codes
enum
{
	SP_UnLatch_Cassette,
	SP_Latch_Cassette
};


// Cover Id for OpenCover api 
const BYTE SP_NoteUnit_Cover = 1;		// SafePay Note Unit Cover Id 

// SafePay CCC_RAW Operation codes

const char	SP_READ_PROPERTY	= 'R';
const char	SP_WRITE_PROPERTY	= 'W';
const char	SP_EXEC_COMMAND		= 'C';


// Function Names in CCC.dll 

const LPCTSTR 	NAME_CCC_ProtocolSelect			= _T( "CCC_ProtocolSelect" );
const LPCTSTR 	NAME_CCC_ProtocolStartV			= _T( "CCC_ProtocolStartV" );
const LPCTSTR 	NAME_CCC_ProtocolStop			= _T( "CCC_ProtocolStop" );
const LPCTSTR 	NAME_CCC_RegisterEventHandler	= _T( "CCC_RegisterEventHandler" );
const LPCTSTR 	NAME_CCC_LogIn					= _T( "CCC_LogIn"	);
const LPCTSTR 	NAME_CCC_LogOut					= _T( "CCC_LogOut" );
const LPCTSTR 	NAME_CCC_Deposit				= _T( "CCC_Deposit" );
const LPCTSTR 	NAME_CCC_GetDeposit				= _T( "CCC_GetDeposit" );
const LPCTSTR 	NAME_CCC_Dispense				= _T( "CCC_Dispense" );
const LPCTSTR 	NAME_CCC_SimulateDispense		= _T( "CCC_SimulateDispense" );
const LPCTSTR 	NAME_CCC_DispenseCommence		= _T( "CCC_DispenseCommence" );
const LPCTSTR 	NAME_CCC_DispenseComplete		= _T( "CCC_DispenseComplete" );
const LPCTSTR 	NAME_CCC_Status					= _T( "CCC_Status" );
const LPCTSTR 	NAME_CCC_FullStatus				= _T( "CCC_FullStatus" );
const LPCTSTR 	NAME_CCC_Check					= _T( "CCC_Check"	);
const LPCTSTR 	NAME_CCC_Message				= _T( "CCC_Message"	);
const LPCTSTR 	NAME_CCC_OpenCover				= _T( "CCC_OpenCover"	);
const LPCTSTR 	NAME_CCC_NoteTransfer			= _T( "CCC_NoteTransfer" );
const LPCTSTR 	NAME_CCC_SetCassetteState		= _T( "CCC_SetCassetteState" );
const LPCTSTR 	NAME_CCC_GetCurrencyInfo		= _T( "CCC_GetCurrencyInfo"  );
const LPCTSTR 	NAME_CCC_GetCurrencyUnitInfo	= _T( "CCC_GetCurrencyUnitInfo" );
const LPCTSTR 	NAME_CCC_GetCashCount			= _T( "CCC_GetCashCount" );	
const LPCTSTR 	NAME_CCC_GetInventoryList		= _T( "CCC_GetInventoryList" );
const LPCTSTR 	NAME_CCC_LoggingStart			= _T( "CCC_LoggingStart" );
const LPCTSTR 	NAME_CCC_LoggingStop			= _T( "CCC_LoggingStop"  );
const LPCTSTR 	NAME_CCC_RAW					= _T( "CCC_RAW"	 );
const LPCTSTR 	NAME_SP_Open					= _T( "SP_Open"	 );
const LPCTSTR 	NAME_SP_Close					= _T( "SP_Close" );
const LPCTSTR 	NAME_SP_BeginDeposit			= _T( "SP_BeginDeposit" );
const LPCTSTR 	NAME_SP_Deposit					= _T( "SP_Deposit"	  );
const LPCTSTR 	NAME_SP_EndDeposit				= _T( "SP_EndDeposit" );
const LPCTSTR	NAME_SP_FixDeposit				= _T( "SP_FixDeposit" );
const LPCTSTR 	NAME_SP_GetDeposit				= _T( "SP_GetDeposit" );
const LPCTSTR 	NAME_SP_Dispense				= _T( "SP_Dispense"	  );


// Note, SafePay is an ASCII Api. All strings are ASCII and don't support UNICODE strings
// Function prototypes in CCC.dll 

extern "C" 
{
	typedef long ( __stdcall * CCC_ProtocolSelect )( LPSTR ConnectStr );
	typedef long ( __stdcall * CCC_ProtocolStartV )( SHORT majorVer, SHORT minorVer );
	typedef	long ( __stdcall * CCC_ProtocolStop )();
	typedef void ( __stdcall * CCC_EventHandler )( WORD eventId, LPVOID data, LONG param );
	typedef long ( __stdcall * CCC_RegisterEventHandler )( CCC_EventHandler handler, BYTE eventMask, LONG param );
	typedef long ( __stdcall * CCC_LogIn )( LPCSTR szOpId, LPCSTR szDateTime );
	typedef long ( __stdcall * CCC_LogOut )();
	typedef long ( __stdcall * CCC_Deposit )( LPLONG amount );
	typedef long ( __stdcall * CCC_GetDeposit )( LPLONG lpAmount, BYTE TransType, LPSTR CashCount, LPSTR Currency );
	typedef long ( __stdcall * CCC_Dispense )( LPLONG lpAmount, BYTE TransType, LPSTR CashCount, LPSTR Currency, BYTE ExchangeMode );
	typedef long ( __stdcall * CCC_SimulateDispense )( LPLONG lpAmount, BYTE TransType, LPSTR CashCount, LPSTR Currency, BYTE ExchangeMode );
	typedef long ( __stdcall * CCC_DispenseCommence )( LPLONG lpAmount, BYTE TransType, LPSTR CashCount, LPSTR Currency, BYTE ExchangeMode );
	typedef long ( __stdcall * CCC_DispenseComplete )( LPLONG lpAmount, LPSTR CashCount, LPSTR Currency );
	typedef long ( __stdcall * CCC_Status )( SPStatus * Status );
	typedef long ( __stdcall * CCC_FullStatus )( SPFullStatus * FullStatus );
	typedef long ( __stdcall * CCC_Check )();
	typedef long ( __stdcall * CCC_Message )( LPSTR Message );
	typedef long ( __stdcall * CCC_OpenCover )( BYTE State );
	typedef long ( __stdcall * CCC_NoteTransfer )( BYTE Cassette, SHORT DumpMode );
	typedef long ( __stdcall * CCC_SetCassetteState )( BYTE Cassette, BYTE State );
	typedef long ( __stdcall * CCC_GetCurrencyInfo )( LPSTR Dispence, LPSTR Deposit );
	typedef long ( __stdcall * CCC_GetCurrencyUnitInfo )( LPCSTR Currency, LPSTR Dispence, LPSTR Deposit );
	typedef long ( __stdcall * CCC_GetCashCount )( LPSTR Currency, LPSTR CashCount );
	typedef long ( __stdcall * CCC_GetInventoryList )( LPSTR Currency, LPSTR CashCount );
	typedef void ( __stdcall * CCC_LoggingStart )( const BYTE Level, LPCSTR LogFilePath );
	typedef void ( __stdcall * CCC_LoggingStop )();
	typedef long ( __stdcall * CCC_RAW )( char op, WORD Idx, LPVOID data, WORD length );
	typedef long ( __stdcall * SP_Open )( LPCSTR OpId, LPCSTR DateTime );
	typedef long ( __stdcall * SP_Close )();
	typedef long ( __stdcall * SP_BeginDeposit )( LPCSTR Currency );
	typedef long ( __stdcall * SP_Deposit )( LPLONG Amount, LPSTR CashCount, LPSTR Currency );
	typedef long ( __stdcall * SP_GetDeposit )( LPLONG Amount, BYTE TransType, LPSTR CashCount, LPSTR Currency );
	typedef long ( __stdcall * SP_FixDeposit )( LPLONG Amount, BYTE TransType, LPSTR CashCount, LPSTR Currency );
	typedef long ( __stdcall * SP_EndDeposit )();
	typedef long ( __stdcall * SP_Dispense )( LPLONG Amount, BYTE TransType, LPSTR CashCount, LPSTR Currency );
};




class CSafePayApi  
{
public:
	CSafePayApi( CmDataCapture& cmDC );
	virtual ~CSafePayApi();

	//	 Startup / Shutdown methods
	void ProtocolStart( short Major, short Minor );
	void ProtocolSelect( LPCSTR Port, LPCSTR ConnectType );
	void ProtocolStop();
	void LogIn( LPCSTR szOperatorId, LPCSTR szDateTime );
	void LoadDLL( LPCSTR szDllPath );
	void LogOut();

	// Deposit / Dispense methods 
	void BeginDeposit( LPCSTR Currency );
	void FixDeposit( LPLONG lpAmount, BYTE TransType, LPSTR CashCount, LPSTR Currency );
	void EndDeposit();
	void PauseDeposit();
	void DispenseCash( LPLONG lpAmount, BYTE TransType, LPSTR CashCount, LPSTR Currency, BYTE ExchangeMode );
	void SimulateDispense( LPLONG lpAmount, BYTE TransType, LPSTR CashCount, LPSTR Currency, BYTE ExchangeMode );

	// Properties methods 
	void GetPropertyString( WORD Idx, LPSTR PropertyStr );
	void GetPropertyNumber( WORD Idx, long& iProperty );
	void SetPropertyString( WORD Idx, LPCSTR Value );
	void SetPropertyNumber( WORD Idx, long iProp );

	// Status methods 
	bool Loaded()	{ return m_bLoaded;   };
	bool Selected()	{ return m_bSelected; };
	bool Started()	{ return m_bStarted;  };
	bool Logging()	{ return m_bLogging;  };
	bool LoggedIn()	{ return m_bLoggedIn; };
	void Ready( LPCTSTR lpFName );
	void CheckHealth();
	void GetMessage( LPSTR Message );
	void GetCurrencyUnitInfo( LPCSTR Currency, LPSTR DispenseString, LPSTR DepositString );
	void GetCurrencyInfo( LPSTR DispenseString, LPSTR DepositString );
	void GetCashCount( LPSTR Currency, LPSTR CashCount );
	void GetInventoryList( LPSTR Currency, LPSTR CashCount );
	void DepositAmount( LPLONG lpAmount, LPSTR CashCount, LPSTR Currency );
	void Status( SPStatus& SpStatus );
	void FullStatus( SPFullStatus& SpFullStatus );
	LPCTSTR GetExtendedResultString();
	long GetLastResult();
	
	long GetSafePayFullType( SPFullStatus& FullStatus );

	bool IsSafePayFull( const SPStatus& status );
	bool IsSafePayJammed( const SPStatus& SpStatus );
	long SafePayJamDecode( const SPStatus& SpStatus);
	LPCTSTR GetSafePayJamCodeText( long lCode );

	bool IsSafePayCoinJam( const SPStatus& SpStatus );
	bool IsSafePayNoteJam( const SPStatus& SpStatus );
	bool IsSafePayGenErrCoin( const SPStatus& SpStatus );
	bool IsSafePayGenErrNote( const SPStatus& SpStatus );
	bool IsSafePayNoteFull( const SPFullStatus& SpFullStatus );
	bool IsSafePayCoinFull( const SPFullStatus& SpFullStatus );

	// Utility Methods 
	void NoteTransfer( BYTE Cassette, SHORT DumpMode );
	void LatchCassette( BYTE Cassette, BYTE Status );

	// Diagnostic Methods 
	void LoggingStop();
	void LoggingStart( BYTE level, LPCSTR szLogPath );
	void OpenCover( BYTE CoverId );

	// Events Methods
	void RegisterEventHandler( CCC_EventHandler handler, BYTE eventMask, LONG param );


protected:
	void	LogExtendedResult( long lResult, LPCTSTR fnName );
	void	LogFailedResult( bool bFailed, LPCTSTR fnName );
	void	DecodePropertyResult( WORD Idx, bool bRead, long ReturnCode, LPCTSTR lpFName );
	FARPROC BindDllFunction( LPCTSTR fnName );
	void	UnLoad();
	
	bool						m_bSelected;
	bool						m_bLoggedIn;
	bool						m_bStarted;
	bool						m_bLoaded;
	bool						m_bLogging;
	mutable	CCriticalSection	m_csLock;
	CmDataCapture&				m_cmDC;
	HMODULE						m_hCCCLib;
	CString						m_csLastErrorMsg;
	long						m_lExtendedResult;
	static	LPCTSTR				m_lpJamReasons[ ];

	// SafePay function pointers for late binding to CCC.dll.  
	CCC_ProtocolSelect			fnCCC_ProtocolSelect;
	CCC_ProtocolStartV			fnCCC_ProtocolStartV;
	CCC_ProtocolStop			fnCCC_ProtocolStop;
	CCC_RegisterEventHandler	fnCCC_RegisterEventHandler;
	CCC_LogIn					fnCCC_LogIn;
	CCC_LogOut					fnCCC_LogOut;
	CCC_Deposit					fnCCC_Deposit;
	CCC_GetDeposit				fnCCC_GetDeposit;
	CCC_Dispense				fnCCC_Dispense;
	CCC_SimulateDispense		fnCCC_SimulateDispense;
	CCC_DispenseCommence		fnCCC_DispenseCommence; 
	CCC_DispenseComplete		fnCCC_DispenseComplete;
	CCC_Status					fnCCC_Status;
	CCC_FullStatus				fnCCC_FullStatus;
	CCC_Check					fnCCC_Check;
	CCC_Message					fnCCC_Message;
	CCC_OpenCover				fnCCC_OpenCover;
	CCC_NoteTransfer			fnCCC_NoteTransfer;
	CCC_SetCassetteState		fnCCC_SetCassetteState;
	CCC_GetCurrencyInfo			fnCCC_GetCurrencyInfo;
	CCC_GetCurrencyUnitInfo		fnCCC_GetCurrencyUnitInfo;
	CCC_GetCashCount			fnCCC_GetCashCount;
	CCC_GetInventoryList		fnCCC_GetInventoryList;
	CCC_LoggingStart			fnCCC_LoggingStart;
	CCC_LoggingStop				fnCCC_LoggingStop;
	CCC_RAW						fnCCC_Raw;

	SP_Open						fnSP_Open;
	SP_Close					fnSP_Close;
	SP_BeginDeposit				fnSP_BeginDeposit;
	SP_Deposit					fnSP_Deposit;
	SP_Dispense					fnSP_Dispense;
	SP_EndDeposit				fnSP_EndDeposit;
	SP_FixDeposit				fnSP_FixDeposit;
	SP_GetDeposit				fnSP_GetDeposit;

};

#endif // !defined(AFX_SAFEPAYAPI_H__B858A64F_6498_4538_BB29_625751863D55__INCLUDED_)
