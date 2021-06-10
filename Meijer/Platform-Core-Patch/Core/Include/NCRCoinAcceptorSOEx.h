/*************************************************************************
 *
 *  NCRCoinAcceptorSoEx.h: NCRCoinAcceptorSO header file for applications.
 * 
 * $Date: 10/29/08 4:31p $
 *
 *************************************************************************
 *
 * Copyright (c) NCR Corporation 2007.  All rights reserved.
 *
 *************************************************************************/

const long NCRDIO_CACCSO_GET_FIRMWARE_VERSION   = 1202;
// Device must be Opened, Claimed and Enabled. 
// Returns version string from the device in the BSTR* argument 
// Returns OPOS_E_ILLEGAL if the device is offline

const long NCRDIO_CACCSO_GET_COINMAPPING_VERSION   = 1203;
// Device must be Opened, Claimed and Enabled. 
// Returns country code of loaded mapping (ex: USD, JPY..) in the BSTR* argument 
// Returns OPOS_E_ILLEGAL if the device is offline

const long NCRDIO_CACCSO_GET_DEVICE_DESCRIPTION   = 1204;
// Device must be Opened, Claimed and Enabled. 
// Returns device description (CF9520) in the BSTR* argument 
// Returns OPOS_E_ILLEGAL if the device is offline

const long NCRDIO_CACCSO_UPDATE_COINMAPPING   = 1205;
// Device must be Opened, Claimed and Enabled. 
// Returns OPOS_SUCCESS if coin mappinge update was successful 
// Returns OPOS_E_ILLEGAL if the device is offline

const long NCRDIO_CACCSO_RESET_COINMAPPING   = 1206;
// Device must be Opened, Claimed and Enabled. 
// Returns OPOS_SUCCESS if operation was successful 
// Returns OPOS_E_ILLEGAL if the device is offline

const long NCRDIO_CACCSO_FORCE_COINMAPPING   = 1207;
// Device must be Opened, Claimed and Enabled. 
// Returns version string from the device in the BSTR* argument 
// Returns OPOS_E_NOHARDWARE if the device is offline

const long NCRDIO_CACCSO_START_UPDATE   = 1208;
// Device must be Opened, Claimed and Enabled. 
// Returns version string from the device in the BSTR* argument 
// Returns OPOS_E_NOHARDWARE if the device is offline

const long NCRDIO_CACCSO_DEVICE_TYPE   = 1209;
// Device must be Opened, Claimed and Enabled. 
// Returns version string from the device in the BSTR* argument 
// Returns OPOS_E_NOHARDWARE if the device is offline

const long NCRDIO_CACCSO_GET_SERIALNUMBER   = 1210;
// Device must be Opened, Claimed and Enabled. 
// Returns version string from the device in the BSTR* argument 
// Returns OPOS_E_NOHARDWARE if the device is offline

