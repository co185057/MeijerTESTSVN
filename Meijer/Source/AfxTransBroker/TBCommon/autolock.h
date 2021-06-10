#ifndef _AUTOLOCKCRITICALSECTION_H_
#define _AUTOLOCKCRITICALSECTION_H_

class CAutoLock
{
public:
   CAutoLock(CRITICAL_SECTION &m_CritSec)
   {
      m_pCritSec= &m_CritSec;
      EnterCriticalSection(m_pCritSec);
   };
   ~CAutoLock()
   {
      LeaveCriticalSection(m_pCritSec);
   };
private:
   CRITICAL_SECTION *m_pCritSec;

};

#endif