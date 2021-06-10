//
// FILE:  USSFProcedures.cpp
//
// TITLE: Display Shop Procedures to support USSF.
//
//
//
// AUTHOR:   Christopher Swift
// DATE:     9/2007
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
//	TAR #429050 UI Changes for G3 (Support PSX 1.24)
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
//               // MS MFC template header
#include <afxpriv.h>                // for USES_CONVERSION
// BeginCodeLocale
#include "Common.h"					// Common includes
#include "USSFCommon.h"
#include "SMStateUSSFMacros.h"

#define COMP_ID   ID_PS
#define T_ID      _T("USSFProcedures")

CPSX*  m_pGAPSX = NULL;          // NewUI - pointer to PSX object

////////////////////////////////////
//GraphicalButtonDisplay Methods!
////////////////////////////////////
long PhysicalButtonDisplay::GetDisplayMapElement(long lIndex, CString &csID)
{
  long rc = 0;

  if (v_Buttons.size() > lIndex)
  {
    csID = v_Buttons[lIndex].csID;
  }
  else
  {
    csID = _T("");
    rc = 1;
  }

  return rc;
}

long PhysicalButtonDisplay::GetDisplayMapElement(CString csControl, long lIndex, CString &csID)
{
  long rc = 1;

  // Can we find the specific control that was pressed?
  for (int i = 0; i < v_Buttons.size(); i++)
  {
	CString csSource = v_Buttons[i].csID, csTarget = csControl;
	csSource.MakeUpper();
	csTarget.MakeUpper();
    if (csSource.Find(csTarget) > -1)
	{
		// Yes, report it!
		csID = v_Buttons[i].csID;
		rc = 0;
	}
  }
  if (rc == 1)
  {
	  // Find by Position, if possible.
	  rc = GetDisplayMapElement(lIndex, csID);
  }

  return rc;
}

long PhysicalButtonDisplay::Display()
{
  long rc = 0;
  COleVariant v, vReturn;
  CString csTemp;
  // Need to decide what we are displaying.
  // Selection Buttons OR Control Buttons.
  // Depends upon the type.
  switch(lType)
  {
  case 0:
	  {
		// Buttons!
		rc = DisplayButtons();
	  }
	  break;
  case 1:
  default:
	  {
		// Grid!
		rc = DisplayGrid();
		if (rc == 0)
		{
			ShowScroll();
		}
		else
		{
		  trace(L5, _T("Primary Control Display Failed."));
		}
	  }
	  break;
  }

  return rc;
}

long PhysicalButtonDisplay::DisplayGrid()
{
	long rc = 0;
	PSXRC PSXrc;

	USES_CONVERSION;

	if (!USSFutil.TestPSX())
	{
		return 0;
	}
  
	// reset the count to 0 to clear all previously set attributes
	COleVariant v = (long) 0, vReturn;
	PSXrc = m_pGAPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
	PSXrc = m_pGAPSX->SetControlProperty(csCtlName, UI::PROPERTYBUTTONCOUNT, v );

	//v = lMaxButtons;
	v = (long)v_Buttons.size();
	PSXrc = m_pGAPSX->SetControlProperty( csCtlName, UI::PROPERTYREDRAW, VARIANT_FALSE );

	PSXrc = m_pGAPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
	PSXrc = m_pGAPSX->SetControlProperty( csCtlName, UI::PROPERTYBUTTONCOUNT, v );

	CString str, csTemp;
	// Need to traverse the display elements from offset for up to MAXBUTTONNUM elements.
	for (long i = 0; i < v_Buttons.size(); i++)
	{
		CString csSubControl;
		csSubControl.Format(_T("%s[%d]"), csCtlName, i);

		//Set button state (pushed/normal/disabled).
		CString csTemp, str;
		str = v_Buttons[i].csEnabled;
		if (!str.CompareNoCase(_T("true")))
		{
			// Now check the status to see if pushed or normal.
			if (v_Buttons[i].lStatus)
			{
				v = (long)UI::Pushed;
			}
			else
			{
				v = (long)UI::Normal;
				//v_Buttons[i].lStatus = 1;
			}
		}
		else
		{
			v = (long)UI::Disabled;
			v_Buttons[i].lStatus = 0;
		}

		/*
		//Set button state (enabled/disabled).
        if (v_Buttons[i].lStatus)
        {
		    v = (long) UI::Pushed;
        }
		else
		{
			str = v_Buttons[i].csEnabled;
			if (!str.CompareNoCase(_T("true")))
			{
				v = (long) UI::Normal;
			}
			else
			{
				v = (long) UI::Disabled;
			}
		}
		*/
		PSXrc = m_pGAPSX->SetControlProperty(csSubControl, UI::PROPERTYSTATE, v);

		//Set Button Picture
		csTemp = v_Buttons[i].csPicture;
		if (csTemp.GetLength() > 0)
		{
			rc = ExpandEnvironmentStrings(csTemp, str.GetBuffer(255), 255);
			v = str;
			PSXrc = m_pGAPSX->SetControlProperty(csSubControl, UI::PROPERTYPICTURE, v);
		}

		//Set Button Logo
		csTemp = v_Buttons[i].csLogo;
		if (csTemp.GetLength() > 0)
		{
			rc = ExpandEnvironmentStrings(csTemp, str.GetBuffer(255), 255);
			v = str;
			PSXrc = m_pGAPSX->SetControlProperty(csSubControl, UI::PROPERTYLOGO, v);
		}

		//Set Button Data
		if (csCtlName.Find(_T("CmdBtn_")) >= 0)
		{
			CString csCommand;
			USSFutil.BuildUICommand(csCommand, _T("SELECT"), csCtlName, v_Buttons[i].csMetaData);
			//v = v_Buttons[i].csMetaData;		// Meta Data
			v = csCommand;		// UI Command
		}
		else
		{
			v = (long)i;						// Index
		}
		PSXrc = m_pGAPSX->SetControlProperty(csSubControl, UI::PROPERTYDATA, v);
	
		// Loop through Button Components and populate respective controls (for now just text).
		// If the main control for the grid is "X", then the button is assumed (by PSX) to be "XButton".
		for (long j = 0; j < v_Buttons[i].v_Components.size(); j++)
		{
			//Set Text Control.
			CString csTemp = v_Buttons[i].v_Components[j].csValue;
			if (csTemp.GetLength() > 0)
			{
				v = csTemp;
				csSubControl.Format(_T("%s[%d].%s"), csCtlName, i, v_Buttons[i].v_Components[j].csID);
				PSXrc = m_pGAPSX->SetControlProperty(csSubControl, UI::PROPERTYTEXT, v);
			}
		}
	}
	PSXrc = m_pGAPSX->SetControlProperty( csCtlName, UI::PROPERTYREDRAW, VARIANT_TRUE );
	return 0;
}

