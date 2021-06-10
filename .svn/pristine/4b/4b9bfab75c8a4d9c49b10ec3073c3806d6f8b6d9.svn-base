////////////////////////////////////////////////////////////////////////////
//
// RecyclerDioParser.h : Header File
//
// Note: This class is reused from Japan SSF code.
// All credit to Japan PS Team for this implementation.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef _RecyclerDioParser_H
#define _RecyclerDioParser_H

/////////////////////////////////////////////////////////////////////////////
// Global Definition
/////////////////////////////////////////////////////////////////////////////

//
// Error Code Definition
//
const long RCYPSR_SUCCESS          =   0;
const long RCYPSR_E_INVALIDPARAM   = 100;
const long RCYPSR_E_UNKNOWNCOMMAND = 101;
const long RCYPSR_E_UNKNOWNDATA    = 102;
const long RCYPSR_E_BUFFERSIZE     = 103;
const long RCYPSR_E_STRINGLENGTH   = 104;
const long RCYPSR_E_INTERNAL       = 105;
const long RCYPSR_E_PARSINGERR     = 106;


//
// Command id definition
//
#define RCYPSR_CMD_STATUSREAD       1       // for CHAN_DI_STATUSREAD
#define RCYPSR_CMD_SEISA            2       // for CHAN_DI_SEISA

//
// Data value definition for DirectIO method
//
// ReadNo with CHAN_DI_STATUSREAD commnand 
#define RCYPSR_DATA_STATREAD01      0x01    // ReadNo 0x01
#define RCYPSR_DATA_STATREAD80      0x80    // ReadNo 0x80

//
// Coin empty status definition (for ReadNo01 with CHAN_DI_STATUSREAD)
//
#define RCYPSR_COIN_EMPTY             0
#define RCYPSR_COIN_NEAREMPTY         1
#define RCYPSR_COIN_SUITABLE          2
#define RCYPSR_COIN_FULL              3
#define RCYPSR_COIN_UNKNOWNSTAT     255

//
// Bill empty status definition (for ReadNo80 with CHAN_DI_STATUSREAD)
//
#define RCYPSR_BILL_EMPTY             0
#define RCYPSR_BILL_NEAREMPTY         1
#define RCYPSR_BILL_SUITABLE          2
#define RCYPSR_BILL_NEARFULL          3
#define RCYPSR_BILL_FULL              4
#define RCYPSR_BILL_NOSTORAGE         5
#define RCYPSR_BILL_UNKNOWNSTAT     255

//
// Result of Parsing for CHAN_DI_STATUSREAD definition (ReadNo is RCYPSR_DATA_STATREAD01)
//
typedef struct _RCYPSRSTATUS01
{
    long    lErrorCode;             // error code.
    CString strNotUsed1;            // Not used.
    WORD    wStatusOf500Yen;        // status of the storage bin for 500-Yen.
    WORD    wStatusOf100Yen;        // status of the storage bin for 100-Yen.
    WORD    wStatusOf50Yen;         // status of the storage bin for 50-Yen.
    WORD    wStatusOf10Yen;         // status of the storage bin for 10-Yen.
    WORD    wStatusOf5Yen;          // status of the storage bin for 5-Yen.
    WORD    wStatusOf1Yen;          // status of the storage bin for 1-Yen.
    WORD    wStatusOfRJ;            // status of the reject box.
    CString strNotUsed2;            // Not used.
    bool    fAbnormalBalance;       // balance status.
    CString strNotUsed3;            // Not used.
    int     nCountOf500YenInRJ;     // count of 500-Yen in the reject box.(it would be zero because RT70 does not have a reject box.)
    int     nCountOf100YenInRJ;     // count of 100-Yen in the reject box.(it would be zero because RT70 does not have a reject box.)
    int     nCountOf50YenInRJ;      // count of 50-Yen in the reject box. (it would be zero because RT70 does not have a reject box.)
    int     nCountOf10YenInRJ;      // count of 10-Yen in the reject box. (it would be zero because RT70 does not have a reject box.)
    int     nCountOf5YenInRJ;       // count of 5-Yen in the reject box.  (it would be zero because RT70 does not have a reject box.)
    int     nCountOf1YenInRJ;       // count of 1-Yen in the reject box.  (it would be zero because RT70 does not have a reject box.)
    int     nCountOfRejectedInRJ;   // count of rejected coins in the reject box.  (it would be zero because RT70 does not have a reject box.)
    CString strNotUsed4;            // Not used.
    int     nCountOf500Yen;         // count of 500-Yen to be collected or dispensed just before.
    int     nCountOf100Yen;         // count of 100-Yen to be collected or dispensed just before.
    int     nCountOf50Yen;          // count of 50-Yen to be collected or dispensed just before.
    int     nCountOf10Yen;          // count of 10-Yen to be collected or dispensed just before.
    int     nCountOf5Yen;           // count of 5-Yen to be collected or dispensed just before.
    int     nCountOf1Yen;           // count of 1-Yen to be collected or dispensed just before.
    int     nCountOfRejected;       // count of coins to be rejected just before.
    CString strNotUsed5;            // Not used.
} RCYPSRSTATUS01, *PRCYPSRSTATUS01;

