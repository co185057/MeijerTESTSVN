// ScotTCPConnectorConstants.h
//
// Contains constant definitions for communicating using the FLTCPConnector object
//
//////////////////////////////////////////////////////////////////////

#if !defined(SCOT_TCPCONNECTOR__INCLUDED_)
#define SCOT_TCPCONNECTOR__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Definitions from TCPConnector.h
/* structMsgHeader usage
Version 	msg type1	      msg type2          msg type3     app data
0	        0:xml             0: xml format 0    app defined   app defined
                                  1: xml format 1    app defined   app defined
                etc.                                 app defined   app defined
                1:json            0: json format 0   app defined   app defined
                                  1: json format 1   app defined   app defined
                etc.                                 app defined   app defined
                other             n/a                app defined   app defined


const enum MSG_FORMAT    //MsgType1
{
    MSG_XML,
    MSG_JSON
};


struct structMsgHeader{
	long length;
	short sVersion;
	short sMsgType1;
	short sMsgType2;
	short sMsgType3;
	long lData;
	
};
		
struct structMessage {
	structMsgHeader hdr;
	BYTE byteData;
};
*/

//"MsgType1" and "MsgType2" s/b defined in TCPConnector.h - shows what is supported by webservice (?)


// "MsgType3" definitions to distinguish between  
#define TCP_APPID_PRIMER 0
const enum TCP_APP_ID   //App Identifiers
{
    APPID_PRIMER=0,        //Primer message internal to TCPConnector
    APPID_MOBILECART,

    //Any SSF app IDs must be greater than APPID_MAXBASEAPPID
    APPID_MAXBASEAPPID=1000
};

const enum MOBILECART_MSGDATATYPE
{
    MOBILECARTDATA_READYFORTRANSACTION,
    MOBILECARTDATA_NOTREADYFORTRANSACTION,
    MOBILECARTDATA_UPLOADTICKET   
};


#endif // !defined(SCOT_TCPCONNECTOR__INCLUDED_)