long PhysicalButtonDisplay::DisplayButtons(void)
{
	long rc = 0;
	PSXRC PSXrc;

	USES_CONVERSION;

	if (!USSFutil.TestPSX())
	{
		return 0;
	}

	COleVariant v;
	// Need to traverse the display elements from offset for up to MAXBUTTONNUM elements.
	for (long i = 0; i < v_Buttons.size(); i++)
	{
		CString thisControl;
		if (v_Buttons[i].csID.GetLength() > 0)
		{
			thisControl = v_Buttons[i].csID;
		}
		else
		{
			continue;
		}
        // Put it on the context!
		CString csTemp, str;
		str = v_Buttons[i].csEnabled;
		if (!str.CompareNoCase(_T("true")))
		{
			// Now check the status to see if pushed or normal.
			if (v_Buttons[i].lStatus)
			{
				v = (long)UI::Pushed;
				PSXrc = m_pGAPSX->SetConfigProperty(thisControl, csCtxName, UI::PROPERTYSTATE, v);
			}
			else
			{
				v = (long)UI::Normal;
				PSXrc = m_pGAPSX->SetConfigProperty(thisControl, csCtxName, UI::PROPERTYSTATE, v);
				//v_Buttons[i].lStatus = 1;
			}
		}
		else
		{
			v = (long)UI::Disabled;
			PSXrc = m_pGAPSX->SetConfigProperty(thisControl, csCtxName, UI::PROPERTYSTATE, v);
			v_Buttons[i].lStatus = 0;
		}

		//Set Button Picture
		csTemp = v_Buttons[i].csPicture;
		if (csTemp.GetLength() > 0)
		{
			rc = ExpandEnvironmentStrings(csTemp, str.GetBuffer(255), 255);
			v = str;
			m_pGAPSX->SetControlProperty(thisControl, UI::PROPERTYPICTURE, v);
		}

		//Set Button Logo
		csTemp = v_Buttons[i].csLogo;
		if (csTemp.GetLength() > 0)
		{
			rc = ExpandEnvironmentStrings(csTemp, str.GetBuffer(255), 255);
			v = str;
			m_pGAPSX->SetControlProperty(thisControl, UI::PROPERTYLOGO, v);
		}

        //v = v_Buttons[i].csMetaData;
		CString csCommand;
		USSFutil.BuildUICommand(csCommand, _T("SELECT"), thisControl, v_Buttons[i].csMetaData);
		v = csCommand;		// UI Command
		PSXrc = m_pGAPSX->SetConfigProperty(thisControl, csCtxName, UI::PROPERTYDATA, v);

		// Loop through Button Components and populate respective controls (for now just text).
		// If the main control for the grid is "X", then the button is assumed (by PSX) to be "XButton".
		for (long j = 0; j < v_Buttons[i].v_Components.size(); j++)
		{
			//Set Text Control.
			CString csTemp = v_Buttons[i].v_Components[j].csValue;
			if (csTemp.GetLength() > 0)
			{
				v = csTemp;
				CString csFieldName = v_Buttons[i].v_Components[j].csID;
				CString csControlReference;
				//if (csFieldName == csCtlName)
				if (csFieldName == thisControl)
				{
					// Simple Button.
					csControlReference = csFieldName;
				}
				else
				{
					// Complex Button w/Embedded Controls.
					//csControlReference.Format(_T("%s.%s"), csCtlName, csFieldName);
					csControlReference.Format(_T("%s.%s"), thisControl, csFieldName);
				}
				PSXrc = m_pGAPSX->SetConfigProperty(csControlReference, csCtxName, UI::PROPERTYTEXTFORMAT, v);
			}
		}
		PSXrc = m_pGAPSX->SetConfigProperty(thisControl, csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
	}
	return rc;
}

long PhysicalButtonDisplay::Refresh()
{
  long rc = 0;

  USES_CONVERSION;

  if (!USSFutil.TestPSX())
  {
    return 0;
  }

  // Need to decide what we are displaying.
  // Selection Buttons OR Control Buttons.
  // Depends upon the type.
  switch(lType)
  {
  case 0:
	  {
		// Buttons!
		rc = RefreshButtons();
		if (!rc)
		{
			trace(L5, _T("Button Refresh Failed."));
		}
	  }
	  break;
  case 1:
  default:
	  {
		// Grid Buttons!
		rc = RefreshGrid();
		if (!rc)
		{
			trace(L5, _T("Grid Refresh Failed."));
		}
	  }
	  break;
  }

  return rc;
}

long PhysicalButtonDisplay::RefreshGrid()
{
	long rc = 0;
	PSXRC PSXrc;
	CString str;
	COleVariant vReturn, v1, v2, vIndex, vParam;
	// Need to traverse the display elements from offset for up to MAXBUTTONNUM elements.
	for (long i = 0; i < v_Buttons.size(); i++)
	{
		CString csSubControl;
		csSubControl.Format(_T("%s[%d]"), csCtlName, i);
        if (v_Buttons[i].lStatus)
        {
		      vIndex = (long) i;
		      vParam = (long) UI::Pushed;
			  vParam.ChangeType(VT_I4);
		      //PSXrc = m_pGAPSX->SendCommand(csSubControl, UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
        }
        else
        {
		      vIndex = (long) i;
		      vParam = (long) UI::Normal;
			  vParam.ChangeType(VT_I4);
		      //PSXrc = m_pGAPSX->SendCommand(csSubControl, UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
        }
		PSXrc = m_pGAPSX->SetControlProperty(csSubControl, UI::PROPERTYSTATE, vParam);
	}
	PSXrc = m_pGAPSX->SetControlProperty( csCtlName, UI::PROPERTYREDRAW, VARIANT_TRUE );
	return rc;
}

long PhysicalButtonDisplay::RefreshButtons(void)
{
	long rc = 0;
	PSXRC PSXrc;

	USES_CONVERSION;

	if (!USSFutil.TestPSX())
	{
		return 0;
	}
	COleVariant v;
	CString str;
	// Need to traverse the display elements from offset for up to MAXBUTTONNUM elements.
	for (long i = 0; i < v_Buttons.size(); i++)
	{
		// Now show the elements starting with specified element offset (or 0).
		CString thisControl;
		if (v_Buttons[i].csID.GetLength() > 0)
		{
			thisControl = v_Buttons[i].csID;
		}
		else
		{
			continue;
		}
		if (v_Buttons[i].lStatus == 0)
		{
			v = (long)UI::Normal;
			PSXrc = m_pGAPSX->SetConfigProperty(thisControl, csCtxName, UI::PROPERTYSTATE, v);
			v_Buttons[i].lStatus = 1;
		}
	}
	return rc;
}
long PhysicalButtonDisplay::Select(CString csButton, CString &csReplace)
{
	long rc = 0;
	bool flag = false;
	csReplace = _T("");
	//Find the Button
	for (int i = 0; i < v_Buttons.size(); i++)
	{
		if (v_Buttons[i].csMetaData == csButton)
		{
			flag = true;
			// If button is already selected, then call DeSelect().
			if (v_Buttons[i].lStatus == 1)
			{
				v_Buttons[i].lStatus = 0;
				return 0;
			}
		}
	}
	if (!flag)
	{
		// Error, Button Specified Does Not Exist!
		trace(L5, _T("Internal Error in PhysicalButtonDisplay::Select(CString, CString&) : Button Not Found."));
	}
	else
	{
		for(long i = 0; i < v_Buttons.size(); i++)
		{
			if (v_Buttons[i].csMetaData == csButton)
			{
				v_Buttons[i].lStatus = 1;
			}
			else
			{
				if (v_Buttons[i].lStatus == 1)
				{
					// If these are radio buttons, then de-select!
					if (lType == 2)
					{
						v_Buttons[i].lStatus = 0;
						csReplace = v_Buttons[i].csID;
					}
				}
			}
		}
	}
	return rc;
}
long PhysicalButtonDisplay::Select(long lButton, long &lReplace)
{
  long rc = 0;
  bool flag = false;

  lReplace = -1;

  // If button is already selected, then call DeSelect().
  if (v_Buttons[lButton].lStatus == 1)
  {
	  return DeSelect(lButton);
  }

  // Is this a radio-button list?
  if (lType == 2)
  {//Radio
    for(long i = 0; i < v_Buttons.size(); i++)
    {
      if (v_Buttons[i].lStatus == 1)
      {
        if (!flag)
        {
          lReplace = i;
          v_Buttons[i].lStatus = 0;
          flag = true;
        }
        else
        {
          trace(L5, _T("Internal Error in PhysicalButtonDisplay::Select() Point A"));
        }
      }
    }
  }
  if (v_Buttons.size() > lButton)
  {
    v_Buttons[lButton].lStatus = 1;
  }
  else
  {
    trace(L5, _T("Internal Error in PhysicalButtonDisplay::Select() Point B"));
    rc = 1;
  }

  return rc;
}
long PhysicalButtonDisplay::DeSelect(CString csButton)
{
	long rc = 1;
	for (int i = 0; i < v_Buttons.size(); i++)
	{
		if (v_Buttons[i].csMetaData == csButton)
		{
			v_Buttons[i].lStatus = 0;
			rc = 0;
			break;
		}
	}
	if (rc)
	{
		trace(L5, _T("Internal Error in PhysicalButtonDisplay::DeSelect()"));
	}
	return rc;
}
long PhysicalButtonDisplay::DeSelect(long lButton)
{
	long rc = 0;
	if (v_Buttons.size() > lButton)
	{
		v_Buttons[lButton].lStatus = 0;
	}
	else
	{
		trace(L5, _T("Internal Error in PhysicalButtonDisplay::DeSelect()"));
		rc = 1;
	}
	return rc;
}
long PhysicalButtonDisplay::ScrollUp(void)
{
	long rc = 0;
	COleVariant vReturn, vScrollMode;
	vScrollMode = (long)UI::PageUp;
	PSXRC PSXrc = m_pGAPSX->SendCommand(csCtlName, UI::COMMANDSCROLL, vReturn, 1, vScrollMode);
	ShowScroll();
	return rc;
}

long PhysicalButtonDisplay::ScrollDown(void)
{
	long rc = 0;
	COleVariant vReturn, vScrollMode;
	vScrollMode = (long)UI::PageDown;
	PSXRC PSXrc = m_pGAPSX->SendCommand(csCtlName, UI::COMMANDSCROLL, vReturn, 1, vScrollMode);
	ShowScroll();
	return rc;
}

long PhysicalButtonDisplay::ShowScroll(void)
{
	long rc = 0;
	PSXRC PSXrc;
	COleVariant v, vReturn;
	CString csTemp;

	if (csScrollUp.GetLength() + csScrollDown.GetLength() > 0)
	{
		bool bCanScrollUp, bCanScrollDown;
		v = (long)UI::PageUp;
		if( PSX_SUCCESS == m_pGAPSX->SendCommand( csCtlName, UI::COMMANDCANSCROLL, vReturn, 1, v ) )
		{
			bCanScrollUp = vReturn.boolVal;		
		}	
		v = (long)UI::PageDown;
		if( PSX_SUCCESS == m_pGAPSX->SendCommand( csCtlName, UI::COMMANDCANSCROLL, vReturn, 1, v ) )
		{
			bCanScrollDown = vReturn.boolVal;		
		}
		if (bCanScrollUp)
		{
			csTemp.Format(_T("SCROLLUP:CONTROL=%s"), csCtlName);
			v = csTemp;
			//PSXrc = m_pGAPSX->SetControlProperty(csScrollUp, UI::PROPERTYDATA, v);
			//PSXrc = m_pGAPSX->SetConfigProperty(csScrollUp, csCtxName, UI::PROPERTYDATA, v);
			PSXrc = m_pGAPSX->SetConfigProperty(csScrollUp, UI::ALL_CONTEXTS, UI::PROPERTYDATA, v);
			v = (long) UI::Normal;
			PSXrc = m_pGAPSX->SetConfigProperty(csScrollUp, csCtxName, UI::PROPERTYSTATE, v);
			PSXrc = m_pGAPSX->SetConfigProperty(csScrollUp, csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
		}
		else
		{
			PSXrc = m_pGAPSX->SetConfigProperty(csScrollUp, csCtxName, UI::PROPERTYVISIBLE, VARIANT_FALSE);
		}
		if (bCanScrollDown)
		{
			csTemp.Format(_T("SCROLLDOWN:CONTROL=%s"), csCtlName);
			v = csTemp;
			//PSXrc = m_pGAPSX->SetControlProperty(csScrollDown, UI::PROPERTYDATA, v);
			//PSXrc = m_pGAPSX->SetConfigProperty(csScrollDown, csCtxName, UI::PROPERTYDATA, v);
			PSXrc = m_pGAPSX->SetConfigProperty(csScrollDown, UI::ALL_CONTEXTS, UI::PROPERTYDATA, v);
			v = (long) UI::Normal;
			PSXrc = m_pGAPSX->SetConfigProperty(csScrollDown, csCtxName, UI::PROPERTYSTATE, v);
			PSXrc = m_pGAPSX->SetConfigProperty(csScrollDown, csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
		}
		else
		{
			PSXrc = m_pGAPSX->SetConfigProperty(csScrollDown, csCtxName, UI::PROPERTYVISIBLE, VARIANT_FALSE);
		}
	}
	return rc;
}

long PhysicalButtonDisplay::FindElementIndex(long lIndex, CString& csID, CString& csMetaData, CString& csEvent)
{
	long rc = -1;
	long lButtonReplace = -1;

	// Check Elements for a match!
	for (long i = 0; i < v_Buttons.size(); i++)
	{
		if (v_Buttons[i].lElementIndex == lIndex)
		{
			Select(lIndex, lButtonReplace);
			Refresh();
			csID = v_Buttons[i].csID;
			csMetaData = v_Buttons[i].csMetaData;
			csEvent = v_Buttons[i].csEvent;
			rc = i;
			break;
		}
	}
	return rc;
}

long PhysicalButtonDisplay::FindElementID(CString& csID, CString& csMetaData, CString& csEvent)
{
	long rc = -1;
	long lButtonReplace = -1;

	// Check Elements for a match!
	for (long i = 0; i < v_Buttons.size(); i++)
	{
		if (v_Buttons[i].csID == csID)
		{
			csMetaData = v_Buttons[i].csMetaData;
			csEvent = v_Buttons[i].csEvent;
			rc = i;
			break;
		}
	}
	return rc;
}
void PhysicalButtonDisplay::PutToSafeArray(int nIndex, COleSafeArray& saArray, CString csString)
{
	USES_CONVERSION;
#ifndef UNICODE
        saArray.PutElement((long*)&nIndex, SysAllocString( T2OLE( csString ) ) );
#else
        saArray.PutElement((long*)&nIndex, SysAllocString(  csString ) );
#endif
}
/////////////////////////////////
// DisplayShop Methods!
/////////////////////////////////
DisplayShop::DisplayShop()
{
	lElementSerial = 0;
}

long DisplayShop::ClearDefinition()
{
	// Clear the myDisplayShop Structures.
	for (long i = 0;i < v_Group.size(); i++)
	{
		for (long j = 0;i < v_Group[i].v_Control.size(); j++)
		{
			v_Group[i].v_Control[j].v_Button.clear();
		}
		v_Group[i].v_Control.clear();
	}
	v_Group.clear();
	return 0;
}

long DisplayShop::LoadDefinition(CString filename)
{
	CString csTemp;

	// First, clear any existing definition.
	ClearDefinition();

	// Interrogates XML File
	CString csXMLFileName = co.csConfigFilePath + _T("\\") + XML_DISPLAYSHOPFILE;

	MSXML2::IXMLDOMDocumentPtr spDoc( __uuidof( MSXML2::DOMDocument60 ) );
	
	//Load PickList XML config file
	if( spDoc->load( (LPCTSTR)csXMLFileName ) != VARIANT_TRUE )
	{
		trace(L5, _T("Load Display Shop XML File Failed"));
		return FALSE;
	}

	// Parse the file into the DisplayShop classes.
	CString csDefGraphics = co.csStateDefaultProduceBMPName;
	int nIndex = csDefGraphics.Find(_T("."));
	CString csGraphicName = csDefGraphics.Left(nIndex-4);
	CString csGraphicExt = csDefGraphics.Right(csDefGraphics.GetLength() - nIndex);
 			
	MSXML2::IXMLDOMNodeListPtr spNodeList, spSubNodeListA, spSubNodeListB, spSubNodeListC;
	MSXML2::IXMLDOMNodePtr spNode, spSubNodeA, spSubNodeB, spSubNodeC, spItem, spSubItemA, spSubItemB, spSubItemC;

	// Get the Store Information.
	spNode = spDoc->selectSingleNode(XML_ROOT);
	// Initialize all data members.
	csName = _T("");
	csURL = _T("");
  
	//Process <URL>
	spItem = spNode->selectSingleNode( XML_ROOT_URL );
	if (spItem != NULL)
	{
		csTemp = LPCTSTR(spItem->text);
		csURL = csTemp;
	}

	//Process Name Attribute
	spItem = spNode->attributes->getNamedItem( XML_NAME );
	if( spItem != NULL && spItem->text.length() )
	{
		csTemp = LPCTSTR(spItem->text);
		csName = csTemp;
	}

	// Get the Display Group Information.
	spNodeList = spDoc->selectNodes(XML_GROUP);

	while( ( spNode = spNodeList->nextNode() ) )
	{
		//long lButtonCount = 1;
		// Initialize Structure (class).
		myDisplayGroupInfo.csID = _T("");
		myDisplayGroupInfo.csDescription = _T("");
		myDisplayGroupInfo.v_Control.clear();

		//Process <IsActive>
		spItem = spNode->selectSingleNode( XML_IS_ACTIVE );
		if (spItem != NULL)
		{
			csTemp = LPCTSTR(spItem->text);
			if (csTemp == _T("false"))
			{
				continue;   // Component Set to Inactive!
			}
		}

		//Process ID Attribute
		spItem = spNode->attributes->getNamedItem( XML_GROUP_ID );
		if( spItem != NULL && spItem->text.length() )
		{
			csTemp = LPCTSTR(spItem->text);
			myDisplayGroupInfo.csID = csTemp;
		}

		//Process <ID>
		spItem = spNode->selectSingleNode( XML_GROUP_ID );
		if (spItem != NULL)
		{
			csTemp = LPCTSTR(spItem->text);
			myDisplayGroupInfo.csID = csTemp;
		}

		// Skip if ID is not specified.
		if (myDisplayGroupInfo.csID.GetLength() == 0)
		{
			continue;
		}

		//Process <Description>
		spItem = spNode->selectSingleNode( XML_GROUP_DESC );
		if (spItem != NULL)
		{
			csTemp = LPCTSTR(spItem->text);
			myDisplayGroupInfo.csDescription = csTemp;
		}

		// Need to do Buttons for this Group!
		myDisplayGroupInfo.v_Control.clear();

		spSubNodeListA = spNode->selectNodes(XML_GROUP_CONTROLS);
		while( ( spSubNodeA = spSubNodeListA->nextNode() ) )
		{
			long lButtonCount = 1;
			myDisplayControlInfo.csID = _T("");
			myDisplayControlInfo.csType = _T("");
			myDisplayControlInfo.csEvent = _T("");
			myDisplayControlInfo.csSize = _T("");
			myDisplayControlInfo.csMaxButtons = _T("16");
			myDisplayControlInfo.csButtonType = _T("");
			myDisplayControlInfo.csScrollUp = _T("");
			myDisplayControlInfo.csScrollDown = _T("");
			myDisplayControlInfo.v_Button.clear();

			//Process <IsActive>
			spItem = spSubNodeA->selectSingleNode( XML_IS_ACTIVE );
			if (spItem != NULL)
			{
				csTemp = LPCTSTR(spItem->text);
				if (csTemp == _T("false"))
				{
					continue;   // Component Set to Inactive!
				}
			}

			//Process ID Attribute
			spItem = spSubNodeA->attributes->getNamedItem( XML_GROUP_CONTROL_ID );
			if( spItem != NULL && spItem->text.length() )
			{
				csTemp = LPCTSTR(spItem->text);
				myDisplayControlInfo.csID = csTemp;
			}

			//Process <ID>
			spItem = spSubNodeA->selectSingleNode( XML_GROUP_CONTROL_ID );
			if (spItem != NULL)
			{
				csTemp = LPCTSTR(spItem->text);
				myDisplayControlInfo.csID = csTemp;
			}
			
			// Skip if ID is not specified.
			if (myDisplayControlInfo.csID.GetLength() == 0)
			{
				continue;
			}

			//Process Type Attribute
			spItem = spSubNodeA->attributes->getNamedItem( XML_GROUP_CONTROL_TYPE );
			if( spItem != NULL && spItem->text.length() )
			{
				csTemp = LPCTSTR(spItem->text);
				if (!csTemp.Mid(0,1).CompareNoCase(_T("G")))
				{
					myDisplayControlInfo.csType = _T("Grid");
				}
				else if (!csTemp.Mid(0,1).CompareNoCase(_T("B")))
				{
					myDisplayControlInfo.csType = _T("Button");
				}
			}

			//Process <Type>
			spItem = spSubNodeA->selectSingleNode( XML_GROUP_CONTROL_TYPE );
			if (spItem != NULL)
			{
				csTemp = LPCTSTR(spItem->text);
				if (!csTemp.Mid(0,1).CompareNoCase(_T("G")))
				{
					myDisplayControlInfo.csType = _T("Grid");
				}
				else if (!csTemp.Mid(0,1).CompareNoCase(_T("B")))
				{
					myDisplayControlInfo.csType = _T("Button");
				}
			}
			
			// Skip if ID is not specified.
			if (myDisplayControlInfo.csType.GetLength() == 0)
			{
				continue;
			}

			//Process <ButtonType>
			spItem = spSubNodeA->selectSingleNode( XML_GROUP_CONTROL_BUTTONTYPE );
			if (spItem != NULL)
			{
				csTemp = LPCTSTR(spItem->text);
				myDisplayControlInfo.csButtonType = csTemp;
			}

			//Process <ScrollUp>
			spItem = spSubNodeA->selectSingleNode( XML_GROUP_CONTROL_SCROLLUP );
			if (spItem != NULL)
			{
				csTemp = LPCTSTR(spItem->text);
				myDisplayControlInfo.csScrollUp = csTemp;
			}

			//Process <ScrollDown>
			spItem = spSubNodeA->selectSingleNode( XML_GROUP_CONTROL_SCROLLDOWN );
			if (spItem != NULL)
			{
				csTemp = LPCTSTR(spItem->text);
				myDisplayControlInfo.csScrollDown = csTemp;
			}

			//Process <Event>
			spItem = spSubNodeA->selectSingleNode( XML_GROUP_CONTROL_EVENT );
			if (spItem != NULL)
			{
				csTemp = LPCTSTR(spItem->text);
				myDisplayControlInfo.csEvent = csTemp;
			}

			//Process <Size>
			spItem = spSubNodeA->selectSingleNode( XML_GROUP_CONTROL_SIZE );
			if (spItem != NULL)
			{
				csTemp = LPCTSTR(spItem->text);
				myDisplayControlInfo.csSize = csTemp;
			}

			//Process <MaxButtons>
			spItem = spSubNodeA->selectSingleNode( XML_GROUP_CONTROL_MAXBUTTONS );
			if (spItem != NULL)
			{
				csTemp = LPCTSTR(spItem->text);
				myDisplayControlInfo.csMaxButtons = csTemp;
			}

			long lRepeatCount = 0;
			spSubNodeListB = spSubNodeA->selectNodes(XML_GROUP_CONTROL_BUTTONS);
			while( ( spSubNodeB = spSubNodeListB->nextNode() ) )
			{
				// Initialize Button Info!
				CString csIDTemplate = _T("");
				CString csPictureTemplate = _T("");
				CString csLogoTemplate = _T("");
				CString csMetaDataTemplate = _T("");
				CString csEventTemplate = _T("");
				myDisplayButtonInfo.bSelected = false;
				CString csEnabledTemplate = _T("true");
				CString csStatusTemplate = _T("normal");
				lRepeatCount = 1;
				myDisplayButtonInfo.v_Components.clear();
				vector<DisplayComponentInfo> v_CompStage;
				v_CompStage.clear();

				//Process <IsActive>
				spItem = spSubNodeB->selectSingleNode( XML_IS_ACTIVE );
				if (spItem != NULL)
				{
					csTemp = LPCTSTR(spItem->text);
					if (csTemp == _T("false"))
					{
						continue;   // Component Set to Inactive!
					}
				}

				//Process ID Attribute
				spItem = spSubNodeB->attributes->getNamedItem( XML_GROUP_CONTROL_BUTTON_ID );
				if( spItem != NULL && spItem->text.length() )
				{
					csIDTemplate = LPCTSTR(spItem->text);
				}

				//Process <ID>
				spItem = spSubNodeB->selectSingleNode( XML_GROUP_CONTROL_BUTTON_ID );
				if (spItem != NULL)
				{
					csIDTemplate = LPCTSTR(spItem->text);
				}

				// Skip if ID is not specified.
				if (csIDTemplate.GetLength() == 0)
				{
					continue;
				}

				//Process <Picture>
				spItem = spSubNodeB->selectSingleNode( XML_GROUP_CONTROL_BUTTON_PICTURE );
				if (spItem != NULL)
				{
					csTemp = LPCTSTR(spItem->text);
					csPictureTemplate = csTemp;
				}

				//Process <Logo>
				spItem = spSubNodeB->selectSingleNode( XML_GROUP_CONTROL_BUTTON_LOGO );
				if (spItem != NULL)
				{
					csTemp = LPCTSTR(spItem->text);
					csLogoTemplate = csTemp;
				}

				//Process <MetaData>
				spItem = spSubNodeB->selectSingleNode( XML_GROUP_CONTROL_BUTTON_METADATA );
				if (spItem != NULL)
				{
					csTemp = LPCTSTR(spItem->text);
					csMetaDataTemplate = csTemp;
				}

				//Process <Event>
				spItem = spSubNodeB->selectSingleNode( XML_GROUP_CONTROL_BUTTON_EVENT );
				if (spItem != NULL)
				{
					csTemp = LPCTSTR(spItem->text);
					csEventTemplate = csTemp;
				}

				spItem = spSubNodeB->selectSingleNode( XML_GROUP_CONTROL_BUTTON_ENABLED );
				if (spItem != NULL)
				{
					csTemp = LPCTSTR(spItem->text);
					csEnabledTemplate = csTemp;
				}

				spItem = spSubNodeB->selectSingleNode( XML_GROUP_CONTROL_BUTTON_STATUS );
				if (spItem != NULL)
				{
					csTemp = LPCTSTR(spItem->text);
					csStatusTemplate = csTemp;
				}

				spItem = spSubNodeB->selectSingleNode( XML_GROUP_CONTROL_BUTTON_REPEAT );
				if (spItem != NULL)
				{
					csTemp = LPCTSTR(spItem->text);
					lRepeatCount = _ttol(csTemp);
				}

				// Need to get the text (if any).
				spSubNodeListC = spSubNodeB->selectNodes(XML_GROUP_CONTROL_BUTTON_COMPONENTS);
				while( ( spSubNodeC = spSubNodeListC->nextNode() ) )
				{
					// Initialize Text Info!
					CString csIDTemplate = _T("");
					CString csValueTemplate = _T("");

					//Process ID Attribute
					spItem = spSubNodeC->attributes->getNamedItem( XML_GROUP_CONTROL_BUTTON_COMPONENTS_ID );
					if( spItem != NULL && spItem->text.length() )
					{
						myDisplayComponentInfo.csID = LPCTSTR(spItem->text);
					}

					//Process <ID>
					spItem = spSubNodeC->selectSingleNode( XML_GROUP_CONTROL_BUTTON_COMPONENTS_ID );
					if (spItem != NULL)
					{
						myDisplayComponentInfo.csID = LPCTSTR(spItem->text);
					}

					// Skip if ID is not specified.
					if (myDisplayComponentInfo.csID.GetLength() == 0)
					{
						continue;
					}

					//Process Type Attribute
					spItem = spSubNodeC->attributes->getNamedItem( XML_GROUP_CONTROL_BUTTON_COMPONENTS_TYPE );
					if( spItem != NULL && spItem->text.length() )
					{
						myDisplayComponentInfo.csType = LPCTSTR(spItem->text);
					}

					//Process <Type>
					spItem = spSubNodeC->selectSingleNode( XML_GROUP_CONTROL_BUTTON_COMPONENTS_TYPE );
					if (spItem != NULL)
					{
						myDisplayComponentInfo.csType = LPCTSTR(spItem->text);
					}

					// Skip if ID is not specified.
					if (myDisplayComponentInfo.csType.GetLength() == 0)
					{
						myDisplayComponentInfo.csType = _T("Text");
					}

					//Process <Value>
					spItem = spSubNodeC->selectSingleNode( XML_GROUP_CONTROL_BUTTON_COMPONENTS_VALUE );
					if (spItem != NULL)
					{
						myDisplayComponentInfo.csValue = LPCTSTR(spItem->text);
					}

					//myDisplayButtonInfo.v_Components.push_back(myDisplayComponentInfo);
					v_CompStage.push_back(myDisplayComponentInfo);
				}

				CString csSubstitution;
				for (long i = 0; i < lRepeatCount; i++)
				{	
					// Do Substitutions.
					csSubstitution.Format(_T("%d"), lButtonCount);
					myDisplayButtonInfo.csID = csIDTemplate;
					myDisplayButtonInfo.csID.Replace(_T("$#"), csSubstitution);
					myDisplayButtonInfo.csPicture = csPictureTemplate;
					myDisplayButtonInfo.csPicture.Replace(_T("$#"), csSubstitution);
					myDisplayButtonInfo.csLogo = csLogoTemplate;
					myDisplayButtonInfo.csLogo.Replace(_T("$#"), csSubstitution);
					myDisplayButtonInfo.csMetaData = csMetaDataTemplate;
					myDisplayButtonInfo.csMetaData.Replace(_T("$#"), csSubstitution);
					myDisplayButtonInfo.csEvent = csEventTemplate;
					myDisplayButtonInfo.csEvent.Replace(_T("$#"), csSubstitution);
					myDisplayButtonInfo.csEnabled = csEnabledTemplate;
					myDisplayButtonInfo.csEnabled.Replace(_T("$#"), csSubstitution);
					myDisplayButtonInfo.csStatus = csStatusTemplate;
					myDisplayButtonInfo.csStatus.Replace(_T("$#"), csSubstitution);

					// Need to repeat the text (if any).
					for (long j = 0; j < v_CompStage.size(); j++)
					{
						// Do the templates for text!
						myDisplayComponentInfo = v_CompStage[j];
						myDisplayComponentInfo.csID.Replace(_T("$#"), csSubstitution);
						myDisplayComponentInfo.csType.Replace(_T("$#"), csSubstitution);
						myDisplayComponentInfo.csValue.Replace(_T("$#"), csSubstitution);
						myDisplayButtonInfo.v_Components.push_back(myDisplayComponentInfo);
					}
					lButtonCount++;
					myDisplayControlInfo.v_Button.push_back(myDisplayButtonInfo);
				}
			}
			if (myDisplayControlInfo.csSize == _T(""))
			{// Substitute the size we counted.
				CString csVariable;
				csVariable.Format(_T("%d"), lButtonCount - 1);
				myDisplayControlInfo.csSize = csVariable;
			}
			myDisplayGroupInfo.v_Control.push_back(myDisplayControlInfo);
		}
		v_Group.push_back(myDisplayGroupInfo);
  }
  // Get the Display Group Information.
  spNodeList = spDoc->selectNodes(XML_SYMBOL);

  while( ( spNode = spNodeList->nextNode() ) )
  {
	// Initialize Structure (class).
	myDisplaySymbolInfo.csName = _T("");
	myDisplaySymbolInfo.csValue = _T("");

	//Process <IsActive>
	spItem = spNode->selectSingleNode( XML_IS_ACTIVE );
	if (spItem != NULL)
	{
		csTemp = LPCTSTR(spItem->text);
		if (csTemp == _T("false"))
		{
			continue;   // Component Set to Inactive!
		}
	}

	//Process <ID>
	spItem = spNode->selectSingleNode( XML_SYMBOL_NAME );
	if (spItem != NULL)
	{
		csName = LPCTSTR(spItem->text);
		csName.MakeUpper();
		csTemp = _T("USSF_") + csName;
		myDisplaySymbolInfo.csName = csTemp;
	}

	//Process <Name>
	spItem = spNode->selectSingleNode( XML_SYMBOL_VALUE );
	if (spItem != NULL)
	{
		myDisplaySymbolInfo.csValue = LPCTSTR(spItem->text);
	}
	USSFutil.SetTransactionVariable(myDisplaySymbolInfo.csName, myDisplaySymbolInfo.csValue);
    SymbolInfo.push_back(myDisplaySymbolInfo);

	// <Language> Sub-node
	spSubNodeListB = spNode->selectNodes(XML_SYMBOL_LANGUAGE);
	while( ( spSubNodeB = spSubNodeListB->nextNode() ) )
	{
		CString thisLCID;
		// First Check the LCID Attribute!
		spItem = spSubNodeB->attributes->getNamedItem( XML_SYMBOL_LANGUAGE_ID );
		if( spItem != NULL && spItem->text.length() )
		{
			// Check the category.
			thisLCID = LPCTSTR(spItem->text);
		}

		//Process <ID>
		spItem = spSubNodeB->selectSingleNode( XML_SYMBOL_LANGUAGE_ID );
		if (spItem != NULL)
		{
			thisLCID = LPCTSTR(spItem->text);
		}

		if (thisLCID.GetLength() == 0)
		{
			continue;
		}

		//Process <Value>
		spItem = spSubNodeB->selectSingleNode( XML_SYMBOL_LANGUAGE_VALUE );
		if (spItem != NULL)
		{
			myDisplaySymbolInfo.csValue = LPCTSTR(spItem->text);
			csTemp = _T("USSF_") + thisLCID + _T("_") + csName;
			myDisplaySymbolInfo.csName = csTemp;
			USSFutil.SetTransactionVariable(myDisplaySymbolInfo.csName, myDisplaySymbolInfo.csValue);
		    SymbolInfo.push_back(myDisplaySymbolInfo);
		}
	}
  }
  return 0;
}

long DisplayShop::InitializeDisplayElements(CString csContext, CString csDisplayGroup)
{
	long rc = 0;
	for(long i = 0; i < DisplayManager.size(); i++)
	{
		DisplayManager[i].Reset();
	}
	DisplayManager.clear();
	if (csDisplayGroup.GetLength() > 0)
	{
		rc = LoadChoices(csDisplayGroup, csContext);
		for (long j = 0; j < DisplayManager.size(); j++)
		{
			rc = DisplayManager[j].Display();
		}
	}
	return rc;
}

long DisplayShop::InvalidateDisplayElements()
{
	for(long i = 0; i < DisplayManager.size(); i++)
	{
		DisplayManager[i].Reset();
	}
	return 0;
}

long DisplayShop::LoadChoices(CString groupID, CString contextID)
{
  trace(L7, _T("Loading Choices for Display Group ID=<%s>; Context=<%s>."), groupID, contextID);

  CString csTemp;
  long rc = 0, count = 0;
  PhysicalButtonDisplay GenericControl;
  DisplayElement thisDisplayElement;
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  // Due to the inflexibility of PSThread, it is necessary to play games to get a list selection to 
  // work properly via PSListLookup. The Data MUST be numeric. As such, I use the index into the 
  // ingredient structure in myDisplayShop. It is an index into an STL vector. The actual data
  // element can then be looked up indirectly. This is a CString and is a key to the ingredient object.
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  // Find the Display ID.
  for (long i = 0; i < v_Group.size(); i++)
  {
    if (v_Group[i].csID == groupID)
    {
      // Found it.
	  for (long j = 0; j < v_Group[i].v_Control.size(); j++)
	  {	
		if (!v_Group[i].v_Control[j].csType.CompareNoCase(_T("Grid")))
		{
			GenericControl.lType = 1;	// Grid (versus Buttons).
		}
		else
		{
			GenericControl.lType = 0;	// Buttons (versus Grid).
		}
		GenericControl.csCtxName = contextID;
		GenericControl.csCtlName = _T("");
		GenericControl.csComponentID = groupID;
		GenericControl.lMaxButtons = 16;
		GenericControl.csScrollUp = _T("");
		GenericControl.csScrollDown = _T("");
		GenericControl.v_Buttons.clear();
		GenericControl.bValid = false;

		// Size
		csTemp = v_Group[i].v_Control[j].csSize;
		USSFutil.ResolveSymbol(csTemp, csTemp);
		long lGroupSize = _ttol(csTemp);

		// ButtonType
		csTemp = v_Group[i].v_Control[j].csButtonType;
		USSFutil.ResolveSymbol(csTemp, csTemp, _T(""));
		if (!(csTemp.CompareNoCase(_T("radio"))))
		{
			if (GenericControl.lType == 1)		// If this is not a Grid, then this has no effect.
			{
				GenericControl.lType = 2;		// Radio.
			}
		}

		// MaxButtons
		csTemp = v_Group[i].v_Control[j].csMaxButtons;
		GenericControl.lMaxButtons = _ttol(csTemp);

		// Control
		GenericControl.csCtlName = v_Group[i].v_Control[j].csID;

		// Scroll
		GenericControl.csScrollUp = v_Group[i].v_Control[j].csScrollUp;
		GenericControl.csScrollDown = v_Group[i].v_Control[j].csScrollDown;
		count = 0;
		// Now show the buttons starting with specified element offset (or 0).
		for (long k = 0; k < v_Group[i].v_Control[j].v_Button.size(); k++)
		{
			if (count++ >= lGroupSize)
			{
				break;
			}
			// Initialize DisplayElement!
			thisDisplayElement.csControl = v_Group[i].v_Control[j].csID;
			thisDisplayElement.csEnabled = _T("false");
			thisDisplayElement.csPicture = _T("");
			thisDisplayElement.csLogo = _T("");
			thisDisplayElement.csID = _T("");
			thisDisplayElement.csMetaData = _T("");
			thisDisplayElement.csEvent = v_Group[i].v_Control[j].csEvent;
			//thisDisplayElement.csText1 = _T("NOVALUE");
			//thisDisplayElement.csText2 = _T("NOVALUE");
			thisDisplayElement.lElementIndex = -1;
			thisDisplayElement.lStatus = -1;
			thisDisplayElement.v_Components.clear();

			csTemp = v_Group[i].v_Control[j].v_Button[k].csID;
			USSFutil.ResolveSymbol(csTemp, csTemp, _T(""));
			thisDisplayElement.csID = csTemp;

			csTemp = v_Group[i].v_Control[j].v_Button[k].csPicture;
			USSFutil.ResolveSymbol(csTemp, csTemp, _T(""));
			thisDisplayElement.csPicture = csTemp;

			csTemp = v_Group[i].v_Control[j].v_Button[k].csLogo;
			USSFutil.ResolveSymbol(csTemp, csTemp, _T(""));
			thisDisplayElement.csLogo = csTemp;

			csTemp = v_Group[i].v_Control[j].v_Button[k].csEnabled;
			USSFutil.ResolveSymbol(csTemp, csTemp, _T("true"));
			thisDisplayElement.csEnabled = csTemp;

			csTemp = v_Group[i].v_Control[j].v_Button[k].csMetaData;
			USSFutil.ResolveSymbol(csTemp, csTemp, _T(""));
			thisDisplayElement.csMetaData = csTemp;

			csTemp = v_Group[i].v_Control[j].v_Button[k].csEvent;
			USSFutil.ResolveSymbol(csTemp, csTemp, _T(""));
			thisDisplayElement.csEvent = csTemp;

			// Need to provide pass-through mechanism for status as well!
			csTemp = v_Group[i].v_Control[j].v_Button[k].csStatus;
			USSFutil.ResolveSymbol(csTemp, csTemp, _T(""));

			if (!csTemp.CompareNoCase(_T("normal")))
			{
				thisDisplayElement.lStatus = 0;
			}
			else if (!csTemp.CompareNoCase(_T("pushed")))
			{
				thisDisplayElement.lStatus = 1;
			}
			else
			{
				thisDisplayElement.lStatus = 0;
			}
			thisDisplayElement.lElementIndex = lElementSerial++;

			// Add the button components (like text, etc.).
			for (long l = 0; l < v_Group[i].v_Control[j].v_Button[k].v_Components.size(); l++)
			{
				myDisplayComponentInfo = v_Group[i].v_Control[j].v_Button[k].v_Components[l];
				csTemp = myDisplayComponentInfo.csValue;
				USSFutil.ResolveSymbol(myDisplayComponentInfo.csValue, myDisplayComponentInfo.csValue, _T(""));
				thisDisplayElement.v_Components.push_back(myDisplayComponentInfo);
			}

			GenericControl.bValid = true;
			GenericControl.v_Buttons.push_back(thisDisplayElement);
		}
	  DisplayManager.push_back(GenericControl);
	  // Reset GenericControl to be a selection type (for OverflowManager).
	  GenericControl.lType = 1;		// Overflow Controls Must Be Selection!
	  }
    }
  }

  return count;
}

long DisplayShop::GetButtonInfo(CString groupID, CString buttonID, CString& Event, CString& POSData)
{
	long rc = 0;

	Event = _T("");
	POSData = _T("");
	// Need to search the vector for the group and return event and POSData.
	// First, Find the Display Group!
	for (long i = 0; i < v_Group.size(); i++)
	{
		if (v_Group[i].csID == groupID)
		{
			rc = 1;
			// Found The Display Group (SelectGroup/ControlGroup)!
			// Now find first occurrence of the button (by ID) from only the first control.
			// Can only be in the first control (we don't really know the originating control.
			// The assumption is that it is the first control.
			for (long j = 0; j < v_Group[i].v_Control.size(); j++)
			//if (v_Group[i].v_Control.size() > 0)
			{
				//long j = 0;
				Event = v_Group[i].v_Control[j].csEvent;	// Default Event for Control.
				for (long k = 0; k < v_Group[i].v_Control[j].v_Button.size(); k++)
				{
					if (v_Group[i].v_Control[j].v_Button[k].csID == buttonID)
					{
						// Found it, set reference parms.
						Event = v_Group[i].v_Control[j].v_Button[k].csEvent;
						POSData = v_Group[i].v_Control[j].v_Button[k].csMetaData;
						USSFutil.ResolveSymbol(Event, Event, _T(""));
						USSFutil.ResolveSymbol(POSData, POSData, _T(""));
						return(2);
					}
				}
			}
		}
	}
	return rc;
}

long DisplayShop::StoreSymbols()
{
  long rc = 0;
  CString csName, csValue;

  // Iterate though the symbols (constants) and store them as transaction variables.
  for (long lIndex = 0; lIndex < SymbolInfo.size(); lIndex++)
  {
	csName = SymbolInfo[lIndex].csName;
	csValue = SymbolInfo[lIndex].csValue;
	USSFutil.SetTransactionVariable(csName, csValue);
  }
  return rc;
}

long DisplayShop::HandleListSelection(CString csIndex, CString& csEvent)
{
	long rc = 0;
	CString csID;
	CString csData;
	
	// This determines what needs to happen when a selection button is pressed.
	// The index is supplied.
	long lIndex = _ttol(csIndex);

	// Search for the index within the group/control/button structure.
	// Need to search the vector for the group and return event and POSData.
	// First, Find the Display Group!

	// Check the DisplayManager.
	for (long i = 0; i < DisplayManager.size(); i++)
	{
		rc = DisplayManager[i].FindElementIndex(lIndex, csID, csData, csEvent);
		if (rc >= 0)
		{
			USSFutil.SetMetaData(csData);
			USSFutil.SetTransactionVariable(_T("USSF_CHOICEID"), csID);
		}
	}
	// If no event, use default!
	if (csEvent.GetLength() == 0)
	{
		csEvent = _T("UnknownSelect");
	}
	return rc;
}

long DisplayShop::HandleControlButton(CString csID, CString& csEvent)
{
	long rc = -1;
	CString csData;

	csEvent = _T("");	
	USSFutil.SetTransactionVariable(_T("USSF_CHOICEID"), csID);
	USSFutil.SetMetaData(_T(""));
	// Make sure the DisplayManager is valid (bValid).
	for (long i = 0; i < DisplayManager.size(); i++)
	{
		if (DisplayManager[i].bValid)
		{
			// Check the SelectionManager, the ControlManager.
			rc = DisplayManager[i].FindElementID(csID, csData, csEvent);
			if (rc >= 0)
			{
				USSFutil.SetMetaData(csData);
			}
		}
	}
	// If no event, use default!
	if (csEvent.GetLength() == 0)
	{
		csEvent = csID;
	}
	return rc;
}
long DisplayShop::ScrollUp(CString csCommand)
{
	long rc = -1;
	CString csTemp;
	//Parse csCommand to determine correct control.
	// SCROLLUP:CONTROL=<Control>
	rc = csCommand.Find(_T("CONTROL="));
	if (rc >= 0)
	{
		csTemp = csCommand.Mid(rc + 8);
	}
	for (long i = 0; i < DisplayManager.size(); i++)
	{
		if (DisplayManager[i].bValid)
		{
			if (csTemp == DisplayManager[i].csCtlName)
			{
				if (DisplayManager[i].lType > 0)
				{
					// Issue command to PSX to scroll up!
					DisplayManager[i].ScrollUp();
				}
			}
		}
	}
	return rc;
}
long DisplayShop::ScrollDown(CString csCommand)
{
	long rc = -1;
	CString csTemp;
	//Parse csCommand to determine correct control.
	// SCROLLDOWN:CONTROL=<Control>
	rc = csCommand.Find(_T("CONTROL="));
	if (rc >= 0)
	{
		csTemp = csCommand.Mid(rc + 8);
	}
	for (long i = 0; i < DisplayManager.size(); i++)
	{
		if (DisplayManager[i].bValid)
		{
			if (csTemp == DisplayManager[i].csCtlName)
			{
				if (DisplayManager[i].lType > 0)
				{
					// Issue command to PSX to scroll up!
					DisplayManager[i].ScrollDown();
				}
			}
		}
	}
	return rc;
}
long DisplayShop::Display()
{
	long rc = -1;
	for (long i = 0; i < DisplayManager.size(); i++)
	{
		if (DisplayManager[i].bValid)
		{
			rc = DisplayManager[i].Display();
		}
	}
	return rc;
}
long DisplayShop::Refresh()
{
	long rc = -1;
	for (long i = 0; i < DisplayManager.size(); i++)
	{
		if (DisplayManager[i].bValid)
		{
			rc = DisplayManager[i].Refresh();
		}
	}
	return rc;
}
long DisplayShop::Select(CString csControl, CString csID, CString &csReplace)
{
	long rc = -1;
	for (long i = 0; i < DisplayManager.size(); i++)
	{
		if (DisplayManager[i].bValid)
		{
			//if (csControl == DisplayManager[i].csCtlName)
			if ((csControl == _T("*")) || (csControl == DisplayManager[i].csCtlName))
			{
				if (DisplayManager[i].lType > 0)
				{
					// Issue command to PSX to scroll up!
					DisplayManager[i].Select(csID, csReplace);
				}
			}
		}
	}
	return rc;
}
long DisplayShop::DeSelect(CString csControl, CString csID)
{
	long rc = -1;
	for (long i = 0; i < DisplayManager.size(); i++)
	{
		if (DisplayManager[i].bValid)
		{
			if ((csControl == _T("*")) || (csControl == DisplayManager[i].csCtlName))
			{
				if (DisplayManager[i].lType > 0)
				{
					// Issue command to PSX to scroll up!
					DisplayManager[i].DeSelect(csID);
				}
			}
		}
	}
	return rc;
}