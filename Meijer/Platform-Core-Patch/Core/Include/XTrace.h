// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/Include/XTrace.h 1     6/23/08 4:55p Sa250050 $
#pragma once
/****************************************************************************/
/*                                                                          */
/* Include File Name:         xtrace.h                                       */
/*                                                                          */
/* Include Title:             This file contains the definitions required   */
/*                            for implementing the tracing functions.       */
/*                                                                          */
/* Status:                    Release 1.0                                   */
/*                                                                          */
/* Notes:                                                                   */
/*   - Dependencies:          <insert dependencies here>                    */
/*   - Restrictions:          <insert restrictions here>                    */
/*                                                                          */
/* Function:                  <explanation of use of these defines>         */
/*                                                                          */
/****************************************************************************/
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/Include/XTrace.h $
 * 
 * 1     6/23/08 4:55p Sa250050
 * FastLane 4.0 E5.3 Build 291 - Core Source
 * 
 * 9     6/14/05 8:57a Dm185016
 * Added nested exception tracing for internal exceptions
 * 
 * 7     5/01/05 9:00a Tp151000
 * Performmance
 * 
 * 6     4/21/05 7:31p Dm185016
 * Added new tracing macro
 * 
 * 4     3/24/05 7:37p Dm185016
 * UNICODE
 * 
 * 3     2/28/05 3:39p Dm185016
 * lint
 * 
 * 2     2/21/05 8:00p Dm185016
 * Added debugging information to new and delete for pooled objects.
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 20    1/18/05 4:06p Dm185016
* UNICODE support
* 
* 19    10/29/04 9:34a Dm185016
* Added missing L9 mask.
* 
* 18    10/05/04 8:28a Dm185016
* Trace object no longer shared.  Trace rewritten from collection of
* static methods to real object.  Access to Trace object controlled via
* handles.
* 
* 16    7/13/04 3:16p Dm185016
* Fix to prevent null registry key from crashing
* 
* 15    6/09/04 3:31p Dm185016
* Fixed application trace datacapture values so that tracing happens
* again.
* 
* 14    6/08/04 3:19p Dm185016
* lint changes
* Hooks to Loggable for data capture
* 
* 12    5/06/04 8:08a Tk124825
* changed form _SCOTSSF to _SCOTSSF_
* 
* 11    5/04/04 6:11p Tk124825
* made the necessary changes for unicode build
* 
* 10    4/20/04 4:04p Tk124825
* Changed to _SCOTSSF_
* 
* 9     4/20/04 3:35p Dm185016
* Fixed compile problem with SSF
* 
* 8     4/20/04 2:26p Dm185016
* Added #ifdef around #define trace.  Not needed for scotapp stuff
* 
* 7     4/12/04 3:33p Dm185016
* Added DataCapture support
* 
* 6     3/19/04 3:33p Dm185016
* Added trace settings from the Registry
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
* Fixed header problems
* 
* 1     1/06/04 9:00a Dm185016
* First Version
*/

//lint -emacro( *, IMODTRACE_DEVELOP )
//lint -emacro( *, ITRACE_DEVELOP )
//lint -emacro( *, ITRACE_ERROR )
//lint -emacro( *, ITRACE_DOT_ERROR )
//lint -emacro( *, ITRACE_PRODUCTION )
//lint -emacro( *, trace )

#include <time.h>
#include "StringComparator.h"
#include "xstring.h"
#include "Utils.h"
#include <stdexcept>
#include <list>
#include "Traceable.h"

#ifndef XQUOTE
#define XQUOTE(y) _T(#y)
#define QUOTE(x) XQUOTE(x)
#endif

#define PTRACE(x) if (debugLevel != 0) { x; }

//--------------------------------------------------------------
// The following macros are defined only when tracing is enabled
//--------------------------------------------------------------
#ifdef IC_TRACE_DEVELOP
#	ifdef _MSC_VER
#		define IMODTRACE_DEVELOP() \
		   	MethodTrace trc(GetTraceObject(), _T(__FUNCTION__),  __LINE__ )
#		define IMODTRACE_DEVELOP_STATIC(HTRACE_VALUE) \
            MethodTrace trc(Trace::GetTraceObject(HTRACE_VALUE), _T(__FUNCTION__),  __LINE__ )
