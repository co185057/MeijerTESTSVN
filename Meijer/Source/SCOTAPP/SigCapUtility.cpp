//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SigCapUtility.CPP
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SigCapUtility.h"

#define COMP_ID ID_SM               // base state component
#define T_ID     _T("CSigCapUtility")



CSigCapUtility::CSigCapUtility()
{
}


CSigCapUtility::~CSigCapUtility()
{
}

int CSigCapUtility::FindNumPoints(const BSTR bstrStrokeData, int nLength)
{
	  const CO_ORD_DATA*   pPoints = (const CO_ORD_DATA*)bstrStrokeData;
	  int nIndex;

	  for ( nIndex = 0; nIndex < nLength; nIndex++ )
	  {
		if ( pPoints[ nIndex ].Y_VAL == -1
		   && pPoints[ nIndex ].X_VAL == -1 )
		   break;
	  }
	  return nIndex; 
}