/**********************************************************************************/
//	BaseTBConnectFL.cpp   :   implementation  file for CBaseTBConnectFL  class            
/***********************************************************************************/


#include "stdafx.h"
#include <afxmt.h>                  // Multithreading header
#include "tbdefs.h"
#include "BaseTBConnectFL.h"
#include "TraceSystem.h"
#include "TBUtilities.h"
#include "customertrxbroker.h"

#define T_ID    _T("CBaseTBConnectFL.cpp")

static  CCriticalSection ScotMsgCriticSec;

const CHAR * cCancel= "cCancel";

// initialize message properties, 
CBaseTBConnectFL::CBaseTBConnectFL()
{
    MsgType   = SCOT_MESSAGE;
    MsgSource = ID_TRANSBROKER;
    MsgWParam = 0;
    hScotInputWnd = NULL;
    m_SMessage.szLeadThruText = NULL;
    m_SMessage.szScreenText = NULL;
    m_SMessage.szPlayWave = NULL;
    m_SMessage.szE_ReceiptText = NULL;
    m_SMessage.szRAP_MessageText = NULL;
    m_SMessage.bHiliteReceiptText = false;
    m_SMessage.bNeedItemCancel = false;
    m_SMessage.bApprovalNeeded = false;
    m_SMessage.bGoBackScanAndBag = false;
    m_SMessage.bHiliteRAPText = false;
    m_szCustomerMessageText = _T("");
    m_szCustomerMessageAudio = _T("");
    m_SDataNeededView.szDescription = NULL;
    m_SDataNeededView.szView = NULL;
    m_SDataNeededView.szIniFile = NULL;
    m_SDataNeededView.szSection = NULL;
}

CBaseTBConnectFL::~CBaseTBConnectFL()
{
    // clear any dynamically allocated mem
    ResetMessageInfo(); 
    ResetDataNeededView();
}

TBRC CBaseTBConnectFL::GetMessageInfo(PSMessageInfo psMessageInfo)
{
    TBRC rc = TB_SUCCESS;
    LPCTSTR  lpThisName = _T("CBaseTBConnectFL::GetMessageInfo()");
    tbtrace( TM_ENTRY, _T("+%s"), lpThisName);

    CopyString(psMessageInfo->szLeadThruText, m_SMessage.szLeadThruText);
    CopyString(psMessageInfo->szScreenText, m_SMessage.szScreenText);
    CopyString(psMessageInfo->szPlayWave, m_SMessage.szPlayWave);
    CopyString(psMessageInfo->szE_ReceiptText, m_SMessage.szE_ReceiptText);
    CopyString(psMessageInfo->szRAP_MessageText, m_SMessage.szRAP_MessageText);
    psMessageInfo->bHiliteReceiptText = m_SMessage.bHiliteReceiptText;
    psMessageInfo->bNeedItemCancel = m_SMessage.bNeedItemCancel;
    psMessageInfo->bApprovalNeeded = m_SMessage.bApprovalNeeded;
    psMessageInfo->bGoBackScanAndBag = m_SMessage.bGoBackScanAndBag;
    psMessageInfo->bHiliteRAPText = m_SMessage.bHiliteRAPText;

//    ResetMessageInfo();  // hr129464 TAR 354729 Moved reset to set.

    tbtrace( TM_ENTRY, _T("-%s"), lpThisName);
    return rc;
}

void CBaseTBConnectFL::SetMessageInfo(PSMessageInfo psMessageInfo)
{
	ResetMessageInfo(); //hr129464 TAR 354729 - moved from get - reset before set.
    CopyString(m_SMessage.szLeadThruText, psMessageInfo->szLeadThruText);
    CopyString(m_SMessage.szScreenText, psMessageInfo->szScreenText);
    CopyString(m_SMessage.szPlayWave, psMessageInfo->szPlayWave);
    CopyString(m_SMessage.szE_ReceiptText, psMessageInfo->szE_ReceiptText);
    CopyString(m_SMessage.szRAP_MessageText, psMessageInfo->szRAP_MessageText);
    m_SMessage.bHiliteReceiptText = psMessageInfo->bHiliteReceiptText;
    m_SMessage.bNeedItemCancel = psMessageInfo->bNeedItemCancel;
    m_SMessage.bApprovalNeeded = psMessageInfo->bApprovalNeeded;
    m_SMessage.bGoBackScanAndBag = psMessageInfo->bGoBackScanAndBag;
    m_SMessage.bHiliteRAPText = psMessageInfo->bHiliteRAPText;
}

void CBaseTBConnectFL::ResetMessageInfo(void)
{
    if(m_SMessage.szLeadThruText)
    {
        delete m_SMessage.szLeadThruText;
    }
    m_SMessage.szLeadThruText = NULL;
    
    if(m_SMessage.szScreenText)
    {
        delete m_SMessage.szScreenText;
    }
    m_SMessage.szScreenText = NULL;
    
    if(m_SMessage.szPlayWave)
    {
        delete m_SMessage.szPlayWave;
    }
    m_SMessage.szPlayWave = NULL;
    
    if(m_SMessage.szE_ReceiptText)
    {
        delete m_SMessage.szE_ReceiptText;
    }
    m_SMessage.szE_ReceiptText = NULL;
    
    if(m_SMessage.szRAP_MessageText)
    {
        delete m_SMessage.szRAP_MessageText;
    }
    m_SMessage.szRAP_MessageText = NULL;

    m_SMessage.bHiliteReceiptText = false;
    m_SMessage.bNeedItemCancel = false;
    m_SMessage.bApprovalNeeded = false;
    m_SMessage.bGoBackScanAndBag = false;
    m_SMessage.bHiliteRAPText = false;
}

