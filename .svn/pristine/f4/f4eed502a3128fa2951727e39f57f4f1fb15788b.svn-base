//////////////////////////////////////////////////////////////////////////////
//
// FILE:  TraceObject.H
//
// TITLE:
//
//
// AUTHOR:  DcL-
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _TraceObject
#define _TraceObject

#include "DllDefine.h"
#include "ErrorObject.h"

// Definitions

#define trace to.Trace              // set to the global trace object

#define LEVEL0  COMP_ID,T_ID,__LINE__, 0
#define LEVEL1  COMP_ID,T_ID,__LINE__, 1
#define LEVEL2  COMP_ID,T_ID,__LINE__, 2
#define LEVEL3  COMP_ID,T_ID,__LINE__, 3
#define LEVEL4  COMP_ID,T_ID,__LINE__, 4
#define LEVEL5  COMP_ID,T_ID,__LINE__, 5
#define LEVEL6  COMP_ID,T_ID,__LINE__, 6
#define LEVEL7  COMP_ID,T_ID,__LINE__, 7
#define LEVEL8  COMP_ID,T_ID,__LINE__, 8
#define LEVEL9  COMP_ID,T_ID,__LINE__, 9

#define L0  LEVEL0
#define L1  LEVEL1
#define L2  LEVEL2
#define L3  LEVEL3
#define L4  LEVEL4
#define L5  LEVEL5
#define L6  LEVEL6
#define L7  LEVEL7
#define L8  LEVEL8
#define L9  LEVEL9    

#define TRACE_FIXED_HEADER  COMP_ID, T_ID, __LINE__

#define TRACE_LEVEL_ERROR	  0x1L 
#define TRACE_LEVEL_WARNING	0x2L 
#define TRACE_LEVEL_INFO	  0x4L 
#define TRACE_LEVEL_DEBUG	  0x8L 

#define traceNew   to.TraceNew              // set to the global trace object
#define traceMem   to.TraceMemUsage
#define traceStack to.TraceStackDump

const int MAX_TRACE_BUF = 3000;
     // max length of any one line
#define MAX_SAVED_TRACES 10

//+THD Performance RFC
enum ThreadEvents {
	PAUSE = 0,
	RESUME,
	STOP,
	PAUSESUCCESSFUL,
	MAX_THREAD_EVENTS
};
//-THD Performance RFC
//class ErrorObject ;

//////////////////////////////////////////////////////////////////////////////
// TraceObject command target

class DLLEXPORT
TraceObject : public CObject
{
public:
  // tracing is off if the trace level is -1
  // tracing get more detailed as the level increases 0,1,2,3...9
  friend class ConfigObject;
  friend class SMSmSystemFunctionsBase ;

  int   nCurrentLevel;              // current trace level -1,0...9
  int   nFocusLevel;                // focused trace level
  long  lFocusComps;                // focus on comp_id mask
  int   m_nMaxLogSize;                // Maximum size of traces.log(KB).  Eventually, it should replace MaxNumberofEntries ;
  int   m_nEntriesToFlush;         // running count indicating number of lines before flush
  bool   m_bTraceOn;                 // tracing flag
  
  int   nSavedLine[MAX_SAVED_TRACES]; // array of last MAX_SAVED_TRACES traces call source line numbers
  CString csSavedTid[MAX_SAVED_TRACES];	// array of last MAX_SAVED_TRACES traces call source trace IDs
  int nSavedEntry;                  // pointer to last traves entry in these arrays
	
  CString m_strTraceFileName;
  CStdioFile m_fileTrace;
	
protected:
  bool  fFirstTime;           //Is it first time to trace
  bool  m_bTraceFileOpen  ;   // flag indicating if the file has been opened or not

  bool m_bFirstMemTime;
  bool m_bHavePSAPI;
  HMODULE m_hPSAPI;
  HANDLE m_hProcess;
  FARPROC m_pMemFunc;

  DWORD m_dwTraceLevels ;     //
  int   m_nCountOfEntries ;    // running count indicating number of lines everytime scotapp runs
  ErrorObject m_eoServices;  

  bool m_bInitialized;
  static TraceObject* ListofAllTraceObjs[100];
  static short        nTraceObjectCount;

  bool OpenTraceFile() ;
  //void TraceFileFlush() ;
  void BackupTraceFile() ;
public:
  TraceObject(LPCTSTR);                    // constructor sets file name and init size
  ~TraceObject();                   // destructor closes open file
	
  int FlushAllObjects(); // only to use by SCOTApp, not to be used by any another module
  bool InitRegistrySettings() ;
  void CloseTraceFile() ;           // will be called from the outside
  int Trace(const long,             // COMP_ID
    LPCTSTR,                         // T_ID, trace ID
    const int,                      // source file line number
    const int,                      // trace level 0,1,2...9
    LPCTSTR,                         // prntf fmt string
    ...);                           // variabler args
  int HexTrace(const long,          // COMP_ID
    LPCTSTR,                         // T_ID, trace ID
    const int,                      // source file line number
    const int,                      // trace level 0,1,2...9
    LPCTSTR,                         // title string
    LPCTSTR,                         // pointer to buffer
    const int);                     // length of buffer

  // new Traces
  int TraceNew(const long	lCompID,	// COMP_ID
			LPCTSTR		  pszSourceName,      // T_ID, trace ID
			const int	  nSourceLineNum ,    // source file line number
			const long	lTraceLevel,        // trace level 
			LPCTSTR		  pszFormat,          // prntf fmt string
			...);                         // variabler args

  int TraceMemUsage(const long lCompId,
                    LPCTSTR szTrcId,
                    const int nSrcLine,
                    const int nLevel);

  int TraceStackDump(const long lCompId,
                     LPCTSTR szTrcId,
                     const int nSrcLine,
                     const int nLevel);
  void UnInitialize();
  //+THD Performance RFC
  void SetTimeToFlush( int nMilliseconds );
  void TraceFileFlush() ;
  //-THD Performance RFC
private:
	//+THD Performance RFC
	CWinThread *m_pTimerThread;
	static UINT TimerThreadEntry( LPVOID pParam );	
	void TimerThread();
	void PauseTimerThread();
	void ResumeTimerThread();
	void StopTimerThread();
	int m_nMillisecondsToFlush;
	HANDLE m_hThreadEvents[ MAX_THREAD_EVENTS ];
	bool m_bFlushFromTimerThread;
	//-THD Performance RFC
//  void Close(void);                 // close the trace file
};

#endif
