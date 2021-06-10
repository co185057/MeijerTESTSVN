//
// FILE:  USSFProcedures.h
//
// TITLE: 
//
//
//
// AUTHOR:   Christopher Swift
// DATE:     9/2007
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DISPLAYSHOPDEFINITION
#define _DISPLAYSHOPDEFINITION

#include <vector>
using namespace std;

const TCHAR XML_DISPLAYSHOPFILE[] = _T("USSFDisplayShop.xml");
const TCHAR XML_IS_ACTIVE[] = _T("IsActive");
const TCHAR XML_SCANCODE[] = _T("ScanCode");
const TCHAR XML_NAME[] = _T("Name");
const TCHAR XML_ROOT[] = _T("//DisplayShop");
const TCHAR XML_ROOT_URL[] = _T("//DisplayShop//URL");
const TCHAR XML_GROUP[] = _T("//DisplayShop//DisplayGroup");
const TCHAR XML_GROUP_ID[] = _T("ID");
const TCHAR XML_GROUP_NAME[] = _T("Name");
const TCHAR XML_GROUP_DESC[] = _T("Description");
const TCHAR XML_GROUP_CONTROLA[] = _T("Control");
const TCHAR XML_GROUP_CONTROLB[] = _T("AlternateControl");
const TCHAR XML_GROUP_BUTTONTYPE[] = _T("ButtonType");
const TCHAR XML_GROUP_EVENT[] = _T("Event");
const TCHAR XML_GROUP_SIZE[] = _T("Size");
const TCHAR XML_GROUP_MAXBUTTONS[] = _T("MaxButtons");
const TCHAR XML_GROUP_MODEL[] = _T("Model");
const TCHAR XML_BUTTON[] = _T("Button");
const TCHAR XML_BUTTON_ID[] = _T("ID");
const TCHAR XML_BUTTON_NAME[] = _T("Name");
const TCHAR XML_BUTTON_CONTROL[] = _T("Control");
const TCHAR XML_BUTTON_DESC[] = _T("Description");
const TCHAR XML_BUTTON_GRAPHIC[] = _T("Graphic");
const TCHAR XML_BUTTON_PRICEDATA[] = _T("PriceData");
const TCHAR XML_BUTTON_POSDATA[] = _T("POSData");
const TCHAR XML_BUTTON_EVENT[] = _T("Event");
const TCHAR XML_BUTTON_ENABLED[] = _T("Enabled");
const TCHAR XML_BUTTON_STATUS[] = _T("Status");
const TCHAR XML_BUTTON_REPEAT[] = _T("Repeat");
const TCHAR XML_SYMBOL[] = _T("//DisplayShop//Symbol");
const TCHAR XML_SYMBOL_NAME[] = _T("Name");
const TCHAR XML_SYMBOL_VALUE[] = _T("Value");

enum PSXButtonPrecendence
{
	BUTTON1 = 0,
	BUTTON2 = 1,
	BUTTON3 = 2,
	BUTTON4 = 3,
	BUTTON5 = 4,
	BUTTON6 = 5,
	BUTTON7 = 6,
	BUTTON8 = 7,
};

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
class DisplaySymbolInfo
{
public:

  CString csName;
  CString csValue;
};

class DisplayButtonInfo
{
public:

  CString csID;
  CString csName;
  CString csControl;
  CString csDescription;
  CString csGraphic;
  CString csPriceData;
  CString csPOSData;
  CString csEvent;
  CString csEnabled;
  //CString csStatus;
  bool bSelected;
  CString csStatus;
};

class DisplayGroupInfo
{
public:

  CString csID;
  CString csName;
  CString csDescription;
  CString csControlA;
  CString csControlB;
  CString csButtonType;		// Same as Model.
  CString csEvent;
  CString csSize;
  CString csMaxButtons;
  //CString csModel;
  vector<DisplayButtonInfo> v_ButtonInfo;

  bool bSelected;
  bool bStandard;
};

// Class Definitions for Physical Display Handler.
class DisplayElement
{
public:
  CString csID;             // ID (from Display Shop).
  CString csControl;		// Control Name (Name in the XML File).
  CString csDesc;           // Button Description.
  CString csGraphic;        // Location of graphic.
  CString csAmount;         // Formatted Price.
  long lStatus;             // Unselected(0);Selected(1).
  long lIndex;              // Index within physical display.
  CString csEnabled;

protected:
};

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 PhysicalButtonDisplay 
// PhysicalButtonDisplay : public 
//#ifdef _BUILDLIB_
//	SMStateBase
//#else
//	SMState
//#endif
{
public:
  CString csCtxName;        // Target PSX Context ID.
  CString csCtlNameA;       // Target PSX Control ID.
  CString csCtlNameB;       // Target Secondary PSX Control ID.
  long lType;               // Control Button(0); Regular Selection(1); Radio Selection(2);
  CString csComponentID;    // Component ID
  long lCurrentScroll;      // For Scrolling.
  long lMaxButtons;
  vector<DisplayElement> v_Buttons;

  vector<DisplayElement>::iterator DisplayElementInfoIter;

//Methods.
  long Display(void);
  long DisplayPrimarySelection(CString csCtlName);
  long DisplaySecondarySelection(CString csCtlName);
  long DisplayControlButtons(void);
  long Refresh(void);
  long RefreshPrimarySelection(CString csCtlName);
  long RefreshSecondarySelection(CString csCtlName);
  long RefreshControlButtons(void);
  long GetDisplayMapElement(CString, long, CString&);
  long GetDisplayMapElement(long, CString&);
  long Select(long, long&);
  long DeSelect(long);
  long ScrollUp(void);
  long ScrollDown(void);

protected:
  //long lEnabledLimit;
};

// NOTE that there are currently no copy/assignment contructors/operators other than default.
// THEREFORE, do not copy this object. The resulting copy/assignment will be less than expected!
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 DisplayShop 
{
public:

  CString csName;
  CString csURL;
  bool bIngredientAlert;
  vector<DisplayGroupInfo> GroupInfo;
  vector<DisplayButtonInfo> ButtonInfo;
  vector<DisplaySymbolInfo> SymbolInfo;
  PhysicalButtonDisplay SelectionManager;
  PhysicalButtonDisplay ControlManager;
  DisplayShop();
  long LoadDefinition(CString fileName = _T(""));
  long ClearDefinition();
  long StoreSymbols();
  long LoadSelectionChoices(CString groupID, CString contextID, CString controlIDA);
  long LoadControlChoices(CString groupID, CString contextID, CString controlIDA);
  long GetButtonInfo(CString groupID, CString buttonID, CString& Event, CString& POSData);

protected:
  vector<DisplayGroupInfo>::iterator GroupInfoIter;
  vector<DisplayButtonInfo>::iterator ButtonInfoIter;
  vector<DisplaySymbolInfo>::iterator SymbolInfoIter;

  CStringArray csaListResult;
  CString csTempResult;

  DisplayGroupInfo myDisplayGroupInfo;
  DisplayButtonInfo myDisplayButtonInfo;
  DisplaySymbolInfo myDisplaySymbolInfo;
};

#endif //_DISPLAYSHOPDEFINITION
