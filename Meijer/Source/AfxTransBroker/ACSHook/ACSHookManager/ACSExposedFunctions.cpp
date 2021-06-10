//
// ACSExposedFunctions.cpp
//
// Change history:
//
// POS97215 Work Request:61233 Name:Matt Condra Date:May 20, 2014
// POS81632 Work Request:59313 Name:Matt Condra Date:December 10, 2013
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ACSExposedFunctions.h"
#include "Utils.h"
#include "TraceImpl.h"
#include <iomanip>

using NCR::Util::endl;

namespace ACSFuncs
{
    namespace
    {
        typedef BOOL (*LPACSExIsInitializedType)();
        typedef BOOL (*LPACSExInitializeType)();
        typedef void (*LPACSExUninitializeType)();
        typedef BOOL (*LPACSValidateOperatorType)(LPCSTR pszOperator, LPCSTR pszPassword);
        typedef BOOL (*LPACSValidateSupervisorType)(LPCSTR pszSupervisor, LPCSTR pszPassword);

        // POS81632
        typedef INT (*LPACSCheckInOutOperatorType)(INT func, LPCSTR pszOperator, LPCSTR pszPassword, INT newRAPid, LPINT currentRAPid, LPINT offlineStatus);

        // POS97215
        typedef INT (*LPACSGetOperatorLastPwdChgTimeType)(LPCSTR pszID, LPINT timeYear, LPINT timeMonth, LPINT timeDay, LPINT timeHour, LPINT timeMinute, LPINT timeSecond);

        // API supporting EMPLID entry, badge scan and biometric authentication
        typedef BOOL (*LPACSExValidateEmployeeOperatorStringType)(LPCSTR pszID, LPCSTR pszPassword, ACSExEntryFlag entryFlag);
        typedef BOOL (*LPACSExValidateEmployeeSupervisorStringType)(LPCSTR pszID, LPCSTR pszPassword, ACSExEntryFlag entryFlag);
        typedef BOOL (*LPACSExValidateEmployeeOperatorDataType)(LPCSTR pszID, PBYTE pData, long size, ACSExEntryFlag entryFlag);
        typedef BOOL (*LPACSExValidateEmployeeSupervisorDataType)(LPCSTR pszID, PBYTE pData, long size, ACSExEntryFlag entryFlag);
        typedef BOOL (*LPACSExGetTerminalBusinessDayType)(long lTerminalID, long * businessDay);
        typedef BOOL (*LPACSExGetEmplidFromBadgeType)(LPCSTR pszBadge, LPSTR pszEmplid);
		
        LPACSExIsInitializedType ACSExIsInitialized;
        LPACSExInitializeType ACSExInitialize;
        LPACSExUninitializeType ACSExUninitialize;
        LPACSValidateOperatorType ACSValidateOperator; 
        LPACSValidateSupervisorType ACSValidateSupervisor;

        // POS81632
		LPACSCheckInOutOperatorType ACSCheckInOutOperator;

        // POS97215
        LPACSGetOperatorLastPwdChgTimeType ACSGetOperatorLastPwdChgTime;

        // API supporting EMPLID entry, badge scan and biometric authentication
        LPACSExValidateEmployeeOperatorStringType ACSExValidateEmployeeOperatorString;
        LPACSExValidateEmployeeSupervisorStringType ACSExValidateEmployeeSupervisorString;
        LPACSExValidateEmployeeOperatorDataType ACSExValidateEmployeeOperatorData;
        LPACSExValidateEmployeeSupervisorDataType ACSExValidateEmployeeSupervisorData;
       
		LPACSExGetTerminalBusinessDayType ACSExGetTerminalBusinessDay;
        LPACSExGetEmplidFromBadgeType ACSExGetEmplidFromBadge;

        HMODULE lib = 0;
    }

    BOOL IsLoaded()
    {
        return (lib != NULL);
    }

