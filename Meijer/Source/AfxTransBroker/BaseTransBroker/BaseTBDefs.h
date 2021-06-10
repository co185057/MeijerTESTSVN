///////////////////////////////////////////////////////////////////////////
///
/// \file BaseTBDefs.h
/// \brief declaration  file Base Information for Integration Base
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////////

#ifndef _BaseTBDefsh
#define _BaseTBDefsh
#include "transbroker.h"

/// \def MAX_KEYSEQUENCE
///	\brief Default to 40
#define MAX_KEYSEQUENCE     40


/// \def TB_BASESTATEID
/// \brief Custom TB Message Screen IDs used by SSF State SMCustomMessageByID
///		These definitions are used by SMCustomMessageByID.cpp to selectively
///		fill in text and wave file values for custom pop-up message screens.
///		in BaseTBSharedDefs.h
#define TB_BASESTATEID             10000 
//#define TB_SOLUTIONSTATEID         11000 // in SolutionTBSharedDefs.h
//#define TB_CUSTOMERSTATEID         12000   // in CustomerTBSharedDefs.h

/// \def TB_MAXBASESTATEID
///	\brief add new states above this one
#define TB_MAXBASESTATEID             (TBSTATE)(TB_BASESTATEID +  999) 

/// \def TB_MAXTBRC
/// \brief no real value - just need a defined max
#define TB_MAXTBRC     (TBRC) -999  

/// \def TB_MAXCORETBSTATEID
/// \brief no real value - just need a defined max
#define TB_MAXCORETBSTATEID (TBSTATE) -999 



#ifdef BASE_TRX_BROKER

