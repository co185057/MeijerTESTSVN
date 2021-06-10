#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/StateMApi.h 1     6/20/08 1:13p Sa250050 $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/StateMApi.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     3/14/05 8:14p Dm185016
 * API statistics
*/

#include "SMConstants.h"
#include "statem.h"


#define API_TRACE(API)\
    CApi __api(&m_lApiCount##API, &m_lApiTime##API, &m_lApiMaxTime##API, &m_lApiMinTime##API);
#define API_STATS(API)\
    trace (L4, _T("Api Name:  %s.  Count %d.  Avg Time:  %d.  Min Time:  %d.  Max Time:  %d."), QUOTE(API)\
    , m_lApiCount##API, (m_lApiCount##API == 0 ? 0 : (m_lApiTime##API / m_lApiCount##API)), m_lApiMinTime##API, m_lApiMaxTime##API);

#define DEFINE_API_STATS(API)\
    long m_lApiCount##API;\
    long m_lApiTime##API;\
    long m_lApiMaxTime##API;\
    long m_lApiMinTime##API;
#define INIT_API_STATS(API)\
    m_lApiCount##API(0),\
    m_lApiTime##API(0),\
    m_lApiMaxTime##API(LONG_MIN),\
    m_lApiMinTime##API(LONG_MAX)


namespace STATE_MACHINE_NAMESPACE
{


class STATEM_API CApi
{

public:

    CApi(long *plCount, long *plTime, long *plMaxTime, long *plMinTime);

    ~CApi();


private:

    clock_t m_ctStart;
    long *m_plCount;
    long *m_plTime;
    long *m_plMaxTime;
    long *m_plMinTime;

};

}
