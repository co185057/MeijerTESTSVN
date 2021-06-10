/*************************************************************************
 *
 *  NCRCashAcceptorSoEx.h: NCRCashAcceptorSO header file for applications.
 * 
 * $Date: 10/29/08 4:31p $
 *
 *************************************************************************
 *
 * Copyright (c) NCR Corporation 2001.  All rights reserved.
 *
 *************************************************************************/



// Flash download Command
// *plData is unused
// *pbstrData contains the path and filename of the bin file. 
// if *pbstrData points to an empty string (""), then the default filename 
// from the registry is returned to the caller. 
// Returns:
//    OPOS_SUCCESS - download started successfully.

const long NCRDIO_BILLSO_FLASH_DOWNLOAD  =  1200;

// Flash download Status
// *plData returns the approximate number of seconds remaining in the load.
// *pbstrData is unused.
// Returns:
//    OPOS_E_BUSY    - The download is still being processed - see *plData
//    OPOS_SUCCESS   - The download completed successfully. 
//    OPOS_E_FAILURE - An error occurred.
//       NCR_EBILLSO_SWITCH_ERROR   - Switch position is wrong. 
//       NCR_EBILLSO_FILE_ERROR     - File error - see event log. 

//extended result codes
const long NCR_EBILLSO_SWITCH_ERROR       = 10 + OPOSERREXT;
// File not found/Access Denied/Format error
const long NCR_EBILLSO_FILE_ERROR         = 11 + OPOSERREXT;
const long NCR_EBILLSO_CASS_OUT           = 12 + OPOSERREXT;
const long NCR_EBILLSO_JAM                = 13 + OPOSERREXT;
const long NCR_EBILLSO_CASS_FULL          = 14 + OPOSERREXT;
const long NCR_EBILLSO_SAME_FIRMWARE      = 15 + OPOSERREXT;
const long NCR_EBILLSO_UNKNOWN_CRC	      = 16 + OPOSERREXT;
// The device Version key (reported by the device) is not known and may be 
// newer than any known firmware version and AllowVersionDowngrade is FALSE
const long NCR_EBILLSO_UNKNOWN_VERSION    = NCR_EBILLSO_UNKNOWN_CRC;
// Device is present but is not able to begin the operation. 
const long NCR_EBILLSO_DEVICE_NOT_READY   = NCR_EBILLSO_SWITCH_ERROR;
// Device is present but did not respond correctly.
const long NCR_EBILLSO_DEVICE_ERROR       = 17 + OPOSERREXT;
const long NCR_EBILL_PUP_ESCROW           = 18 + OPOSERREXT;   // Bill in escrow at power-up
const long NCR_EBILL_PAUSED               = 19 + OPOSERREXT;   // Bill in escrow and Bill in bezel
// Operating System or Runtime environment error
const long NCR_EBILLSO_SYSTEM_ERROR       = 20 + OPOSERREXT;
// Application aborted the operation. 
const long NCR_EBILLSO_ABORTED            = 21 + OPOSERREXT;


const long NCRDIO_BILLSO_DOWNLOAD_STATUS =  1201;
// Flash download Status
// *plData returns the approximate number of seconds remaining in the load.
// *pbstrData is unused.
// Returns:

const long NCRDIO_BILLSO_GET_FIRMWARE_VERSION   = 1202;
// Device must be Opened, Claimed and Enabled. 
// Returns revision# in the long* argument.
// Returns version string (from the registry if available) in 
// the BSTR* argument 
// Returns OPOS_E_NOHARDWARE if the device is offline
// Returns OPOS_E_ILLEGAL for Ardac
// For CCNet, returns "US17xx" 

const long NCRDIO_BILLSO_GET_MODEL              = 1203;
// Device must be Opened, Claimed and Enabled. 
// Returns 20 in the long* argument for USD devices and 22 for CAD devices. 
// Returns OPOS_E_NOHARDWARE if the device is offline
// Returns OPOS_E_ILLEGAL for Ardac
// CCNet returns "SM"

const long NCRDIO_BILLSO_GET_FIRMWARE_VERSION_KEY   = 1204;
// Device must be Opened, Claimed
// In bstrData, returns 
//          "CRC,Model,Revision" string for ZT1207R
//          "SM-US17XX" for ccNet (new CashCode)
//          TBD for ZT1207R
//          TBD for SC66
//          TBD for SC83
// Returns OPOS_E_NOHARDWARE if the device is offline
// Returns OPOS_E_ILLEGAL for Ardac, Old CashCode

const long NCRDIO_BILLSO_FLASH_DOWNLOAD_EX   = 1205;
// Begin Flash Download Command
// This works in one of two ways. 
// 1) *pbstrData contains the path and filename of the file. 
//    Here, the calling application passes the filename
//    The download is forced assuming file is found and is valid 
// 2) *pbstrData contains an empty string.
//    Here, the Registry/ControlFile is used to determine the filename
//    The download will only occur if the version database indicates that
//    the device needs an update. 
// *plData should be zero on entry. (Later some bitmask options will be added.)
#define DL_FLAG_FORCE_DOWNLOAD         1  // Always download if possible
#define DL_FLAG_SHOW_PROGRESS_DIALOG   2  // Show 
// Returns: 
// OPOS_SUCCESS   - download started successfully.
//    *plData returns the total number of bytes to be sent. 
//    *pbstrData contains the fully qualified path and filename of the file. 
//    This may be different that the filename passed if a download is 
//    already in progress. 
// OPOS_E_NOTCLAIMED - Device must be claimed
// OPOS_E_BUSY       - Bill in Escrow
// OPOS_E_NOHARDWARE - Device is offline
// OPOS_E_FAILURE   
//    NCR_EBILLSO_SAME_FIRMWARE     - Device firmware is up to date
//    NCR_EBILLSO_FILE_ERROR        - see event log. 
//    NCR_EBILLSO_SYSTEM_ERROR      - see event log. 
//    NCR_EBILLSO_DEVICE_NOT_READY  - Device not ready - see event log. 
//    NCR_EBILLSO_UNKNOWN_VERSION   - Device version not known
//    NCR_EBILLSO_DEVICE_ERROR      - Device responded incorrectly

