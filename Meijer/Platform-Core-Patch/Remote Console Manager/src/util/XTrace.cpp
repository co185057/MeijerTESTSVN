// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/XTrace.cpp 1     6/20/08 1:13p Sa250050 $
/******************************************************************************
* Trace
*
* Author: David Miller, $Author: Sa250050 $
* Version: $Revision: 1 $
*****************************************************************************/
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/XTrace.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 10    6/14/05 8:57a Dm185016
 * Added nested exception tracing for internal exceptions
 * 
 * 8     5/01/05 9:00a Tp151000
 * Performmance
 * 
 * 7     4/12/05 2:47p Dm185016
 * 
 * 6     4/12/05 11:46a Dm185016
 * Fixed ASSERT when error code unknown.
 * 
 * 5     3/28/05 5:12p Dm185016
 * Fixed DrWatson on my desktop
 * 
 * 4     3/24/05 7:37p Dm185016
 * UNICODE
 * 
 * 3     2/28/05 3:39p Dm185016
 * lint
** 
** 2     2/21/05 8:00p Dm185016
** Added debugging information to new and delete for pooled objects.
** 
** 1     1/26/05 2:32p Dm185016
** Moved to new Repository.
* 
* 18    1/25/05 3:50p Dm185016
* TAR 287899 - Memory leak during termination.
* 
* 17    1/18/05 4:06p Dm185016
* UNICODE support
* 
* 16    10/05/04 8:28a Dm185016
* Trace object no longer shared.  Trace rewritten from collection of
* static methods to real object.  Access to Trace object controlled via
* handles.
* 
* 14    7/13/04 3:16p Dm185016
* Fix to prevent null registry key from crashing
* 
* 13    6/30/04 3:48p Dm185016
* Added use of new IStringBuffer class for performance of dump routine.
* 
* 12    6/08/04 3:19p Dm185016
* lint changes
* Hooks to Loggable for data capture
* 
* 11    5/21/04 9:53a Dm185016
* lint
* 
* 10    5/05/04 9:09a Dm185016
* Fixed missing trace using datacapture
* 
* 9     5/04/04 7:05p Tk124825
* Made the neseccary Unicode changes
* 
* 8     4/12/04 3:33p Dm185016
* Added DataCapture support
* 
* 7     3/19/04 3:33p Dm185016
* Added trace settings from the Registry
* 
* 6     3/01/04 10:19a Dm185016
* Added variable argument processing
* 
* 5     2/24/04 3:44p Dm185016
* Added compatibility with SCOT tracing.
* 
* 4     2/10/04 11:55a Dm185016
* Added support for exceptions
* 
* 3     1/23/04 3:42p Dm185016
* New DLL Support
* 
* 2     1/16/04 9:33a Dm185016
* Fixed problem with junk displaying when unknown return code is
* formatted.
* 
* 1     1/06/04 9:00a Dm185016
* First Version
*/

//lint --e{534}
/*lint -save -e1550 */

#include "stdafx.h"
#include "xtrace.h"
#include "registry.h"
#include "LoggableManager.h"

const int BUFFER_SIZE               = 80;

bool Trace::m_bInitialized = false;
FILE *Trace::traceFile;
CTraceMap Trace::m_traces;
LONG Trace::m_lInstanceCount = 0;
PTRACE Trace::m_pDefaultTrace = NULL;

FILE *tFile = NULL;

static int  traceFileCount = 0;

static time_t startTime;

ThreadMap Trace::indentLevels;

static const _TCHAR *blank = _T(" ");
static long lLineCnt = 0;

//static char *strappend(char *, char *);

MethodTrace::MethodTrace(Trace &traceObject, const _TCHAR * name, LPCTSTR line, DWORD dwMaskEntry, DWORD dwMaskExit) throw()
    : m_modName(name)
    , m_dwMaskEntry(dwMaskEntry)
    , m_dwMaskExit(dwMaskExit)
    , m_trace(traceObject)
{
    IString msg(_T(">> "));

    msg += name;
    msg += _T(":");
    msg += line;
    if (m_trace.debugInProgress) m_trace.write(msg, m_dwMaskEntry);

    m_trace.incLevel();
}


