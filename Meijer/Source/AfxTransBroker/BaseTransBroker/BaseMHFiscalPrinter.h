//////////////////////////////////////////////////////////////////////////
///
/// \file BaseMHFiscalPrinter.h
/// \brief interface for the CBaseMHFiscalPrinter class.
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////////
#if !defined(AFX_BASEMHFISCALPRINTER_H__563EDCC9_0A3F_4A01_935E_74656F4B0311__INCLUDED_)
#define AFX_BASEMHFISCALPRINTER_H__563EDCC9_0A3F_4A01_935E_74656F4B0311__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MsgHandler.h"
#include "FiscalPrinterMsg.h"

//////////////////////////////////////////////////////////////////////
///
/// CBaseMHFiscalPrinter is an implementaion of CBaseMHFiscalPrinter
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////
class CBaseMHFiscalPrinter : public CMsgHandler  
{
public:
		//////////////////////////////////////////////////////////////////////
		///
		/// Destructor
		///
		//////////////////////////////////////////////////////////////////////
		virtual ~CBaseMHFiscalPrinter();

    // This is required functionality for all MH classes

		//////////////////////////////////////////////////////////////////////
		///
		/// ProcessMessage, Process the message posted by TB. TB analyzes the message 
		///	       and returns the state of the host application, based on the presentation 
		///        data in the message. 
		///
		///	@param CTBMsgObj *pMsg - Pointer to the TB message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE ProcessMessage(CTBMsgObj *pMsgObj);

protected:
		//////////////////////////////////////////////////////////////////////
		///
		/// Constructor
		///
		//////////////////////////////////////////////////////////////////////
		CBaseMHFiscalPrinter();

    // Message Handlers

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleDirectIO, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandleDirectIO( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleBeginFiscalReceipt, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandleBeginFiscalReceipt( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleEndFiscalReceipt, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandleEndFiscalReceipt( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandlePrintRecItem, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandlePrintRecItem( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandlePrintRecTotal, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandlePrintRecTotal( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleBeginNonFiscal, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandleBeginNonFiscal( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleEndNonFiscal, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandleEndNonFiscal( CFiscalPrinterMsg* pMsg);

		//////////////////////////////////////////////////////////////////////
		///
		/// HandlePrintNormal, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandlePrintNormal( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandlePrintRecItemAdjustment, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandlePrintRecItemAdjustment( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandlePrintRecMessage, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandlePrintRecMessage( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandlePrintRecSubtotal, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandlePrintRecSubtotal( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandlePrintRecSubtotalAdjustment, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandlePrintRecSubtotalAdjustment( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandlePrintRecVoid, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandlePrintRecVoid( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandlePrintRecVoidItem, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandlePrintRecVoidItem( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandlePrintZReport, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandlePrintZReport( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleBeginTraining, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandleBeginTraining( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleEndTraining, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandleEndTraining( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleGetData, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandleGetData( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleGetDate, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandleGetDate( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandlePrintRecNotPaid, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandlePrintRecNotPaid( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandlePrintRecRefund, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandlePrintRecRefund( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleResetPrinter, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandleResetPrinter( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleSetDate, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandleSetDate( CFiscalPrinterMsg* pMsg );

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleSetHeaderLine, The function code handlers - These functions need to be overridden in the
		///       solution-specific level for appropriate handling.
		///
		///	@param CFiscalPrinterMsg* pMsg- the Fiscal printer message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE HandleSetHeaderLine( CFiscalPrinterMsg* pMsg );

    // Response Functions

		//////////////////////////////////////////////////////////////////////
		///
		/// SendOutputCompleteResp, ### ADD API DOC ###
		///
		//////////////////////////////////////////////////////////////////////
    virtual void SendOutputCompleteResp();

		//////////////////////////////////////////////////////////////////////
		///
		/// SendDirectIOResp, ### ADD API DOC ###
		///
		///	@param long command
		///	@param CString response
		//////////////////////////////////////////////////////////////////////
    virtual void SendDirectIOResp(long Command, const CString &Response);

		//////////////////////////////////////////////////////////////////////
		///
		/// SendGetDataResp, ### ADD API DOC ###
		///
		///	@param long DataItem
		///	@param CString return data
		//////////////////////////////////////////////////////////////////////
    virtual void SendGetDataResp(long DataItem, const CString ReturnData);

		//////////////////////////////////////////////////////////////////////
		///
		/// SendGetDateResp, ### ADD API DOC ###
		///
		///	@param CString return date
		//////////////////////////////////////////////////////////////////////
    virtual void SendGetDateResp(const CString ReturnDate );

		
private:
		//////////////////////////////////////////////////////////////////////
		///
		/// Hide copy const
		///
		//////////////////////////////////////////////////////////////////////
    CBaseMHFiscalPrinter(const CBaseMHFiscalPrinter &);                    // hide copy const

		//////////////////////////////////////////////////////////////////////
		///
		/// Hide assignment
		///
		//////////////////////////////////////////////////////////////////////
    CBaseMHFiscalPrinter & operator =(const CBaseMHFiscalPrinter &); // hide assignment

};

#endif // !defined(AFX_BASEMHFISCALPRINTER_H__563EDCC9_0A3F_4A01_935E_74656F4B0311__INCLUDED_)
