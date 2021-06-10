/* ======================================================================= */
/* =              Copyright (c) NCR Corporation 1989-2001                = */
/* ======================================================================= */
/* ACSIOClientApi.h 4.8 */
/****************************************************************************
ACSIOClient.h
  
Purpose: 
   Definition of external interface to the ACSIOClient library.

Comments: 


Change history:

$Log: /Integration_Solution/ACS_Solution/CURRENT_DEV/Source/AfxTransBroker/ACSHook/include/ACSIOClientApi.h $
 * 
 * 1     3/19/10 4:13p Sm133076
 * Added ACSIO headers so that ACSHookManager can use the prototypes
 * directly since it no longer uses LoadLibrary to load the corresponding
 * DLLs.
 * 
 * Comment at 090714 11:24:53  by atleng-cwatson  
 * TAR#: 409598 
 * Event notification for message arrivals was replaced with a semaphore 
 * that is incremented every time a message arrives on a channel and is 
 * decremented every time a message is retrieved. 
 * 
 * Comment at 090204 16:51:32  by psd-pparks  
 * FEATURE#: 382611 
 * Added ACSIOGetContextFields, ACSIO_AUXMSG_HEADER 
 * 
 * Comment at 080903 16:58:36  by psd-pparks  
 * TAR#: 383942 
 * This time I really did add the ACSIOSetTraceInstanceName function. 
 * 
 * Comment at 080830 10:32:02  by psd-pparks  
 * TAR#: 383942 
 * Added API function to set trace instance name prior to calling  
 * initialization. 
 * 
 * Comment at 070628 15:44:05  by psd-pparks  
 * FEATURE#: 338007 
 * Added declaration of new API functions in ACSIOClient.dll. 

****************************************************************************/

#ifndef H_ACSIOCLIENTAPI
#define H_ACSIOCLIENTAPI

#ifdef WIN32

#include <windows.h>

#ifdef ACSIOCLIENT_EXPORTS
#define ACSIOCLIENT_API __declspec(dllexport)
#else
#define ACSIOCLIENT_API __declspec(dllimport)
#endif

typedef void *HCLIENT ;
typedef void (CALLBACK* CONTEXTSINK)(LPSTR, void *);
typedef void (CALLBACK* SEQUENCESINK)(int, void *);

#define ACSIO_NO_DATA            MAKE_HRESULT( SEVERITY_ERROR, FACILITY_ITF, 900 )
#define ACSIO_NOT_INITIALIZED    MAKE_HRESULT( SEVERITY_ERROR, FACILITY_ITF, 901 )
#define ACSIO_NOT_CONNECTED      MAKE_HRESULT( SEVERITY_ERROR, FACILITY_ITF, 902 )
#define ACSIO_ALREADY_CONNECTED  MAKE_HRESULT( SEVERITY_ERROR, FACILITY_ITF, 903 )

/* Do not define any HRESULTS after this one */
#define ACSIO_OBJECT_0           MAKE_HRESULT( SEVERITY_SUCCESS, FACILITY_ITF, 950 )

#define ACSIO_MULTITHREADED      0
#define ACSIO_SINGLETHREADED     2 