MethodTrace::MethodTrace(Trace &traceObject, const _TCHAR * name, int line, DWORD dwMaskEntry, DWORD dwMaskExit) throw()
    : m_modName(name)
    , m_dwMaskEntry(dwMaskEntry)
    , m_dwMaskExit(dwMaskExit)
    , m_trace(traceObject)
{
    IString msg(_T(">> "));

    msg += name;
    msg += _T(":");
    msg += (const _TCHAR *)IString(line);
    if (m_trace.debugInProgress) m_trace.write(msg, m_dwMaskEntry);

    m_trace.incLevel();
}


MethodTrace::~MethodTrace()
{
    IString msg(_T("<< "));
    msg += (const _TCHAR *)m_modName;

	m_trace.decLevel();

    if (m_trace.debugInProgress) m_trace.write(msg, m_dwMaskExit);
}


Trace::Trace()
    : debugInProgress(false)
    , allocFile(NULL)
    , writeLineNumber(0)
    , fileSize(0)
    , threshold(0)
    , m_pszDataCapTitle(NULL)
    , m_pszRegistryKey(NULL)
    , m_pszTraceFileName(NULL)
    , m_pDC(NULL)
    , m_bUseDataCapture(true)
    , m_hTrace(++m_lInstanceCount)
    , m_pLoggableMgr(new CLoggableManager(m_hTrace))
{
}


Trace::~Trace()
{
	// destroy the data capture object
    if( m_pDC != NULL )
	{
        m_pDC->Flush();
		m_pDC->Cleanup();
		delete m_pDC;
		m_pDC = NULL;
	}
 
    delete [] m_pszTraceFileName;
    m_pszTraceFileName = NULL;
    delete [] m_pszRegistryKey;
    m_pszRegistryKey = NULL;
    delete [] m_pszDataCapTitle;
    m_pszDataCapTitle = NULL;

    delete m_pLoggableMgr;

    if (allocFile != NULL)
    {
        //TODO:  Need to close this file
        allocFile = NULL;
    }
}


ILoggable &Trace::getLoggable(HTRACE hTrace, const _TCHAR *pName)
{
    return GetTraceObject(hTrace).getLoggable(pName);
}


ILoggable &Trace::getLoggable(HTRACE hTrace, DWORD dw)
{
    return GetTraceObject(hTrace).getLoggable(dw);
}


ILoggable &Trace::getLoggable(const _TCHAR *pName)
{
    return (*m_pLoggableMgr).getLoggable(pName);
}


ILoggable &Trace::getLoggable(DWORD dw)
{
    return (*m_pLoggableMgr).getLoggable(dw);
}


void Trace::decLevel()
{
	if (indentLevels[(int)GetCurrentThreadId()] > 0)
        indentLevels[(int)GetCurrentThreadId()]--;
}


void Trace::incLevel()
{
    indentLevels[(int)GetCurrentThreadId()]++;
}


void Trace::logException(const IException &e, const _TCHAR *pszFile, const _TCHAR *pszFunction, int nLineNo) throw()
{ 
    int maxLoc = e.locationCount();
    for (int i = 0; i<maxLoc; i++)
    {
        const IExceptionLocation &location = e.locationAtIndex(i);
        write(_T("File:  ") + IString(location.fileName()) + _T(".  Function:  ") + IString(location.functionName()) + _T(".  Line:  ") + IString(location.lineNumber()), ITRACE_LEVEL_ERROR);
    }
    logException((const std::exception &)e, pszFile, pszFunction, nLineNo);
}


