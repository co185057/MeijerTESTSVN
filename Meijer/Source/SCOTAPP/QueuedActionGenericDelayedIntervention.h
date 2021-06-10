#ifndef _CQUEUED_ACTION_DELAYED_GENERIC_INTERVENTION
#define _CQUEUED_ACTION_DELAYED_GENERIC_INTERVENTION

#include "IQueuedAction.h"

class CDataNeededUtil;
enum DMTriColorLightColor;
enum DMTriColorLightState;

#ifdef _SCOTSSF_
class DLLIMPORT
#elif defined _SCOT
class DLLEXPORT
#else
class 
#endif
 CQueuedActionGenericDelayedIntervention : public IQueuedAction
{
public:
    CQueuedActionGenericDelayedIntervention(const CString &csData);
    virtual ~CQueuedActionGenericDelayedIntervention();

    virtual SMStateBase* HandleQueuedAction(SMStateBase* pCurrentState);

    /**
     * \return 
     *
     * \note   
     */
    virtual CString GetInterventionInstruction(void) const;
    virtual CString GetVideoPath();
    virtual int GetReportingID();
    virtual void SetReportingID(int iValue);
    virtual DMTriColorLightColor GetLightColor();
    virtual DMTriColorLightState GetLightState();

protected:
    CDataNeededUtil* m_DataNeededUtil;
    CString m_csInterventionInstruction;
    DMTriColorLightColor m_dmTriColor;
    DMTriColorLightState m_dmTriState;
};

#endif // _CQUEUED_ACTION_DELAYED_GENERIC_INTERVENTION
