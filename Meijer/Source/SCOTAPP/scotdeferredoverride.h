/* ======================================================================= */
/* =              Copyright (c) NCR Corporation 1989-1999                = */
/* ======================================================================= */
/* scotdeferredoverride.h 11.3 */
/* ======================================================================= */ 
/* =              Copyright (c) NCR Corporation 1989-1996                = */ 
/* ======================================================================= */ 

/*********************************************
 * File:        SCOTDEFERREDOVERRIDE.H
 *
 * Purpose:     SCOT Delayed Override module.
 *
 * Change History:
 *
 *
 *      $Log: /Fastlane/4.0 Patch B/Core/Source/SCOTAPP/scotdeferredoverride.h $
 * 
 * 1     1/26/05 2:08p Bd123405
 * Initial check-in of 4.0 Patch B
* 
* 11    3/14/03 6:39p Td150001
* unicode conversion
* 
* 10    4/17/02 7:31p Am200037
* added new datamember to struct for EAS item active approval
* 
* 8     8/20/01 10:16a Td150001
* Added new flag for Call for help Approval from Rap
* 
* 5     11/23/99 3:46p Bp150003
* TAR#126778 Quantity restricted items need approval
* 
* 4     11/02/99 3:59p Yw100000
* Change Other Payment approval logic
* 
* 3     8/24/99 2:17p Zhangf
* 
* 2     4/28/99 1:37p Xiey
* Initital changes to support RAP
 * 
 * Comment at 990122 12:10:28  by scot-tburk  
 * FEATURE#: 96865 
 * Add RAP functionality to SCOT 1.0 
 * Begin debugging Scotdo.h changes. 
 * 
 * Comment at 990115 09:21:33  by scot-tburk  
 * FEATURE#: 96865 
 * Add RAP functionality to SCOT 1.0 
 * Initial Scotdo.h changes for initial build. Debug next.  
 * 
 * Comment at 990108 11:24:48  by scot-tburk  
 * FEATURE#: 96865 
 * Add RAP functionality to SCOT 1.0 
 *  
 * Revision 10.1 of this file was copied from archive 
 * scot10 to scot11, and the revision numbering changed 
 * to reflect this.  NO CODE CHANGES WERE MADE 
 * 
 * Comment at 981022 09:39:56  by mjr  
 * FEATURE#: 91536 
 * Add application files from SCOT Source Safe to codegate.  
* 
* 17    3/24/98 10:02a Barnhs
* include weirdness
* 
* 16    3/23/98 3:10p Burkt
* TAR #73969 
* Application does not correctly handle nested error/override conditions.
* This is not good, but it appears to be better. 
* 
* 15    3/11/98 2:13p Purchm
* Add error level to Delayoverride object.  Need to gray out CANCEL
* button in scot error screen for store mode when the error level is 6.
* 
* 14    2/18/98 5:51p Purchm
* Add AddOnlyErrorText function to add the text to the delayoverride
* object, but not to bump up the number for errors.
* 
* 13    10/10/97 4:14p Garrac
* More approval stuff.
* 
* 12    10/08/97 6:17p Garrac
* Lots o changes.
* 
* 11    10/08/97 3:12p Garrac
* Added void ratio stuff.
* 
* 10    9/24/97 8:41p Garrac
* Added security approval functions.
* 
* 9     9/22/97 10:25p Garrac
* More approval stuff.
* 
* 8     9/19/97 5:00p Garrac
* More approval changes.
* 
* 7     9/18/97 12:42p Garrac
* Added security event approval code.
* 
* 6     9/15/97 5:25p Garrac
* Update for approval functions in SCOTDO.H
* 
* 5     9/12/97 11:49a Garrac
* Added approval code.
* 
* 4     9/11/97 6:44p Garrac
* Updates for approval.
 */


#ifndef _SCOTDEFERREDOVERRIDE_H
#define _SCOTDEFERREDOVERRIDE_H

typedef struct 
   {
   long  nItemCount;
   long  nApprovedCount;
   long  nUnapprovedCount;
   int   bApprovalRequired;
   int   bDropoffRequired;
   int   bSuspendRequested;
   } SCOTDO_INFO;


typedef struct 
   {
   int bAnyApprovalRequired;
   int bVoidApprovalRequired;
   int bCouponApprovalRequired;
   int bAgeApprovalRequired;
   int bVisualVerifyApprovalRequired;
   int bCallForHelpApprovalRequired; /*Flag for Call For Help approval from Rap*/
   int bOtherPaymentApprovalRequired;
   int bSecurityApprovalRequired;
   int bSigCapApprovalRequired;
   int bSigCapRedoRequired;         /*  SigCap must be re-done (signature) */
   int bUnknownItem;
   int bRecalledItem;
   int bTimeRestrictedItem;
   int bKeyedUnknown;
   int bPriceUnknown;
   int bQuantityRestrictedItem;
   int bApplicationError;
   LPCTSTR sApplicationErrorText;
   int nApplicationEventId;
   int bDeviceError;
   LPCTSTR sDeviceErrorText;
   int nDeviceClass;
   int nDeviceId;

   int bSuspendApproved;
   int bSuspendApprovalRequired;
   int bSuspendRequested;
   int bDropoffRequired;            /* Something must be dropped off    */
   int bDropoffApproved;            /* Go make the drop, RAP is ready   */
   int bDropoffApprovalRequired;    /* RAP not ready                    */
   int bUnDeActivatedItemApprovalRequired;
   } SCOTDO_APPROVAL_INFO;


#endif
