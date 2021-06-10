/////////////////////////////////////////////////////////////////////////////
//
// RecyclerDioParser.cpp : Implementation File
// All credit to Japan PS Team for this implementation.
//


/////////////////////////////////////////////////////////////////////////////
// Include header files
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "RecyclerDioParser.h"

/////////////////////////////////////////////////////////////////////////////
// Logging Control
/////////////////////////////////////////////////////////////////////////////

#if defined(JAPAN)
#define _TRACE_LOG          1
#define _TRACE_DETAILLOG    0
#else
#define _TRACE_LOG          0
#define _TRACE_DETAILLOG    0
#endif

/////////////////////////////////////////////////////////////////////////////
// Parsing Macro
/////////////////////////////////////////////////////////////////////////////

#define MASK_BIT12  0x0000000C
#define MASK_BIT34  0x00000003  
#define GET_BIT12(x)    ((DWORD)(((DWORD)x & MASK_BIT12) >> 2))
#define GET_BIT34(x)    ((DWORD)( (DWORD)x & MASK_BIT34))

#define GET_COINSTAT(x) ((x == 0) ? RCYPSR_COIN_EMPTY       :   \
                         (x == 1) ? RCYPSR_COIN_NEAREMPTY   :   \
                         (x == 2) ? RCYPSR_COIN_SUITABLE    :   \
                         (x == 3) ? RCYPSR_COIN_FULL        :   \
                         RCYPSR_COIN_UNKNOWNSTAT)

#define GET_BILLSTAT(x) ((x == 0) ? RCYPSR_BILL_EMPTY       :   \
                         (x == 1) ? RCYPSR_BILL_NEAREMPTY   :   \
                         (x == 2) ? RCYPSR_BILL_SUITABLE    :   \
                         (x == 3) ? RCYPSR_BILL_NEARFULL    :   \
                         (x == 4) ? RCYPSR_BILL_FULL        :   \
                         (x == 5) ? RCYPSR_BILL_NOSTORAGE   :   \
                         RCYPSR_BILL_UNKNOWNSTAT)


/////////////////////////////////////////////////////////////////////////////
// Local Definition
/////////////////////////////////////////////////////////////////////////////

//
// Parser function prototype definition
//
typedef LONG (_cdecl *PFNDIOPARSER)(LONG, LONG*, LPCTSTR, PVOID, UINT);

//
// Data parser table definition
//
typedef struct _DATAPARSERTBL       // Command with Data parser table
{
    int                 nIndex;             // record index. value of less than zero means end of table.
    LONG                lData;              // command id
    UINT                nBuffSize;          // result buffer size to be required
    UINT                nStrLength;         // length of string to be parsed
    PFNDIOPARSER        pfnParser;          // parser functon
} DATAPARSERTBL, *PDATAPARSERTBL;

//
// Command parser table definition
//
typedef struct _CMDPASERTBL         // Command without Data parser table
{
    int                 nIndex;             // record index. value of less than zero means end of table.
    LONG                lCommand;           // command id
    PDATAPARSERTBL      pDataParserTbl;     // sub parser table
    UINT                nBuffSize;          // result buffer size to be required
    UINT                nStrLength;         // length of string to be parsed
    PFNDIOPARSER        pfnParser;          // parser functon
} CMDPASERTBL, *PCMDPASERTBL;

//
// Field Length table definition
//
typedef struct _FIELDLENGTHTBL  // Field length table
{
    int nIndex;                     // record index. value of less than zero means end of table.
    int nField;                     // field number.
    int nLength;                    // length of this field.
} FIELDLENGTHTBL, *PFIELDLENGTHTBL;

//
// Paser Tables
//
CMDPASERTBL   CmdParserTable[];
DATAPARSERTBL StatusReadParserTbl[];


/////////////////////////////////////////////////////////////////////////////
// String Data Format Definition
/////////////////////////////////////////////////////////////////////////////
//
// Field Data Length of STATUSREAD(ReadNo01)
//
#define STATREAD01_LEN_ERRORCODE         4
#define STATREAD01_LEN_NOTUSED1          2
#define STATREAD01_LEN_STORAGE1          1
#define STATREAD01_LEN_STORAGE2          1
#define STATREAD01_LEN_STORAGE3          1
#define STATREAD01_LEN_STORAGE4          1
#define STATREAD01_LEN_NOTUSED2          2
#define STATREAD01_LEN_BALANCEFLAG       2
#define STATREAD01_LEN_NOTUSED3          4
#define STATREAD01_LEN_COUNT500YINRJ     2
#define STATREAD01_LEN_COUNT100YINRJ     2
#define STATREAD01_LEN_COUNT50YINRJ      2
#define STATREAD01_LEN_COUNT10YINRJ      2
#define STATREAD01_LEN_COUNT5YINRJ       2
#define STATREAD01_LEN_COUNT1YINRJ       2
#define STATREAD01_LEN_COUNTRJINRJ       2
#define STATREAD01_LEN_NOTUSED4          2
#define STATREAD01_LEN_COUNT500Y         2
#define STATREAD01_LEN_COUNT100Y         2
#define STATREAD01_LEN_COUNT50Y          2
#define STATREAD01_LEN_COUNT10Y          2
#define STATREAD01_LEN_COUNT5Y           2
#define STATREAD01_LEN_COUNT1Y           2
#define STATREAD01_LEN_COUNTRJ           2
#define STATREAD01_LEN_NOTUSED5          16
#define STATREAD01_LEN_TOTAL            ( STATREAD01_LEN_ERRORCODE      + \
                                          STATREAD01_LEN_NOTUSED1       + \
                                          STATREAD01_LEN_STORAGE1       + \
                                          STATREAD01_LEN_STORAGE2       + \
                                          STATREAD01_LEN_STORAGE3       + \
                                          STATREAD01_LEN_STORAGE4       + \
                                          STATREAD01_LEN_NOTUSED2       + \
                                          STATREAD01_LEN_BALANCEFLAG    + \
                                          STATREAD01_LEN_NOTUSED3       + \
                                          STATREAD01_LEN_COUNT500YINRJ  + \
                                          STATREAD01_LEN_COUNT100YINRJ  + \
                                          STATREAD01_LEN_COUNT50YINRJ   + \
                                          STATREAD01_LEN_COUNT10YINRJ   + \
                                          STATREAD01_LEN_COUNT5YINRJ    + \
                                          STATREAD01_LEN_COUNT1YINRJ    + \
                                          STATREAD01_LEN_COUNTRJINRJ    + \
                                          STATREAD01_LEN_NOTUSED4       + \
                                          STATREAD01_LEN_COUNT500Y      + \
                                          STATREAD01_LEN_COUNT100Y      + \
                                          STATREAD01_LEN_COUNT50Y       + \
                                          STATREAD01_LEN_COUNT10Y       + \
                                          STATREAD01_LEN_COUNT5Y        + \
                                          STATREAD01_LEN_COUNT1Y        + \
                                          STATREAD01_LEN_COUNTRJ        + \
                                          STATREAD01_LEN_NOTUSED5 )