#		define IMODTRACE_DEVELOP_EX(ENTRY_MASK, EXIT_MASK) \
		   	MethodTrace trc(GetTraceObject(), _T(__FUNCTION__),  __LINE__, ENTRY_MASK, EXIT_MASK )
#	else
#		define IMODTRACE_DEVELOP(modname) \
		   	MethodTrace trc(GetTraceObject(), modname,  QUOTE(__LINE__) )
#		define IMODTRACE_DEVELOP_EX(ENTRY_MASK, EXIT_MASK) \
		   	MethodTrace trc(GetTraceObject(), modname,  QUOTE(__LINE__), ENTRY_MASK, EXIT_MASK )
#	endif

   #define IFUNCTRACE_DEVELOP() \
   	Invalid macro
   #define ITRACE_INPROGRESS \
     (GetTraceObject().debugInProgress)
   #define ITRACE_DEVELOP(p1) \
   	if (GetTraceObject().debugInProgress) GetTraceObject().write((p1), ITRACE_LEVEL_DEBUG)
   #define ITRACE_DEVELOP_STATIC(hTrace, p1) \
   {\
    Trace &traceObject = Trace::GetTraceObject(hTrace);\
   	if (traceObject.debugInProgress) traceObject.write((p1), ITRACE_LEVEL_DEBUG);\
   }
    #define ITRACE_NEW_STATIC(hTrace, pObject, oSize, pszFile, lLine)\
    {\
        Trace &traceObject = Trace::GetTraceObject(hTrace);\
        if (traceObject.debugInProgress) traceObject.traceNew(pObject, oSize, pszFile, lLine);\
    }
    #define ITRACE_DELETE_STATIC(hTrace, pObject, oSize, pszFile, lLine)\
    {\
        Trace &traceObject = Trace::GetTraceObject(hTrace);\
        if (traceObject.debugInProgress) traceObject.traceDelete(pObject, oSize, pszFile, lLine);\
    }
    #define ITRACE_DELETE2_STATIC(hTrace, pObject, oSize)\
    {\
        Trace &traceObject = Trace::GetTraceObject(hTrace);\
        if (traceObject.debugInProgress) traceObject.traceDelete(pObject, oSize);\
    }
    #define XTRACE_DEVELOP(p1) \
   	write((p1), ITRACE_LEVEL_DEBUG)
    #define ITRACE_DUMP(id, ptr, length) \
	   if (GetTraceObject().debugInProgress) GetTraceObject().logBin(id, ptr, length, ITRACE_LEVEL_DEBUG)
    #define ITRACE_DUMP_STATIC(hTrace, id, ptr, length) \
    {\
        Trace &traceObject = Trace::GetTraceObject(hTrace);\
	    if (traceObject.debugInProgress) traceObject.logBin(id, ptr, length, ITRACE_LEVEL_DEBUG);\
    }
   #define ITRACE_DUMP_PRODUCTION(id, ptr, length) \
	   GetTraceObject().logBin(id, ptr, length, ITRACE_LEVEL_ALWAYS)
   #ifndef IC_TRACE_RUNTIME
     	#define IC_TRACE_RUNTIME
   #endif
	#define ITRACE_FLUSH() \
    	flushall();
	#define ITRACE_VARIABLE(var)\
		ITRACE_DEVELOP(QUOTE(var) _T("=") + IString((ULONG)var).d2x());
#else
    #define IMODTRACE_DEVELOP()
    #define IFUNCTRACE_DEVELOP()
    #define ITRACE_INPROGRESS {}
    #define ITRACE_DEVELOP(p1) {}
    #define XTRACE_DEVELOP(p1) {}
    #define ITRACE_DUMP(id, ptr, length)
    #define ITRACE_FLUSH() {}
    #define ITRACE_VARIABLE(var) {}
#endif

//#define _USE_TRACE() TraceSubSystem tss

//--------------------------------------------------------------
// The following are always defined
//--------------------------------------------------------------
#define ITRACE_ERROR(p1) \
    GetTraceObject().write(_T("File:  ") + IString(_T(__FILE__)) + _T(".  Function:  ") + IString(_T(__FUNCTION__)) + _T(".  Line:  ") + IString(__LINE__) + IString(_T(".  ERROR:  ")) + (p1), ITRACE_LEVEL_ERROR)
