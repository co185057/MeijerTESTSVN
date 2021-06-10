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
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <afxtempl.h>               // MS MFC template header
#include <afxpriv.h>                // for USES_CONVERSION

// BeginCodeLocale
#include "Common.h"					// Common includes

#include "USSFProcedures.h"
#include "SMStateUSSFMacros.h"
#include "USSFCommon.h"

#define COMP_ID   ID_PS
#define T_ID      _T("USSFProcedures")

CPSX*  m_pGAPSX = NULL;          // NewUI - pointer to PSX object
DisplayShop *myDisplayShop = NULL;

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
	CString csSource = v_Buttons[i].csControl, csTarget = csControl;
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

  // Need to decide what we are displaying.
  // Selection Buttons OR Control Buttons.
  // Depends upon the type.
  switch(lType)
  {
  case 0:
	  {
		// Control Buttons!
		rc = DisplayControlButtons();
	  }
	  break;
  case 1:
  default:
	  {
		// Selection Buttons!
		//lEnabledLimit = 0;
		rc = DisplayPrimarySelection(csCtlNameA);
		if (rc == 0)
		{
		  if (csCtlNameB != _T(""))
		  {
			rc = DisplaySecondarySelection(csCtlNameB);
			if (rc != 0)
			{
			  trace(L5, _T("Secondary Selection Display Failed."));
			}
		  }
          // Make them invisible, by default.
          m_pGAPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCtxName, UI::PROPERTYVISIBLE, VARIANT_FALSE);
          m_pGAPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCtxName, UI::PROPERTYVISIBLE, VARIANT_FALSE);
          if (lMaxButtons < v_Buttons.size())  // Should be compared against actual number of enabled buttons.
          //if (lMaxButtons < lEnabledLimit)  // Should be compared against actual number of enabled buttons.
		  {//SCROLLING ACTIVE!
           // Place Text!
	        COleVariant v;
	        v = _T("Back");
            m_pGAPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCtxName, UI::PROPERTYTEXTFORMAT, v);
	        v = _T("Next");
            m_pGAPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCtxName, UI::PROPERTYTEXTFORMAT, v);

           // Make them visible, then decide if disabled or normal.
            m_pGAPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
            m_pGAPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);

            if (lCurrentScroll > 0)
			{// DISPLAY LEFT SCROLL BUTTON!
              COleVariant v;
              v = (long) UI::Normal;
              m_pGAPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCtxName, UI::PROPERTYSTATE, v);
			}
            else
			{
              COleVariant v;
              v = (long) UI::Disabled;
              m_pGAPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCtxName, UI::PROPERTYSTATE, v);
			}

            if (lCurrentScroll + lMaxButtons < v_Buttons.size())
			{// DISPLAY RIGHT SCROLL BUTTON!
              COleVariant v;
              v = (long) UI::Normal;
              m_pGAPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCtxName, UI::PROPERTYSTATE, v);
			}
            else
			{
              COleVariant v;
              v = (long) UI::Disabled;
              m_pGAPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCtxName, UI::PROPERTYSTATE, v);
			}
		  }
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

long PhysicalButtonDisplay::DisplayPrimarySelection(CString csCtlName)
{
  long rc = 0;
  PSXRC PSXrc;

  USES_CONVERSION;

  long count = 0;
  long offset = 0;
	//long nButtonCount = MAXBUTTONNUM;

  if (!USSFutil.TestPSX())
  {
    return 0;
  }

  // reset the count to 0 to clear all previously set attributes
  COleVariant v = (long) 0;
  PSXrc = m_pGAPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
  PSXrc = m_pGAPSX->SetControlProperty(csCtlName, UI::PROPERTYBUTTONCOUNT, v );

  v = lMaxButtons;
  PSXrc = m_pGAPSX->SetControlProperty( csCtlName, UI::PROPERTYREDRAW, VARIANT_FALSE );

  PSXrc = m_pGAPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
  PSXrc = m_pGAPSX->SetControlProperty( csCtlName, UI::PROPERTYBUTTONCOUNT, v );

  CString str = _T("");
  COleSafeArray  saTextArray, saImageArray, saIDArray, saIndexArray, saStateArray;
  saTextArray.Create(VT_BSTR, 1, (DWORD*)&lMaxButtons);
  saImageArray.Create(VT_BSTR, 1, (DWORD*)&lMaxButtons);
  saIndexArray.Create(VT_VARIANT, 1, (DWORD*)&lMaxButtons);
  saStateArray.Create(VT_I4, 1, (DWORD*)&lMaxButtons);

  COleVariant vReturn, v1, v2, vParam, vIndex;

  // Need to traverse the display elements from offset for up to MAXBUTTONNUM elements.
  DisplayElementInfoIter = v_Buttons.begin();
  while (DisplayElementInfoIter != v_Buttons.end())
  {
    // Now show the elements starting with specified element offset (or 0).
    if (offset >= lCurrentScroll) 
    {
      // Max buttons reached?
      if (count < lMaxButtons)
      {
        if ((*DisplayElementInfoIter).lStatus)
        {
		      vIndex = (long) count;
		      //vParam = (long) UI::Highlighted;
		      vParam = (long) UI::Pushed;
		      //vParam = (long) UI::Disabled;
		      //vParam = (long) 2;
			  vParam.ChangeType(VT_I4);
		      PSXrc = m_pGAPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
          //m_pGAPSX->SetControlProperty(csCtlName, UI::PROPERTYSTATE, vParam);
		      //v = (long) UI::Highlighted;
		      //v = (long) UI::Disabled;
		      v = (long) UI::Pushed;
		      //v = (long) 2;
//#ifndef UNICODE
//          saStateArray.PutElement(&count, &v );
//#else
          saStateArray.PutElement(&count, &v );
//#endif
        }
        else
        {
		      vIndex = (long) count;
			  str = (*DisplayElementInfoIter).csEnabled;
			  USSFutil.ResolveSymbol(str, str, _T("false"));
			  if (!str.CompareNoCase(_T("true")))
			  {
				vParam = (long) UI::Normal;
				v = (long) UI::Normal;
				// Keep count of highest enabled button.
				//lEnabledLimit = count;
			  }
			  else
			  {
				vParam = (long) UI::Disabled;
				v = (long) UI::Disabled;

				//v2 = VARIANT_FALSE;
				PSXrc = m_pGAPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, vIndex, VARIANT_FALSE );
			  }

		      PSXrc = m_pGAPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
          //m_pGAPSX->SetControlProperty(csCtlName, UI::PROPERTYSTATE, vParam);
//#ifndef UNICODE
//          saStateArray.PutElement(&count, &v );
//#else
          saStateArray.PutElement(&count, &v );
//#endif
        }

        // Put it on the context!
		    str = (*DisplayElementInfoIter).csDesc;
			USSFutil.ResolveSymbol(str, str, _T("NOVALUE"));

#ifndef UNICODE
        saTextArray.PutElement(&count, SysAllocString( T2OLE( str ) ) );
#else
        saTextArray.PutElement(&count, SysAllocString(  str ) );
#endif

		    str = (*DisplayElementInfoIter).csGraphic;
			USSFutil.ResolveSymbol(str, str, _T(""));
#ifndef UNICODE
        saImageArray.PutElement(&count, SysAllocString( T2OLE( str ) ) );
#else
        saImageArray.PutElement(&count, SysAllocString(  str ) );
#endif

        v = (long) offset;	
        saIndexArray.PutElement(&count, &v );
        count++;
      }
    }
    offset++;
    DisplayElementInfoIter++;
  }

  // Get Rid of Extra Buttons!
  for(long  i = count; i < lMaxButtons; i++ )
  {
	// Reset all physical button positions.
	v1 = i;
	v2 = (long) UI::Disabled; // disable
	PSXrc = m_pGAPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONSTATE, vReturn, 2, v1, v2 );
			
	v2 = VARIANT_FALSE;
	PSXrc = m_pGAPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
  }

  v = saStateArray;
  PSXrc = m_pGAPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONSTATELIST, vReturn, 1, v );

  v = saTextArray;
  PSXrc = m_pGAPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );

  v = saImageArray;
  PSXrc = m_pGAPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONLOGOLIST, vReturn, 1, v );

  v = saIndexArray;
  PSXrc = m_pGAPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );

  PSXrc = m_pGAPSX->SetControlProperty( csCtlName, UI::PROPERTYREDRAW, VARIANT_TRUE );

  return rc;
}

