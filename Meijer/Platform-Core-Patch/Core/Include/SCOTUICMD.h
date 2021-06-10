/////////////////////////////////////////////////////////////////////////////////
// FastLane user interface can be controlled by Transaction Broker or by any module
// external module in FastLane environment.
// This header file SCOTUICOM.h (SCOT UI COMMAND interface header file)
// defines the structures shared between SCOT and TB(or any other requesting module).
// 
// Created on 02/03/2003
/////////////////////////////////////////////////////////////////////////////////

#ifndef SCOTTB_UI_INTERFACE_HEADER
#define SCOTTB_UI_INTERFACE_HEADER

#include <OAIDL.H>

#include <afxWin.h>

//extern long CopySCOTString(LPTSTR& Destination, LPCTSTR Source);
// fixed size chosen for its simplicity and based on requirements
#define QUERY_MAX   100		// maximum number of variables/properties of an item that can be queried in a single call

#define DELETE_UIPTR(ptrName) { if ((ptrName) && _tcsclen(ptrName)) delete ptrName; ptrName = NULL;}

#define DELETE_UIVAR_ARRAY(pArrName) { \
									bool bContinue = true; \
									for (int nIndex = 0; ((nIndex < QUERY_MAX) && bContinue); ++nIndex)  \
									{ \
										if (pArrName[nIndex]) \
										{ \
										    pArrName[nIndex]->CleanUp(); \
											delete pArrName[nIndex]; \
											pArrName[nIndex] = NULL; \
										} \
										else \
										{ \
											bContinue = false; \
										} \
									} \
								  }; 

#define CopySCOTString(Destination, Source) { \
												long Len = 0; \
												if (Source) \
													Len = _tcsclen(Source);\
												if (Len)\
												{\
													Destination = (LPTSTR) new _TCHAR[Len+1];\
													memcpy(Destination, Source, (Len+1) * sizeof(_TCHAR));\
												}\
												else\
												{\
													Destination = NULL;\
												}\
											};



// Union Structure Type  Used with S_UI_CMD.eStructType
typedef enum  {
    UI_STR_RECEIPT = 1,
    UI_STR_FINDITEM,
    UI_STR_SELECTEDITEM,
    UI_STR_SCROLL,
    UI_STR_TRANSVAR,
    UI_STR_CONTEXT,
    UI_STR_SEND_EVENT,

    //Please add any new entries above this line
    UI_STR_LASTINLIST = 2000
} eUI_STR;

// Receipt manipulation commands used w/ S_UI_Receipt.eItemCmd
typedef enum  {
    UI_ITEM_CMD_READVARS = 1,
    UI_ITEM_CMD_UPDATE,
    UI_ITEM_CMD_INSERT,
    UI_ITEM_CMD_REMOVEITEM,
    UI_ITEM_CMD_REMOVEVARS,
    UI_ITEM_CMD_CLEARRCPT,
    UI_ITEM_CMD_CLEARRCPTITEMVARS,

    //Please add any new entries above this line
    UI_ITEM_CMD_LASTINLIST = 2000
} eUI_ITEM_CMD;

// Receipt scroll commands used w/ S_UI_Scroll.eScrollCmd
typedef enum  {
    UI_SCROLL_LINEUP = 1,
    UI_SCROLL_LINEDOWN,
    UI_SCROLL_PAGEUP,
    UI_SCROLL_PAGEDOWN,
    UI_SCROLL_TOP,
    UI_SCROLL_BOTTOM,

    //Please add any new entries above this line
    UI_SCROLL_LASTINLIST = 2000
} eUI_SCROLL_CMD;

// Access control commands used w/ S_UI_SelectedItem.eAccessCmd
//				 and w/ S_UI_TransactionVar.eAccessCmd
typedef enum  {
    UI_ACCESS_GET = 1,
    UI_ACCESS_SET,

    //Please add any new entries above this line
    UI_ACCESS_LASTINLIST
} eUI_ACCESS_CMD;



// Event Codes used w/ S_UI_SendEvent.lEventCode
typedef enum  {
    UI_EVENT_BUTTONCLICK = 1,
    UI_EVENT_CHANGESTATE,
    UI_EVENT_CHANGEVISIBLE,
        
    //Please add any new entries above this line
    UI_EVENT_LASTINLIST = 2000
} eUI_EVENT_CMD;