    BOOL Load(LPCTSTR pszDll)
    {
        Trace trace(_T("ACSFuncs::Load"));

        if( lib != NULL )
        {
            trace(DCM_INFO, _T("ACSExposed Functions library already loaded."));
            return TRUE;
        }

        _ASSERT(pszDll && *pszDll);

        // cleanup any residual allocations
        trace(DCM_INFO, _T("Loading ACSExposedFunctions library..."));
        lib = LoadLibrary(pszDll);

        if( !lib )
        {
            trace(DCM_ERROR)
					<< _T("Failed to Load the ACSExposedFunctions library '") << pszDll
					<< _T("'. Error: ") << Utils.GetLastErrorString().GetString() << endl;
            return FALSE;
        }

        BOOL bRet = TRUE;

        //
        // get the function pointers and ensure that we get
        // all of them successfully before returning
        //
        bRet = bRet && ( ACSExIsInitialized = (LPACSExIsInitializedType)GetProcAddress(lib, "ACSExIsInitialized") );
        bRet = bRet && ( ACSExInitialize = (LPACSExInitializeType)GetProcAddress(lib, "ACSExInitialize") );
        bRet = bRet && ( ACSExUninitialize = (LPACSExUninitializeType)GetProcAddress(lib, "ACSExUninitialize") );
        bRet = bRet && ( ACSValidateOperator = (LPACSValidateOperatorType)GetProcAddress(lib, "ACSValidateOperator") );
        bRet = bRet && ( ACSValidateSupervisor = (LPACSValidateSupervisorType)GetProcAddress(lib, "ACSValidateSupervisor") );

        // POS81632
        bRet = bRet && ( ACSCheckInOutOperator = (LPACSCheckInOutOperatorType)GetProcAddress(lib, "ACSCheckInOutOperator") );

        // POS97215
        bRet = bRet && ( ACSGetOperatorLastPwdChgTime = (LPACSGetOperatorLastPwdChgTimeType)GetProcAddress(lib, "ACSGetOperatorLastPwdChgTime") );

        if( !bRet )
        {
            trace(DCM_ERROR, _T("Failed to GetProcAddress on one or more of the ACSExposedFunctions Client Functions."));
        }

        //
        // Optional APIs not required unless Employee ID and/or biometric authentication is required
        //
        BOOL bOptRet = TRUE;
        bOptRet = bOptRet && ( ACSExValidateEmployeeOperatorString = 
            (LPACSExValidateEmployeeOperatorStringType)GetProcAddress(lib, "ACSExValidateEmployeeOperatorString") );
        bOptRet = bOptRet && ( ACSExValidateEmployeeSupervisorString = 
            (LPACSExValidateEmployeeSupervisorStringType)GetProcAddress(lib, "ACSExValidateEmployeeSupervisorString") );
        bOptRet = bOptRet && ( ACSExValidateEmployeeOperatorData = 
            (LPACSExValidateEmployeeOperatorDataType)GetProcAddress(lib, "ACSExValidateEmployeeOperatorData") );
        bOptRet = bOptRet && ( ACSExValidateEmployeeSupervisorData = 
            (LPACSExValidateEmployeeSupervisorDataType)GetProcAddress(lib, "ACSExValidateEmployeeSupervisorData") );

        if( !bOptRet )
        {
            trace(DCM_INFO, _T("ACSExposedFunctions library does not implement the extended API for Employee ID and biometric authentication. "));
        }

		bOptRet = bOptRet && ( ACSExGetTerminalBusinessDay = 
            	(LPACSExGetTerminalBusinessDayType)GetProcAddress(lib, "ACSExGetTerminalBusinessDay") );

		if( !bOptRet )
		{
            trace(DCM_INFO, _T("ACSExposedFunctions library does not implement the extended API for ExGetTerminalBusinessDay. "));
		}

        bOptRet = bOptRet && ( ACSExGetEmplidFromBadge = 
            	(LPACSExGetEmplidFromBadgeType)GetProcAddress(lib, "ACSExGetEmplidFromBadge") );

		if( !bOptRet )
		{
            trace(DCM_INFO, _T("ACSExposedFunctions library does not implement the extended API for ExGetEmplidFromBadge. "));
		}

        if( !ExInitialize() )
        {
            trace(DCM_ERROR, _T("ACSExposedFunctions::ACSExInitialize() call failed. Continuing anyway."));
        }

        return bRet;
    }

