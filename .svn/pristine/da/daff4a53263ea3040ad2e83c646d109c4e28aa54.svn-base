#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMLexicalHandler.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)SMLexicalHandler.h      $Revision: 1 $  $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMLexicalHandler.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     2/28/05 3:54p Dm185016
 * lint
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 3     10/29/04 9:23a Dm185016
* Made trace optional
* 
* 1     6/30/04 3:26p Dm185016
* Added handler for comments.
*/

#include "statem.h"
#include "SMConstants.h"
#include "SAXLexicalHandlerImpl.h"

namespace STATE_MACHINE_NAMESPACE 
{

class STATEM_API CSMLexicalHandler : public SAXLexicalHandlerImpl
{
public:
    CSMLexicalHandler() : m_pLoggable(NULL) {};
    virtual ~CSMLexicalHandler() 
    {
        m_pLoggable = NULL;
    };

    virtual HRESULT STDMETHODCALLTYPE comment (
    /*[in]*/ unsigned short * pwchChars,
    /*[in]*/ int cchChars );

protected:

    ILoggable &loggable() const;

private:

    CSMLexicalHandler(const CSMLexicalHandler &);               // Hide the Copy construcor
    CSMLexicalHandler &operator =(const CSMLexicalHandler &);   // Hide the assignment operator

    mutable ILoggable *m_pLoggable;
};

}
