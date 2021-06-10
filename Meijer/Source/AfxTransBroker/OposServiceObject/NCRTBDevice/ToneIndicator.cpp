// ToneIndicator.cpp: implementation of the CToneIndicator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToneIndicator.h"
#include "ToneIndicatorMsg.h"
#include "TBService.h"
#include "SocketForTBDevice.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToneIndicator::CToneIndicator(int nPort, int nId,  CmDataCapture& cmDc)
                :CTBDevice(OPOS_CLASSKEY_TONE,nId,nPort, cmDc)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CToneIndicator::CToneIndicator") );

}

CToneIndicator::~CToneIndicator()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CToneIndicator::~CToneIndicator") );

}

long CToneIndicator::Open( CTBService* pService, COposRegistry& oRegistry )
{
    BOOL    bCapPitch;
    BOOL    bCapVolume;

    long    lResult;

    bCapPitch = oRegistry.ReadBool( _T("CapPitch"), 0, true );
    bCapVolume = oRegistry.ReadBool(_T("CapVolume"), 0, true );
   
    pService->InitializeToneIndicator( bCapPitch, bCapVolume );

    lResult = CTBDevice::Open( pService, oRegistry );

	return lResult;
}

void CToneIndicator::Close( CTBService* pService )
{
	CTBDevice::Close( pService);
}

void CToneIndicator::ProcessRawData( void* pData, DWORD dwByteLen )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CToneIndicator::ProcessRawData") );
    m_cmDc.DCPrintf(TRACE_ALWAYS, _T("Message not expected!!!!"));
}

long CToneIndicator::Sound(long NumberOfCycles, long InterSoundWait)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CToneIndicator::Sound") );
    CToneIndicatorMsg Msg;
    tToneInfo ToneData;

    ToneData.lInterToneWait = this->m_pTBService->GetPropertyNumber(PIDXTone_InterToneWait);
    ToneData.lTone1Duration = this->m_pTBService->GetPropertyNumber(PIDXTone_Tone1Duration);
    ToneData.lTone1Pitch = this->m_pTBService->GetPropertyNumber(PIDXTone_Tone1Pitch);
    ToneData.lTone1Volume = this->m_pTBService->GetPropertyNumber(PIDXTone_Tone1Volume);
    ToneData.lTone2Duration = this->m_pTBService->GetPropertyNumber(PIDXTone_Tone2Duration);
    ToneData.lTone2Pitch = this->m_pTBService->GetPropertyNumber(PIDXTone_Tone2Pitch);
    ToneData.lTone2Volume = this->m_pTBService->GetPropertyNumber(PIDXTone_Tone2Volume);

    Msg.ToneMethod( cSoundImmediate );
    Msg.ToneDetails( ToneData );
    Msg.NumberCycles( NumberOfCycles );
    Msg.InterSoundWait( InterSoundWait );

    long lResult = NotifyTB( &Msg );

    BOOL bAsync = this->m_pTBService->GetPropertyNumber(PIDXTone_AsyncMode);

    // For synchronous mode, delay as if we were playing the sounds.
    if ( (OPOS_SUCCESS == lResult) && !bAsync )
    {
        DWORD dwWaitTime;

        long lCycles = NumberOfCycles;
        if (NumberOfCycles < 1)
        {
            lCycles = 1;
        }
        long lWait = InterSoundWait;
        if ( lWait < 0 )
        {
            lWait = 0;
        }
        dwWaitTime = ((ToneData.lTone1Duration +
                     ToneData.lInterToneWait +
                     ToneData.lTone2Duration) * lCycles) +
                     ((lCycles - 1)*lWait);

        Sleep( dwWaitTime );
    }

    return lResult;
}

long CToneIndicator::SoundImmediate()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CToneIndicator::SoundImmediate") );
    CToneIndicatorMsg Msg;
    tToneInfo ToneData;

    ToneData.lInterToneWait = this->m_pTBService->GetPropertyNumber(PIDXTone_InterToneWait);
    ToneData.lTone1Duration = this->m_pTBService->GetPropertyNumber(PIDXTone_Tone1Duration);
    ToneData.lTone1Pitch = this->m_pTBService->GetPropertyNumber(PIDXTone_Tone1Pitch);
    ToneData.lTone1Volume = this->m_pTBService->GetPropertyNumber(PIDXTone_Tone1Volume);
    ToneData.lTone2Duration = this->m_pTBService->GetPropertyNumber(PIDXTone_Tone2Duration);
    ToneData.lTone2Pitch = this->m_pTBService->GetPropertyNumber(PIDXTone_Tone2Pitch);
    ToneData.lTone2Volume = this->m_pTBService->GetPropertyNumber(PIDXTone_Tone2Volume);

    Msg.ToneMethod( cSoundImmediate );
    Msg.ToneDetails( ToneData );
    Msg.NumberCycles( 0);
    Msg.InterSoundWait( 0 );

    return NotifyTB( &Msg );
}

void CToneIndicator::ClearOutput()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CToneIndicator::ClearOutput") );
    CToneIndicatorMsg Msg;
    tToneInfo ToneData;

    ToneData.lInterToneWait = 0;
    ToneData.lTone1Duration = 0;
    ToneData.lTone1Pitch = 0;
    ToneData.lTone1Volume = 0;
    ToneData.lTone2Duration = 0;
    ToneData.lTone2Pitch = 0;
    ToneData.lTone2Volume = 0;

    Msg.ToneMethod( cClearOutput );
    Msg.ToneDetails( ToneData );
    Msg.NumberCycles( 0 );
    Msg.InterSoundWait( 0 );

    NotifyTB( &Msg );
}

// 
// Function:  NotifyTB
// Purpose:  Sends a message to the TB
// Inputs: pBuffer - buffer containing the message
//         lBufferLen - length of the data to be sent
// Outputs:  OPOS_SUCCESS
// Assumptions:  None
// Comments:  None
// 
long CToneIndicator::NotifyTB( CToneIndicatorMsg* msg )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CToneIndicator::NotifyTB") );

    if (CSocketForTBDevice::GetSpecVersion() > 1) 
    {
        int iMsgSize = 0;
        BYTE *pBuf = msg->Serialize( iMsgSize );

        int iSent = Send(pBuf, iMsgSize);
        delete [] pBuf;
        if (iSent == SOCKET_ERROR)
        {
           m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Failed to write to PipeServer Socket"));
        }
    }
    else
    {
        return OPOS_E_ILLEGAL;
    }
    return OPOS_SUCCESS;
}
