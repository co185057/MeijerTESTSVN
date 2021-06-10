#pragma once

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#ifdef AUTHENTICATIONCLIENTLIBRARY_EXPORTS
#define AUTHENTICATIONCLIENTLIBRARY_API __declspec(dllexport)
#else
#define AUTHENTICATIONCLIENTLIBRARY_API __declspec(dllimport)
#endif

//
// The macro FT_DEFINED is defined in the header files supplied by Digital Persona. Add this
// check in order to prevent the redefinition compile error when building the AuthenticationClientLibrary.
//
#ifndef FT_DEFINED

#define IN 
#define OUT

typedef enum
{
	FT_WAITING_FOR_IMAGE,		// now waiting for an image from the fingerprint reader
								//	param1 = area acquired given as % ;
								//	param2 = time in ms since FT_isImage was called
	FT_IMAGE_READY,				// an image has been acquired
								//	param1 = raw image size; 
								//	param2 = raw image (the same returned by FT_acquireImage functino) address
	FT_FINGER_TOUCHING,
	FT_FINGER_REMOVED,
	FT_READY_TO_FILL_BUF,		// ready to fill the application display buffer
								//	param1 = buffer address,
								//	param2 = unused
	FT_BUF_FILLED,				// the application display buffer has been filled
								//	param1 = buffer address,
								//	param2 = unused
	FT_IMAGE_INFO,				// provides the info about the acquired image
								//	param1 = FT_IMG_QUALITY
								//	param2 = unused
	FT_FEATURES_INFO,			// provides info about the extracted features
								//	param1 = FT_FTR_QUALITY
								//	param2 = unused
	FT_DEVICE_CONNECTION_ERROR	// provides info about an error in the connection with the device
};

typedef unsigned char FT_BYTE;
typedef int FT_STATUS_CODE;
typedef unsigned int FT_DWORD;

typedef struct
{
	FT_STATUS_CODE code;
	FT_DWORD param1;			// the meaning of param1 and param2 varies according to code
	FT_DWORD param2;
}
FT_STATUS, *FT_STATUS_PT;

typedef enum
{
	FT_ID_CANCEL,				// cancel
	FT_ID_CONTINUE,				// go on
}
FT_ACTION, *FT_ACTION_PT;   


typedef enum              /* features quality */
{
  FT_GOOD_FTR,            /* the features quality is good */
  FT_NOT_ENOUGH_FTR,      /* not enough features */
  FT_NO_CENTRAL_REGION,   /* the fp does not contain the central portion of the finger */
  FT_UNKNOWN_FTR_QUALITY, /* features quality was not be determined */
	FT_AREA_TOO_SMALL				/* Fingerprint area is too small */
} 
FT_FTR_QUALITY, *FT_FTR_QUALITY_PT;

typedef  FT_ACTION  (*FT_GET_ACTION) (IN FT_STATUS_PT, void *);

#define FT_ERR_NO_INIT                 -1   /* fp software not initialized */
#define FT_ERR_INVALID_CONTEXT        -17   /* the given context is invalid */
#define FT_ERR_NO_DEVICE_CONNECTED    -19   /* no device connected */
#define FT_ERR_UILINK_NOT_INITIALIZED -20   /* UI link not initialized */
#define FT_ERR_INVALID_BUFFER         -11   /* the buffer is invalid */
#define FT_ERR_DEVICE_SERVER_FAILED   -512

#endif	// FT_DEFINED


typedef  FT_ACTION  ( __stdcall *FT_GET_ACTION_NET) (IN FT_STATUS_PT, void *);


enum {
	RC_SUCCESS,				///< Operation was successful.
	RC_FAILURE,				///< General failure
	RC_FP_NO_MATCH,			///< No match found for fingerprint data.
	RC_DUPLICATE,			///< More than one potential match found in fingerprint verify.
	RC_DEVICE_PLUGGED=1000,	///< Fingerprint device plugged in.
	RC_DEVICE_UNPLUGGED,	///< Fingerprint device unplugged.
	RC_SERVER_READY,		///< ASA Server is ready.
	RC_SERVER_NOTREADY		///< ASA Server is not ready.
};