void Trace::logException(const std::exception &e, const _TCHAR *pszFile, const _TCHAR *pszFunction, int nLineNo) throw()
{ 
    
    try
    {
        CA2T sTempBuf(e.what());
        write(_T("File:  ") + IString(pszFile) + _T(".  Function:  ") + IString(pszFunction) + _T(".  Line:  ") + IString(nLineNo) + _T(".  ERROR:  ") + (LPTSTR)sTempBuf, ITRACE_LEVEL_ERROR);
        CA2T sName(typeid(e).name());
        write(IString(_T("Exception:  Caught ")) + (LPTSTR)sName, ITRACE_LEVEL_ERROR);
    }
    catch( const __non_rtti_object  &e )
    {  
        CA2T sTempBuf(e.what());
        write((const _TCHAR *)IString(sTempBuf), ITRACE_LEVEL_ERROR); 
    }
    catch( const bad_typeid &e )
    {
        CA2T sTempBuf(e.what());
        write((const _TCHAR *)IString(sTempBuf), ITRACE_LEVEL_ERROR); 
    }
    catch( ... )
        { XTRACE_ERROR(_T("Exception caught")); }
}


void Trace::write(const IString &msg, DWORD mask) throw() 
{ 
    write((const _TCHAR *)msg, mask);
}


void Trace::write(const std::string &msg, DWORD dwMask) throw()
{ write(msg.c_str(), dwMask); }


void Trace::write(const std::wstring &msg, DWORD dwMask) throw()
{ write(msg.c_str(), dwMask); }

#define MAX_TRACE_BUF 256
static _TCHAR szBuff[MAX_TRACE_BUF+1];

void Trace::write(  long,                   // COMP_ID
                    LPCTSTR,                // T_ID, trace ID
                    int,                    // source file line number
                    DWORD dwMask,           // trace level 0,1,2...9
                    LPCTSTR sFormatString,  // prntf fmt string
                    ...) throw()            // variable args
{	
    va_list argptr;
    va_start( argptr, sFormatString );        // variable parameter list processing

    _vsntprintf( szBuff, MAX_TRACE_BUF, sFormatString, argptr );	// variable parameter list processing

    write0(szBuff, dwMask);
}


void Trace::write(LPCSTR pMsg, DWORD dwMask) throw()
{
    CA2T convertedString(pMsg);
    write0(convertedString, dwMask);
}


void Trace::write(LPCWSTR pMsg, DWORD dwMask) throw()
{
    CW2T convertedString(pMsg);
    write0(convertedString, dwMask);
}


void Trace::write0(LPCTSTR pMsg, DWORD dwMask) throw()
{
    int bytesWritten = 0;

    try
    {

        clock_t millis = clock();

        time_t seconds = startTime + millis / CLK_TCK;
        millis %= (int)CLK_TCK;

        if (m_bUseDataCapture)
        {
            m_pDC->DoDCPrintf(dwMask, pMsg);
        }
        else if (traceFile == NULL)
        {
           _tprintf(_T("%ld%03ld: %d:%d %*s%s\n")
                , seconds
                , millis
                , GetCurrentProcessId()
                , GetCurrentThreadId()
                , indentLevels[(int)GetCurrentThreadId()]*2
                , blank
                , (_TCHAR *)pMsg);
            flushall();
        }
        else
        {
            bytesWritten = _ftprintf(traceFile, _T("%5ld) %ld%03.3ld: %d:%d %*s%s\n")
                , lLineCnt++
                , seconds
                , millis
                , GetCurrentProcessId()
                , GetCurrentThreadId()
                , indentLevels[(int)GetCurrentThreadId()]*2
                , blank
                , (_TCHAR *)pMsg);
            fflush(traceFile);

            fileSize += bytesWritten;
        }

        if ((threshold != 0) && (fileSize > threshold))
        {
            fileSize = 0;

            // open new log file
            IString fn(m_pszTraceFileName);
            traceFileCount++;
            fn += _T(".");
            fn += traceFileCount;
            _TCHAR *oldTraceFile = new _TCHAR[fn.length()+1];
            _tcscpy(oldTraceFile, (const _TCHAR *)fn);

            // close the log file.
            fclose(traceFile);

            // Rename the file
            ::_trename(m_pszTraceFileName, oldTraceFile);

            // Reopen the log file
            traceFile = _tfopen(m_pszTraceFileName, _T("w"));

            // compress the old log file
    //      compressLog(oldTraceFile);
        }
    }
    catch (...)
        { _tprintf(_T("Error detected in write()")); }
}


