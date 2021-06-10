// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/XError.cpp 1     6/20/08 1:13p Sa250050 $
/****************************************************************************/
/*                                                                          */
/* Module Name:               XError                                         */
/*                                                                          */
/* Module Title:              Exception Object                              */
/*                                                                          */
/****************************************************************************/
/*
// $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/XError.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 9     6/14/05 8:56a Dm185016
 * Corrected locationAtIndex() method for exception tracing.
 * 
 * 6     4/05/05 12:05p Dm185016
 * Removed code that was causing an exception.
 * 
 * 5     3/24/05 7:34p Dm185016
 * UNICODE
 * 
 * 4     3/23/05 1:39p Tp151000
 * Per David M. to fixed build problem
 * 
 * 3     2/28/05 6:03p Dm185016
 * Unicode fix
 *
 * 2     2/28/05 3:36p Dm185016
 * lint
 *
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
//
// 9     1/18/05 4:04p Dm185016
// UNICODE support
//
// 8     10/04/04 7:02p Dm185016
// Moved to namespace utils.
//
// 6     5/18/04 7:21p Dm185016
// lint
//
// 5     4/12/04 3:30p Dm185016
// Fixed Unicode problems
//
// 4     2/18/04 9:16a Dm185016
// Fixed system error information
//
// 3     2/10/04 11:54a Dm185016
// IException has been made into a derivative of std::exception.
//
// 2     1/23/04 3:41p Dm185016
// DLL Support
//
// 1     1/06/04 9:00a Dm185016
// First Version
*/


/****************************************************************************/
/* Include Files                                                            */
/****************************************************************************/

#include "StdAfx.h"
#include "xerror.h"

using namespace utils;


/*------------------------------- Constructors -------------------------------*/
IExceptionLocation::IExceptionLocation ( const _TCHAR*   pszFileName,
                       const _TCHAR*   pszFunctionName,
                       unsigned long ulLineNumber )
    : pClFile(pszFileName)
    , pClFunction(pszFunctionName)
    , ulClLineNo(ulLineNumber)
{
//  IMODTRACE_DEVELOP("IExceptionLocation::IExceptionLocation ( const char*, const char*, unsigned long)");
}


IString IExceptionLocation::toString() const
{
    IString locationString(_T("    File: ") + IString(pClFile));
//  locationString += ".  Function: " + IString((pClFunction != NULL) ? pClFunction : "<Unknown>") +
//                 ".  Line: " + IString(ulClLineNo);
    locationString += _T(".  Function: ");
    locationString += IString((pClFunction != NULL) ? pClFunction : _T("<Unknown>"));
    locationString += _T(".  Line: ") + IString(ulClLineNo);

    return locationString;
}


IException::IException ( )
    : exsevCl(unrecoverable)
    , m_hrClErrorId(S_OK)
    , ulexlocClCount(0)
    , ulClTxtLvlCount(0)
    , m_pErrorInfo(NULL)
{
}


IException::IException ( const _TCHAR*       pszErrorText,
             HRESULT     ulErrID,
             Severity          severity )
	: exsevCl(severity)
    , m_hrClErrorId(ulErrID)
    , ulexlocClCount(0)
    , ulClTxtLvlCount(0)
    , m_pErrorInfo(NULL)
    , m_sErrorText(pszErrorText)
{
}


IException::IException ( const IException& exc )
    : exception(exc)
{
    _assign(exc);
}


IException
    &IException::operator= ( const IException& exc )
{
//  IMODTRACE_DEVELOP("IException::IException ( const IException&)");

    ((std::exception *)this)->operator=(exc);

    _assign(exc);

    return *this;
}


void IException::_assign( const IException& exc )
{
    m_sErrorText        = exc.m_sErrorText;
    m_sAErrorText       = exc.m_sAErrorText;
    m_hrClErrorId       = exc.m_hrClErrorId;
    ulClTxtLvlCount     = exc.ulClTxtLvlCount;
    ulexlocClCount      = exc.ulexlocClCount;
    exsevCl             = exc.exsevCl;

    setErrorInfo(exc.m_pErrorInfo);

    for (size_t i = 0; i < ulexlocClCount; i++)
    {
        exlocClArray[i] = exc.exlocClArray[i];
    }
}


