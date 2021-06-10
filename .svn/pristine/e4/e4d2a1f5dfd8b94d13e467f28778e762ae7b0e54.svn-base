#include "stdafx.h"
#include "Options.h"
#include "Utils.h"
#include <map>
#include "tstring.h"
#include "TraceImpl.h"

namespace Options
{
   namespace
   {
      long terminalNumber = -1;

      CAtlMap<CString, CString> m_cmOptionMap;
      CAtlMap<CString, CString> m_cmDefaults;

      CCriticalSection m_csOptions;

      /*************************************************************************
      Reads in the .dat, .000, and .xxx option files to create the option map.
      *************************************************************************/
      void CreateOptionMap(const CString& strFileName)
      {
         Trace trace(_T("CreateOptionMap"));

         trace(DCM_INFO, _T("Create Option Map"));
          
         TCHAR szSKeys[4048];
          
         for(int i = 0; i < 3; i++)
         {
            CString tmpFile;
            switch(i)
            {
            case 0:	
			      tmpFile = strFileName.Left(strFileName.GetLength()-4) + _T(".dat"); 
			      break;
            case 1: 
			      tmpFile = strFileName.Left(strFileName.GetLength()-4) + _T(".000"); 
			      break;
            case 2: 
			      tmpFile = strFileName;
			      break;
            };
            int lRC = GetPrivateProfileSectionNames(szSKeys, sizeof(szSKeys)/sizeof(TCHAR), tmpFile);
            if(lRC <= 0)
            {
                  continue;
            }
              
            TCHAR *pSKey = szSKeys;
            CString szSWorkingKey;
              
            while(*pSKey)
            {   
                  szSWorkingKey = pSKey;
                  pSKey = pSKey + szSWorkingKey.GetLength() + 1;
                  
                  TCHAR szKeys[4048];
                  GetPrivateProfileString(szSWorkingKey, NULL, NULL, szKeys, 
                     sizeof(szKeys)/sizeof(TCHAR), tmpFile);
                  
                  TCHAR *pKey = szKeys;    
                  CString szWorkingKey;
                  
                  while(*pKey)
                  {
                     szWorkingKey = pKey;
                     pKey = pKey + szWorkingKey.GetLength() + 1;
                      
                     if(szWorkingKey == _T("SMMWtMaxTol"))
                        int i = 1;
                      
                     CString szOptVal;
                     TCHAR tmp[4096];
                     GetPrivateProfileString(szSWorkingKey, szWorkingKey, _T("NotFound"), 
                        tmp, sizeof(tmp)/sizeof(TCHAR), tmpFile);
                      
                     szOptVal = tmp;
                     if(szOptVal == _T("NotFound"))
                     {
                        pSKey = NULL;
                     }
				      m_csOptions.Enter();
                      
                     m_cmOptionMap.SetAt(szWorkingKey, szOptVal);
                     m_csOptions.Leave();
                  }
            } 
         }
      }

      /*************************************************************************
      Read in Terminal number from registry
      *************************************************************************/
      int DetermineTerminalNumber(HKEY hKey)
      {
         Trace trace(_T("DetermineTerminalNumber"));

	      trace(DCM_INFO, _T("DetermineTerminalNumber"));

	      //
	      // if we have a valid terminal number simply return it
	      // 
	      if( terminalNumber >= 0 )
	      {
		      return terminalNumber;
	      }
      	
	      //
	      // the terminal number has not been initialized - get the value from the registry
	      //
         LONG     lRC;
         DWORD dwDataSize;
         TCHAR* TerminalNumber=_T("TerminalNumber");
         TCHAR szTerminalNumber[4]=_T("001");    // 3-digit number as file suffix
          
         dwDataSize= sizeof(szTerminalNumber);
         lRC = RegQueryValueEx(hKey, TerminalNumber, NULL, NULL, 
            (LPBYTE)szTerminalNumber, &dwDataSize);
          
	      if( !lRC )
	      {
		      TCHAR *ptmp;
		      return _tcstol(szTerminalNumber, &ptmp, 10);
	      }
	      else
	      {
		      trace(DCM_ERROR, _T("Failed to retrieve the TerminalNumber specified in the registry."));
            RegCloseKey(hKey);
            return -1;
         }
      }

      /*************************************************************************
      Reads in the base or customer (depending on parameter) option file name 
      from the registry, and returns full path info of option file name
      *************************************************************************/
      CString GetOptionFileName(BOOL bCustFile)
      {
         Trace trace(_T("GetOptionFileName"));

         trace(DCM_INFO, _T("Retrieving Option filename."));
         // Read terminal number from registry
         LONG     lRC;
         HKEY     hKey;
         int nTerminalNumber;
         //LPCSTR   szFilePath;
         DWORD dwDataSize;
         TCHAR custfilename[20]; // allow longer filename
         TCHAR* CustOptionFile=_T("CustomerOptionFile");
         TCHAR  basefilename[20]; 
         TCHAR* BaseOptionFile=_T("BaseOptionFile");
         CString csOptionFileName;
      	
	      // Get Handle to Base TB Registry Key - Base Options stored here
         if (lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTTB"), 
            0, KEY_QUERY_VALUE, &hKey))
	      {
            return _T("");
	      }
          