#define ITRACE_ERROR_STATIC(hTrace, p1) \
    {\
    Trace &traceObject = Trace::GetTraceObject(hTrace);\
    traceObject.write(_T("File:  ") + IString(_T(__FILE__)) + _T(".  Function:  ") + IString(_T(__FUNCTION__)) + _T(".  Line:  ") + IString(__LINE__) + IString(_T(".  ERROR:  ")) + (p1), ITRACE_LEVEL_ERROR);\
    }
#define XTRACE_ERROR(p1) \
    write(_T("File:  ") + IString(_T(__FILE__)) + _T(".  Function:  ") + IString(_T(__FUNCTION__)) + _T(".  Line:  ") + IString(__LINE__) + IString(_T(".  ERROR:  ")) + (p1), ITRACE_LEVEL_ERROR)
#define ITRACE_DOT_ERROR() \
    GetTraceObject().write(_T("File:  ") + IString(_T(__FILE__)) + _T(".  Function:  ") + IString(_T(__FUNCTION__)) + _T(".  Line:  ") + IString(__LINE__) + IString(_T(".  ERROR:  Unknown exception caught")), ITRACE_LEVEL_ERROR)
#define ITRACE_DOT_ERROR_STATIC(hTrace) \
    Trace::GetTraceObject(hTrace).write(_T("File:  ") + IString(_T(__FILE__)) + _T(".  Function:  ") + IString(_T(__FUNCTION__)) + _T(".  Line:  ") + IString(__LINE__) + IString(_T(".  ERROR:  Unknown exception caught")), ITRACE_LEVEL_ERROR)
#define XTRACE_DOT_ERROR() \
    write(_T("File:  ") + IString(_T(__FILE__)) + _T(".  Function:  ") + IString(_T(__FUNCTION__)) + _T(".  Line:  ") + IString(__LINE__) + IString(_T(".  ERROR:  Unknown exception caught")), ITRACE_LEVEL_ERROR)
#define ITRACE_INFORMATIONAL(p1) \
    GetTraceObject().write(IString(_T("INFO:  ")) + (p1), ITRACE_LEVEL_INFORMATIONAL)
#define ITRACE_WARNING(p1) \
    GetTraceObject().write(IString(_T("WARNING:  ")) + (p1), ITRACE_LEVEL_WARNING)
#define ITRACE_EXCEPTION(e) \
    CBaseObject::GetTraceObject().logException(e, _T(__FILE__), _T(__FUNCTION__), __LINE__)
#define ITRACE_EXCEPTION_STATIC(hTrace, e) \
    Trace::GetTraceObject(hTrace).logException(e, _T(__FILE__), _T(__FUNCTION__), __LINE__)
#define XTRACE_EXCEPTION(e) \
    logException(e, _T(__FILE__), _T(__FUNCTION__), __LINE__)

#define ITRACE_LASTERROR(var)\
    GetTraceObject().write(QUOTE(var) _T(":  ") + Trace::formatLastError(QUOTE(var), ::GetLastError()), ITRACE_LEVEL_ERROR);
#define ITRACE_PRODUCTION(p1) \
    GetTraceObject().write(p1, ITRACE_LEVEL_ALWAYS)

#include <map>

#define TRACE_LOG   _T("trace.log")
#define TRACE_SIZE  _T("trace.size")
#define DEFAULT_REGISTRY_KEY _T("SOFTWARE\\NCR\\")
#define REG_KEY_DATA_CAPTURE _T("DataCapture")

#define L0_MASK 0x80000000
#define L1_MASK 0x40000000
#define L2_MASK 0x20000000
#define L3_MASK 0x10000000
#define L4_MASK 0x08000000
#define L5_MASK 0x04000000
#define L6_MASK 0x02000000
#define L7_MASK 0x01000000
#define L8_MASK 0x00800000
#define L9_MASK 0x00400000

