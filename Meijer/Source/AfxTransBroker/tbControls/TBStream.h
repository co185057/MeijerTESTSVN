#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>

// ITBStream
[
	object,
	uuid(3FF844BD-E48C-4E60-B5AF-12827E1F9C22),
	dual,
	helpstring("ITBStream Interface"),
	pointer_default(unique)
]

__interface ITBStream : public IDispatch
{
   [id(1), helpstring("method StreamOut")] HRESULT StreamOut([in,out] LONG* cb, [out,retval] BYTE** pBuffer);
	[id(2), helpstring("method ParseStream")] HRESULT ParseStream([in] VARIANT Var);
	[id(3), helpstring("method ParseBuffer")] HRESULT ParseBuffer([in] BYTE* pBuffer);
	[id(4), helpstring("method GetMessage")] HRESULT GetMessage([out] LONG* o_pTBMsgObj);
};
