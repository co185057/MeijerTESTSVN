#pragma once

#ifdef _UNICODE
#define getString getWString
#define getAttribute getWAttribute
#else
#define getString getAString
#define getAttribute getAAttribute
#endif
#include "Utils.h"

namespace XMLUtilities
{
     UTILS_API void dumpAttributes (
        /*[in]*/ Trace &traceObject
        /*[in]*/ , ISAXAttributes __RPC_FAR * pAttributes );

     UTILS_API const char *getAString(
        /* [in] */ const wchar_t __RPC_FAR *pwchVal,
        /* [in] */ int cchVal);

     UTILS_API const wchar_t *getWString(
        /* [in] */ const wchar_t __RPC_FAR *pwchVal,
        /* [in] */ int cchVal);

     UTILS_API void getString(
        /* [in] */ const wchar_t __RPC_FAR *pwchVal,
        /* [in] */ int cchVal,
        /* [in] */ wchar_t __RPC_FAR *pwchResult);

     UTILS_API const char *getAAttribute(
        /*[in]*/ const wchar_t __RPC_FAR * pwchNamespaceUri,
        /*[in]*/ int cchNamespaceUri,
        /* [in] */ const wchar_t __RPC_FAR *pwchAttrName,
        /* [in] */ int cchAttrName,
        /*[in]*/ ISAXAttributes __RPC_FAR * pAttributes );

     UTILS_API const wchar_t *getWAttribute(
        /*[in]*/ const wchar_t __RPC_FAR * pwchNamespaceUri,
        /*[in]*/ int cchNamespaceUri,
        /* [in] */ const wchar_t __RPC_FAR *pwchAttrName,
        /* [in] */ int cchAttrName,
        /*[in]*/ ISAXAttributes __RPC_FAR * pAttributes );
}
