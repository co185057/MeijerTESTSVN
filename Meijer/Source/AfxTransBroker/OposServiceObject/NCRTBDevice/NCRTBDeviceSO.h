#if !defined(AFX_NCRTBDEVICESO_H__2B5E45C1_37E8_11D5_9C83_0090276FD28D__INCLUDED_)
#define AFX_NCRTBDEVICESO_H__2B5E45C1_37E8_11D5_9C83_0090276FD28D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NCRTBDeviceSO.h : header file
//

/////////////////////////////////////////////////////////////////////////////
//  Extension of some OLE macros
/////////////////////////////////////////////////////////////////////////////

#define MY_DECLARE_OLECREATE(class_name) \
		   DECLARE_OLECREATE(class_name) \
		public: \
			static AFX_DATA const TCHAR szProgID[]; \
			static const TCHAR * GetExternalName() \
				{ return class_name::szProgID; } \
			static REFCLSID GetClassID() \
				{ return class_name::guid; } \

#define MY_IMPLEMENT_OLECREATE(class_name, external_name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
		   IMPLEMENT_OLECREATE(class_name, external_name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
			const AFX_DATADEF TCHAR class_name::szProgID[] = _T(external_name); \


/////////////////////////////////////////////////////////////////////////////
// CNCRTBDeviceSO command target

class CNCRTBDeviceSO : public CCmdTarget
{
	DECLARE_DYNCREATE(CNCRTBDeviceSO)

	CNCRTBDeviceSO();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNCRTBDeviceSO)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CNCRTBDeviceSO();

	// Generated message map functions
	//{{AFX_MSG(CNCRTBDeviceSO)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	MY_DECLARE_OLECREATE(CNCRTBDeviceSO)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CNCRTBDeviceSO)
	afx_msg long COFreezeEvents(BOOL bFreeze);
	afx_msg long CheckHealth(long lLevel);
	afx_msg long Claim(long lTimeout);
	afx_msg long Close();
	afx_msg long DirectIO(long lCommand, long FAR* pData, BSTR FAR* pString);
	afx_msg long GetPropertyNumber(long lPropIndex);
	afx_msg BSTR GetPropertyString(long lPropIndex);
	afx_msg long OpenService(LPCTSTR sClass, LPCTSTR sName, LPDISPATCH pDispatch);
	afx_msg long Release();
	afx_msg void SetPropertyNumber(long lPropIndex, long lProperty);
	afx_msg void SetPropertyString(long lPropIndex, LPCTSTR sProperty);
	afx_msg long ClearOutput();
	afx_msg long ClearInput();
	//line display
	afx_msg long ClearDescriptors();
	afx_msg long ClearText();
	afx_msg long CreateWindow(long lViewportRow, long lViewportColumn, long lViewportHeight, long lViewportWidth, long lWindowHeight, long lWindowWidth);
	afx_msg long DestroyWindow();
	afx_msg long DisplayText(LPCTSTR sData, long lAttribute);
	afx_msg long DisplayTextAt(long lRow, long lColumn, LPCTSTR sData, long lAttribute);
	afx_msg long ScrollText(long lDirection, long lUnits);
	afx_msg long SetDescriptor(long lDescriptor, long lAttribute);
	afx_msg long RefreshWindow(long lWindow);
	//cash drawer
	afx_msg long OpenDrawer();
	afx_msg long WaitForDrawerClose(long BeepTimeout, long BeepFrequency, long BeepDuration, long BeepDelay);
	//POS Printer or MICR
    afx_msg long BeginInsertion(long Timeout);
    afx_msg long BeginRemoval(long Timeout);
    afx_msg long EndInsertion();
    afx_msg long EndRemoval();
	//POS Printer
    afx_msg long CutPaper(long Percentage);
    afx_msg long PrintBarCode(long Station, LPCTSTR Data, long Symbology, long Height, long Width, long Alignment, long TextPosition);
    afx_msg long PrintBitmap(long Station, LPCTSTR FileName, long Width, long Alignment);
    afx_msg long PrintImmediate(long Station, LPCTSTR Data);
    afx_msg long PrintNormal(long Station, LPCTSTR Data);
    afx_msg long PrintTwoNormal(long Stations, LPCTSTR Data1, LPCTSTR Data2);
    afx_msg long RotatePrint(long Station, long Rotation);
    afx_msg long SetBitmap(long BitmapNumber, long Station, LPCTSTR FileName, long Width, long Alignment);
    afx_msg long SetLogo(long Location, LPCTSTR Data);
	afx_msg long TransactionPrint(long Station, long Control);
	afx_msg long ValidateData(long Station, LPCTSTR Data);
	//Scale
	afx_msg long ReadWeight(long* pWeightData, long Timeout);
    // Fiscal Printer
    afx_msg long GetErrorLevel();
	afx_msg void SetErrorLevel(long nNewValue);
	afx_msg long GetErrorOutID();
	afx_msg void SetErrorOutID(long nNewValue);
	afx_msg long GetErrorState();
	afx_msg void SetErrorState(long nNewValue);
	afx_msg BSTR GetErrorString();
	afx_msg void SetErrorString(LPCTSTR lpszNewValue);
	afx_msg BOOL GetFlagWhenIdle();
	afx_msg void SetFlagWhenIdle(BOOL bNewValue);
	afx_msg long GetErrorStation();
	afx_msg void SetErrorStation(long nNewValue);
	afx_msg long BeginFiscalDocument(long DocumentAmount);
	//afx_msg long BeginInsertion(long Timeout);
	afx_msg long BeginFiscalReceipt(BOOL PrintHeader);
	afx_msg long BeginFixedOutput(long Station, long DocumentType);
	afx_msg long BeginItemList(long VatID);
	afx_msg long BeginNonFiscal();
	afx_msg long BeginTraining();
	afx_msg long ClearError();
	//afx_msg long BeginRemoval(long Timeout);
	afx_msg long EndFiscalDocument();
	afx_msg long EndFiscalReceipt(BOOL PrintHeader);
	afx_msg long EndFixedOutput();
	//afx_msg long EndInsertion();
	afx_msg long EndItemList();
	afx_msg long EndNonFiscal();
	//afx_msg long EndRemoval();
	afx_msg long EndTraining();
	afx_msg long GetData(long DataItem, long FAR* OptArgs, BSTR FAR* Data);
	afx_msg long GetDate(BSTR FAR* Date);
	afx_msg long GetTotalizer(long VatID, long OptArgs, BSTR FAR* Data);
	afx_msg long GetVatEntry(long VatID, long OptArgs, long FAR* VatRate);
	afx_msg long PrintDuplicateReceipt();
	//afx_msg long PrintNormal(long Station, LPCTSTR Data);
	afx_msg long PrintFiscalDocumentLine(LPCTSTR DocumentLine);
	afx_msg long PrintFixedOutput(long DocumentType, long LineNumber, LPCTSTR Data);
	afx_msg long PrintPowerLossReport();
	afx_msg long PrintRecItem(LPCTSTR Description, CURRENCY Price, long Quantity, long VatInfo, CURRENCY UnitPrice, LPCTSTR UnitName);
	afx_msg long PrintPeriodicTotalsReport(LPCTSTR Date1, LPCTSTR Date2);
	afx_msg long PrintRecItemAdjustment(long AdjustmentType, LPCTSTR Description, CURRENCY Amount, long VatInfo);
	afx_msg long PrintRecMessage(LPCTSTR Message);
	afx_msg long PrintRecNotPaid(LPCTSTR Description, CURRENCY Amount);
	afx_msg long PrintRecSubtotal(CURRENCY Amount);
	afx_msg long PrintRecRefund(LPCTSTR Description, CURRENCY Amount, long VatInfo);
	afx_msg long PrintRecTotal(CURRENCY Total, CURRENCY Payment, LPCTSTR Description);
	afx_msg long PrintRecSubtotalAdjustment(long AdjustmentType, LPCTSTR Description, CURRENCY Amount);
	afx_msg long PrintRecVoid(LPCTSTR Description);
	afx_msg long PrintRecVoidItem(LPCTSTR Description, CURRENCY Amount, long Quantity, long AdjustmentType, CURRENCY Adjustment, long VatInfo);
	afx_msg long PrintReport(long ReportType, LPCTSTR StartNum, LPCTSTR EndNum);
	afx_msg long PrintXReport();
	afx_msg long ResetPrinter();
	afx_msg long SetDate(LPCTSTR Date);
	afx_msg long PrintZReport();
	afx_msg long SetStoreFiscalID(LPCTSTR ID);
	afx_msg long SetTrailerLine(long LineNumber, LPCTSTR Text, BOOL DoubleWidth);
	afx_msg long SetVatTable();
	afx_msg long SetHeaderLine(long LineNumber, LPCTSTR Text, BOOL DoubleWidth);
	afx_msg long SetPOSID(LPCTSTR POSID, LPCTSTR CashierID);
	afx_msg long SetVatValue(long VatID, LPCTSTR VatValue);
	afx_msg long VerifyItem(LPCTSTR ItemName, long VatID);
	afx_msg BOOL CheckEntry(short nOptions, BSTR FAR* pnStn);
	afx_msg void ClaimDevice(long TimeOut);
	afx_msg void ReleaseDevice();
    //KeyLock
	afx_msg long WaitForKeylockChange(long KeyPosition, long Timeout);
    //ToneIndicator
    afx_msg long Sound(long NumberOfCycles, long InterSoundWait);
    afx_msg long SoundImmediate();

	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

private:
      TOposService<class CTBDevice, class CTBService> m_oService;
	  BOOL m_bReallyClaimed;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NCRTBDEVICESO_H__2B5E45C1_37E8_11D5_9C83_0090276FD28D__INCLUDED_)
