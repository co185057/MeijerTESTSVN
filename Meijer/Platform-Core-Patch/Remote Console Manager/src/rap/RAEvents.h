//
// SCOTRAEventInfo.h
//
// This file defines the Categories and Priorities of SCOTRA Events
// It also defines the Approval codes and states, and the custom
// HRESULTS that SCOTRA.DLL can return
//
// It should be used by the Remote Approval Program (or any other 
// client of SCOTRA)
//

/*lint -e528 */

#ifndef _SCOTRAEVENTINFO_H
#define _SCOTRAEVENTINFO_H

// Priorites

#define RA_PRIORITY_DEFER      0x00000001  // This will defer until EOT
#define RA_PRIORITY_URGENT     0x00000002  // This is holding up the customer


// Categories

#define RA_CAT_BITMAP(x)          (1 << x)

static int RA_CAT_UNBITMAP(unsigned int num)
{
  unsigned int index = 0;

  while (num > 1)
  {
    num = num >> 1;
    index++;
  }
  return index;
}


#define RA_STATE_INACTIVE			0
#define RA_STATE_ITEMIZATION		1
#define RA_STATE_TENDER				2
#define RA_STATE_WAIT_FOR_APPROVAL	3
#define RA_STATE_DATA_NEEDED        4
#define RA_STATE_BAG_ITEM			5
#define RA_STATE_PICKING_UP_ITEMS   6
#define RA_STATE_SECURITY_VIOLATION 7

//Tar 213249
//The following defines will be removed in a future release because the request to enable or disable assist mode
//could also occur while in these other states, such as Itemization.  This implementation has
//been redone to process these requests in the same way the AssistModeToggleToMain is handled.

#define RA_STATE_ASSISTOK           8 //awk.4.2.2 App says we can play with Assist mode.
#define RA_STATE_ASSISTNOTOK        9 //awk.4.2.2 App says we can't.
//Tar 213249

#define RA_CAT_OTHER           0                   // Catch-all "other" event.
#define RA_CAT_UNKNOWN         RA_CAT_BITMAP(0)    // 0x00000001
#define RA_CAT_RECEIPT         RA_CAT_BITMAP(1)    // 0x00000002
#define RA_CAT_DEVERR          RA_CAT_BITMAP(2)    // 0x00000004
#define RA_CAT_SECURITY        RA_CAT_BITMAP(3)    // 0x00000008
#define RA_CAT_VOID            RA_CAT_BITMAP(4)    // 0x00000010
#define RA_CAT_COLLECTION      RA_CAT_BITMAP(5)    // 0x00000020
#define RA_CAT_AGERESTRICT     RA_CAT_BITMAP(6)    // 0x00000040
#define RA_CAT_EFT             RA_CAT_BITMAP(7)    // 0x00000080
#define RA_CAT_AUDACTION       RA_CAT_BITMAP(8)    // 0x00000100
#define RA_CAT_OUTOFSERVICE    RA_CAT_BITMAP(9)    // 0x00000200
#define RA_CAT_MEDIASHORTAGE   RA_CAT_BITMAP(10)   // 0x00000400
#define RA_CAT_PRICENEEDED     RA_CAT_BITMAP(11)   // 0x00000800
#define RA_CAT_UNKNOWNITEM     RA_CAT_BITMAP(12)   // 0x00001000
#define RA_CAT_RECALLEDITEM    RA_CAT_BITMAP(13)   // 0x00002000
#define RA_CAT_TIMERESTRICT    RA_CAT_BITMAP(14)   // 0x00004000
#define RA_CAT_SUSPEND         RA_CAT_BITMAP(15)   // 0x00008000
#define RA_CAT_CALLFORHELP     RA_CAT_BITMAP(16)   // 0x00010000
#define RA_CAT_NEGATIVEDEPT    RA_CAT_BITMAP(17)   // 0x00020000
#define RA_CAT_VISUALVERIFY    RA_CAT_BITMAP(18)   // 0x00040000
#define RA_CAT_SIGCAP          RA_CAT_BITMAP(19)   // 0x00080000
#define RA_CAT_OTHERPAYMENT    RA_CAT_BITMAP(20)   // 0x00100000
#define RA_CAT_QTYRESTRICT     RA_CAT_BITMAP(21)   // 0x00200000
#define RA_CAT_DATANEEDED      RA_CAT_BITMAP(22)   // 0x00400000
#define RA_CAT_EASITEMACTIVE   RA_CAT_BITMAP(23)   // 0x00800000
#define RA_CAT_STALE           RA_CAT_BITMAP(24)   // 0x01000000
#define RA_CAT_RESTRICTEDNOTALLOWED    RA_CAT_BITMAP(25)   // 0x02000000
#define RA_CAT_HOPPERSUBSTITUTION    RA_CAT_BITMAP(26)   // 0x04000000
#define RA_CAT_SUSPENSIONDECLINED	RA_CAT_BITMAP(27)	// 0x08000000	// TAR 352409
#define RA_CAT_COUPON          RA_CAT_BITMAP(28)    // TAR400905 

