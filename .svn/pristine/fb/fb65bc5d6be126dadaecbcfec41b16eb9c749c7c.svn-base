// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/actions/ValidateBirthday.cpp 4     12/16/08 2:03p Sa250050 $
/*
 * @(#)ValidateBirthday.cpp    $Revision: 4 $ $Date: 12/16/08 2:03p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/actions/ValidateBirthday.cpp $
 * 
 * 4     12/16/08 2:03p Sa250050
 * Patch G Build 337 Migration
 * 
 * 3     10/21/08 4:08p Sm133076
 * Patch G Migration
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     2/28/05 4:15p Dm185016
 * lint
 * 
 * 2     2/08/05 7:12p Tp151000
 * TAR 292777
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
// 
// 3     12/09/04 1:46p Dm185016
// Added trace.  TAR 287222.
// 
// 2     11/19/04 1:37p Dm185016
// Added trace information.
// 
// 1     11/16/04 11:23a Dm185016
*/
#include "StdAfx.h"


#include "ValidateBirthday.h"
#include "RemoteAPMgr.h"
#include "LocaleEx.h"

using namespace rap;
using namespace fastlane;
using namespace utils;


CValidateBirthday::CValidateBirthday()
{
    CRCMActionBase::SetName(_T("CValidateBirthday"));
}


CValidateBirthday::~CValidateBirthday()
{
}


HRESULT CValidateBirthday::Execute(bool *pResult, IMessage &)
{
    IMODTRACE_DEVELOP();

	int birthMonth=0;
	int birthDay=0;
	int birthYear=0;

	CString csEntryDate = rapmgr().GetEntryText();
	int nEntryDateLength = (int)csEntryDate.GetLength();
    ITRACE_DEVELOP(_T("nEntryDateLength:  ") + IString(nEntryDateLength));

	COleDateTime currentDate = COleDateTime::GetCurrentTime();
	
	int nNoCenturyDigit = 0;
    if(CLocale::GetCenturyDigitCount() == 0)
        nNoCenturyDigit = 2;
    else
        nNoCenturyDigit = 4;

    int nShortDateOrdering = CLocale::GetLocaleShortDateOrdering(); 
    ITRACE_DEVELOP(_T("nShortDateOrdering:  ") + IString(nShortDateOrdering));
	// Not physically allowed to enter more, only less
	switch (nEntryDateLength)
	{
        case 6 :
            switch (nShortDateOrdering)
		    {
		        case 0: // Month-Day-Year
			        birthMonth  = _ttoi(csEntryDate.Left(2));
			        birthDay    = _ttoi(csEntryDate.Mid(2,2));
			        birthYear   = _ttoi(csEntryDate.Right(2));
			        break;
		        case 1: // Day-Month-Year
			        birthDay    = _ttoi(csEntryDate.Left(2));
			        birthMonth  = _ttoi(csEntryDate.Mid(2,2));
			        birthYear   = _ttoi(csEntryDate.Right(2));
			        break;
		        case 2: // Year-Month-Day
			        birthYear   = _ttoi(csEntryDate.Left(2));
			        birthMonth  = _ttoi(csEntryDate.Mid(2,2));
			        birthDay    = _ttoi(csEntryDate.Right(2));
			        break;
		        default:
			        break;
		    }
            break;
        case 8 :
		    switch (nShortDateOrdering)
		    {
		        case 0: // Month-Day-Year
			        birthMonth  = _ttoi(csEntryDate.Left(2));
			        birthDay    = _ttoi(csEntryDate.Mid(2,2));
			        birthYear   = _ttoi(csEntryDate.Right(4));
			        break;
		        case 1: // Day-Month-Year
			        birthDay    = _ttoi(csEntryDate.Left(2));
			        birthMonth  = _ttoi(csEntryDate.Mid(2,2));
			        birthYear   = _ttoi(csEntryDate.Right(4));
			        break;
		        case 2: // Year-Month-Day
			        birthYear   = _ttoi(csEntryDate.Left(4));
			        birthMonth  = _ttoi(csEntryDate.Mid(4,2));
			        birthDay    = _ttoi(csEntryDate.Right(2));
			        break;
		        default:
			        break;
		    }
            break;
        default :
            *pResult = false;
            return S_OK;
	}

    // If the 2 digit year they entered is greater
    // than or equal to the current 2 digit year, assume they
    // mean 19xx. If it's less, assume they mean 20xx.
    if (nNoCenturyDigit==2)
    {
        if ( birthYear > _ttoi(currentDate.Format(_T("%y"))) )
		    birthYear += 1900;
        else
		    birthYear += 2000;
    }
    
    COleDateTime birthDate(birthYear, birthMonth, birthDay, 0, 0, 0);
    const COleDateTime &validBirthDate = rapmgr().GetValidBirthday();
    ITRACE_DEVELOP(_T("birthDate:  ") + IString((const _TCHAR *)birthDate.Format()));
    ITRACE_DEVELOP(_T("valid birthDate:  ") + IString((const _TCHAR *)validBirthDate.Format()));
		
    // If the date isn't valid (e.g. 11/35/62 or 02/29/98 (not leap yr.)),
    // return false.
    int datelength = CLocale::GetDateFormatCount();
    if(nEntryDateLength != datelength)
    {
        *pResult = false;
        return S_OK;
    }

    if (birthDate.GetStatus() == COleDateTime::valid)
	{
		//Format the birth date before sending to SCOT
		rapmgr().SetEntryText(CLocale::DateFormat(birthMonth, birthDay, birthYear));

        // Newer birthday will be greater than valid date
        //*pResult = true;
        //*pResult = (birthDate < validBirthDate);
		
		if ( birthDay == 0 || birthMonth == 0 || ( nNoCenturyDigit == 4 && birthYear == 0 ))
		{
			*pResult = false;
		}
		else
		{
			*pResult = true;
		}

	}
	else
        *pResult = false;

    return S_OK;
}


template <>
IAction* CCreateable<CValidateBirthday, IAction>::NewInstance()
{
    IAction *pAction = new CValidateBirthday;

    return pAction;
}


template <>
void CCreateable<CValidateBirthday, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ValidateBirthday)
DEFINE_ACTION_RELEASE_FXN(ValidateBirthday)
