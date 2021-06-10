#pragma once

/*lint -emacro(506, VERIFY_PSX_AND_LOG) */
/*lint -esym(666, VERIFY_PSX_AND_LOG) */
/*lint -emacro(506, VERIFY_PSX) */
/*lint -esym(666, VERIFY_PSX) */

#include "PSXError.h"

#define VERIFY_PSX_AND_LOG(EXPRESSION)\
do\
{\
    ITRACE_DEVELOP(_T("Verifying:  ") QUOTE(EXPRESSION));\
    PSXRC throwaway_rc = (EXPRESSION);\
    if ( IsRCError( throwaway_rc ) )\
    {   CPSXErrorInfo error(throwaway_rc, QUOTE(EXPRESSION));\
        ITRACE_ERROR(error.text());\
    }\
    break;\
} while(1)


#define VERIFY_PSX(hr, EXPRESSION)\
do\
{\
    ITRACE_DEVELOP(_T("Verifying:  ") QUOTE(EXPRESSION));\
    PSXRC throwaway_rc = (EXPRESSION);\
    if ( IsRCError( throwaway_rc ) )\
    {\
        CPSXErrorInfo error(throwaway_rc, QUOTE(EXPRESSION));\
        ITRACE_ERROR(error.text());\
        hr = error.errorId();\
    }\
    else\
        hr = S_OK;\
    break;\
} while(1)
