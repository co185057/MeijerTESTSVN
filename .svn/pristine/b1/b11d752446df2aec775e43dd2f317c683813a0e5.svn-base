//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  PSInterface.H
//
// TITLE:   Display Presentation Services Interface (Low Level interface to NCR DPS)
// This module handles all DPS calls from PSProcedures (High Level)
//
//  There are a number of classes defined in here.
//
//    PSInterface       is the lowest level class, it serves all frames and
//                      non-list type windows, Load(), Show(), Hide(),
//                      Content() and Button() operations
//
//    PSInterfaceList1  serves all list type windows, it is based on the
//                      PSInterface class and adds Add(),Get(),Reset() and
//                      Select() functions to list operations.  This class
//                      keeps available a description for each entry
//                      Add() adds an entry with a description
//                      Get() gets the selected entry description
//                      GetAt() gets a specified entry description
//                      Select() highlights an entry
//                      UnSelect() un-highlights an entry
//                      Reset() empties the list and releases all memory
//
//    PSInterfaceList2  serves all scrolling list type windows, it is based
//                      on the PSInterfaceList1 class and adds Scroll()
//                      operations and keeps available a code for each entry.
//                      Add() same as Add() plus a code
//                      Get() same as Get() plus a code
//                      GetAt() same as GetAt() plus a code
//                      ScrollUp() moves the highlight up to the next entry
//                        that is not an INFO_ITEM code
//                      ScrollDown() moves the highlight down to the next entry
//                        that is not an INFO_ITEM code
//                      SetScrolling() enables or disables the scrolling buttons
//                        based on the current selection
//    PSInterfaceList3  based on PSInterfaceList2, it adds an array of HBITMAP
//                      for use with produce graphics
//    PSInterfaceErcpt  serves all receipt type windows, it is based on the
//                      PSInterfaceList2 class and keeps available a quantity,
//                      weight, price and tare code for each entry
//                      Add() same as Add() plus a qty,wgt,price,unitprice, tare
//                      Get() same as Get() plus a qty,wgt,price,unitprice, tare
//                      GetAt() same as GetAt() plus a qty,wgt,price,tare
//                      Select() same as Select() plus a flag for strikethru
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef NewUI

#ifndef _PSINTERFACE
#define _PSINTERFACE

// this macro defines a literal for tracing and traces the entry point
#define _WHO(a)               \
	LPCTSTR szWho = #a;            \