//
// Field Data Length of STATUSREAD(ReadNo80)
//
#define STATREAD80_LEN_ERRORCODE         4
#define STATREAD80_LEN_NOTUSED1          4
#define STATREAD80_LEN_STORAGE1          1
#define STATREAD80_LEN_STORAGE2          1
#define STATREAD80_LEN_STORAGE3          1
#define STATREAD80_LEN_STORAGE4          1
#define STATREAD80_LEN_STORAGE5          1
#define STATREAD80_LEN_STORAGE6          1
#define STATREAD80_LEN_NOTUSED2          6
#define STATREAD80_LEN_BALANCEFLAG       2
#define STATREAD80_LEN_OPERATIONFLAG     2
#define STATREAD80_LEN_COUNT10000Y       4
#define STATREAD80_LEN_COUNT5000Y        4
#define STATREAD80_LEN_COUNT2000Y        4
#define STATREAD80_LEN_COUNT1000Y        4
#define STATREAD80_LEN_NOTUSED3          24
#define STATREAD80_LEN_TOTAL            ( STATREAD80_LEN_ERRORCODE      + \
                                          STATREAD80_LEN_NOTUSED1       + \
                                          STATREAD80_LEN_STORAGE1       + \
                                          STATREAD80_LEN_STORAGE2       + \
                                          STATREAD80_LEN_STORAGE3       + \
                                          STATREAD80_LEN_STORAGE4       + \
                                          STATREAD80_LEN_STORAGE5       + \
                                          STATREAD80_LEN_STORAGE6       + \
                                          STATREAD80_LEN_NOTUSED2       + \
                                          STATREAD80_LEN_BALANCEFLAG    + \
                                          STATREAD80_LEN_OPERATIONFLAG  + \
                                          STATREAD80_LEN_COUNT10000Y    + \
                                          STATREAD80_LEN_COUNT5000Y     + \
                                          STATREAD80_LEN_COUNT2000Y     + \
                                          STATREAD80_LEN_COUNT1000Y     + \
                                          STATREAD80_LEN_NOTUSED3 )
//
// Field Data Length of SEISA
//
#define SEISA_LEN_CNTOF2000Y                3
#define SEISA_LEN_CNTOF10000Y               3
#define SEISA_LEN_CNTOF5000Y                3
#define SEISA_LEN_CNTOF1000Y                3
#define SEISA_LEN_CNTOF2000YINCST           3
#define SEISA_LEN_CNTOF10000YINCST          3
#define SEISA_LEN_CNTOF5000YINCST           3
#define SEISA_LEN_CNTOF1000YINCST           3
#define SEISA_LEN_CNTOF500Y                 3
#define SEISA_LEN_CNTOF100Y                 3
#define SEISA_LEN_CNTOF50Y                  3
#define SEISA_LEN_CNTOF10Y                  3
#define SEISA_LEN_CNTOF5Y                   3
#define SEISA_LEN_CNTOF1Y                   3
#define SEISA_LEN_CNTOF2000YINLASTCST       3
#define SEISA_LEN_CNTOF10000YINLASTCST      3
#define SEISA_LEN_CNTOF5000YINLASTCST       3
#define SEISA_LEN_CNTOF1000YINLASTCST       3
#define SEISA_LEN_CNTOF2000YINCMLTCST       4
#define SEISA_LEN_CNTOF10000YINCMLTCST      4
#define SEISA_LEN_CNTOF5000YINCMLTCST       4
#define SEISA_LEN_CNTOF1000YINCMLTCST       4
#define SEISA_LEN_NOTEBALANCESTAT           1
#define SEISA_LEN_NOTEFULLSTAT              1
#define SEISA_LEN_COINBALANCESTAT           1
#define SEISA_LEN_COINFULLSTAT              1
#define SEISA_LEN_NUMOFNOTERJ               3
#define SEISA_LEN_NUMOFCOINRJ               3
#define SEISA_LEN_OVERFLOW500Y              3
#define SEISA_LEN_OVERFLOW100Y              3
#define SEISA_LEN_OVERFLOW50Y               3
#define SEISA_LEN_OVERFLOW10Y               3
#define SEISA_LEN_OVERFLOW5Y                3
#define SEISA_LEN_OVERFLOW1Y                3
#define SEISA_LEN_TOTAL                 ( SEISA_LEN_CNTOF2000Y              + \
                                          SEISA_LEN_CNTOF10000Y             + \
                                          SEISA_LEN_CNTOF5000Y              + \
                                          SEISA_LEN_CNTOF1000Y              + \
                                          SEISA_LEN_CNTOF2000YINCST         + \
                                          SEISA_LEN_CNTOF10000YINCST        + \
                                          SEISA_LEN_CNTOF5000YINCST         + \
                                          SEISA_LEN_CNTOF1000YINCST         + \
                                          SEISA_LEN_CNTOF500Y               + \
                                          SEISA_LEN_CNTOF100Y               + \
                                          SEISA_LEN_CNTOF50Y                + \
                                          SEISA_LEN_CNTOF10Y                + \
                                          SEISA_LEN_CNTOF5Y                 + \
                                          SEISA_LEN_CNTOF1Y                 + \
                                          SEISA_LEN_CNTOF2000YINLASTCST     + \
                                          SEISA_LEN_CNTOF10000YINLASTCST    + \
                                          SEISA_LEN_CNTOF5000YINLASTCST     + \
                                          SEISA_LEN_CNTOF1000YINLASTCST     + \
                                          SEISA_LEN_CNTOF2000YINCMLTCST     + \
                                          SEISA_LEN_CNTOF10000YINCMLTCST    + \
                                          SEISA_LEN_CNTOF5000YINCMLTCST     + \
                                          SEISA_LEN_CNTOF1000YINCMLTCST     + \
                                          SEISA_LEN_NOTEBALANCESTAT         + \
                                          SEISA_LEN_NOTEFULLSTAT            + \
                                          SEISA_LEN_COINBALANCESTAT         + \
                                          SEISA_LEN_COINFULLSTAT            + \
                                          SEISA_LEN_NUMOFNOTERJ             + \
                                          SEISA_LEN_NUMOFCOINRJ             + \
                                          SEISA_LEN_OVERFLOW500Y            + \
                                          SEISA_LEN_OVERFLOW100Y            + \
                                          SEISA_LEN_OVERFLOW50Y             + \
                                          SEISA_LEN_OVERFLOW10Y             + \
                                          SEISA_LEN_OVERFLOW5Y              + \
                                          SEISA_LEN_OVERFLOW1Y )


/////////////////////////////////////////////////////////////////////////////
// Local Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////

long ParseStatusRead01(LONG Command, LONG* pData, LPCTSTR lpszString, PVOID pvResult, UINT nBufferSize);
long ParseStatusRead80(LONG Command, LONG* pData, LPCTSTR lpszString, PVOID pvResult, UINT nBufferSize);
long ParseSeisa       (LONG Command, LONG* pData, LPCTSTR lpszString, PVOID pvResult, UINT nBufferSize);


/////////////////////////////////////////////////////////////////////////////
// CRecyclerDioParser Class Initializations
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// Function:    Constructor
//
// Prototype:   CRecyclerDioParser::CRecyclerDioParser();
//
// Inputs:      nothing
//
// Outputs:     nothing
//
/////////////////////////////////////////////////////////////////////////////

