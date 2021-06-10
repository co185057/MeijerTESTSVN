#pragma once

/*lint -esym( 666, VERIFY_AND_LOG ) */
#define VERIFY_AND_LOG(EXPRESSION)\
{\
    HRESULT throwaway_hr = (EXPRESSION);\
    if (throwaway_hr != S_OK)\
        ITRACE_ERROR(QUOTE(EXPRESSION) _T(" returned error:  ") + IString(throwaway_hr).d2x());\
}


/*lint -esym( 666, VERIFY_WIN32_AND_LOG ) */
#define VERIFY_WIN32_AND_LOG(EXPRESSION)\
{\
    if (FALSE == (EXPRESSION))\
    {\
        ISystemErrorInfo error;\
        ITRACE_ERROR(QUOTE(EXPRESSION) _T(" returned error:  ") + IString(error));\
    }\
}


#ifndef _DEBUG

#ifndef _ASSERTEX
#define _ASSERTEX(msg) ((void)0)
#endif

#else

#ifndef _ASSERTEX
#define _ASSERTEX(msg)\
    /*lint -save -e717 */\
    do { CT2A aString(msg);\
            if ((1 == _CrtDbgReport(_CRT_ERROR, __FILE__, __LINE__, NULL, aString))) \
            _CrtDbgBreak(); } while (0)\
    /*lint -restore */
#endif

#endif

