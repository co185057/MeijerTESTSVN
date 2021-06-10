// DataNeededUtil.h: interface for the CDataNeededUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATANEEDEDUTIL_H__39AF22D2_5881_4D21_8553_A08C8DE69F88__INCLUDED_)
#define AFX_DATANEEDEDUTIL_H__39AF22D2_5881_4D21_8553_A08C8DE69F88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TransBroker.h"
#include "SMStateConstants.h"

#ifndef _CPPUNIT
#include "DllDefine.h"
#else
#include "TestMacros.h"
#include "FakeSCOTDataEntry.h"
#endif

//const enum SCOT_LANGUAGE_TYPE;
class SCOTDATAENTRY;

#ifdef _SCOTSSF_
class DLLIMPORT
#elif defined _SCOT
class DLLEXPORT
#else
class 
#endif 
CDataNeededUtil  
{
public:
    
    CDataNeededUtil(CString csDescription, 
                    CString csView,
                    CString csIniFile,
                    CString csSection );
    CDataNeededUtil(CString csDescription,
                    CString csView);
    CDataNeededUtil(CString csView);
    virtual ~CDataNeededUtil();
    
    virtual CString GetDescription();
    virtual CString GetView();
    virtual CString GetINIFile();
    virtual CString GetSection();
    virtual CString GetViewString();
    virtual CString GetViewStringFromINI();
    virtual CString GetString(CString csOptionString);
    virtual bool GetInt(CString csOptionString, int &iValue);
    virtual bool GetBool(CString csOptionString, bool &bValue);
    virtual CString GetViewStringTranslated(bool bStoreMode);
    virtual SCOTDATAENTRY* GetScotDataEntryObject();
    
protected:
    virtual CString GetViewFromFile(CString csIniFile, CString csSection);
    
    CString m_csSection;
    CString m_csIniFile;
    CString m_csView;
    CString m_csDescription;
    
    CString m_csViewString;                  ///Combined INI options and sView options
    CString m_csViewStringINI;               ///Options from INI file
    SCOTDATAENTRY m_scotDataEntry;
};

#endif // !defined(AFX_DATANEEDEDUTIL_H__39AF22D2_5881_4D21_8553_A08C8DE69F88__INCLUDED_)
