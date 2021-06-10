/////////////////////////////////////////////////////////////////////
//
// Opos.h
//
//   General header file for OPOS Applications.
//
// Modification history
// ------------------------------------------------------------------
// 95-12-08 OPOS Release 1.0                                     CRM
// 97-06-04 OPOS Release 1.2                                     CRM
//   Add OPOS_FOREVER.
//   Add BinaryConversion values.
//
/////////////////////////////////////////////////////////////////////

#if !defined(OPOS_H)
#define      OPOS_H


/////////////////////////////////////////////////////////////////////
// OPOS "State" Property Constants
/////////////////////////////////////////////////////////////////////

const LONG OPOS_S_CLOSED        = 1;
const LONG OPOS_S_IDLE          = 2;
const LONG OPOS_S_BUSY          = 3;
const LONG OPOS_S_ERROR         = 4;


/////////////////////////////////////////////////////////////////////
// OPOS "ResultCode" Property Constants
/////////////////////////////////////////////////////////////////////

const LONG OPOSERR    = 100;
const LONG OPOSERREXT = 200;

const LONG OPOS_SUCCESS         =  0;
const LONG OPOS_E_CLOSED        =  1 + OPOSERR;
const LONG OPOS_E_CLAIMED       =  2 + OPOSERR;
const LONG OPOS_E_NOTCLAIMED    =  3 + OPOSERR;
const LONG OPOS_E_NOSERVICE     =  4 + OPOSERR;
const LONG OPOS_E_DISABLED      =  5 + OPOSERR;
const LONG OPOS_E_ILLEGAL       =  6 + OPOSERR;
const LONG OPOS_E_NOHARDWARE    =  7 + OPOSERR;
const LONG OPOS_E_OFFLINE       =  8 + OPOSERR;
const LONG OPOS_E_NOEXIST       =  9 + OPOSERR;
const LONG OPOS_E_EXISTS        = 10 + OPOSERR;
const LONG OPOS_E_FAILURE       = 11 + OPOSERR;
const LONG OPOS_E_TIMEOUT       = 12 + OPOSERR;
const LONG OPOS_E_BUSY          = 13 + OPOSERR;
const LONG OPOS_E_EXTENDED      = 14 + OPOSERR;

/////////////////////////////////////////////////////////////////////
// OPOS "OpenResult" Property Constants
/////////////////////////////////////////////////////////////////////

// The following can be set by the control object.
const LONG OPOS_OR_ALREADYOPEN  = 301;
    // Control Object already open.
const LONG OPOS_OR_REGBADNAME   = 302;
    // The registry does not contain a key for the specified
    // device name.
const LONG OPOS_OR_REGPROGID    = 303;
    // Could not read the device name key's default value, or
    // could not convert this Prog ID to a valid Class ID.
const LONG OPOS_OR_CREATE       = 304;
    // Could not create a service object instance, or
    // could not get its IDispatch interface.
const LONG OPOS_OR_BADIF        = 305;
    // The service object does not support one or more of the
    // method required by its release.
const LONG OPOS_OR_FAILEDOPEN   = 306;
    // The service object returned a failure status from its
    // open call, but doesn't have a more specific failure code.
const LONG OPOS_OR_BADVERSION   = 307;
    // The service object major version number is not 1.

// The following can be returned by the service object if it
// returns a failure status from its open call.
const LONG OPOS_ORS_NOPORT      = 401;
    // Port access required at open, but configured port
    // is invalid or inaccessible.
const LONG OPOS_ORS_NOTSUPPORTED= 402;
    // Service Object does not support the specified device.
const LONG OPOS_ORS_CONFIG      = 403;
    // Configuration information error.
const LONG OPOS_ORS_SPECIFIC    = 450;
    // Errors greater than this value are SO-specific.


/////////////////////////////////////////////////////////////////////
// OPOS "BinaryConversion" Property Constants
/////////////////////////////////////////////////////////////////////

const LONG OPOS_BC_NONE         = 0;
const LONG OPOS_BC_NIBBLE       = 1;
const LONG OPOS_BC_DECIMAL      = 2;


/////////////////////////////////////////////////////////////////////
// "CheckHealth" Method: "Level" Parameter Constants
/////////////////////////////////////////////////////////////////////

const LONG OPOS_CH_INTERNAL     = 1;
const LONG OPOS_CH_EXTERNAL     = 2;
const LONG OPOS_CH_INTERACTIVE  = 3;

/////////////////////////////////////////////////////////////////////
// OPOS "CapPowerReporting", "PowerState", "PowerNotify" Property
//   Constants
/////////////////////////////////////////////////////////////////////

const LONG OPOS_PR_NONE         = 0;
const LONG OPOS_PR_STANDARD     = 1;
const LONG OPOS_PR_ADVANCED     = 2;

const LONG OPOS_PN_DISABLED     = 0;
const LONG OPOS_PN_ENABLED      = 1;

const LONG OPOS_PS_UNKNOWN      = 2000;
const LONG OPOS_PS_ONLINE       = 2001;
const LONG OPOS_PS_OFF          = 2002;
const LONG OPOS_PS_OFFLINE      = 2003;
const LONG OPOS_PS_OFF_OFFLINE  = 2004;

/////////////////////////////////////////////////////////////////////
// "ErrorEvent" Event: "ErrorLocus" Parameter Constants
/////////////////////////////////////////////////////////////////////

const LONG OPOS_EL_OUTPUT       = 1;
const LONG OPOS_EL_INPUT        = 2;
const LONG OPOS_EL_INPUT_DATA   = 3;


/////////////////////////////////////////////////////////////////////
// "ErrorEvent" Event: "ErrorResponse" Constants
/////////////////////////////////////////////////////////////////////

const LONG OPOS_ER_RETRY        = 11;
const LONG OPOS_ER_CLEAR        = 12;
const LONG OPOS_ER_CONTINUEINPUT= 13;


/////////////////////////////////////////////////////////////////////
// "StatusUpdateEvent" Event: Common "Status" Constants
/////////////////////////////////////////////////////////////////////

const LONG OPOS_SUE_POWER_ONLINE      = 2001;
const LONG OPOS_SUE_POWER_OFF         = 2002;
const LONG OPOS_SUE_POWER_OFFLINE     = 2003;
const LONG OPOS_SUE_POWER_OFF_OFFLINE = 2004;

/////////////////////////////////////////////////////////////////////
// General Constants
/////////////////////////////////////////////////////////////////////

const LONG OPOS_FOREVER         = -1;


#endif                  // !defined(OPOS_H)
