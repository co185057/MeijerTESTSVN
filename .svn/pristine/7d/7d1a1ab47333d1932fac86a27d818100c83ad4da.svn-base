#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/content/ControlImpl.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ControlImpl.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/content/ControlImpl.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:24p Dm185016
 * Moved to new Repository.
* 
* 2     11/16/04 11:45a Dm185016
* Cleaned up code
* 
* 1     10/29/04 9:17a Dm185016
*/

using namespace std;

#include "XMLContent.h"
#include "SMConstants.h"
#include "xerror.h"
#include <vector>
#include <map>

using namespace STATE_MACHINE_NAMESPACE;

namespace fastlane
{


class CControlImpl
    : public CXMLContent
{
public:

    CControlImpl(HTRACE hTrace, const StringType &name, const StringType &type);
    virtual ~CControlImpl(void) throw();

	virtual int GetId() const throw();

	virtual const _TCHAR *GetComment() const throw();

	virtual const _TCHAR *GetName() const throw();
    
    virtual const _TCHAR *GetType() const throw();

    virtual void SetIgnored(bool);
    virtual bool IsIgnored() const throw();

    DECLARE_STRING_PROPERTY(Name);
    DECLARE_STRING_PROPERTY(Comment);
    DECLARE_STRING_PROPERTY(Type);

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);     //lint !e1735

private:

    CControlImpl(const CControlImpl &);                     // Hide the Copy construcor
    CControlImpl &operator =(const CControlImpl &);   // Hide the assignment operator

	static int m_nControlCounter;

    bool m_bIgnored;

    int m_nId;
};

typedef CControlImpl *PCONTROL;
typedef vector<PCONTROL> CControlsVector;

//typedef hash_map<const _TCHAR *, IState *> CStatesMap;
typedef map<StringType, PCONTROL, StringComparator> CControlsMap;

}