typedef enum
{
	FT_UNSOLICITED_INFO=1000,			// NCR extension - Provides info about the get/register fingerprint 
										//	FT_STATUS_PT.param1
										//		Return codes:
										//			RC_FAILURE						Failure for some unknown reason.
										//			RC_DEVICE_PLUGGED				Fingerprint device plugged in.
										//			RC_DEVICE_UNPLUGGED				Fingerprint device unplugged.
										//			RC_SERVER_READY					ASA Server is ready.
										//			RC_SERVER_NOTREADY				ASA Server is not ready.
										//	FT_STATUS_PT.param2 = unused
										//	void * = unused

	FT_GET_FINGERPRINT_COMPLETE,		// NCR extension - Provides info about the get/register fingerprint 
										//	FT_STATUS_PT.param1
										//		Return codes:
										//			RC_SUCCESS						all ok
										//			RC_FAILURE						Failure for some unknown reason.
										//		Errors:
										//			FT_ERR_NO_INIT					fp software not initialized
										//			FT_ERR_INVALID_CONTEXT			context invalid
										//			FT_ERR_NO_DEVICE_CONNECTED		no device connected
										//			FT_ERR_UILINK_NOT_INITIALIZED	UI link not initialized
										//			FT_ERR_INVALID_BUFFER			callback returned invalid buffer
										//			FT_ERR_DEVICE_SERVER_FAILED		error calling server driver
										//	FT_STATUS_PT.param2 = Number of bytes that make up the fingerprint data
										//	void * = FT_BYTE*	Fingerprint data

	FT_REGISTER_FINGERPRINT_COMPLETE,	// NCR extension - Provides info about the get/register fingerprint 
										//	FT_STATUS_PT.param1
										//		Return codes:
										//			RC_SUCCESS					all ok
										//			RC_FAILURE					Failure for some unknown reason.
										//		Errors:
										//			FT_ERR_NO_INIT				fp software not initialized
										//			FT_ERR_NO_MEMORY			no enough memory
										//	FT_STATUS_PT.param2 = Number of bytes that make up the fingerprint data
										//	void * = FT_BYTE*	Fingerprint data

	FT_SAVE_USER_COMPLETE,				// NCR extension - Provides info about the save user 
										//	FT_STATUS_PT.param1
										//		Return codes:
										//			RC_SUCCESS					all ok
										//			RC_FAILURE					Failure for some unknown reason.
										//		Errors:
										//			none
										//	FT_STATUS_PT.param2 = unused
										//	void * = unused

	FT_DELETE_USER_COMPLETE,			// NCR extension - Provides info about the delete user
										//	FT_STATUS_PT.param1
										//		Return codes:
										//			RC_SUCCESS					all ok
										//			RC_FAILURE					Failure for some unknown reason.
										//		Errors:
										//			none
										//	FT_STATUS_PT.param2 = unused
										//	void * = unused

	FT_VERIFY_USER_COMPLETE,			// NCR extension - Provides info about the verify user 
										//	FT_STATUS_PT.param1
										//		Return codes:
										//			RC_SUCCESS					all ok - The UserID and Password are returned.
										//			RC_FAILURE					Failure for some unknown reason.
										//		Errors:
										//			RC_FP_NO_MATCH				No match found in fingerprint database.
										//			RC_DUPLICATE				Duplicate fingerprint found
										//	FT_STATUS_PT.param2 = unused
										//	void * = FT_USERDATA_PT				NOTE: If RC_Duplicate than "pUserID"
										//										contains a list of duplicate user ids.

	FT_GET_USERS_COMPLETE,				// NCR extension - Provides info about the get users
										//	FT_STATUS_PT.param1
										//		Return codes:
										//			RC_SUCCESS					all ok
										//			RC_FAILURE					Failure for some unknown reason.
										//		Errors:
										//			none
										//	FT_STATUS_PT.param2 = Number of users
										//	void * = FT_USERDATAWITHDATES_PT
	FT_PURGE_USER_COMPLETE,
	FT_CONNECT_DEVICE_STATUS,
	FT_DISCONNECT_DEVICE_STATUS
};