    void Unload()
    {
        Trace trace(_T("ACSFuncs::Unload"));

         if( lib )
         {
               ExUninitialize();

               if( !FreeLibrary(lib) )
               {
                  trace(DCM_WARNING)
							<< _T("ACSFuncs::Unload - FreeLibrary call Failed to unload the ACSExposedFunctions library. Error: ") << Utils.GetLastErrorString().GetString() << endl;
               }
         }

        ACSExIsInitialized = NULL;
        ACSExInitialize = NULL;
        ACSExUninitialize = NULL;
        ACSValidateOperator = NULL;
        ACSValidateSupervisor = NULL;

        // POS81632
        ACSCheckInOutOperator = NULL;

        // POS97215
        ACSGetOperatorLastPwdChgTime = NULL;

        ACSExValidateEmployeeOperatorString = NULL;
        ACSExValidateEmployeeSupervisorString = NULL;
        ACSExValidateEmployeeOperatorData = NULL;
        ACSExValidateEmployeeSupervisorData = NULL;
		  ACSExGetTerminalBusinessDay = NULL;
        ACSExGetEmplidFromBadge = NULL;

        lib = NULL;
    }
    
    BOOL ExIsInitialized()
    {
        Trace trace(_T("ACSFuncs::ExIsInitialized"));

        trace(DCM_INFO, _T("+ACSFuncs::ExIsInitialized"));
        BOOL bRet = FALSE;
        if( ACSExIsInitialized )
        {
            bRet = ACSExIsInitialized();
        }
        else
        {
            trace(DCM_ERROR, _T("ACSExIsInitialized export is not available, returning FALSE."));
        }
        trace(DCM_INFO, _T("-ACSFuncs::ExIsInitialized"));
        return bRet;
    }

    BOOL ExInitialize()
    {
        Trace trace(_T("ACSFuncs::ExInitialize"));
        
        trace(DCM_INFO, _T("+ACSFuncs::ExInitialize"));
        BOOL bRet = FALSE;
        if( ACSExInitialize )
        {
            trace(DCM_INFO, _T("Loading ACSExposedFunctions library..."));
            bRet = ACSExInitialize();
        }
        else
        {
            trace(DCM_ERROR, _T("ACSExInitialize export is not available, returning FALSE."));
        }
        trace(DCM_INFO, _T("-ACSFuncs::ExInitialize"));
        return bRet;
    }

    void ExUninitialize()
    {
        Trace trace(_T("ACSFuncs::ExUninitialize"));
        trace(DCM_INFO, _T("+ACSFuncs::ExUninitialize"));
        if( ACSExUninitialize )
        {
            ACSExUninitialize();
        }
        else
        {
            trace(DCM_ERROR, _T("ACSExUninitialize export is not available, returning FALSE."));
        }
        trace(DCM_INFO, _T("-ACSFuncs::ExUninitialize"));
    }

    BOOL ValidateOperator(LPCTSTR pszOperator, LPCTSTR pszPassword)
    {
        Trace trace(_T("ACSFuncs::ValidateOperator"));
        trace(DCM_INFO, _T("+ACSFuncs::ValidateOperator"));
        BOOL bRet = FALSE;
        if( ACSValidateOperator )
        {
            bRet = ACSValidateOperator(CT2A(pszOperator), CT2A(pszPassword));
        }
        else
        {
            trace(DCM_ERROR, _T("ACSValidateOperator export is not available, returning FALSE."));
        }
        trace(DCM_INFO, _T("-ACSFuncs::ValidateOperator"));
        return bRet;
    }

    BOOL ValidateSupervisor(LPCTSTR pszSupervisor, LPCTSTR pszPassword)
    {
        Trace trace(_T("ACSFuncs::ValidateSupervisor"));
        trace(DCM_INFO, _T("+ACSFuncs::ValidateSupervisor"));
        BOOL bRet = FALSE;
        if( ACSValidateSupervisor )
        {
            bRet = ACSValidateSupervisor(CT2A(pszSupervisor), CT2A(pszPassword));
        }
        else
        {
            trace(DCM_ERROR, _T("ACSValidateSupervisor export is not available, returning FALSE."));
        }
        trace(DCM_INFO, _T("-ACSFuncs::ValidateSupervisor"));
        return bRet;
    }