/// \struct CoreTBStates
/// \brief mapping from TBState
struct {
   TBSTATE key;
   TCHAR* szname;
}CoreTBStates[]=
{
   {TB_CLOSED,                          _T("TB_CLOSED")}, ///< TB_CLOSED 
   {TB_CMOS_DOSCLOSE,                   _T("TB_CMOS_DOSCLOSE")}, ///< TB_CMOS_DOSCLOSE
   {TB_CMOS_DOSOPEN,                    _T("TB_CMOS_DOSOPEN")},
   {TB_CMOS_DOSREAD,                    _T("TB_CMOS_DOSREAD")},
   {TB_CMOS_DOSSEEK,                    _T("TB_CMOS_DOSSEEK")},
   {TB_CMOS_DOSWRITE,                   _T("TB_CMOS_DOSWRITE")},
   {TB_CMOS_PURGE,                      _T("TB_CMOS_PURGE")},
   {TB_COMPLETEPRINT,                   _T("TB_COMPLETEPRINT")},
   {TB_COUPONEXCEEDED,                  _T("TB_COUPONEXCEEDED")},
   {TB_COUPONNOTMATCH,                  _T("TB_COUPONNOTMATCH")},
   {TB_IGNORE,                          _T("TB_IGNORE")},
   {TB_INVALIDITEMVOID,                 _T("TB_INVALIDITEMVOID")},
   {TB_ITEMQTYEXCEEDED,                 _T("TB_ITEMQTYEXCEEDED")},
   {TB_ITEMRECALLED,                    _T("TB_ITEMRECALLED")},
   {TB_ITEMSOLD,                        _T("TB_ITEMSOLD")},
   {TB_ITEMUNKNOWN,                     _T("TB_ITEMUNKNOWN")},
   {TB_LOYALTYCARD,                     _T("TB_LOYALTYCARD")},
   {TB_LOYALTYINVALID,                  _T("TB_LOYALTYINVALID")},
   {TB_NEEDOVERRIDE,                    _T("TB_NEEDOVERRIDE")},
   {TB_NEEDPRICE,                       _T("TB_NEEDPRICE")},
   {TB_NEEDQUANTITY,                    _T("TB_NEEDQUANTITY")},
   {TB_NEEDTARE,                        _T("TB_NEEDTARE")},
   {TB_NEEDWEIGHT,                      _T("TB_NEEDWEIGHT")},
   {TB_OPTIONLOADING,                   _T("TB_OPTIONLOADING")},
   {TB_PRINTIMMEDIATE,                  _T("TB_PRINTIMMEDIATE")},
   {TB_PRINTIMMEDIATECUT,               _T("TB_PRINTIMMEDIATECUT")},
   {TB_READY,                           _T("TB_READY")},
   {TB_SECURED,                         _T("TB_SECURED")},
   {TB_TENDERACCEPTED,                  _T("TB_TENDERACCEPTED")},
   {TB_TENDERDECLINED,                  _T("TB_TENDERDECLINED")},
   {TB_TOTAL,                           _T("TB_TOTAL")},
   {TB_TRXEND,                          _T("TB_TRXEND")},
   {TB_TRXLIMITEXCEEDED,                _T("TB_TRXLIMITEXCEEDED")},
   {TB_TRXVOIDED,                       _T("TB_TRXVOIDED")},
   {TB_TRXVOIDEXCEEDED,                 _T("TB_TRXVOIDEXCEEDED")},
   {TB_UNKNOWN,                         _T("TB_UNKNOWN")},
   {TB_VOIDLIMITEXCEEDED,               _T("TB_VOIDLIMITEXCEEDED")},
   {TB_VOIDNOTMATCH,                    _T("TB_VOIDNOTMATCH")},
   {TB_VOIDITEMBEFORECOUPON,            _T("TB_VOIDITEMBEFORECOUPON")},
   {TB_TRXSUSPENDED,                    _T("TB_TRXSUSPENDED")},
   {TB_TRXSUSPENSIONUNCOMPLETED,        _T("TB_TRXSUSPENSIONUNCOMPLETED")},
   {TB_TENDERDECLINEDKEEPCARD,          _T("TB_TENDERDECLINEDKEEPCARD")},
   {TB_CASHWAUTHORIZED,                 _T("TB_CASHWAUTHORIZED")},
   {TB_CASHWNOTAUTHORIZED,              _T("TB_CASHWNOTAUTHORIZED")},
   {TB_CASHWCONFIRMED,                  _T("TB_CASHWCONFIRMED")},
   {TB_CASHWNOTCONFIRMED,               _T("TB_CASHWNOTCONFIRMED")},
   {TB_CASHWCANCELLED,                  _T("TB_CASHWCANCELLED")},
   {TB_INVALIDPIN,                      _T("TB_INVALIDPIN")},
   {TB_EPSOFFLINE,                      _T("TB_EPSOFFLINE")},
   {TB_EPSSODOK,                        _T("TB_EPSSODOK")},
   {TB_EPSSODNOK,                       _T("TB_EPSSODNOK")},
   {TB_EPSEODOK,                        _T("TB_EPSEODOK")},
   {TB_EPSEODNOK,                       _T("TB_EPSEODNOK")},
   {TB_TENDERFOODSTAMPINSUFFICIENT,     _T("TB_TENDERFOODSTAMPINSUFFICIENT")},
   {TB_FOODSTAMPELIGIBLETOTAL,          _T("TB_FOODSTAMPELIGIBLETOTAL")},
   {TB_ITEM_QUANTITY_RESTRICTED,        _T("TB_ITEM_QUANTITY_RESTRICTED")},
   {TB_VISUAL_VERIFY,                   _T("TB_VISUAL_VERIFY")},
   {TB_TIMERESTRICTEDITEM,              _T("TB_TIMERESTRICTEDITEM")},
   {TB_TENDERCASHBENEFITINSUFFICIENT,   _T("TB_TENDERCASHBENEFITINSUFFICIENT")},
   {TB_ITEMSALECOMPLETED,               _T("TB_ITEMSALECOMPLETED")},
   {TB_INVALIDAMOUNT,                   _T("TB_INVALIDAMOUNT")},
   {TB_EPSONLINE,                       _T("TB_EPSONLINE")},
   {TB_NEWENCRYPTIONKEY,                _T("TB_NEWENCRYPTIONKEY")},
   {TB_INTTRXREPLY,                     _T("TB_INTTRXREPLY")},
   {TB_ATMPOWERUP,                      _T("TB_ATMPOWERUP")},
   {TB_ATMOFFLINE,                      _T("TB_ATMOFFLINE")},
   {TB_ATMONLINE,                       _T("TB_ATMONLINE")},
   {TB_ATMEJPRINTCOMPLETE,              _T("TB_ATMEJPRINTCOMPLETE")},
   {TB_TENDEREBTINSUFFICIENT,           _T("TB_TENDEREBTINSUFFICIENT")},
   {TB_MESSAGESCREEN,                   _T("TB_MESSAGESCREEN")},
   {TB_ENTEREDTRAINING,                 _T("TB_ENTEREDTRAINING")},
   {TB_LEFTTRAINING,                    _T("TB_LEFTTRAINING")},
   {TB_TENDERCANCELLED,                 _T("TB_TENDERCANCELLED")},
   {TB_CUSTOMERMESSAGE,                 _T("TB_CUSTOMERMESSAGE")},
   {TB_REWARDLINE,                      _T("TB_REWARDLINE")},
   {TB_CRATEABLEITEM,                   _T("TB_CRATEABLEITEM")},
   {TB_NEEDWEIGHT_BIZERBA,              _T("TB_NEEDWEIGHT_BIZERBA")},
   {TB_SENDMESSAGETOSCOT,               _T("TB_SENDMESSAGETOSCOT")},
   {TB_TRXEND_PRINTANDDISPENSE,         _T("TB_TRXEND_PRINTANDDISPENSE")},
   {TB_CLOSED_NOPRINTANDNODISPENSE,     _T("TB_CLOSED_NOPRINTANDNODISPENSE")},
   {TB_NEEDMOREDATA,                    _T("TB_NEEDMOREDATA")},
   {TB_NEEDMICR,                        _T("TB_NEEDMICR")},
   {TB_ENDORSEDOCUMENT,                 _T("TB_ENDORSEDOCUMENT")},
   {TB_NEWCASHDRAWERSTATE,              _T("TB_NEWCASHDRAWERSTATE")},
   {TB_NEWSECURITYLEVEL,                _T("TB_NEWSECURITYLEVEL")},
   {TB_DOCUMENTNUM,                     _T("TB_DOCUMENTNUM")},
   {TB_COMPLETEPRINTANDCUT,             _T("TB_COMPLETEPRINTANDCUT")},
   {TB_ASSISTMODESTATUSCHANGED,         _T("TB_ASSISTMODESTATUSCHANGED")},
   {TB_PRINTER_VERIFY,                  _T("TB_PRINTER_VERIFY")},
   {TB_CREATE_REPORT_ENTRY,		        _T("TB_CREATE_REPORT_ENTRY")},
   {TB_ASSISTMODETABFLUSH,		        _T("TB_ASSISTMODETABFLUSH")},
   {TB_GETBUSINESSDATE,                 _T("TB_GETBUSINESSDATE")},
   {TB_HOSTOFFLINE,                     _T("TB_HOSTOFFLINE")},
   {TB_HOSTONLINE,                      _T("TB_HOSTONLINE")},
   {TB_CHANGESTATE,                     _T("TB_CHANGESTATE")},
   {TB_OUTOFSYNC_STOP,                  _T("TB_OUTOFSYNC_STOP")},
   {TB_OUTOFSYNC_ASSISTMODE,            _T("TB_OUTOFSYNC_ASSISTMODE")},
   {TB_TENDERVOIDED,	                _T("TB_TENDERVOIDED")},
   {TB_LANECLOSED,	                    _T("TB_LANECLOSED")},
   {TB_LANEOPEN,	                    _T("TB_LANEOPEN")},
   {TB_REPRINTRECEIPT,	                _T("TB_REPRINTRECEIPT")},
   {TB_REPRINTRECEIPTFAILED,	        _T("TB_REPRINTRECEIPTFAILED")},
   {TB_SENDEODCASHREPORT,				_T("TB_SENDEODCASHREPORT")},
   {TB_MAXCORETBSTATEID,                _T("TB_MAXCORETBSTATEID")},
};

