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
#ifndef _CMobileCartParser
#define _CMobileCartParser

#include "MobileCartParserBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 CMobileCartParser : public CMobileCartParserBase

{
public:
    CMobileCartParser();
    ~CMobileCartParser();

    virtual long Initialize();
    virtual void UnInitialize();
    virtual void Reset();

    virtual void SendItemSold(ItemDetails &itemDetails, bool bRecalledItem);
    virtual void SetTotalDue(long lTotal);
    virtual void SetPOSTransactioNum(int nTransactionNumber);
    virtual _bstr_t GetFinishRequest();
    virtual CString ProcessCart(_bstr_t bstrCartXML);
    virtual _bstr_t updateItemMap(long id, 
                          bool bVoid, 
                          const CString &upc,
                          long lextendedPrice, 
                          const CString &sDesc,
                          bool bRecalledItem
                          );
    void GetItems(int &nValidItems);
    _bstr_t buildItemList();
    
    void SetSendCustomerIDAsScan(bool bSendCustomerIDAsScan);
    bool SetCustomerIDFilter(int nIndex, CString csFilter);
    bool IsValidCustomerID(CString csCustomerID);

    void AddBarCodeConversionOptionToMap(LPCTSTR szKey, CString csValue);
    bool ClearBarCodeConversionOptionMap();

    void (__stdcall *m_fReturnCart)(CString csCart);
    CString UpdateNode(CString csXML, CString csNodeName, CString csNodeValue);
    CString UpdateNode(CString csXML, CString csNodeName, long lNodeValue);

    int GetNumRussiaEgaisItems() {return m_nRussiaEgaisItems;};
    int CMobileCartParser::GetAuditItemList( int & nPositionNum, bstr_t & bstrAuditItems ); 

protected:
    _bstr_t CMobileCartParser::serializeItemMap();

    // Sync object if required
    CCriticalSection m_CriticalRequest;

    bool IsFilterMatch(CString csFilterString, CString csCompareString);
    long GetRescanCode(MSXML2::IXMLDOMNodePtr pItem);
    bool IsEgaisItem(MSXML2::IXMLDOMNodePtr pItem);

    long m_lVoidedItems;
    long m_lNumberOfItems;
    int m_nRussiaEgaisItems; //RussiaAlcohol EGAIS Items

    DECLARE_DYNCREATE(CMobileCartParser)// MFC Runtime class/object information
};
#endif

