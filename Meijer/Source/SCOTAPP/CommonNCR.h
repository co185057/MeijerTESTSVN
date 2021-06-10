//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  CommonNCR.h
//
// TITLE:   Header file to inlcude most common headers authored by NCR
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _COMMONNCR
#define _COMMONNCR

#include "ncrdevmgr.h"						  // NCR device manager
#include "SATypes.h"							  // NCR security agent
#include "SSConst.h"						// NCR Smart Scale constants
#include "SCOTPriceSound.h"				  // NCR sound, only used by DM
#include "SCOTPSApp.h"						  // NCR Presentation Services
// NewUI TK include PSX
#include "PSX API.h"
#include "DPSElements.h"					  // NCR Presentation Services elements


#include "devmgr.h"								  // NCR device manager
#include "securityagent.h"				  // NCR security agent
#include "smartscale.h"				  // NCR security agent
#include "dbmgrclient.h"				  // NCR security agent
#include "HardTotals.h"             // NCR hard totals
#include "ncrsscodigitalreceiptgen.h" //SR Personalization Digital Receipt

// Macro to tag a class as copy inhibited by implementing a private
// copy constructor and assignment operator.

#define DECLARE_COPY_INHIBIT(class_name)	\
private:									\
	class_name (const class_name &);		\
	class_name & operator = (const class_name &);
#endif