// Structure describing a receipt item or transaction 
//	variable name - value pair used with S_UI_Receipt.pUiVars 
//	and S_UI_TransactionVar.pUiVars
typedef struct 
{
	long lStructId;			// reserved
	long lOpStatus;
	eUI_ACCESS_CMD	eAccessCmd;	
	LPTSTR		lpcVarName;
	VARIANT		vVarValue;

	void InitStruct()
	{
		lpcVarName = NULL;
		VariantInit(&vVarValue);
	}
	void SetVarName(LPCTSTR szName) {CopySCOTString(lpcVarName, szName);}
	bool ValidateStruct()
	{
		bool bReturn = false;
		if (lpcVarName && _tcsclen(lpcVarName))
		{
			bReturn = true;
		}
		return bReturn;
	}
	void CleanUp()
	{
		DELETE_UIPTR(lpcVarName);
		VariantClear(&vVarValue);
	}
} S_UI_VAR, *PS_UI_VAR;


// Structure to manipulate receipt items
//		UiBase.lStructType = 1
typedef struct S_UI_Receipt_type
{
	LPTSTR	   lpcItemId;
	LPTSTR	   lpcRcptStructName;
	LPTSTR	   lpcRefItemId;
	eUI_ITEM_CMD  eItemCmd;
	PS_UI_VAR	   pUiVars[QUERY_MAX]; 

	void InitStruct()
	{
		lpcItemId = NULL;
		lpcRcptStructName = NULL;
		lpcRefItemId = NULL;
		for(int i=0; (i < QUERY_MAX);++i)
		{
			pUiVars[i] = NULL;
		}
		eItemCmd = UI_ITEM_CMD_LASTINLIST;
	}
	void SetItemID(LPCTSTR szName)    {CopySCOTString(lpcItemId, szName);}
	void SetRcptName(LPCTSTR szName)  {CopySCOTString(lpcRcptStructName, szName);}
	void SetRefItemID(LPCTSTR szName) {CopySCOTString(lpcRefItemId, szName);}
	bool ValidateStruct()
	{
		bool bReturn = false;
		if ((lpcRcptStructName && _tcsclen(lpcRcptStructName)) &&
			(lpcItemId && _tcsclen(lpcItemId)) &&
			// ensure atleast one valid element exists
			(pUiVars[0] && pUiVars[0]->ValidateStruct())
		   )
		{
			bReturn = true;
		}
		return bReturn;
	}
	void CleanUp()
	{
		DELETE_UIPTR(lpcItemId);
		DELETE_UIPTR(lpcRcptStructName);
		DELETE_UIPTR(lpcRefItemId);
		DELETE_UIVAR_ARRAY(pUiVars);
	} //
} S_UI_Receipt;

// Structure to search receipt items against (multiple) criterea returning ItemId array
//		UiBase.lStructType = 2
typedef struct 
{
	LPTSTR		lpcRcptStructName;
	PS_UI_VAR	pUiVars[QUERY_MAX];  
	int			nArrSize;    // item id array size
	LPTSTR*     ItemIdArray; // array with each element storing the item id
	void InitStruct()
	{
		lpcRcptStructName = NULL;
		ItemIdArray = NULL;
		nArrSize = 0;
		for(int i=0; (i < QUERY_MAX);++i)
		{
			pUiVars[i] = NULL;
		}
	}
	void SetRcptName(LPCTSTR szName)  {CopySCOTString(lpcRcptStructName, szName);}
	bool ValidateStruct()
	{
		bool bReturn = false;
		if (lpcRcptStructName && _tcsclen(lpcRcptStructName))
		{
			bReturn = true;
		}
		return bReturn;
	}
	void CleanUp()
	{
		DELETE_UIPTR(lpcRcptStructName);
		DELETE_UIVAR_ARRAY(pUiVars);
		if (ItemIdArray && nArrSize>0)
		{
			int nIDCount = nArrSize-1;
			while ((nIDCount>= 0) && ItemIdArray[nIDCount])
			{
				delete[] ItemIdArray[nIDCount];
				--nIDCount;
			}
			delete[] ItemIdArray;
			ItemIdArray = NULL;
		}
	}
} S_UI_FindItem;

