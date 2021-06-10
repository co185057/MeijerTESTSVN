#include "StdAfx.h"                 // MS MFC always needed first

#include "Common.h"                 // MGV common includes
#include "SMSystemMessageStartup.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SystemMessageStartup")

// MFC Runtime class/object information
IMPLEMENT_DYNCREATE(SMSystemMessageStartup, CObject)

DEFINE_TIMESTAMP

SMSystemMessageStartup::SMSystemMessageStartup
                (LPCTSTR szMessage,   // char message text or NULL
                 PSTEXT psMessage,   // message text
                 PSTEXT promptText,  // prompt text or NULL
                 long   promptValue, // prompt area sale value
                 long   deviceClass, // device class, -1 for none
                 bool   checkHealth, // DMCheckHealth after msg?
                 bool displayHTML) 
// Construct our parent, most things are handled there
: SMSystemMessageStartupBase( szMessage,
                              psMessage,
                              promptText,
                              promptValue,
                              deviceClass,
                              checkHealth,
                              displayHTML)
{
    IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSystemMessageStartup::SMSystemMessageStartup()
{
    IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSystemMessageStartup::Deliver(LPCTSTR szMessage,   // char message text or NULL
                     PSTEXT psMessage,   // message text
                     PSTEXT promptText,  // prompt text or NULL
                     long   promptValue, // prompt area sale value
                     long   deviceClass, // device class, -1 for none
                     bool   checkHealth,
                     bool displayHTML)
{
  SMSystemMessageStartupBase::Deliver( szMessage,
                    psMessage,
                    promptText,
                    promptValue,
                    deviceClass,
                    checkHealth,
                    displayHTML);


  return this;
}

bool SMSystemMessageStartup::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMSystemMessageStartupBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMSystemMessageStartup::PSButton1(void)
{
	dm.m_nStartDeviceErrorDeviceClass = 0; //SSCOI-45183
    return SMSystemMessageStartupBase::PSButton1();
}

SMStateBase * SMSystemMessageStartup::PSButton2(void)
{
    return SMSystemMessageStartupBase::PSButton2();
}

SMStateBase * SMSystemMessageStartup::createLevel1Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth,
                                           bool displayHTML)
{
    return SMSystemMessageStartupBase::createLevel1Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
}

CString SMSystemMessageStartup::getWaveFile()
{
    return SMSystemMessageStartupBase::getWaveFile();
}
