#ifndef _SolutionTBSharedDefsh
#define _SolutionTBSharedDefsh

#include "..\AfxTransBroker\BaseTransBroker\BaseTBSharedDefs.h"

#define TBPROP_RAPDATANEEDED_CUSTENTRY	_T("DataNeededCustEntry")

#define TB_SOLUTIONSTATEID         11000   // in CustomerTBSharedDefs.h
#define TBCMD_CUSTOMERID            8000


// *** Remember - when you add a state here, add it to CustomerTBStates below.
// CustomerTBStates is used to trace all return codes.
//#define  TB_FIRST_STATE_HERE             (TBSTATE)(TB_CUSTOMERSTATEID +  1) 

#define  TB_NEED_SCANANDBAG						(TBSTATE)(TB_SOLUTIONSTATEID +  1)
#define  TB_NEEDCOUPONCHANGE                    (TBSTATE)(TB_SOLUTIONSTATEID +  2)
#define  TB_NEEDIMMEDATEAGECHECK                (TBSTATE)(TB_SOLUTIONSTATEID +  3)

#define  TB_MAXSOLUTIONSTATEID                  (TBSTATE)(TB_SOLUTIONSTATEID + 999)


#ifdef SOLUTION_TBSTATES

struct {
    long key;
    TCHAR* szname;
}SolutionTBStates[]=
{
   
    {TB_NEED_SCANANDBAG,               _T( "TB_NEED_SCANANDBAG")},
    {TB_NEEDCOUPONCHANGE,               _T( "TB_NEEDCOUPONCHANGE")},
    {TB_NEEDIMMEDATEAGECHECK,           _T( "TB_NEEDIMMEDATEAGECHECK")},

    // All states should be added above TB_MAXCUSTOMERSTATEID
    {TB_MAXSOLUTIONSTATEID,              _T("TB_MAXSOLUTIONSTATEID")}
};

#endif  // if CUSTOMER_TBSTATES

typedef enum
{
    TBCMD_POS_READY = TBCMD_BASE_TBCMD_LAST,
    TBCMD_POS_SIGNEDIN,
	TBCMD_SENDKEYEDDATA,
	TBCMD_GETDISPLAYXML,
	TBCMD_SETSELECTEDITEM,
    TBCMD_GET_POS_ERROR_NUM,
    TBCMD_ENTER_ASSISTMODE,
    TBCMD_EXIT_ASSISTMODE,
    TBCMD_ASSISTMODE_BUTTON_ALLOWED,
    TBCMD_SET_TB_STATE,
    TBCMD_GET_CUSTOMER_SELECTED_TENDER,
	TBCMD_CASHDRAWER_FAILED,
    TBCMD_HIDEINPUT,
	TBCMD_TMDISCOUNT,
	TBCMD_GMTOTAL,
	TBCMD_RESET_TMDISCOUNT_GMTOTAL,
	TBCMD_SENDSCANDATA,
    TBCMD_CHECK_FORCE_LOAD,
	TBCMD_GET_BITMAP,
    TBCMD_TENDER_IN_PROGRESS,
    TBCMD_ANY_REWARD_CARDS,
    TBCMD_SEND_ANYRESPONSE_TO_POS,
    TBCMD_IS_CUST_REQUEST_CREDIT_ENABLED,
    TBCMD_ENTERING_FINISH_AND_PAY,
    TBCMD_DUPLICATE_RECEIPT_PRNT,
	TBCMD_GET_OPTIONS,
	TBCMD_GET_AGEREQUIRED,
	TBCMD_MSGVOIDABLE,
    // Multi-bitmap & dynamic printer bitmap support
    TBCMD_MBMP_ENABLED,
    TBCMD_GET_MBMP_SYNCRAP,
    TBCMD_MBMP_GET_FLREQ,
    TBCMD_MBMP_UPD_FLREQ,
    TBCMD_MBMP_GET_RAPREQ,
    TBCMD_MBMP_UPD_RAPREQ,
    // Support for check tendering with second printer at FL (FL 4.0 Patch F+)
    TBCMD_SET_CHECK_AT_LANE_AVAILABLE,

    // Other useful command
    TBCMD_GET_TB_STATE,

    //Add new command IDs above this line
	TBCMD_SOLUTION_TBCMD_LAST
} SOLUTION_TBCMD_ID;
#endif // _solnDefsh