// Structure to get/set the selected item of a receipt
// ** Set not currently supported.
//		UiBase.lStructType = 3
typedef struct 
{
	LPTSTR		lpcRcptStructName;
	eUI_ACCESS_CMD	eAccessCmd;		// Set may not be supported
	LPTSTR		lpcItemId;
	void InitStruct()
	{
		lpcRcptStructName = NULL;
		lpcItemId = NULL;
		eAccessCmd = UI_ACCESS_LASTINLIST;
	}
	void SetRcptName(LPCTSTR szName)  {CopySCOTString(lpcRcptStructName, szName);}
	void SetItemID(LPCTSTR szName)    {CopySCOTString(lpcItemId, szName);}
	bool ValidateStruct()
	{
		bool bReturn = false;
		if ((lpcRcptStructName && _tcsclen(lpcRcptStructName)) &&
			(lpcItemId && _tcsclen(lpcItemId))
		   )
		{
			bReturn = true;
		}
		return bReturn;
	}
	void CleanUp()
	{
		DELETE_UIPTR(lpcItemId);
		DELETE_UIPTR(lpcRcptStructName);
	}
} S_UI_SelectedItem;

// Structure to direct scrolling of a receipt.
//		UiBase.lStructType = 4
typedef struct 
{
	LPTSTR		lpcRcptStructName;
	eUI_SCROLL_CMD	eScrollCmd;
	void InitStruct()
	{
		lpcRcptStructName = NULL;
		eScrollCmd = UI_SCROLL_LASTINLIST;
	}
	void SetContextName(LPCTSTR szName) {CopySCOTString(lpcRcptStructName, szName);}
	bool ValidateStruct()
	{
		bool bReturn = false;
		if (lpcRcptStructName && _tcsclen(lpcRcptStructName))
		{
			bReturn = true;
		}
		return bReturn;
	}
	void CleanUp()
	{
		DELETE_UIPTR(lpcRcptStructName);
	}
} S_UI_Scroll;

// Structure to get/set transaction variable values
//		UiBase.lStructType = 5
typedef struct 
{
	PS_UI_VAR	   pUiVars[QUERY_MAX];  // access type -get/set is set in this array for each transaction var
	void InitStruct()
	{
		for(int i=0; (i < QUERY_MAX);++i)
		{
			pUiVars[i] = NULL;
		}
	}
	bool ValidateStruct()
	{
		bool bReturn = false;
		// ensure atleast one valid element exists
		if (pUiVars[0] && pUiVars[0]->ValidateStruct())
		{
			bReturn = true;
		}
		return bReturn;
	}
	void CleanUp()
	{
		DELETE_UIVAR_ARRAY(pUiVars);
	} 
} S_UI_TransactionVar;

// Structure to request display of a configuration file defined context
//		UiBase.lStructType = 6
typedef struct 
{
	LPTSTR		lpcContextName;
	long		lClientCategoryMask;
	void SetContextName(LPCTSTR szName) {CopySCOTString(lpcContextName, szName);}
	void InitStruct()
	{
		lpcContextName = NULL;
	}
	bool ValidateStruct()
	{
		bool bReturn = false;
		if (lpcContextName && _tcsclen(lpcContextName))
		{
			bReturn = true;
		}
		return bReturn;
	}
	void CleanUp()
	{
		DELETE_UIPTR(lpcContextName);
	}
} S_UI_Context;

// Structure to inject an event into the UI (e.g. a button press)
//		UiBase.lStructType = 7
typedef struct 
{
	LPTSTR		lpcContextName;
	LPTSTR		lpcControlName;
	long		lEventCode;
	VARIANT		vEventData;
	void InitStruct()
	{
		lpcContextName = NULL;
		lpcControlName = NULL;
		VariantInit(&vEventData);
	}
	void SetContextName(LPCTSTR szName) {CopySCOTString(lpcContextName, szName);}
	void SetControlName(LPCTSTR szName) {CopySCOTString(lpcControlName, szName);}
	bool ValidateStruct()
	{
		bool bReturn = false;
		if ( 
			(lpcContextName && _tcsclen(lpcContextName)) &&
			(lpcControlName && _tcsclen(lpcControlName))
		   )
		{
			bReturn = true;
		}
		return bReturn;
	}
	void CleanUp()
	{
		DELETE_UIPTR(lpcContextName);
		DELETE_UIPTR(lpcControlName);
		VariantClear(&vEventData);
	}
} S_UI_SendEvent;



