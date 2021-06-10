//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmEnhancedAssistMode.h
//
// TITLE: Class header for Enhanced Assist mode keyboard entry mode
//
// AUTHOR:    Robert Susanto
//
// CHANGE HISTORY:
//
// POS309081/POS31053 Work Request:50864 Name:Robert Susanto Date:August 1, 2012
// POS309081/POS31050 Work Request:50864 Name:Robert Susanto Date:April 23, 2012
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSMENHANCEDASSISTMODE
#define _SMSMENHANCEDASSISTMODE

#include "SMSmAssistMode.h"



#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmEnhancedAssistMode : public SMSmAssistMode
{
public:
    SMSmEnhancedAssistMode();
    SMStateBase  *Initialize();
    virtual void     UnInitialize();

    virtual SMStateBase *TBParse(MessageElement *me);

        
protected:
	
    virtual void SetDynakeys();
    virtual void TBShowDisplay(bool bRefresh = false, bool bRepaintLastScreen = false);
    virtual SMStateBase  *PSAssistKey(const long nKey);
    virtual SMStateBase  *DMScanner();             // scanned item

    virtual SMStateBase  *PSButton1();

    bool m_bEnhancedAssistMode; // POS30981 rrs

    bool m_bDisplayKepayd; 

    bool m_bAuditDone;  // POS31053 rrs

private:
    

  DECLARE_DYNCREATE(SMSmEnhancedAssistMode) // MFC Runtime class/object information
};

#endif