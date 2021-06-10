// commServer.cpp : Implementation of WinMain

#include "stdafx.h"
#include "resource.h"
//#include "commRouter.h"
#include "commServerDlg.h"

// The module attribute causes WinMain to be automatically implemented for you
[ module(service, uuid = "{06D2012A-BC6E-4FB1-A0C2-4B8962F37D1E}", 
		 name = "commServer", 
		 helpstring = "commServer 1.0 Type Library", 
		 resource_name="IDS_SERVICENAME") ]
class CcommServerModule
{
public:
	HRESULT PreMessageLoop(int nShowCmd)
	{
	//	_ASSERT(CcommRouter::m_pDlg == NULL);

		CcommServerDlg *pDlg = new CcommServerDlg();
		pDlg->Create(NULL);
		pDlg->ShowWindow(SW_SHOWNORMAL);
		//_ASSERT(CcommRouter::m_pDlg != NULL);

		return __super::PreMessageLoop(nShowCmd);
	}
   HRESULT InitializeSecurity() throw()
   {
      // TODO : Call CoInitializeSecurity and provide the
      // appropriate security settings for your service.
      // Suggested - PKT Level Authentication, 
      // Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
      // and an appropiate Non NULL Security Descriptor.
      //CoInitializeSecurity(NULL, 
      return S_OK;
   }
}
;