#ifdef __cplusplus
extern "C"
{
#endif

// Initialize and Uninitialize each thread that uses ACSIO
ACSIOCLIENT_API HRESULT ACSIOInitialize(  DWORD dwCoInit, HCLIENT *hClient );
typedef HRESULT (*ACSIOInitialize_ptr_t)(  DWORD, HCLIENT *);

ACSIOCLIENT_API HRESULT ACSIOUninitialize( HCLIENT hClient ) ;
typedef HRESULT (*ACSIOUninitialize_ptr_t)( HCLIENT ) ;

// Create/Connect to prenamed channels
ACSIOCLIENT_API HRESULT ACSIOConnectContextChannel( HCLIENT hClient  );
typedef HRESULT (*ACSIOConnectContextChannel_ptr_t)( HCLIENT );

ACSIOCLIENT_API HRESULT ACSIOConnectSequenceChannel( HCLIENT hClient  );
typedef HRESULT (*ACSIOConnectSequenceChannel_ptr_t)( HCLIENT );

// Context and Sequence I/O
ACSIOCLIENT_API HRESULT ACSIOSendContext( HCLIENT hClient, LPSTR pszContextName, void * pContextData );
typedef HRESULT (*ACSIOSendContext_ptr_t)( HCLIENT , LPSTR , void * );

ACSIOCLIENT_API HRESULT ACSIOGetContext( HCLIENT hClient, CONTEXTSINK lpFunc ) ;
typedef HRESULT (*ACSIOGetContext_ptr_t)( HCLIENT , CONTEXTSINK ) ;

ACSIOCLIENT_API HRESULT ACSIOGetContextFields(HCLIENT hClient, BSTR* structName, VARIANT* fields);
typedef HRESULT (*ACSIOGetContextFields_ptr_t)(HCLIENT, BSTR*, VARIANT*);

ACSIOCLIENT_API HRESULT ACSIOSendSequence( HCLIENT hClient, int nSequence, void * pKeyboard) ;
typedef HRESULT (*ACSIOSendSequence_ptr_t)( HCLIENT , int , void * ) ;

ACSIOCLIENT_API HRESULT ACSIOGetSequence( HCLIENT hClient, SEQUENCESINK lpFunc ) ;
typedef HRESULT (*ACSIOGetSequence_ptr_t)( HCLIENT , SEQUENCESINK ) ;

// Auxiliary named channels creation and I/O
ACSIOCLIENT_API HRESULT ACSIOConnectAuxiliaryChannel( HCLIENT hClient ) ;
typedef HRESULT (*ACSIOConnectAuxiliaryChannel_ptr_t)( HCLIENT );

ACSIOCLIENT_API HRESULT ACSIOSendRaw( HCLIENT hClient, int nBufferLength, BYTE *pBuffer ) ;
typedef HRESULT (*ACSIOSendRaw_ptr_t)( HCLIENT, int, BYTE *) ;

ACSIOCLIENT_API HRESULT ACSIOGetRaw( HCLIENT hClient, int *nBufferLength, BYTE *pBuffer ) ;
typedef HRESULT (*ACSIOGetRaw_ptr_t)( HCLIENT, int *, BYTE *) ;

// Receive "data available" events
/******************************************************************************
THIS API FUNCTION IS DEPRECATED. PLEASE SEE THE DOCUMENTATION FOR 
ACSIOWaitForData FOR THE REPLACEMENT METHOD OF RETRIEVING ACSIO MESSAGES.
******************************************************************************/
ACSIOCLIENT_API HRESULT ACSIOAdvise( HCLIENT hClient, HANDLE hSyncEventObject ) ;
typedef HRESULT (*ACSIOAdvise_ptr_t)( HCLIENT, HANDLE) ;

/******************************************************************************
THIS API FUNCTION IS DEPRECATED. PLEASE SEE THE DOCUMENTATION FOR 
ACSIOWaitForData FOR THE REPLACEMENT METHOD OF RETRIEVING ACSIO MESSAGES.
******************************************************************************/
ACSIOCLIENT_API HRESULT ACSIOUnadvise( HCLIENT hClient ) ;
typedef HRESULT (*ACSIOUnadvise_ptr_t)(HCLIENT) ;

ACSIOCLIENT_API HRESULT ACSIOSetMaxQueueEntries( HCLIENT hClient, int iMaxEntries );
typedef HRESULT (*ACSIOSetMaxQueueEntries_ptr_t)(HCLIENT, int);

// Output Filtering
///////////////////////////////////////////////////////////////////////////////
ACSIOCLIENT_API HRESULT ACSIOAddFilter( HCLIENT hClient, LPSTR pszContextName ) ;
typedef BOOL (*ACSIOAddFilter_ptr_t)( HCLIENT, LPSTR );

ACSIOCLIENT_API BOOL ACSIOGetErrorText( HCLIENT hClient, char *pszText, int nBufferSize );
typedef BOOL (*ACSIOGetErrorText_ptr_t)( HCLIENT, char *, int );


/* Add a field to the list of fields that should be masked prior to output. 
For security purposes, there is intentionally no function to remove a field from the 
list while the application is running. */
ACSIOCLIENT_API HRESULT ACSIOAddStructMaskField(HCLIENT hClient, LPCSTR structName, LPCSTR fieldName);
typedef HRESULT (*ACSIOAddStructMaskField_ptr_t)(HCLIENT, LPCSTR, LPCSTR);

/* Determine if a given field in a given struct should be masked. */
ACSIOCLIENT_API BOOL ACSIOIsMaskField(HCLIENT hClient, LPCSTR structName, LPCSTR fieldName);
typedef BOOL (*ACSIOIsMaskField_ptr_t)( HCLIENT, LPCSTR, LPCSTR);

/* Determine if a given struct contains masked fields. */
ACSIOCLIENT_API BOOL ACSIOStructHasMaskField(HCLIENT hClient, LPCSTR structName);
typedef BOOL (*ACSIOStructHasMaskField_ptr_t)(HCLIENT, LPCSTR);

/* Retrieve the list of fields that are currently configured to be masked on output. 
The fields are stored sequentially, separated by a null character. The last string in the 
list is followed by two null characters. The pCharCount parameter, if non-null, should 
point to a DWORD that receives the total size, in characters, of the memory buffer that 
stores the entire list, including null separators and terminators. */
ACSIOCLIENT_API LPCTSTR ACSIOGetStructMaskFields(HCLIENT hClient, DWORD* pCharCount);
typedef LPCTSTR (*ACSIOGetStructMaskFields_ptr_t)(HCLIENT, DWORD*);

/* Set the name of the trace instance for this run of the library. Returns TRUE if the 
name was successfully prior to the creation of the trace instance, FALSE otherwise. */
ACSIOCLIENT_API BOOL ACSIOSetTraceInstanceName(LPCSTR name);
typedef BOOL (*ACSIOSetTraceInstanceName_ptr_t)(LPCSTR /*name*/);


/* Instruct ACSIO to send notifications of new data arriving on the specified channel. */
ACSIOCLIENT_API DWORD ACSIOAdviseEx(
   HANDLE hClient);

typedef DWORD (*ACSIOAdviseEx_ptr_t)(
   HANDLE /*hClient*/);


/* Instruct ACSIO to stop sending notifications on the specified channel. */
ACSIOCLIENT_API DWORD ACSIOUnadviseEx(
   HANDLE hClient);

typedef DWORD (*ACSIOUnadviseEx_ptr_t)(
   HANDLE /*hClient*/);


/* 
Waits until a new message arrives on the specified ACSIO channel or the timeout interval 
elapses.

Return values: 

* ACSIO_OBJECT_0: an ACSIO message arrived on the hClient channel before the timeout expired
* WAIT_TIMEOUT: the timeout expired with no messages
* WAIT_FAILED: The function has failed. To get extended error information, call GetLastError().

*/
ACSIOCLIENT_API DWORD ACSIOWaitForData(
   HANDLE hClient, 
   DWORD timeout);

typedef DWORD (*ACSIOWaitForData_ptr_t)(
   HANDLE /*hClient*/, 
   DWORD /*timeout*/);


/* 
Waits until a new message arrives on one of the specified ACSIO channels or the time-out 
interval elapses. 

Return values: 

* ACSIO_OBJECT_0 + n: the return value minus ACSIO_OBJECT_0 indicates the pClientArray 
      array index of the channel that satisfied the wait. If data arrived on more than one 
      channel during the call, this is the array index of the channel with the smallest 
      index value of all the signaled objects.
* WAIT_TIMEOUT: the timeout expired with no messages
* WAIT_FAILED: The function has failed. To get extended error information, call GetLastError().

*/
ACSIOCLIENT_API DWORD ACSIOWaitForDataMultiple(
   DWORD clientCount, 
   CONST HANDLE* pClientArray, 
   DWORD timeout);

typedef DWORD (*ACSIOWaitForDataMultiple_ptr_t)(
   DWORD /*clientCount*/, 
   CONST HANDLE* /*pClientArray*/, 
   DWORD /*timeout*/);


/*
Waits until a new message arrives on the specified ACSIO channel, one of the specified objects 
is in the signaled state, or the time-out interval elapses. 

Return values: 

* ACSIO_OBJECT_0: an ACSIO message arrived on the hClient channel before the timeout expired
* WAIT_OBJECT_0 to WAIT_OBJECT_0 + (handleCount - 1): the return value minus WAIT_OBJECT_0 
      indicates the pHandles array index of the object that satisfied the wait. If more than 
      one object became signaled during the call, this is the array index of the signaled 
      object with the smallest index value of all the signaled objects.
* WAIT_TIMEOUT: the timeout expired with no messages
* WAIT_FAILED: The function has failed. To get extended error information, call GetLastError().

*/
ACSIOCLIENT_API DWORD ACSIOWaitForDataEvent(
   HANDLE hClient, 
   DWORD handleCount,
   CONST HANDLE *pHandles,
   DWORD timeout);

typedef DWORD (*ACSIOWaitForDataEvent_ptr_t)(
   HANDLE /*hClient*/, 
   DWORD /*handleCount*/,
   CONST HANDLE* /*pHandles*/,
   DWORD /*timeout*/);


/*
Waits until a new message arrives on one of the specified ACSIO channels, one of the specified 
objects is in the signaled state, or the time-out interval elapses. 

Return values: 

* ACSIO_OBJECT_0 to ACSIO_OBJECT_0 + (clientCount - 1): the return value minus ACSIO_OBJECT_0 
      indicates the pClientArray array index of the channel that satisfied the wait. If data 
      arrived on more than one channel during the call, this is the array index of the channel 
      with the smallest index value of all the signaled objects.
* WAIT_OBJECT_0 to WAIT_OBJECT_0 + (handleCount - 1): the return value minus WAIT_OBJECT_0 
      indicates the pHandles array index of the object that satisfied the wait. If more than 
      one object became signaled during the call, this is the array index of the signaled 
      object with the smallest index value of all the signaled objects.
* WAIT_TIMEOUT: the timeout expired with no messages
* WAIT_FAILED: The function has failed. To get extended error information, call GetLastError().

*/
ACSIOCLIENT_API DWORD ACSIOWaitForDataEventMultiple(
   DWORD clientCount, 
   CONST HANDLE* pClientArray, 
   DWORD handleCount,
   CONST HANDLE *pHandles,
   DWORD timeout);

typedef DWORD (*ACSIOWaitForDataEventMultiple_ptr_t)(
   DWORD /*clientCount*/, 
   CONST HANDLE* /*pClientArray*/, 
   DWORD /*handleCount*/,
   CONST HANDLE* /*pHandles*/,
   DWORD /*timeout*/);


/*
Waits until a new message arrives on the specified ACSIO channel, one of the specified objects 
is in the signaled state, an I/O completion routine or asynchronous procedure call (APC) is 
queued to the thread, or the time-out interval elapses. The array of objects can include input 
event objects, which you specify using the wakeMask parameter.

Return values: 

* ACSIO_OBJECT_0: an ACSIO message arrived on the hClient channel before the timeout expired
* WAIT_OBJECT_0 to WAIT_OBJECT_0 + (handleCount - 1): the return value minus WAIT_OBJECT_0 
      indicates the pHandles array index of the object that satisfied the wait. If more than 
      one object became signaled during the call, this is the array index of the signaled 
      object with the smallest index value of all the signaled objects.
* WAIT_OBJECT_0 + handleCount: New input of the type specified in the wakeMask parameter 
      is available in the thread's input queue. 
* WAIT_TIMEOUT: the timeout expired with no messages
* WAIT_FAILED: The function has failed. To get extended error information, call GetLastError().

*/
ACSIOCLIENT_API DWORD ACSIOWaitForDataEventMsg(
   HANDLE hClient, 
   DWORD handleCount,
   CONST HANDLE *pHandles,
   DWORD timeout,
   DWORD wakeMask,
   DWORD flags);

typedef DWORD (*ACSIOWaitForDataEventMsg_ptr_t)(
   HANDLE /*hClient*/, 
   DWORD /*handleCount*/,
   CONST HANDLE* /*pHandles*/,
   DWORD /*timeout*/,
   DWORD /*wakeMask*/,
   DWORD /*flags*/);


/*
Waits until a new message arrives on one of the specified ACSIO channels, one of the specified 
objects is in the signaled state, an I/O completion routine or asynchronous procedure call (APC) 
is queued to the thread, or the time-out interval elapses. The array of objects can include input 
event objects, which you specify using the wakeMask parameter.

Return values: 

* ACSIO_OBJECT_0 to ACSIO_OBJECT_0 + (clientCount - 1): the return value minus ACSIO_OBJECT_0 
      indicates the pClientArray array index of the channel that satisfied the wait. If data 
      arrived on more than one channel during the call, this is the array index of the channel 
      with the smallest index value of all the signaled objects.
* WAIT_OBJECT_0 to WAIT_OBJECT_0 + (handleCount - 1): the return value minus WAIT_OBJECT_0 
      indicates the pHandles array index of the object that satisfied the wait. If more than 
      one object became signaled during the call, this is the array index of the signaled 
      object with the smallest index value of all the signaled objects.
* WAIT_OBJECT_0 + handleCount: New input of the type specified in the wakeMask parameter 
      is available in the thread's input queue. 
* WAIT_TIMEOUT: the timeout expired with no messages
* WAIT_FAILED: The function has failed. To get extended error information, call GetLastError().

*/
ACSIOCLIENT_API DWORD ACSIOWaitForDataEventMsgMultiple(
   DWORD clientCount, 
   CONST HANDLE* pClientArray, 
   DWORD handleCount,
   CONST HANDLE *pHandles,
   DWORD timeout,
   DWORD wakeMask,
   DWORD flags);

typedef DWORD (*ACSIOWaitForDataEventMsgMultiple_ptr_t)(
   DWORD clientCount, 
   CONST HANDLE* pClientArray, 
   DWORD handleCount,
   CONST HANDLE* pHandles,
   DWORD timeout,
   DWORD wakeMask,
   DWORD flags);


/* This header must be at the start of all messages sent to ACSIO on the auxiliary channel. */
typedef struct 
{
   /* The size, in bytes, of this structure. ALWAYS initialize this field with 
   sizeof(ACSIO_AUXMSG_HEADER). That way if fields are added to the structure the 
   size will change accordingly. */
   UINT structSize;

   /* A numeric identifier used by ACS to interpret the contents of the message 
   payload. */
   DWORD payloadID;

   /* The size, in bytes, of the message payload. */
   UINT payloadSize;
} ACSIO_AUXMSG_HEADER;


#ifdef __cplusplus
}
#endif

#endif /* #ifdef WIN32 */

#endif /* #ifndef H_ACSIOCLIENTAPI */
