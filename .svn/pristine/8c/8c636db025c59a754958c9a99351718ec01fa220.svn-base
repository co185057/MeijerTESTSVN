#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/PSXError.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)PSXError.h      $Revision: 1 $  $Date: 6/20/08 1:13p $
 *
 * Copyright 2004, NCR Corporation.
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with msi.
 */

#include "utils.h"
#include "xerror.h"

#include "PSX API.h"
using namespace UI;

#define PSX_ERROR_BASE                 0xC0050000L


#define ITHROWPSXERROR(id)\
    {\
        CPSXErrorInfo error(id, _T(""));\
    	ITHROWPSXERRORI(id, error)\
    }

#define ITHROWPSXERRORI(id, info)\
    {\
    	CPSXError exc(info.text(), id);\
        exc.setSeverity(IException::unrecoverable);\
     	exc.addLocation(IEXCEPTION_LOCATION());\
        exc.setErrorInfo(&info);\
	    throw(exc);\
    }

#pragma warning( push )
#pragma warning( disable : 4251 )

class UTILS_API CPSXErrorInfo
    : public utils::IErrorInfo
{
public:
    /*------------------------------- Constructors -------------------------------*/
        CPSXErrorInfo ( PSXRC, const _TCHAR *pszText = _T("") );

    virtual
        ~CPSXErrorInfo ( );

    virtual const TCHAR *text ( ) const;

    virtual HRESULT errorId ( ) const;

    virtual IErrorInfo* copy( ) const;

protected:

    CPSXErrorInfo (  );

	const TCHAR *lookupError(PSXRC) const;
    void Init ( PSXRC, const TCHAR *, const TCHAR * );

private:
    CPSXErrorInfo ( const CPSXErrorInfo& errorInformation );

    CPSXErrorInfo
        &operator= ( const CPSXErrorInfo& errorInformation );

    IString m_szErrorText;
    HRESULT m_nErrorRC;
}; // CPSXErrorInfo


IEXCLASSDECLARE_API(CPSXError,IException,UTILS_API);

#pragma warning( pop )
