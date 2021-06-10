//////////////////////////////////////////////////////////////////////////////////////////////////
//  This module was added for TTW620 to handle Generic Restrictions
// 
#ifndef _SMUSSFMANAGER
#define _SMUSSFMANAGER

#include "SMUSSFManagerBase.h"
#include "PSProcedures.h"
//#include <vector>
//using namespace std;

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMUSSFManager : public SMUSSFManagerBase // kind of CustomerMode TTW959
{
public:
  SMUSSFManager();
  virtual SMStateBase *Initialize(void);
	
  DECLARE_DYNCREATE(SMUSSFManager)  // MFC Runtime class/object information

protected:

  virtual SMStateBase *TBIdleTimer(void);
  virtual SMStateBase *MakeNextMove(CString thisContext = _T(""));
  virtual SMStateBase *PureMethod(void){return STATE_NULL;}
  virtual SMStateBase *PerformAction(CString thisAction, bool &bActionTaken);


};

#endif