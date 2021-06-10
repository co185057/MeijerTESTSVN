// SolutionCurrentSigCap.h: interface for the CSolutionCurrentSigCap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SolutionCurrentSigCap_H__INCLUDED_)
#define AFX_SolutionCurrentSigCap_H__INCLUDED_

#include "BaseCurrentSigCap.h"
#include "transbroker.h"

class CSolutionCurrentSigCap : public CBaseCurrentSigCap
{ 
public:
    virtual ~CSolutionCurrentSigCap();

	virtual HFILE GetSlipData(void);
	virtual TBRC ReleaseSlipData(HFILE hSlipData);
    virtual void AddStoreReceiptFile( CFile* pFile );
	virtual void GetSlipBitmap( BSTR* pData);
    virtual TBRC PreprocessSignatureData(LPCTSTR lpFileName,BSTR bstrSignatureData, LPCTSTR lpReceiptType, BOOL* bProcessData);


protected:
    CObList m_lstSlips;
	CObList m_lstNonSig;
    CSolutionCurrentSigCap();

private:
    CSolutionCurrentSigCap(CSolutionCurrentSigCap &);                    // hide copy const
    CSolutionCurrentSigCap & operator =(const CSolutionCurrentSigCap &); // hide assignment
};

#endif 
