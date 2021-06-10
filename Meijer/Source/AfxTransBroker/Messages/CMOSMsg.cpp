// CMOSMsg.cpp: implementation of the CCMOSMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CMOSMsg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCMOSMsg::CCMOSMsg() :
   CTBMsgObj(TB_DEV_CMOS),
	m_Command(CMD_UNDEF),
	m_Flags(FLAG_READONLY),
	m_nFileHandle(0),
	m_dwOffset(0),
	m_usPermissions(0),
	m_dwReturnCode(0),
	m_pData(NULL),
	m_nDataSize(0)
{

}

CCMOSMsg::~CCMOSMsg()
{
   delete [] m_pData;
}

