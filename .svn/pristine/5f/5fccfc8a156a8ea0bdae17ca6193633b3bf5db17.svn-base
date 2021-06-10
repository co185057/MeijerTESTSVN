// TrainingMode.h: interface for the CSCOTTrainingMode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(SCOT_TRAININGMODE_H)
#define SCOT_TRAININGMODE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DllDefine.h"

typedef enum
{
	SCOTTM_OFF,
	SCOTTM_ON,
	SCOTTM_NOTALLOWED
} TRAININGMODE_STATE;

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
 CSCOTTrainingMode  
{
public:
	// member functions
	long TurnOff();
	long TurnOn();
	bool IsTMOn();
	bool IsAllowed();
	bool IsTransactionApproved();
	void SetTransactionApproved(bool bApproved);
	CSCOTTrainingMode();
	virtual ~CSCOTTrainingMode();

	// data members

private:
	// member functions
	TRAININGMODE_STATE GetCurrentTMState();
	void			   SetTMState(TRAININGMODE_STATE nTrainingState);

	// data members
	TRAININGMODE_STATE m_nTMCurrentState;
	bool m_bTransApproved;
};

#endif // !defined(SCOT_TRAININGMODE_H
