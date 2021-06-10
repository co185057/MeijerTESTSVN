// TBMsgObjBase.h: interface for the CTBMsgObjBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TBMSGOBJBASE_H__4DE82155_5F6B_4181_BA41_F31ACF9F9114__INCLUDED_)
#define AFX_TBMSGOBJBASE_H__4DE82155_5F6B_4181_BA41_F31ACF9F9114__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _TB_CONTROLS_
typedef enum
{
   TB_DEV_CMOS=1,                // CMOS device
   TB_DEV_DISPLAYINTERMEDIATE,   // intermediate display
   TB_DEV_DISPLAYPRIMARY,        // The primary display
   TB_DEV_DISPLAYSECONDARY,      // The secondary display
   TB_DEV_DISPLAYIMMEDIATE,      // Assistmode display or monitor all display msgs
   TB_DEV_DRAWER1,               // Primary cash drawer
   TB_DEV_DRAWER2,               // Secondary cash drawer
   TB_DEV_EPSMODULE,             // Electronic payment server as a virtual device
   TB_DEV_KEYBOARD,              // Keyboard device
   TB_DEV_KEYLOCK,				 // keylock device
   TB_DEV_MSR,                   // MSR device
   TB_DEV_PRINTERJOURNAL,        // Journal printer device
   TB_DEV_PRINTERRECEIPT,        // Customer receipt printer device
   TB_DEV_PRINTERSLIP,           // Slip printer device
   TB_DEV_SCALE,                 // Scale device
   TB_DEV_SERIAL,				 // Serial device
   TB_DEV_SCANNER,               // Scanner device
   TB_DEV_ATMMODULE,             // Cash Withdrawal dll as a virtual device
   TB_DEV_UNKNOWN,                // Unknown device or event
   TB_DEV_DISPLAYDATANEEDED,     // Data Needed state     RFC 197844
   TB_DEV_UICOMMAND,             // Send PS commands to FL
} TBDEVICEID;
#else
#include "ScotMsg.h"
#endif

// These device Ids extend TBDEVICEID in scotmsg.h
#define IB_DEV_VAL  10000


#define TB_DEV_MICR                 (TBDEVICEID)(IB_DEV_VAL + 1)
#define TB_DEV_PINPAD               (TBDEVICEID)(IB_DEV_VAL + 2)
#define TB_DEV_RETURN_STATE         (TBDEVICEID)(IB_DEV_VAL + 3)
#define TB_DEV_ITEM_DETAILS         (TBDEVICEID)(IB_DEV_VAL + 4)
#define TB_DEV_REWARD_DETAILS       (TBDEVICEID)(IB_DEV_VAL + 5)
#define TB_DEV_TOTAL_DETAILS        (TBDEVICEID)(IB_DEV_VAL + 6)
#define TB_DEV_TENDER_DETAILS       (TBDEVICEID)(IB_DEV_VAL + 7)
#define TB_DEV_CUSTOM               (TBDEVICEID)(IB_DEV_VAL + 8)
//#define TBDEV_POLL_DEVICE        (TBDEVICEID)(IB_DEV_VAL + 9)
//#define TBDEV_TCP                (TBDEVICEID)(IB_DEV_VAL + 10)
#define TB_DEV_FISCALPRINTER       (TBDEVICEID)(IB_DEV_VAL + 11)
#define TB_DEV_TONEINDICATOR       (TBDEVICEID)(IB_DEV_VAL + 12)
#define TB_DEV_POSSTATE            (TBDEVICEID)(IB_DEV_VAL + 13) //Sinan

// The following are maintained for backward compatibility
#define TBDEV_MICR                 TB_DEV_MICR
#define TBDEV_PINPAD               TB_DEV_PINPAD
#define TBDEV_RETURN_STATE         TB_DEV_RETURN_STATE
#define TBDEV_ITEM_DETAILS         TB_DEV_ITEM_DETAILS
#define TBDEV_REWARD_DETAILS       TB_DEV_REWARD_DETAILS
#define TBDEV_TOTAL_DETAILS        TB_DEV_TOTAL_DETAILS
#define TBDEV_TENDER_DETAILS       TB_DEV_TENDER_DETAILS
#define TBDEV_CUSTOM               TB_DEV_CUSTOM
// The above are maintained for backward compatibility


