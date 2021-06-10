#pragma once
#include "tbmsgobj.h"

#import <msxml4.dll>
using namespace MSXML2;

#define VERSION_GENOBJ_BIN 3
#define VERSION_GENOBJ_XML 4

class CTBGenMsg :
	public CTBMsgObj
{
public:
    typedef enum { 
        RESET_FIELD_DATA = 0,
        RESET_XML_DOC  = 1
    } ResetOptions;

    static _bstr_t m_bstrTBControlsXML;
    static bool m_bInitializedOnce;

	//CTBGenMsg();
	CTBGenMsg(BYTE msgId=0, bool bXML =false, bool bValidate =false);
	virtual ~CTBGenMsg(void);

	virtual void SetTBMsgId(BYTE tbMsgId);
	virtual int getLength(void);
	virtual void Parse(BYTE *pBuf);
	virtual BYTE* Serialize(int &byteLength);

public:
	BYTE m_byCustomMessageType;
	void Load();
	void SetValue(LPCTSTR szName, _variant_t vValue);
	//void SetValue(BSTR bstr, _variant_t vValue);
	_variant_t GetValue(LPCTSTR szName);
	_variant_t GetValue2(LPCTSTR szName); //wrapper to catch exceptions
	//_variant_t GetValue(BSTR szName);
	VARTYPE GetVtTypeFromStringType(LPCWSTR szwType);
	virtual void Reset(ResetOptions option);
	_bstr_t getXML();
	void setXML(BSTR bstrXml);
	void SetXMLOutPut(bool bXML);
	bool GetXMLOutPut();
	void SetValidate(bool bValidate);
	bool GetValidate();
	void SetXMLOutPutRaw(bool bXML);
	void SetValidateRaw(bool bValidate);
	void SetTBMsgIdRaw(BYTE tbMsgId);
    void SetName(_bstr_t bstrName);
    CString GetName();

protected:
	void* m_pData;
	int m_nDataSize;
	//bool m_bSerializeOutputAsXML;
    CString m_sName;
    void LoadIfNeeded();
    void ClearAll();

	MSXML2::IXMLDOMDocument2Ptr m_pMsgDoc;
	MSXML2::IXMLDOMElementPtr m_pFields;
	MSXML2::IXMLDOMElementPtr m_pMessageNode;
	bool m_bXmlLoaded;
	bool m_bValidate;
private:
	void CreateXMLDoc();
   void LoadRegistryOptions();
};