#if !defined(_SCOT) && !defined(_SCOTSSF_)
    #define LEVEL0  0,_T(__FUNCTION__),__LINE__, 0
    #define LEVEL1  0,_T(__FUNCTION__),__LINE__, 1
    #define LEVEL2  0,_T(__FUNCTION__),__LINE__, 2
    #define LEVEL3  0,_T(__FUNCTION__),__LINE__, 3
    #define LEVEL4  0,_T(__FUNCTION__),__LINE__, 4
    #define LEVEL5  0,_T(__FUNCTION__),__LINE__, 5
    #define LEVEL6  0,_T(__FUNCTION__),__LINE__, 6
    #define LEVEL7  0,_T(__FUNCTION__),__LINE__, 7
    #define LEVEL8  0,_T(__FUNCTION__),__LINE__, 8
    #define LEVEL9  0,_T(__FUNCTION__),__LINE__, 9
    //#define LEVEL0  COMP_ID,T_ID,__LINE__, 0
    //#define LEVEL1  COMP_ID,T_ID,__LINE__, 1
    //#define LEVEL2  COMP_ID,T_ID,__LINE__, 2
    //#define LEVEL3  COMP_ID,T_ID,__LINE__, 3
    //#define LEVEL4  COMP_ID,T_ID,__LINE__, 4
    //#define LEVEL5  COMP_ID,T_ID,__LINE__, 5
    //#define LEVEL6  COMP_ID,T_ID,__LINE__, 6
    //#define LEVEL7  COMP_ID,T_ID,__LINE__, 7
    //#define LEVEL8  COMP_ID,T_ID,__LINE__, 8
    //#define LEVEL9  COMP_ID,T_ID,__LINE__, 9

    #define L0  0,_T(__FUNCTION__),__LINE__, L0_MASK
    #define L1  0,_T(__FUNCTION__),__LINE__, L1_MASK
    #define L2  0,_T(__FUNCTION__),__LINE__, L2_MASK
    #define L3  0,_T(__FUNCTION__),__LINE__, L3_MASK
    #define L4  0,_T(__FUNCTION__),__LINE__, L4_MASK
    #define L5  0,_T(__FUNCTION__),__LINE__, L5_MASK
    #define L6  0,_T(__FUNCTION__),__LINE__, L6_MASK
    #define L7  0,_T(__FUNCTION__),__LINE__, L7_MASK
    #define L8  0,_T(__FUNCTION__),__LINE__, L8_MASK
    #define L9  0,_T(__FUNCTION__),__LINE__, L9_MASK
#endif

//struct _ThreadVariableStruct
//{
//    int indentLevel;
//};

//typedef struct _ThreadVariableStruct ThreadVariableStruct;

typedef std::map <int, int, std::less<int> > ThreadMap;

class CmDataCapture;
class ILoggable;
class CLoggableManager;
class IException;

//#if !defined(_SCOT) && !defined(_SCOTSSF_)
//
//class UTILS_API TraceSubSystem
//{
//    public:
//
//        TraceSubSystem(const _TCHAR * name = _T(TRACE_LOG));
//        virtual ~TraceSubSystem() throw();
//
//};
//#endif

//----------------------------------------------------
// Masks available for Data Capture
//
enum DataCaptureMasks
{
    eDCWin32Utils       = 0x00000001,     // Use for Win32 related features
    eDCAllocations      = 0x00000002,     // Use for xnew allocation tracing
    eDCContentHandler   = 0x00000004,     // Use to suppress content handler tracing 
    eDCPool             = 0x00000008,     // Use to suppress Pooled operations
    eDCTokenizing       = 0x00000010,     // Use to suppress Tokenizer/iterator operations
    eDCXml              = 0x00000020,     // Use to suppress XML debug operations
    eDCEntry            = 0x00000040,     // Use to suppress Pooled operations
    eDCExit             = 0x00000080,     // Use to suppress Pooled operations
} ;

// Constants for use with the datacapture facility
const DWORD ITRACE_LEVEL_ERROR          =  0x10000000;
const DWORD ITRACE_LEVEL_WARNING        =  0x20000000;
const DWORD ITRACE_LEVEL_INFORMATIONAL  =  0x40000000;
const DWORD ITRACE_LEVEL_DEBUG          =  0x80000000;
const DWORD ITRACE_LEVEL_ALWAYS         =  0x08000000;

#define MASK_RESERVED_FOR_UTILS 0x000000FF
#define MASK_AVAILABLE_TO_APP   0xffffff00
//
// All other mask bits are available
//----------------------------------------------------



#pragma warning( push )
#pragma warning( disable : 4251 )

class UTILS_API MethodTrace;
class UTILS_API Trace;
typedef Trace *PTRACE;
//typedef std::list<PTRACE> CTraceList;
typedef std::map<StringType, PTRACE, StringComparator> CTraceMap;
typedef long HTRACE;

class UTILS_API Trace
{
public:

    friend class UTILS_API MethodTrace;