long PhysicalButtonDisplay::DisplaySecondarySelection(CString csCtlName)
{
  long rc = 0;

  USES_CONVERSION;

  long count = 0;
  long offset = 0;
	//long nButtonCount = MAXBUTTONNUM;

  if (!USSFutil.TestPSX())
  {
    return 0;
  }

  // reset the count to 0 to clear all previously set attributes
  COleVariant v = (long) 0;
  m_pGAPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
	m_pGAPSX->SetControlProperty(csCtlName, UI::PROPERTYBUTTONCOUNT, v );

  v = lMaxButtons;
  m_pGAPSX->SetControlProperty( csCtlName, UI::PROPERTYREDRAW, VARIANT_FALSE );

  m_pGAPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
  m_pGAPSX->SetControlProperty( csCtlName, UI::PROPERTYBUTTONCOUNT, v );

	CString str = _T("");
  COleSafeArray  saTextArray, saImageArray, saIDArray, saIndexArray, saStateArray;
  saTextArray.Create(VT_BSTR, 1, (DWORD*)&lMaxButtons);
  saImageArray.Create(VT_BSTR, 1, (DWORD*)&lMaxButtons);
  saIndexArray.Create(VT_VARIANT, 1, (DWORD*)&lMaxButtons);
  saStateArray.Create(VT_VARIANT, 1, (DWORD*)&lMaxButtons);

  COleVariant vReturn, v1, v2, vParam, vIndex;

  // Need to traverse the display elements from offset for up to MAXBUTTONNUM elements.
  DisplayElementInfoIter = v_Buttons.begin();
  while (DisplayElementInfoIter != v_Buttons.end())
  {
    // Now show the base types starting with specified element offset (or 0).
    if (offset >= lCurrentScroll)
    {
      // Max buttons reached?
      if (count < lMaxButtons)
      {
        if ((*DisplayElementInfoIter).lStatus)
        {
		      vIndex = (long) count;
		      //vParam = (long) UI::Highlighted;
		      vParam = (long) UI::Pushed;
		      //m_pGAPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
          m_pGAPSX->SetControlProperty(csCtlName, UI::PROPERTYSTATE, vParam);
        }
        else
        {
		      vIndex = (long) count;
		      vParam = (long) UI::Normal;
          m_pGAPSX->SetControlProperty(csCtlName, UI::PROPERTYSTATE, vParam);
		      //m_pGAPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
        }

        //else
        {
		      vIndex = (long) count;
			  str = (*DisplayElementInfoIter).csEnabled;
			  USSFutil.ResolveSymbol(str, str, _T("false"));
			  if (!str.CompareNoCase(_T("true")))
			  {
				vParam = (long) UI::Normal;
				v = (long) UI::Normal;
				// Keep count of highest enabled button.
				//lEnabledLimit = count;
			  }
			  else
			  {
				vParam = (long) UI::Disabled;
				v = (long) UI::Disabled;

				//v2 = VARIANT_FALSE;
				m_pGAPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, vIndex, VARIANT_FALSE );
			  }

		      m_pGAPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
          //m_pGAPSX->SetControlProperty(csCtlName, UI::PROPERTYSTATE, vParam);
#ifndef UNICODE
          saStateArray.PutElement(&count, v );
#else
          saStateArray.PutElement(&count, v );
#endif
        }

        // Put it on the context!
		    str = (*DisplayElementInfoIter).csAmount;
			USSFutil.ResolveSymbol(str, str, _T("NOVALUE"));

#ifndef UNICODE
        saTextArray.PutElement(&count, SysAllocString( T2OLE( str ) ) );
#else
        saTextArray.PutElement(&count, SysAllocString(  str ) );
#endif

		    //str = (*DisplayElementInfoIter).csGraphic;
        //EXTERNALIZE
		    str = _T("c:\\scot\\image\\SafewayDeli\\10x7_Grpx_Ribbon.png");
#ifndef UNICODE
        saImageArray.PutElement(&count, SysAllocString( T2OLE( str ) ) );
#else
        saImageArray.PutElement(&count, SysAllocString(  str ) );
#endif

        v = (long) offset;	
        saIndexArray.PutElement(&count, &v );
        count++;
      }
    }
    offset++;
    DisplayElementInfoIter++;
  }

  // Get Rid of Extra Buttons!
  for(long  i = count; i < lMaxButtons; i++ )
	{
    // Reset all physical button positions.
    v1 = i;
	  v2 = (long) UI::Disabled; // disable
		m_pGAPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONSTATE, vReturn, 2, v1, v2 );
			
    v2 = VARIANT_FALSE;
		m_pGAPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
  }

  v = saStateArray;
  m_pGAPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONSTATELIST, vReturn, 1, v );

  v = saTextArray;
  m_pGAPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );

  v = saImageArray;
  m_pGAPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONLOGOLIST, vReturn, 1, v );
  //v = saIDArray;
  //m_pGAPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );

  v = saIndexArray;
  m_pGAPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );

  m_pGAPSX->SetControlProperty( csCtlName, UI::PROPERTYREDRAW, VARIANT_TRUE );

  return rc;
}