//
// Result of Parsing for CHAN_DI_STATUSREAD definition (ReadNo is RCYPSR_DATA_STATREAD80)
//
typedef struct _RCYPSRSTATUS80
{
    long    lErrorCode;             // error code.
    CString strNotUsed1;            // Not used.
    WORD    wStatusOf10000Yen;      // status of the storage for 10000-Yen.
    WORD    wStatusOf5000Yen;       // status of the storage for 5000-Yen.
    WORD    wStatusOf2000Yen;       // status of the storage for 2000-Yen.
    WORD    wStatusOf1000Yen;       // status of the storage for 1000-Yen.
    WORD    wStatusOfCassette;      // status of the cassette.
    WORD    wStatusOfRJ;            // status of the reject unit.
    CString strNotUsed2;            // Not used.
    bool    fAbnormalBalance;       // balance status.
    CString strOperationFlag;       // note and coin operation flag.
    int     nCountOf10000Yen;       // count of 10000-Yen to be collected or dispensed just before.
    int     nCountOf5000Yen;        // count of 5000-Yen to be collected or dispensed just before.
    int     nCountOf2000Yen;        // count of 2000-Yen to be collected or dispensed just before.
    int     nCountOf1000Yen;        // count of 1000-Yen to be collected or dispensed just before.
    CString strNotUsed3;            // Not used.
} RCYPSRSTATUS80, *PRCYPSRSTATUS80;

//
// Result of Parsing for CHAN_DI_SEISA definition
//
typedef struct _RCYPSRSEISA
{
    int     nCountOf2000Yen;                // count of 2000-Yen in the storage.
    int     nCountOf10000Yen;               // count of 10000-Yen in the storage.
    int     nCountOf5000Yen;                // count of 5000-Yen in the storage.
    int     nCountOf1000Yen;                // count of 1000-Yen in the storage.
    int     nCountOf2000YenInCst;           // count of 2000-Yen in the cassette.
    int     nCountOf10000YenInCst;          // count of 10000-Yen in the cassette.
    int     nCountOf5000YenInCst;           // count of 5000-Yen in the cassette.
    int     nCountOf1000YenInCst;           // count of 1000-Yen in the cassette.
    int     nCountOf500Yen;                 // count of 500-Yen in the storage.
    int     nCountOf100Yen;                 // count of 100-Yen in the storage.
    int     nCountOf50Yen;                  // count of 50-Yen in the storage.
    int     nCountOf10Yen;                  // count of 10-Yen in the storage.
    int     nCountOf5Yen;                   // count of 5-Yen in the storage.
    int     nCountOf1Yen;                   // count of 1-Yen in the storage.
    int     nCountOf2000YenInLastCst;       // count of 2000-Yen in last sampling cassette.
    int     nCountOf10000YenInLastCst;      // count of 10000-Yen in last sampling cassette.
    int     nCountOf5000YenInLastCst;       // count of 5000-Yen in last sampling cassette.
    int     nCountOf1000YenInLastCst;       // count of 1000-Yen in last sampling cassette.
    int     nCountOf2000YenInCmltCst;       // count of 2000-Yen in cumulative sampling cassette.
    int     nCountOf10000YenInCmltCst;      // count of 10000-Yen in cumulative sampling cassette.
    int     nCountOf5000YenInCmltCst;       // count of 5000-Yen in cumulative sampling cassette.
    int     nCountOf1000YenInCmltCst;       // count of 1000-Yen in cumulative sampling cassette.
    WORD    wNoteBalanceStatus;             // note balance status flag.
    WORD    wNoteFullStatus;                // note full status flag.
    WORD    wCoinBalanceStatus;             // coin balance status flag.
    WORD    wCoinFullStatus;                // coin full status flag.
    int     nNumberOfNoteRJ;                // number of note rejected.
    int     nNumberOfCoinRJ;                // number of coin rejected.
    int     nOverflowOf500Yen;              // overflow 500-Yen.
    int     nOverflowOf100Yen;              // overflow 100-Yen.
    int     nOverflowOf50Yen;               // overflow 50-Yen.
    int     nOverflowOf10Yen;               // overflow 10-Yen.
    int     nOverflowOf5Yen;                // overflow 5-Yen.
    int     nOverflowOf1Yen;                // overflow 1-Yen.
} RCYPSRSEISA, *PRCYPSRSEISA;