#pragma warning( push )
#pragma warning( disable : 4311 )

void Trace::traceNew( void *pObject, size_t nSize, const _TCHAR *pszFile, long lLine )
{
    IStringBuffer szbOutput(128);
    szbOutput += pszFile;
    szbOutput += _T(",");
    szbOutput += lLine;
    szbOutput += _T(",new,");
    szbOutput += IString((ULONG)pObject).d2x();
    szbOutput += _T(",");
    szbOutput += (unsigned long)nSize;
    write((const _TCHAR *)szbOutput, ITRACE_LEVEL_DEBUG);
}

void Trace::traceDelete(void *pObject, size_t nSize, const _TCHAR *pszFile, long lLine)
{
    IStringBuffer szbOutput(128);
    szbOutput += pszFile;
    szbOutput += _T(",");
    szbOutput += lLine;
    szbOutput += _T(",delete,");
    szbOutput += IString((ULONG)pObject).d2x();
    szbOutput += _T(",");
    szbOutput += (unsigned long)nSize;
    write((const _TCHAR *)szbOutput, ITRACE_LEVEL_DEBUG);
}

#pragma warning( pop )

bool Trace::isCaptureActive(DWORD dwMask) throw()
{
    return GetDataCapture().IsCaptureActive(dwMask) != 0;
}


HTRACE Trace::initialize(const _TCHAR *pEnvName, const _TCHAR *pRegistryKey)
{
    time(&startTime);

    PTRACE pTrace = NULL;
    if (pRegistryKey == NULL)
        pTrace = m_pDefaultTrace;
    else
    {
        CTraceMap::iterator iter = m_traces.find(pRegistryKey);
        if (iter != m_traces.end())
            return iter->second->Handle();

        pTrace = new Trace;
        m_traces[pRegistryKey] = pTrace;
    }

    if (!m_bInitialized)
    {
	    tFile = _tfopen(_T("trace__.log"), _T("w"));
        m_bInitialized = true;
    }

    _ftprintf(tFile, IString(_T(__FUNCTION__)) + _T(".  pRegistryKey:  %s\n"), (pRegistryKey != NULL) ? pRegistryKey : _T("NULL"));

    clock_t millis = clock();
    time_t seconds = startTime + millis / CLK_TCK;
    millis %= (int)CLK_TCK;
    _ftprintf(tFile, _T("Starting at:  %ld%03.3ld: PID/TID:%d/%d\n")
        , seconds
        , millis
        , GetCurrentProcessId()
        , GetCurrentThreadId());

    flushall();

    {
        Trace &traceObject = *pTrace;
        
        if (!traceObject.initRegistrySettings(pRegistryKey))
            traceObject.initEnvironmentSettings(pEnvName);

        if ( traceObject.m_bUseDataCapture )
        {
            _ftprintf(tFile, _T("Using data capture\n"));

            traceObject.GetDataCapture();
        }

        else
        {
            if (traceObject.m_pszTraceFileName == NULL)
                _ftprintf(tFile, TRACE_LOG _T("= was not set.  Defaulting to OFF\n"));

            if (traceObject.threshold == 0)
                _ftprintf(tFile, TRACE_SIZE _T("= was not set.  Defaulting to 0\n"));
        }

        traceObject.debugInProgress = true;

        flushall();
    }

    return pTrace->Handle();
}


