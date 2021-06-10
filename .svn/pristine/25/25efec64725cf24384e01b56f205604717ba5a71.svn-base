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

const TCHAR XML_DISPLAYSHOPFILE[] =								_T("USSFDisplayShop.xml");
const TCHAR XML_IS_ACTIVE[] =									_T("IsActive");
const TCHAR XML_SCANCODE[] =									_T("ScanCode");
const TCHAR XML_NAME[] =										_T("Name");
const TCHAR XML_ROOT[] =										_T("//DisplayShop");
const TCHAR XML_ROOT_URL[] =									_T("//DisplayShop//URL");
const TCHAR XML_GROUP[] =										_T("//DisplayShop//DisplayGroup");
const TCHAR XML_GROUP_ID[] =									_T("ID");
//const TCHAR XML_GROUP_NAME[] =									_T("Name");				// Deprecated - Redundant.
const TCHAR XML_GROUP_DESC[] =									_T("Description");
const TCHAR XML_GROUP_CONTROLS[] =								_T("Controls//Control");
const TCHAR XML_GROUP_CONTROL_ID[] =							_T("ID");				// Control Name.
const TCHAR XML_GROUP_CONTROL_TYPE[] =							_T("Type");				// Matrix (Grid), Singleton
const TCHAR XML_GROUP_CONTROL_BUTTONTYPE[] =					_T("ButtonType");		// Radio versus non-Radio
const TCHAR XML_GROUP_CONTROL_SCROLLUP[] =						_T("ScrollUp");			// Button -> UP.<control>
const TCHAR XML_GROUP_CONTROL_SCROLLDOWN[] =					_T("ScrollDown");		// Button -> DOWN.<control>
const TCHAR XML_GROUP_CONTROL_EVENT[] =							_T("Event");			// USSF Event
const TCHAR XML_GROUP_CONTROL_SIZE[] =							_T("Size");				// Total Buttons
const TCHAR XML_GROUP_CONTROL_MAXBUTTONS[] =					_T("MaxButtons");		// On Screen
//const TCHAR XML_GROUP_CONTROL_MODEL[] =							_T("Model");			// Deprecated
const TCHAR XML_GROUP_CONTROL_BUTTONS[] =						_T("Buttons//Button");
const TCHAR XML_GROUP_CONTROL_BUTTON_ID[] =						_T("ID");				// Control
//const TCHAR XML_GROUP_CONTROL_BUTTON_NAME[] =					_T("Name");					// Deprecated
//const TCHAR XML_GROUP_CONTROL_BUTTON_CONTROL[] =				_T("Control");
//const TCHAR XML_GROUP_CONTROL_BUTTON_DESC[] =					_T("Description");
//const TCHAR XML_GROUP_CONTROL_BUTTON_GRAPHIC[] =				_T("Graphic");
const TCHAR XML_GROUP_CONTROL_BUTTON_PICTURE[] =				_T("Picture");
const TCHAR XML_GROUP_CONTROL_BUTTON_LOGO[] =					_T("Logo");
//const TCHAR XML_GROUP_CONTROL_BUTTON_PRICEDATA[] =				_T("PriceData");
const TCHAR XML_GROUP_CONTROL_BUTTON_METADATA[] =				_T("MetaData");
const TCHAR XML_GROUP_CONTROL_BUTTON_EVENT[] =					_T("Event");			// USSF Event
const TCHAR XML_GROUP_CONTROL_BUTTON_ENABLED[] =				_T("Enabled");			// Does Button Get Displayed?
const TCHAR XML_GROUP_CONTROL_BUTTON_STATUS[] =					_T("Status");			// Deprecated
const TCHAR XML_GROUP_CONTROL_BUTTON_REPEAT[] =					_T("Repeat");			// Number of Times to Repeat.
const TCHAR XML_GROUP_CONTROL_BUTTON_COMPONENTS[] =				_T("Components//Control");	// Button Text.
const TCHAR XML_GROUP_CONTROL_BUTTON_COMPONENTS_ID[] =			_T("ID");				// Button Control Name
const TCHAR XML_GROUP_CONTROL_BUTTON_COMPONENTS_TYPE[] =		_T("Type");				// Button Control Type
const TCHAR XML_GROUP_CONTROL_BUTTON_COMPONENTS_VALUE[] =		_T("Value");			// Button Value

const TCHAR XML_SYMBOL[] =					_T("//DisplayShop//Symbol");
const TCHAR XML_SYMBOL_NAME[] =				_T("Name");
const TCHAR XML_SYMBOL_VALUE[] =			_T("Value");
const TCHAR XML_SYMBOL_LANGUAGE[] =			_T("Languages//Language");
const TCHAR XML_SYMBOL_LANGUAGE_ID[] =		_T("LCID");
const TCHAR XML_SYMBOL_LANGUAGE_VALUE[] =	_T("Value");

