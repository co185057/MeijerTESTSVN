//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecBagAreaBackup.h
//
// TITLE: Class header for SecBagAreaBackup
//
//
// AUTHOR:    Thanhhieu Pham
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECBAGAREABACKUPSTATE
#define _SECBAGAREABACKUPSTATE

#include "SMSecBagAreaBackupBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecBagAreaBackup : public SMSecBagAreaBackupBase
{
public:
	SMSecBagAreaBackup();
	
  DECLARE_DYNCREATE(SMSecBagAreaBackup)// MFC Runtime class/object information
};

#endif
