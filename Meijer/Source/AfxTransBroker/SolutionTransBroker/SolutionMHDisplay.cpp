//////////////////////////////////////////////////////////////////////
//
// SolutionMHDisplay.cpp: implementation of the CSolutionMHDisplay class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SolutionMHDisplay.h"
#include "TraceSystem.h"
#include "CustomerCurrentDisplay.h"

#define T_ID    _T("CSolutionMHDisplay")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSolutionMHDisplay::CSolutionMHDisplay()
{
}

CSolutionMHDisplay::~CSolutionMHDisplay()
{
}

TBSTATE CSolutionMHDisplay::ProcessMessage(CTBMsgObj *pMsgObj)
{
    tbtraceIO(_T("ProcessMessage"));

	// Set the current XML display info
	CTBGenMsg *pGenMsg= dynamic_cast<CTBGenMsg*> (pMsgObj);
	CCustomerCurrentDisplay::instance()->setXML(pGenMsg->getXML());

	//we shouldn't really parse on the display. TB parses on the POSState
	//so, just ignore the actual message unless something special is needed.
	return TB_IGNORE;
}
