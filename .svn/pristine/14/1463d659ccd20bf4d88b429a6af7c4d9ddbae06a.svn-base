#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/System.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)System.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/System.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     3/01/05 2:04p Dm185016
*/
#include "Utils.h"
#include "Singleton.h"

namespace utils
{

class ISystem;

class UTILS_API CSystem
{
public:

    virtual ~CSystem();

    static const _TCHAR *GetComputerName();

private:

    CSystem();

    ISystem &system() 
    {
        _ASSERTE(m_pSystem!=NULL);
        return *m_pSystem; 
    }

    ISystem *m_pSystem;

    static CSystem theSystem;
};


class UTILS_API ISystem
{
public:

    virtual ~ISystem() {}

    virtual const _TCHAR *GetComputerName() const=0;

private:

    ISystem();
    ISystem(const ISystem &);               // Hide copy constructor
    ISystem &operator =(const ISystem &);   // Hide the assignment operator

};

}