CRecyclerDioParser::CRecyclerDioParser()
{
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:    Destructor
//
// Prototype:   CRecyclerDioParser::~CRecyclerDioParser();
//
// Inputs:      nothing
//
// Outputs:     nothing
//
/////////////////////////////////////////////////////////////////////////////

CRecyclerDioParser::~CRecyclerDioParser()
{
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:    Root parsing function.
//
// Prototype:   long Parse(LONG Command, LONG* pData, LPCTSTR lpszString, 
//                         PVOID pvResult, UINT nBufferSize)
//
// Inputs:      LONG    Command      : [in]         command id
//              LONG*   pData        : [in]         ptr. to command data
//              LPCTSTR lpszString   : [in]         string returned by DirectIO function.
//              PVOID   pvResult     : [in / out]   ptr. to result of parsing
//              UINT    nBufferSize  : [in]         number of bytes of the result buffer
//
// Outputs:     long lResult
//
/////////////////////////////////////////////////////////////////////////////

long CRecyclerDioParser::Parse(LONG Command, LONG* pData, LPCTSTR lpszString, 
                               PVOID pvResult, UINT nBufferSize)
{
    // debugging...
#if (_TRACE_LOG)
    trace(L6, _T("+Parse(%d, 0x%08X, 0x%08X, 0x%08X, %d)"), Command, pData, lpszString, pvResult, nBufferSize);
#endif
    // initialize

    long lResult = RCYPSR_E_INVALIDPARAM;

    // check the arguments

    if ((! pvResult) || (nBufferSize == 0) || (! lpszString))
    {
        // debugging...
#if (_TRACE_LOG)
        trace(L6, _T("-Parse(): lResult=%d"), lResult);
#endif
        // exit...

        return (lResult);
    }

    // parse the command

    for (PCMDPASERTBL pCmdTable = CmdParserTable; pCmdTable->nIndex >= 0; pCmdTable++)
    {
        // re-initialize result status

        lResult = RCYPSR_E_UNKNOWNCOMMAND;

        // is this target command ?

        if (pCmdTable->lCommand != Command)
        {
            continue;
        }

        // re-initialize result status

        lResult = RCYPSR_E_INTERNAL;

        // initialize variables to be checked

        PFNDIOPARSER pfnParser      = NULL;
        UINT         nNeedBuffSize  = 0;
        UINT         nNeedStrLength = 0;

        // is the only one parser function defined for this command ?

        if (pCmdTable->pfnParser)
        {
            pfnParser      = pCmdTable->pfnParser;
            nNeedBuffSize  = pCmdTable->nBuffSize;
            nNeedStrLength = pCmdTable->nStrLength;
        }

        // are some parser function defined by each pData parameter ?

        else if (pCmdTable->pDataParserTbl)
        {
            // attempt to find parser function

            for (PDATAPARSERTBL pDataTbl = pCmdTable->pDataParserTbl; pDataTbl->nIndex >= 0; pDataTbl++)
            {
                // re-initialize result status

                lResult = RCYPSR_E_UNKNOWNDATA;

                // valid data ptr is passed ?

                if (! pData)
                {   
                    lResult = RCYPSR_E_INVALIDPARAM;

                    break;
                }

                // found parser function ?

                if (pDataTbl->lData != *pData)
                {
                    continue;
                }

                // save variables

                pfnParser      = pDataTbl->pfnParser;
                nNeedBuffSize  = pDataTbl->nBuffSize;
                nNeedStrLength = pDataTbl->nStrLength;

                break;
            }
        }

        // check to see if suitable parameters are passed
    
        if (pfnParser)
        {
            // is the buffer size suitable ?

            if (nNeedBuffSize != nBufferSize)
            {
                // set up result status

                lResult = RCYPSR_E_BUFFERSIZE;
            }

            // is the length of string suitable ?

            else if (nNeedStrLength != _tcslen(lpszString))
            {
                // set up result status

                lResult = RCYPSR_E_STRINGLENGTH;
            }

            // suitable parameters are passed.

            else
            {
                // pass control to parser function

                lResult = (pfnParser)(Command, pData, lpszString, pvResult, nBufferSize);
            }
        }

        break;
    }

    // debugging...
#if (_TRACE_LOG)
    trace(L6, _T("-Parse(): lResult=%d"), lResult);
#endif
    // exit...

    return (lResult);
}


/////////////////////////////////////////////////////////////////////////////
// Local Function Implementation
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// Function:    Parsing CHAN_DI_STATUSREAD Command with ReadNo01 string.
//
// Prototype:   bool ParseStatusRead01(LONG Command, LONG* pData, LPCTSTR lpszString, 
//                         PVOID pvResult, UINT nBufferSize)
//
// Inputs:      LONG    Command      : [in]         command id
//              LONG*   pData        : [in]         ptr. to command data
//              LPCTSTR lpszString   : [in]         string returned by DirectIO function.
//              PVOID   pvResult     : [in / out]   ptr. to result of parsing
//              UINT    nBufferSize  : [in]         number of bytes of the result buffer
//
// Outputs:     long lResult
//
/////////////////////////////////////////////////////////////////////////////

long ParseStatusRead01(LONG Command, LONG* pData, LPCTSTR lpszString, 
                               PVOID pvResult, UINT nBufferSize)
{
    // debugging...
#if (_TRACE_LOG)
    trace(L6, _T("+ParseStatusRead01()"));
#endif
    // initialize

    long lResult = RCYPSR_E_PARSINGERR;

    // debugging...
#if (_TRACE_LOG)
    trace(L6, _T("*ParseStatusRead01() : Data=%d, String=%s"), *pData, lpszString);
#endif

    // copy string data to working buffer

    CString strBuffer = lpszString;

    // get target buffer ptr.

    PRCYPSRSTATUS01 pTarget = (PRCYPSRSTATUS01)pvResult;

    // define parse sequence

    enum 
    {
        SEQ_ERRORCODE = 0,          
        SEQ_NOTUSED1,       SEQ_STORAGE1,       SEQ_STORAGE2,       
        SEQ_STORAGE3,       SEQ_STORAGE4,       SEQ_NOTUSED2,       
        SEQ_BALANCEFLAG,    SEQ_NOTUSED3,       SEQ_COUNT500YINRJ,  
        SEQ_COUNT100YINRJ,  SEQ_COUNT50YINRJ,   SEQ_COUNT10YINRJ,   
        SEQ_COUNT5YINRJ,    SEQ_COUNT1YINRJ,    SEQ_COUNTRJINRJ,    
        SEQ_NOTUSED4,       SEQ_COUNT500Y,      SEQ_COUNT100Y,      
        SEQ_COUNT50Y,       SEQ_COUNT10Y,       SEQ_COUNT5Y,        
        SEQ_COUNT1Y,        SEQ_COUNTRJ,        SEQ_NOTUSED5,       
        SEQ_END
    };

    // define field length table

    static FIELDLENGTHTBL FieldLenTbl[] = 
    {
        {  1,   SEQ_ERRORCODE,      STATREAD01_LEN_ERRORCODE        },
        {  2,   SEQ_NOTUSED1,       STATREAD01_LEN_NOTUSED1         },
        {  3,   SEQ_STORAGE1,       STATREAD01_LEN_STORAGE1         },
        {  4,   SEQ_STORAGE2,       STATREAD01_LEN_STORAGE2         },
        {  5,   SEQ_STORAGE3,       STATREAD01_LEN_STORAGE3         },
        {  6,   SEQ_STORAGE4,       STATREAD01_LEN_STORAGE4         },
        {  7,   SEQ_NOTUSED2,       STATREAD01_LEN_NOTUSED2         },
        {  8,   SEQ_BALANCEFLAG,    STATREAD01_LEN_BALANCEFLAG      },
        {  9,   SEQ_NOTUSED3,       STATREAD01_LEN_NOTUSED3         },
        { 10,   SEQ_COUNT500YINRJ,  STATREAD01_LEN_COUNT500YINRJ    },
        { 11,   SEQ_COUNT100YINRJ,  STATREAD01_LEN_COUNT100YINRJ    },
        { 12,   SEQ_COUNT50YINRJ,   STATREAD01_LEN_COUNT50YINRJ     },
        { 13,   SEQ_COUNT10YINRJ,   STATREAD01_LEN_COUNT10YINRJ     },
        { 14,   SEQ_COUNT5YINRJ,    STATREAD01_LEN_COUNT5YINRJ      },
        { 15,   SEQ_COUNT1YINRJ,    STATREAD01_LEN_COUNT1YINRJ      },
        { 16,   SEQ_COUNTRJINRJ,    STATREAD01_LEN_COUNTRJINRJ      },
        { 17,   SEQ_NOTUSED4,       STATREAD01_LEN_NOTUSED4         },
        { 18,   SEQ_COUNT500Y,      STATREAD01_LEN_COUNT500Y        },
        { 19,   SEQ_COUNT100Y,      STATREAD01_LEN_COUNT100Y        },
        { 20,   SEQ_COUNT50Y,       STATREAD01_LEN_COUNT50Y         },
        { 21,   SEQ_COUNT10Y,       STATREAD01_LEN_COUNT10Y         },
        { 22,   SEQ_COUNT5Y,        STATREAD01_LEN_COUNT5Y          },
        { 23,   SEQ_COUNT1Y,        STATREAD01_LEN_COUNT1Y          },
        { 24,   SEQ_COUNTRJ,        STATREAD01_LEN_COUNTRJ          },
        { 25,   SEQ_NOTUSED5,       STATREAD01_LEN_NOTUSED5         },
        { -1,   -1,                 -1                              }
    };
    
    // initialize working variable

    int nFirst = 0;
    int nSeq   = 0;

    for(nSeq = SEQ_ERRORCODE, nFirst = 0; nSeq != SEQ_END; nSeq++)
    {
        // initialize 

        int    nLength   = 0;
        
        // attempt to get current sequence info.

        for (PFIELDLENGTHTBL pTable = FieldLenTbl; pTable->nIndex >= 0; pTable++)
        {
            // found same sequence ?

            if (pTable->nField == nSeq)
            {
                // get length

                nLength = pTable->nLength;

                break;
            }
        }

        // could get field length ?

        if (nLength == 0)
        {
            continue;
        }

        // initialize working variables

        ULONG  nValue    = 0;
        TCHAR* pszString = NULL;
        int    nBase     = 16;

        // get field string

        CString strField = strBuffer.Mid(nFirst, nLength);

        // what field ?

        switch (nSeq)
        {
        case SEQ_ERRORCODE:
            pTarget->lErrorCode = _ttol((LPCTSTR)strField);
            break;
        case SEQ_NOTUSED1:
            pTarget->strNotUsed1 = strField;
            break;
        case SEQ_STORAGE1:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->wStatusOf500Yen = (WORD)(GET_COINSTAT(GET_BIT12(nValue)));
            pTarget->wStatusOf100Yen = (WORD)(GET_COINSTAT(GET_BIT34(nValue)));
            break;
        case SEQ_STORAGE2:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->wStatusOf50Yen = (WORD)(GET_COINSTAT(GET_BIT12(nValue)));
            pTarget->wStatusOf10Yen = (WORD)(GET_COINSTAT(GET_BIT34(nValue)));
            break;
        case SEQ_STORAGE3:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->wStatusOf5Yen = (WORD)(GET_COINSTAT(GET_BIT12(nValue)));
            pTarget->wStatusOf1Yen = (WORD)(GET_COINSTAT(GET_BIT34(nValue)));
            break;
        case SEQ_STORAGE4:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->wStatusOfRJ = (WORD)(GET_COINSTAT(GET_BIT12(nValue)));
            break;
        case SEQ_NOTUSED2:
            pTarget->strNotUsed2 = strField;
            break;
        case SEQ_BALANCEFLAG:
            pTarget->fAbnormalBalance = (strField.Right(1).CompareNoCase(_T("1")) == 0) ? 
                                      true : false;
            break;
        case SEQ_NOTUSED3:
            pTarget->strNotUsed3 = strField;
            break;
        case SEQ_COUNT500YINRJ:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOf500YenInRJ = (int)nValue;
            break;
        case SEQ_COUNT100YINRJ:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOf100YenInRJ = (int)nValue;
            break;
        case SEQ_COUNT50YINRJ:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOf50YenInRJ = (int)nValue;
            break;
        case SEQ_COUNT10YINRJ:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOf10YenInRJ = (int)nValue;
            break;
        case SEQ_COUNT5YINRJ:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOf5YenInRJ = (int)nValue;
            break;
        case SEQ_COUNT1YINRJ:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOf1YenInRJ = (int)nValue;
            break;
        case SEQ_COUNTRJINRJ:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOfRejectedInRJ = (int)nValue;
            break;
        case SEQ_NOTUSED4:
            pTarget->strNotUsed4 = strField;
            break;
        case SEQ_COUNT500Y:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOf500Yen = (int)nValue;
            break;
        case SEQ_COUNT100Y:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOf100Yen = (int)nValue;
            break;
        case SEQ_COUNT50Y:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOf50Yen = (int)nValue;
            break;
        case SEQ_COUNT10Y:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOf10Yen = (int)nValue;
            break;
        case SEQ_COUNT5Y:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOf5Yen = (int)nValue;
            break;
        case SEQ_COUNT1Y:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOf1Yen = (int)nValue;
            break;
        case SEQ_COUNTRJ:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOfRejected = (int)nValue;
            break;
        case SEQ_NOTUSED5:
            pTarget->strNotUsed5 = strField;
            break;
        default:
            break;
        }

        // preparate for next string

        nFirst += nLength;
    }

    // set up result status

    lResult = RCYPSR_SUCCESS;

    // debugging...
#if (_TRACE_DETAILLOG)
    trace(L6, _T("*ParseStatusRead01() : lErrorCode=%d"),               pTarget->lErrorCode);
    trace(L6, _T("*ParseStatusRead01() : strNotUsed1=%s"),              (LPCTSTR)pTarget->strNotUsed1);
    trace(L6, _T("*ParseStatusRead01() : wStatusOf500Yen=%d"),          pTarget->wStatusOf500Yen);
    trace(L6, _T("*ParseStatusRead01() : wStatusOf100Yen=%d"),          pTarget->wStatusOf100Yen);
    trace(L6, _T("*ParseStatusRead01() : wStatusOf50Yen=%d"),           pTarget->wStatusOf50Yen);
    trace(L6, _T("*ParseStatusRead01() : wStatusOf10Yen=%d"),           pTarget->wStatusOf10Yen);
    trace(L6, _T("*ParseStatusRead01() : wStatusOf5Yen=%d"),            pTarget->wStatusOf5Yen);
    trace(L6, _T("*ParseStatusRead01() : wStatusOf1Yen=%d"),            pTarget->wStatusOf1Yen);
    trace(L6, _T("*ParseStatusRead01() : wStatusOfRJ=%d"),              pTarget->wStatusOfRJ);
    trace(L6, _T("*ParseStatusRead01() : strNotUsed2=%s"),              (LPCTSTR)pTarget->strNotUsed2);
    trace(L6, _T("*ParseStatusRead01() : fAbnormalBalance=%d"),         pTarget->fAbnormalBalance);
    trace(L6, _T("*ParseStatusRead01() : strNotUsed3=%s"),              (LPCTSTR)pTarget->strNotUsed3);
    trace(L6, _T("*ParseStatusRead01() : nCountOf500YenInRJ=%d"),       pTarget->nCountOf500YenInRJ);
    trace(L6, _T("*ParseStatusRead01() : nCountOf100YenInRJ=%d"),       pTarget->nCountOf100YenInRJ);
    trace(L6, _T("*ParseStatusRead01() : nCountOf50YenInRJ=%d"),        pTarget->nCountOf50YenInRJ);
    trace(L6, _T("*ParseStatusRead01() : nCountOf10YenInRJ=%d"),        pTarget->nCountOf10YenInRJ);
    trace(L6, _T("*ParseStatusRead01() : nCountOf5YenInRJ=%d"),         pTarget->nCountOf5YenInRJ);
    trace(L6, _T("*ParseStatusRead01() : nCountOf1YenInRJ=%d"),         pTarget->nCountOf1YenInRJ);
    trace(L6, _T("*ParseStatusRead01() : nCountOfRejectedInRJ=%d"),     pTarget->nCountOfRejectedInRJ);
    trace(L6, _T("*ParseStatusRead01() : strNotUsed4=%s"),              (LPCTSTR)pTarget->strNotUsed4);
    trace(L6, _T("*ParseStatusRead01() : nCountOf500Yen=%d"),           pTarget->nCountOf500Yen);
    trace(L6, _T("*ParseStatusRead01() : nCountOf100Yen=%d"),           pTarget->nCountOf100Yen);
    trace(L6, _T("*ParseStatusRead01() : nCountOf50Yen=%d"),            pTarget->nCountOf50Yen);
    trace(L6, _T("*ParseStatusRead01() : nCountOf10Yen=%d"),            pTarget->nCountOf10Yen);
    trace(L6, _T("*ParseStatusRead01() : nCountOf5Yen=%d"),             pTarget->nCountOf5Yen);
    trace(L6, _T("*ParseStatusRead01() : nCountOf1Yen=%d"),             pTarget->nCountOf1Yen);
    trace(L6, _T("*ParseStatusRead01() : nCountOfRejected=%d"),         pTarget->nCountOfRejected);
    trace(L6, _T("*ParseStatusRead01() : strNotUsed5=%s"),              (LPCTSTR)pTarget->strNotUsed5);
#endif

    // debugging...
#if (_TRACE_LOG)
    trace(L6, _T("-ParseStatusRead01()"));
#endif
    // exit...

    return (lResult);
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:    Parsing CHAN_DI_STATUSREAD Command with ReadNo80 string.
//
// Prototype:   bool ParseStatusRead80(LONG Command, LONG* pData, LPCTSTR lpszString, 
//                         PVOID pvResult, UINT nBufferSize)
//
// Inputs:      LONG    Command      : [in]         command id
//              LONG*   pData        : [in]         ptr. to command data
//              LPCTSTR lpszString   : [in]         string returned by DirectIO function.
//              PVOID   pvResult     : [in / out]   ptr. to result of parsing
//              UINT    nBufferSize  : [in]         number of bytes of the result buffer
//
// Outputs:     long lResult
//
/////////////////////////////////////////////////////////////////////////////

long ParseStatusRead80(LONG Command, LONG* pData, LPCTSTR lpszString, 
                               PVOID pvResult, UINT nBufferSize)
{
    // debugging...
#if (_TRACE_LOG)
    trace(L6, _T("+ParseStatusRead80()"));
#endif
    // initialize

    long lResult = RCYPSR_E_PARSINGERR;

    // debugging...
#if (_TRACE_LOG)
    trace(L6, _T("*ParseStatusRead80() : Data=%d, String=%s"), *pData, lpszString);
#endif

    // copy string data to working buffer

    CString strBuffer = lpszString;

    // get target buffer ptr.

    PRCYPSRSTATUS80 pTarget = (PRCYPSRSTATUS80)pvResult;

    // define parse sequence

    enum 
    {
        SEQ_ERRORCODE = 0,          
        SEQ_NOTUSED1,       SEQ_STORAGE1,       SEQ_STORAGE2,       
        SEQ_STORAGE3,       SEQ_STORAGE4,       SEQ_STORAGE5,
        SEQ_STORAGE6,       SEQ_NOTUSED2,       SEQ_BALANCEFLAG,
        SEQ_OPERATIONFLAG,  SEQ_COUNT10000Y,    SEQ_COUNT5000Y,
        SEQ_COUNT2000Y,     SEQ_COUNT1000Y,     SEQ_NOTUSED3,
        SEQ_END
    };

    // define field length table

    static FIELDLENGTHTBL FieldLenTbl[] = 
    {
        {  1,   SEQ_ERRORCODE,      STATREAD80_LEN_ERRORCODE        },
        {  2,   SEQ_NOTUSED1,       STATREAD80_LEN_NOTUSED1         },
        {  3,   SEQ_STORAGE1,       STATREAD80_LEN_STORAGE1         },
        {  4,   SEQ_STORAGE2,       STATREAD80_LEN_STORAGE2         },
        {  5,   SEQ_STORAGE3,       STATREAD80_LEN_STORAGE3         },
        {  6,   SEQ_STORAGE4,       STATREAD80_LEN_STORAGE4         },
        {  7,   SEQ_STORAGE5,       STATREAD80_LEN_STORAGE5         },
        {  8,   SEQ_STORAGE6,       STATREAD80_LEN_STORAGE6         },
        {  9,   SEQ_NOTUSED2,       STATREAD80_LEN_NOTUSED2         },
        { 10,   SEQ_BALANCEFLAG,    STATREAD80_LEN_BALANCEFLAG      },
        { 11,   SEQ_OPERATIONFLAG,  STATREAD80_LEN_OPERATIONFLAG    },
        { 12,   SEQ_COUNT10000Y,    STATREAD80_LEN_COUNT10000Y      },
        { 13,   SEQ_COUNT5000Y,     STATREAD80_LEN_COUNT5000Y       },
        { 14,   SEQ_COUNT2000Y,     STATREAD80_LEN_COUNT2000Y       },
        { 15,   SEQ_COUNT1000Y,     STATREAD80_LEN_COUNT1000Y       },
        { 16,   SEQ_NOTUSED3,       STATREAD80_LEN_NOTUSED3         },
        { -1,   -1,                 -1                              }
    };
    
    // initialize working variable

    int nFirst = 0;
    int nSeq   = 0;

    for(nSeq = SEQ_ERRORCODE, nFirst = 0; nSeq != SEQ_END; nSeq++)
    {
        // initialize 

        int    nLength   = 0;
        
        // attempt to get current sequence info.

        for (PFIELDLENGTHTBL pTable = FieldLenTbl; pTable->nIndex >= 0; pTable++)
        {
            // found same sequence ?

            if (pTable->nField == nSeq)
            {
                // get length

                nLength = pTable->nLength;

                break;
            }
        }

        // could get field length ?

        if (nLength == 0)
        {
            continue;
        }

        // initialize working variables

        ULONG  nValue    = 0;
        TCHAR* pszString = NULL;
        int    nBase     = 16;

        // get field string

        CString strField = strBuffer.Mid(nFirst, nLength);

        // what field ?

        switch (nSeq)
        {
        case SEQ_ERRORCODE:
            pTarget->lErrorCode = _ttol((LPCTSTR)strField);
            break;
        case SEQ_NOTUSED1:
            pTarget->strNotUsed1 = strField;
            break;
        case SEQ_STORAGE1:
            nValue = (ULONG)(_ttol((LPCTSTR)strField));
            pTarget->wStatusOf10000Yen = (WORD)(GET_BILLSTAT(nValue));
            break;
        case SEQ_STORAGE2:
            nValue = (ULONG)(_ttol((LPCTSTR)strField));
            pTarget->wStatusOf5000Yen = (WORD)(GET_BILLSTAT(nValue));
            break;
        case SEQ_STORAGE3:
            nValue = (ULONG)(_ttol((LPCTSTR)strField));
            pTarget->wStatusOf2000Yen = (WORD)(GET_BILLSTAT(nValue));
            break;
        case SEQ_STORAGE4:
            nValue = (ULONG)(_ttol((LPCTSTR)strField));
            pTarget->wStatusOf1000Yen = (WORD)(GET_BILLSTAT(nValue));
            break;
        case SEQ_STORAGE5:
            nValue = (ULONG)(_ttol((LPCTSTR)strField));
            pTarget->wStatusOfCassette = (WORD)(GET_BILLSTAT(nValue));
            break;
        case SEQ_STORAGE6:
            nValue = (ULONG)(_ttol((LPCTSTR)strField));
            pTarget->wStatusOfRJ = (WORD)(GET_BILLSTAT(nValue));
            break;
        case SEQ_NOTUSED2:
            pTarget->strNotUsed2 = strField;
            break;
        case SEQ_BALANCEFLAG:
            pTarget->fAbnormalBalance = (strField.Right(1).CompareNoCase(_T("1")) == 0) ? 
                                      true : false;
            break;
        case SEQ_OPERATIONFLAG:
            pTarget->strOperationFlag = strField;
            break;
        case SEQ_COUNT10000Y:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOf10000Yen = (int)nValue;
            break;
        case SEQ_COUNT5000Y:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOf5000Yen = (int)nValue;
            break;
        case SEQ_COUNT2000Y:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOf2000Yen = (int)nValue;
            break;
        case SEQ_COUNT1000Y:
            nValue = _tcstoul((LPCTSTR)strField, &pszString, nBase);
            pTarget->nCountOf1000Yen = (int)nValue;
            break;
        case SEQ_NOTUSED3:
            pTarget->strNotUsed3 = strField;
            break;
        default:
            break;
        }

        // preparate for next string

        nFirst += nLength;
    }

    // set up result status

    lResult = RCYPSR_SUCCESS;

    // debugging...
#if (_TRACE_DETAILLOG)
    trace(L6, _T("*ParseStatusRead80() : lErrorCode=%d"),           pTarget->lErrorCode);
    trace(L6, _T("*ParseStatusRead80() : strNotUsed1=%s"),          (LPCTSTR)pTarget->strNotUsed1);
    trace(L6, _T("*ParseStatusRead80() : wStatusOf10000Yen=%d"),    pTarget->wStatusOf10000Yen);
    trace(L6, _T("*ParseStatusRead80() : wStatusOf5000Yen=%d"),     pTarget->wStatusOf5000Yen);
    trace(L6, _T("*ParseStatusRead80() : wStatusOf2000Yen=%d"),     pTarget->wStatusOf2000Yen);
    trace(L6, _T("*ParseStatusRead80() : wStatusOf1000Yen=%d"),     pTarget->wStatusOf1000Yen);
    trace(L6, _T("*ParseStatusRead80() : wStatusOfCassette=%d"),    pTarget->wStatusOfCassette);
    trace(L6, _T("*ParseStatusRead80() : wStatusOfRJ=%d"),          pTarget->wStatusOfRJ);
    trace(L6, _T("*ParseStatusRead80() : strNotUsed2=%s"),          (LPCTSTR)pTarget->strNotUsed2);
    trace(L6, _T("*ParseStatusRead80() : fAbnormalBalance=%d"),     pTarget->fAbnormalBalance);
    trace(L6, _T("*ParseStatusRead80() : strOperationFlag=%s"),     (LPCTSTR)pTarget->strOperationFlag);
    trace(L6, _T("*ParseStatusRead80() : nCountOf10000Yen=%d"),     pTarget->nCountOf10000Yen);
    trace(L6, _T("*ParseStatusRead80() : nCountOf5000Yen=%d"),      pTarget->nCountOf5000Yen);
    trace(L6, _T("*ParseStatusRead80() : nCountOf2000Yen=%d"),      pTarget->nCountOf2000Yen);
    trace(L6, _T("*ParseStatusRead80() : nCountOf1000Yen=%d"),      pTarget->nCountOf1000Yen);
    trace(L6, _T("*ParseStatusRead80() : strNotUsed3=%s"),          (LPCTSTR)pTarget->strNotUsed3);
#endif
    // debugging...
#if (_TRACE_LOG)
    trace(L6, _T("-ParseStatusRead80()"));
#endif
    // exit...

    return (lResult);
}

/////////////////////////////////////////////////////////////////////////////
//
// Function:    Parsing CHAN_DI_SEISA Command string.
//
// Prototype:   bool ParseSeisa(LONG Command, LONG* pData, LPCTSTR lpszString, 
//                              PVOID pvResult, UINT nBufferSize)
//
// Inputs:      LONG    Command      : [in]         command id
//              LONG*   pData        : [in]         ptr. to command data
//              LPCTSTR lpszString   : [in]         string returned by DirectIO function.
//              PVOID   pvResult     : [in / out]   ptr. to result of parsing
//              UINT    nBufferSize  : [in]         number of bytes of the result buffer
//
// Outputs:     long lResult
//
/////////////////////////////////////////////////////////////////////////////

long ParseSeisa(LONG Command, LONG* pData, LPCTSTR lpszString, PVOID pvResult, UINT nBufferSize)
{
    // debugging...
#if (_TRACE_LOG)
    trace(L6, _T("+ParseSeisa()"));
#endif
    // initialize

    long lResult = RCYPSR_E_PARSINGERR;

    // debugging...
#if (_TRACE_LOG)
    trace(L6, _T("*ParseSeisa() : String=%s"), lpszString);
#endif

    // copy string data to working buffer

    CString strBuffer = lpszString;

    // get target buffer ptr.

    PRCYPSRSEISA pTarget = (PRCYPSRSEISA)pvResult;

    // define parse sequence

    enum 
    {
        SEQ_CNTOF2000Y = 0,         
        SEQ_CNTOF10000Y,            SEQ_CNTOF5000Y,             SEQ_CNTOF1000Y,     
        SEQ_CNTOF2000YINCST,        SEQ_CNTOF10000YINCST,       SEQ_CNTOF5000YINCST,    
        SEQ_CNTOF1000YINCST,        SEQ_CNTOF500Y,              SEQ_CNTOF100Y,      
        SEQ_CNTOF50Y,               SEQ_CNTOF10Y,               SEQ_CNTOF5Y,    
        SEQ_CNTOF1Y,                SEQ_CNTOF2000YINLASTCST,    SEQ_CNTOF10000YINLASTCST,
        SEQ_CNTOF5000YINLASTCST,    SEQ_CNTOF1000YINLASTCST,    SEQ_CNTOF2000YINCMLTCST,
        SEQ_CNTOF10000YINCMLTCST,   SEQ_CNTOF5000YINCMLTCST,    SEQ_CNTOF1000YINCMLTCST,
        SEQ_NOTEBALANCESTAT,        SEQ_NOTEFULLSTAT,           SEQ_COINBALANCESTAT,
        SEQ_COINFULLSTAT,           SEQ_NUMOFNOTERJ,            SEQ_NUMOFCOINRJ,
        SEQ_OVERFLOW500Y,           SEQ_OVERFLOW100Y,           SEQ_OVERFLOW50Y,
        SEQ_OVERFLOW10Y,            SEQ_OVERFLOW5Y,             SEQ_OVERFLOW1Y,
        SEQ_END
    };

    // define field length table

    static FIELDLENGTHTBL FieldLenTbl[] = 
    {
        {  1,   SEQ_CNTOF2000Y,                 SEISA_LEN_CNTOF2000Y                },
        {  2,   SEQ_CNTOF10000Y,                SEISA_LEN_CNTOF10000Y               },
        {  3,   SEQ_CNTOF5000Y,                 SEISA_LEN_CNTOF5000Y                },
        {  4,   SEQ_CNTOF1000Y,                 SEISA_LEN_CNTOF1000Y                },
        {  5,   SEQ_CNTOF2000YINCST,            SEISA_LEN_CNTOF2000YINCST           },
        {  6,   SEQ_CNTOF10000YINCST,           SEISA_LEN_CNTOF10000YINCST          },
        {  7,   SEQ_CNTOF5000YINCST,            SEISA_LEN_CNTOF5000YINCST           },
        {  8,   SEQ_CNTOF1000YINCST,            SEISA_LEN_CNTOF1000YINCST           },
        {  9,   SEQ_CNTOF500Y,                  SEISA_LEN_CNTOF500Y                 },
        { 10,   SEQ_CNTOF100Y,                  SEISA_LEN_CNTOF100Y                 },
        { 11,   SEQ_CNTOF50Y,                   SEISA_LEN_CNTOF50Y                  },
        { 12,   SEQ_CNTOF10Y,                   SEISA_LEN_CNTOF10Y                  },
        { 13,   SEQ_CNTOF5Y,                    SEISA_LEN_CNTOF5Y                   },
        { 14,   SEQ_CNTOF1Y,                    SEISA_LEN_CNTOF1Y                   },
        { 15,   SEQ_CNTOF2000YINLASTCST,        SEISA_LEN_CNTOF2000YINLASTCST       },
        { 16,   SEQ_CNTOF10000YINLASTCST,       SEISA_LEN_CNTOF10000YINLASTCST      },
        { 17,   SEQ_CNTOF5000YINLASTCST,        SEISA_LEN_CNTOF5000YINLASTCST       },
        { 18,   SEQ_CNTOF1000YINLASTCST,        SEISA_LEN_CNTOF1000YINLASTCST       },
        { 19,   SEQ_CNTOF2000YINCMLTCST,        SEISA_LEN_CNTOF2000YINCMLTCST       },
        { 20,   SEQ_CNTOF10000YINCMLTCST,       SEISA_LEN_CNTOF10000YINCMLTCST      },
        { 21,   SEQ_CNTOF5000YINCMLTCST,        SEISA_LEN_CNTOF5000YINCMLTCST       },
        { 22,   SEQ_CNTOF1000YINCMLTCST,        SEISA_LEN_CNTOF1000YINCMLTCST       },
        { 23,   SEQ_NOTEBALANCESTAT,            SEISA_LEN_NOTEBALANCESTAT           },
        { 24,   SEQ_NOTEFULLSTAT,               SEISA_LEN_NOTEFULLSTAT              },
        { 25,   SEQ_COINBALANCESTAT,            SEISA_LEN_COINBALANCESTAT           },
        { 26,   SEQ_COINFULLSTAT,               SEISA_LEN_COINFULLSTAT              },
        { 27,   SEQ_NUMOFNOTERJ,                SEISA_LEN_NUMOFNOTERJ               },
        { 28,   SEQ_NUMOFCOINRJ,                SEISA_LEN_NUMOFCOINRJ               },
        { 29,   SEQ_OVERFLOW500Y,               SEISA_LEN_OVERFLOW500Y              },
        { 30,   SEQ_OVERFLOW100Y,               SEISA_LEN_OVERFLOW100Y              },
        { 31,   SEQ_OVERFLOW50Y,                SEISA_LEN_OVERFLOW50Y               },
        { 32,   SEQ_OVERFLOW10Y,                SEISA_LEN_OVERFLOW10Y               },
        { 33,   SEQ_OVERFLOW5Y,                 SEISA_LEN_OVERFLOW5Y                },
        { 34,   SEQ_OVERFLOW1Y,                 SEISA_LEN_OVERFLOW1Y                },
        { -1,   -1,                             -1                                  }
    };
    
    // initialize working variable

    int nFirst = 0;
    int nSeq   = 0;

    for(nSeq = SEQ_CNTOF2000Y, nFirst = 0; nSeq != SEQ_END; nSeq++)
    {
        // initialize 

        int    nLength   = 0;
        
        // attempt to get current sequence info.

        for (PFIELDLENGTHTBL pTable = FieldLenTbl; pTable->nIndex >= 0; pTable++)
        {
            // found same sequence ?

            if (pTable->nField == nSeq)
            {
                // get length

                nLength = pTable->nLength;

                break;
            }
        }

        // could get field length ?

        if (nLength == 0)
        {
            continue;
        }

        // initialize working variables

        ULONG  nValue    = 0;
        TCHAR* pszString = NULL;
        int    nBase     = 16;

        // get field string

        CString strField = strBuffer.Mid(nFirst, nLength);

        // what field ?

        switch (nSeq)
        {
        case SEQ_CNTOF2000Y:
            pTarget->nCountOf2000Yen = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF10000Y:
            pTarget->nCountOf10000Yen = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF5000Y:
            pTarget->nCountOf5000Yen = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF1000Y:
            pTarget->nCountOf1000Yen = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF2000YINCST:
            pTarget->nCountOf2000YenInCst = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF10000YINCST:
            pTarget->nCountOf10000YenInCst = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF5000YINCST:
            pTarget->nCountOf5000YenInCst = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF1000YINCST:
            pTarget->nCountOf1000YenInCst = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF500Y:
            pTarget->nCountOf500Yen = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF100Y:
            pTarget->nCountOf100Yen = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF50Y:
            pTarget->nCountOf50Yen = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF10Y:
            pTarget->nCountOf10Yen = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF5Y:
            pTarget->nCountOf5Yen = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF1Y:
            pTarget->nCountOf1Yen = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF2000YINLASTCST:
            pTarget->nCountOf2000YenInLastCst = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF10000YINLASTCST:
            pTarget->nCountOf10000YenInLastCst = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF5000YINLASTCST:
            pTarget->nCountOf5000YenInLastCst = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF1000YINLASTCST:
            pTarget->nCountOf1000YenInLastCst = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF2000YINCMLTCST:
            pTarget->nCountOf2000YenInCmltCst = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF10000YINCMLTCST:
            pTarget->nCountOf10000YenInCmltCst = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF5000YINCMLTCST:
            pTarget->nCountOf5000YenInCmltCst = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_CNTOF1000YINCMLTCST:
            pTarget->nCountOf1000YenInCmltCst = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_NOTEBALANCESTAT:
            pTarget->wNoteBalanceStatus = (WORD)_ttoi((LPCTSTR)strField);
            break;
        case SEQ_NOTEFULLSTAT:
            pTarget->wNoteFullStatus = (WORD)_ttoi((LPCTSTR)strField);
            break;
        case SEQ_COINBALANCESTAT:
            pTarget->wCoinBalanceStatus = (WORD)_ttoi((LPCTSTR)strField);
            break;
        case SEQ_COINFULLSTAT:
            pTarget->wCoinFullStatus = (WORD)_ttoi((LPCTSTR)strField);
            break;
        case SEQ_NUMOFNOTERJ:
            pTarget->nNumberOfNoteRJ = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_NUMOFCOINRJ:
            pTarget->nNumberOfCoinRJ = _ttoi((LPCTSTR)strField); 
            break;
        case SEQ_OVERFLOW500Y:
            pTarget->nOverflowOf500Yen = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_OVERFLOW100Y:
            pTarget->nOverflowOf100Yen = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_OVERFLOW50Y:
            pTarget->nOverflowOf50Yen = _ttoi((LPCTSTR)strField);
            break;      
        case SEQ_OVERFLOW10Y:
            pTarget->nOverflowOf10Yen = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_OVERFLOW5Y:
            pTarget->nOverflowOf5Yen = _ttoi((LPCTSTR)strField);
            break;
        case SEQ_OVERFLOW1Y:
            pTarget->nOverflowOf1Yen = _ttoi((LPCTSTR)strField);
            break;
        default:
            break;
        }

        // preparate for next string

        nFirst += nLength;
    }

    // set up result status

    lResult = RCYPSR_SUCCESS;

    // debugging...
#if (_TRACE_DETAILLOG)
    trace(L6, _T("*ParseSeisa() : nCountOf2000Yen=%d"),             pTarget->nCountOf2000Yen);
    trace(L6, _T("*ParseSeisa() : nCountOf10000Yen=%d"),            pTarget->nCountOf10000Yen);
    trace(L6, _T("*ParseSeisa() : nCountOf5000Yen=%d"),             pTarget->nCountOf5000Yen);
    trace(L6, _T("*ParseSeisa() : nCountOf1000Yen=%d"),             pTarget->nCountOf1000Yen);
    trace(L6, _T("*ParseSeisa() : nCountOf2000YenInCst=%d"),        pTarget->nCountOf2000YenInCst);
    trace(L6, _T("*ParseSeisa() : nCountOf10000YenInCst=%d"),       pTarget->nCountOf10000YenInCst);
    trace(L6, _T("*ParseSeisa() : nCountOf5000YenInCst=%d"),        pTarget->nCountOf5000YenInCst);
    trace(L6, _T("*ParseSeisa() : nCountOf1000YenInCst=%d"),        pTarget->nCountOf1000YenInCst);
    trace(L6, _T("*ParseSeisa() : nCountOf500Yen=%d"),              pTarget->nCountOf500Yen);
    trace(L6, _T("*ParseSeisa() : nCountOf100Yen=%d"),              pTarget->nCountOf100Yen);
    trace(L6, _T("*ParseSeisa() : nCountOf50Yen=%d"),               pTarget->nCountOf50Yen);
    trace(L6, _T("*ParseSeisa() : nCountOf10Yen=%d"),               pTarget->nCountOf10Yen);
    trace(L6, _T("*ParseSeisa() : nCountOf5Yen=%d"),                pTarget->nCountOf5Yen);
    trace(L6, _T("*ParseSeisa() : nCountOf1Yen=%d"),                pTarget->nCountOf1Yen);
    trace(L6, _T("*ParseSeisa() : nCountOf2000YenInLastCst=%d"),    pTarget->nCountOf2000YenInLastCst);
    trace(L6, _T("*ParseSeisa() : nCountOf10000YenInLastCst=%d"),   pTarget->nCountOf10000YenInLastCst);
    trace(L6, _T("*ParseSeisa() : nCountOf5000YenInLastCst=%d"),    pTarget->nCountOf5000YenInLastCst);
    trace(L6, _T("*ParseSeisa() : nCountOf1000YenInLastCst=%d"),    pTarget->nCountOf1000YenInLastCst);
    trace(L6, _T("*ParseSeisa() : nCountOf2000YenInCmltCst=%d"),    pTarget->nCountOf2000YenInCmltCst);
    trace(L6, _T("*ParseSeisa() : nCountOf10000YenInCmltCst=%d"),   pTarget->nCountOf10000YenInCmltCst);
    trace(L6, _T("*ParseSeisa() : nCountOf5000YenInCmltCst=%d"),    pTarget->nCountOf5000YenInCmltCst);
    trace(L6, _T("*ParseSeisa() : nCountOf1000YenInCmltCst=%d"),    pTarget->nCountOf1000YenInCmltCst);
    trace(L6, _T("*ParseSeisa() : wNoteBalanceStatus=%d"),          pTarget->wNoteBalanceStatus);
    trace(L6, _T("*ParseSeisa() : wNoteFullStatus=%d"),             pTarget->wNoteFullStatus);
    trace(L6, _T("*ParseSeisa() : wCoinBalanceStatus=%d"),          pTarget->wCoinBalanceStatus);
    trace(L6, _T("*ParseSeisa() : wCoinFullStatus=%d"),             pTarget->wCoinFullStatus);
    trace(L6, _T("*ParseSeisa() : nNumberOfNoteRJ=%d"),             pTarget->nNumberOfNoteRJ);
    trace(L6, _T("*ParseSeisa() : nNumberOfCoinRJ=%d"),             pTarget->nNumberOfCoinRJ);
    trace(L6, _T("*ParseSeisa() : nOverflowOf500Yen=%d"),           pTarget->nOverflowOf500Yen);
    trace(L6, _T("*ParseSeisa() : nOverflowOf100Yen=%d"),           pTarget->nOverflowOf100Yen);
    trace(L6, _T("*ParseSeisa() : nOverflowOf50Yen=%d"),            pTarget->nOverflowOf50Yen);
    trace(L6, _T("*ParseSeisa() : nOverflowOf10Yen=%d"),            pTarget->nOverflowOf10Yen);
    trace(L6, _T("*ParseSeisa() : nOverflowOf5Yen=%d"),             pTarget->nOverflowOf5Yen);
    trace(L6, _T("*ParseSeisa() : nOverflowOf1Yen=%d"),             pTarget->nOverflowOf1Yen);
#endif
    // debugging...
#if (_TRACE_LOG)
    trace(L6, _T("-ParseSeisa()"));
#endif
    // exit...

    return (lResult);
}



/////////////////////////////////////////////////////////////////////////////
// Parser Table Data Definition
/////////////////////////////////////////////////////////////////////////////
//
// Command Parser Table
//
static CMDPASERTBL CmdParserTable[] =
{
    {  1, RCYPSR_CMD_STATUSREAD,    StatusReadParserTbl,  0,                    0,                  NULL                        },
    {  2, RCYPSR_CMD_SEISA,         NULL,                 sizeof(RCYPSRSEISA),  SEISA_LEN_TOTAL,    (PFNDIOPARSER)(ParseSeisa)  },
    { -1, -1,                       NULL,                -1,                    0,                  NULL                        }
};
//
// StatusRead Parser Table
//
static DATAPARSERTBL StatusReadParserTbl[] =
{
    {  1, RCYPSR_DATA_STATREAD01,   sizeof(RCYPSRSTATUS01), STATREAD01_LEN_TOTAL,   (PFNDIOPARSER)(ParseStatusRead01) },
    {  2, RCYPSR_DATA_STATREAD80,   sizeof(RCYPSRSTATUS80), STATREAD80_LEN_TOTAL,   (PFNDIOPARSER)(ParseStatusRead80) },
    { -1, -1,                       0,                      0,                      NULL                              }
};


/////////////////////// End of Program //////////////////////////////////////