// Msg ID's per TB Msg Spec
enum MSG_SPEC_DEVICE_IDS
{
   TBMSG_KEYBOARD    =0x10,
   TBMSG_DISPLAY1    =0x20,
   TBMSG_DISPLAY2    =0x21,
   TBMSG_PRINTER     =0x30,
   TBMSG_FPTR        =0x3A,
   TBMSG_MSR         =0x40, 
   TBMSG_SCANNER     =0x4A, 
   TBMSG_CASH_DRAWER =0x54, 
   TBMSG_PINPAD      =0x66,
   TBMSG_MICR        =0x67,
   TBMSG_MICR2       =0xC0,
   TBMSG_SCALE       =0x6A, 
   TBMSG_CMOS        =0x85,
   TBMSG_KEYLOCK     =0x90,
   TBMSG_ITEM        =0xA0,
   TBMSG_TENDER      =0xA1,
   TBMSG_TOTAL       =0xA2,
   TBMSG_REWARD      =0xA3,
   TBMSG_CUSTOM      =0xB0,
   TBMSG_TONE        =0x70,
   TBMSG_POSSTATE    =0xFE,
   TBMSG_SUBSCRIBE   =0xFF 
};

//#include "connectbase.h"

#pragma pack(1)
typedef struct _tSpecHeaderV01{
	ULONG	nLen;
	ULONG	nVersion;
	BYTE	messageID;
	_tSpecHeaderV01():nLen(0),nVersion(1),messageID(0){};
} tSpecHeaderV01;

#define tRouteHeader tSpecHeaderV01 
#pragma pack()

class CTBMsgObj
{
public:
   CTBMsgObj(TBDEVICEID devId);
   virtual ~CTBMsgObj();

   TBDEVICEID GetDeviceId();
   BYTE GetTBMsgId();
	void SetTBMsgId(BYTE tbMsgId);
	int  GetMsgVersion();
	void SetMsgVersion(int nVersion);

   CString ParseString(UINT nlen);
	CString ParseL1String();
	CString ParseL2String();
	CString ParseL4String();
	ULONG ParseULong();
	USHORT ParseUShort();
	BYTE ParseByte();
   BYTE* ParseBinaryBuffer(UINT nlen);
   void ParseHeader(BYTE *pBuf);

   void SerializeByte(BYTE byteVal);
   void SerializeByte(bool bVal);
   void SerializeUShort(USHORT usVal);
   void SerializeULong(ULONG ulVal);
   void SerializeString(const CString & csVal);
   void SerializeL1String(const CString &csVal);
   void SerializeL2String(const CString &csVal);
   void SerializeL4String(const CString &csVal);
   void SerializeBinaryBuffer(BYTE *pBuf, UINT nlen);
   void SerializeHeader(int &byteLength);

   virtual BYTE* Serialize(int &byteLength) =0;
   virtual void Parse(BYTE *pBuf)=0;

	BYTE* GetAndForgetExtensionData();

	int UserLong(void);
	void UserLong(int nNewVal);
	void UserString(BYTE* pNewVal);
    void UserString(const CString &csUserString);
	CString UserString();

protected:
   virtual int getLength(void)=0;

   void SetDeviceId(TBDEVICEID device);
   CString DblToDecString(double dbl);
   long DecStringToLong(LPCTSTR szVal);

	virtual int GetLengthExtensionData();
	virtual void ParseExtensionData();
	virtual void SerializeExtensionData();
   UINT m_nOffset;
   BYTE * m_pBuf;
   tRouteHeader m_Hdr;
	int m_nExtension;
	CString m_csUserString;
	BYTE* m_pExtensionBuf;

private:
   TBDEVICEID  m_dwDeviceId;
   BYTE m_TBMsgId;
};

#endif // !defined(AFX_TBMSGOBJBASE_H__4DE82155_5F6B_4181_BA41_F31ACF9F9114__INCLUDED_)