IException::~IException ( )
{
    try
    {
        delete m_pErrorInfo;
    }
    catch(...){}
}


void IException::setErrorInfo (const utils::IErrorInfo *pErrorInfo )
{
    if (pErrorInfo != NULL)
        m_pErrorInfo = pErrorInfo->copy();
}


IException &IException::addLocation      ( const IExceptionLocation& location )
{
//  IMODTRACE_DEVELOP("IException::addLocation      ( const IExceptionLocation&)");

    if (ulexlocClCount < MAX_LOC)
    {
        exlocClArray[ulexlocClCount++] = location;
    }

    return *this;
}


const IExceptionLocation &IException::locationAtIndex ( unsigned long lIndex) const 
{ 
    if (lIndex < MAX_LOC)
        return exlocClArray[lIndex]; 
    else
        return exlocClArray[MAX_LOC-1]; 
}
//
//
//IException &IException::logExceptionData ( )
//{
//    return *this;
//}


const _TCHAR *IException::text ( unsigned long ) const
{
    return m_sErrorText.c_str();
}


const char *IException::what() const throw( )
{
    CT2A temp(m_sErrorText.c_str());
    m_sAErrorText = (const char *)temp;
    return m_sAErrorText.c_str();
}


ISystemErrorInfo::ISystemErrorInfo ( const _TCHAR* systemFunctionName)
    : bClError(FALSE)
    , m_hrClErrorId((HRESULT)GetLastError())
{
    IString error = _T("(") + IString(systemFunctionName) + _T(":")
        + IString(m_hrClErrorId).d2x() + _T(")")
        + Trace::formatLastError(systemFunctionName, m_hrClErrorId);

    m_sErrorText = (StringType)error;
}


ISystemErrorInfo::ISystemErrorInfo ( HRESULT systemErrorId,
                                     const _TCHAR* systemFunctionName)
    : bClError(FALSE)
    , m_hrClErrorId(systemErrorId)
{
    IString error = _T("(") + IString(systemFunctionName) + _T(":")
        + IString(m_hrClErrorId).d2x() + _T(")")
        + Trace::formatLastError(systemFunctionName, m_hrClErrorId);

    m_sErrorText = (StringType)error;
}


ISystemErrorInfo::ISystemErrorInfo ( const ISystemErrorInfo& errorInformation )
    : utils::IErrorInfo(errorInformation)
{
    bClError        = errorInformation.bClError;
    m_hrClErrorId   = errorInformation.m_hrClErrorId;

    m_sErrorText = errorInformation.m_sErrorText;
}


ISystemErrorInfo::~ISystemErrorInfo ( ) { }


ISocketErrorInfo::ISocketErrorInfo ( const _TCHAR* )
	: bClError(false)
	, m_hrClErrorId(0)
//  : ulClErrorId(WSAGetLastError())
{
//  IMODTRACE_DEVELOP("ISocketErrorInfo::ISocketErrorInfo ( unsigned long, const char*)");


    m_sErrorText = lookupError(m_hrClErrorId);
}


ISocketErrorInfo::ISocketErrorInfo ( HRESULT socketErrorId,
                                     const _TCHAR* )
    : bClError(false)
    , m_hrClErrorId(socketErrorId)
{
//  IMODTRACE_DEVELOP("ISocketErrorInfo::ISocketErrorInfo ( unsigned long, const char*)");


    m_sErrorText = lookupError(m_hrClErrorId);
}


ISocketErrorInfo::ISocketErrorInfo ( const ISocketErrorInfo& errorInformation )
    : utils::IErrorInfo(errorInformation)
{
    bClError        = errorInformation.bClError;
    m_hrClErrorId   = errorInformation.m_hrClErrorId;

   m_sErrorText = errorInformation.m_sErrorText;
}


