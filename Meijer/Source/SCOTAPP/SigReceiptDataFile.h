/*
-------------------------------------------------------------------------------

 File Name       : SigReceiptDataFile.h   
 Description     : Declaration file for TB class : CSigReceiptDataFile
 

 Copyright (C) NCR Corporation 1999

-------------------------------------------------------------------------------
 Date    | Changes Made                                     | Version | By
-------------------------------------------------------------------------------
 12/10/99  Original version                                   01.00.00  Yafu
-------------------------------------------------------------------------------
*/

#ifndef _SIGRECEIPTDATAFILE_H_
#define _SIGRECEIPTDATAFILE_H_

#define DEGITIZER_WIDTH			2317	// in pixel
#define DEGITIZER_HEIGHT		1738	// in pixel
#define WINDOW_WIDTH			  400		// in pixel
#define WINDOW_HEIGHT			  300		// in pixel

//Pad WINDOW_WIDTH to 4-byte boundary		
#define bitmap_row_bytes(w)		(((WINDOW_WIDTH+31)/32)*4)				

typedef struct
{
	CString	szTransNum;				// Unique identifier of a transaction		
	long	lLaneNum;				// SCOT Lane Num 
	long	lTransTotal;			// Total amount of a transaction			   
	CString	szCreditCardNum;		// Credit Card Number. It might be partial number   
	CString	szApprovalCd;			// Credit Card Approval Code		  
	CString	szCreditCardOwnerName;	// Textual description  of the error		 
	CTime   SigCapTimeStamp;		// The timestamp where a electronic signature is commited 
} SigCapTransData;

typedef enum
{
  SIGCAP_ERROR_TOKEN = 0,
  SIGCAP_SLIP_TOKEN = 1,
  SIGCAP_SIG_TOKEN =2,
  SIGCAP_FILE_END_TOKEN = 3,
  SIGCAP_RECEIPT_TYPE
}  SIGCAP_TOKEN_TYPE ;


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
CSigReceiptDataFile : public CObject 
{
public:

	CSigReceiptDataFile();							
	virtual ~CSigReceiptDataFile();						
	CSigReceiptDataFile(LPCTSTR szFileName);
	
	HANDLE	OpenFile(bool bWrite);
	void	  CloseFile();

	void	SaveSigCapTransData(const SigCapTransData& pTransData)  ;
	void	SaveSignatureData(BSTR pSigData) ;

    void    SaveSigCapSlipData(HFILE hFile, BSTR pSignatureData);
	bool	PrintSigReceiptFile() ;
	bool	FileAvailable() ;
    void    ProcessSlipData(CString& csSlipData, BSTR pSignatureData,LPCTSTR lpReceiptType);
    int     GetSigTokenStartPos(const CString& csSlipData) const; //tar 239361

    long    GetSigWidth() const {return m_lSigWidth;};
    long    GetSigHeight() const {return m_lSigHeight;};

    void    SetSigWidth(const long lValue);
    void    SetSigHeight(const long lValue);


private:
	long AsciiToHex(CString csInString, BYTE *&btOutString);
	HANDLE		    m_hSReceiptFile;
	bool			    m_bWrite ;
	CString			  m_csFileName;
	CString		  m_csReceiptFilePath;
	CString	      m_csTempBitmapFile;
  SIGCAP_TOKEN_TYPE m_ePreviousToken ;
  SIGCAP_TOKEN_TYPE m_ePrePreviousToken ;

  CString       m_csSlipData ;

  long          m_lSigWidth;
  long          m_lSigHeight;
	
protected:	
	// methods
	long ConvertSig5991toPoints(BSTR pRawData, BYTE* pPoints) ; 
	long CalculatePointArraySize(BSTR pRawData);
	bool ConvertSigPointsToBitmap(BYTE* pPoints, long lPointCount, LPCTSTR BitmapFileName, int nWindowWidth, int nWindowHeight) ;
	void ConnectTwoPoints(long x, long y, long x1, long y1) ;
	void PlotPointInBitmapData(long x, long y) ;
//TAR 195179 & 191490 start
public:
	BOOL EnterSignaturesFromFile(CString csFileName);
	void BackupSigReceiptFile() ;
    bool ConvertSig5991toBitmap(BSTR pRawData, LPCTSTR csBitmapFileName, int nWindowWidth, int nWindowHeight) ;
protected:
//TAR 195179 & 191490 end
  void SaveBadSigFile() ;

	// help functions 
	void SaveString(const CString& pStr) ;
	void SaveLong(long lData) ;
	void SaveTimeStamp(const CTime& tmTime) ;
	bool ReadString   (CString& Str);
	bool ReadLong     (long& lData);
	bool ReadTimeStamp(CTime& tmTime);
  bool SaveToken(const CString& strToken) ;
  bool ReadToken(CString& strToken) ;
  SIGCAP_TOKEN_TYPE ParseToken(const CString& strToken) ;

	BOOL PrintTransData() ;
    BOOL PrintSlipData();
	void PrintSignatureData(LPCTSTR lpReceiptType) ;
	CString FormatCurrency(long lTotal) ;

};

#endif // _SIGRECEIPTDATAFILE_H_

