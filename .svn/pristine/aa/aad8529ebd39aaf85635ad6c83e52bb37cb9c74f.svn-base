#include "stdafx.h"
#include "tbgenmsg.h"
#include "SafeArrayVariant.h"
//#include <atlctl.h>

#define VERSION_GENOBJ_BIN 3
#define VERSION_GENOBJ_XML 4

const _bstr_t cFIELD = _bstr_t(OLESTR("field"));

_bstr_t  CTBGenMsg::m_bstrTBControlsXML = _bstr_t(OLESTR("c:\\scot\\config\\TBControls.xml"));
bool     CTBGenMsg::m_bInitializedOnce= false;

CTBGenMsg::CTBGenMsg( BYTE msgId /*=0*/, bool bXML /* =false */, bool bValidate /*=false*/)
:   CTBMsgObj((TBDEVICEID)0),
	m_pData(NULL),
	m_nDataSize(0),
	m_pFields(NULL),
	m_pMessageNode(NULL),
	m_bXmlLoaded(false),
	m_bValidate(false),
    m_sName(_T(""))
{
	bXML ? SetMsgVersion(VERSION_GENOBJ_XML) : SetMsgVersion(VERSION_GENOBJ_BIN);
	CreateXMLDoc();
	SetTBMsgId(msgId);
    SetXMLOutPutRaw(bXML);
	SetValidate(bValidate);
}
void CTBGenMsg::SetTBMsgIdRaw(BYTE tbMsgId)
{
	CTBMsgObj::SetTBMsgId(tbMsgId);
   if (m_pMessageNode == NULL) {
		m_pMessageNode= m_pMsgDoc->createElement("message");
		_ASSERT(m_pMessageNode != NULL);
		m_pFields= m_pMsgDoc->createElement("fields");
		m_pMessageNode->appendChild(m_pFields);
	}
	CString csMsgid;
	csMsgid.Format(_T("%02X"), tbMsgId); 
	m_pMessageNode->setAttribute(OLESTR("msgid"), _bstr_t((LPCTSTR)csMsgid));
}

void CTBGenMsg::SetName(_bstr_t bstrName)
{
   if (m_pMessageNode == NULL) {
		m_pMessageNode= m_pMsgDoc->createElement("message");
		_ASSERT(m_pMessageNode != NULL);
		m_pFields= m_pMsgDoc->createElement("fields");
		m_pMessageNode->appendChild(m_pFields);
	}
	
	m_sName= (LPCTSTR) bstrName;
	m_pMessageNode->setAttribute(OLESTR("name"), bstrName);
   LoadIfNeeded();
}

CString CTBGenMsg::GetName()
{
   	return m_sName;
}

void CTBGenMsg::LoadIfNeeded()
{
    if (m_bValidate && (GetTBMsgId() || m_sName.GetLength()))
		Load();
}
void CTBGenMsg::SetTBMsgId(BYTE tbMsgId)
{
	SetTBMsgIdRaw(tbMsgId);
    LoadIfNeeded();
}

void CTBGenMsg::SetXMLOutPutRaw(bool bXML)
{
	bXML ? SetMsgVersion(VERSION_GENOBJ_XML) : SetMsgVersion(VERSION_GENOBJ_BIN);
	if (m_pMessageNode) {
		_bstr_t bstrXML= bXML ? OLESTR("1") : OLESTR("0");
		m_pMessageNode->setAttribute(OLESTR("xmloutput"), bstrXML);
	}
}

void CTBGenMsg::SetXMLOutPut(bool bXML)
{
	SetXMLOutPutRaw(bXML);
	if (!bXML)
		SetValidate(true);	// validate must be true for binary
}

bool CTBGenMsg::GetXMLOutPut()
{
	return (GetMsgVersion() == VERSION_GENOBJ_XML);
}

void CTBGenMsg::SetValidateRaw(bool bValidate)
{
	m_bValidate= bValidate;
   if (m_pMessageNode == NULL) {
		m_pMessageNode= m_pMsgDoc->createElement("message");
		_ASSERT(m_pMessageNode != NULL);
		m_pFields= m_pMsgDoc->createElement("fields");
		m_pMessageNode->appendChild(m_pFields);
	}
	_bstr_t bstrValidate= bValidate ? OLESTR("1") : OLESTR("0");
	m_pMessageNode->setAttribute(OLESTR("validate"), bstrValidate);
}

