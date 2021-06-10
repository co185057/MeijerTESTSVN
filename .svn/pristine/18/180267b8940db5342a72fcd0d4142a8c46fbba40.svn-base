//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  "IGTObject.h"
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GTOBJECT
#define _GTOBJECT

#include "IGTObject.h"
/////////////////////////////////////////////////////////////////////////////
// GTObject command target
/*
#ifdef _DEBUG
    #import <GTOUD.dll> rename_namespace("GTO")
#else
    #import <GTOU.dll> rename_namespace("GTO")
#endif
    */
// SSCOADK-2499 Sample SSF will not compile stand-alone
//              Use HeaderGenerator project to build tlh/tli headers if necessary.
#include <GTOU.tlh>

class CGTObject : public IGTObject
{
public:
    CGTObject(IDispatch *pObj);
    virtual ~CGTObject();

	virtual _bstr_t XML();
	virtual _variant_t GetValue2(_bstr_t Name);
	virtual long GetListCount(_bstr_t Name);
	virtual unsigned char SetCurrentNode(_bstr_t path, long index);
	virtual _variant_t GetListValue2(_bstr_t Name, long index);

    GTO::IGTGenObjPtr GetGenObj();
    GTO::IGTGenObjPtr pGenObj;
};

#endif

