#ifndef _BaseTBSharedDefsh
#define _BaseTBSharedDefsh


#define   TBRC_MAXVALUE_INBASE  10000

#define TB_BASESTATEID           10000	//in BaseTBSharedDefs.h
#define TB_CUSTOMSTATEID         12000   // in CustomerTBSharedDefs.h

// *** Remember - when you add a state here, add it to CustomerTBStates below.
// CustomerTBStates is used to trace all return codes.
//#define  TB_FIRST_STATE_HERE             (TBSTATE)(TB_CUSTOMERSTATEID +  1) 
typedef enum TB_CUSTOMERRETURNCODES
{
TB_SETBMP               = (TB_CUSTOMSTATEID +  1),
TB_NEED_GC_AMOUNT,
TB_NEED_GC_SWIPE,
TB_NEED_PC_SWIPE ,
TB_NEEDASSISTMODE,
TB_CANCEL_ITEM,
TB_NEEDPONUMBER,
TB_UPDATE_PIM_ITEM,
TB_NEED_COUPON_INSERT,
TB_NEED_ALPHANUMERICINPUT,

TB_MAXCUSTOMSTATEID ,

};

enum SYNCSTATE
{
   SYNC_SCANANDBAG,
   SYNC_FINISH,
   SYNC_ATTRACT,
   SYNC_UNKNOWN
};

#ifdef CUSTOM_TBSTATES
struct {
    long key;
    TCHAR* szname;
}CustomTBStates[]=
{
    {TB_SETBMP,                         _T("TB_SETBMP")},
    {TB_NEED_GC_AMOUNT,                 _T("TB_NEED_GC_AMOUNT")},
    {TB_NEED_GC_SWIPE,                  _T("TB_NEED_GC_SWIPE")},
    {TB_NEED_PC_SWIPE,                  _T("TB_NEED_PC_SWIPE")},
    {TB_NEEDASSISTMODE,                 _T("TB_NEEDASSISTMODE")},
    {TB_CANCEL_ITEM,                    _T("TB_CANCEL_ITEM")},
    {TB_NEEDPONUMBER,                   _T("TB_NEEDPONUMBER")},
    {TB_UPDATE_PIM_ITEM,                _T("TB_UPDATE_PIM_ITEM")},
    {TB_NEED_COUPON_INSERT,             _T("TB_NEED_COUPON_INSERT")},
    {TB_NEED_ALPHANUMERICINPUT,         _T("TB_NEED_ALPHANUMERICINPUT")},
       
    {TB_MAXCUSTOMSTATEID,             _T("TB_MAXCUSTOMSTATEID")}
};
#endif  

typedef enum {
  TB_FL_GETNEXTSTATE = TB_BASESTATEID + 1,
  TB_FL_SETNEXTSTATE,
  TB_UNINIT_CLEANUP
} BASETBSTATEIDS;

typedef enum
{
    TBCMD_GET_TBPROPERTY = 6000,
	TBCMD_SET_TBPROPERTY,
	TBCMD_GET_HOOKPROPERTY,
	TBCMD_SET_HOOKPROPERTY,

    //Add new command IDs above this line
	TBCMD_BASE_TBCMD_LAST
} BASE_TBCMD_ID;

#endif