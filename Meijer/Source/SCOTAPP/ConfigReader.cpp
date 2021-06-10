//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  ConfigReader.cpp
//
// TITLE:
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifndef _CPPUNIT
  #include "Common.h" 
  #include "PSConfigData.h"
#endif

#include "ConfigReader.h"

#define COMP_ID ID_CO
#define T_ID _T("CR")

#define UNSET_OPTVALUE _T("?")

int ConfigReader::nTerminalNo = 999;

////////////////////////////
// Constructor
////////////////////////////
ConfigReader::ConfigReader()
:m_pApp(NULL)
{

}

////////////////////////////
// Destructor
////////////////////////////
ConfigReader::~ConfigReader()
{
#ifndef _SCOTSSF_
    if(m_pApp != NULL)
    {
        delete m_pApp;
        m_pApp = NULL;
    }
#endif
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      long ConfigReader::Initialize
//
// Purpose:     Initialize ConfigReader object
//
// Parameters:  const int nTerminal - the terminal number of the lane
//
// Returns:     long
//
/////////////////////////////////////////////////////////////////////////////////////
long ConfigReader::Initialize(const int nTerminal)
{
    trace(L6, _T("+ConfigReader::Initialize(): Terminal Number=%d"), nTerminal);

    GetWinApp();
    SetTerminalNo(nTerminal);

    trace(L6, _T("-ConfigReader::Initialize()"));

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      void ConfigReader::SetTerminalNo
//
// Purpose:     Sets the value of terminal number variable
//
// Parameters:  int nTerminal - the terminal number of the lane
//
// Returns:     void
//
/////////////////////////////////////////////////////////////////////////////////////
void ConfigReader::SetTerminalNo(int nValue)
{
    nTerminalNo = nValue;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      WINAPP* ConfigReader::GetWinApp
//
// Purpose:     Retrieves the scot windows app
//
// Parameters:  void
//
// Returns:     WINAPP*  - the scot windows app
//
/////////////////////////////////////////////////////////////////////////////////////
WINAPP* ConfigReader::GetWinApp()
{
    if(m_pApp == NULL)
    {
        #ifdef _SCOTSSF_
            m_pApp = AfxGetApp();
        #else 
            #ifndef _CPPUNIT
                m_pApp = new CScotWinApp();
            #else
                m_pApp = new CFakeScotWinApp();
            #endif
        #endif
    }

    return m_pApp;
}


/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      int ConfigReader::GetTerminalNo
//
// Purpose:     Retrieves the terminal number 
//
// Parameters:  void
//
// Returns:     int  - the terminal number
//
/////////////////////////////////////////////////////////////////////////////////////
int ConfigReader::GetTerminalNo()
{
    return nTerminalNo;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      bool ConfigReader::GetFlagOption
//
// Purpose:     The method to call to retrieve the value of a flag option from either the config file (first time retrieval)
//              or from the map  (if options was already loaded from config file)
//
// Parameters:  CString csSection  - the Section in the config file where the option belongs
//              CString csOption   - the Option name in the config file
//              bool fDefaultVal   - used as default value of the option if not found on the file and map
//              CString csFileName - the filename of the config file, can either a (.dat) extension or none
//                                   this will be used to look through csFileName.dat, csFileName.000 and csFileName.XYZ files
//
// Returns:     bool - the value of the option
//
/////////////////////////////////////////////////////////////////////////////////////
bool ConfigReader::GetFlagOption(CString csSection, CString csOption, bool fDefaultVal, CString csFileName)
{
    trace(L6, _T("+ConfigReader::GetFlagOption()"));

    bool fValue = false;

    CString csValue = GetStringOption(csSection, csOption, UNSET_OPTVALUE, csFileName);

    if(csValue==UNSET_OPTVALUE)
    {
        fValue = fDefaultVal;
    }
    else
    {
        csValue.MakeUpper();
        
        if(csValue.Left(1)==_T("1"))
        {
            fValue = true;
        }
        else if(csValue.Left(1)==_T("Y"))
        {
            fValue = true;
        }
    }

    trace(L6, _T("-ConfigReader::GetFlagOption() returns [%s]%s = %d"), csSection, csOption, fValue);
    
    return fValue;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      int ConfigReader::GetIntOption
//
// Purpose:     The method to call to retrieve the value of an option with an integer value from either the config file (first time retrieval)
//              or from the map  (if options was already loaded from config file)
//
// Parameters:  CString csSection  - the Section in the config file where the option belongs
//              CString csOption   - the Option name in the config file
//              int nDefaultVal    - used as default value of the option if not found on the file and map
//              CString csFileName - the filename of the config file, can either a (.dat) extension or none
//                                   this will be used to look through csFileName.dat, csFileName.000 and csFileName.XYZ files
//
// Returns:     int - the value of the option
//
/////////////////////////////////////////////////////////////////////////////////////
int ConfigReader::GetIntOption(CString csSection, CString csOption, int nDefaultVal, CString csFileName)
{
    trace(L6, _T("+ConfigReader::GetIntOption()"));

    int nValue = -99999;

    CString csValue = GetStringOption(csSection, csOption, UNSET_OPTVALUE, csFileName);

    if(csValue==UNSET_OPTVALUE)
    {
        nValue = nDefaultVal;
    }
    else
    {
        nValue = _ttoi(csValue);
    }

    trace(L6, _T("-ConfigReader::GetIntOption() returns [%s]%s = %d"), csSection, csOption, nValue);
    
    return nValue;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      CString ConfigReader::GetStringOption
//
// Purpose:     The method to call to retrieve the value of an option with a string value from either the config file (first time retrieval)
//              or from the map  (if options was already loaded from config file)
//
// Parameters:  CString csSection  - the Section in the config file where the option belongs
//              CString csOption   - the Option name in the config file
//              CString csDefaultVal  - used as default value of the option if not found on the file and map
//              CString csFileName - the filename of the config file, can either a (.dat) extension or none
//                                   this will be used to look through csFileName.dat, csFileName.000 and csFileName.XYZ files
//
// Returns:     CString - the value of the option
//
/////////////////////////////////////////////////////////////////////////////////////
CString ConfigReader::GetStringOption(CString csSection, CString csOption, CString csDefaultVal, CString csFileName)
{
    trace(L6, _T("+ConfigReader::GetStringOption()"));

    CString csValue(UNSET_OPTVALUE);

    CString csSecOptKey = ConcatSectionOptionStrings(csSection, csOption);

    LPCTSTR szKey = (LPCTSTR) csSecOptKey;

    if(!SearchAndSetValueFromMap(szKey, csValue))
    {   
        trace(L6, _T("Option not found in map."));

        csValue = GetStringFromFile( m_pApp, csSection, csOption, csDefaultVal, csFileName);
        AddOptionToMap(szKey, csValue);

    }
    else
    {
        trace(L6, _T("Option found in map."));
    }
    
    trace(L6, _T("-ConfigReader::GetStringOption() returns [%s]%s = %s"), csSection, csOption, csValue );

    return csValue;

}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      CString ConfigReader::GetStringFromFile
//
// Purpose:     The method to call to retrieve option value from file, 
//              it uses the csFileName parameter to look through a .dat config file including it's .000 or .XYZ (TerminalNo) overrides.
//
// Parameters:  WinApp - the scot win application
//              CString csSec  - the Section in the config file where the option belongs
//              CString csOpt   - the Option name in the config file
//              CString csDefault    - used as default value of the option if not found on the config files and map
//              CString csFileName - the filename of the config file, can either a (.dat) extension or none
//                                   this will be used to look through csFileName.dat, csFileName.000 and csFileName.XYZ files
//
// Returns:     CString - the value of the option
//
/////////////////////////////////////////////////////////////////////////////////////
CString ConfigReader::GetStringFromFile(WINAPP* pApp, CString csSec, CString csOpt, CString csDefault, CString csFileName)
{
    trace(L7, _T("+ConfigReader::GetScotStringOptionValue()"));

    CString csOptionValue = csDefault;

    //filename should not be empty or does not start with a space
    if(csFileName.IsEmpty() || (csFileName.SpanIncluding(_T(" ")) != _T("")))
    {
        trace(L6, _T("Empty filename or starts with a space"));
    }
    else
    {
        LPCTSTR pszProfileName = pApp->GETPROFILENAME;
        
        CString csFileName000;
        CString csFileNameXYZ;
        bool isScotOpt= false;

        csFileName.MakeUpper();

        if(csFileName.Find(_T("SCOTOPTS")) != -1 )
        {
            isScotOpt = true;
        }

        //if filename is not scotopt and has extension
        if(!isScotOpt && (csFileName.Find(_T(".")) != -1))
        {
            csFileName.Format(co.csConfigFilePath + _T("\\") + _T("") + csFileName);

            if (STATE(State)::OpenSCOTFile(csFileName, false, false))
            {
                trace(L6, _T("Search option on %s"), csFileName );

                pApp->SETPROFILENAME(csFileName);
                csOptionValue = GetStringValue(pApp,(LPCTSTR) csSec, (LPCTSTR) csOpt, csOptionValue, true);
            }

        }
        else
        {
            if(isScotOpt)
            {
                csFileName.Format(co.csConfigFilePath + _T("\\") + SCOTOPTS + _T(".DAT"));
                csFileName000.Format(co.csConfigFilePath + _T("\\") + SCOTOPTS + _T(".%03d"),0);
                csFileNameXYZ.Format(co.csConfigFilePath + _T("\\") + SCOTOPTS + _T(".%03d"), GetTerminalNo());
            }
            else
            {
                CString csFile = csFileName;

                csFileName.Format(co.csConfigFilePath + (_T("\\")) + csFileName + _T(".DAT"));
                csFileName000.Format(co.csConfigFilePath + (_T("\\")) + csFile + _T(".%03d"),0);
                csFileNameXYZ.Format(co.csConfigFilePath + (_T("\\")) + csFile + _T(".%03d"), GetTerminalNo());
            }

            if (STATE(State)::OpenSCOTFile(csFileName, true, true))
            {
                trace(L6, _T("Search option on %s"), csFileName );

                pApp->SETPROFILENAME(csFileName);
                csOptionValue = GetStringValue(pApp,(LPCTSTR) csSec, (LPCTSTR) csOpt, csOptionValue, true);
            }

            if (STATE(State)::OpenSCOTFile(csFileName000, false, false))
            {
                trace(L6, _T("Search option on %s"), csFileName000 );

                pApp->SETPROFILENAME(csFileName000);
                csOptionValue = GetStringValue(pApp,(LPCTSTR) csSec, (LPCTSTR) csOpt, csOptionValue, true);
            }

            if (STATE(State)::OpenSCOTFile(csFileNameXYZ, false, false))
            {
                trace(L6, _T("Search option on %s"), csFileNameXYZ );

                pApp->SETPROFILENAME(csFileNameXYZ);
                csOptionValue = GetStringValue(pApp,(LPCTSTR) csSec, (LPCTSTR) csOpt, csOptionValue, true);
            }
        }

        pApp->SETPROFILENAME(pszProfileName);
    }


    trace(L7, _T("-ConfigReader::GetScotStringOptionValue() returns: %s"), csOptionValue);

    return csOptionValue;

}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      bool ConfigReader::GetStringValue
//
// Purpose:     This method retrieves the string value of an option under the section specified from the config file
//
// Parameters:  WinApp - the scot win application
//              LPCTSTR szSection  - the Section in the config file where the option belongs
//              LPCTSTR szOption   - the Option name in the config file
//              CString csDefault  - used as default value of the option if not found on the config files and map
//              const bool fErrorLog - flag to indicate if to log a trace when the option is not found on the config file
//
// Returns:     CString csRet - the string value of the option
//
/////////////////////////////////////////////////////////////////////////////////////
CString ConfigReader::GetStringValue(WINAPP* pApp, LPCTSTR szSection, LPCTSTR szOption, CString csDefault, const bool fErrorLog)
{
    trace(L7, _T("+ConfigReader::GetStringValue()"));

    CString csRet(UNSET_OPTVALUE);

    if(pApp)
    {
        csRet = pApp->GetProfileString(szSection, szOption, UNSET_OPTVALUE);
    }
    if(csRet == UNSET_OPTVALUE)
    {
        if(fErrorLog)
        {
            trace(L6, _T("Config option not found for [%s]-%s, using default = %s"), szSection, szOption, csDefault);
        }       

        csRet = csDefault;
    }

    trace(L6, _T("-ConfigReader::GetStringValue() returns: %s"), csRet);

    return csRet;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      bool ConfigReader::LoadSectionOptionsToMap
//
// Purpose:     The method loads all the options and its values under a section in the config file
//              each option and value pair is then saved to the map if not found on the option map
//
// Parameters:  CString csFileName   - the config file to read, if scotopts it will read through .dat and its overrides (.000 & .Ter#)
//                                     if not scotopts & without extension specified, it will read through csFilename.dat and its overrides (.000 & .Ter#)
//                                     if not scotopts & extension is specified, it will read through that specific file alone
//
//              CString csSection  - the section name in the config file
//
// Returns:     void
//
/////////////////////////////////////////////////////////////////////////////////////
void ConfigReader::LoadSectionOptionsToMap(CString csFileName, CString csSection) 
{
    trace(L6, _T("+ConfigReader::LoadSectionOptionsToMap()"));

    //filename should not be empty or does not start with a space
    if(csFileName.IsEmpty() || (csFileName.SpanIncluding(_T(" ")) != _T("")))
    {
        trace(L6, _T("Empty filename or starts with a space"));
    }
    else
    {
        CString csFileName000;
        CString csFileNameXYZ;

        bool isScotOpt= false;

        csFileName.MakeUpper();

        if(csFileName.Find(_T("SCOTOPTS")) != -1 )
        {
            isScotOpt = true;
        }

        //if filename is not scotopt and has extension
        if(!isScotOpt && (csFileName.Find(_T(".")) != -1))
        {
            csFileName.Format(co.csConfigFilePath + _T("\\") + _T("") + csFileName);

            if (STATE(State)::OpenSCOTFile(csFileName, false, false))
            {
                trace(L6, _T("Loading section options from %s ..."), csFileName );
                RetrieveSectionOptions(csFileName, csSection);
            }

        }
        else
        {
            if(isScotOpt)
            {
                csFileName.Format(co.csConfigFilePath + _T("\\") + SCOTOPTS + _T(".DAT"));
                csFileName000.Format(co.csConfigFilePath + _T("\\") + SCOTOPTS + _T(".%03d"),0);
                csFileNameXYZ.Format(co.csConfigFilePath + _T("\\") + SCOTOPTS + _T(".%03d"), GetTerminalNo());
            }
            else
            {
                CString csFile = csFileName;

                csFileName.Format(co.csConfigFilePath + (_T("\\")) + csFileName + _T(".DAT"));
                csFileName000.Format(co.csConfigFilePath + (_T("\\")) + csFile + _T(".%03d"),0);
                csFileNameXYZ.Format(co.csConfigFilePath + (_T("\\")) + csFile + _T(".%03d"), GetTerminalNo());
            }

            if (STATE(State)::OpenSCOTFile(csFileName, true, true))
            {
                trace(L6, _T("Loading section options from %s ..."), csFileName );
                RetrieveSectionOptions(csFileName, csSection);
            }

            if (STATE(State)::OpenSCOTFile(csFileName000, false, false))
            {
                trace(L6, _T("Loading section options from %s ..."), csFileName000 );
                RetrieveSectionOptions(csFileName000, csSection);
            }

            if (STATE(State)::OpenSCOTFile(csFileNameXYZ, false, false))
            {
                trace(L6, _T("Loading section options from %s ..."), csFileNameXYZ );
                RetrieveSectionOptions(csFileNameXYZ, csSection);
            }
        }
    }

    trace(L6, _T("-ConfigReader::LoadSectionOptionsToMap()"));

}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      void ConfigReader::RetrieveSectionOptions
//
// Purpose:     This retrieves the options within the section of a specified configuration file
//              
//
// Parameters:  CString csSource  - the config file with complete file path 
//              CString csSection - the section name 
//
// Returns:     void
//
/////////////////////////////////////////////////////////////////////////////////////
void ConfigReader::RetrieveSectionOptions(CString csSource, CString csSection)
{
    trace(L7, _T("+ConfigReader::RetrieveSectionOptions()"));

#ifndef _CPPUNIT
    CPSConfigData pscd;
#else
    CFakePSConfigData pscd;
#endif

    CString csValue;
    CString csOption;

    pscd.SetSource(csSource);

    int index = 0;

    while(pscd.EnumSectionValues(csSection, csOption, index++))
    {       
        pscd.GetString(csSection, csOption, csValue, _T(""));

        trace(L6, _T("csSection=%s, csOption=%s, csValue=%s"), csSection, csOption, csValue);

        CString csSecOptKey = ConcatSectionOptionStrings(csSection, csOption);

        LPCTSTR szKey = (LPCTSTR) csSecOptKey;

        AddOptionToMap(szKey, csValue);

    }

    trace(L7, _T("-ConfigReader::RetrieveSectionOptions()"));
}


/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      bool ConfigReader::ConcatSectionOptionStrings
//
// Purpose:     The method concatenates a section name with an option name
//              The format sectionName_optionName
//
// Parameters:  CString csSec   - the section name 
//              CString csOpt   - the option name 
//
// Returns:     CString csRet   - the merged string separated by "_"
//
/////////////////////////////////////////////////////////////////////////////////////
CString ConfigReader::ConcatSectionOptionStrings(CString csSec, CString csOpt)
{
    trace(L7, _T("+ConfigReader::ConcatSectionOptionStrings()"));

    CString csRet = _T("");

    csRet = csSec + _T("_") + csOpt;

    trace(L6, _T("-ConfigReader::ConcatSectionOptionStrings() returns = %s"), csRet);

    return csRet;
}


/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      bool ConfigReader::SearchAndSetValueFromMap
//
// Purpose:     The method checks if an option is included in the options_map,
//              if included, it sets the csVal parameter to its value
//
// Parameters:  LPCTSTR szKey   - the key which is sectionName_optionName 
//              CString& csVal  - reference to the string variable to hold the option's value
//
// Returns:     BOOL - returns true if option and its value is already mapped, false if not
//
/////////////////////////////////////////////////////////////////////////////////////
BOOL ConfigReader::SearchAndSetValueFromMap(LPCTSTR szKey, CString& csVal)
{
    trace(L7, _T("+ConfigReader::SearchAndSetValueFromMap()"));

    BOOL bRet;

    bRet = options_map.Lookup(szKey,csVal);
    
    trace(L7, _T("-ConfigReader::SearchAndSetValueFromMap(): Is option found? = %s"), bRet? "Yes": "No");

    return bRet;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      void ConfigReader::AddOptionToMap
//
// Purpose:     The method add the option-value pair in the map
//
// Parameters:  LPCTSTR szKey   - the key which is sectionName_optionName 
//              CString csValue - the value of the option
//
// Returns:     void
//
/////////////////////////////////////////////////////////////////////////////////////
void ConfigReader::AddOptionToMap(LPCTSTR szKey, CString csValue)
{
    trace(L6, _T("+ConfigReader::AddOptionToMap()"));

    options_map[szKey] = csValue;

    trace(L6, _T("-ConfigReader::AddOptionToMap()"));
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      bool ConfigReader::ClearOptionsMap
//
// Purpose:     The method removes all entries of the map (empties it)
//
// Parameters:  void
//
// Returns:     BOOL - returns true if map is empty, false if not
//
/////////////////////////////////////////////////////////////////////////////////////
BOOL ConfigReader::ClearOptionsMap()
{
    BOOL fRet; 

    trace(L6, _T("+ConfigReader::ClearOptionsMap()"));

    options_map.RemoveAll();

    fRet = options_map.IsEmpty();

    trace(L6, _T("-ConfigReader::ClearOptionsMap() returns = %d"), fRet);

    return fRet;
}