    // POS81632
    BOOL CheckInOutOperator(INT func, LPCTSTR pszOperator, LPCTSTR pszPassword, INT newRAPid, LPINT currentRAPid, LPINT offlineStatus)
    {
        Trace trace(_T("ACSFuncs::CheckInOutOperator"));
        trace(DCM_INFO, _T("+"));
        trace(DCM_INFO) << _T("func:") << func << _T(" oper:") << (pszOperator ? pszOperator : _T("null")) << _T(" newRAPid:") << newRAPid << endl;

        INT nRet = -1;
        if( ACSCheckInOutOperator )
        {
            nRet = ACSCheckInOutOperator(func, CT2A(pszOperator), CT2A(pszPassword), newRAPid, currentRAPid, offlineStatus);
        }
        else
        {
            trace(DCM_ERROR, _T("ACSCheckInOutOperator export is not available"));
        }

        trace(DCM_INFO) << _T("ret:") << nRet << _T(" currentRAPid:") << (currentRAPid ? *currentRAPid : -1) << _T(" offlineStatus:") << (offlineStatus ? *offlineStatus : -1) << endl;
        trace(DCM_INFO, _T("-"));
        return nRet;
    }

    // POS97215
    BOOL GetOperatorLastPwdChgTime(LPCTSTR pszOperator, LPINT timeYear, LPINT timeMonth, LPINT timeDay, LPINT timeHour, LPINT timeMinute, LPINT timeSecond)
    {
        Trace trace(_T("ACSFuncs::GetOperatorLastPwdChgTime"));
        trace(DCM_INFO, _T("+"));
        trace(DCM_INFO) << _T(" oper:") << (pszOperator ? pszOperator : _T("null")) << endl;

        INT nRet = -1;
        if( ACSGetOperatorLastPwdChgTime )
        {
            nRet = ACSGetOperatorLastPwdChgTime(CT2A(pszOperator), timeYear, timeMonth, timeDay, timeHour, timeMinute, timeSecond);
        }
        else
        {
            trace(DCM_ERROR, _T("ACSGetOperatorLastPwdChgTime export is not available"));
        }

        trace(DCM_INFO) << _T("ret:") << nRet << _T(" year:") << (timeYear ? *timeYear : -1) << _T(" month:") << (timeMonth ? *timeMonth : -1) << _T(" day:") << (timeDay ? *timeDay : -1) 
           << _T(" hour:") << (timeHour ? *timeHour : -1) << _T(" minute:") << (timeMinute ? *timeMinute : -1) << _T(" second:") << (timeSecond ? *timeSecond : -1) << endl;
        trace(DCM_INFO, _T("-"));
        return nRet;
    }

    BOOL ExValidateEmployeeOperatorString(LPCTSTR pszID, LPCTSTR pszPassword, ACSExEntryFlag entryFlag)
    {
        Trace trace(_T("ACSFuncs::ExValidateEmployeeOperatorString"));
        trace(DCM_INFO, _T("+ACSFuncs::ExValidateEmployeeOperatorString"));
        BOOL bRet = FALSE;
        if( ACSExValidateEmployeeOperatorString )
        {
            bRet = ACSExValidateEmployeeOperatorString(CT2A(pszID), CT2A(pszPassword), entryFlag);
        }
        else
        {
            trace(DCM_ERROR, _T("ACSExValidateEmployeeOperatorString export is not available, returning FALSE."));
        }
        trace(DCM_INFO, _T("-ACSFuncs::ExValidateEmployeeOperatorString"));
        return bRet;
    }

