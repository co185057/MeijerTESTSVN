/* ======================================================================= */
/* =              Copyright (c) NCR Corporation 1989-2001                = */
/* ======================================================================= */
/* DCapApi.h 160.8 */
/****************************************************************************

Author: 
   Paul M. Parks (pp230000) 
   paul@parkscomputing.com

Purpose: 
   This is a tracing library for capturing program state to a file or to 
   debug output to simplify analysis of runtime problems, particularly in 
   production environments. It provides an API for creating capture instances 
   and writing entries to an instance.

   Replaces DCap.dll version 2.x, which was an adaptation of CMDCap.dll 1.x 
   for general use in ACS. 

Comments: 

*****************************************************************************/

#ifndef H_CMDCAP_API
#define H_CMDCAP_API

#ifndef DCAP_DISABLED

#define DCAP_ENABLED 1

#ifdef CMDCAP_EXPORTS
#define CMDCAP_API __declspec(dllexport)
#else
#define CMDCAP_API __declspec(dllimport)
#endif

#include "windows.h"

#ifdef __cplusplus
extern "C"
{
#endif


/* New 3.0 API */

/* 
Get the version of Data Capture that is currently loaded. 

Parameters:

  pMajor:  Pointer to SHORT that will receive the major version number. 
      May be NULL if the value is not needed.

  pMinor: Pointer to SHORT that will receive the minor version number.
      May be NULL if the value is not needed.

  pRevision: Pointer to SHORT that will receive the revision number. 
      May be NULL if the value is not needed.
      
Returns TRUE if values were successfully retrieved, FALSE otherwise.
*/
CMDCAP_API BOOL DcGetVersion(
   SHORT* pMajor,
   SHORT* pMinor,
   SHORT* pRevision);

/* 
Create a new data capture instance.

Parameters:

  idString: A null-terminated string that will name the instance. 

  initString: A null-terminated string that will be written at the beginning of 
      the log.

Returns a HANDLE to the new instance. This handle will be passed to the 
rest of the DCap API functions to identify the new instance. 
*/
CMDCAP_API HANDLE DcCreateInstance(
   LPCSTR idString,  
   LPCSTR initString);

/*
Close an existing data-capture instance.

Parameters:
   
  hCap: A HANDLE to a data-capture instance.

*/
CMDCAP_API void DcCloseInstance(
   HANDLE hCap);

/*
Write a trace entry.

Parameters:

  hCap: A HANDLE to a data-capture instance.

  mask: The capture mask that must be set in the specified data 
      capture instance in order for the entry to be written.

  context: A null-terminated string identifying the program-specific 
      context in which the entry was recorded, or NULL if not needed.
   
  msg: A null-terminated string containing the message to be written.

*/
CMDCAP_API void DcWrite(
   HANDLE hCap,
   DWORD mask, 
   LPCSTR context, 
   LPCSTR msg);

/*
Write a trace entry as a formatted string.

Parameters:

  hCap: A HANDLE to a data-capture instance.

  mask: The capture mask that must be set in the specified data 
      capture instance in order for the entry to be written.

  context: A null-terminated string identifying the program-specific 
      context in which the entry was recorded, or NULL if not needed.
   
  fmt: A null-terminated string containing the format string to be 
      filled in with the remaining parameters.

  ...: A list of parameters to substitute into the format string.

*/
CMDCAP_API void DcWritef(
   HANDLE hCap,
   DWORD mask, 
   LPCSTR context, 
   LPCSTR fmt,
   ...);

/*
Write a trace entry as a byte dump of a block of memory.

Parameters:

  hCap: A HANDLE to a data-capture instance.

  mask: The capture mask that must be set in the specified data 
      capture instance in order for the entry to be written.

  context: A null-terminated string identifying the program-specific 
      context in which the entry was recorded, or NULL if not needed.
   
  msg: A null-terminated string containing the message to be written.

  bytes: A pointer to a block of memory to be dumped to the log.

  charCount: The number of bytes to be dumped from the block of memory.

*/
CMDCAP_API void DcByteDump(
   HANDLE hCap,
   DWORD mask, 
   LPCSTR context,
   LPCVOID bytes, 
   DWORD charCount,
   LPCSTR msg);

/*
Write a trace entry as a formatted string.

Parameters:

  hCap: A HANDLE to a data-capture instance.

  mask: The capture mask that must be set in the specified data 
      capture instance in order for the entry to be written.

  context: A null-terminated string identifying the program-specific 
      context in which the entry was recorded, or NULL if not needed.
   
  fmt: A null-terminated string containing the format string to be 
      filled in with the remaining parameters.

  bytes: A pointer to a block of memory to be dumped to the log.
  
  charCount: The number of bytes to be dumped from the block of memory.

  ...: A list of parameters to substitute into the format string.

*/
CMDCAP_API void DcByteDumpf(
   HANDLE hCap,
   DWORD mask, 
   LPCSTR context,
   LPCVOID bytes, 
   DWORD charCount,
   LPCSTR fmt,
   ...);

/*
Flush all buffered entries.

Parameters:

  hCap: A HANDLE to a data-capture instance.

*/
CMDCAP_API BOOL DcFlush(
   HANDLE hCap);

/*
Read settings from the registry.

Parameters:

  hCap: A HANDLE to a data-capture instance.

  baseKeyName: Path to the enclosing key in the registry.

  keyName: Name of the key containing the capture settings.

Returns TRUE if settins were read successfully, FALSE otherwise.
*/
CMDCAP_API BOOL DcReadSettings(
   HANDLE hCap,
   LPCSTR baseKeyName, 
   LPCSTR keyName);

/*
Write current settings to the registry.

Parameters:

  hCap: A HANDLE to a data-capture instance.

*/
CMDCAP_API BOOL DcWriteSettings(
   HANDLE hCap);

/*
Set the flags that control where log entries are recorded.

Parameters:

  hCap: A HANDLE to a data-capture instance.

  destFlags: A bit mask that control the output settings.

      1 = Delayed write to data capture application
      2 = Delayed write to debug output
      4 = Delayed write to file
      10 = Immediate write to data capture application
      20 = Immediate write to debug output
      40 = Immediate write to file
      
Returns the previous bit mask.
*/
CMDCAP_API DWORD DcSetDest(
   HANDLE hCap,
   DWORD destFlags);

/*
Get the flags that control where entries are recorded.

Parameters:

  hCap: A HANDLE to a data-capture instance.

Returns the current bit mask.
*/
CMDCAP_API DWORD DcGetDest(
   HANDLE hCap);

/*
Set the mask that controls which trace entries are recorded and 
which are ignored.

Parameters:

  hCap: A HANDLE to a data-capture instance.

  mask: A bit mask that sets the bits that a trace entry must match 
      in order for the entry to be written.

      0x00000000 = Off
      0x0000000F = Error
      0x000000FF = Warning
      0x00000FFF = Audit
      0x0000FFFF = Info
      0x000FFFFF = Verbose
      0x00FFFFFF = Debug
      0xFFFFFFFF = All messages

      User-defined bit patterns may also be used.

Returns the previous bit mask.
*/
CMDCAP_API DWORD DcSetMask(
   HANDLE hCap,
   DWORD mask);

/*
Get the mask that controls which trace entries are recorded and 
which are ignored.

Parameters:

  hCap: A HANDLE to a data-capture instance.

Returns the current bit mask.
*/
CMDCAP_API DWORD DcGetMask(
   HANDLE hCap);

/*
Set the name of the file to which trace entries are written, if any.

Parameters:

  hCap: A HANDLE to a data-capture instance.

  fSpec: The path of the file to which log entries will be written, or 
      NULL if no capture file is required.

Returns TRUE if successful, FALSE otherwise.
*/
CMDCAP_API BOOL DcSetFilePath(
   HANDLE hCap,
   LPCSTR fSpec);

/*
Get the name of the file to which trace entries are written, if any.

Parameters:

  hCap: A HANDLE to a data-capture instance.

Returns the current file path.
*/
CMDCAP_API LPCSTR DcGetFilePath(
   HANDLE hCap);

/*
Set the maximum size, in KB, to which a log file may grow before it 
is written to a backup and a new file is started.

Parameters:

  hCap: A HANDLE to a data-capture instance.

  maxKSize: The maximum size, in KB, to which a log file is allowed 
      to grow.

Returns the current maximum file size.
*/
CMDCAP_API DWORD DcSetFileSize(
   HANDLE hCap,
   DWORD maxKSize);

/*
Get the maximum size, in KB, to which a log file may grow before it 
is written to a backup and a new file is started.

Parameters:

  hCap: A HANDLE to a data-capture instance.

Returns the current maximum file size.
*/
CMDCAP_API DWORD DcGetFileSize(
   HANDLE hCap);

/*
Set the prefix to be written before each trace entry.

Parameters:

  hCap: A HANDLE to a data-capture instance.

  prefix: A null-terminated string, or NULL.

Returns TRUE if successful, FALSE otherwise.
*/
CMDCAP_API BOOL DcSetPrefix(
   HANDLE hCap,
   LPCSTR prefix);

/*
Get the prefix to be written before each trace entry.

Parameters:

  hCap: A HANDLE to a data-capture instance.

Returns the current prefix.
*/
CMDCAP_API LPCSTR DcGetPrefix(
   HANDLE hCap);

/*
Set the bit mask that controls the stamp that will be placed on each 
trace entry.

Parameters:

  hCap: A HANDLE to a data-capture instance.

  flags: The bit mask containing the flags to set.

      100 = Current date in MM/DD format
      300 = Current date in YYYY/MM/DD format
      010 = Current time in hh:mm:ss format (hour, minutes, seconds)
      030 = Current hh:mm:ss.SSS format (hour, minutes, seconds, milliseconds)
      001 = ID of thread that created the entry
      1000 = Capture mask at which the log entry was created.

Returns the current bit mask.
*/
CMDCAP_API DWORD DcSetStamp(
   HANDLE hCap,
   DWORD flags);

/*
Get the bit mask that controls the stamp that will be placed on each 
trace entry.

Parameters:

  hCap: A HANDLE to a data-capture instance.

Returns the current bit mask.
*/
CMDCAP_API DWORD DcGetStamp(
   HANDLE hCap);

/*
Determine if the requested capture mask is currently set. If this function 
returns TRUE, then a log entry written for that mask will be recorded.

Parameters:

  hCap: A HANDLE to a data-capture instance.

  mask: A bit mask that contains the bits to test.
  
      0x00000000 = Off
      0x0000000F = Error
      0x000000FF = Warning
      0x00000FFF = Audit
      0x0000FFFF = Info
      0x000FFFFF = Verbose
      0x00FFFFFF = Debug
      0xFFFFFFFF = All messages
    
      User-defined bit patterns may also be used.
      
Returns TRUE if the bit mask matches the current trace mask, and FALSE 
otherwise.
*/
CMDCAP_API BOOL DcIsMaskSet(
   HANDLE hCap,
   DWORD mask);


/*
Get the name of the instance that was provided as the idString parameter 
in the call to DcCreateInstance.

Parameters:

  hCap: A HANDLE to a data-capture instance.

Returns the instance ID string, or an empty string if no instance is 
found for the requested handle.
*/
CMDCAP_API LPCSTR DcGetInstanceID(
   HANDLE hCap);



/************************************************************************
Old, deprecated API. This API is still supported, but calls to all of 
these functions are now forwarded to the new API described above. 
Please use only the new API in new code.
************************************************************************/

/* DEPRECATED: Use DcByteDump instead. */
CMDCAP_API BOOL CMDCapCapture(
   HANDLE hCap,
   DWORD mask, 
   LPCSTR capString,
   LPCVOID bytes, 
   DWORD byteCount
   );

/* DEPRECATED: Use DcCloseInstance instead. */
CMDCAP_API BOOL CMDCapCleanup(
   HANDLE hCap
   );

/* DEPRECATED: Use DcWritef instead */
CMDCAP_API BOOL CMDCapDataPrintf(
   HANDLE hCap,
   DWORD mask, 
   LPCVOID bytes, 
   DWORD byteCount, 
   LPCSTR fmt, 
   ...
   );

/* DEPRECATED: Use DcWrite instead */
CMDCAP_API void DCapPrint(
   HANDLE hCap,
   DWORD msgLevel, 
   LPCSTR context, 
   LPCSTR msg
   );

/* DEPRECATED: Use DcByteDump instead */
CMDCAP_API void DCapRaw(
   HANDLE hCap, 
   DWORD msgLevel, 
   LPCSTR context, 
   LPCSTR msg,
   LPCVOID pData,
   size_t size
   );

/* DEPRECATED: Use DcWrite instead */
CMDCAP_API void CMDCapEnqueue(
   HANDLE hCap,
   DWORD msgLevel, 
   LPCSTR context, 
   LPCSTR msg
   );

/* DEPRECATED: Use DcByteDump instead */
CMDCAP_API void CMDCapEnqueueRaw(
   HANDLE hCap, 
   DWORD msgLevel, 
   LPCSTR context, 
   LPCSTR msg,
   LPCVOID pData,
   size_t size
   );

/* DEPRECATED: Use DcFlush instead */
CMDCAP_API BOOL CMDCapFlush(
   HANDLE hCap
   );

/* DEPRECATED: Use DcCreateInstance instead */
CMDCAP_API HANDLE CMDCapInitialize(
   LPCSTR idString,  
   LPCSTR initString
   );

/* DEPRECATED: Use DcWritef instead */
CMDCAP_API BOOL CMDCapPrintf(
   HANDLE hCap,
   DWORD mask, 
   LPCSTR fmt, 
   ...
   );

/* DEPRECATED: Use DcReadSettings instead */
CMDCAP_API BOOL CMDCapReadRegistry(
   HANDLE hCap,
   LPCSTR baseKeyName, 
   LPCSTR keyName
   );                    

/* DEPRECATED: Use DcSetDest instead */
CMDCAP_API DWORD CMDCapSetCaptureControl(
   HANDLE hCap,
   DWORD control
   );

/* DEPRECATED: Use DcGetDest instead */
CMDCAP_API DWORD CMDCapGetCaptureControl(
   HANDLE hCap
   );

/* DEPRECATED: Use DcSetMask instead */
CMDCAP_API DWORD CMDCapSetCaptureMask(
   HANDLE hCap,
   DWORD captureMask
   );

/* DEPRECATED: Use DcGetMask instead */
CMDCAP_API DWORD CMDCapGetCaptureMask(
   HANDLE hCap
   );

/* DEPRECATED: Use DcSetFilePath instead */
CMDCAP_API BOOL CMDCapSetFile(
   HANDLE hCap,
   LPCSTR fSpec, 
   DWORD maxKSize
   );

/* DEPRECATED: Use DcSetPrefix instead */
CMDCAP_API BOOL CMDCapSetPrefix(
   HANDLE hCap,
   LPCSTR prefix
   );

/* DEPRECATED: Use DcSetTimestamp instead */
CMDCAP_API BOOL CMDCapSetTimeOptions(
   HANDLE hCap,
   DWORD timeOptions
   );

#ifdef __cplusplus
}
#endif

