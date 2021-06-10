// ToneIndicatorMsg.h: interface for the CToneIndicatorMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TONEINDICATORMSG_H__15E5D16C_7206_4E8B_A5A0_3E3B0581F921__INCLUDED_)
#define AFX_TONEINDICATORMSG_H__15E5D16C_7206_4E8B_A5A0_3E3B0581F921__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"

typedef struct _tToneInfo {
    long lTone1Pitch;
    long lTone1Volume;
    long lTone1Duration;
    long lTone2Pitch;
    long lTone2Volume;
    long lTone2Duration;
    long lInterToneWait;
} tToneInfo;

typedef enum _eToneMethod {
    cClearOutput,
    cSound,
    cSoundImmediate
} eToneMethod;

class CToneIndicatorMsg : public CTBMsgObj  
{
public:
	CToneIndicatorMsg();
    CToneIndicatorMsg( eToneMethod ToneMethod, tToneInfo ToneData, 
        long lNumberCycles, long lInterSoundWait );

	virtual ~CToneIndicatorMsg();

    virtual void Parse(BYTE *pBuf);
    virtual BYTE* Serialize(int &byteLength);

    tToneInfo ToneDetails( );
    void ToneDetails( tToneInfo ToneData );

    eToneMethod ToneMethod();
    void ToneMethod( eToneMethod Method );

    long NumberCycles();
    void NumberCycles( long lValue );

    long InterSoundWait();
    void InterSoundWait( long lValue );

protected:
   virtual int getLength(void);

private:
    eToneMethod m_ToneMethod;
    tToneInfo   m_ToneData;
    long        m_lNumberCycles;
    long        m_lInterSoundWait;
};

#endif // !defined(AFX_TONEINDICATORMSG_H__15E5D16C_7206_4E8B_A5A0_3E3B0581F921__INCLUDED_)
