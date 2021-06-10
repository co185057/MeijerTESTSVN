//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCustomMessage.h
//
// TITLE: Class header for CustomMessage State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CUSTOMMESSAGESTATE
#define _CUSTOMMESSAGESTATE

#include "SMCustomMessageBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

SMCustomMessage : public SMCustomMessageBase    // TAR #127247
{
public:
    SMCustomMessage();

    virtual SMStateBase *Initialize(void);
	//POS18049
    virtual void UnInitialize(void);
    virtual bool PSAssistMenuLegalForState();

protected:
    virtual SMStateBase *PSButton1(void);
	virtual SMStateBase *PSButton2(void); // POS39475
	virtual SMStateBase *TimedOut(void);  // POS39475
    virtual SMStateBase *PSEnterKey(void);

    DECLARE_DYNCREATE(SMCustomMessage)    // MFC Runtime class/object information

private:
  CString csFoodStampText;// POS260040
  CString csSetItemAsideText; //VSTS 131568

};

#endif