    virtual ~Trace();

    inline bool debug() const { return debugInProgress; }
    inline void setDebug(bool newDebug) { debugInProgress = newDebug; }
    static IString formatLastError(LPCSTR, HRESULT errorId = -1) throw();
    static IString formatLastError(LPCWSTR, HRESULT errorId = -1) throw();
    static IString formatLastSeh(unsigned int, struct _EXCEPTION_POINTERS*);
    void logLastError(LPCTSTR) throw();
    void logBin(LPCTSTR, LPCTSTR, unsigned, DWORD) throw();
    void write(const IString &msg, DWORD mask) throw();
    void write(LPCSTR, DWORD) throw();
    void write(LPCWSTR, DWORD) throw();
	void write(const std::string &, DWORD) throw();
	void write(const std::wstring &, DWORD) throw();
	void write(long,             // COMP_ID
                LPCTSTR,          // T_ID, trace ID
                int,              // source file line number
                DWORD,            // trace level 0,1,2...9
                LPCTSTR,          // prntf fmt string
                ...) throw();		// variable args
    void logException(const std::exception &e, const _TCHAR *, const _TCHAR *, int) throw();
    void logException(const IException &e, const _TCHAR *, const _TCHAR *, int) throw();

    void traceNew( void *, size_t, const _TCHAR *pszFile, long lLine );
    void traceDelete(void *, size_t, const _TCHAR *pszFile = _T("*"), long lLine = -1);

    bool isCaptureActive(DWORD dwMask) throw();
    
    LPCTSTR getPrefix();
	
    void dumpText(LPCTSTR, const void *, size_t, FILE *pFile=NULL) throw();

    bool debugInProgress;

    static HTRACE initialize(LPCTSTR pValueName = TRACE_LOG
                            , LPCTSTR pRegistryKey = NULL);
    static BOOL terminate(HTRACE);

    HTRACE Handle() const { return m_hTrace; }

    static Trace &GetTraceObject(HTRACE hTrace);
    
    static Trace *FindTraceObject(HTRACE hTrace);

    ILoggable &getLoggable(const _TCHAR *pName = NULL);

    ILoggable &getLoggable(DWORD);

    static ILoggable &getLoggable(HTRACE hTrace, const _TCHAR * = NULL);

    static ILoggable &getLoggable(HTRACE hTrace, DWORD);

protected :

    void write0(LPCTSTR, DWORD) throw();

    void incLevel();
    void decLevel();

    bool initRegistrySettings(const _TCHAR *);
    bool initEnvironmentSettings(const _TCHAR *);

    CmDataCapture &GetDataCapture();

    bool operator == ( HTRACE ) const;

private :

	Trace();
    Trace(const Trace &);
    Trace &operator= (const Trace&);

    static bool m_bInitialized;
    static FILE *traceFile;
    FILE *allocFile;
    int  writeLineNumber;
    int  fileSize;
    int  threshold;
    _TCHAR *m_pszDataCapTitle;
    _TCHAR *m_pszRegistryKey;
    _TCHAR *m_pszTraceFileName;

    CmDataCapture *m_pDC;
    bool m_bUseDataCapture;

	static ThreadMap indentLevels;

    //static CTraceList traces;
    static CTraceMap m_traces;
    static PTRACE   m_pDefaultTrace;

    HTRACE  m_hTrace;

    CLoggableManager *m_pLoggableMgr;

    static LONG m_lInstanceCount;
};

#pragma warning( pop )

class UTILS_API MethodTrace
{
public:

    MethodTrace(Trace &trace
        , const _TCHAR *
        , LPCTSTR
        , DWORD dwMaskEntry = eDCEntry
        , DWORD dwMaskExit = eDCExit
        ) throw();
    MethodTrace(Trace &trace
        , const _TCHAR *
        , int
        , DWORD dwMaskEntry = eDCEntry
        , DWORD dwMaskExit = eDCExit
        ) throw();
    virtual ~MethodTrace();

private:

    MethodTrace(const MethodTrace &);
    MethodTrace &operator =(const MethodTrace &); // Hidden assignment operator

    IString m_modName;
    DWORD m_dwMaskEntry;
    DWORD m_dwMaskExit;

    Trace &m_trace;

};


#if !defined(_SCOT) && !defined(_SCOTSSF_)
    #define trace GetTraceObject().write
#endif
