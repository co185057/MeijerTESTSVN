#define IMPLEMENT_TBSTREAM(CObj, m_Msg) \
STDMETHODIMP CObj::StreamOut(LONG* cb, BYTE** pBuffer) \
{ \
   *pBuffer= m_Msg.Serialize((int &)*cb); \
   return S_OK; \
} \
 \
 \
STDMETHODIMP CObj::ParseStream(VARIANT i_varpBuf) \
{ \
	ULONG lnBytes; \
	BYTE* pBuffer= CSafeArrayVariant::GetNewBufferFromVariant(i_varpBuf, lnBytes); \
	m_Msg.Parse(pBuffer); \
	delete [] pBuffer; \
	return S_OK; \
} \
 \
STDMETHODIMP CObj::ParseBuffer(BYTE *pBuf) \
{ \
	m_Msg.Parse(pBuf); \
	return S_OK; \
} \
 \
STDMETHODIMP CObj::GetMessage(LONG* o_pTBMsgObj) \
{ \
	*o_pTBMsgObj= (LONG)(CTBMsgObj*)&m_Msg; \
	return S_OK; \
} \


#define PARSE_AND_NOTIFY(Obj) \
{ \
HRESULT hr = CoCreateInstance(__uuidof(C##Obj), NULL, CLSCTX_INPROC_SERVER, __uuidof(ITBStream), (void**) &pStream); \
if (pStream) \
{ \
	pStream->ParseBuffer(pbuffer); \
	I##Obj *pInterface; \
	pStream->QueryInterface(__uuidof(I##Obj), (void**) &pInterface); \
	if( m_pfnEventCallbackProc ) \
	{ \
		IDispatchPtr spDisp = pInterface; \
		m_pfnEventCallbackProc(Receive##Obj##Event, spDisp); \
		spDisp = NULL; \
	} \
	_ItbConnectEvents_Receive##Obj(pInterface); \
	pInterface->Release(); \
	pStream.Release(); \
	} \
}