void CTBGenMsg::SetValidate(bool bValidate)
{
	SetValidateRaw(bValidate);
    LoadIfNeeded();
}

bool CTBGenMsg::GetValidate()
{
	if (m_pMessageNode) {
		_bstr_t bstrValidate= m_pMessageNode->getAttribute(OLESTR("validate"));
		m_bValidate= (_ttoi((LPCTSTR) bstrValidate) != 0) ;
	}
	return m_bValidate;
}

CTBGenMsg::~CTBGenMsg(void)
{
	delete [] m_pData;
}

void CTBGenMsg::Reset(ResetOptions option)
{
    switch (option) 
    {
    case RESET_FIELD_DATA:
        {
            if (m_pFields) 
            {
                MSXML2::IXMLDOMElementPtr ElementPtr;
                MSXML2::IXMLDOMNodeListPtr spList= m_pFields->selectNodes(cFIELD);

                _variant_t vStrName, vStrValue;
                while ((ElementPtr= spList->nextNode()) != NULL) 
                {
                    _variant_t vType= ElementPtr->GetdataType();

                    switch ( GetVtTypeFromStringType( vType.bstrVal) ) 
                    {
                    case VT_BSTR:
                        ElementPtr->put_nodeTypedValue(_variant_t(""));
                        break;
                    case VT_I1:
                        {
                            _variant_t vResetValue((BYTE)0);
                            //vResetValue.cVal=0;
                            vResetValue.ChangeType(VT_I1);
                            ElementPtr->put_nodeTypedValue(vResetValue);
                        }
                        break;
                    case VT_BOOL:
                        {
                            _variant_t vResetValue(false);
                            //vResetValue=false;
                            ElementPtr->put_nodeTypedValue(vResetValue);
                        }
                        break;
                    case VT_I2:
                        {
                            _variant_t vResetValue((short)0, VT_I2);
                            //vResetValue=0;
                            //vResetValue.ChangeType(VT_I2);
                            ElementPtr->put_nodeTypedValue(vResetValue);
                        }
                        break;
                    case VT_I4:
                        {
                            _variant_t vResetValue(0L, VT_I4);
                            //vResetValue=0;
                            //vResetValue.ChangeType(VT_I4);
                            ElementPtr->put_nodeTypedValue(vResetValue);
                        }
                        break;
					case (VT_ARRAY | VT_UI1):
						{
							VARIANT vArray = CSafeArrayVariant::GetVariantFromBuffer(NULL,0);
							ElementPtr->put_nodeTypedValue(vArray);
							SafeArrayDestroy( vArray.parray );
						}
						break;
                    default:
                        _ASSERT(false);
                    }
                }
            }
        }
        break;
    case RESET_XML_DOC:
        {
            ClearAll();
        }
        break;
    default:
        {
            CString csError(_T("Unknown Reset Option"));
            throw csError;
            break;
        }
    }
}

void CTBGenMsg::ClearAll()
{
	delete [] m_pData;
    m_pData=NULL;
	m_nDataSize=0;

	m_pFields= NULL;
	m_pMessageNode= NULL;
	m_bXmlLoaded= false;
	m_bValidate= false;
    m_sName= _T("");

	SetMsgVersion(VERSION_GENOBJ_BIN); // default
	SetTBMsgId(0);
	SetValidate(false);
}

int CTBGenMsg::getLength(void)
{
	int cb= sizeof(tRouteHeader);
	if (GetXMLOutPut()) {
		cb += (m_pMessageNode->xml.length()+1) * sizeof(TCHAR);
	}
	else {
		MSXML2::IXMLDOMElementPtr ElementPtr;

		_variant_t vStrName, vStrValue;
		MSXML2::IXMLDOMNodeListPtr spList= m_pFields->selectNodes(cFIELD);
		while ((ElementPtr= spList->nextNode()) != NULL) {
			_variant_t vValue= ElementPtr->GetdataType();
			switch ( GetVtTypeFromStringType( vValue.bstrVal) ) {
				case VT_BSTR:
					cb += 2 + SysStringLen(ElementPtr->nodeTypedValue.bstrVal)*sizeof(TCHAR);
					break;
				case VT_I1:
				case VT_BOOL:
					cb+= 1;
					break;
				case VT_I2:
					cb+= 2;
					break;
				case VT_I4:
					cb+=4;
					break;
				case (VT_ARRAY|VT_UI1):
					{
						cb += 4;
						_variant_t vTest = ElementPtr->nodeTypedValue;
						if ( vTest.vt == (VT_ARRAY|VT_UI1))
						{
							cb += vTest.parray->rgsabound->cElements;
						}
					}
					break;
				default:
					_ASSERT(false);
			}
		}
	}
	return cb;
}

