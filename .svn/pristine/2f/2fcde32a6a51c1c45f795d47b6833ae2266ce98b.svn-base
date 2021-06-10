#include "StdAfx.h"                 // MS MFC always needed first
#ifndef _CPPUNIT
#include "Common.h"                 // MGV common includes
#include "SMSystemMessageStartup.h" // MGV sys msg 3 state
#include "SMSmMediaStatus.h"
#include "LaneStateData.h"
#else
#include "SMSystemMessageStartupBase.h"
#endif //CPPUNIT


#include <assert.h>

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SystemMessageStartupBase")

#ifndef _CPPUNIT
#include "MethodTrace.h"
#endif //CPPUNIT

// MFC Runtime class/object information
IMPLEMENT_DYNCREATE(SMSystemMessageStartupBase, CObject)

DEFINE_TIMESTAMP

/////////////////////////////////////////////////////
// Constructor
// sys msg 4 is level 1 with op/passwd
/////////////////////////////////////////////////////
#ifndef _CPPUNIT
SMSystemMessageStartupBase::SMSystemMessageStartupBase
                (LPCTSTR szMessage,   // char message text or NULL
                 PSTEXT psMessage,   // message text
                 PSTEXT promptText,  // prompt text or NULL
                 long   promptValue, // prompt area sale value
                 long   deviceClass, // device class, -1 for none
                 bool   checkHealth, // DMCheckHealth after msg?
                 bool displayHTML)  //  SR672
// Construct our parent, most things are handled there
: STATE(SystemMessage4)(szMessage,
                        psMessage,
                        promptText,
                        promptValue,
                        deviceClass,
                        checkHealth,
                        displayHTML), 
                        m_bDeviceErrorNotFixed(false)

