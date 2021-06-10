//-----------------------------------------------------------------------------
// NOTE:  DO NOT use #pragma once.  This header needs to completely
// expand everytime it is used
//-----------------------------------------------------------------------------
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/algorithms/AlgorithmBegin.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)AlgorithmBegin.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/algorithms/AlgorithmBegin.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     3/22/05 4:07p Dm185016
 * UNICODE
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


#pragma push_macro("trace")
#undef trace
#define trace pAction->GetTraceObject().write

#pragma push_macro("IMODTRACE_DEVELOP")
#undef IMODTRACE_DEVELOP
#define IMODTRACE_DEVELOP() \
    MethodTrace trc(pAction->GetTraceObject(), _T(__FUNCTION__),  __LINE__ )

#pragma push_macro("ITRACE_DEVELOP")
#undef ITRACE_DEVELOP
#define ITRACE_DEVELOP(p1) \
   	if (pAction->GetTraceObject().debugInProgress) pAction->GetTraceObject().write((p1), ITRACE_LEVEL_DEBUG)

#pragma push_macro("ITRACE_ERROR")
#undef ITRACE_ERROR
#define ITRACE_ERROR(p1) \
    pAction->GetTraceObject().write(_T("File:  ") + IString(_T(__FILE__)) + _T(".  Function:  ") + IString(_T(__FUNCTION__)) + _T(".  Line:  ") + IString(__LINE__) + IString(_T(".  ERROR:  ")) + (p1), ITRACE_LEVEL_ERROR)

#pragma push_macro("ITRACE_DOT_ERROR")
#undef ITRACE_DOT_ERROR
#define ITRACE_DOT_ERROR() \
    pAction->GetTraceObject().write(_T("File:  ") + IString(_T(__FILE__)) + _T(".  Function:  ") + IString(_T(__FUNCTION__)) + _T(".  Line:  ") + IString(__LINE__) + IString(_T(".  ERROR:  Unknown exception caught")), ITRACE_LEVEL_ERROR)