typedef struct S_SCOTUI_CMD
{
	eUI_STR  eStructType ;	// identifies union type
	long lUserData;		    // for the convience of TB
	long lStatus;			// operation status
	long lStatusIndex;		// used to indicate which of a series of operations failed.
    HWND hStatusReturn2Wnd; // reserved field1
    long lUnKnown1;         // reserved field1
	union 
	{
		S_UI_Receipt		Rcpt;
		S_UI_FindItem		Find;
		S_UI_SelectedItem	Sel;
		S_UI_Scroll			Scroll;
		S_UI_TransactionVar Trans;
		S_UI_Context		Cntx;
		S_UI_SendEvent		Send;
	};
    struct S_SCOTUI_CMD* pNextUICommand; // link a next UI command or set it to NULL
	void CleanUnionField()
	{
		switch (eStructType)
		{
			case UI_STR_RECEIPT:
			{
				Rcpt.CleanUp();
			}
			break;
			case UI_STR_FINDITEM:
			{
				Find.CleanUp();
			}
			break;
			case UI_STR_SELECTEDITEM:
			{
				Sel.CleanUp();
			}
			break;
			case UI_STR_SCROLL:
			{
				Scroll.CleanUp();


			}
			break;
			case UI_STR_TRANSVAR:
			{
				Trans.CleanUp();
			}
			break;
			case UI_STR_CONTEXT:
			{
				Cntx.CleanUp();
			}
			break;
			case UI_STR_SEND_EVENT:
			{
				Send.CleanUp();
			}
			break;
			case UI_STR_LASTINLIST:
			default:
			break;
		} // end of switch
	} // end of CleanUnionField
	void InitUnionField()
	{
		switch (eStructType)
		{
			case UI_STR_RECEIPT:
			{
				Rcpt.InitStruct();
			}
			break;
			case UI_STR_FINDITEM:
			{
				Find.InitStruct();
			}
			break;
			case UI_STR_SELECTEDITEM:
			{
				Sel.InitStruct();
			}
			break;
			case UI_STR_SCROLL:
			{
				Scroll.InitStruct();
			}
			break;
			case UI_STR_TRANSVAR:
			{
				Trans.InitStruct();
			}
			break;
			case UI_STR_CONTEXT:
			{
				Cntx.InitStruct();
			}
			break;
			case UI_STR_SEND_EVENT:
			{
				Send.InitStruct();
			}
			break;
			case UI_STR_LASTINLIST:
			default:
			break;
		} // end of switch
	} // end of InitUnionField
	void CleanUp()
	{
		if (pNextUICommand)
		{
			pNextUICommand->CleanUp();
			delete pNextUICommand;
		}
		CleanUnionField();
		InitUnionField();
	}; // end of CleanUp()
} S_UI_CMD, *PS_UI_CMD;

// UICMD_EXAMPLE Start (Modify existing receipt item's price
//struct 	S_UI_VAR*  pUiVars[1];

// pUiVars[0] = new S_UI_VAR;
// pUiVars[0]->lpcVarName = _T("ExtendedPrice");
// pUiVars[0]->vVarValue = COleVariant(long(123));

// S_UI_CMD* pUiStruct= new S_UI_CMD;
// pUiStruct->Rcpt.pUiVars = &pUiVars[0];
// pUiStruct->Rcpt.lpcItemId = _T("Item003");
// pUiStruct->Rcpt.lpcRcptStructName = _T("NameOfFLReceiptStrucureToModify");
// pUiStruct->Rcpt.eItemCmd = UI_ITEM_CMD_UPDATE;		// item id
// UICMD_EXAMPLE end

#endif // SCOTTB_UI_INTERFACE_HEADER