_bstr_t CTBGenMsg::getXML()
{
   _bstr_t bstrVal;
	if (m_pMessageNode){
		bstrVal= m_pMessageNode->xml;
	}
	return bstrVal;
}

void CTBGenMsg::setXML(BSTR bstrXml)
{
		if( VARIANT_FALSE == m_pMsgDoc->loadXML(bstrXml) )
		{
//		    _ASSERT(FALSE);
	        MSXML2::IXMLDOMParseErrorPtr pError;
            pError = m_pMsgDoc->parseError;
            _bstr_t strResult = _bstr_t(OLESTR("setXML failed on ")) + 
                                _bstr_t(OLESTR("\nReason: ")) + _bstr_t(pError->Getreason()) +
                                _bstr_t(OLESTR("\nSource: ")) + _bstr_t(pError->GetsrcText()) +
                                _bstr_t(OLESTR("\nLine: ")) + _bstr_t(pError->Getline());
		    throw CString((LPCTSTR)strResult);
		}

		m_pMessageNode= m_pMsgDoc->documentElement;
		_ASSERT(m_pMessageNode != NULL);
		if (m_pMessageNode) {
			m_pFields= m_pMessageNode->selectSingleNode(_bstr_t(OLESTR("fields")));
			//_ASSERT(m_pFields != NULL);
			MSXML2::IXMLDOMAttributePtr pAttr= m_pMessageNode->attributes->getNamedItem(_bstr_t(OLESTR("msgid")));;
			if (pAttr) 
			{
				_bstr_t bstrMsgId= pAttr->nodeValue;
				int nMsgId;
#if _MSC_VER >= 7000
				_stscanf_s((LPCTSTR) bstrMsgId, _T("%02X"), &nMsgId);
#else
				_stscanf((LPCTSTR) bstrMsgId, _T("%02X"), &nMsgId);
#endif
				SetTBMsgIdRaw((BYTE) nMsgId);
			}
			pAttr= m_pMessageNode->attributes->getNamedItem(_bstr_t(OLESTR("validate")));;
			if (pAttr) 
			{
				_bstr_t bstrValidate= pAttr->nodeValue;
				int nValue= _ttoi((LPCTSTR) bstrValidate);
				SetValidateRaw(nValue ? true:false);
			}
			pAttr= m_pMessageNode->attributes->getNamedItem(_bstr_t(OLESTR("xmloutput")));;
			if (pAttr) 
			{
				_bstr_t bstrValue= pAttr->nodeValue;
				int nValue= _ttoi((LPCTSTR) bstrValue);
				SetXMLOutPutRaw(nValue ? true: false);
			}
			pAttr= m_pMessageNode->attributes->getNamedItem(_bstr_t(OLESTR("name")));;
			if (pAttr) 
			{
				_bstr_t bstrValue= pAttr->nodeValue;
                m_sName= (LPCTSTR) bstrValue;
			}
		}
//		CString stest((BSTR)m_pMessageNode->xml);
//		ATLTRACE(stest);
}

void CTBGenMsg::Parse(BYTE *pBuf)
{
   ParseHeader(pBuf);         // must be called first
   
	if (GetXMLOutPut()) {
		CString str((const TCHAR*)&pBuf[m_nOffset], (m_Hdr.nLen-sizeof(tRouteHeader))/sizeof(TCHAR));
		BSTR bstr= str.AllocSysString();
		setXML(bstr);
		SysFreeString(bstr);
	}
	else {
		_bstr_t bstrName, bstrValue;
		delete [] m_pData;
		m_pData= new BYTE[m_Hdr.nLen];
		Load();
		MSXML2::IXMLDOMElementPtr ElementPtr;
		MSXML2::IXMLDOMNodeListPtr spList= m_pFields->selectNodes(cFIELD);
		while ((ElementPtr= spList->nextNode()) != NULL) {
			bstrName= ElementPtr->getAttribute(_bstr_t(OLESTR("name")));
			_variant_t vType= ElementPtr->GetdataType();
			switch ( GetVtTypeFromStringType( vType.bstrVal) ) {
				case VT_BSTR:
						bstrValue= ParseL2String();
						//SetValue( bstrName.GetBSTR(), bstrValue.GetBSTR());
						SetValue( (LPCTSTR) bstrName, bstrValue);
					break;
				case VT_I1:
				case VT_BOOL:
					SetValue( (LPCTSTR) bstrName, (BYTE)ParseByte());
					break;
				case VT_I2:
               {
                  _variant_t sVar((short) ParseUShort(), VT_I2);
					   SetValue( (LPCTSTR) bstrName, sVar);
               }
					break;
				case VT_I4:
               {
                  _variant_t sVar((long)ParseULong(), VT_I4);
					   SetValue( (LPCTSTR) bstrName, sVar);
               }
			   break;
				case (VT_ARRAY|VT_UI1):
				{
					ULONG ulDataLen = ParseULong();
					BYTE* pData = ParseBinaryBuffer( ulDataLen );
					VARIANT vArray = CSafeArrayVariant::GetVariantFromBuffer( pData, ulDataLen );
					SetValue( (LPCTSTR) bstrName, vArray );
					SafeArrayDestroy(vArray.parray);
				}
				break;
				default:
					_ASSERT(false);
			}
		}
	}
}

