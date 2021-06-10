//*************************************************************************************
//	BaseTBConnectPos.cpp	:	Definition file for the CBaseTBConnectPos class
//
//*************************************************************************************

#include "stdafx.h"
#include "BaseTBConnectPOS.h"
#include "TraceSystem.h"
#include "CustomerPOSEmulation.h"
#include "keyboardMsg.h"

#define T_ID    _T("BaseTBConnectPOS")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseTBConnectPOS::CBaseTBConnectPOS()
{
    m_pEmulationObj= NULL;
    m_pTBScotMsg = NULL;
}

CBaseTBConnectPOS::~CBaseTBConnectPOS()
{
    
}

TBRC CBaseTBConnectPOS::CopyFileFromServer(LPCTSTR szFileName, 
                                           LPCTSTR szDestinationFilePath, const BOOL fForce)
{
    // Incomplete Base TB Function: This function needs to be implemented for the Base TB code.
    /*<%ASIS
    return TB_SUCCESS;
    %>*/
//    tbtrace( TM_DEBUG, _T("+-CopyFileFromServer() Empty Base Functionality"));
    return TB_SUCCESS;
}

TBRC CBaseTBConnectPOS::CopyFileToServer(LPCTSTR szFileName, LPCTSTR szSourceFilePath)
{
    // Incomplete Base TB Function: This function needs to be implemented for the Base TB code.
    /*<%ASIS
    return TB_SUCCESS;
    %>*/
    TBRC rc = TB_SUCCESS;
//    tbtrace( TM_DEBUG, _T("+-CopyFileToServer() Empty Base Functionality"));
    return rc;
}

TBRC CBaseTBConnectPOS::LogErrorToServer(LPCTSTR szErrorText, const int nSeverity,
                                         const int nComponentID, LPCTSTR szComponentID,
                                         LPCTSTR szModuleID, const int nModuleLineNumber)
{
    // Incomplete Base TB Function: This function needs to be implemented for the Base TB code.
    /*<%ASIS
    return TB_SUCCESS;
    %>*/
    TBRC rc = TB_SUCCESS;
//    tbtrace( TM_DEBUG, _T("+-LogErrorToServer() Empty Base Functionality"));
    return rc;
}

TBRC CBaseTBConnectPOS::InitHostAppExit(void)
{
    // Incomplete Base TB Function: This function needs to be implemented for the Base TB code.
    /*<%ASIS
    return TB_SUCCESS;
    %>*/
//    tbtrace( TM_DEBUG, _T("+-InitHostAppExit() Empty Base Functionality"));
    return TB_SUCCESS;
}

TBRC CBaseTBConnectPOS::SendMsg2Host(LPCTSTR szSend, const int iMsgLen,
                                     const TBDEVICEID nDeviceClass, const UCHAR nFunctionCode,
                                     const long	lOptFileHandle,	const bool fOriginatePipeMsg)
{
    // Incomplete Base TB Function: This function needs to be implemented for the Base TB code.
    /*<%ASIS
    return TB_SUCCESS;
    %>*/
//    tbtrace( TM_DEBUG, _T("+-SendMsg2Host() Empty Base Functionality"));
    return TB_SUCCESS;
}

bool CBaseTBConnectPOS::InitEmulation()
{
    if(! m_pEmulationObj) {
        m_pEmulationObj = (ATBEmulationBase*) new CCustomerPOSEmulation();
        m_pEmulationObj->Initialize();
        m_pEmulationObj->SetTBConnection(this);
        m_pTBScotMsg= CCustomerTBScotMsg::instance();
    }
    else
    {
        tbtrace(TM_WARNING, _T("ATBEmulationBase already initialized. Ignoring InitEmulation"));
    }
    return true;
}

void CBaseTBConnectPOS::Shutdown()
{
    if(m_pEmulationObj != NULL)
    {
        m_pEmulationObj->Uninitialize();
        delete m_pEmulationObj;
        m_pEmulationObj = NULL;
    }
    if ( m_pTBScotMsg != NULL )
    {
        delete m_pTBScotMsg;
        m_pTBScotMsg = NULL;
    }
}


void CBaseTBConnectPOS::ReceiveMsg(CTBMsgObj * pMsg)
{
    m_pTBScotMsg->SendScotMsg(pMsg->GetDeviceId(), 0, NULL, (LPARAM) pMsg);
}

void CBaseTBConnectPOS::SendMsgToPOS(CTBMsgObj &Msg)
{
    m_pEmulationObj->PostHostMsg(&Msg);
}

BOOL CBaseTBConnectPOS::SendSequence( CString &csStr)
{
   BOOL bSuccess=TRUE;

   LPCTSTR szStartDelim = _T("{");
   LPCTSTR szStopDelim = _T("}");

   int posStart= csStr.Find(szStartDelim);
   if (posStart >=0)
   {
      int posStop= csStr.Find(szStopDelim, posStart);
      if (posStop > 0) 
      {
         if (posStart > 0)
             SendData( (LPARAM)(LPCTSTR) csStr.Left(posStart));

         int nStartDelimLen= _tcslen(szStartDelim);
         int nStopDelimLen= _tcslen(szStopDelim);
         // get tag
         CString csTagName= csStr.Mid(posStart + nStartDelimLen, 
                                      posStop - posStart - nStartDelimLen);

         SendInput( (LPARAM )(LPCTSTR )csTagName);

         csStr = csStr.Mid(posStop + nStopDelimLen);
         // recurse...
         if (csStr.GetLength())
             SendSequence(csStr);
      }
   }
   else
   {
      SendData( (LPARAM )(LPCTSTR) csStr);
   }
   return bSuccess;
}

/*************************************************************************
* SendData - EMPTY!!  implementation is TB and Hook specific
*
* Parameters: 
*  LPARAM lparam - user defined buffer of data to send to POS
*
* Returns: TRUE if input messages sent, FALSE otherwise
*************************************************************************/
BOOL CBaseTBConnectPOS::SendData( LPARAM lparam)
{
	tbtrace(TM_INFO, _T( "SendData(%s) - Empty "), (LPCTSTR) lparam);
    return TRUE;
}

/*************************************************************************
* SendInput - Send key input to the POS EMPTY!!  implementation is TB and Hook specific
*
* Parameters: 
*  LPARAM lparam - some key to send as defined in option file - Assumes null terminated 
*
* Returns: FALSE 
*************************************************************************/
BOOL CBaseTBConnectPOS::SendInput( LPARAM lparam)
{
	tbtrace(TM_INFO, _T( "SendInput(%s) - Empty"), (LPCTSTR)lparam);
    return TRUE;
}


