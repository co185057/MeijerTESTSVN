//-----------------------------------------------------------------------------
// NOTE:  DO NOT use #pragma once.  This header needs to completely
// expand everytime it is used
//-----------------------------------------------------------------------------
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/algorithms/AlgorithmEnd.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)AlgorithmEnd.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
 *
 * Copyright 2004 by NCR Corporation,
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/algorithms/AlgorithmEnd.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
* 
* 1     10/05/04 9:45a Dm185016
* Moved to state machine from security manager since they were common
* 
* 1     10/04/04 6:25p Dm185016
* Files to control the tracing macros for algorithms.
*/


#pragma pop_macro("ITRACE_DOT_ERROR")
#pragma pop_macro("ITRACE_ERROR")
#pragma pop_macro("ITRACE_DEVELOP")
#pragma pop_macro("IMODTRACE_DEVELOP")
#pragma pop_macro("trace")
