//**********************************************************************************
//	TBDefs.h   :   declaration file for transaction broker structures and # defines
//                                                                                
//	                                                                              
//	$Header:
//
//	CHANGES: Start                                                                
//	CHANGES: End                                                                  
//                                                                                
//                                                                                
//***********************************************************************************


#ifndef TBDEFS_H
#define TBDEFS_H



#include "CmClass.h"
#include "NCREventLog.h"
#include "ScotEventCat.h"
#include "ScotMsg.h"
#include "TransBroker.h"
#include "OPOSScan.h"


#define TB_EVENTLOG_SOURCE	_T("SCOTTB")
#define DCAP_PREFIX			_T("TB")





//	Transaction Broker return code ranges :
//	-------------------------------------
//	Positive	Successful or warning errors
//	-1 to -99	Operational errors
//	-100 to -199	Programming errors - should be fixed in the application code
//	-666 and below	Unexpected errors
#define    TB_SUCCESS					0		// Successful
#define    TB_E_INVALIDUSERID			-2		// Invalid user ID
#define    TB_E_INVALIDPASSWORD			-3		// Invalid user password
#define    TB_E_INVALIDSTATE			-4		// Invalid procedure to perform at this time
#define    TB_E_FILENOTFOUND			-5		// File not found
#define    TB_E_INCOMPLETEPROCEDURE	-6		// Procedure not complete
#define    TB_E_INVALIDWINDOW			-7		// Invalid window handle passed
#define    TB_E_DISKFULL					-8		// Disk is full
#define    TB_E_INVALIDPARAM			-9		// Invalid parameter(s).
#define    TB_E_NOTINITIALIZED			-100	// TB has not been successfully initialized
#define    TB_E_NOHOST					-101	// Error in communication with the host application.  This may be due to the mismatch TB and host application.  The application should terminate.
#define    TB_E_UNEXPECTED				-666	// Unexpected fatal error.  The application should terminate.

#define EMPTY_STRING _T("")

#ifdef _STOREMINDER_
extern CNCREventLog*	   pgEventLog;	// eventlog object to log events in NT eventlog
#else
extern CObject*         pgEventLog;
#endif


#endif // TBDEFS_H