// Note: Once success is returned from this function, the application must
// call NCRDIO_BILLSO_DOWNLOAD_PENDING until a status other than OPOS_E_BUSY 
// is returned. Alternately, the application can Release() the device, which 
// aborts the download. 
// 
const long NCRDIO_BILLSO_DOWNLOAD_PENDING    = 1206;
// Returns:
//    OPOS_E_BUSY    - Download in progress
//       *plData returns the number of bytes sent to the device. 
//    OPOS_SUCCESS   - Download completed successfully
//       *plData returns total number of bytes sent
//    OPOS_E_ILLEGAL - Download is not pending
//    OPOS_E_NOHARDWARE - Device went offline. 
//    OPOS_E_FAILURE - The download has failed, see extended result code
//       *plData returns the number of bytes sent to the device. 
//       NCR_EBILLSO_LOAD_ERROR  - Block Write to device failed. 
//       NCR_EBILLSO_LOAD_ABORTED- User aborted the download. 
//       NCR_EBILLSO_DEVICE_ERROR- Device returned an error. 
// Note: *pbstrData contains filename for all statuses except OPOS_E_ILLEGAL.

const long NCRDIO_BILLSO_ABORT_DOWNLOAD      = 1207;
//    OPOS_E_ILLEGAL - Download is not pending

const long NCRDIO_BILLSO_GET_LAST_DL_STATUS  = 1208;
//    OPOS_SUCCESS
//    *pbstrData returns Time/Date of last download
//    OPOS_E_NOSERVICE  - No Prior Download info available
//    OPOS_E_BUSY       - Download is pending. Use NCRDIO_BILLSO_DOWNLOAD_PENDING

const long NCRDIO_BILLSO_GET_BUILD_VERSION  = 1209;
//    OPOS_SUCCESS
//    *pbstrData contains build version.

const long NCRDIO_BILLSO_GET_DOWNLOAD_TIME_LEFT  = 1210;
// Returns:
//    OPOS_E_BUSY    - Download in progress
//       *plData returns the approximate time (in seconds) left.
//    OPOS_E_NOEXIST - download has not started, estimate is not available yet....
//    OPOS_E_ILLEGAL - Download is not pending
// Note: this method does not complete the download sequence, use
//       NCRDIO_BILLSO_DOWNLOAD_PENDING 


const long NCRDIO_BILLSO_GET_DL_CHECK_STATUS = 1211;
//    OPOS_SUCCESS
//    *pbstrData returns Time/Date of the last time the firmware was checked.
//    OPOS_E_NOSERVICE  - No Prior check info available

// EEBDS Support - logan
const long NCRDIO_BILLSO_GET_FIRMWARE_VERSION_KEY_APP   = 1212;
// Device must be Opened, Claimed
// In bstrData, returns 
//          "CRC,Model,Revision" string for ZT1207R
//          "SM-US17XX" for ccNet (new CashCode)
//          TBD for ZT1207R
//          TBD for SC66
//          TBD for SC83
// Returns OPOS_E_NOHARDWARE if the device is offline
// Returns OPOS_E_ILLEGAL for Ardac, Old CashCode, when EEBDS is not supported, and when
//         and existing download is on-going.

const long NCRDIO_BILLSO_FLASH_DOWNLOAD_APP   = 1213;
// Begin Flash Download Command
// This works in one of two ways. 
// 1) *pbstrData contains the path and filename of the file. 
//    Here, the calling application passes the filename
//    The download is forced assuming file is found and is valid 
// 2) *pbstrData contains an empty string.
//    Here, the Registry/ControlFile is used to determine the filename
//    The download will only occur if the version database indicates that
//    the device needs an update. 
// *plData should be zero on entry. (Later some bitmask options will be added.)
const long NCR_EBILLSO_EEBDS_NOT_SUPPORTED       = 22 + OPOSERREXT;
// Variant download skipped, application FW not latest
const long NCR_EBILLSO_APP_NOT_LATEST			 = 23 + OPOSERREXT;

// end EEBDS Support


const long NCRDIO_RECYCLERSO_GET_BILL_COUNT = 1600;
// Used by the Recycler (CashCode)
// Retreive the Bill Count and Bill AcceptedList
// assuming the 3 DIRECTIO parms are: long  nCommand, long* plData, CString& sData)
// Output: plData is the Bill Count
// Output: sData is the Accept list
// when the return is OPOS_SUCCESS

const long NCRDIO_RECYCLERSO_UNLOAD_NOTES_TO_DROPBOX = 1612;
// Used by the Recycler (CashCode)
// Unloads all or specified amount of bills to the DropBox
// assuming the 3 DIRECTIO parms are: long  nCommand, long* plData, CString& sData)
// When unloading certain number of bills to the dropbox
// Input: plData is number of bills to dispense
// Input: sData is the cassette number to dispense from
// - or - 
// when unloading all bills to the dropbox
// Input: sData is the string "ALL"
// when the return is OPOS_SUCCESS

const long NCRDIO_RECYCLERSO_SHOW_CASSETTE_STATUS = 1613;
// Used by the Recycler (CashCode)
// Show how many bills present for each Bill Type (list of bills that could be dispensed)
// assuming the 3 DIRECTIO parms are: long  nCommand, long* plData, CString& sData)
// Output: status string goes into sData
// when the return is OPOS_SUCCESS