#else

/* These stubs will be removed by the optimizer so that trace code may be 
left in the source but disabled just by defining DCAP_DISABLED in the build 
settings. */

#define DcGetVersion(pMajor, pMinor, pRevision) (0)
#define DcCreateInstance(idString, initString) (0)
#define DcCloseInstance(hCap) (0)
#define DcWrite(hCap, mask, context, msg) (0)
#define DcByteDump(hCap, mask, context, bytes, charCount, msg) (0)
#define DcFlush(hCap) (0)
#define DcReadSettings(hCap, baseKeyName, keyName) (0)
#define DcWriteSettings(hCap) (0)
#define DcSetDest(hCap, destFlags) (0)
#define DcGetDest(hCap) (0)
#define DcSetMask(hCap, mask) (0)
#define DcGetMask(hCap) (0)
#define DcSetFilePath(hCap, fSpec) (0)
#define DcGetFilePath(hCap) ("")
#define DcSetFileSize(hCap, maxKSize) (0)
#define DcGetFileSize(hCap) (0)
#define DcSetPrefix(hCap, prefix) (0)
#define DcGetPrefix(hCap) ("")
#define DcSetStamp(hCap, flags) (0)
#define DcGetStamp(hCap) (0)
#define DcIsMaskSet(hCap, mask) (0)
#define DcGetInstanceID(hCap) ("")
#define CMDCapCapture(hCap, mask, capString, bytes, byteCount) (0)
#define CMDCapCleanup(hCap) (0)
#define DCapPrint(hCap, msgLevel, context, msg) (0)
#define DCapRaw(hCap, msgLevel, context, msg, pData, size) (0)
#define CMDCapEnqueue(hCap, msgLevel, context, msg) (0)
#define CMDCapEnqueueRaw(hCap, msgLevel, context, msg, pData, size) (0)
#define CMDCapFlush(hCap) (0)
#define CMDCapInitialize(idString, initString) (0)
#define CMDCapReadRegistry(hCap, baseKeyName, keyName) (0)
#define CMDCapSetCaptureControl(hCap, control) (0)
#define CMDCapGetCaptureControl(hCap) (0)
#define CMDCapSetCaptureMask(hCap, captureMask) (0)
#define CMDCapGetCaptureMask(hCap) (0)
#define CMDCapSetFile(hCap, fSpec, maxKSize) (0)
#define CMDCapSetPrefix(hCap, prefix) (0)
#define CMDCapSetTimeOptions(hCap, timeOptions) (0)

