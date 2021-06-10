/////////////////////////////////////////////////////////////////////
//
// OposScal.h
//
//   Scale header file for OPOS Applications.
//
// Modification history
// ------------------------------------------------------------------
// 95-12-08 OPOS Release 1.0                                     CRM
//
/////////////////////////////////////////////////////////////////////

#if !defined(OPOSSCAL_H)
#define      OPOSSCAL_H


#include "Opos.h"


/////////////////////////////////////////////////////////////////////
// "WeightUnit" Property Constants
/////////////////////////////////////////////////////////////////////

const LONG SCAL_WU_GRAM         = 1;
const LONG SCAL_WU_KILOGRAM     = 2;
const LONG SCAL_WU_OUNCE        = 3;
const LONG SCAL_WU_POUND        = 4;


/////////////////////////////////////////////////////////////////////
// "ResultCodeExtended" Property Constants for Scale
/////////////////////////////////////////////////////////////////////

const LONG OPOS_ESCAL_OVERWEIGHT= 1 + OPOSERREXT; // ReadWeight


#endif                  // !defined(OPOSSCAL_H)