typedef struct
{
	LPCTSTR		pUserID;				// User id
	LPCTSTR		pPwd;					// User password
}
FT_USERDATA, *FT_USERDATA_PT;

typedef struct
{
	FT_USERDATA	UserData;				// User id/password/etc.
	FT_BYTE*	pFeatures;				// Fingerprint features
	int			RecommendedFtrLen;		// Fingerprint features length
}
FT_SAVEUSER, *FT_SAVEUSER_PT;

typedef struct
{
	FT_USERDATA	UserData;				// User id/password/etc.
	SYSTEMTIME DateEntered;				// Date/Time entered.
	SYSTEMTIME LastModified;			// Date/Time last modified.	
	SYSTEMTIME LastVerified;			// Date/Time last verified.
}
FT_USERDATAWITHDATES, *FT_USERDATAWITHDATES_PT;

//
// The signature of a callback method should look like the following:
//
//		FT_ACTION getRegisterAction (FT_STATUS_PT pStatus, void *pParam);
//

AUTHENTICATIONCLIENTLIBRARY_API BOOL FT_Open_Net(IN FT_GET_ACTION_NET func);					// This method is called when the client application is started. .Net support
AUTHENTICATIONCLIENTLIBRARY_API BOOL FT_Open(IN FT_GET_ACTION func);							// This method is called when the client application is started.
AUTHENTICATIONCLIENTLIBRARY_API BOOL FT_GetFingerprint();										// This method uses the Digital Persona fingerprint reader API in order to collect 1 fingerprint for verification
AUTHENTICATIONCLIENTLIBRARY_API BOOL FT_RegisterFingerprint();									// This method uses the Digital Persona fingerprint reader API in order to collect 4 fingerprints for registration
AUTHENTICATIONCLIENTLIBRARY_API BOOL FT_SaveUser(FT_SAVEUSER_PT pFPData);						// This method uses the ASAInterface to store the users information to the ASA server.
AUTHENTICATIONCLIENTLIBRARY_API BOOL FT_DeleteUser(LPCTSTR pUserID);							// This method uses the ASAInterface to delete the users information from the ASA server.
AUTHENTICATIONCLIENTLIBRARY_API BOOL FT_VerifyUser(FT_BYTE*	pFeatures, int RecommendedFtrLen);	// This method uses the ASAInterface to verify a fingerprint with the ASA server. If the fingerprint is verified the user data is returned to the client application..
AUTHENTICATIONCLIENTLIBRARY_API BOOL FT_GetUsers();												// This method uses the ASAInterface to get all the users that are stored on the ASA server.
AUTHENTICATIONCLIENTLIBRARY_API BOOL FT_Abort();												// This method is used in order to terminate the getFingerprint/registerFingerprint/saveUser/deleteUser/verifyUser/getUsers methods.
AUTHENTICATIONCLIENTLIBRARY_API BOOL FT_Purge();												// This method is used to purge old records in the database.
AUTHENTICATIONCLIENTLIBRARY_API BOOL FT_Close();												// This method is called when the client application is exiting.
AUTHENTICATIONCLIENTLIBRARY_API BYTE* FT_GetBPBuffer();											// This method returns the display buffer. ORU uses the display buffer to draw the fingerprint.
AUTHENTICATIONCLIENTLIBRARY_API BOOL FT_SetRectDraw(int nWidht, int nHeight);					// This method sets width and height of display buffer size.
AUTHENTICATIONCLIENTLIBRARY_API unsigned int FT_GetNumberOfDevices(void);						// This method resturns the number of fingerprint devices connected to the machine.  This is called by ORU to determine if fingerprint device is present.
AUTHENTICATIONCLIENTLIBRARY_API BOOL FT_ConnectDevice();
AUTHENTICATIONCLIENTLIBRARY_API BOOL FT_DisconnectDevice();
#ifdef __cplusplus
}
#endif


