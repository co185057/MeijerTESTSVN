/////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  DPSCompatibility.H
//
//
// This module contains all the PSProceduresBase class APIs that are obsolete with new
// UI implementation in the 3.1.
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DPSCOMPATIBILITY
#define _DPSCOMPATIBILITY


inline CString PSProceduresBase::GetMascotBmpName(const int nMsg) 
{ 
    trace(L6,_T("+/-GetMascotBmpName() - This function is obsolete."));
    return "";
}

inline int PSProceduresBase::FormatMessageToReceipt(CString &text, bool bIsSmallRcpt, int nLines)
{
    trace(L6,_T("+/-FormatMessageToReceipt() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::AddMessageToReceipt2(CString text, const bool hilite)
{
    trace(L6,_T("+/-AddMessageToReceipt2() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::PromptAmount(long lAmt)// display amount in prompt area
{
     trace(L6,_T("+/-PromptAmount() - This function is obsolete."));
     return 0;
}

inline long PSProceduresBase::HideEcho()  // hide echo  
{
    trace(L6,_T("+/-HideEcho() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::SecurityMessageModal(LPCTSTR szText)// display text in message 12 area
{
    trace(L6,_T("+/-SecurityMessageModal() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::SecurityMessageNonmodal(LPCTSTR szText)// display text in message 12 area
{
    trace(L6,_T("+/-SecurityMessageNonmodal() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::Message13(PSTEXT nText)
{
    trace(L6,_T("+/-Message13() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::Message13(LPCTSTR szText)
{
    trace(L6,_T("+/-Message13() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::Message23(const PSTEXT nText, bool bIsModal)
{
    trace(L6,_T("+/-Message23() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::Message23(LPCTSTR szText, bool bIsModal)
{
    trace(L6,_T("+/-Message23() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::Message24(const PSTEXT nText, bool bIsModal)
{
    trace(L6,_T("+/-Message24() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::Message24(LPCTSTR szText, bool bIsModal)
{
    trace(L6,_T("+/-Message24() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::Message2(const PSTEXT nText)
{
    trace(L6,_T("+/-Message2() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::Message2(LPCTSTR szText)
{
    trace(L6,_T("+/-Message2() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::Message3(const PSTEXT nText)
{
    trace(L6,_T("+/-Message3() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::Message3(LPCTSTR szText)
{
    trace(L6,_T("+/-Message3() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::InfoMessage(LPCTSTR szText)
{
    trace(L6,_T("+/-InfoMessage() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::GetTareList(CString &csDesc)// get the curr selection desc
{
    trace(L6,_T("+/-GetTareList() - This function is obsolete."));
    return 0;
}

inline int PSProceduresBase::GetCashList(CString &csDesc)// get the curr selection desc
{
    trace(L6,_T("+/-GetCashList() - This function is obsolete."));
    return 0;
}

inline int PSProceduresBase::GetWideList(CString &csCode)// get the curr selection desc
{
    trace(L6,_T("+/-GetWideList() - This function is obsolete."));
    return 0;
}

inline void PSProceduresBase::PSHideKeyboard()
{
    trace(L6,_T("+/-PSHideKeyboard() - This function is obsolete."));
}

inline void PSProceduresBase::PSHideAlphaNumericKeypad()
{
    trace(L6,_T("+/-PSHideAlphaNumericKeypad() - This function is obsolete."));
}

inline void PSProceduresBase::PSHideNumericKeypad()
{
    trace(L6,_T("+/-PSHideNumericKeypad() - This function is obsolete."));
}

inline void PSProceduresBase::PSHideSMMNumericKeypad()
{
   trace(L6,_T("+/-PSHideSMMNumericKeypad() - This function is obsolete."));
}

inline void PSProceduresBase::PSBackSpaceKbd()
{
    trace(L6,_T("+/-PSBackSpaceKbd() - This function is obsolete."));
    return;
}

inline long PSProceduresBase::HideButtons()
{
    trace(L6,_T("+/-HideButtons() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::HideSalesReceipt() 
{
    trace(L6,_T("+/-HideSalesReceipt() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::DisableButtons(void)
{
    trace(L6,_T("+/-DisableButtons() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::DisableSelectionList(void)
{
    trace(L6,_T("+/-DisableSelectionList() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::HideReceiptButtons(void)
{
    trace(L6,_T("+/-HideReceiptButtons() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::DisableReceiptButtons(void)
{
    trace(L6,_T("+/-DisableReceiptButtons() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::Background(const BMPS nBmp,const int nMode)// change the background
{
    trace(L6,_T("+/-Background() - This function is obsolete."));
    return 0;
}

inline bool PSProceduresBase::SetDynamicBackground(CString *pcsFileName)
{
    trace(L6,_T("+/-SetDynamicBackground() - This function is obsolete."));
    return 0;
}

inline void PSProceduresBase::SetCMMascotImage(const BMPS nBmp)// change the mascot image
{
    trace(L6,_T("+/-SetCMMascotImage() - This function is obsolete."));
}

inline long PSProceduresBase::AddSecurityLog(LPCTSTR szText)// add a text string to the security log list
{
    trace(L6,_T("+/-AddSecurityLog() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::UpdateMediaList(const long index, CString text)
{
    trace(L6,_T("+/-UpdateMediaList() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::ShowWideList(void)
{
    trace(L6,_T("+/-ShowWideList() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::ShowKeyboard(void)
{
    trace(L6,_T("+/-ShowKeyboard() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::ShowAlphaNumericKeyboard(void)
{
    trace(L6,_T("+/-ShowAlphaNumericKeyboard() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::ShowNumericKeypad(void)
{
    trace(L6,_T("+/-ShowNumericKeypad() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::ShowSMMNumericKeypad(const bool hideDecimal)
{
    trace(L6,_T("+/-ShowSMMNumericKeypad() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::ShowTareList(void)
{
    trace(L6,_T("+/-ShowTareList() - This function is obsolete."));
    return 0;
}

inline void PSProceduresBase::SetBackground(const BMPS nBmp)// just the BMP number
{
   trace(L6,_T("+/-SetBackground() - This function is obsolete."));
}
inline void PSProceduresBase::HideCMWindows() 
{
    trace(L6,_T("+/-HideCMWindows() - This function is obsolete."));
}

inline void PSProceduresBase::HideCMButtons() 
{
    trace(L6,_T("+/-HideCMButtons() - This function is obsolete."));
}

inline long PSProceduresBase::AddToReport(const CString& text, const CString& csReceiptSrc)
{
	//The implementation is moved to SMSmReportsMenuBase
    trace(L6,_T("+/-AddToReport() - This function is obsolete."));
	return 0;
}
inline long PSProceduresBase::AddRewardItem2(CString& csDesc, long lRewardID, long lValue, long lEmphasis)
{
    trace(L6,_T("+/-AddRewardItem2() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::InsertRewardItem(long nIndex, CString& csDesc, long RewardID, long lValue, long lEmphasis)
{
    trace(L6,_T("+/-InsertRewardItem() - This function is obsolete."));
    return 0;
}

inline BOOL PSProceduresBase::ConnectToServer(int iFocus, int iDisplay)
{   
    trace(L6,_T("+/-ConnectToServer() - This function is obsolete."));
    return 0;
}

inline void PSProceduresBase::SetCMGobackButtonVisible(bool visible) 
{
    trace(L6,_T("+/-SetCMGobackButtonVisible() - This function is obsolete."));

}

inline void PSProceduresBase::ShowCMSmallReceipt()
{
    trace(L6,_T("+/-ShowCMSmallReceipt() - This function is obsolete."));
}

inline long PSProceduresBase::HideVideoLarge(void)
{
    trace(L6,_T("+/-HideVideoLarge() - This function is obsolete."));
    return 0;
}  

inline long PSProceduresBase::HideVideoSmall(void)
{
    trace(L6,_T("+/-HideVideoSmall() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::HideVideoProduce(void)
{
    trace(L6,_T("+/-HideVideoProduce() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::ShowVideoLarge(void)
{
    trace(L6,_T("+/-ShowVideoLarge() - This function is obsolete."));
    return 0;
}  

inline long PSProceduresBase::ShowVideoSmall(void)
{
    trace(L6,_T("+/-ShowVideoSmall() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::ShowVideoProduce(void)
{
    trace(L6,_T("+/-ShowVideoProduce() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::SMShowItemReceipt(const bool fSetScrolling)
{
    trace(L6,_T("+/-SMShowItemReceipt() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::SMShowRestrictedItemReceipt(const bool fSetScrolling)
{
    trace(L6,_T("+/-SMShowRestrictedItemReceipt() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::SMShowVoidedItemReceipt(const bool fSetScrolling)
{
    trace(L6,_T("+/-SMShowVoidedItemReceipt() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::SMShowCouponReceipt(const bool fSetScrolling)
{
    trace(L6,_T("+/-SMShowCouponReceipt() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::SMShowVisualItemReceipt(const bool fSetScrolling)
{
    trace(L6,_T("+/-SMShowVisualItemReceipt() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::SMShowSecurityLog(void)
{
    trace(L6,_T("+/-SMShowSecurityLog() - This function is obsolete."));
    return 0;
}

inline long PSProceduresBase::SMShowMediaList(void)
{
    trace(L6,_T("+/-SMShowMediaList() - This function is obsolete."));
    return 0;
}
#endif