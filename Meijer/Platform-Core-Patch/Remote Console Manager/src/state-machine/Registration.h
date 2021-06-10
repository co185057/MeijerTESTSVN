#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Registration.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)Registration.cpp      $Revision: 1 $  $Date: 6/20/08 1:13p $
 *
 * Copyright 2004, NCR Corporation.
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/Registration.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 1     10/04/04 6:36p Dm185016
* Moved to state machine
* 
* 2     3/01/04 10:02a Dm185016
* Added actions for clearing exceptions
* 
* 1     2/10/04 11:36a Dm185016
* New SecurityManager component
*/

typedef const _TCHAR **regmap_t;

#define BEGIN_REGISTRATION_MAP(MAP_NAME)\
    static const _TCHAR *MAP_NAME[] = \
    {

#define REGISTRATION_ENTRY(NAME)\
      QUOTE(NAME) ,

#define END_REGISTRATION_MAP()\
    NULL\
};

#define CREATE_FXN_NAME(NAME) _T("Create")##QUOTE(NAME)

#define RELEASE_FXN_NAME(NAME) _T("Release")##QUOTE(NAME)