bool Trace::initEnvironmentSettings(const _TCHAR *pValueName)
{
    bool retValue = false;
    _TCHAR *traceValue = _tgetenv(pValueName);

    if (traceValue != NULL)
    {
        _ftprintf(tFile, _T("Tracing:  %s.\n"), traceValue);
        if (!_tcsicmp(traceValue, _T("OFF")))
        {
        }
        else if (!_tcsicmp(traceValue, _T("ON")))
        {
            retValue = true;
        }
        else
        {
            retValue = true;
//              traceFile = fopen("jguidll.log", "w");
            m_pszTraceFileName = new _TCHAR[_tcslen(traceValue)+1];
            _tcscpy(m_pszTraceFileName, traceValue);
            m_pszTraceFileName[_tcslen(traceValue)] = 0;
            traceFile = _tfopen(m_pszTraceFileName, _T("w"));
			_ftprintf(tFile, _T("m_pszTraceFileName=%s\n"), m_pszTraceFileName);
        }

        traceValue = _tgetenv(TRACE_SIZE);

        if (traceValue != NULL)
        {
            int multiplier;
            _TCHAR type[10];

            int fieldCount = _stscanf(traceValue, _T("%d%s"), &multiplier, &type);
            switch (fieldCount)
            {
                case 0 :
                    _tprintf(_T("Error:  No fields specified"));
                    break;
                case 1:
                    threshold = multiplier;
                    break;
                case 2 :
                    switch(type[0])
                    {
                        case 'M' :
                        case 'm' :
                            threshold = multiplier * 1024 * 1024;
                            break;
                        case 'K' :
                        case 'k' :
                            threshold = multiplier * 1024;
                            break;
                        default :
                            break;
                    }
                    break;
                default :
                    break;
                }
            _ftprintf(tFile, TRACE_SIZE _T("=%s.  threshold=%d\n"), traceValue, threshold);
        }
    }

    return retValue;
}


bool Trace::initRegistrySettings(const _TCHAR *pRegistryKey)
{
    bool retValue = false;

    if (pRegistryKey == NULL)
        return false;

    m_pszRegistryKey = new _TCHAR[_tcslen(pRegistryKey)+1];
    _tcscpy(m_pszRegistryKey, pRegistryKey);

    CRegistry stringRegistry;

    const _TCHAR *traceValue;

    // See if data capture is defined.  Use it if so.
    m_bUseDataCapture = stringRegistry.ContainsSubKey( m_pszRegistryKey, REG_KEY_DATA_CAPTURE );
    if ( m_bUseDataCapture )
    {
        traceValue = 
            CRegistryValue<const _TCHAR *>::GetRegistryValue( stringRegistry
                                        , m_pszRegistryKey
                                        , _T("title")
                                        , _T("None") );
        m_pszDataCapTitle = new _TCHAR[_tcslen(traceValue)+1];
        _tcscpy(m_pszDataCapTitle, traceValue);

        return true;
    }

    traceValue = 
        CRegistryValue<const _TCHAR *>::GetRegistryValue( stringRegistry
                                       , m_pszRegistryKey
                                       , _T("trace.log")
                                       , NULL );

    if (traceValue[0] != 0)
    {
        _ftprintf(tFile, _T("Tracing:  %s.\n"), traceValue);
        if (!_tcsicmp(traceValue, _T("OFF")))
        {
        }
        else if (!_tcsicmp(traceValue, _T("ON")))
        {
            retValue = true;
        }
        else
        {
            retValue = true;
            m_pszTraceFileName = new _TCHAR[_tcslen(traceValue)+1];
            _tcscpy(m_pszTraceFileName, traceValue);
            m_pszTraceFileName[_tcslen(traceValue)] = 0;
            traceFile = _tfopen(m_pszTraceFileName, _T("w"));
			_ftprintf(tFile, _T("m_pszTraceFileName=%s\n"), m_pszTraceFileName);
        }

        traceValue = _tgetenv(TRACE_SIZE);

        if (traceValue != NULL)
        {
            int multiplier;
            _TCHAR type[10];

            int fieldCount = _stscanf(traceValue, _T("%d%s"), &multiplier, &type);
            switch (fieldCount)
            {
                case 0 :
                    _tprintf(_T("Error:  No fields specified"));
                    break;
                case 1:
                    threshold = multiplier;
                    break;
                case 2 :
                    switch(type[0])
                    {
                        case 'M' :
                        case 'm' :
                            threshold = multiplier * 1024 * 1024;
                            break;
                        case 'K' :
                        case 'k' :
                            threshold = multiplier * 1024;
                            break;
                        default :
                            break;
                    }
                    break;
                default :
                    break;
                }
            _ftprintf(tFile, TRACE_SIZE _T("=%s.  threshold=%d\n"), traceValue, threshold);
        }
    }

    return retValue;
}