         nTerminalNumber = DetermineTerminalNumber(hKey);
         if(nTerminalNumber < 0)
         {
            return _T("");
         }
          
         if(bCustFile)
         {
            dwDataSize= sizeof(custfilename);
            lRC = RegQueryValueEx(hKey, CustOptionFile, NULL, NULL, 
                  (LPBYTE)custfilename, &dwDataSize);
            if(lRC)
            {
			      _ASSERT(FALSE);
                  RegCloseKey(hKey);
                  return _T("");
            }
            csOptionFileName.Format(_T("%s\\%s.%03d"),_T("C:\\SCOT\\CONFIG"),
                  custfilename, nTerminalNumber);   
         }
         else
         {
            dwDataSize= sizeof(basefilename);
            lRC = RegQueryValueEx(hKey, BaseOptionFile, NULL, NULL, 
                  (LPBYTE)basefilename, &dwDataSize);
            if(lRC)
            {
			      _ASSERT(FALSE);
                  RegCloseKey(hKey);
                  return _T("");
            }
            csOptionFileName.Format(_T("%s\\%s.%03d"),_T("C:\\SCOT\\CONFIG"),
                  basefilename, nTerminalNumber);
         }
          
         ::RegCloseKey(hKey);
          
         return csOptionFileName;
      }  


      /*************************************************************************
      Loads base options
      *************************************************************************/
      void LoadStandardOptions()
      {
         Trace trace(_T("LoadStandardOptions"));

	     trace(DCM_INFO, _T("Loading Base Options."));
         CreateOptionMap(GetOptionFileName(FALSE));
      }

