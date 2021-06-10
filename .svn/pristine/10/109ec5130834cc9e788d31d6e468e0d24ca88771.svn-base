// APipeSink.h: interface for the APipeSink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APIPESINK_H__C610AE37_08EB_4C21_B88A_C39A1A61DF68__INCLUDED_)
#define AFX_APIPESINK_H__C610AE37_08EB_4C21_B88A_C39A1A61DF68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class APipeSink  
{
public:
	virtual void ProcessRawHostData( DWORD dwPipeID, void* pData, DWORD dwByteLength ) = 0;
	virtual void PipeFailure( DWORD dwPipeID ) = 0;
};

#endif // !defined(AFX_APIPESINK_H__C610AE37_08EB_4C21_B88A_C39A1A61DF68__INCLUDED_)
