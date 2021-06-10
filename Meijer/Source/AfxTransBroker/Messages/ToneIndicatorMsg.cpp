// ToneIndicatorMsg.cpp: implementation of the CToneIndicatorMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToneIndicatorMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToneIndicatorMsg::CToneIndicatorMsg() :
   CTBMsgObj(TB_DEV_TONEINDICATOR)
{
    m_ToneData.lInterToneWait = 0;
    m_ToneData.lTone1Duration = 0;
    m_ToneData.lTone1Pitch = 0;
    m_ToneData.lTone1Volume = 0;
    m_ToneData.lTone2Duration = 0;
    m_ToneData.lTone2Pitch = 0;
    m_ToneData.lTone2Volume = 0;
    m_ToneMethod = cClearOutput;
    m_lNumberCycles = 0;
    m_lInterSoundWait = 0;
}

CToneIndicatorMsg::CToneIndicatorMsg( eToneMethod ToneMethod, tToneInfo ToneData,
                                     long lNumberCycles, long lInterSoundWait) :
   CTBMsgObj(TB_DEV_TONEINDICATOR)
{
    m_ToneMethod = ToneMethod;
    m_ToneData = ToneData;
    m_lNumberCycles = lNumberCycles;
    m_lInterSoundWait = lInterSoundWait;
}

CToneIndicatorMsg::~CToneIndicatorMsg()
{

}

int CToneIndicatorMsg::getLength(void)
{
    int nSize = 0;

    nSize += (int)sizeof(tRouteHeader);    // Size of the header
    nSize += (int)sizeof(BYTE);            // Function Code
    nSize += (int)(7 * sizeof(long));      // OPOS Tones data
    nSize += (int)(2 * sizeof(long));      // Command Arguments
    nSize += GetLengthExtensionData();

    return nSize;
}


void CToneIndicatorMsg::Parse(BYTE *pBuf)
{
    ParseHeader(pBuf);         // must be called first 

    m_ToneMethod = (eToneMethod)ParseByte();
    m_ToneData.lInterToneWait = (long)ParseULong();
    m_ToneData.lTone1Duration = (long)ParseULong();
    m_ToneData.lTone1Pitch = (long)ParseULong();
    m_ToneData.lTone1Volume = (long)ParseULong();
    m_ToneData.lTone2Duration = (long)ParseULong();
    m_ToneData.lTone2Pitch = (long)ParseULong();
    m_ToneData.lTone2Volume = (long)ParseULong();
    m_lNumberCycles = (long)ParseULong();
    m_lInterSoundWait = (long)ParseULong();

    ParseExtensionData();
}

BYTE* CToneIndicatorMsg::Serialize(int &byteLength)
{
    byteLength = 0;
    SerializeHeader(byteLength); // must be called first

    SerializeByte((BYTE)m_ToneMethod);
    SerializeULong((unsigned long)m_ToneData.lInterToneWait);
    SerializeULong((unsigned long)m_ToneData.lTone1Duration);
    SerializeULong((unsigned long)m_ToneData.lTone1Pitch);
    SerializeULong((unsigned long)m_ToneData.lTone1Volume);
    SerializeULong((unsigned long)m_ToneData.lTone2Duration);
    SerializeULong((unsigned long)m_ToneData.lTone2Pitch);
    SerializeULong((unsigned long)m_ToneData.lTone2Volume);
    SerializeULong((unsigned long)m_lNumberCycles);
    SerializeULong((unsigned long)m_lInterSoundWait);
    SerializeExtensionData();

    return m_pBuf;
}

tToneInfo CToneIndicatorMsg::ToneDetails( )
{
    return this->m_ToneData;
}

void CToneIndicatorMsg::ToneDetails( tToneInfo ToneData )
{
    m_ToneData = ToneData;
}

eToneMethod CToneIndicatorMsg::ToneMethod()
{
    return this->m_ToneMethod;
}

void CToneIndicatorMsg::ToneMethod( eToneMethod Method )
{
    m_ToneMethod = Method;
}

long CToneIndicatorMsg::NumberCycles()
{
    return this->m_lNumberCycles;
}

void CToneIndicatorMsg::NumberCycles( long lValue )
{
    m_lNumberCycles = lValue;
}

long CToneIndicatorMsg::InterSoundWait()
{
    return this->m_lInterSoundWait;
}

void CToneIndicatorMsg::InterSoundWait( long lValue )
{
    m_lInterSoundWait = lValue;
}
