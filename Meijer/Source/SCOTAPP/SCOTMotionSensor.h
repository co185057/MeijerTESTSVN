// SCOTMotionSensor.h: interface for the CSCOTMotionSensor class.
//
// Author:  Leila Parker
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOTMotionSensor_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)
#define AFX_SCOTMotionSensor_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EASMotionSensor.h"	

class CSCOTMotionSensor : public CSCOTDevice
{
public:
	CSCOTMotionSensor();
	virtual ~CSCOTMotionSensor();

	virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID) = 0;
	virtual long UnInitialize() = 0;
	virtual long Disable() = 0;
	virtual long Enable() = 0;
	virtual void DataEvent(long Status) {};
	virtual void ErrorEvent(LONG	ResultCode, 
							LONG	ResultCodeExtended, 
							LONG	ErrorLocus, 
							LONG*	pErrorResponse) {};
	virtual void Claim(bool) = 0;
	virtual void SetDataEventEnabled(bool) = 0;
};

class CSCOTOnTrak : public CSCOTMotionSensor
{
public:
	CSCOTOnTrak();
	virtual ~CSCOTOnTrak();

	virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID);
	virtual long UnInitialize();
	virtual long Disable();
	virtual long Enable();
	virtual void DataEvent(long Status);
	virtual void ErrorEvent(LONG	ResultCode, 
							LONG	ResultCodeExtended, 
							LONG	ErrorLocus, 
							LONG*	pErrorResponse);
	virtual void Claim(bool);
	virtual void SetDataEventEnabled(bool);

protected:
	CMotionSensor m_EASMotionSensor;
};

class CSCOTEmulatorMotionSensor : public CSCOTOnTrak
{
public:
  CSCOTEmulatorMotionSensor() {};
  virtual ~CSCOTEmulatorMotionSensor() {};
};
#endif // !defined(AFX_SCOTMotionSensor_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)