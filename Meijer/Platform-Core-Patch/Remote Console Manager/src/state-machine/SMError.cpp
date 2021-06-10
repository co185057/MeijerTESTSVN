// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMError.cpp 1     6/20/08 1:13p Sa250050 $
/****************************************************************************/
/*                                                                          */
/* Module Name:               SMError                                         */
/*                                                                          */
/* Module Title:              Exception Object                              */
/*                                                                          */
/****************************************************************************/
/*
// $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMError.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 10    5/31/05 11:37a Dm185016
 * New errors for Data Model Support
 * 
 * 7     3/25/05 10:48a Dm185016
 * Actions as Expression support
 * 
 * 6     3/10/05 3:22p Dm185016
 * Added missing case clauses.
 * 
 * 5     3/03/05 3:17p Dm185016
 * TAR 291700 - Added error value for invalid lookup table type.
 * 
 * 4     2/28/05 3:53p Dm185016
 * lint
 * 
 * 3     2/17/05 5:58p Dm185016
 * Fixed problem displaying XML errors.
* 
* 2     2/14/05 2:20p Dm185016
* Lookup table support
* 
* 1     1/26/05 2:27p Dm185016
* Moved to new Repository.
* 
* 14    1/12/05 6:27p Dm185016
* Added internal error definition.
* 
* 13    11/09/04 3:34p Dm185016
* Support for state lookup tables
* 
* 12    10/14/04 1:46p Dm185016
* New code for unknown message name.
* 
* 11    10/04/04 6:41p Dm185016
* Made names more sensible for non-security related projects.
* Fixed trace output for error codes.
* 
* 9     6/30/04 3:26p Dm185016
* Made into base class for CSecurityError class
* 
* 8     6/02/04 4:19p Dm185016
* Added new invalid timer error code
* 
* 7     5/04/04 4:45p Dm185016
* Added validation of identifiers
*
*6     4/30/04 1:03p Dm185016
*New error codes
*
*5     4/20/04 4:43p Dm185016
*Made a derivative of CXMLErrorInfo
*
*4     4/12/04 3:31p Dm185016
* Added error codes for missing parameter from tag
* 
* 3     2/24/04 3:33p Dm185016
* Added definitions for multiply defined actions
* 
* 2     2/10/04 11:40a Dm185016
* New Expression evaluation support
* 
* 1     1/27/04 1:54p Dm185016
* Renamed from SecurityError
* 
* 1     1/16/04 9:00a Dm185016
* New base object providing formatting for errors (HRESULTs) obtained
* during the parsing process.
*/

/****************************************************************************/
/* Include Files                                                            */
/****************************************************************************/

#include "StdAfx.h"
#include "SMError.h"

CSMErrorInfo::CSMErrorInfo (  )
{}


CSMErrorInfo::CSMErrorInfo ( HRESULT hr, 
							  int nLine, 
							  int nCol, 
							  const _TCHAR *pszFile, 
							  const _TCHAR *pszType )
{
    Init( lookupError(hr), hr, nLine, nCol, pszFile, pszType );
}


CSMErrorInfo::CSMErrorInfo ( HRESULT securityErrorId
						   , const StringType &)
{
    Init( lookupError(securityErrorId), securityErrorId );
}