Trace &Trace::GetTraceObject(HTRACE hTrace)
{
    Trace *pTrace = FindTraceObject(hTrace);

    _ASSERTE(pTrace!=NULL);

    return *pTrace;
}


Trace *Trace::FindTraceObject(HTRACE hTrace)
{
    Trace *pTrace = NULL;
    bool bFound = false;
    
    CTraceMap::iterator iter = m_traces.begin();
    for (;
         !bFound && (iter != m_traces.end());
         iter++)
    {
        pTrace = iter->second;

        bFound = (*pTrace == hTrace);
    }

    return pTrace;
}


BOOL Trace::terminate(HTRACE hTrace)
{
    Trace *pTrace = FindTraceObject(hTrace);

    if (pTrace != NULL)
    {
        m_traces.erase(pTrace->m_pszRegistryKey);
        delete pTrace;
    }

    if (m_traces.empty())
    {
        flushall();

        if (traceFile != NULL)
        {
            fclose(traceFile);
        }

        m_bInitialized = false;
    }

    return TRUE;
}


bool Trace::operator == ( HTRACE  rhs ) const
{
    return (Handle() == rhs);
}


LPCTSTR Trace::getPrefix()
{
    static _TCHAR retValue[256];

    _itot(writeLineNumber, retValue, 10);

    return _T("");
}

/**********************************************************************
 * LogStrToFile
 *
 * log data to file
 **********************************************************************/
void Trace::logBin(LPCTSTR id, LPCTSTR pBuf, unsigned nLen, DWORD /* dwMask */) throw()
{
    dumpText(id, (const void *)pBuf, nLen);
}


IString Trace::formatLastError(LPCWSTR msg, HRESULT errorId) throw()
{
    CW2A aMsg(msg);
    return formatLastError(aMsg, errorId);
}


IString Trace::formatLastError(LPCSTR msg, HRESULT errorId) throw()
{
    LPCTSTR pArgs[2];

    CA2T sMsg(msg);
    if (msg != NULL)
        pArgs[0] = (const _TCHAR *)sMsg;
    else
        pArgs[0] = _T("");
    pArgs[1] = 0;

    TCHAR szBuffer[BUFFER_SIZE];
    szBuffer[0] = 0;

    unsigned long errId = (unsigned)errorId;

    if (errId == 0xFFFFFFFF)
    	errId = GetLastError();

    DWORD msgLength = ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM,
                                       NULL,
                                       errId,
                                       MAKELANGID (LANG_ENGLISH, SUBLANG_ENGLISH_US),
                                       szBuffer,
                                       BUFFER_SIZE,
                                       (va_list*)pArgs);
    if (msgLength == 0)
    {
        if (traceFile != 0)
            _ftprintf(traceFile, IString(_T(__FUNCTION__)) + _T(".  errId=\"") + IString(errId) 
                        + _T(".  msg=\"") + IString(pArgs[0]) 
                        + _T("\".  GetLastError()=") + IString(errId));
        _tcscpy(szBuffer, _T("No information available for:  ") + IString(errId).d2x());
    }

    return IString(szBuffer);
}