    BOOL ExValidateEmployeeSupervisorString(LPCTSTR pszID, LPCTSTR pszPassword, ACSExEntryFlag entryFlag)
    {
        Trace trace(_T("ACSFuncs::ExValidateEmployeeSupervisorString"));
        trace(DCM_INFO, _T("+ACSFuncs::ExValidateEmployeeSupervisorString"));
        BOOL bRet = FALSE;
        if( ACSExValidateEmployeeSupervisorString )
        {
            bRet = ACSExValidateEmployeeSupervisorString(CT2A(pszID), CT2A(pszPassword), entryFlag);
        }
        else
        {
            trace(DCM_ERROR, _T("ACSExValidateEmployeeSupervisorString export is not available, returning FALSE."));
        }
        trace(DCM_INFO, _T("-ACSFuncs::ExValidateEmployeeSupervisorString"));
        return bRet;
    }

    BOOL ExValidateEmployeeOperatorData(LPCTSTR pszID, PBYTE pData, long size, ACSExEntryFlag entryFlag)
    {
        Trace trace(_T("ACSFuncs::ExValidateEmployeeOperatorData"));
        trace(DCM_INFO, _T("+ACSFuncs::ExValidateEmployeeOperatorData"));
        BOOL bRet = FALSE;
        if( ACSExValidateEmployeeOperatorData )
        {
            bRet = ACSExValidateEmployeeOperatorData(CT2A(pszID), pData, size, entryFlag);
        }
        else
        {
            trace(DCM_ERROR, _T("ACSExValidateEmployeeOperatorData export is not available, returning FALSE."));
        }
        trace(DCM_INFO, _T("-ACSFuncs::ExValidateEmployeeOperatorData"));
        return bRet;
    }

    BOOL ExValidateEmployeeSupervisorData(LPCTSTR pszID, PBYTE pData, long size, ACSExEntryFlag entryFlag)
    {
        Trace trace(_T("ACSFuncs::ExValidateEmployeeSupervisorData"));
        trace(DCM_INFO, _T("+ACSFuncs::ExValidateEmployeeSupervisorData"));
        BOOL bRet = FALSE;
        if( ACSExValidateEmployeeSupervisorData )
        {
            bRet = ACSExValidateEmployeeSupervisorData(CT2A(pszID), pData, size, entryFlag);
        }
        else
        {
            trace(DCM_ERROR, _T("ACSExValidateEmployeeSupervisorData export is not available, returning FALSE."));
        }
        trace(DCM_INFO, _T("-ACSFuncs::ExValidateEmployeeSupervisorData"));
        return bRet;
    }


	BOOL ExGetTerminalBusinessDay(long lTerminalID, long * businessDay)
    {
        Trace trace(_T("ACSFuncs::ExGetTerminalBusinessDay"));
        trace(DCM_INFO, _T("+ACSFuncs::ExGetTerminalBusinessDay"));

        BOOL bRet = FALSE;
        if( ACSExGetTerminalBusinessDay )
        {
            bRet = ACSExGetTerminalBusinessDay((int)lTerminalID, businessDay);
        }
        else
        {
            trace(DCM_ERROR, _T("ExGetTerminalBusinessDay export is not available, returning FALSE."));
        }
        trace(DCM_INFO, _T("-ACSFuncs::ExGetTerminalBusinessDay"));
        return bRet;
    }

    BOOL ExGetEmplidFromBadge(LPCTSTR pszBadge, LPTSTR pszEmplid)
    {
        Trace trace(_T("ACSFuncs::ExGetEmplidFromBadge"));
        trace(DCM_INFO, _T("+ACSFuncs::ExGetEmplidFromBadge"));

        BOOL bRet = FALSE;
        if( ACSExGetEmplidFromBadge )
        {
            char ansiEmplid[MAX_PATH] = {0};
            bRet = ACSExGetEmplidFromBadge(CT2A(pszBadge), ansiEmplid);
            if( bRet )
            {
                _tcsncpy(pszEmplid, CA2T(ansiEmplid), MAX_PATH);
            }
            else
            {
                trace(DCM_ERROR, _T("ExGetEmplidFromBadge call failed."));
                pszEmplid[0] = NULL;
            }
        }
        else
        {
            trace(DCM_ERROR, _T("ExGetEmplidFromBadge export is not available, returning FALSE."));
        }
        trace(DCM_INFO, _T("-ACSFuncs::ExGetEmplidFromBadge"));
        return bRet;
    }
}