/*
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
*/
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

class DisplayComponentInfo
{
public:

  CString csID;
  CString csType;
  CString csValue;
};

class DisplayButtonInfo
{
public:

  CString csID;
  CString csPicture;
  CString csLogo;
  CString csMetaData;
  CString csEvent;
  CString csEnabled;
  bool bSelected;
  CString csStatus;
  vector<DisplayComponentInfo> v_Components;
};

class DisplayControlInfo
{
public:
	CString csID;
	CString csType;				// Button, Grid
	CString csButtonType;
	CString csScrollUp;
	CString csScrollDown;
	CString csEvent;
	CString csSize;
	CString csMaxButtons;
	vector<DisplayButtonInfo> v_Button;
};

class DisplayGroupInfo
{
public:

  CString csID;
  CString csDescription;
  vector<DisplayControlInfo> v_Control;
};

// Class Definitions for Physical Display Handler.
class DisplayElement
{
public:
  CString csID;             // ID (from Display Shop).
  CString csControl;		// Control Name (Name in the XML File).
  CString csPicture;
  CString csLogo;			// Location of graphic.
  long lStatus;             // Unselected(0);Selected(1).
  long lElementIndex;       // Index within physical display.
  CString csEnabled;
  CString csMetaData;
  CString csEvent;
  vector<DisplayComponentInfo> v_Components;
protected:
};

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 PhysicalButtonDisplay 
{
public:
  PhysicalButtonDisplay(){bValid = false;}
  CString csCtxName;        // Target PSX Context ID.
  CString csCtlName;        // Target PSX Control ID.
  long lType;               // Control Button(0); Regular Selection(1); Radio Selection(2);
  CString csComponentID;    // Component ID
  //long lCurrentScroll;      // For Scrolling.
  long lMaxButtons;
  CString csScrollUp;
  CString csScrollDown;
  bool bValid;
  vector<DisplayElement> v_Buttons;

//Methods.
  long Reset(){v_Buttons.clear();bValid = false;return 0;}
  long Display(void);
  long DisplayGrid(void);
  long DisplayButtons(void);
  long Refresh(void);
  long RefreshGrid(void);
  long RefreshButtons(void);
  long GetDisplayMapElement(CString, long, CString&);
  long GetDisplayMapElement(long, CString&);
  long Select(CString, CString&);
  long Select(long, long&);
  long DeSelect(long);
  long DeSelect(CString);
  long ScrollUp(void);
  long ScrollDown(void);
  long ShowScroll(void);
  long FindElementIndex(long, CString&, CString&, CString&);
  long FindElementID(CString&, CString&, CString&);
  void PutToSafeArray(int nIndex, COleSafeArray& saArray, CString csString);
protected:
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

  DisplayShop();
  long LoadDefinition(CString fileName = _T(""));
  long ClearDefinition();
  long StoreSymbols();
  //long LoadDisplayChoices(CString groupID, CString contextID);
  //long LoadSelectionChoices(CString groupID, CString contextID, CString controlIDA = _T(""));
  //long LoadControlChoices(CString groupID, CString contextID, CString controlIDA = _T(""));
  long LoadChoices(CString groupID, CString contextID);
  long GetButtonInfo(CString groupID, CString buttonID, CString& Event, CString& POSData);
  long HandleListSelection(CString csIndex, CString& csEvent);
  long HandleControlButton(CString csID, CString& csEvent);
  long InitializeDisplayElements(CString csContext, CString csDisplayGroup);
  long InvalidateDisplayElements();
  long ScrollUp(CString csCommand);
  long ScrollDown(CString csCommand);
  long Display();
  long Refresh();
  long Select(CString csControl, CString csID, CString &csReplace);
  long DeSelect(CString csControl, CString csID);

  CString csName;
  CString csURL;
  bool bIngredientAlert;
  vector<DisplayGroupInfo> v_Group;
  vector<DisplaySymbolInfo> SymbolInfo;
  //PhysicalButtonDisplay SelectionManager;
  //PhysicalButtonDisplay ControlManager;
  //vector<PhysicalButtonDisplay> OverflowManager;
  vector<PhysicalButtonDisplay> DisplayManager;
  PhysicalButtonDisplay GridManager;
  PhysicalButtonDisplay ButtonManager;

protected:
  CStringArray csaListResult;
  CString csTempResult;

  DisplayGroupInfo myDisplayGroupInfo;
  DisplayControlInfo myDisplayControlInfo;
  DisplayButtonInfo myDisplayButtonInfo;
  DisplayComponentInfo myDisplayComponentInfo;
  DisplaySymbolInfo myDisplaySymbolInfo;

  long lElementSerial;
};

#endif //_DISPLAYSHOPDEFINITION