//#pragma argsused(pInfo)
IString Trace::formatLastSeh(unsigned int code, struct _EXCEPTION_POINTERS*)
{
 // IMODTRACE_DEVELOP("Trace::formatLastError()");
    IString msg(_T("Structured Exception code:  "));
    msg += IString(code).c2x();
    switch (code)
    {
        case EXCEPTION_ACCESS_VIOLATION :
            msg += _T("The thread tried to read from or write to a virtual address for which it does not have the appropriate access. ");
            break;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED :
            msg += _T("The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking. ");
            break;
        case EXCEPTION_BREAKPOINT :
            msg += _T(" A breakpoint was encountered. ");
            break;
        case EXCEPTION_DATATYPE_MISALIGNMENT :
            msg += _T(" The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on. ");
            break;
        case EXCEPTION_FLT_DENORMAL_OPERAND :
            msg += _T(" One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value. ");
            break;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO :
            msg += _T(" The thread tried to divide a floating-point value by a floating-point divisor of zero. ");
            break;
        case EXCEPTION_FLT_INEXACT_RESULT :
            msg += _T(" The result of a floating-point operation cannot be represented exactly as a decimal fraction. ");
            break;
        case EXCEPTION_FLT_INVALID_OPERATION :
            msg += _T(" This exception represents any floating-point exception not included in this list. ");
            break;
        case EXCEPTION_FLT_OVERFLOW :
            msg += _T(" The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type. ");
            break;
        case EXCEPTION_FLT_STACK_CHECK :
            msg += _T(" The stack overflowed or underflowed as the result of a floating-point operation. ");
            break;
        case EXCEPTION_FLT_UNDERFLOW :
            msg += _T(" The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type. ");
            break;
        case EXCEPTION_ILLEGAL_INSTRUCTION :
            msg += _T(" The thread tried to execute an invalid instruction. ");
            break;
        case EXCEPTION_IN_PAGE_ERROR :
            msg += _T(" The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network. ");
            break;
        case EXCEPTION_INT_DIVIDE_BY_ZERO :
            msg += _T(" The thread tried to divide an integer value by an integer divisor of zero. ");
            break;
        case EXCEPTION_INT_OVERFLOW :
            msg += _T(" The result of an integer operation caused a carry out of the most significant bit of the result. ");
            break;
        case EXCEPTION_INVALID_DISPOSITION :
            msg += _T(" An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception. ");
            break;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION :
            msg += _T(" The thread tried to continue execution after a noncontinuable exception occurred. ");
            break;
        case EXCEPTION_PRIV_INSTRUCTION :
            msg += _T(" The thread tried to execute an instruction whose operation is not allowed in the current machine mode.");
            break;
        case EXCEPTION_SINGLE_STEP :
            msg += _T(" A trace trap or other single-instruction mechanism signaled that one instruction has been executed. ");
            break;
        case EXCEPTION_STACK_OVERFLOW :
            msg += _T(" The thread used up its stack ");
            break;
        default :
            msg += _T(" Unknown error detected");
            break;
    }

    return IString(msg);
}


void Trace::logLastError(LPCTSTR msg) throw()
{ 
    try
    {
#ifdef _UNICODE
        _tprintf(_T("%s\n"), (const _TCHAR *)formatLastError(msg)); 
#else
        CA2T wmsg(msg);
        _tprintf(_T("%s\n"), (const _TCHAR *)formatLastError(const_cast<const _TCHAR *>((_TCHAR *)wmsg))); 
#endif
    }
    catch(...)
        { XTRACE_DOT_ERROR(); }
}


#define DUMP_LEN 16

#define INPUT_STRING _T("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x1a\x00")
static IString _input(INPUT_STRING, sizeof(INPUT_STRING)/sizeof(_TCHAR));