const _TCHAR *ISocketErrorInfo::lookupError( HRESULT socketErrorId) const
{
    const _TCHAR *retValue;

    switch (socketErrorId)
    {
        case  WSANOTINITIALISED  :      retValue = _T("10093:  Successful WSAStartup() has not occured yet.");             break;
        case  WSAENETDOWN :             retValue = _T("10050:  Network id down.");             break;
        case  WSAEACCES :               retValue = _T("10013:  Permission denied.");    break;

        case  WSAEINTR :                retValue = _T("10004:  Interrupted function call.");    break;
        case  WSAEBADF :                retValue = _T("10009:  WSAEBADF.");    break;
        case  WSAEFAULT :               retValue = _T("10014:  Bad address.");    break;
        case  WSAEINVAL :               retValue = _T("10022:  Invalid argument.");    break;
        case  WSAEMFILE :               retValue = _T("10024:  Too many open files.");    break;
        case  WSAEWOULDBLOCK :          retValue = _T("10035:  Resource temporarily unavailable.");    break;
        case  WSAEINPROGRESS :          retValue = _T("10036:  Operation is now in progress.");    break;
        case  WSAEALREADY :             retValue = _T("10037:  Operation already in progress.");    break;
        case  WSAENOTSOCK :             retValue = _T("10038:  Socket operation on non-socket.");    break;
        case  WSAEDESTADDRREQ :         retValue = _T("10039:  Destination address required.");    break;
        case  WSAEMSGSIZE :             retValue = _T("10040:  Message too long.");    break;
        case  WSAEPROTOTYPE :           retValue = _T("10041:  Protocol wrong type for socket.");    break;
        case  WSAENOPROTOOPT :          retValue = _T("10042:  Bad protocol option.");    break;
        case  WSAEPROTONOSUPPORT :      retValue = _T("10043:  Protocol not supported.");    break;
        case  WSAESOCKTNOSUPPORT :      retValue = _T("10044:  Socket type not supported.");    break;
        case  WSAEOPNOTSUPP :           retValue = _T("10045:  Operation not supported.");    break;
        case  WSAEPFNOSUPPORT :         retValue = _T("10046:  Protocol family not supported.");    break;
        case  WSAEAFNOSUPPORT :         retValue = _T("10047:  Address family not supported by protocol family.");    break;
        case  WSAEADDRINUSE :           retValue = _T("10048:  Address already in use.");    break;
        case  WSAEADDRNOTAVAIL :        retValue = _T("10049:  Cannot assign requested address.");    break;
        case  WSAENETUNREACH :          retValue = _T("10051:  Network is unreachable.");    break;
        case  WSAENETRESET    :         retValue = _T("10052:  Network dropped connection on reset.");    break;
        case  WSAECONNABORTED :         retValue = _T("10053:  Software caused connection abort.");    break;
        case  WSAECONNRESET   :         retValue = _T("10054:  Connection reset by peer.");    break;
        case  WSAENOBUFS      :         retValue = _T("10055:  No buffer space available.");    break;
        case  WSAEISCONN      :         retValue = _T("10056:  Socket is already connected.");    break;
        case  WSAENOTCONN     :         retValue = _T("10057:  Socket is not connected.");    break;
        case  WSAESHUTDOWN    :         retValue = _T("10058:  Cannot send socket after shutdown.");    break;
        case  WSAETOOMANYREFS :         retValue = _T("?????:  WSAETOOMANYREFS.");    break;
        case  WSAETIMEDOUT    :         retValue = _T("10060:  Connection timed out.");    break;
        case  WSAECONNREFUSED :         retValue = _T("10061:  Connection refused.");    break;
        case  WSAELOOP        :         retValue = _T("10062:  WSAELOOP.");    break;
        case  WSAENAMETOOLONG :         retValue = _T("10063:  WSANAMETOOLONG.");    break;
        case  WSAEHOSTDOWN    :         retValue = _T("10064:  Host is down.");    break;
        case  WSAEHOSTUNREACH :         retValue = _T("10065:  No route to host.");    break;
        case  WSAENOTEMPTY    :         retValue = _T("?????:  WSANOTEMPTY.");    break;
        case  WSAEPROCLIM     :         retValue = _T("10067:  Too many processes.");    break;
        case  WSAEUSERS       :         retValue = _T("?????:  WSAEUSERS.");    break;
        case  WSAEDQUOT       :         retValue = _T("?????:  WSAEDQUOT.");    break;
        case  WSAESTALE       :         retValue = _T("?????:  WSAESTALE.");    break;
        case  WSAEREMOTE      :         retValue = _T("?????:  WSAEREMOTE.");    break;
        case  WSAEDISCON      :         retValue = _T("10101:  Graceful shutdown in progress.");    break;
        case  WSASYSNOTREADY     :      retValue = _T("10091:  Network subsystem is unavailable.");    break;
        case  WSAVERNOTSUPPORTED :      retValue = _T("10092:  WINSOCK.DLL version out of range.");    break;
        default :                       retValue = _T("Unknown return code.");
    }

    return retValue;
}


