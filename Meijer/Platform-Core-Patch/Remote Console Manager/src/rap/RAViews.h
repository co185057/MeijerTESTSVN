#ifndef _RAVIEWS_INCLUDED
#define _RAVIEWS_INCLUDED

#define DATANEEDEDVIEW int

const enum DATANEEDEDVIEWS
   {
   DATANEEDED_NOVIEW = -1, //tar 252987
   DATANEEDED_APPROVEVOIDEDITEM,       // 0 - Voided Item Approval view
   DATANEEDED_CONFIRMSUSPEND,          // 1 - Confirm Suspend view
   DATANEEDED_MISMATCHWEIGHT,
   DATANEEDED_UNEXPECTEDINCREASE,
   DATANEEDED_DONTBAGTHRESHOLDEXCEEDED,
   DATANEEDED_NEWITEMENTRY,
   DATANEEDED_BAGITEMTIMEREXPIRED,
   DATANEEDED_DONEREMOVINGTHRESHOLDEXCEEDED,
   DATANEEDED_WALKAWAYTIMEREXPIRED,
   DATANEEDED_PRICENOTMATCH,
   DATANEEDED_COUPONTHRESHOLDEXCEEDED,
   DATANEEDED_COUPONDOLLARMAX,
   DATANEEDED_PRICEPROMPT,
   DATANEEDED_MOREDATA,
   DATANEEDED_BAGBOXTHRESHOLDEXCEEDED,
   DATANEEDED_UNEXPECTEDDECREASE,
   DATANEEDED_DEVICEERROR,
   DATANEEDED_QUANTITY ,  // Updates Item Object's Quantity
   DATANEEDED_ENTERITEM,         // Requests an item QTY/SKU and enters item
   DATANEEDED_ENTERCOUPON,       // Requests a coupon QTY/AMOUNT and updates item
   DATANEEDED_MEASUREMENT,       // Request a measument of "Description" and updates ???
   DATANEEDED_ASSEMBLYFEE,
   DATANEEDED_VERIFYIDENTITY,    // verify the customer name for BIC card
	DATANEEDED_AUTHORIZATIONNUM,  // enter authorization number for credit cards
   DATANEEDED_CSASUSPEND,        // Request suspend confirmation for services
   DATANEEDED_DLIDNUMBER,        // enter driver's license or id number
	DATANEEDED_CANCELLTRANSACTION,// cancel transaction
   DATANEEDED_ENTERACCOUNTNUMBER,// Manual Credit Card Number Entry
   DATANEEDED_ENTEREXPDATE,      // Manual Credit Card Date entry.
	DATANEEDED_PRICEPRMT,         // price prompt
   DATANEEDED_ASSISTEDTENDER,    // Tender requiring RAP assistance.
   DATANEEDED_CLOSELANE,         // awk.2.6.2 TAR192509
   DATANEEDED_ENDORSEDOCUMENT,   // Endorsement for a document is needed by TB
   DATANEEDED_DOCUMENTNUM,       // Document number (View Name: "DocumentNum")
   DATANEEDED_BAGAREABACKUP,
   DATANEEDED_UNDEACTIVATEDITEM, // Undeactivated item on TABelt detected
   DATANEEDED_VERIFYSIGNATURE,   // awk.4.11.2 TAR199004
   DATANEEDED_ASSISTEDITEM,       // RFC 205489 RJF 6/11/02
   DATANEEDED_GIFTCARDLIMIT,	// RFC 211574, approval for gift card purchase
   DATANEEDED_TAKECHANGETIMEREXPIRED,        // Take Change Timer Expired
   DATANEEDED_STARTTRANSACTION,  // RFC 228102
   DATANEEDED_THANKYOUEXPIRED,   //RFC 209702 ThankYou Security Timer expired, Escalate to RAP
   DATANEEDED_INSERTCOUPONTIMEREXPIRED,        // TAR 219178 - Escalate Insert Coupon
   DATANEEDED_COLLECTGIFTCARD,	 //Collect Gift Card view
   DATANEEDED_HOSTOFFLINE,		 //Host Offline view
   DATANEEDED_WLDBFAILURE,		 //WLDB failed
   DATANEEDED_STARTTRANSACTIONAM,
   DATANEEDED_SAFETYCOVEROPEN,
   DATANEEDED_MULTIPACKITEM,         //TAR 256154
   DATANEEDED_OUTOFSYNC_STOP,
   DATANEEDED_OUTOFSYNC_ASSIST,
   DATANEEDED_SUSPENSION_DECLINE,	// TAR 352409  view no=51
   DATANEEDED_STARTTRAININGTRANS,  //view no=52
   DATANEEDED_BAGBOXNOTVALID, //SR085
   DATANEEDED_MAXCOUPONLIMIT,				//SR678
   DATANEEDED_MAXCOUPONLIMITFINALIZATION,	//SR678
   DATANEEDED_CHECKONLANE,	 //RFC 356020
   DATANEEDED_CHECKONLANERAP, //RFC 356020
   DATANEEDED_LASTVIEW =10000                // Make sure this is the last view in the list.
   };

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
#endif