BYTE* CTBGenMsg::Serialize(int &byteLength)
{
	// all binary formats must exist in the tbControls.xml and must be validated
   SerializeHeader(byteLength);
	if (GetXMLOutPut()) {
		_bstr_t bstrxml(m_pMessageNode->xml);
		TCHAR *pchar= (TCHAR*)bstrxml;

		memcpy(&m_pBuf[m_nOffset], pchar, (m_pMessageNode->xml.length()+1)* sizeof(TCHAR));
	}
	else {
		//SerializeBinaryBuffer((BYTE*) m_pData, m_nDataSize);

		MSXML2::IXMLDOMElementPtr ElementPtr;
		MSXML2::IXMLDOMNodeListPtr spList= m_pFields->selectNodes(cFIELD);
		while ((ElementPtr= spList->nextNode()) != NULL) {
			_variant_t vValue= ElementPtr->nodeTypedValue;
            VARTYPE varType= vValue.vt;

			if (varType == VT_NULL)
                varType= GetVtTypeFromStringType((LPCWSTR)_bstr_t(ElementPtr->GetdataType()));

			switch(varType) {
				case VT_BSTR:
					SerializeL2String(CString(vValue.bstrVal));
					break;
				case VT_BOOL:
					SerializeByte(vValue.boolVal? true:false);
					break;
				case VT_I1:
				case VT_UI1:
					SerializeByte(vValue.bVal);
					break;
				case VT_I2:
				case VT_UI2:
					SerializeUShort(vValue.uiVal);
					break;
				case VT_I4:
				case VT_UI4:
				case VT_INT:
					SerializeULong(vValue.ulVal);
					break;
				case (VT_ARRAY|VT_UI1):
				{
					BYTE* pSafe;
					ULONG ulLen = 0;
					pSafe = CSafeArrayVariant::GetNewBufferFromVariant( vValue, ulLen );
					SerializeULong(ulLen);
					SerializeBinaryBuffer(pSafe, ulLen);
					delete[] pSafe;
				}
				break;
				default:
					_ASSERT(false);
			}
		}
	}
	//MessageBox(NULL, m_pMessageNode->xml, "Serialize", MB_OK);
   return m_pBuf;
}

VARTYPE CTBGenMsg::GetVtTypeFromStringType(LPCWSTR szwType)
{
		if (szwType == NULL)
			return VT_BSTR;
		if (wcscmp(szwType, L"string") == 0) 
			return VT_BSTR;
		else if (wcscmp(szwType, L"boolean") == 0) 
			return VT_BOOL;
		else if ((wcscmp(szwType, L"byte") == 0) ||
					 (wcscmp(szwType, L"i1") == 0))
			return VT_I1;
		else if ((wcscmp(szwType, L"short") == 0) ||
					 (wcscmp(szwType, L"i2") == 0))
			return VT_I2;
		else if ((wcscmp(szwType, L"int") == 0) ||
					 (wcscmp(szwType, L"i4") == 0) ||
					 (wcscmp(szwType, L"long") == 0))
			return VT_I4;
		else if ((wcscmp(szwType, L"bin.base64") == 0))
			return VT_ARRAY|VT_UI1;
		return VT_BSTR;
}

