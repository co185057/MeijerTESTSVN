// ScotState.cpp: implementation of the CScotState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dftrans.h"
#include "ScotState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScotState::CScotState()
{

}

CScotState::~CScotState()
{

}

COleDateTime& CScotState::GetTime()
{
  return m_Time;
}

LPCTSTR CScotState::GetTimeString()
{
  return NULL;
}

DWORD CScotState::GetStateID()
{
  return NULL;

}

LPCTSTR CScotState::GetStateDesc()
{
  return NULL;

}

int CScotState::GetType()
{
  return NULL;

}

LPCTSTR CScotState::GetTypeDesc()
{
  return NULL;

}

LPCTSTR CScotState::GetTerminal()
{
  return NULL;

}

int CScotState::GetTransNum()
{
  return NULL;

}

int CScotState::GetStoreNum()
{
  return NULL;

}

BOOL CScotState::IsTimeInUTC()
{
  return FALSE;
}
