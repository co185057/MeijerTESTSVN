// CMOSMsg.h: interface for the CCMOSMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMOSMSG_H__0B2C82CA_48D6_43B3_BDD4_8CCE243AB5EB__INCLUDED_)
#define AFX_CMOSMSG_H__0B2C82CA_48D6_43B3_BDD4_8CCE243AB5EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"

// define duplicate enum values
#define FLAG_BEGINFILE  FLAG_BINARY
#define FLAG_ENDFILE    FLAG_WRITEONLY
#define FLAG_CURRENTPOS FLAG_READONLY

class CCMOSMsg : public CTBMsgObj 
{
public:
   typedef enum { CMD_UNDEF = 0x00,
                  CMD_OPEN  = 0x02,
                  CMD_CLOSE = 0x04,
                  CMD_READ  = 0x0A,
                  CMD_WRITE = 0x0B,
                  CMD_SEEK  = 0x12,
                  CMD_PURGE = 0x88
   } CommandsList;

   typedef enum { FLAG_BINARY    = 0x00,
                  FLAG_READONLY  = 0x01,
                  FLAG_WRITEONLY = 0x02,
                  FLAG_READWRITE = 0x03,
                  FLAG_APPEND    = 0x04,
                  FLAG_TEXT      = 0x08,
                  FLAG_CREATE    = 0x10,
                  FLAG_EXCLUSIVE = 0x20,
                  FLAG_TRUNCATE  = 0x40
//                FLAG_BEGINFILE = 0X00   Defined above
//                FLAG_ENDFILE   = 0X02   Defined above
//                FLAG_CURRENTPOS= 0X01   Defined above

   } FlagList;

   typedef enum { PERM_READONLY  = 0x01,
                  PERM_READWRITE = 0x03
   } PermissionsList;


public:
	CCMOSMsg();
	virtual ~CCMOSMsg();

   inline CommandsList CCMOSMsg::Command()
   {
      return m_Command;
   }

   inline void Command(CommandsList nCommand)
   {
      m_Command = nCommand;
   }

   inline FlagList Flags()
   {
      return m_Flags;
   }

   inline void Flags(FlagList nFlag)
   {
      m_Flags = nFlag;
   }

   inline int FileHandle()
   {
      return m_nFileHandle;
   }

   inline void FileHandle(int nFH)
   {
      m_nFileHandle = nFH;
   }

   inline DWORD Offset()
   {
      return m_dwOffset;
   }

   inline void Offset(DWORD dwOffset)
   {
      m_dwOffset = dwOffset;
   }

   inline unsigned short Permissions()
   {
      return m_usPermissions;
   }

   inline void Permissions(unsigned short nPerm)
   {
      m_usPermissions = nPerm;

   }

   inline DWORD ReturnCode()
   {
      return m_dwReturnCode;
   }

   inline void ReturnCode(DWORD dwRetCode)
   {
      m_dwReturnCode = dwRetCode;
   }

   inline BYTE* Data()
   {
      return m_pData;
   }

   inline void Data(BYTE * pData, int nDataSize)
   {
      if(m_pData)
         delete [] m_pData;
      m_pData = new BYTE[nDataSize];
      memcpy(m_pData, pData, nDataSize);
      m_nDataSize = nDataSize;
   }

   inline int DataSize()
   {
      return m_nDataSize;
   }

protected:
	CommandsList m_Command;
	FlagList m_Flags;
	int	m_nFileHandle;
	DWORD m_dwOffset;
	unsigned short m_usPermissions;
	DWORD m_dwReturnCode;
	BYTE* m_pData;
	int m_nDataSize;

};

#endif // !defined(AFX_CMOSMSG_H__0B2C82CA_48D6_43B3_BDD4_8CCE243AB5EB__INCLUDED_)