long PhysicalButtonDisplay::DisplayControlButtons(void)
{
  long rc = 0;

  USES_CONVERSION;

  long count = 0;
  long offset = 0;

  if (!USSFutil.TestPSX())
  {
    return 0;
  }

  COleVariant v;
  v = _T("");
  m_pGAPSX->SetConfigProperty(_T("SMButton1"), csCtxName, UI::PROPERTYTEXTFORMAT, v);
  m_pGAPSX->SetConfigProperty(_T("SMButton1"), csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
  v = (long)UI::Disabled;
  m_pGAPSX->SetConfigProperty(_T("SMButton1"), csCtxName, UI::PROPERTYSTATE, v);
  v = _T("");
  m_pGAPSX->SetConfigProperty(_T("SMButton2"), csCtxName, UI::PROPERTYTEXTFORMAT, v);
  m_pGAPSX->SetConfigProperty(_T("SMButton2"), csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
  v = (long)UI::Disabled;
  m_pGAPSX->SetConfigProperty(_T("SMButton2"), csCtxName, UI::PROPERTYSTATE, v);
  v = _T("");
  m_pGAPSX->SetConfigProperty(_T("SMButton3"), csCtxName, UI::PROPERTYTEXTFORMAT, v);
  m_pGAPSX->SetConfigProperty(_T("SMButton3"), csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
  v = (long)UI::Disabled;
  m_pGAPSX->SetConfigProperty(_T("SMButton3"), csCtxName, UI::PROPERTYSTATE, v);
  v = _T("");
  m_pGAPSX->SetConfigProperty(_T("SMButton4"), csCtxName, UI::PROPERTYTEXTFORMAT, v);
  m_pGAPSX->SetConfigProperty(_T("SMButton4"), csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
  v = (long)UI::Disabled;
  m_pGAPSX->SetConfigProperty(_T("SMButton4"), csCtxName, UI::PROPERTYSTATE, v);
  v = _T("");
  m_pGAPSX->SetConfigProperty(_T("SMButton5"), csCtxName, UI::PROPERTYTEXTFORMAT, v);
  m_pGAPSX->SetConfigProperty(_T("SMButton5"), csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
  v = (long)UI::Disabled;
  m_pGAPSX->SetConfigProperty(_T("SMButton5"), csCtxName, UI::PROPERTYSTATE, v);
  v = _T("");
  m_pGAPSX->SetConfigProperty(_T("SMButton6"), csCtxName, UI::PROPERTYTEXTFORMAT, v);
  m_pGAPSX->SetConfigProperty(_T("SMButton6"), csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
  v = (long)UI::Disabled;
  m_pGAPSX->SetConfigProperty(_T("SMButton6"), csCtxName, UI::PROPERTYSTATE, v);
  v = _T("");
  m_pGAPSX->SetConfigProperty(_T("SMButton7"), csCtxName, UI::PROPERTYTEXTFORMAT, v);
  m_pGAPSX->SetConfigProperty(_T("SMButton7"), csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
  v = (long)UI::Disabled;
  m_pGAPSX->SetConfigProperty(_T("SMButton7"), csCtxName, UI::PROPERTYSTATE, v);
  v = _T("");
  m_pGAPSX->SetConfigProperty(_T("SMButton8"), csCtxName, UI::PROPERTYTEXTFORMAT, v);
  m_pGAPSX->SetConfigProperty(_T("SMButton8"), csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
  v = (long)UI::Disabled;
  m_pGAPSX->SetConfigProperty(_T("SMButton8"), csCtxName, UI::PROPERTYSTATE, v);

  // Need to traverse the display elements from offset for up to MAXBUTTONNUM elements.
  DisplayElementInfoIter = v_Buttons.begin();
  while (DisplayElementInfoIter != v_Buttons.end())
  {
    // Now show the elements starting with specified element offset (or 0).
    if (offset >= lCurrentScroll)
    {
      // Max buttons reached?
      if (count < lMaxButtons)
      {
		CString thisControl;
		if ((*DisplayElementInfoIter).csControl.GetLength() > 0)
		{
			thisControl = (*DisplayElementInfoIter).csControl;
		}
		else
		{
			thisControl = _T("SMButton");
			thisControl.Format(_T("SMButton%01d"), count + 1);
		}
        // Put it on the context!
		CString str = (*DisplayElementInfoIter).csDesc;
		USSFutil.ResolveSymbol(str, str, _T("NOVALUE"));

		v = str;
		m_pGAPSX->SetConfigProperty(thisControl, csCtxName, UI::PROPERTYTEXTFORMAT, v);

		str = (*DisplayElementInfoIter).csEnabled;
		USSFutil.ResolveSymbol(str, str, _T("false"));
		if (!str.CompareNoCase(_T("true")))
		{
			v = (long)UI::Normal;
			m_pGAPSX->SetConfigProperty(thisControl, csCtxName, UI::PROPERTYSTATE, v);
			(*DisplayElementInfoIter).lStatus = 1;
		}
		else
		{
			v = (long)UI::Disabled;
			m_pGAPSX->SetConfigProperty(thisControl, csCtxName, UI::PROPERTYSTATE, v);
			(*DisplayElementInfoIter).lStatus = 0;
		}

        v = (long) offset;	
        count++;
      }
    }
    offset++;
    DisplayElementInfoIter++;
  }

  return rc;
}

long PhysicalButtonDisplay::Refresh()
{
  long rc = 0;

  USES_CONVERSION;

  //long count = 0;
  //long offset = 0;
  //long nButtonCount = MAXBUTTONNUM;

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
		// Control Buttons!
		rc = RefreshControlButtons();
	  }
	  break;
  case 1:
  default:
	  {
		// Selection Buttons!
		rc = RefreshPrimarySelection(csCtlNameA);

		if (rc == 0)
		{
			if (csCtlNameB != _T(""))
			{
				rc = RefreshSecondarySelection(csCtlNameB);
				if (rc != 0)
				{
					trace(L5, _T("Secondary Selection Refresh Failed."));
				}
			}
		}
		else
		{
			trace(L5, _T("Primary Selection Refresh Failed."));
		}
	  }
	  break;
  }

  return rc;
}

long PhysicalButtonDisplay::RefreshPrimarySelection(CString csCtlName)
{
  long rc = 0;
  long count = 0;
  long offset = 0;

  // reset the count to 0 to clear all previously set attributes
  //COleVariant v = (long) 0;
  //m_pGAPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
	//m_pGAPSX->SetControlProperty(csCtlName, UI::PROPERTYBUTTONCOUNT, v );

  //v = nButtonCount;
  //m_pGAPSX->SetControlProperty( csCtlName, UI::PROPERTYREDRAW, VARIANT_FALSE );

  //m_pGAPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
  //m_pGAPSX->SetControlProperty( csCtlName, UI::PROPERTYBUTTONCOUNT, v );

  CString str = _T("");

  COleVariant vReturn, v1, v2, vIndex, vParam;

  // Need to traverse the display elements from offset for up to MAXBUTTONNUM elements.
  DisplayElementInfoIter = v_Buttons.begin();
  while (DisplayElementInfoIter != v_Buttons.end())
  {
    // Now show the base types starting with specified element offset (or 0).
    if (offset >= lCurrentScroll)
    {
      // Max buttons reached?
      if (count < lMaxButtons)
      {
        if ((*DisplayElementInfoIter).lStatus)
        {
		      vIndex = (long) count;
		      vParam = (long) UI::Pushed;
			  vParam.ChangeType(VT_I4);
		      m_pGAPSX->SendCommand(csCtlNameA, UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
        }
        else
        {
		      vIndex = (long) count;
		      vParam = (long) UI::Normal;
			  vParam.ChangeType(VT_I4);
		      m_pGAPSX->SendCommand(csCtlNameA, UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
        }

        count++;
      }
	}
    offset++;
	DisplayElementInfoIter++;
  }

  m_pGAPSX->SetControlProperty( csCtlNameA, UI::PROPERTYREDRAW, VARIANT_TRUE );

  return rc;
}

long PhysicalButtonDisplay::RefreshSecondarySelection(CString csCtlName)
{
  long rc = 0;
  long count = 0;
  long offset = 0;

  // reset the count to 0 to clear all previously set attributes
  //COleVariant v = (long) 0;
  //m_pGAPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
	//m_pGAPSX->SetControlProperty(csCtlName, UI::PROPERTYBUTTONCOUNT, v );

  //v = nButtonCount;
  //m_pGAPSX->SetControlProperty( csCtlName, UI::PROPERTYREDRAW, VARIANT_FALSE );

  //m_pGAPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
  //m_pGAPSX->SetControlProperty( csCtlName, UI::PROPERTYBUTTONCOUNT, v );

  CString str = _T("");

  COleVariant vReturn, v1, v2, vIndex, vParam;

  // Need to traverse the display elements from offset for up to MAXBUTTONNUM elements.
  DisplayElementInfoIter = v_Buttons.begin();
  while (DisplayElementInfoIter != v_Buttons.end())
  {
    // Now show the base types starting with specified element offset (or 0).
    if (offset >= lCurrentScroll)
    {
      // Max buttons reached?
      if (count < lMaxButtons)
      {
        if ((*DisplayElementInfoIter).lStatus)
        {
		      vIndex = (long) count;
		      vParam = (long) UI::Highlighted;
			  vParam.ChangeType(VT_I4);
		      m_pGAPSX->SendCommand(csCtlNameB, UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
        }
        else
        {
		      vIndex = (long) count;
		      vParam = (long) UI::Normal;
			  vParam.ChangeType(VT_I4);
		      m_pGAPSX->SendCommand(csCtlNameB, UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
        }

        count++;
      }
	}
    offset++;
	DisplayElementInfoIter++;
  }

  m_pGAPSX->SetControlProperty( csCtlNameB, UI::PROPERTYREDRAW, VARIANT_TRUE );

  return rc;
}

long PhysicalButtonDisplay::RefreshControlButtons(void)
{
  long rc = 0;

  USES_CONVERSION;

  long count = 0;
  long offset = 0;

  if (!USSFutil.TestPSX())
  {
    return 0;
  }

  COleVariant v;
  v = _T("");

  // Need to traverse the display elements from offset for up to MAXBUTTONNUM elements.
  DisplayElementInfoIter = v_Buttons.begin();
  while (DisplayElementInfoIter != v_Buttons.end())
  {
    // Now show the elements starting with specified element offset (or 0).
    if (offset >= lCurrentScroll)
    {
      // Max buttons reached?
      if (count < lMaxButtons)
      {
		CString thisControl = _T("SMButton");
		thisControl.Format(_T("SMButton%01d"), count + 1);
        // Put it on the context!
		v = (*DisplayElementInfoIter).csDesc;
		m_pGAPSX->SetConfigProperty(thisControl, csCtxName, UI::PROPERTYTEXTFORMAT, v);
		//m_pGAPSX->SetConfigProperty(thisControl, csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);

		// If button ID=EXIT, then must check to see if can exit assist mode.
		bool bFlag = true;
		CString csTempstr = (*DisplayElementInfoIter).csID;
		csTempstr.MakeUpper();
		if (csTempstr == _T("EXIT"))
		{
			//if (!TBStateIsReadyToSell())
			//{
			//	trace(L5, _T("Disabled EXIT Button - Not Ready to Sell."));
			//	bFlag = false;
			//}
		}
		if (bFlag)
		{
			if ((*DisplayElementInfoIter).lStatus == 0)
			{
				v = (long)UI::Normal;
				m_pGAPSX->SetConfigProperty(thisControl, csCtxName, UI::PROPERTYSTATE, v);
				(*DisplayElementInfoIter).lStatus = 1;
			}
		}
		else
		{
			if ((*DisplayElementInfoIter).lStatus == 1)
			{
				v = (long)UI::Disabled;
				m_pGAPSX->SetConfigProperty(thisControl, csCtxName, UI::PROPERTYSTATE, v);
				(*DisplayElementInfoIter).lStatus = 0;
			}
		}

        v = (long) offset;	
        count++;
      }
    }
    offset++;
    DisplayElementInfoIter++;
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
  //PSXRC rc;

  //COleVariant vReturn;
  //PSXRC rc = m_pGAPSX->SendCommand(csCtlNameA, UI::COMMANDSCROLL,  vReturn,  1, UI::PageUp);


  lCurrentScroll -= lMaxButtons;
  if (lCurrentScroll <= 0)
  {
    trace(L7, _T("Scrolled Back to Start"));
    lCurrentScroll = 0;
  }

  return rc;
}

long PhysicalButtonDisplay::ScrollDown(void)
{
  long rc = 0;
  //PSXRC rc;

  //COleVariant vReturn;
  //PSXRC rc = m_pGAPSX->SendCommand(csCtlNameA, UI::COMMANDSCROLL,  vReturn,  1, UI::PageDown);

  lCurrentScroll += lMaxButtons;
  if (lCurrentScroll >= v_Buttons.size())
  {
    trace(L7, _T("Scrolled Back to Start"));
    lCurrentScroll = 0;
  }

  return rc;
}

/////////////////////////////////
// DisplayShop Methods!
/////////////////////////////////
DisplayShop::DisplayShop()
{
  //csActiveProductDescription = _T("");
  //csActiveProductID = _T("");
  //csActiveBaseType = _T("");
  //csActiveBaseTypeHeader = _T("");
  //bIngredientAlert = false;
}

//SMStateBase *DisplayShop::Initialize()
//{
//  return STATE_NULL;
//}

long DisplayShop::ClearDefinition()
{
  // Clear the myDisplayShop Structure.
  //return TraverseDisplayShop(ACTION_DESTROY);

	GroupInfoIter = GroupInfo.begin();
	while (GroupInfoIter != GroupInfo.end())
	{
		(*GroupInfoIter).v_ButtonInfo.erase(
					(*GroupInfoIter).v_ButtonInfo.begin(),
					(*GroupInfoIter).v_ButtonInfo.end());
		GroupInfoIter++;
	}
	GroupInfo.erase(GroupInfo.begin(), GroupInfo.end());

  return 0;
}

long DisplayShop::LoadDefinition(CString filename)
{
  CString csTemp;

  // First, clear any existing definition.
  ClearDefinition();

  // Interrogates XML File
	CString csXMLFileName = co.csConfigFilePath + _T("\\") + XML_DISPLAYSHOPFILE;

	MSXML2::IXMLDOMDocumentPtr spDoc( __uuidof( MSXML2::DOMDocument40 ) );
	//MSXML2::IXMLDOMDocumentPtr spDoc;
	
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
	
	//Process <DefaultLanguage>
	//CString csDefLangCode(_T(""));
	//spNode = spDoc->selectSingleNode(XML_DEFAULT_LANGUAGE);
	//if (spNode != NULL)
	//{
	//	csDefLangCode = LPCTSTR(spNode->text);
	//}

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
		long lButtonCount = 1;
		// Initialize Structure (class).
		myDisplayGroupInfo.csID = _T("");
		myDisplayGroupInfo.csName = _T("");
		myDisplayGroupInfo.csDescription = _T("");
		myDisplayGroupInfo.csEvent = _T("");
		myDisplayGroupInfo.csSize = _T("");
		myDisplayGroupInfo.csMaxButtons = _T("16");
		//myDisplayGroupInfo.csModel = _T("");

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
		spItem = spNode->selectSingleNode( XML_GROUP_ID );
		if (spItem != NULL)
		{
			csTemp = LPCTSTR(spItem->text);
			myDisplayGroupInfo.csID = csTemp;
		}

		//Process <Name>
		spItem = spNode->selectSingleNode( XML_GROUP_NAME );
		if (spItem != NULL)
		{
			csTemp = LPCTSTR(spItem->text);
			myDisplayGroupInfo.csName = csTemp;
		}

		//Process <Description>
		spItem = spNode->selectSingleNode( XML_GROUP_DESC );
		if (spItem != NULL)
		{
			csTemp = LPCTSTR(spItem->text);
			myDisplayGroupInfo.csDescription = csTemp;
		}

		//Process <ControlA>
		spItem = spNode->selectSingleNode( XML_GROUP_CONTROLA );
		if (spItem != NULL)
		{
			csTemp = LPCTSTR(spItem->text);
			myDisplayGroupInfo.csControlA = csTemp;
		}

		//Process <ControlB>
		spItem = spNode->selectSingleNode( XML_GROUP_CONTROLB );
		if (spItem != NULL)
		{
			csTemp = LPCTSTR(spItem->text);
			myDisplayGroupInfo.csControlB = csTemp;
		}

		//Process <ButtonType>
		spItem = spNode->selectSingleNode( XML_GROUP_BUTTONTYPE );
		if (spItem != NULL)
		{
			csTemp = LPCTSTR(spItem->text);
			myDisplayGroupInfo.csButtonType = csTemp;
		}

		//Process <Event>
		spItem = spNode->selectSingleNode( XML_GROUP_EVENT );
		if (spItem != NULL)
		{
			csTemp = LPCTSTR(spItem->text);
			myDisplayGroupInfo.csEvent = csTemp;
		}

		//Process <Size>
		spItem = spNode->selectSingleNode( XML_GROUP_SIZE );
		if (spItem != NULL)
		{
			csTemp = LPCTSTR(spItem->text);
			myDisplayGroupInfo.csSize = csTemp;
		}

		//Process <MaxButtons>
		spItem = spNode->selectSingleNode( XML_GROUP_MAXBUTTONS );
		if (spItem != NULL)
		{
			csTemp = LPCTSTR(spItem->text);
			myDisplayGroupInfo.csMaxButtons = csTemp;
		}

		//Process <Model>
		spItem = spNode->selectSingleNode( XML_GROUP_MODEL );
		if (spItem != NULL)
		{
			csTemp = LPCTSTR(spItem->text);
			myDisplayGroupInfo.csButtonType = csTemp;
		}

		// Need to do Buttons for this Group!
		myDisplayGroupInfo.v_ButtonInfo.erase(myDisplayGroupInfo.v_ButtonInfo.begin(), 
											  myDisplayGroupInfo.v_ButtonInfo.end());

		long lRepeatCount = 0;
		spSubNodeListA = spNode->selectNodes(XML_BUTTON);
		while( ( spSubNodeA = spSubNodeListA->nextNode() ) )
		{
			// Initialize Button Info!
			//myDisplayButtonInfo.csID = _T("");
			CString csIDTemplate = _T("");
			//myDisplayButtonInfo.csName = _T("");
			CString csNameTemplate = _T("");
			//myDisplayButtonInfo.csControl = _T("");
			CString csControlTemplate = _T("");
			//myDisplayButtonInfo.csDescription = _T("");
			CString csDescriptionTemplate = _T("");
			//myDisplayButtonInfo.csGraphic = _T("");
			CString csGraphicTemplate = _T("");
			//myDisplayButtonInfo.csPriceData = _T("");
			CString csPriceDataTemplate = _T("");
			//myDisplayButtonInfo.csPOSData = _T("");
			CString csPOSDataTemplate = _T("");
			//myDisplayButtonInfo.csEvent = _T("");
			CString csEventTemplate = _T("");
			myDisplayButtonInfo.bSelected = false;
			//myDisplayButtonInfo.csEnabled = _T("true");
			CString csEnabledTemplate = _T("true");
			CString csStatusTemplate = _T("normal");
			lRepeatCount = 1;
			// Erase v_ButtonInfo Vector.
			//myDisplayGroupInfo.v_ButtonInfo.erase(myDisplayGroupInfo.v_ButtonInfo.begin(),
			//									  myDisplayGroupInfo.v_ButtonInfo.end());

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
		  spItem = spSubNodeA->selectSingleNode( XML_BUTTON_ID );
		  if (spItem != NULL)
		  {
			  csTemp = LPCTSTR(spItem->text);
			  csIDTemplate = csTemp;
		  }

		  //Process <Name>
		  spItem = spSubNodeA->selectSingleNode( XML_BUTTON_NAME );
		  if (spItem != NULL)
		  {
			csTemp = LPCTSTR(spItem->text);
			csNameTemplate = csTemp;
		  }

		  //Process <Control>
		  spItem = spSubNodeA->selectSingleNode( XML_BUTTON_CONTROL );
		  if (spItem != NULL)
		  {
			csTemp = LPCTSTR(spItem->text);
			csControlTemplate = csTemp;
		  }

		  //Process <Description>
		  spItem = spSubNodeA->selectSingleNode( XML_BUTTON_DESC );
		  if (spItem != NULL)
		  {
			csTemp = LPCTSTR(spItem->text);
			csDescriptionTemplate = csTemp;
		  }

		  //Process <Graphic>
		  spItem = spSubNodeA->selectSingleNode( XML_BUTTON_GRAPHIC );
		  if (spItem != NULL)
		  {
			csTemp = LPCTSTR(spItem->text);
			csGraphicTemplate = csTemp;
		  }

		  //Process <PriceData>
		  spItem = spSubNodeA->selectSingleNode( XML_BUTTON_PRICEDATA );
		  if (spItem != NULL)
		  {
			csTemp = LPCTSTR(spItem->text);
			// Create CString from CSV values.
			//myDisplayButtonInfo.csPOSData = (char)atoi(csTemp);
			csPriceDataTemplate = csTemp;
		  }

		  //Process <POSData>
		  spItem = spSubNodeA->selectSingleNode( XML_BUTTON_POSDATA );
		  if (spItem != NULL)
		  {
			csTemp = LPCTSTR(spItem->text);
			// Create CString from CSV values.
			//myDisplayButtonInfo.csPOSData = (char)atoi(csTemp);
			csPOSDataTemplate = csTemp;
		  }

		  //Process <Event>
		  spItem = spSubNodeA->selectSingleNode( XML_BUTTON_EVENT );
		  if (spItem != NULL)
		  {
			csTemp = LPCTSTR(spItem->text);
			csEventTemplate = csTemp;
		  }

		  spItem = spSubNodeA->selectSingleNode( XML_BUTTON_ENABLED );
		  if (spItem != NULL)
		  {
			csTemp = LPCTSTR(spItem->text);
			csEnabledTemplate = csTemp;
		  }

		  spItem = spSubNodeA->selectSingleNode( XML_BUTTON_STATUS );
		  if (spItem != NULL)
		  {
			csTemp = LPCTSTR(spItem->text);
			csStatusTemplate = csTemp;
		  }

		  spItem = spSubNodeA->selectSingleNode( XML_BUTTON_REPEAT );
		  if (spItem != NULL)
		  {
			csTemp = LPCTSTR(spItem->text);
			lRepeatCount = _ttol(csTemp);
		  }
		  CString csSubstitution;
		  for (long i = 0; i < lRepeatCount; i++)
		  {	
			// Do Substitutions.
			csSubstitution.Format(_T("%d"), lButtonCount);
			myDisplayButtonInfo.csID = csIDTemplate;
			myDisplayButtonInfo.csID.Replace(_T("$#"), csSubstitution);
			myDisplayButtonInfo.csName = csNameTemplate;
			myDisplayButtonInfo.csName.Replace(_T("$#"), csSubstitution);
			myDisplayButtonInfo.csControl = csControlTemplate;
			myDisplayButtonInfo.csControl.Replace(_T("$#"), csSubstitution);
			myDisplayButtonInfo.csDescription = csDescriptionTemplate;
			myDisplayButtonInfo.csDescription.Replace(_T("$#"), csSubstitution);
			myDisplayButtonInfo.csGraphic = csGraphicTemplate;
			myDisplayButtonInfo.csGraphic.Replace(_T("$#"), csSubstitution);
			myDisplayButtonInfo.csPriceData = csPriceDataTemplate;
			myDisplayButtonInfo.csPriceData.Replace(_T("$#"), csSubstitution);
			myDisplayButtonInfo.csPOSData = csPOSDataTemplate;
			myDisplayButtonInfo.csPOSData.Replace(_T("$#"), csSubstitution);
			myDisplayButtonInfo.csEvent = csEventTemplate;
			myDisplayButtonInfo.csEvent.Replace(_T("$#"), csSubstitution);
			myDisplayButtonInfo.csEnabled = csEnabledTemplate;
			myDisplayButtonInfo.csEnabled.Replace(_T("$#"), csSubstitution);
			myDisplayButtonInfo.csStatus = csStatusTemplate;
			myDisplayButtonInfo.csStatus.Replace(_T("$#"), csSubstitution);
			lButtonCount++;
			myDisplayGroupInfo.v_ButtonInfo.push_back(myDisplayButtonInfo);
		  }
	  }

	  if (myDisplayGroupInfo.csSize == _T(""))
	  {// Substitute the size we counted.
		  CString csVariable;
		  csVariable.Format(_T("%d"), lButtonCount - 1);
		  myDisplayGroupInfo.csSize = csVariable;
	  }

      GroupInfo.push_back(myDisplayGroupInfo);
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
		csTemp = LPCTSTR(spItem->text);
		csTemp.MakeUpper();
		csTemp = _T("USSF_") + csTemp;
		myDisplaySymbolInfo.csName = csTemp;
	}

	//Process <Name>
	spItem = spNode->selectSingleNode( XML_SYMBOL_VALUE );
	if (spItem != NULL)
	{
		csTemp = LPCTSTR(spItem->text);
		myDisplaySymbolInfo.csValue = csTemp;
	}
	USSFutil.SetTransactionVariable(myDisplaySymbolInfo.csName, myDisplaySymbolInfo.csValue);
    SymbolInfo.push_back(myDisplaySymbolInfo);
  }
  return 0;
}

long DisplayShop::LoadSelectionChoices(CString groupID, CString contextID, CString controlID)
{
  trace(L7, _T("Loading Choices for Display Group ID=<%s>; Context=<%s>; Control=<%s>."),
            groupID, contextID, controlID);

  long count = 0;
  long rc = 0;
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  // Due to the inflexibility of PSThread, it is necessary to play games to get a list selection to 
  // work properly via PSListLookup. The Data MUST be numeric. As such, I use the index into the 
  // ingredient structure in myDisplayShop. It is an index into an STL vector. The actual data
  // element can then be looked up indirectly. This is a CString and is a key to the ingredient object.
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  DisplayElement thisDisplayElement;

  SelectionManager.csCtxName = contextID;
  SelectionManager.csCtlNameA = controlID;
  SelectionManager.csComponentID = groupID;
  SelectionManager.lCurrentScroll = 0;
  SelectionManager.lMaxButtons = 16;	// Could Be Externalized.

  long lMinAllowed = 0, lMaxAllowed = 999;

  // Regular Selection Button, by definition.
  SelectionManager.lType = 1;	// Selection - Non-Radio.
  SelectionManager.v_Buttons.erase(SelectionManager.v_Buttons.begin(), SelectionManager.v_Buttons.end());

  // Find the Display ID.
  GroupInfoIter = GroupInfo.begin();
  while (GroupInfoIter != GroupInfo.end())
  {
    if ((*GroupInfoIter).csID == groupID)
    {
      // Found it.

	  CString csTempstr = (*GroupInfoIter).csMaxButtons;
	  SelectionManager.lMaxButtons = _ttol(csTempstr);	// Could Be Externalized.

      long offset = 0;
	  // If Control is specified, use it instead of the default!
	  if ((*GroupInfoIter).csControlA != _T(""))
	  {
		  SelectionManager.csCtlNameA = (*GroupInfoIter).csControlA;
	  }
	  if ((*GroupInfoIter).csControlB != _T(""))
	  {
		  SelectionManager.csCtlNameB = (*GroupInfoIter).csControlB;
	  }
	  csTempstr = (*GroupInfoIter).csButtonType;
	  USSFutil.ResolveSymbol(csTempstr, csTempstr);
	  if (!(csTempstr.CompareNoCase(_T("radio"))))
	  {
		  SelectionManager.lType = 2;		// Radio.
	  }

	  // Let's get the size!
	  csTempstr = (*GroupInfoIter).csSize;
	  USSFutil.ResolveSymbol(csTempstr, csTempstr);
	  long lGroupSize = _ttol(csTempstr);

      // Now show the buttons starting with specified element offset (or 0).
      ButtonInfoIter = (*GroupInfoIter).v_ButtonInfo.begin();
      while ((ButtonInfoIter != (*GroupInfoIter).v_ButtonInfo.end()) &&
			 (count < lGroupSize))
      {
		csTempstr = (*ButtonInfoIter).csID;
		USSFutil.ResolveSymbol(csTempstr, csTempstr);
		thisDisplayElement.csID = csTempstr;

		if ((*ButtonInfoIter).csDescription.GetLength() > 0)
		{
		  csTempstr = (*ButtonInfoIter).csDescription;
		}
		else
		{
		  csTempstr = (*ButtonInfoIter).csName;
		}
		USSFutil.ResolveSymbol(csTempstr, csTempstr);
		thisDisplayElement.csDesc = csTempstr;

		csTempstr = (*ButtonInfoIter).csGraphic;
		USSFutil.ResolveSymbol(csTempstr, csTempstr);
		thisDisplayElement.csGraphic = csTempstr;

		csTempstr = (*ButtonInfoIter).csPriceData;
		USSFutil.ResolveSymbol(csTempstr, csTempstr);
		thisDisplayElement.csAmount = csTempstr;

		csTempstr = (*ButtonInfoIter).csEnabled;
		USSFutil.ResolveSymbol(csTempstr, csTempstr);
		thisDisplayElement.csEnabled = csTempstr;
		//thisDisplayElement.csAmount = _T("");

		// Need to provide pass-through mechanism for status as well!
        csTempstr = (*ButtonInfoIter).csStatus;
		USSFutil.ResolveSymbol(csTempstr, csTempstr);

        if (!csTempstr.CompareNoCase(_T("normal")))
		{
			thisDisplayElement.lStatus = 0;
		}
		else if (!csTempstr.CompareNoCase(_T("pushed")))
		{
			thisDisplayElement.lStatus = 1;
		}
		else
		{
			thisDisplayElement.lStatus = 0;
		}
		// Following Code Replaced to Allow for Pass-Through Data
		//if ((*ButtonInfoIter).bSelected)
		//{
		//  thisDisplayElement.lStatus = 1;
		//}
		//else
		//{
		//  thisDisplayElement.lStatus = 0;
		//}

		thisDisplayElement.lIndex = count++;

        SelectionManager.v_Buttons.push_back(thisDisplayElement);
		ButtonInfoIter++;
      }
    }
    GroupInfoIter++;
  }

  return count;
}

long DisplayShop::LoadControlChoices(CString groupID, CString contextID, CString controlID)
{
  trace(L7, _T("Loading Choices for Display Group ID=<%s>; Context=<%s>; Control=<%s>."),
            groupID, contextID, controlID);

  long count = 0;
  long rc = 0;
  CString csTempstr;
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  // Due to the inflexibility of PSThread, it is necessary to play games to get a list selection to 
  // work properly via PSListLookup. The Data MUST be numeric. As such, I use the index into the 
  // ingredient structure in myDisplayShop. It is an index into an STL vector. The actual data
  // element can then be looked up indirectly. This is a CString and is a key to the ingredient object.
  //////////////////////////////////////////////////////////////////////////////////////////////////////

  DisplayElement thisDisplayElement;

  ControlManager.csCtxName = contextID;
  ControlManager.csCtlNameA = controlID;
  ControlManager.csComponentID = groupID;
  ControlManager.lCurrentScroll = 0;
  ControlManager.lMaxButtons = 8;	// Could Be Externalized.

  long lMinAllowed = 0, lMaxAllowed = 999;

  // Control Button, by definition.
  ControlManager.lType = 0;
  ControlManager.v_Buttons.erase(ControlManager.v_Buttons.begin(), ControlManager.v_Buttons.end());

  // Find the Display ID.
  GroupInfoIter = GroupInfo.begin();
  while (GroupInfoIter != GroupInfo.end())
  {
    if ((*GroupInfoIter).csID == groupID)
    {
      // Found it.
      long offset = 0;
      // Now show the buttons starting with specified element offset (or 0).
      ButtonInfoIter = (*GroupInfoIter).v_ButtonInfo.begin();
      while (ButtonInfoIter != (*GroupInfoIter).v_ButtonInfo.end())
      {
        csTempstr = (*ButtonInfoIter).csID;
		USSFutil.ResolveSymbol(csTempstr, csTempstr);
		thisDisplayElement.csID = csTempstr;

        if ((*ButtonInfoIter).csDescription.GetLength() > 0)
        {
          csTempstr = (*ButtonInfoIter).csDescription;
        }
        else
        {
          csTempstr = (*ButtonInfoIter).csName;
        }
		USSFutil.ResolveSymbol(csTempstr, csTempstr);
		thisDisplayElement.csDesc = csTempstr;

		csTempstr = (*ButtonInfoIter).csControl;
		USSFutil.ResolveSymbol(csTempstr, csTempstr);
		thisDisplayElement.csControl = csTempstr;

        csTempstr = (*ButtonInfoIter).csGraphic;
		USSFutil.ResolveSymbol(csTempstr, csTempstr);
		thisDisplayElement.csGraphic = csTempstr;

        thisDisplayElement.csAmount = _T("");

        csTempstr = (*ButtonInfoIter).csStatus;
		USSFutil.ResolveSymbol(csTempstr, csTempstr);

        if (!csTempstr.CompareNoCase(_T("normal")))
		{
			thisDisplayElement.lStatus = 0;
		}
		else if (!csTempstr.CompareNoCase(_T("pushed")))
		{
			thisDisplayElement.lStatus = 1;
		}
		else
		{
			thisDisplayElement.lStatus = 0;
		}
		// Following Code Replaced to Allow for Pass-Through Data
        //if ((*ButtonInfoIter).bSelected)
        //{
        //  thisDisplayElement.lStatus = 1;
        //}
        //else
        //{
        //  thisDisplayElement.lStatus = 0;
        //}
		csTempstr = (*ButtonInfoIter).csEnabled;
		USSFutil.ResolveSymbol(csTempstr, csTempstr);
		thisDisplayElement.csEnabled = csTempstr;

        thisDisplayElement.lIndex = count++;

        ControlManager.v_Buttons.push_back(thisDisplayElement);
		ButtonInfoIter++;
      }
    }
    GroupInfoIter++;
  }

  return count;
}

long DisplayShop::GetButtonInfo(CString groupID, CString buttonID, CString& Event, CString& POSData)
{
	long count = 0, rc = 0;

	Event = _T("");
	POSData = _T("");
	// Need to search the vector for the group and return event and POSData.
	// Find the Display ID.
	GroupInfoIter = GroupInfo.begin();
	while (GroupInfoIter != GroupInfo.end())
	{
		if ((*GroupInfoIter).csID == groupID)
		{
			Event = (*GroupInfoIter).csEvent;
			rc = 1;
			// Found it!
			// Now find the button (by ID).
			ButtonInfoIter = (*GroupInfoIter).v_ButtonInfo.begin();
			while (ButtonInfoIter != (*GroupInfoIter).v_ButtonInfo.end())
			{
				if ((*ButtonInfoIter).csID == buttonID)
				{
					// Found it, set reference parms.
					Event = (*ButtonInfoIter).csEvent;
					POSData = (*ButtonInfoIter).csPOSData;
					rc = 2;
				}
				ButtonInfoIter++;
			}
		}
		GroupInfoIter++;
	}
	return rc;
}

long DisplayShop::StoreSymbols()
{
  long rc = 0;
  CString csName, csValue;

  // Iterate though the symbols (constants) and store them as transaction variables.

  SymbolInfoIter = SymbolInfo.begin();
  while (SymbolInfoIter != SymbolInfo.end())
  {
	csName = (*SymbolInfoIter).csName;
	csValue = (*SymbolInfoIter).csValue;
	USSFutil.SetTransactionVariable(csName, csValue);
	SymbolInfoIter++;
  }

  return rc;
}