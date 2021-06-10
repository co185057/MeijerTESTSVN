// SCOTEASSensor.h: interface for the CSCOTEASDeactivator class
//
// Author:  Leila Parker
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOTEASSensor_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)
#define AFX_SCOTEASSensor_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OposEASDeactivator.h"	

class CSCOTEASDeactivator : public CSCOTDevice
{
public:
	CSCOTEASDeactivator();
	virtual ~CSCOTEASDeactivator();

	virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID) = 0;
	virtual long UnInitialize() = 0;
	virtual long Disable() = 0;
	virtual long Enable() = 0;
	virtual void DataEvent(long Status) {};
	virtual void ErrorEvent(LONG	ResultCode, 
							LONG	ResultCodeExtended, 
							LONG	ErrorLocus, 
							LONG*	pErrorResponse) {};

	virtual void SetDataEventEnabled(long) = 0;

};

class CSCOTSensormatic : public CSCOTEASDeactivator
{
public:
	CSCOTSensormatic();
	virtual ~CSCOTSensormatic();

	virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID);
	virtual long UnInitialize();
	virtual long Disable();
	virtual long Enable();
	virtual void DataEvent(long Status);
	virtual void ErrorEvent(LONG	ResultCode, 
							LONG	ResultCodeExtended, 
							LONG	ErrorLocus, 
							LONG*	pErrorResponse);

	virtual void SetDataEventEnabled(long);

protected:
	EASDeactivator m_EASDeactivator;
};

class CSCOTEmulatorEAS : public CSCOTSensormatic
{
public:
  CSCOTEmulatorEAS() {};
  virtual ~CSCOTEmulatorEAS() {};
};
#endif // !defined(SCOTEASSensor_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)