void CTBGenMsg::LoadRegistryOptions()
{
	HKEY	hKey;
	long	rc;
	DWORD disposition;
	LPCTSTR const NCR_TBSUBKEY = _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTTB");
   LPCTSTR const NCR_TBCONTROLS= _T("TBControlsXMLFile");

	TCHAR sz[300];
	DWORD dwDataSize = sizeof(sz);

   //m_bstrTBControlsXML

   rc = RegCreateKeyEx( HKEY_LOCAL_MACHINE,    // handle of main key
		                   NCR_TBSUBKEY, // addr of subkey name
		                   0,                      // reserved
		                   0,                      // no type for subkey value?
		                   REG_OPTION_NON_VOLATILE,    // will be saved across system reboots
		                   KEY_ALL_ACCESS,         // desired security access
		                   NULL,                   // default security, don't care
		                   &hKey,                  // subkey handle is returned
		                   &disposition );         // returns either REG_CREATED_NEW_KEY or
	                                             // REG_OPENED_EXISTING_KEY
 
	rc = RegQueryValueEx(hKey, NCR_TBCONTROLS, NULL,NULL, (LPBYTE) sz, &dwDataSize);
	if (rc==ERROR_SUCCESS)
   	m_bstrTBControlsXML = sz;
   else 
   {
      CString cs= (LPCTSTR) m_bstrTBControlsXML;
     	rc = RegSetValueEx( hKey,                  // subkey handle from creation/opening
			                  NCR_TBCONTROLS, // addr of value name
			                  0,                     // reserved
			                  REG_SZ,             // value type
			                  (BYTE *)cs.GetBuffer(0),
                           (DWORD)cs.GetLength() * sizeof(TCHAR) + sizeof(TCHAR));	 // data length
   }

   rc = RegCloseKey(hKey);
}

void CTBGenMsg::Load()
{
	if (m_bXmlLoaded)
		return;

	MSXML2::IXMLDOMParseErrorPtr pError;
	MSXML2::IXMLDOMElementPtr ElementPtr;

   // try to get tbcontrols.xml location from regsitry once
   if (!m_bInitializedOnce)   //static member - one time registry loading
   {
      m_bInitializedOnce=true;
      LoadRegistryOptions();
   }
   // Load and validate the specified file into the DOM.
   HRESULT hr = m_pMsgDoc->load(m_bstrTBControlsXML);
   pError = m_pMsgDoc->parseError;

   // Return validation results in message to the user.
   if (pError->errorCode != S_OK)
   {
      _bstr_t strResult = _bstr_t(OLESTR("Validation failed on ")) + m_bstrTBControlsXML +
         _bstr_t(OLESTR("\nReason: ")) + _bstr_t(pError->Getreason()) +
         _bstr_t(OLESTR("\nSource: ")) + _bstr_t(pError->GetsrcText()) +
         _bstr_t(OLESTR("\nLine: ")) + _bstr_t(pError->Getline());
		throw CString((LPCTSTR)strResult);
   }

	CString csxPathString;
    if (GetTBMsgId() != 0) // load by msgid
    	csxPathString.Format(_T("//message[@msgid = \"%02X\"]"), GetTBMsgId()); 
    else    // load by name
    	csxPathString.Format(_T("//message[@name = \"%s\"]"), (LPCTSTR)GetName()); 
	m_pMsgDoc->setProperty(_bstr_t(OLESTR("SelectionLanguage")), _bstr_t(OLESTR("XPath")));
   m_pMessageNode= m_pMsgDoc->selectSingleNode( _bstr_t (csxPathString));
   if (m_pMessageNode)
   {
      m_pFields= m_pMessageNode->selectSingleNode(_bstr_t(OLESTR("fields")));

		MSXML2::IXMLDOMNodeListPtr spList= m_pFields->selectNodes(cFIELD);
		while ((ElementPtr= spList->nextNode()) != NULL) {
			_variant_t vfType=ElementPtr->getAttribute(_bstr_t("ftype"));
			if (wcscmp(L"long", vfType.bstrVal)==0)
				vfType=L"int";
			ElementPtr->put_dataType(vfType.bstrVal);
			//vfType =ElementPtr->GetdataType();
		}
	}
	else {
		CString cs;
		cs.Format(_T("[%s] not found"), csxPathString);
		throw cs;
	}
    Reset(RESET_FIELD_DATA);
	m_bXmlLoaded=true;
}

