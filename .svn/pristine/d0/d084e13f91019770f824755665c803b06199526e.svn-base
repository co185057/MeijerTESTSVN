//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    MobileCartParser.h
//
// TITLE:   Parses Revision Mobile Cart XML
//
// AUTHOR:  Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// MobileCartParser
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CMobileCartParserBase
#define _CMobileCartParserBase

#ifndef _CPPUNIT
#include <map>
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include "Common.h"
#include "DllDefine.h"
#else
#include "FakeCommon.h"
#endif

typedef struct _MobileIDFilter 
{
    CString         csFilterType;                  //in cm
    CString         csFilterString;                //Name
 } MOBILEIDFILTER, *LPMOBILEIDFILTER;

typedef CMap<CString, LPCTSTR, CString, LPCTSTR> MobileBarcodeConversionMap;

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
 CMobileCartParserBase : public CObject

{
public:
    CMobileCartParserBase();
    ~CMobileCartParserBase();

    virtual long Initialize();
    virtual void UnInitialize();
    virtual void Reset();

    virtual void SendItemSold(ITEMDETAILS &itemDetails, bool bRecalledItem);
    virtual void SetTotalDue(long lTotal);
    virtual void SetPOSTransactioNum(int nTransactionNumber);
    virtual _bstr_t GetFinishRequest();
    virtual void GetItems(int &nValidItems, CString &str);
    virtual CString ProcessCart(_bstr_t bstrCartXML);

    // Not yet sure if item by item messages will be sent after cart transfer
    // This method will be used to record item details without message send
    virtual _bstr_t updateItemMap(long id, 
                          bool bVoid, 
                          const CString &upc,
                          long lextendedPrice, 
                          const CString &sDesc,
                          bool bRecalledItem
                          );
    

    virtual void SetSendCustomerIDAsScan(bool bSendCustomerIDAsScan);
    virtual bool SetCustomerIDFilter(int nIndex, CString csFilter);
    virtual bool IsValidCustomerID(CString csCustomerID);

    virtual void AddBarCodeConversionOptionToMap(LPCTSTR szKey, CString csValue);
    virtual bool ClearBarCodeConversionOptionMap();

    void (__stdcall *m_fReturnCart)(CString csCart);
    virtual void CopyNode(MSXML2::IXMLDOMDocumentPtr pDoc, MSXML2::IXMLDOMNodePtr pContainer, MSXML2::IXMLDOMNodePtr pNewContainer, LPCTSTR sNodeName, LPCTSTR sNodeValue);
    CString UpdateNode(CString csXML, CString csNodeName, CString csNodeValue);
    CString UpdateNode(CString csXML, CString csNodeName, long lNodeValue);
//#endif
    _bstr_t m_bstrCartFile;
protected:

    MSXML2::IXMLDOMDocumentPtr pCartDocument;
    MSXML2::IXMLDOMDocumentFragmentPtr pCartItemGroup;

    // Sync object if required
    CCriticalSection m_CriticalRequest;

    virtual _bstr_t CMobileCartParserBase::serializeItemMap();
    virtual bool IsFilterMatch(CString csFilterString, CString csCompareString);

    // Unique SSCO cart identifier
    _bstr_t     m_bstrCartID;

    bool m_bInitialized;
    long m_lTotalDue;
    int m_nTransactionNum;

    bool m_bSendCustomerIDAsScan;
    CArray<LPMOBILEIDFILTER,LPMOBILEIDFILTER> caIDFilters;

    MobileBarcodeConversionMap ca_map;

    DECLARE_DYNCREATE(CMobileCartParserBase)// MFC Runtime class/object information
};
#endif

