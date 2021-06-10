/////////////////////////////////////////////////////////////////////////////
//
// SSConst.h
//
//   This file defines Smart Scale constants needed by the application.
//
//   Copyright (c) 2001 NCR.  All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef SMARTSCALE_SSCONST_H           // Prevent multiple inclusions      
#define SMARTSCALE_SSCONST_H

/*****************************************************************************/
/* Timer Type Constants                                                      */
/*****************************************************************************/
#define SS_ALL_TIMERS        1           
#define SS_SLOW_CONCLUSION   2
#define SS_ZERO_WEIGHT       3
#define SS_SC_WATCHDOG       4
#define SS_NEW_ZERO_WEIGHT_ITEM   5

/*****************************************************************************/
/* Failure type Constants                                                    */
/*****************************************************************************/
#define SS_OPEN_FAILED			1
#define SS_CLAIM_FAILED			2
#define SS_ENABLE_FAILED		3
#define SS_COMM_FAILED			4
#define SS_FAILED_OVERWEIGHT	5
#define SS_FAILED_UNDERWEIGHT	6

#define INVALID_WEIGHT 0x08000000

#endif