#ifndef _SCOT_DEVICECONTEXT_STORE_
#define _SCOT_DEVICECONTEXT_STORE_

////
//#include <afxdisp.h>
//#include <afxcoll.h>
//#include <afxmt.h>



//#import "c:\\winnt\\system32\\msxml4.dll" 
//using namespace MSXML2;
#include "Common.h"

typedef CMap <CString, LPCTSTR, CString, LPCTSTR> key_value_pair_t;

#define DEVICECONTEXTS_FILE_DEFAULT  "%APP_DRIVE%\\scot\\config\\ContextExtend.xml"

class CDeviceCtxStore
{
public:
	CDeviceCtxStore();
	virtual ~CDeviceCtxStore() = 0;
	
	virtual bool SetCurrentContext(LPCTSTR pszCtxName) = 0;
	virtual CString GetCurrentContext() {
											return m_csContextName;
										};
protected:
	CString m_csContextName;
};


class CDeviceCtxStoreXml : public CDeviceCtxStore
{
public:
	CDeviceCtxStoreXml();
	CDeviceCtxStoreXml(const CString& csFile);
	virtual ~CDeviceCtxStoreXml();
	virtual bool SetFileName(const CString& csFile);
	bool Load();
	bool UnLoad();
	virtual bool SetCurrentContext(LPCTSTR pszCtxName);
	virtual bool GetAllDevicePropertyNames(CStringArray& csPropList);
	virtual bool SetCurrentDeviceProperty(LPCTSTR pszDevicePropName);
	virtual bool GetAllItemsForCurrentDevicePropetry(key_value_pair_t& ItemPairs);
	virtual bool GetDevicePropertyItemValue(LPCTSTR pszDevicePropItemName, CString& csDevicePropItemValue);
	virtual bool SetDevicePropertyItemValue(LPCTSTR pszDevicePropItemName, LPCTSTR pszDevicePropItemValue);


private:
	MSXML2::IXMLDOMDocumentPtr m_spDoc;			// main doc object
	MSXML2::IXMLDOMNodeListPtr m_spList;		// list of device properties
	MSXML2::IXMLDOMAttributePtr m_spAttr;		// list of items in a device property node
	MSXML2::IXMLDOMNamedNodeMapPtr m_spAttrList;// list of device properties
	MSXML2::IXMLDOMNodePtr m_spNode;				// a device property node
	_bstr_t m_bstrCtx;
	CString m_csFileName;

	// methods
	bool GetAllItemsForThisDevicePropetry(LPCTSTR pszDevicePropName, key_value_pair_t& ItemPairs);


};

#endif  // _SCOT_DEVICECONTEXT_STORE_
