#pragma once

#ifdef ACSEXPOSEDFUNCTIONSTESTSTUB_EXPORTS
#define ACSEXPOSEDFUNCTIONSTESTSTUB_API __declspec(dllexport)
#else
#define ACSEXPOSEDFUNCTIONSTESTSTUB_API __declspec(dllimport)
#endif

ACSEXPOSEDFUNCTIONSTESTSTUB_API BOOL ACSExIsInitialized();
ACSEXPOSEDFUNCTIONSTESTSTUB_API BOOL ACSExInitialize();
ACSEXPOSEDFUNCTIONSTESTSTUB_API void ACSExUninitialize();
ACSEXPOSEDFUNCTIONSTESTSTUB_API BOOL ACSValidateOperator(PCSTR pszOperator, PCSTR pszPassword);
ACSEXPOSEDFUNCTIONSTESTSTUB_API BOOL ACSValidateSupervisor(PCSTR pszSupervisor, PCSTR pszPassword);

// API supporting EMPLID entry, badge scan and biometric authentication
ACSEXPOSEDFUNCTIONSTESTSTUB_API typedef enum ExEntryFlag { ACSEX_OPER_KEYED = 0, ACSEX_OPER_SCANNED = 1 } ACSExEntryFlag;
ACSEXPOSEDFUNCTIONSTESTSTUB_API BOOL ACSExValidateEmployeeOperatorString(PCSTR pszID, PCSTR pszPassword, ACSExEntryFlag entryFlag);
ACSEXPOSEDFUNCTIONSTESTSTUB_API BOOL ACSExValidateEmployeeSupervisorString(PCSTR pszID, PCSTR pszPassword, ACSExEntryFlag entryFlag);
ACSEXPOSEDFUNCTIONSTESTSTUB_API BOOL ACSExValidateEmployeeOperatorData(PCSTR pszID, PBYTE pData, long size, ACSExEntryFlag entryFlag);
ACSEXPOSEDFUNCTIONSTESTSTUB_API BOOL ACSExValidateEmployeeSupervisorData(PCSTR pszID, PBYTE pData, long size, ACSExEntryFlag entryFlag);