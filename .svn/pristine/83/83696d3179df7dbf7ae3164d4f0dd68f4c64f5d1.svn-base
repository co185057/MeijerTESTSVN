#include "stdafx.h"
#include "GTObject.h"

CGTObject::CGTObject(IDispatch *pObj) :
pGenObj(pObj)
{

}

CGTObject::~CGTObject() 
{

}

_bstr_t CGTObject::XML()
{
	return pGenObj->xml;
}

_variant_t CGTObject::GetValue2(_bstr_t Name)
{
	return pGenObj->GetValue(Name);
}

long CGTObject::GetListCount(_bstr_t Name)
{
	return pGenObj->GetListCount(Name);
}

unsigned char CGTObject::SetCurrentNode(_bstr_t path, long index)
{
	return pGenObj->SetCurrentNode(path, index);
}

_variant_t CGTObject::GetListValue2(_bstr_t Name, long index)
{
	return pGenObj->GetListValue2(Name, index);
}