#define RA_CAT_ALL            0xFFFFFFFF

// Make sure the following symbol is ALWAYS #defined to be the LAST category
// #define RA_CAT_MAX            25 // TAR 352409 Commented
#define RA_CAT_MAX            28	// TAR 352409
#define RA_NUM_CAT            (RA_CAT_MAX + 1)


// SCOTRA Approval Codes : PENDING should ALWAYS be 0 (zero) to allow for 
// testing if (approvalCode) to mean if an approval code has been set.
// To test if an approval has been "approved" used if (RA_APPROVED(code))

#define RA_APPROVED(rc) ((BOOL) rc > RA_CODE_PENDING)


// No action has occurred yet
#define RA_CODE_PENDING         0  

// "Approved" means different things depending on what type
// of approval it is.
#define RA_CODE_APPROVED        1  

// This means we want to suspend the transaction at the next
// legal place.  This is analagous to NOT approving something.
#define RA_CODE_SUSPEND         2  

// This means we "approve" it such that we ask them to
// come by the RAP station and drop something off or get
// something checked AFTER they have finished the trans.
#define RA_CODE_VERIFY_APPROVED 3  

// This means the SCOT station denied the approval, most
// likely because the application was in store mode when 
// the approval request came in.
#define RA_CODE_DENIED         -1  

// This means the SCOT station wants the action re-done.
// Should be used for asking for another signature.
#define RA_CODE_REDO           -2  

// This means the operator passed in RequestApprove didn't have
// sufficient privilege for this approval.                                   
#define RA_CODE_NOPRIVILEGE    -3


// SCOTRA Custom HRESULT's

#define FACILITY_SCOTRA            FACILITY_ITF
#define SCOTRA_ERR_BASE            0x200

#define MAKE_RA_HRESULT(code)      MAKE_HRESULT(SEVERITY_ERROR,  \
                                                FACILITY_SCOTRA, \
                                                SCOTRA_ERR_BASE + code)

#define RA_APPROVED_ALREADY        MAKE_RA_HRESULT(1)
#define RA_IN_STORE_MODE           MAKE_RA_HRESULT(2)
#define RA_TRANSACTION_NOT_ACTIVE  MAKE_RA_HRESULT(3)
#define RA_NO_SUCH_OBJECT          MAKE_RA_HRESULT(4)
#define RA_NOT_REMOTELY_APPROVABLE MAKE_RA_HRESULT(5)
#define RA_SVR_NOT_INITIALIZED     MAKE_RA_HRESULT(6)

#define RAP_RECEIPT_LINE_HIGHLIGHT          _T("[highlightX]")
#define RAP_RECEIPT_LINE_HIGHLIGHT_SUBITEM  _T("[highlightX][subitem]")
#define RAP_RECEIPT_LINE_SUMMARY            _T("[SummaryX]")
#define RAP_RECEIPT_LINE_ALERT              _T("[Alert]")

// DO NOT USE these for new code.  Only included for compatibility with SSFs.
#define RAP_RECEIPT_LINE_HIGHLIGHT_OLD   _T("[highlight]")
#define RAP_RECEIPT_LINE_SUMMARY_OLD     _T("[Summary]")

#endif