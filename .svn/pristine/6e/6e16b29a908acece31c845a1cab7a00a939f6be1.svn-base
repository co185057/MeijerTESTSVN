#ifndef _CQUEUED_ACTION_SHOW_MESSAGE_SCREEN
#define _CQUEUED_ACTION_SHOW_MESSAGE_SCREEN

#include "IQueuedAction.h"

#define QUEUED_MESSAGESCREENTITLE _T("MESSAGESCREENTITLE")
#define QUEUED_MESSAGESCREENTEXT _T("MESSAGESCREENTEXT")
#define QUEUED_MESSAGESCREENITEMNEEDCANCEL _T("MESSAGESCREENITEMNEEDCANCEL")
#define QUEUED_MESSAGESCREENWAVEFILE _T("MESSAGESCREENWAVEFILE")
#define QUEUED_MESSAGESCREENGOBACKSCANANDBAG _T("MESSAGESCREENGOBACKSCANANDBAG")


#ifdef _SCOTSSF_
class DLLIMPORT
#elif defined _SCOT
class DLLEXPORT
#else
class 
#endif
 CQueuedActionShowMessageScreen : public IQueuedAction
{
public:
    CQueuedActionShowMessageScreen(const CString &csData);
    virtual ~CQueuedActionShowMessageScreen();

    virtual SMStateBase* HandleQueuedAction(SMStateBase* pCurrentState);

protected:
    CString m_csMessageScreenTitle;
    CString m_csMessageScreenText;
    CString m_csPlayWave;
};

#endif // _CQUEUED_ACTION_SHOW_MESSAGE_SCREEN
