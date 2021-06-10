#include "StdAfx.h"

//lint --e{119, 534}

#include "XMLUtilities.h"
#ifdef ITRACE_DEVELOP
#undef ITRACE_DEVELOP
#endif

#define ITRACE_DEVELOP(p1) \
   	if (traceObject.debugInProgress) traceObject.write((p1), ITRACE_LEVEL_DEBUG)

void XMLUtilities::dumpAttributes (
    /*[in]*/ Trace &traceObject
    /*[in]*/ , ISAXAttributes __RPC_FAR * pAttributes )
{
    int iAttrCount = 0;
    pAttributes->getLength(&iAttrCount);
    wchar_t *pwchAttrLocalName;
    int iLocalName;
    for (int i=0; i<iAttrCount; i++)
    {
        pAttributes->getLocalName(i, &pwchAttrLocalName, &iLocalName);
        ITRACE_DEVELOP((wchar_t *)L"Attribute name:  " + IString(getString(pwchAttrLocalName, iLocalName)));
    }
}


const char *XMLUtilities::getAString(
            /* [in] */ const wchar_t __RPC_FAR *pwchVal,
            /* [in] */ int cchVal)
{
    static wchar_t wval[1000];
    cchVal = cchVal>999 ? 999 : cchVal;
    wcsncpy( wval, pwchVal, (unsigned)cchVal ); 
    wval[cchVal] = 0;

	static char val[1000];
	wcstombs(val, wval, 1000);
    return val;
}


const wchar_t *XMLUtilities::getWString(
            /* [in] */ const wchar_t __RPC_FAR *pwchVal,
            /* [in] */ int cchVal)
{
    static wchar_t val[1000];
    cchVal = cchVal>999 ? 999 : cchVal;
    wcsncpy( val, pwchVal, (unsigned)cchVal ); 
    val[cchVal] = 0;
    return val;
}

void XMLUtilities::getString(
    /* [in] */ const wchar_t __RPC_FAR *pwchVal,
    /* [in] */ int cchVal,
    /* [in] */ wchar_t __RPC_FAR *pwchResult)
{
    cchVal = cchVal>999 ? 999 : cchVal;
    wcsncpy( pwchResult, pwchVal, (unsigned)cchVal ); 
    pwchResult[cchVal] = 0;
}

const char *XMLUtilities::getAAttribute(
    /*[in]*/ const wchar_t __RPC_FAR * pwchNamespaceUri,
    /*[in]*/ int cchNamespaceUri,
    /* [in] */ const wchar_t __RPC_FAR *pwchAttrName,
    /* [in] */ int cchAttrName,
    /*[in]*/ ISAXAttributes __RPC_FAR * pAttributes )
{
    wchar_t *pwchAttrValue;
    int iAttrValue;
    if (S_OK == pAttributes->getValueFromName(
                    const_cast<wchar_t *>(pwchNamespaceUri), 
                    cchNamespaceUri, 
                    const_cast<wchar_t *>(pwchAttrName),
                    cchAttrName,
                    &pwchAttrValue,
                    &iAttrValue))
    {
        return XMLUtilities::getAString(pwchAttrValue, iAttrValue);
    }

    static char val[1] = { 0 };
    return val;
}


const wchar_t *XMLUtilities::getWAttribute(
    /*[in]*/ const wchar_t __RPC_FAR * pwchNamespaceUri,
    /*[in]*/ int cchNamespaceUri,
    /* [in] */ const wchar_t __RPC_FAR *pwchAttrName,
    /* [in] */ int cchAttrName,
    /*[in]*/ ISAXAttributes __RPC_FAR * pAttributes )
{
    wchar_t *pwchAttrValue;
    int iAttrValue;
    if (S_OK == pAttributes->getValueFromName(
                    const_cast<wchar_t *>(pwchNamespaceUri), 
                    cchNamespaceUri, 
                    const_cast<wchar_t *>(pwchAttrName),
                    cchAttrName,
                    &pwchAttrValue,
                    &iAttrValue))
    {
        return XMLUtilities::getWString(pwchAttrValue, iAttrValue);
    }

    static wchar_t val[1] = { 0 };
    return val;
}