const _TCHAR *CSMErrorInfo::lookupError( HRESULT smErrorId)
{
    const _TCHAR *retValue;

    switch (smErrorId)
    {
        case STATEM_NO_RULES_DEFINED              :  retValue = _T("No State Machine Rules Defined for State/Message");             break;
        case STATEM_INVALID_ACTION_ID             :  retValue = _T("Invalid Action ID");             break;
        case STATEM_INVALID_MSG_ID                :  retValue = _T("Invalid Message ID");             break;
        case STATEM_INVALID_STATE_ID              :  retValue = _T("Invalid State ID");             break;
        case STATEM_UNDEFINED_ACTION_ID           :  retValue = _T("Undefined Action ID");             break;
        case STATEM_UNDEFINED_MSG_ID              :  retValue = _T("Undefined Message ID");             break;
        case STATEM_UNDEFINED_STATE_ID            :  retValue = _T("Undefined State ID");             break;
        case STATEM_OBJECT_ALREADY_DEFINED        :  retValue = _T("Element already defined");             break;
        case STATEM_DUPLICATE_ACTION_ID           :  retValue = _T("Duplicate Action ID detected");             break;
        case STATEM_DUPLICATE_MSG_ID              :  retValue = _T("Duplicate Message ID detected");             break;
        case STATEM_DUPLICATE_STATE_ID            :  retValue = _T("Duplicate State ID detected");             break;
        case STATEM_DUPLICATE_VARIABLE_ID         :  retValue = _T("Duplicate Variable ID detected");             break;
        case STATEM_DUPLICATE_INITIAL_STATE       :  retValue = _T("Only one <initial> state may be defined");             break;
        case STATEM_UNDEFINED_VARIABLE            :  retValue = _T("Undefined Variable");             break;
        case STATEM_INVALID_VARIABLE_ID           :  retValue = _T("Invalid Variable ID");             break;
        case STATEM_UNDEFINED_VARIABLE_ID         :  retValue = _T("Undefined Variable ID");             break;
        case STATEM_UNDEFINED_INTERNAL_ACTION     :  retValue = _T("Undefined Internal Action");             break;
        case STATEM_UNDEFINED_OPERATION           :  retValue = _T("Undefined Operation");             break;
        case STATEM_DUPLICATE_TIMER_ID            :  retValue = _T("Duplicate Timer ID detected");             break;
        case STATEM_DUPLICATE_CONSTANT_ID         :  retValue = _T("Duplicate Constant ID detected");             break;
        case STATEM_UNDEFINED_TIMER_ID            :  retValue = _T("Undefined Timer ID");             break;
        case STATEM_UNDEFINED_CONSTANT_ID         :  retValue = _T("Undefined Constant ID");             break;
        case STATEM_DUPLICATE_GLOBAL_STATE        :  retValue = _T("Only one <global/> state may be defined");             break;
        case STATEM_GLOBAL_STATE_NAME_NOT_ALLOWED :  retValue = _T("Cannot specify <name> with <global/>");             break;
        case STATEM_INVALID_CONSTANT_ID           :  retValue = _T("Invalid Constant ID");             break;
        case STATEM_INVALID_TIMER_ID              :  retValue = _T("Invalid Timer ID");             break;
        case STATEM_UNDEFINED_TIMER               :  retValue = _T("Undefined Timer");             break;
        case STATEM_UNDEFINED_CONSTANT            :  retValue = _T("Undefined Constant");             break;
        case STATEM_TIMEOUT_ALREADY_IN_PROGRESS   :  retValue = _T("Timeout already in progress");             break;
        case STATEM_UNABLE_TO_START_TIMER         :  retValue = _T("Unable to start timer");             break;
        case STATEM_TIMEOUT_NOT_IN_PROGRESS       :  retValue = _T("Timeout not in progress");             break;
        case STATEM_UNABLE_TO_STOP_TIMER          :  retValue = _T("Unable to stop timer");             break;
        case STATEM_MISSING_TIMER_NAME            :  retValue = _T("Missing timer name in <action>");             break;
        case STATEM_MISSING_TIMEOUT_VALUE         :  retValue = _T("Missing timeout value in <action>");             break;
        case STATEM_INVALID_TIMEOUT_VALUE         :  retValue = _T("Invalid timeout value in <action>");             break;
        case STATEM_MISSING_VARIABLE_NAME         :  retValue = _T("Missing variable name in <action>");             break;
        case STATEM_DUPLICATE_LIBRARY_NAME        :  retValue = _T("Duplicate Library name detected");             break;
        case STATEM_MISSING_EXPRESSION            :  retValue = _T("Missing expression in <action>");             break;
        case STATEM_INVALID_EXPRESSION_ID         :  retValue = _T("Invalid Expression ID");             break;
        case STATEM_UNDEFINED_EXPRESSION_ID       :  retValue = _T("Undefined Expression");             break;
        case STATEM_MISSING_RC                    :  retValue = _T("Missing return code <parameter>");             break;
        case STATEM_MULTIPLE_ACTIONS_DEFINED      :  retValue = _T("Multiple actions were defined for <rule>");             break;
        case STATEM_MISSING_PARAMETER             :  retValue = _T("Missing <parameter> tag for <rule>");             break;
        case STATEM_INVALID_PARAMETER             :  retValue = _T("Invalid <parameter> tag defined for <rule>");             break;
        case STATEM_MISSING_INCLUDE_NAME_ATTR     :  retValue = _T("Missing name= attribute for <include>");             break;
        case STATEM_INVALID_UNKNOWN_ERROR_SM      :  retValue = _T("Unknown exception caught in state machine");             break;
        case STATEM_NO_INITIAL_STATE		      :  retValue = _T("No <state> was defined with the <initial/> tag");             break;
        case STATEM_INVALID_IDENTIFIER            :  retValue = _T("Invalid identifier");             break;
        case STATEM_INVALID_TIMER_INTERVAL_ZERO   :  retValue = _T("Invalid timer interval.  0 is not allowed");             break;
        case STATEM_UNDEFINED_TAG                 :  retValue = _T("Undefined state machine tag");             break;
        case STATEM_NO_KEY_MAPPING                :  retValue = _T("Message name not defined in key/message map");             break;
        case STATEM_MSG_CREATION_FAILED           :  retValue = _T("Message creation failed");             break;
        case STATEM_UNDEFINED_MSG_NAME            :  retValue = _T("Undefined Message name");             break;
        case STATEM_UNDEFINED_STATETABLE_ID       :  retValue = _T("Undefined State Lookup Table name");             break;
        case STATEM_DUPLICATE_STATETABLE_ID       :  retValue = _T("Duplicate State Lookup Table name");             break;
        case STATEM_INTERNAL_ERROR                :  retValue = _T("Internal Programming Error detected");             break;
        case STATEM_UNDEFINED_LOOKUPTABLE_ID      :  retValue = _T("Undefined Lookup Table name");             break;
        case STATEM_DUPLICATE_LOOKUPTABLE_ID      :  retValue = _T("Duplicate Lookup Table name");             break;
        case STATEM_INVALID_LOOKUPTABLE_TYPE      :  retValue = _T("Invalid Lookup Table type.  Valid values are \"string\" or \"int\".");             break;
        case STATEM_INVALID_EXPRESSION_ACTION     :  retValue = _T("Parameter list for Action not valid for expression.");             break;
        case STATEM_INVALID_EXPRESSION_ACTION_PLIST :  retValue = _T("Action not valid for expression.");             break;
        case STATEM_DM_OBJECT_ALREADY_REGISTERED  :  retValue = _T("Data Model object already registered.");             break;
        case STATEM_DM_OBJECT_NOT_REGISTERED      :  retValue = _T("Data Model object not registered.");             break;
        case STATEM_DM_PROPERTY_ALREADY_REGISTERED  :  retValue = _T("Data Model property already registered.");             break;
        case STATEM_DM_PROPERTY_NOT_REGISTERED      :  retValue = _T("Data Model property not registered.");             break;
        default :
        {
            retValue = CXMLErrorInfo::lookupError(smErrorId);
        }
    }

    return retValue;
}


CSMErrorInfo::~CSMErrorInfo ( ) { }


IEXCLASSIMPLEMENT(CSMError,IException);