__forceinline void DcVariableStubVoid(HANDLE hCap, ...) {}
__forceinline BOOL DcVariableStubBool(HANDLE hCap, ...) { return FALSE; }

#define DcWritef DcVariableStubVoid
#define DcByteDumpf DcVariableStubVoid
#define CMDCapDataPrintf DcVariableStubBool
#define CMDCapPrintf DcVariableStubBool

#endif // #ifndef DCAP_DISABLED

#define NCR_BASE "SOFTWARE\\NCR"
#define NCR_DCAP_BASE "SOFTWARE\\NCR\\DCap"

/* Data capture masks. */

#define DCM_CAPTURE    0xFFFFFFFF /* Capture everything. */

#define DCM_NONE       0x000000
#define DCM_ERROR      0x00000F
#define DCM_WARNING    0x0000FF
#define DCM_AUDIT      0x000FFF
#define DCM_INFO       0x00FFFF
#define DCM_VERBOSE    0x0FFFFF
#define DCM_DEBUG      0xFFFFFF

#define DCM_API        0x0000000F // Capture all API information.
#define DCM_APIHIGH    0x00000001 //   High-level traces: Entry/Exit.
#define DCM_APIMID     0x00000002 //   Mid-level traces: GetPropertyXxx.
#define DCM_APILOW     0x00000004 //   Low-level traces.

