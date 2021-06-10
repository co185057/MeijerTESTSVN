// DataNeededUtil.cpp: implementation of the CDataNeededUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataNeededUtil.h"
#include "ConfigObjectConstants.h"
#include "PSConfigData.h"

#define COMP_ID ID_SM
#define T_ID _T("DNUtil")

/*
 *  Utility class for handling Dataneeded information
 */

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataNeededUtil::CDataNeededUtil(CString csDescription, 
                                 CString csView,
                                 CString csIniFile,
                                 CString csSection)
{
    m_csDescription = csDescription;
    m_csView = csView;
    m_csIniFile = csIniFile;
    m_csSection = csSection;
    m_csViewStringINI = GetViewFromFile(csIniFile, csSection);
    m_csViewString = GetViewStringFromINI() + GetView();
    m_scotDataEntry.FromString(m_csViewString);
}
CDataNeededUtil::CDataNeededUtil(CString csDescription,
                                 CString csView)
{
    m_csDescription = csDescription;
    m_csView = csView;
    m_csIniFile = _T("");
    m_csViewStringINI = _T("");
    m_csViewString = csView;
    m_scotDataEntry.FromString(m_csViewString);
}

CDataNeededUtil::CDataNeededUtil(CString csView)
{
    m_csView = csView;
    m_csIniFile = _T("");
    m_csViewStringINI = _T("");
    m_csViewString = csView;
    m_scotDataEntry.FromString(m_csViewString);
    m_csDescription = m_scotDataEntry.GetSummaryDescription();

}

CDataNeededUtil::~CDataNeededUtil()
{

}

CString CDataNeededUtil::GetSection()
{
    return m_csSection;
}

CString CDataNeededUtil::GetINIFile()
{
    return m_csIniFile;
}

CString CDataNeededUtil::GetView()
{
    CString csReturn = m_csView;
    csReturn.TrimRight();
    if (!csReturn.IsEmpty() && (csReturn.ReverseFind(_T(';')) != csReturn.GetLength()-1))
    {
        csReturn += _T(";");
    }
    return csReturn;
}

CString CDataNeededUtil::GetDescription()
{
    return m_csDescription;
}

CString CDataNeededUtil::GetViewFromFile(CString csIniFile, CString csSection)
{
    SCOTDATAENTRY cDataEntry;

    if(!csIniFile.IsEmpty())
    {
        if(!csSection.IsEmpty())
        {   //Dataneeded parameters to be retrieved from INI file
            cDataEntry.FromConfigData(csIniFile, csSection, true, false);
        }
    }
       
    return cDataEntry.ToString();
}

CString CDataNeededUtil::GetViewString()
{
    return m_csViewString;
}

CString CDataNeededUtil::GetViewStringFromINI()
{
    CString csReturn = m_csViewStringINI;
    csReturn.TrimRight();
    if (!csReturn.IsEmpty() && (csReturn.ReverseFind(_T(';')) != csReturn.GetLength()-1))
    {
        csReturn += _T(";");
    }
    return csReturn;
}

CString CDataNeededUtil::GetString(CString csOptionString)
{
    return m_scotDataEntry.ExtractString(csOptionString);
}

bool CDataNeededUtil::GetInt(CString csOptionString, int &iValue)
{
    bool bResult = false;
    CString csTemp = GetString(csOptionString);

    if (!csTemp.IsEmpty())
    {
        iValue = _ttoi((LPCTSTR)csTemp);
        bResult = true;
    }
    return bResult;
}

bool CDataNeededUtil::GetBool(CString csOptionString, bool &bValue)
{
    bool bResult = false;
    CString csTemp = GetString(csOptionString);

    if (!csTemp.IsEmpty())
    {
        if (csTemp.FindOneOf(_T("0nN")) != -1)
        {
            bValue = false;
            bResult = true;
        }
        else if (csTemp.FindOneOf(_T("1yY")) != -1)
        {
            bValue = true;
            bResult = true;
        }
    }
    return bResult;
}

CString CDataNeededUtil::GetViewStringTranslated(bool bStoreMode)
{
    
    CString csRDNLaneMessage, csRDNLaneLeadthru, csRDNValues;  //Strings for Rap Data Needed customer display
    CString csTranslatedViewString = m_csViewString;

    PSCONFIGDATA cd;

    if (csTranslatedViewString.IsEmpty())
        return csTranslatedViewString;

    cd.FormatString(csTranslatedViewString, bStoreMode);


    if (bStoreMode)
    {
        csRDNLaneMessage = m_scotDataEntry.GetRDNLaneMessage();
        csRDNLaneLeadthru = m_scotDataEntry.GetRDNLaneLeadthru();
        cd.FormatString(csRDNLaneMessage, false);
        cd.FormatString(csRDNLaneLeadthru, false);
    }
    
    //Build customer-language messages for RapDataNeeded screen.
    csRDNValues = _T("");
    if (csRDNLaneLeadthru != _T(""))
    {
        csRDNValues += _T("RDNLANELEADTHRU=") + csRDNLaneLeadthru + _T(";");
    }
    if (csRDNLaneMessage != _T(""))
    {
        csRDNValues += _T("RDNLANEMESSAGE=") + csRDNLaneMessage + _T(";");
    }
    
    // RFC 265805 end
    csTranslatedViewString = csTranslatedViewString + csRDNValues;

    return csTranslatedViewString;

}

SCOTDATAENTRY* CDataNeededUtil::GetScotDataEntryObject()
{
    return &m_scotDataEntry;
}