void Trace::dumpText(LPCTSTR text, const void * dump, size_t dumpLen, FILE *pFile) throw()
{
    char *index;
    char *remainder;
    ULONG   offset;

    size_t	remainderLength = dumpLen % DUMP_LEN;
    size_t   wholePartLength = dumpLen - remainderLength;
    bool remainderInTwoParts = false;

    try
    {
        IStringBuffer output;

        if (pFile == NULL)
        {
            XTRACE_DEVELOP(_T("Dumping ") + IString(text) + _T("@ address = ") + IString (reinterpret_cast<__int64>(const_cast<void *>(dump))).d2x() +
                    _T(" for 0x") + IString((long)dumpLen).d2x() + _T(" bytes."));
        }
        for (offset = 0, index = (char *)dump;
            offset < wholePartLength;
            offset += DUMP_LEN, index += DUMP_LEN)
        {

  	  	    output += _T("(");
  	  	    output += IString(offset).d2x().rightJustify(8, '0');
            output += _T(") ");
            output += IString(index, DUMP_LEN/2).c2x();
            output += _T(" ");
            output += IString(index+DUMP_LEN/2, DUMP_LEN/2).c2x();
            output += _T("  <");
            output += IString(index, DUMP_LEN).translate(_input, _T("...................")) + _T(">");

            if (pFile == NULL)
            {
                XTRACE_DEVELOP((const _TCHAR *)output);
            }
            else
            {
                _ftprintf(pFile, _T("%s\n"), (const _TCHAR *)output);
            }
            output.clear();
    //      fprintf(stderr, "(%8X)  %8x %8x %8x %8x", offset, *(int *)index, index+4, index+8, index+12);
        } /* endfor */

        if (remainderLength > 0)
	    {
            remainder = index;

            output += _T("(");
            output += IString(offset).d2x().rightJustify(8, '0') + _T(") ");
            size_t partLength = remainderLength;

            if (remainderLength > (DUMP_LEN / 2))
            {
                output += IString(index, DUMP_LEN/2).c2x().leftJustify(15, ' ');
                output += _T(" ");
                index += DUMP_LEN/2;
        	    partLength -= DUMP_LEN / 2;
                remainderInTwoParts = true;
            }

            if (partLength > 0)
            {
                output += IString(index, partLength).c2x().leftJustify(15, ' ');
                output += _T(" ");

                if (!remainderInTwoParts)
                {
                    if (partLength == 8)
                        output += _T("                ");
                    else
                        output += _T("                 ");
                }
            }

            output += _T("  <");
            output += IString(remainder, remainderLength).translate(_input, _T("..................."));
            output += _T(">");
            if (pFile == NULL)
            {
                XTRACE_DEVELOP((const _TCHAR *)output);
            }
            else
            {
                _ftprintf(pFile, _T("%s\n"), (const _TCHAR *)output);
            }
        }
    }
    catch(...)
        { _tprintf(IString(_T(__FUNCTION__)) + _T(".  Error caught")); }
}


CmDataCapture &Trace::GetDataCapture()
{
    if (m_pDC == NULL)
    {
        m_pDC = new CmDataCapture;

        CmDataCapture &cmDc = *m_pDC;
        cmDc.SetCaptureMask( 0xFFFFFFFF );
        cmDc.SetPrefix( _T("SecurityManager:") );
        cmDc.SetCaptureControl( NCRCAP_FILE_NOW );
        cmDc.SetTimeOptions( NCRCAP_TIME_MILLI + NCRCAP_TIME_TID );

        IString sKey = m_pszRegistryKey;
        sKey += _T("\\");

        if (!cmDc.ReadRegistry((const _TCHAR *)sKey, REG_KEY_DATA_CAPTURE))
        {
            _ftprintf(tFile, _T("Unable to read data capture settings from %s%s\n")
                         , m_pszRegistryKey, REG_KEY_DATA_CAPTURE );
        }

//      cmDc.Initialize(m_pszDataCapTitle);
    }

    return *m_pDC;
}
//
//
//TraceSubSystem::TraceSubSystem(const _TCHAR * name)
//{
//    Trace::initialize(name, NULL);
//}
//
//
//TraceSubSystem::~TraceSubSystem() throw()
//{
//    Trace::terminate(0);
//}

/*lint -restore */