#define DCM_EVENTS     0x000000F0 // Capture all event information.
#define DCM_EVENTQUEUE 0x00000010 //   Queuing of events.
#define DCM_EVENTFIRE  0x00000020 //   Firing of and return from events.

#define DCM_OPS        0x00000F00 // Capture all internal operations.
#define DCM_PRINTEROPS 0x00000100 //   Printer operations.
#define DCM_DRAWEROPS  0x00000200 //   Drawer operations.
#define DCM_MICROPS    0x00000400 //   Micr operations.
#define DCM_OPSLOW     0x00000800 //   Low level operations.

#define DCM_POLLS      0x0000F000 // Capture polls.

#define DCM_RAW        0x000F0000 // Capture all raw information.
#define DCM_RAWSTATUS  0x00010000 //   Serial state changes.
#define DCM_RAWIO      0x00020000 //   I/O to device, except polls.
#define DCM_RAWPOLLS   0x00040000 //   I/O due to polls.
#define DCM_RAWLOW     0x00080000 //   Low level input.

// The following masks are in effect only for debug builds.
#define DCM_MUTEX      0x00100000 // Capture mutex locks/unlocks.
#define DCM_PARSE      0x00200000 // Capture parse data.
#define DCM_PTRESC     0x00400000 // Capture printer escapes.


