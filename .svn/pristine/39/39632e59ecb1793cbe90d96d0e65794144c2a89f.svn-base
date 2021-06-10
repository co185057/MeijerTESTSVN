// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/System.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)System.cpp    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/System.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     3/01/05 2:04p Dm185016
*/
#include "StdAfx.h"
#include "System.h"

using namespace utils;


CSystem CSystem::theSystem;


CSystem::CSystem()
    : m_pSystem(NULL)
{
    //m_pSystem = new CWin32System;
}


CSystem::~CSystem() 
{
    delete m_pSystem;
}


const _TCHAR *CSystem::GetComputerName() { return theSystem.system().GetComputerName(); }
