#pragma once

//
// Public Types/Constants
//
class CCriticalSectionLock
{
protected:
   CCriticalSection & m_cs;

public:
   CCriticalSectionLock(CCriticalSection & cs, BOOL bInitialLock = FALSE)
      : m_cs(cs)
   {
      if( bInitialLock )
      {
         cs.Enter();
      }
   }
   inline void Enter(void) { m_cs.Enter(); }
   inline void Leave(void) { m_cs.Leave(); }
   ~CCriticalSectionLock()
   {
      m_cs.Leave();
   }
};

class CAppModel;
class CACSHook;
class CContexts;
class CControls;
class CProperties;


class CUtils
{
public:

   //
   // ACSHook stub
   //
   class CACSHookStub
   {
      friend class CUtils;

   protected:
      CACSHook * m_pACSHook;

   public:
      CACSHookStub(void);
      virtual ~CACSHookStub(void);

      bool SendInput(LPCTSTR pszInput);
      bool SendControlToTB(LPCTSTR pszControl);
      bool SendControlToApp(LPCTSTR pszControl);
      void FireEvent(long lEvent, long lData, LPCTSTR szData1 = NULL, LPCTSTR szData2 = NULL);

   };

   //
   // AppModel stub
   //
   class CAppModelStub
   {
      friend class CUtils;

   protected:
      CAppModel * m_pAppModel;

   public:
      CAppModelStub(void);
      virtual ~CAppModelStub(void);

      const PAPPCONTEXTFIELD GetContextField(LPCTSTR szContext, LPCTSTR szContextField);
   };


public:
   //
   // Public Methods
   // 
   CUtils();
   virtual ~CUtils();
   
   double GetMS(void);
   double GetMSDiff(double dLastMS);
   CString GetLastErrorString(void);
   CString GetLastErrorString(DWORD dwError);
   CString GetLoadedModulePath(LPCTSTR pszPartialName);
   void   HexTrace(DWORD dcMask, const BYTE * data, long length);
   inline CContexts & Contexts(void) { return *m_pContexts; }
   inline CControls & Controls(void) { return *m_pControls; }
   inline CProperties & Properties(void) { return *m_pProps; }
   inline CACSHookStub & ACSHook(void) { return m_ACSHook; }
   //inline CAppModelStub & AppModel(void) { return m_AppModel; }

   inline void SetContextsObject(CContexts & contexts) { m_pContexts = & contexts; }
   inline void SetControlsObject(CControls & controls) { m_pControls = & controls; }
   inline void SetACSHookObject(CACSHook & acsHook) { m_ACSHook.m_pACSHook = &acsHook; }
   inline void SetPropertiesObject(CProperties & props) { m_pProps = &props; }
   //inline void SetAppModelObject(CAppModel & appModel) { m_AppModel.m_pAppModel = &appModel; }

private:
   CACSHookStub m_ACSHook;
   CContexts * m_pContexts;
   CControls * m_pControls;
   CProperties * m_pProps;


};

/* Global utilities class used by all */
extern CUtils Utils;
