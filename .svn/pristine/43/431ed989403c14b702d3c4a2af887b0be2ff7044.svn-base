// DisplayMsg.cpp: implementation of the CDisplayMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DisplayMsg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDisplayMsg::CDisplayMsg(TBDEVICEID devId /*=TB_DEV_DISPLAYPRIMARY*/) :
   CTBMsgObj(devId),
   m_nWindowId(0),
   m_unTextRow(0),
   m_unTextColumn(0),
   m_Type(NORMAL)
{
}

CDisplayMsg::~CDisplayMsg()
{

}


BYTE* CDisplayMsg::Serialize(int &byteLength)
{
    SerializeHeader(byteLength); // must be called first 
    
   // serialize all the data
   //SerializeString(ScreenName());
   //...

   SerializeExtensionData();
   return m_pBuf;
    
}

void CDisplayMsg::Parse(BYTE *pBuf)
{
    ParseHeader(pBuf); // must be called first

    // parse all data in
    //ScreenName(ParseString(usScreenName));
    //...

    ParseExtensionData();
}


int CDisplayMsg::getLength(void)
{
   return   sizeof(tRouteHeader) + 
            // add all the data lengths
            //ScreenName().GetLength() * sizeof(TCHAR) + 
            GetLengthExtensionData();
}