ISocketErrorInfo::~ISocketErrorInfo ( ) { }


ICommunicationErrorInfo::ICommunicationErrorInfo ( const COMSTAT &cs,
                        DWORD ce,
                        const _TCHAR* commFunctionName)
    : bClError(false)
    , comstat(cs)
    , comError(ce)
{
    m_sErrorText = lookupError(commFunctionName);
}


ICommunicationErrorInfo::ICommunicationErrorInfo ( const ICommunicationErrorInfo& errorInformation )
    : utils::IErrorInfo(errorInformation)
{
    bClError        = errorInformation.bClError;
    comstat         = errorInformation.comstat;
    comError        = errorInformation.comError;

   m_sErrorText = errorInformation.m_sErrorText;
}


const _TCHAR *ICommunicationErrorInfo::lookupError(const _TCHAR *pFunctionName) const
{
    _TCHAR *pBuffer = new _TCHAR[2000];

    pBuffer[0] = 0;
    _tcscat(pBuffer, _T("Communication Error in function:  "));

    if (pFunctionName != NULL)
        _tcscat(pBuffer, pFunctionName);

    if (comError & CE_BREAK)    _tcscat(pBuffer, _T("\n  The hardware detected a break condition."));
    if (comError & CE_DNS)      _tcscat(pBuffer, _T("\n   Windows 95 and Windows 98: A parallel device is not selected."));
    if (comError & CE_FRAME)    _tcscat(pBuffer, _T("\n   The hardware detected a framing error."));
    if (comError & CE_IOE)      _tcscat(pBuffer, _T("\n   An I/O error occurred during communications with the device."));
    if (comError & CE_MODE)     _tcscat(pBuffer, _T("\n   The requested mode is not supported, or the hFile parameter is invalid. If this value is specified, it is the only valid error."));
    if (comError & CE_OOP)      _tcscat(pBuffer, _T("\n   Windows 95 and Windows 98: A parallel device signaled that it is out of paper."));
    if (comError & CE_OVERRUN)  _tcscat(pBuffer, _T("\n   A character-buffer overrun has occurred. The next character is lost."));
    if (comError & CE_PTO)      _tcscat(pBuffer, _T("\n   Windows 95 and Windows 98: A time-out occurred on a parallel device."));
    if (comError & CE_RXOVER)   _tcscat(pBuffer, _T("\n   An input buffer overflow has occurred. There is either no room in the input buffer, or a character was received after the end-of-file (EOF) character."));
    if (comError & CE_RXPARITY) _tcscat(pBuffer, _T("\n   The hardware detected a parity error."));
    if (comError & CE_TXFULL)   _tcscat(pBuffer, _T("\n  The application tried to transmit a character, but the output buffer was full."));

    return pBuffer;
}


ICommunicationErrorInfo::~ICommunicationErrorInfo ( ) {}


//IEXCLASSIMPLEMENT(IInvalidRequest,IException);
  IInvalidRequest :: IInvalidRequest(const _TCHAR* a, HRESULT b)\
    : IException((const _TCHAR *)(IString(a) + _T(".  Code=") + IString(b).d2x()), b)\
        { }\
  const _TCHAR* IInvalidRequest :: name() const\
    {\
     return ( _T("IInvalidRequest") );\
    }\
  IInvalidRequest :: ~IInvalidRequest() {;}\
  IInvalidRequest :: IInvalidRequest(const IInvalidRequest & a) : IException(a) {;}

IEXCLASSIMPLEMENT(IEmptyException,IException);
IEXCLASSIMPLEMENT(ISystemError,IException);
IEXCLASSIMPLEMENT(ISocketError,IException);
IEXCLASSIMPLEMENT(IAccessError,IException);
IEXCLASSIMPLEMENT(IInvalidParameter,IException);
IEXCLASSIMPLEMENT(IOutOfMemory,IException);
IEXCLASSIMPLEMENT(ISehError,IException);
IEXCLASSIMPLEMENT(IOError,IException);