//
// Note Balance Status Check Macro (wNoteBalanceStatus)
//
#define F_NOTE_BALANCE_STORAGE          0x0001
#define F_NOTE_BALANCE_CST              0x0002
#define F_NOTE_BALANCE_LASTCST          0x0004
#define F_NOTE_BALANCE_CMLVCST          0x0008
#define IS_NOTE_STORAGEABNORMAL(x)      (WORD)((WORD)(x) & F_NOTE_BALANCE_STORAGE)
#define IS_NOTE_CSTABNORMAL(x)          (WORD)((WORD)(x) & F_NOTE_BALANCE_CST    )
#define IS_NOTE_LASTCST_ABNORMAL(x)     (WORD)((WORD)(x) & F_NOTE_BALANCE_LASTCST)
#define IS_NOTE_CMLVCST_ABNORMAL(x)     (WORD)((WORD)(x) & F_NOTE_BALANCE_CMLVCST)
//
// Note Full Status Check Macro (wNoteFullStatus)
//
#define F_NOTE_FULL_RJ                  0x0001
#define F_NOTE_FULL_CASSETTE            0x0002
#define IS_NOTE_RJFULL(x)               (WORD)((WORD)(x) & F_NOTE_FULL_RJ      )
#define IS_NOTE_CASSETTEFULL(x)         (WORD)((WORD)(x) & F_NOTE_FULL_CASSETTE)
//
// Coin Balance Status Check Macro (wCoinBalanceStatus)
//
#define F_COIN_BALANCE_STORAGE          0x0001
#define IS_COIN_STORAGEABNORMAL(x)      (WORD)((WORD)(x) & F_COIN_BALANCE_STORAGE)
//
// Coin Full Status Check Macro (wCoinFullStatus)
//
#define F_COIN_FULL_RJ                  0x0001
#define F_COIN_FULL_STORAGE             0x0002
#define F_COIN_FULL_CHARGINGPART        0x0008
#define IS_COIN_RJFULL(x)               (WORD)((WORD)(x) & F_COIN_FULL_RJ          )
#define IS_COIN_STORAGEFULL(x)          (WORD)((WORD)(x) & F_COIN_FULL_STORAGE     )
#define IS_COIN_CHARGINGPARTFULL(x)     (WORD)((WORD)(x) & F_COIN_FULL_CHARGINGPART)


/////////////////////////////////////////////////////////////////////////////
// CRecyclerDioParser Class Definition
/////////////////////////////////////////////////////////////////////////////

class CRecyclerDioParser
{
public:

    //-------------------------------------------------
    // Constructor / Destructor
    //-------------------------------------------------

    CRecyclerDioParser();
    virtual ~CRecyclerDioParser();

    //-------------------------------------------------
    // Operations
    //-------------------------------------------------

    static long Parse(LONG Command, LONG* pData, LPCTSTR lpszString, PVOID pvResult, UINT nBufferSize);
};



/////////////////////////////////////////////////////////////////////////////
#endif // #ifndef _RecyclerDioParser_H
/////////////////////////////////////////////////////////////////////////////