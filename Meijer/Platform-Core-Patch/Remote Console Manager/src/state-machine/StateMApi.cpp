// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/StateMApi.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)StateMApi.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/StateMApi.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     3/14/05 8:14p Dm185016
 * API statistics
*/

#include "StdAfx.h"
#include "StateMApi.h"

using namespace STATE_MACHINE_NAMESPACE;


CApi::CApi(long *plCount, long *plTime, long *plMaxTime, long *plMinTime)
    : m_plCount(plCount)
    , m_plTime(plTime)
    , m_plMaxTime(plMaxTime)
    , m_plMinTime(plMinTime)
{
    m_ctStart = clock();
}


CApi::~CApi()
{
    (*m_plCount)++;
    long lTime = clock() - m_ctStart;
    (*m_plTime) += lTime;

    if (*m_plMaxTime < lTime)
        (*m_plMaxTime) = lTime;
    if (*m_plMinTime > lTime)
        (*m_plMinTime) = lTime;
}