/* Constants */

/*   for SetCaptureControl... select OFF, or one or more of others. */
#define NCRCAP_OFF         0x0000   // No data capture.


/* The following options capture by enqueuing data, which is processed
by a lower-priority thread: Choose for best performance. */

#define NCRCAP_DCAP_APP    0x01 // Display to data capture application.
#define NCRCAP_DEBUG       0x02 // Display to debugger.
#define NCRCAP_FILE        0x04 // Write to file.
#define NCRCAP_RETAIL_MSG  0x08 // Serial debugger output (Windows CE only).


   /* The following options capture by processing inline.
Choose to ensure immediate capture, but may cause some delays to
the caller (e.g. capture to file will flush to disk before returning). */

#define NCRCAP_DCAP_APP_NOW   0x10 // Display to data capture application immediately.
#define NCRCAP_DEBUG_NOW      0x20 // Display to debugger immediately.
#define NCRCAP_FILE_NOW       0x40 // Write to file immediately.
#define NCRCAP_RETAIL_MSG_NOW 0x80 // Write to serial debugger immediately.


/*   for m_dwTimeOptions... select zero or more. */

#define NCRCAP_STAMP_DATE_MD           0x0100  // Display MM/DD
#define NCRCAP_STAMP_DATE_YMD          0x0300  // Display YYYY/MM/DD
#define NCRCAP_STAMP_TIME_HMS          0x0010  // Display hh:mm:ss
#define NCRCAP_STAMP_TIME_HMS_MILLIS   0x0030  // Display hh:mm:ss.SSS
#define NCRCAP_STAMP_THREAD            0x0001  // Display Thread ID
#define NCRCAP_STAMP_MASK              0x1000  // Display mask of log line

#endif /* #ifndef H_CMDCAP_API */