      /*************************************************************************
      Loads customer options
      *************************************************************************/
      void LoadCustomerOptions()
      {
         Trace trace(_T("LoadCustomerOptions"));

	     trace(DCM_INFO, _T("Loading Customer Options."));
         CreateOptionMap(GetOptionFileName(TRUE));
      }

   }

   void Initialize(long terminalNumberInit)
   {
	   terminalNumber = terminalNumberInit;
      LoadStandardOptions();
      LoadCustomerOptions();
       
	   m_csOptions.Enter();
       
      // TraceWriter default settings
      m_cmDefaults.SetAt(_T("TBTraceSize"),_T("500000"));
      m_cmDefaults.SetAt(_T("TBTraceOn"), _T("Y"));
      m_cmDefaults.SetAt(_T("TBTraceFileName"), _T("c:\\scot\\bin\\TBTrace.log"));
      m_cmDefaults.SetAt(_T("TBTraceMask"), _T("7"));
      //
      m_cmDefaults.SetAt(_T("CashDrawerAtRAP"), _T("Y"));
       
      m_cmDefaults.SetAt(_T("CurrentLevel"), _T("3"));
      m_cmDefaults.SetAt(_T("FocusLevel"), _T("0"));
      m_cmDefaults.SetAt(_T("Cashback1"), _T("Y"));
       
      m_cmDefaults.SetAt(_T("TBUsesPipeServer"), _T("Y"));
      m_cmDefaults.SetAt(_T("PreScotStartDependsOnPostScotStop"), _T("Y"));
      m_cmDefaults.SetAt(_T("ISO639PrimaryLanguageCode"), _T("EN"));
       
      m_cmDefaults.SetAt(_T("TB_FT_TENDER"), _T("Y"));
      m_cmDefaults.SetAt(_T("TB_FT_ITEMSALE"), _T("Y"));
      m_cmDefaults.SetAt(_T("TB_FT_SUSPEND"), _T("Y"));
      m_cmDefaults.SetAt(_T("TB_FT_DEBITACCOUNTTYPE"), _T("Y"));
      m_cmDefaults.SetAt(_T("TB_FT_EXTENDEDDETAILS"), _T("Y"));
      m_cmDefaults.SetAt(_T("TB_FT_TRAININGMODE"), _T("Y"));
      m_cmDefaults.SetAt(_T("TB_FT_QUICKITEMENTRY"), _T("N"));
      m_cmDefaults.SetAt(_T("TB_FT_ENDORSEMENT"), _T("Y"));
      m_cmDefaults.SetAt(_T("TB_FT_SECURITYLEVEL"), _T("Y"));
      m_cmDefaults.SetAt(_T("TB_FT_MICR"), _T("Y"));
      m_cmDefaults.SetAt(_T("TB_FT_CASHDRAWER"), _T("Y"));
      m_cmDefaults.SetAt(_T("TB_FT_AMSTATUS"), _T("Y"));
      m_cmDefaults.SetAt(_T("TB_FT_SUPPORT_PIM"), _T("Y"));
       
	   m_csOptions.Leave();
   }



   BOOL SetOptionValue(LPCTSTR szSectionName, LPCTSTR szOptName, LPCTSTR szOptValue)
   {
      Trace trace(_T("SetOptionValue"));

	   trace(DCM_INFO)
			<< _T("SetOptionValue(") << szSectionName << _T(", ") << szOptName << _T(", ") << szOptValue << _T(")") << NCR::Util::endl;

		BOOL bRet = FALSE;

	   _ASSERT(szSectionName);
	   _ASSERT(szOptName);
	   _ASSERT(szOptValue);
   		
      _ASSERT(_tcslen(szSectionName) && _tcslen(szOptName));
       
      CString strCustFile = GetOptionFileName(TRUE);
       
      // update the new value in the file
      if( WritePrivateProfileString(szSectionName, szOptName, szOptValue, strCustFile) )
      {
         // set the new value in the cached map
		   m_csOptions.Enter();
         m_cmOptionMap.SetAt(szOptName, szOptValue);
         m_csOptions.Leave();
         bRet = TRUE;
      }
       
      return bRet;
   }

   /*************************************************************************
   Retrieve value of option passed as parameter
   *************************************************************************/
   CString OptionValue(LPCTSTR szOptName)
   {
      CString csRetVal(_T(""));
	   m_csOptions.Enter();
      if (!m_cmOptionMap.Lookup(szOptName, csRetVal))
         m_cmDefaults.Lookup(szOptName, csRetVal);
      m_csOptions.Leave();
      return csRetVal;
   }


   /*************************************************************************
   Retrieve value of option passed as parameter
   If the value is not found in the map, then the default value 
   will be returned. Finally if a default value is returned
   and the bUpdateIfNoExist flag is set, then the default value
   will be written out to file.
   *************************************************************************/
   CString OptionValue(LPCTSTR szOptName, LPCTSTR szDefaultValue, BOOL bUpdateIfNoExist)
   {
      Trace trace(_T("OptionValue"), false);

	   CString csRetVal(_T(""));
	   m_csOptions.Enter();
      if (!m_cmOptionMap.Lookup(szOptName, csRetVal))
	   {
         if( !m_cmDefaults.Lookup(szOptName, csRetVal) )
		   {
			   csRetVal = szDefaultValue;
			   if( bUpdateIfNoExist )
			   {
				   if( !SetOptionValue(_T("AutoDefaults"), szOptName, szDefaultValue) )
				   {
					   m_csOptions.Leave();
					   trace(DCM_ERROR)
							<< _T("Failed to update the option file with default entry: ") << szOptName
							<< _T("=") << szDefaultValue << NCR::Util::endl;
				   }
			   }
		   }
	   }

      m_csOptions.Leave();
      return csRetVal;
   }

   /*************************************************************************
   Calls BoolVal, which will return a BOOL value for the option passed in 
   as the parameter
   *************************************************************************/
   BOOL BoolOptionValue(LPCTSTR szOptName)
   {
      return BoolVal(OptionValue(szOptName));
   }

   /*************************************************************************
   Retrieve value of option passed as parameter
   If the value is not found in the map, then the default value 
   will be returned. Finally if a default value is returned
   and the bUpdateIfNoExist flag is set, then the default value
   will be written out to file.
   *************************************************************************/
   BOOL BoolOptionValue(LPCTSTR szOptName, BOOL bDefaultValue, BOOL bUpdateIfNoExist)
   {
      Trace trace(_T("BoolOptionValue"));

	   CString csRetVal(_T(""));
	   m_csOptions.Enter();
      if (!m_cmOptionMap.Lookup(szOptName, csRetVal))
	   {
         if( !m_cmDefaults.Lookup(szOptName, csRetVal) )
		   {
			   csRetVal = bDefaultValue ? _T("1") : _T("0");
			   if( bUpdateIfNoExist )
			   {
				   if( !SetOptionValue(_T("AutoDefaults"), szOptName, csRetVal ) )
				   {
					   m_csOptions.Leave();
					   trace(DCM_ERROR, _T("Failed to update the option file with default entry: %s=%d"), szOptName, bDefaultValue);
				   }
			   }
		   }
	   }

      m_csOptions.Leave();
      return BoolVal(csRetVal);
   }

   /*************************************************************************
   Returns a BOOL value for the option passed in as the parameter
   *************************************************************************/
   BOOL BoolVal(CString strVal)
   {
      strVal.MakeUpper();
	   return ((strVal == _T("Y")) || (strVal == _T("T")) || (strVal == _T("1")) ||
	  	      (strVal == _T("YES")) || (strVal == _T("TRUE"))
			   );
   }

   /*************************************************************************
   Returns an int value for the option passed in as the parameter
   *************************************************************************/
   long LongVal(LPCTSTR szVal)
   {
      TCHAR *ptmp;
      return _tcstol(szVal, &ptmp, 10);
   }

   /*************************************************************************
   Returns an int value for the option passed in as the parameter.
   _tstof not compiling - function not unicode compliant
   *************************************************************************/
   float FloatVal(LPCTSTR szVal)
   {
      return (float)_tstof(szVal);
   }
}