{
    IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSystemMessageStartupBase::SMSystemMessageStartupBase() : 
                        m_bDeviceErrorNotFixed(false)
{
    IMPLEMENT_TIMESTAMP
}

SMSystemMessageStartupBase::~SMSystemMessageStartupBase()
{
}


SMStateBase  *SMSystemMessageStartupBase::Deliver(LPCTSTR szMessage,
                                            PSTEXT psMessage,   
                                            PSTEXT promptText,  
                                            long   promptValue, 
                                            long   deviceClass, 
                                            bool   checkHealth,
                                            bool displayHTML) 
{
    STATE(SystemMessage4)::Deliver(szMessage, 
                                  psMessage,
                                  promptText,
                                  promptValue,
                                  deviceClass,
                                  checkHealth,
                                  displayHTML);

    return this;
}

bool SMSystemMessageStartupBase::DeliverDescriptorList(
                                              CDescriptorList &dlParameterList)
{
    return STATE(SystemMessage4)::DeliverDescriptorList(dlParameterList);
}

SMStateBase * SMSystemMessageStartupBase::PSButton1(void)
{
    TRACE_METHOD(_T("SMSystemMessageStartupBase::PSButton1"));

    // If not authenticated yet, then prompt for a password.
    if(inputMode != OPERATOR_PASSWORD_ACCEPTED)
    {
        return STATE(SystemMessage4)::PSButton1();
    } 

    // If returning from "not fixed" screen, then toggle back to initial 
    // error screen.
    if(m_bDeviceErrorNotFixed)
    {
        m_bDeviceErrorNotFixed = false;             // Reset toggle.
        showScreen(true);
        return STATE_NULL;
    }

    // If this error requires the cash/paper status screen to fix.
    if(theSystemMessage.IsMediaStatusError())
    {
        trace(L6, _T("This error requires cash management to correct."));
        fStateFreeze = false;       // No choice.  We have to leave. 

        // Flag this as a special kind of error.
        m_nDevInitStatus = DMProcedures::DM_DEVICE_CASH_LEVEL_ERROR;     
        RETURNSTATE(SmMediaStatus);
    }

    assert(dm.m_nStartDeviceErrorDeviceClass != -1);
    if(dm.m_nStartDeviceErrorDeviceClass <0)
    {
        trace(L4, _T("FATAL ERROR:  Invalid device class [%d]!"),
              dm.m_nStartDeviceErrorDeviceClass);
        return STATE_NULL;
    }

    CLaneStateData laneStateData(m_pAppPSX);
    laneStateData.SendLaneStateData(_T("OutOfService2"), _T(""), false, false);

    // Change to the "Startup" context so that the "I Corrected the Problem"
    // button won't be there to be pressed again while we're reloading.
    CString csCurrentCtx = ps.GetCurrentContext();  // Save for later restore.
    ps.SetCMFrame(_T("Startup"));
    ps.Echo(DM_LOADING);
    DMSayPhrase(KEYPADVALID);
    ps.ShowCMFrame();

    // Try to load the device.  If it fails, then inform the user.
    m_nDevInitStatus = dm.ReloadDevice(dm.m_nStartDeviceErrorDeviceClass);
    if(m_nDevInitStatus != OPOS_SUCCESS)
    {
        trace(L6, _T("Device error still exists!  ReloadDevice returns [%d]"),
              m_nDevInitStatus);

        m_bDeviceErrorNotFixed = true;
        ps.SetCMFrame(csCurrentCtx);    // Restore previous screen.
        return theSystemMessage.ShowDeviceErrorNotFixedScreen();
    }

    // If reload succeeded, then check all of the devices again.
    m_nDevInitStatus = DMInitialize();
    if(m_nDevInitStatus != OPOS_SUCCESS)
    {
        trace(L4, _T("Devices still in error.  DMInitialize() returns [%d]"),
              m_nDevInitStatus);

        // Note:  We are not returning ShowDeviceErrorNotFixedScreen here
        //        because there may be a different error.  So we will drop down
        //        to the STATE_RETURN and re-enter this state with the new
        //        error.
        //        This is not optimal because the user will have to login again
        //        to see the same error.
        //        This needs to be fixed, but not addressing this now (This is
        //        also how the old dev error handling worked before).
    }

    return STATE_RETURN;
}

SMStateBase * SMSystemMessageStartupBase::PSButton2(void)
{
    TRACE_METHOD(_T("SMSystemMessageStartupBase::PSButton2"));

    // +TAR 452451:  Prevent attempted transition to TakeReceipt.  Base class
    //               is incorrectly looking for the word "SMOutOfService" in 
    //               the current state name.  This state doesn't have that, of
    //               course so that kludge fails.
    //               Using this approach because it has a lower risk of 
    //               breaking existing degraded mode code.
    bool bCashChangerError = theSystemMessage.m_bCashChangerError;
    theSystemMessage.m_bCashChangerError = false;

    SMStateBase *pRetState = STATE(SystemMessage4)::PSButton2();

    theSystemMessage.m_bCashChangerError = bCashChangerError;

    if(pRetState == STATE_RESUME)           // Going into degraded mode?
    {
        m_nDevInitStatus = DMInitialize();  // Make sure other devices are up.
    }
    return pRetState;
    // -TAR 452451
}

SMStateBase* SMSystemMessageStartupBase::createLevel1Message( LPCTSTR szMsg,
                                                      PSTEXT pstMsg,
                                                      PSTEXT prompt,
                                                      long   value,
                                                      long   device,
                                                      bool   checkHealth,
                                                      bool displayHTML)
{
    TRACE_METHOD(_T("SMSystemMessageStartupBase::createLevel1Message"));

    trace(L6, _T("Caught error for dev class %d while in startup error."),
          device); 

    // Force a login for this error when it is finally popped off the stack.
    // If we don't do this, then a SystemMessage1 will be pushed on the stack.
    // This will cause a SystemMessage4 to be pushed.
    enum INPUT_MODE inputSave = inputMode;
    inputMode = PROMPTING_FOR_OPERATOR;

    SMStateBase *pRet = STATE(SystemMessage4)::createLevel1Message(szMsg,
                                                                   pstMsg,
                                                                   prompt,
                                                                   value,
                                                                   device,
                                                                   checkHealth,
                                                                   displayHTML); 
    // Restore input mode.
    inputMode = inputSave;

    return pRet;
}
#endif //_CPPUNIT

