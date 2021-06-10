// ACSHookClientCPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void cls(void);

typedef void (WINAPI *LPACSHOOKCALLBACKPROC)(long lEvent, long lData, BSTR bstrData1, BSTR bstrData2);
void WINAPI ACSHookCallbackProc(long lEvent, long lData, BSTR bstrData1, BSTR bstrData2)
{
	// CAUTION: we're in a foreign thread when this is called, so don't do anything funny, 
	// and don't take long doing whatever you wanna do in here either!
	_bstr_t bstr1(bstrData1,FALSE);
	_bstr_t bstr2(bstrData2, FALSE);

}

int _tmain(int argc, _TCHAR* argv[])
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	/*
	TBC::ItbConnectPtr spCon;

	HRESULT hr = spCon.CreateInstance(__uuidof(TBC::CtbConnect));

	try
	{
	spCon->Connect();

	spCon->Disconnect();
	}
	catch(_com_error & cex)
	{
	ATLTRACE2(cex.ErrorMessage());
	ATLTRACE2(_T("\n"));
	}
	*/
	_tprintf(_T("Creating ACSHookManager Instance...\n"));
	ACSHOOK::IACSHookPtr spACS;
	HRESULT hr = spACS.CreateInstance(__uuidof(ACSHOOK::CACSHook));
	if( FAILED(hr) )
	{
		ATLTRACE2("Failed to create spACS\n");
		Sleep(5000);
		return 0;
	}

	_variant_t vtCallback;


	vtCallback.ullVal = (ULONGLONG)ACSHookCallbackProc;
	vtCallback.vt = VT_UI8;


	spACS->SetCallback(vtCallback);

	try
	{
		_tprintf(_T("Connecting...\n"));
		spACS->Connect();

		BOOL bOnce = FALSE;
		int iKey;

		do
		{
			cls();
			_tprintf(_T("Commands: \n")
				_T("* <Enter> : exit\n")
                _T("* B       : Run GetEmplidFromBadge for TM51511100 - TM51511810\n")
                _T("* V       : Run through all Validate calls with 123456789/1\n")
				_T("* <Space> : ValidateOperator U:100 P:1\n")
				_T("* other key: ValidateOperator U:1234 P:1\n")
				_T("> "));

			if((iKey = _getche()) != 13)
			{
                if( iKey == 'v' || iKey == 'V' )
                {
                    spACS->ValidateOperator(_T("123456789"), _T("1"));
                    spACS->ValidateSupervisor(_T("123456789"), _T("1"));
                    spACS->ExValidateEmployeeOperatorString(_T("123456789"), _T("1"), ACSHOOK::ACSEX_OPER_KEYED);
                    spACS->ExValidateEmployeeSupervisorString(_T("123456789"), _T("1"), ACSHOOK::ACSEX_OPER_SCANNED);
                    spACS->ExValidateEmployeeOperatorData(_T("123456789"), _variant_t(_T("ExValidateEmployeeOperatorData")),ACSHOOK::ACSEX_OPER_KEYED);
                    spACS->ExValidateEmployeeSupervisorData(_T("123456789"), _variant_t(_T("ExValidateEmployeeSupervisorData")),ACSHOOK::ACSEX_OPER_SCANNED);
                    BYTE byData[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0xba, 0xad, 0xf0, 0x0d, 0x00, };
                    
                    CComSafeArrayBound bound[1];
                    bound[0].SetCount(0);
                    bound[0].SetLowerBound(0);
                    CComSafeArray<BYTE> sa = CComSafeArray<BYTE>(bound,1);
                    sa.Add(sizeof(byData),byData);
                    CComVariant vData(sa);

                    spACS->ExValidateEmployeeOperatorData(_T("123456789"), vData, ACSHOOK::ACSEX_OPER_KEYED);
                    spACS->ExValidateEmployeeSupervisorData(_T("123456789"), vData, ACSHOOK::ACSEX_OPER_SCANNED);
                }
                else if( iKey == 'b' || iKey == 'B' )
                {
                    TCHAR szBadge[MAX_PATH];
                    _bstr_t bstrEmplid;

                    for(int i = 1100; i <= 1810; i++)
                    {
                        _stprintf(szBadge, _T("TM5151%04d"), i);

                        VARIANT_BOOL vbRet = spACS->ExGetEmplidFromBadge(szBadge, bstrEmplid.GetAddress());
                        if( VARIANT_TRUE == vbRet )
                        {
                            _tprintf(_T("\nExGetEmplidFromBadge(%s) succeeded with result = %s"), szBadge, (LPCTSTR)bstrEmplid);
                        }
                        else
                        {
                            _tprintf(_T("\nExGetEmplidFromBadge(%s) failed with result = %s"), szBadge, (LPCTSTR)bstrEmplid);
                        }
                    }
                }
                else
                {
				    _bstr_t strUser = iKey == 32 ? OLESTR("810") : OLESTR("1234");

				    if( spACS->ValidateOperator(strUser, OLESTR("1")) )
				    {
					    _tprintf(_T("\nValidateOperator==TRUE\n"));
				    }
				    else
				    {
					    _tprintf(_T("\nValidateOperator==FALSE\n"));
				    }
                }
			}
			if( iKey != 13 )
			{
				_tprintf(_T("\nStrike any key to continue...\n"));
				_getche();
			}
		} while( iKey != 13);

		_tprintf(_T("Disconnecting...\n"));
		spACS->Disconnect();
	}
	catch(_com_error & cex)
	{
		_tprintf(_T("%s\n"), cex.ErrorMessage());
	}

	// 
	// we will block here on exit within the ACSFuncs.Unload calling FreeLibrary() for ACSExposedFunctions 
	//
	_tprintf(_T("Releasing ACSHookManager object...\n"));
	spACS = NULL;
	_tprintf(_T("ACSHookManager object released. Exiting.\nStrike any key to continue...\n"));
	_getche();
}

void cls(){
	CONSOLE_SCREEN_BUFFER_INFO conInfo;
	DWORD dummy;
	COORD home = { 0, 0 };
	HANDLE hConsoleOut;

	hConsoleOut = GetStdHandle( STD_OUTPUT_HANDLE );

	if(hConsoleOut == INVALID_HANDLE_VALUE) return;

	GetConsoleScreenBufferInfo( hConsoleOut , &conInfo );
	FillConsoleOutputCharacter( hConsoleOut ,' ', conInfo.dwSize.X * conInfo.dwSize.Y, home, &dummy );
	SetConsoleCursorPosition(hConsoleOut,home);
}