trace(WHOLEVEL, #a);

//////////////////////////////////////////////////////////////////////////////////////////////////

long PSInterface_Initialize(void);  // must call this once

long PSInterface_UnInitialize(void); //

//////////////////////////////////////////////////////////////////////////////////////////////////
#include "DllDefine.h"
#include "ScotPSApp.h"
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 

///////////////////////////////////////////////////////////////////////////////////////

PSSimpleInterface : public CObject  // low level proc for Display Presentation Services
{

protected:
  long CheckRC(
    const int,                      // as per error(), _LINE_
    const int,                      // as per error()
    LPCTSTR,                         // common return code checking, return code
    const long,                     // ...return code
    const long,                     // ...element ID
    const LPCTSTR)  const;            // ...error desc
	
  long ShowElement(                 // freq called show element
    const int,                      // ...Idc of object
    const ULONG,                    // ...funcs like PS_SF_ADD | PS_SF_SHOW
    LPCTSTR,                         // ...for szContent or NULL
    const long,                     // ...for lExtraData
    const int,                      // ...for nIndex
    SELEMENT &) ;
	//const;                    // ...structure to fill
	
public:
  LPCTSTR  szWindow;                 // name of the window, like wCMFrame
  HWND    hWnd;                     // handle to the loaded window from PSGetWindowHandle()
  bool    fVisible;                 // flag set if visible
  bool    fLoaded;                  // was loaded, Ex's are not loaded
  LPCTSTR  szBackground;             // name of background BMP file
  bool    bSetContentSuccessful;    // true - successfully loaded the set content
  long    nLocalXtraValue;
  PSSimpleInterface(LPCTSTR);        // constructor with pointer this window's name
  bool  bReceipt;
  virtual bool IsReceipt() { return bReceipt;};
	
	//Create public ShowElement2 to load private ShowElement    Feng2
  long ShowElement2(                // freq called show element
    const int,                      // ...Idc of object
    const ULONG,                    // ...funcs like PS_SF_ADD | PS_SF_SHOW
    LPCTSTR,                         // ...for szContent or NULL
    const long,                     // ...for lExtraData
    const int,                      // ...for nIndex
    SELEMENT &)    ;                 // ...structure to fill
    //const;
	
  long GetHandle(const bool);       // get this window's handle, PSGetWindowHandle(), errors T/F
  long GetControlHandle(const long, HWND&); // get handle of a control in a window
  long Load(const bool);            // explicitly load this window, PShowWindowEx(). errors T/F
  long Show(void);                  // show this window, PSShowWindow(...SHOW)
  long Hide(void);                  // hide this window, PSShowWindow(...HIDE)
  long Content( const long,         // set element ID contents
                LPCTSTR);      // ...text content
	
  long Content(const long,          // set element ID contents
               const PSTEXT); // ...ps text number

  long Content(const long,          // set element ID contents
               LPCTSTR, const bool); // ...ps text number

  void Init(LPCTSTR);                // pointer this window's name  
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
   PSInterface : public PSSimpleInterface
{
private:
  bool  fButton[8];                 // status of buttons
	
public:
  
  PSInterface(LPCTSTR);              // constructor with pointer this window's name
  long Button(
    const long,                     // show a button, button ID
    LPCTSTR,                         // ...button text desc
    const bool);                    // ...true=enable/show or false=disable/hide
	
  long Button(
    const long,                     // show a button, button ID
    const PSTEXT,                   // ...button ps text number
    const bool);                    // ...true=enable/show or false=disable/hide

  long ButtonFlash(
    const long,                     // flash a button
    const bool);                    // ... T=Flash, F=no Flash
	
  long ButtonDisable(const long);   // disable a button, IDC of button
  long GraphicButtonDisable(const long,const long); //IDC, Index
  long GraphicButtonEnable(const long,const long); //IDC, Index
  long GraphicButtonHide(const long,const long); //IDC, Index

  bool ButtonValid(const int n) // takes numbers 1-8
  {
    return fButton[n-1];
  }
  
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
   PSInterfaceList1 : public PSInterface
{
protected:
  long ListIdc;                     // DC id for this list IDC_...
  long lLastSelected;               // selected element if any or zero
  CStringArray csItemDesc;
  
  //Begin_Suspend
  //CUIntArray cuCouponList;         //Coupon List  Suspend
  //End_Suspend

public:
  long  lCountList;                 // count of items added to this list (if it is a list)
  int   m_nTwoLine;
  int   bStrikethru;
  long  haveSelectableSalesItems;
  
  PSInterfaceList1(                 // constructor with name and Idc
    LPCTSTR,                         // ... name of window
    const long);                    // ... Idc of list
    
  long Add(                         // add an item to a list
    LPCTSTR,                         // ... list entry description
    const bool=false);              // ... T/F highlight this entry
	
  long Add(                         // add an item to a list
    const PSTEXT,                   // ...ps text number
    const bool=false);              // ... T/F highlight this entry

  long Update(                      // update an item to a list
    const long,                     // entry index    
    LPCTSTR,                      	// ... text
    const bool=false);              // ... T/F highlight this entry

  long Remove(const long lIndex);   // remove an item to a list
  
  long InsertAt(                         // add an item to a list
	const long lIndex,				// insert at this index
    LPCTSTR,                         // ... list entry description
    const bool=false);              // ... T/F highlight this entry

	
  long Reset(void);                 // reset and empty this receipt or list
	
  long Get(CString &);              // get selected list entry description
	
  long GetAt(                       // get passed entry desription
    const long,                     // ... entry index 1,2,3,4
    CString &);                     // ... returned description

  long SetTopIndex(void);           //set top item index on receipt - tar 182942
	
  long Select(const long);          // highlight this entry, 1,2,3...
	
  long UnSelect(void);              // un-highlights last selection

  void Init(LPCTSTR,                 // pointer this window's name                                               
            const long);            // ... Idc of list
	
  long GetListCount(void);          // return the number of items in the list.
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
   PSInterfaceList2 : public PSInterfaceList1
{
protected:
  CStringArray csItemCode;
	long lLastNonInfo;								// index of last non-info entry 1,2,3...

public:
  PSInterfaceList2(                 // constructor with name and Idc
    LPCTSTR,                         // ... name of window
    const long);                    // ... Idc of list
	
  long Add(                         // add an item to a list
    LPCTSTR,                         // ... item entry description
    LPCTSTR,                         // ... item code that goes with this entry
    const bool=false);              // ... T/F highlight this entry
	
  long Reset(void);                 // reset and empty this receipt or list
	
  long Get(CString &,CString &);    // get selected list entry desc and code
	
  long GetAt(                       // get passed entry desription
    const long,                     // ... entry index 1,2,3,4
    CString &,                      // ... returned description
    CString &);                     // ... returned item code
	
  long ScrollUp(const bool);        // scroll this list window up
	
  long ScrollDown(const bool);      // scroll this list window down

  long ShowMostCurrent(void);       // scroll to the most current page.
  long ShowTopList(void);           // scroll to the top of the list.

  long SetScrolling(void);          // enable/disable scroll buttons on receipt

  void Init(LPCTSTR,                 // pointer this window's name                                               
            const long);            // ... Idc of list
	
};
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
   PSInterfaceGraphicList : public PSInterfaceList2
{
protected:
  CArray<HBITMAP,HBITMAP> caHBmp;

public:
  PSInterfaceGraphicList(                 // constructor with name and Idc
    LPCTSTR,                         // ... name of window
    const long);                    // ... Idc of list
	
  long Add(                         // add an item to a list
    LPCTSTR,                         // ... item entry description
    LPCTSTR,                         // ... item code that goes with this entry
    const bool=false,               // ... T/F highlight this entry
	const HBITMAP=NULL);			// ... bitmap handle				

  long Reset(void);                 // reset and empty this receipt or list	  
};
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
   PSInterfaceErcpt : public PSInterfaceList2
{
protected:
  CDWordArray lQuantity;
  CDWordArray lWeight;
  CDWordArray lPrice;
  CDWordArray lEntryIDList;
  CDWordArray lUnitPrice;
  CDWordArray lTare;
  CDWordArray lRestrictedAge;
  CDWordArray lScanType;	// 0- manual, other values as per OPOSScan.h
  CByteArray  bAutomaticApproval;  //true for automatic approval for items sold in assist mode
  CStringArray csExceptionCategory; 
  CByteArray bHasSecurityTag;	//true for item having security tag

public:
  PSInterfaceErcpt(                 // constructor with name and Idc
    LPCTSTR,                         // ... name of window
    const long);                    // ... Idc of list

  long Add(                         // add an item to a list
    const bool,				              // ... T/F highlight this entry
    LPCTSTR,                         // ... item entry description
    LPCTSTR,                         // ... item code that goes with this entry
    const long,                     // ... item quantity, used if voided later
    const long,                     // ... item weight, used if voided later
    const long,                     // ... item tare, used if voided later
    const long,						// ... extended item price, used if voided later
    const long,						// ... unit item price, used if voided later
	const long lBarcodeType,        // ... item scan type, 0 - code is manually entered
	const long lEntryIDFromTB = -999999999,		// .... Tb associates a unique entry id for each item sold
	const long = 0,                 // the restricted age
	LPCTSTR = NULL,					//exception category
	const bool = false);			// item has security tag                	

  long InsertAt(                         // add an item to a list
    const long nIndex,				// position or index to insert
    const bool,				        // ... T/F highlight this entry
    LPCTSTR,                         // ... item entry description
    LPCTSTR,                         // ... item code that goes with this entry
    const long,                     // ... item quantity, used if voided later
    const long,                     // ... item weight, used if voided later
    const long,                     // ... item tare, used if voided later
    const long,						// ... extended item price, used if voided later
    const long,						// ... unit item price, used if voided later
	const long lBarcodeType,        // ... item scan type, 0 - code is manually entered
	const long lEntryIDFromTB,		// .... Tb associates a unique entry id for each item sold
	const long = 0,                 // the restricted age
	LPCTSTR = NULL,					// exception category
	const bool = false);			// Item has security tag               	
  long Remove(const long nIndex);

   // add a price to an item in column 2
  // nOperation = 0 - add, 1- insert, 2 - delete
  long Column2(
    LPCTSTR, long nIndex = 1, short nOperation = 0);          // ... formatted price
	
  long Reset(void);                 // reset and empty this receipt or list
	
  long Get(                         // get selection desription
    CString &,                      // ... returned description
    CString &,                      // ... returned item code
    long &,                         // ... returned item quantity
    long &,                         // ... returned item weight
    long &,                         // ... returned item price
    long &,                         // ... returned unit price
    long &,
	long & lBarcodeType);           // ... returned item scan type, 0 - code is manually entered
									// return item restricted age

  long GetAt(                       // get passed entry desription
    const long,                     // ... entry index 1,2,3,4
    CString &,                      // ... returned description
    CString &,                      // ... returned item code
    long &,                         // ... returned item quantity
    long &,                         // ... returned item weight
    long &,                         // ... returned item price
    long &,                         // ... returned unit price
    long &,					                // ... returned item tare code
	long & lBarcodeType,            // ... returned item scan type, 0 - code is manually entered
    CString &);                     // ... exception category
	
  long GetResAgeAt(                 // get the restricted age  
    const long,						//entry index 1,2,3,4
    long &);							// age

  long GetSecurityTagItem(			// get the security tag item
	const long,						// entry index 1,2,3,4
	CString &,						// ... returned description
	CString &,						// ... returned item code
	bool &);						// item has security tag


  long GetCurrentItemAutoApprovalFlag(bool &);  // get the automatic approval flag
  long GetAutoApprovalFlagAt(const long, bool &);  // get the automatic approval flag at a certain index
  long SetCurrentItemAutoApprovalFlag(const bool);  // set the automatic approval flag
  long SetAutoApprovalFlagAt(const long, const bool);//TAR 174877
  long SetTopIndex(void);     //set top item index on receipt - tar 182942	
  long Select(
    const long,                     // ...index into list or receipt, 1,2,3...
    const bool);                    // ...modify, strike through line T/F

  void Init(LPCTSTR,                 // pointer this window's name                                               
            const long);            // ... Idc of list

  long GetEntryIDAt(const long nDx, long  &lCurrentEntryID); // nDx ..1,2,3....
  long GetVoidItemIndex();
};

#define BMPS int

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
PSFrame : public PSInterface
{
public:
  virtual long HideButtons();
  virtual long HideEcho();
  virtual long HideControl(const long id);  // hide this control.

  PSFrame(LPCTSTR szWName);
  virtual long ClearMessages(void) = 0;
  virtual long HideWindows(void) = 0;
  virtual long ShowFrame(LPCTSTR weight);
  virtual void SetBackground(const CString nBmp);

protected:
  int m_NumButtons;
  bool m_bHasHelpButton;
};

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
PSCMFrame : public PSFrame
{
public:
  PSCMFrame(LPCTSTR szWName);
  virtual long ClearMessages(void);
  virtual long HideWindows(void);
};

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
PSSMFrame : public PSFrame
{
public:
  PSSMFrame(LPCTSTR szWName);

  virtual long ClearMessages(void);
  virtual long HideWindows(void);
};

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
PSAMFrame : public PSFrame
{
public:
  PSAMFrame(LPCTSTR szWName);

  virtual long ClearMessages(void);
  virtual long HideWindows(void);
};

#endif

#endif