TBRC CBaseTBConnectFL::GetCustomerMessage(LPTSTR* psCustomerMessageText, 
                                          LPTSTR* psCustomerMessageAudio)
{
    TBRC rc = TB_SUCCESS;
    LPCTSTR  lpThisName = _T("CBaseTBConnectFL::GetCustomerMessage()");
    tbtrace( TM_ENTRY, _T("+%s"), lpThisName);
    
    CopyString(*psCustomerMessageText, m_szCustomerMessageText);
    CopyString(*psCustomerMessageAudio, m_szCustomerMessageAudio);

    ResetCustomerMessageInfo();

    tbtrace( TM_ENTRY, _T("-%s"), lpThisName);
    return rc;
}

void CBaseTBConnectFL::SetCustomerMessageText(CString szVal)
{
    m_szCustomerMessageText = szVal;
}

void CBaseTBConnectFL::SetCustomerMessageAudio(CString szVal)
{
    m_szCustomerMessageAudio = szVal;
}

void CBaseTBConnectFL::ResetCustomerMessageInfo()
{
    m_szCustomerMessageText.Empty();
    m_szCustomerMessageAudio.Empty();
}

TBRC CBaseTBConnectFL::GetDataNeededView(PSDataNeededView psDataNeededView)
{
    TBRC rc = TB_SUCCESS;
    LPCTSTR  lpThisName = _T("CBaseTBConnectFL::GetDataNeededView()");
    tbtrace( TM_ENTRY, _T("+%s"), lpThisName);

    CopyString(psDataNeededView->szDescription, m_SDataNeededView.szDescription);
    CopyString(psDataNeededView->szView, m_SDataNeededView.szView);
    CopyString(psDataNeededView->szIniFile, m_SDataNeededView.szIniFile);
    CopyString(psDataNeededView->szSection, m_SDataNeededView.szSection);

    ResetDataNeededView();

    tbtrace( TM_ENTRY, _T("-%s"), lpThisName);
    return rc;
}

void CBaseTBConnectFL::SetDataNeededView(PSDataNeededView psDataNeededView)
{
    CopyString(m_SDataNeededView.szDescription, psDataNeededView->szDescription);
    CopyString(m_SDataNeededView.szView, psDataNeededView->szView);
    CopyString(m_SDataNeededView.szIniFile, psDataNeededView->szIniFile);
    CopyString(m_SDataNeededView.szSection, psDataNeededView->szSection);
}

void CBaseTBConnectFL::ResetDataNeededView(void)
{
    if (m_SDataNeededView.szDescription)
    {
        delete m_SDataNeededView.szDescription;
        m_SDataNeededView.szDescription = NULL;
    }

    if (m_SDataNeededView.szView)
    {
        delete m_SDataNeededView.szView;
        m_SDataNeededView.szView = NULL;
    }

    if (m_SDataNeededView.szIniFile)
    {
        delete m_SDataNeededView.szIniFile;
        m_SDataNeededView.szIniFile = NULL;
    }

    if (m_SDataNeededView.szSection)
    {
        delete m_SDataNeededView.szSection;
        m_SDataNeededView.szSection = NULL;
    }
}

TBRC CBaseTBConnectFL::SendDataNeeded(LPCTSTR szDataNeededDetails)
{
    TBRC rc = TB_SUCCESS;
    tbtraceIO(_T("SendDataNeeded"));
    tbtrace( TM_INFO, _T("szDataNeededDetails=%s"), szDataNeededDetails);

    long lRet = 0;
    CString csDataEntryDetails(szDataNeededDetails);
    CString csDataEntered, csCommand;

    int iFind = csDataEntryDetails.Find(_T(";"));
    if (-1 != iFind)
    {
        csDataEntered = csDataEntryDetails.Right(csDataEntryDetails.GetLength()-1-iFind);
        if (iFind)
        {
            csCommand = csDataEntryDetails.Left(iFind);
        }
    }

    CCustomerTrxBroker *pTrx = CCustomerTrxBroker::instance();
    
    // reset the last needed property
    pTrx->TBSetProperty(_T("DATANEEDED_VALUE"), _T(""));
    
    // Send to message map handler, appending the RETDN code
    CString strState = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();
    strState += _T("_RETDN");
    if( csCommand.GetLength() )
    {
       strState += _T("_") + csCommand;
    }
         
    pTrx->SetCurrentInProgressState(strState);
    pTrx->TBSetProperty(_T("DATANEEDED_VALUE"), csDataEntered);

    pTrx->TriggerMsgMap();

    return rc;
}
