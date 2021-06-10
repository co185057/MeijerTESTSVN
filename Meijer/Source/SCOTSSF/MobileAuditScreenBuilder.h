// MobileAuditScreenBuilder.h: interface for the CMobileAuditScreenBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOBILEAUDITSCREENBUILDER_H__DAC51D78_6410_4F9D_9F91_D4673FB7088D__INCLUDED_)
#define AFX_MOBILEAUDITSCREENBUILDER_H__DAC51D78_6410_4F9D_9F91_D4673FB7088D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
CMobileAuditScreenBuilder  
{
public:
	CMobileAuditScreenBuilder();
	virtual ~CMobileAuditScreenBuilder();

    void GetPartialAuditScreenData(_bstr_t a_bstrtStatus,
                                   _bstr_t a_bstrtResult,
                                   _bstr_t & a_bstrtSection,
                                   _bstr_t & a_bstrtMessage);

    void GetPartialAuditErrorScreenData(_bstr_t a_bstrtStatus,
                                        _bstr_t a_bstrtResult,
                                        _bstr_t & a_bstrtSection,
                                        _bstr_t & a_bstrtMessage);

    void GetFullAuditErrorScreenData(_bstr_t a_bstrtStatus,
                                        _bstr_t a_bstrtResult,
                                        _bstr_t & a_bstrtSection,
                                        _bstr_t & a_bstrtMessage);

    void GetErrorScreenData(_bstr_t a_bstrtStatus,
                                        _bstr_t a_bstrtResult,
                                        _bstr_t & a_bstrtSection,
                                        _bstr_t & a_bstrtMessage);

    void GetFullAuditScreenData(_bstr_t a_bstrtStatus,
                                _bstr_t a_bstrtResult,
                                _bstr_t & a_bstrtSection,
                                _bstr_t & a_bstrtMessage);

    void GetEndAuditScreenData(_bstr_t a_bstrtStatus,
                               _bstr_t a_bstrtResult,
                               _bstr_t & a_bstrtSection,
                               _bstr_t & a_bstrtMessage);

    void GetExitAuditScreenData(_bstr_t a_bstrStatus,
                                _bstr_t a_bstrtResult,
                                _bstr_t & a_bstrtSection,
                                _bstr_t & a_bstrtMessage);
    
    void GetAbortAuditSureScreenData(_bstr_t a_bstrStatus,
                                     _bstr_t a_bstrtResult,
                                     _bstr_t & a_bstrtSection,
                                     _bstr_t & a_bstrtMessage);

                           

protected:

    _bstr_t GetPartialRescanScanMoreItemsString();
    _bstr_t GetPartialRescanScanOrFinishString();
    _bstr_t GetPartialRescanErrorOrFullString();
    _bstr_t GetFullRescanErrorString();
    _bstr_t GetErrorString();
    _bstr_t GetPartialAuditContinueString();
    _bstr_t GetFullRescanScanMoreItemsString();
    _bstr_t GetEndAuditString();
    _bstr_t GetExitingAuditString();
};

#endif // !defined(AFX_MOBILEAUDITSCREENBUILDER_H__DAC51D78_6410_4F9D_9F91_D4673FB7088D__INCLUDED_)
