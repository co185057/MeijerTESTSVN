/////////////////////////////////////////////////////////////////////
//
// OposMsr.h
//
//   Magnetic Stripe Reader header file for OPOS Applications.
//
// Modification history
// ------------------------------------------------------------------
// 95-12-08 OPOS Release 1.0                                     CRM
// 97-06-04 OPOS Release 1.2                                     CRM
//   Add ErrorReportingType values.
//
/////////////////////////////////////////////////////////////////////

#if !defined(OPOSMSR_H)
#define      OPOSMSR_H


#include "Opos.h"


/////////////////////////////////////////////////////////////////////
// "TracksToRead" Property Constants
/////////////////////////////////////////////////////////////////////

const LONG MSR_TR_1             = 1;
const LONG MSR_TR_2             = 2;
const LONG MSR_TR_3             = 4;

const LONG MSR_TR_1_2           = MSR_TR_1 | MSR_TR_2;
const LONG MSR_TR_1_3           = MSR_TR_1 | MSR_TR_3;
const LONG MSR_TR_2_3           = MSR_TR_2 | MSR_TR_3;

const LONG MSR_TR_1_2_3         = MSR_TR_1 | MSR_TR_2 | MSR_TR_3;


/////////////////////////////////////////////////////////////////////
// "ErrorReportingType" Property Constants
/////////////////////////////////////////////////////////////////////

const LONG MSR_ERT_CARD         = 0;
const LONG MSR_ERT_TRACK        = 1;


/////////////////////////////////////////////////////////////////////
// "ErrorEvent" Event: "ResultCodeExtended" Parameter Constants
/////////////////////////////////////////////////////////////////////

const LONG OPOS_EMSR_START      = 1 + OPOSERREXT;
const LONG OPOS_EMSR_END        = 2 + OPOSERREXT;
const LONG OPOS_EMSR_PARITY     = 3 + OPOSERREXT;
const LONG OPOS_EMSR_LRC        = 4 + OPOSERREXT;


#endif                  // !defined(OPOSMSR_H)
