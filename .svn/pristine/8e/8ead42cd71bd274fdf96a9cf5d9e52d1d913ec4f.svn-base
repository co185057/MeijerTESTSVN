// SolutionCurrentReceipt.h: interface for the CSolutionCurrentReceipt class.
//
//////////////////////////////////////////////////////////////////////
#include "BaseTBDefs.h"
#include "BaseCurrentReceipt.h"

#if !defined(AFX_SolutionCurrentReceipt_H__INCLUDED_)
#define AFX_SolutionCurrentReceipt_H__INCLUDED_

class CSolutionCurrentReceipt : CBaseCurrentReceipt
{
public:
    virtual ~CSolutionCurrentReceipt();

    // Support FastLane calls
    virtual HFILE GetReceiptData(void);
    virtual TBRC ReleaseReceiptData(HFILE hReceiptData);

    // Internal calls
    virtual bool FinishReceipt( bool bDiscard = false );
    virtual bool WriteToReceipt( CString csReceiptData );
    virtual bool IsPrinting();
    virtual bool IsInProgress();
    virtual bool PurgeAllReceipts( bool bRemoveFiles );
    virtual int GetReceiptFileCount();
    virtual void OverrideInProgress( bool bVal );
    virtual void SetPrintWhenFinished( bool bVal );
	virtual void	PrintAllSavedReceipts();
	virtual void SetIsPrinting( bool bVal );
	virtual CString PerserveBitmap( CString csFile );
	virtual void CheckForStoreReceipt( CString csData );
	virtual void ResetStoreRecMatch( );
	virtual void AddStoreRecMatch( CString csData );
	
protected:
    CSolutionCurrentReceipt();
    CString m_strFileName;
    CFile* m_pReceiptFile;
	CCriticalSection m_Crit;
	 bool m_bIsStoreReceipt;
	 virtual bool GetPrintWhenFinished( );
	 virtual void CheckReceiptDir();
	 virtual CString GenerateIndex( );
	 long m_nPrintRequest;
	 CString m_csActiveIndex;
	 CObList m_lstReceipts;
	 CString m_csReceiptDir;
	 CStringList m_lstStoreMatch;
	  
    bool m_bIsPrinting;


private:
    virtual CFile* GetNewReceiptFile();
    
    CSolutionCurrentReceipt(CSolutionCurrentReceipt &);                    // hide copy const
    CSolutionCurrentReceipt & operator =(const CSolutionCurrentReceipt &); // hide assignment

    bool m_bPrintWhenFinished;
    bool m_bInProgressOverride;
    
};

#endif 

