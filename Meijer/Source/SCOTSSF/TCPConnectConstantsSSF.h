//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    TCPConnectConstantsSSF.h
//
// TITLE:   Definitions used for TCPWebConnect
//
// AUTHOR:  Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// TCPConnectDefines for SSF
//////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#define TCP_MOBILECARTMSG_START_AUDIT                           4  // SCO to FLServices
#define TCP_MOBILECARTMSG_START_AUDIT_RESPONSE                  5  // FLServices to SCO
#define TCP_MOBILECARTMSG_ADD_AUDIT_ITEM_REQUEST                6  // SCO to FLServices
#define TCP_MOBILECARTMSG_ADD_AUDIT_ITEM_RESPONSE               7  // FLServices to SCO
#define TCP_MOBILECARTMSG_AUDIT_END                             8  // SCO to FLServices
#define TCP_MOBILECARTMSG_AUDIT_END_RESPONSE                    11 // FLServices to SCO
#define TCP_MOBILECARTMSG_SWITCH_TO_FULL_AUDIT                  12  // SCO to FLServices
#define TCP_MOBILECARTMSG_SWITCH_TO_FULL_AUDIT_RESPONSE         13  // FLServices to SCO
#define TCP_MOBILECARTMSG_CONTINUE_AFTER_ERROR                  14  // FLServices to SCO
#define TCP_MOBILECARTMSG_CONTINUE_AFTER_ERROR_RESPONSE         15  // FLServices to SCO
#define TCP_MOBILECARTMSG_SSCO_TRANSACTION_STATUS               26  // SCO to FLServices
#define TCP_MOBILECARTMSG_FAILURE_REPORT                        27  // FLServices to SSCO
#define TCP_MOBILECARTMSG_GETLASTSTATUS                         28  // FLServices to SSCO

