#ifndef _SECMGRPROCEDURES_CONSTANTS_H
#define _SECMGRPROCEDURES_CONSTANTS_H

// API for SaySecurity cases
#define SEC_NONE 0								  // case 0:No signal
#define SEC_LOW  1								  // case 1:S_LOW
#define SEC_MED  2								  // case 2:S_MED
#define SEC_HIGH 3								  // case 3:S_HIGH

// API for SaySignal cases
#define SIG_LOW  1 //case 1: S_LOW
#define SIG_MED  3								  //case 2: case 3: S_MED
#define SIG_HIGH 6								  //case 4:case 5:case 6: S_HIGH
#define SIGSEC_LOW  100 //case SECURITY_LOW:S_LOW
#define SIGSEC_MED  101 //case SECURITY_MED:S_MED
#define SIGSEC_HIGH 102 //case SECURITY_HIGH:S_HIGH

#define NO_TONE (0x1000);

#define MAX_SECLEVEL  4

#endif // _SECMGRPROCEDURES_CONSTANTS_H
