//
// ACSExposedFunctions.h
//
// Change history:
//
// POS97215 Work Request:61233 Name:Matt Condra Date:May 20, 2014
// POS81632 Work Request:59313 Name:Matt Condra Date:December 10, 2013
//
//////////////////////////////////////////////////////////////////////
#pragma once

namespace ACSFuncs
{
	BOOL Load(LPCTSTR pszDll);
   void Unload();
	BOOL IsLoaded();

	BOOL ExIsInitialized();
	BOOL ExInitialize();
	void ExUninitialize();
	BOOL ValidateOperator(LPCTSTR pszOperator, LPCTSTR pszPassword);
	BOOL ValidateSupervisor(LPCTSTR pszSupervisor, LPCTSTR pszPassword);
	
    // POS81632
    INT CheckInOutOperator(INT func, LPCTSTR pszOperator, LPCTSTR pszPassword, INT newRAPid, LPINT currentRAPid, LPINT offlineStatus);

    // POS97215
    INT GetOperatorLastPwdChgTime(LPCTSTR pszID, LPINT timeYear, LPINT timeMonth, LPINT timeDay, LPINT timeHour, LPINT timeMinute, LPINT timeSecond);

    // API supporting EMPLID entry, badge scan and biometric authentication
    typedef enum ExEntryFlag { ACSEX_OPER_KEYED = 0, ACSEX_OPER_SCANNED = 1 } ACSExEntryFlag;
    BOOL ExValidateEmployeeOperatorString(LPCTSTR pszID, LPCTSTR pszPassword, ACSExEntryFlag entryFlag);
    BOOL ExValidateEmployeeSupervisorString(LPCTSTR pszID, LPCTSTR pszPassword, ACSExEntryFlag entryFlag);
    BOOL ExValidateEmployeeOperatorData(LPCTSTR pszID, PBYTE pData, long size, ACSExEntryFlag entryFlag);
    BOOL ExValidateEmployeeSupervisorData(LPCTSTR pszID, PBYTE pData, long size, ACSExEntryFlag entryFlag);
	BOOL ExGetTerminalBusinessDay(long lTerminal, long * businessDay);
    BOOL ExGetEmplidFromBadge(LPCTSTR pszBadge, LPTSTR pszEmplid);
}