void CTBGenMsg::SetValue(LPCTSTR szName, _variant_t vValue)
{
	MSXML2::IXMLDOMElementPtr pField;

	CString csxPathString;
	csxPathString.Format(_T("field[@name = \"%s\"]"), szName); 
	if (m_pMessageNode==NULL) {
		if(m_bValidate) {
			Load();
		}
		else {
			m_pMessageNode= m_pMsgDoc->createElement("message");
			_ASSERT(m_pMessageNode != NULL);
			m_pFields= m_pMsgDoc->createElement("fields");
			m_pMessageNode->appendChild(m_pFields);
		}
	}
	_ASSERT(m_pFields != NULL);
	if (m_pFields) {
		pField= m_pFields->selectSingleNode( _bstr_t(csxPathString));
		if ((pField == NULL) && !m_bValidate) {
			pField= m_pMsgDoc->createElement(cFIELD);
			if (pField) {
				pField->setAttribute(_bstr_t(OLESTR("name")),_bstr_t(szName));
				m_pFields->appendChild(pField);
			}
		}
		if(pField) {
			if (m_bValidate) {
				_variant_t vDataType;
				vDataType= pField->GetdataType();
				if (vDataType.vt != VT_NULL) {
					try 
					{
						vValue.ChangeType( GetVtTypeFromStringType(vDataType.bstrVal));
						pField->put_nodeTypedValue(vValue);
					}
					catch(...) 
					{
						switch (vValue.vt) {
							case VT_I2:
							case VT_UI2:
							case VT_I4:
							case VT_UI4:
							case VT_INT:
								TCHAR buf[100];
#if _MSC_VER >= 7000
								pField->PutnodeTypedValue(_ltot_s(vValue.lVal, buf, 10));
#else
								pField->PutnodeTypedValue(_ltot(vValue.lVal, buf, 10));
#endif
								break;
							case (VT_ARRAY|VT_UI1):
								pField->PutnodeTypedValue( vValue );
					 			break;
							default:
								{
									// if string and length is zero - assume field not being set
									if( vValue.vt == VT_BSTR && !((_bstr_t)vValue).length() )
									{
										pField->put_nodeTypedValue(vValue);
									}
									else
									{
										CString cs;
										cs.Format(_T("Invalid type for field:%s"), szName);
										throw cs;
									}
								}
						}
					}
				}
				else
				{
					CString cs;
					cs.Format(_T("field [%s] type not defined"), szName);
					throw cs;
				}
			}
			else { // not validating... all are bstr, except VT_ARRAY
				if ( vValue.vt == (VT_ARRAY | VT_UI1) )
				{
					pField->PutnodeTypedValue( vValue );
				}
				else
				{
					vValue.ChangeType(VT_BSTR);
					pField->put_text(vValue.bstrVal);
				}
			}
		}
		else {
			CString cs;
			cs.Format(_T("field [%s] not found"), szName);
			throw cs;
		}
	}
	else
		throw CString(_T("Message Not Loaded"));
}

//_variant_t CTBGenMsg::GetValue(BSTR szName)
//{
//	_bstr_t bstr(szName,false);
//	//return GetValue(_bstr_t(szName).GetBSTR());
//	return GetValue((LPCTSTR) bstr);
//}

_variant_t CTBGenMsg::GetValue2(LPCTSTR szName)
{
	_variant_t v;
	try{
		v = GetValue(szName);
	}
	catch (...) {
		v = _T("");
	}
	return v;
}

_variant_t CTBGenMsg::GetValue(LPCTSTR szName)
{
	MSXML2::IXMLDOMNodePtr pField;
	CString csxPathString;
   _variant_t v;
	csxPathString.Format(_T("field[@name = \"%s\"]"), szName); 
	if (m_pFields) {
		pField= m_pFields->selectSingleNode(_bstr_t(csxPathString));
		if (pField) {
			v= pField->nodeTypedValue;
			return v;
		}
		else {
			CString cs;
			cs.Format(_T("field [%s] not found"), szName);
			throw cs;
		}
	}
	else
		throw CString(_T("Message Not Loaded"));
   v.vt=VT_EMPTY;
   return v;
}

void CTBGenMsg::CreateXMLDoc()
{
    if (m_pMsgDoc == NULL) {
        m_pMsgDoc.CreateInstance(__uuidof(DOMDocument40));
        m_pMsgDoc->async = VARIANT_FALSE;
        m_pMsgDoc->validateOnParse = VARIANT_TRUE;
        m_pMsgDoc->resolveExternals = VARIANT_TRUE;
        m_pMsgDoc->preserveWhiteSpace=true;
    }
}
