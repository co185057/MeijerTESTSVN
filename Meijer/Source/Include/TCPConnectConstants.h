//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    TCPConnectConstants.h
//
// TITLE:   Definitions used for TCPWebConnect
//
// AUTHOR:  Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// TCPConnectDefines
//////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

//The following GPEVTNEW definition will be needed for pre-6.0.4 SSF implementations
//const enum GPEVTNEW
//{
//    GP_TCP_CONNECTED=GP_LASTBASEMSG+1,
//    GP_TCP_DISCONNECTED,
//    GP_TCP_RECEIVED
//};

#define TCP_CONNECTION_1 1

#define TCP_APPID_PRIMERDATA  0
#define TCP_APPID_MOBILECART  1

//Mobile Cart message type indicators
#define TCP_MOBILECARTMSG_CARTPOSTED               0
#define TCP_MOBILECARTMSG_READYFORTRANSACTION      1
#define TCP_MOBILECARTMSG_NOTREADYFORTRANSACTION   2
#define TCP_MOBILECARTMSG_UPLOADCARTINFO           3