/// \struct CoreTBRC
/// \brief Mapping to TBRC
struct {
   TBRC key;
   TCHAR* szname;
}CoreTBRCs[]=
{
   {TB_SUCCESS_IMMEDIATE,           _T("TB_SUCCESS_IMMEDIATE")},
   {TB_SUCCESS,                     _T("TB_SUCCESS")},
   {TB_E_INVALIDUSERID,             _T("TB_E_INVALIDUSERID")},
   {TB_E_INVALIDUSERPASSWORD,       _T("TB_E_INVALIDUSERPASSWORD")},
   {TB_E_INVALIDSTATE,              _T("TB_E_INVALIDSTATE")},
   {TB_E_FILENOTFOUND,              _T("TB_E_FILENOTFOUND")},
   {TB_E_INCOMPLETEPROCEDURE,       _T("TB_E_INCOMPLETEPROCEDURE")},
   {TB_E_INVALIDWINDOW,             _T("TB_E_INVALIDWINDOW")},
   {TB_E_DISKFULL,                  _T("TB_E_DISKFULL")},
   {TB_E_INVALIDPARAM,              _T("TB_E_INVALIDPARAM")},
   {TB_E_NOHOSTCREDIT,              _T("TB_E_NOHOSTCREDIT")},
   {TB_E_NOWORKINGKEY,              _T("TB_E_NOWORKINGKEY")},
   {TB_E_FUNCTIONNOTSUPPORTED,      _T("TB_E_FUNCTIONNOTSUPPORTED")},
   {TB_E_ITEMFAILED,                _T("TB_E_ITEMFAILED")},
   {TB_E_CARDDATA_MISSING,          _T("TB_E_CARDDATA_MISSING")},
   {TB_E_NOTINITIALIZED,            _T("TB_E_NOTINITIALIZED")},
   {TB_E_NOHOST,                    _T("TB_E_NOHOST")},
   {TB_E_UNEXPECTED,                _T("TB_E_UNEXPECTED")},

   // add new TBRC's above this one
   {TB_MAXTBRC,                     _T("TB_MAXTBRC")},
};
#endif

#endif


