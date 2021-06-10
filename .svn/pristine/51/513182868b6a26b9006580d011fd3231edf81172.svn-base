    //////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // FILE:  PSInterface.CPP
    //
    // TITLE:   Display Presentation Services Interface (Low Level interface to NCR DPS)
    // This module handles all DPS calls from PSProcedures (High Level)
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // CHANGES: Start
    // CHANGES: End
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    #include <stdafx.h>                 // MS MFC always needed first
    //               // MS MFC template header
    #include <afxmt.h>                  // MS MFC template header
#ifndef NewUI
    #include "Common.h" 
    #include "PSInterface.h"            // Low level PS header
    #include "PSThread.h"               // PS thread header
    #include "PSIntList.h"
    
    DLLEXPORT extern HDEVCONTEXT hDC;   // Handle to the device context from PSThread()
    DLLEXPORT extern CEvent cPSThreadEvent;
    #ifdef _BUILDLIB_
    DLLEXPORT extern PSProceduresBase &ps;
    //#else
    //DLLEXPORT extern PSProcedures ps;
    #endif
    // local defines
    #define COMP_ID     ID_PS
    #define T_ID        _T("PSi")
    #define WHOLEVEL    L9
    
    ///////////////////////////////////////////////////////////////////////
    // Private (non-public) routines
    ///////////////////////////////////////////////////////////////////////
    
    static long RcError(const int nLine,
                        const int nLevel,
                        LPCTSTR szWho,   // caller name
                        LPCTSTR szWindow, // name of window
                        const int nIdc,// ID of element or 0
                        LPCTSTR szDesc,  // desc of event
                        LPCTSTR szText,  // formatted text
                        const PSRC rc)  // rc from PS function call
    {
      if (nIdc)
      {
        ScotError(COMP_ID,T_ID,nLine,nLevel,SM_CAT_SYSTEM_CALL, SCOTAPP_PSINTERFACE,
          _T("%s|%s|%d|%s|%s|%d"),
          szWho,szWindow,nIdc,szDesc,szText,rc);
        return rc;
      }
    	
      ScotError(COMP_ID,T_ID,nLine,nLevel,SM_CAT_SYSTEM_CALL, SCOTAPP_PSINTERFACE2,
        _T("%s|%s|%s|%d"),
        szWho,szDesc,szText,rc);
    	
      return rc;
    }
    
    ///////////////////////////////////////////////////////////////////////
    static long CheckPSRC(const int nLine,
                          const int nLevel,
                          LPCTSTR szWho,// caller name
                          const PSRC rc, // rc from PS function call
                          LPCTSTR szWindow, // name of window
                          const int nIdc,//ID of element or 0
                          LPCTSTR szDesc) // desc of event
    {
      _TCHAR *w;
    	
      trace(L9,_T("CheckPSRC"));
    	
      switch (rc)
      {
      case PS_SUCCESS: return 0;
    		
      case PS_ALREADYINITIALIZED:
        w = _T("SCOTPS has already been initialized"); break;
    		
      case PS_NOSELECTION:
        w = _T("No item is selected"); break;
    		
      case PS_E_BUFFERTOOSMALL:
        w = _T("Buffer is too small for data copying"); break;
    		
      case PS_E_INVALIDINDEX:
        w = _T("Index passed is not valid"); break;
    		
      case PS_E_NOTSUPPORTED:
        w = _T("Command is not supported"); break;
    		
      case PS_E_INVALIDCALLBACK:
        w = _T("Callback function is not valid"); break;
    		
      case PS_E_INVALIDDEVCONTEXT:
        w = _T("The hDC passed is not valid"); break;
    		
      case PS_E_INVALIDPARAM:
        w = _T("At least one of the parameters is invalid"); break;
    		
      case PS_E_INVALIDCOMMAND:
        w = _T("Command is not valid in the usage context"); break;
    		
      case PS_E_INVALIDSHOWFLAG:
        w = _T("Specified show flag is invalid"); break;
    		
      case PS_E_INVALIDPOINTER:
        w = _T("Specified pointer is invalid"); break;
    		
      case PS_E_DEVICENOTSUPPORTED:
        w = _T("No PSM configured for this device"); break;
    		
      case PS_E_INVALIDPSM:
        w = _T("Invalid or non-existence PSM"); break;
    		
      case PS_E_INVALIDPSFILE:
        w = _T("Invalid or non-existence PS or related files"); break;
    		
      case PS_E_INVALIDPS:
        w = _T("Invalid or non-existence PS"); break;
    		
      case PS_E_INVALIDTEMPLATE:
        w = _T("Invalid or non-existence template"); break;
    		
      case PS_E_INVALIDWINDOW:
        w = _T("Invalid or non-existence window"); break;
    		
      case PS_E_INVALIDELEMENT:
        w = _T("Invalid or non-existence element"); break;
    		
      case PS_E_INVALIDMEDIA:
        w = _T("Invalid or non-existence media for use in PS"); break;
    		
      case PS_E_OUTOFMEMORY:
        w = _T("Memory allocation error"); break;
    		
      case PS_E_CREATEFAIL:
        w = _T("Creation of window failed"); break;
    		
      case PS_E_UNEXPECTED:
        w = _T("Really unexpected error"); break;

		case PS_E_REMOTEINVALIDPARMS:
		  w = _T("Remote parameter count mismatch"); break;
    		
      default:
        w = _T("Unknown PS error"); break;
      }
    	
      return RcError(nLine,nLevel,szWho,szWindow,nIdc,szDesc,w,rc);
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSInterface_Initialize(void)   // initialization, must be called once
    {
        return 0;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSInterface_UnInitialize(void)// shut down
    {
        return 0;
    }
        
    ///////////////////////////////////////////////////////////////////////
    PSSimpleInterface::PSSimpleInterface(LPCTSTR szWName) // constructor for each window object
    {
      // must have a artwork template defined and passed
      ASSERT(szWName);
      ASSERT(*szWName!=0);
    	
      // save name and initialize other vars
      szWindow = szWName;
      hWnd = NULL;
      fVisible = false;
	  bSetContentSuccessful = true;
      szBackground = NULL;
	  nLocalXtraValue = 0;
    }  
    ///////////////////////////////////////////////////////////////////////
    long PSSimpleInterface::GetHandle(const bool fErrors)// get handle of alreay loaded window
    {
#ifndef NewUI
      _WHO(GetHandle);
    	
      // get window handle
      hWnd = PSGetWindowHandle(hDC,szWindow);
      if (!hWnd)
      {
        if (!fErrors) return -1;
        PSRC rc = PS_E_UNEXPECTED;
        if (CheckRC(__LINE__,C_FATAL,szWho,rc,0,szWindow)) return rc;
      }
      return 0;
#endif
return 0;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSSimpleInterface::GetControlHandle(const long lID, HWND& hControlWnd)// get handle of alreay loaded window
    {
#ifndef NewUI
      _WHO(GetControlHandle);
    	
      SELEMENT sEle = {0};
      PSRC rc = 0;

      static _TCHAR szBuff[100];
      if (!hWnd) return 0;						  // not loaded
      else 
	  {
		sEle.nIndex   = PS_CURRENTSELECTION;
		sEle.pchBuffer = szBuff;
		sEle.lBufferSize = 100;
		rc=PSGetElement(hDC, hWnd, lID, &sEle);
		if (rc==PS_SUCCESS)
		{
			hControlWnd = sEle.hWindow;
			ASSERT(hControlWnd) ;		// hControlWnd should not be NULL
		}
		else
			rc = -1;  // Make sure we return failure if PSGetElement fails.  ncr_barn

		// if no selection is current on this list avoid the -51 error
		return rc;
	  }
#endif
return 0;
     }

    ///////////////////////////////////////////////////////////////////////
    long PSSimpleInterface::Load(const bool fErrors) // load a window by name
    {
#ifndef NewUI
      _WHO(Load);
      PSRC rc;
    	
      if (hWnd) return 0;               // already loaded
    	
      // load artwork into memory
      rc = PSShowPresentation(hDC,szWindow,PS_SF_HIDE);
      if (rc!=PS_SUCCESS && !fErrors) return -1;
      if (CheckRC(__LINE__,C_FATAL,szWho,rc,0,szWindow)) return rc;
      fLoaded = true;
      return GetHandle(fErrors);        // get hWnd
#endif
return 0;
    }

    
    ///////////////////////////////////////////////////////////////////////
    long PSSimpleInterface::Show(void)     // show a window by hWnd or name
    {
#ifndef NewUI
      _WHO(Show);
      PSRC rc = 0;

//      TAR 191910 Eliminate possibility of an earlier failure preventing
//      element from ever being shown again
//      if (!hWnd || fVisible || !bSetContentSuccessful) return 0;   // not loaded or already visible or content wasn't set
      if (!hWnd || fVisible) return 0;   // not loaded or already visible or content wasn't set
      if (fLoaded)
      {
        rc = PSShowWindow(hDC,hWnd,PS_SF_SHOW);
      }
      else
      {
        rc = PSShowWindowEx(hDC,szWindow,PS_SF_SHOW);
      }
    	
      if (CheckRC(__LINE__,C_FATAL,szWho,rc,0,szWindow)) return rc;
      fVisible = true;
      return 0;
#endif
return 0;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSSimpleInterface::Hide(void)        // hide a window
    {
#ifndef NewUI
      _WHO(Hide);
      PSRC rc;
    	
//      TAR 191910 Eliminate possibility of an earlier failure preventing
//      element from ever being hidden again
//      if (!hWnd || !fVisible || !bSetContentSuccessful) return 0;   // not loaded or already visible or content wasn't set
      if (!hWnd || !fVisible) return 0;   // not loaded or already visible or content wasn't set

      rc = PSShowWindow(hDC,hWnd,PS_SF_HIDE);
      if (CheckRC(__LINE__,C_FATAL,szWho,rc,0,szWindow)) return rc;
    
      fVisible = false;
      return 0;
#endif
return 0;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSSimpleInterface::Content(const long lIdc,        // set window element contents
                                    const PSTEXT nTxt)      // ps text number
    {
#ifndef NewUI
      return Content(lIdc,ps.GetPSText(nTxt));
#endif
return 0;
    }                                                
    ///////////////////////////////////////////////////////////////////////
    long PSSimpleInterface::Content(const long lIdc, // set window element contents
                                    LPCTSTR szContent)
    {
#ifndef NewUI
      _WHO(Content);
      SELEMENT sEle = {0};
      PSRC rc = 0;
    	
      if (!hWnd) return 0;              // not loaded
      rc = ShowElement(lIdc,PS_SF_SHOW,szContent,0,0,sEle);	// see macro
	  if (rc != PS_SUCCESS)
	  {
		  // failed on content, note it and let us gracefully handle other operations such as show or hide
		  bSetContentSuccessful = false;
	  }
      return CheckRC(__LINE__,C_RETURN,szWho,rc,lIdc,szContent);  // TAR 111991
#endif
return 0;
    }

    ///////////////////////////////////////////////////////////////////////
    long PSSimpleInterface::Content(const long lIdc, // set window element contents
                                    LPCTSTR szContent, const bool hide)
    {
#ifndef NewUI
      _WHO(Content);
      SELEMENT sEle = {0};
      PSRC rc = 0;
    	
      if (!hWnd) return 0;              // not loaded
	  if (hide)
      {
        if (szContent)
          rc = ShowElement(lIdc,PS_SF_DISABLE | PS_SF_SHOW,
		                   szContent,0,0,sEle);	
		else
          rc = ShowElement(lIdc,PS_SF_DISABLE | PS_SF_HIDE,
		                   szContent,0,0,sEle);	
      }
	  else
      {
        rc = ShowElement(lIdc,PS_SF_SHOW,szContent,0,0,sEle);	
      }
	  if (rc != PS_SUCCESS)
	  {
		  // failed on content, note it and let us gracefully handle other operations such as show or hide
		  bSetContentSuccessful = false;
	  }
      return CheckRC(__LINE__,C_RETURN,szWho,rc,lIdc,szContent);  // TAR 111991
#endif
return 0;
    }


    ///////////////////////////////////////////////////////////////////////
    long PSSimpleInterface::CheckRC(const int nLine,
                                    const int nLevel,
                                    LPCTSTR szWho,// PS func name
                                    const PSRC rc, // PS func return code
                                    const long lIdc, // ID of element or 0
                                    LPCTSTR szDesc) // desc of event
                                    const
    {
      trace(L9,_T("CheckRC"));
      return CheckPSRC(nLine,nLevel,szWho,rc,szWindow,lIdc,szDesc);
    }
   
	 
    ///////////////////////////////////////////////////////////////////////
    long PSSimpleInterface::ShowElement(const int nIdc,
                                        const ULONG ulFunc,
                                        LPCTSTR szText,
                                        const long lXtra,
                                        const int nDx,
                                        SELEMENT &sEle)
    {
#ifndef NewUI
      long rc= 0;
      sEle.fShow = ulFunc;
      sEle.szContent = szText;
	  /*
	  if (IsReceipt() && ((ulFunc == PS_SF_ADD) || (ulFunc == PS_SF_INSERT)))
	  {
		// if not a column request but a receipt operation
		if (!((nIdc == IDC_RECEIPTCOL2) && (ulFunc == PS_SF_ADD)))
		{
			++nLocalXtraValue;
		}
		sEle.lExtraData = nLocalXtraValue;
	  }
	  else
	  */
	  {
		sEle.lBitmapData = lXtra;
		sEle.lExtraData = nLocalXtraValue;
	  }
      sEle.nIndex = nDx;
      rc = PSShowElement(hDC, hWnd, nIdc, &sEle);
      return rc;
#endif
return 0;
    }
    
    
    ///////////////////////////////////////////////////////////////////////
    //ShowElement to load ShowElement
    long PSSimpleInterface::ShowElement2(const int nIdc,
      			          				 const ULONG ulFunc,
    					         		 LPCTSTR szText,
    							         const long lXtra,
    				          			 const int nDx,
    						         	 SELEMENT &sEle)
    {
#ifndef NewUI
    	return(ShowElement(nIdc, ulFunc,szText, lXtra,nDx, sEle));
#endif
return 0;
    }

    ///////////////////////////////////////////////////////////////////////
    // pointer this window's name
    void PSSimpleInterface::Init(LPCTSTR szWName)
    {
#ifndef NewUI
      // must have a artwork template defined and passed
      ASSERT(szWName);
      ASSERT(*szWName!=0);
    	
      // save name and initialize other vars      
      hWnd = NULL;
      fVisible = false;
      szBackground = NULL;
      szWindow = szWName;      
	  bReceipt = false;
#endif
    }
    

    ///////////////////////////////////////////////////////////////////////
    PSInterface::PSInterface(LPCTSTR szWName) // constructor for each window object
	          	:PSSimpleInterface(szWName) 
    {
      // must have a artwork template defined and passed
      ASSERT(szWName);
      ASSERT(*szWName!=0);
             
    }  
        
    ///////////////////////////////////////////////////////////////////////
    long PSInterface::Button(const long lIdc,// show a button
                             LPCTSTR szContent,// button text
                             const bool fEnable)// T=Enabled, F=Disabled but still visible
    {
#ifndef NewUI
      _WHO(Button);
      SELEMENT sEle = {0};
      PSRC rc = 0;
    	
      if (!hWnd) return 0;              // not loaded
    	
      if (szContent)
      {
        if (fEnable)
        {
          rc = ShowElement(lIdc,
            PS_SF_ENABLE | PS_SF_SHOW,szContent,0,0,sEle);
        }
        else
        {
          rc = ShowElement(lIdc,
            PS_SF_DISABLE | PS_SF_SHOW,szContent,0,0,sEle);
        }
        // only save button status for buttons 1-8
        if (lIdc>=IDC_BUTTON1 && lIdc<=IDC_BUTTON8)
          fButton[lIdc-IDC_BUTTON1] = true;
      }
      else
      {
        rc = ShowElement(lIdc,
          PS_SF_DISABLE | PS_SF_HIDE,szContent,0,0,sEle);
        if (lIdc>=IDC_BUTTON1 && lIdc<=IDC_BUTTON8)
          fButton[lIdc-IDC_BUTTON1] = false;
      }
    	
      return CheckRC(__LINE__,C_FATAL,szWho,rc,lIdc,szContent);
#endif
return 0;

    }
    
   
    ///////////////////////////////////////////////////////////////////////
    long PSInterface::Button(const long lIdc,// show a button
                             const PSTEXT nTxt, // button ps text number
                             const bool fEnable)// T=Enabled, F=Disabled but still visible
    {
#ifndef NewUI
	  if (!nTxt)
	  {
		_WHO(Button);
		SELEMENT sEle = {0};
		PSRC rc = 0;

		if (!hWnd) return 0;              // not loaded
        rc = ShowElement(lIdc,
          PS_SF_DISABLE | PS_SF_HIDE,NULL,0,0,sEle);
        if (lIdc>=IDC_BUTTON1 && lIdc<=IDC_BUTTON8)
          fButton[lIdc-IDC_BUTTON1] = false;
		return CheckRC(__LINE__,C_FATAL,szWho,rc,lIdc,NULL);
	  }
	  else
	  {
		return Button(lIdc,ps.GetPSText(nTxt),fEnable);
	  }
#endif
return 0;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSInterface::ButtonFlash(const long lIdc,// flash a button
    	    					  const bool fFlash)// T=Flash, F=no Flash
    {
#ifndef NewUI
      _WHO(Button);
      SELEMENT sEle = {0};
      PSRC rc = 0;
    	
      if (!hWnd) return 0;              // not loaded
    	
      if (fFlash)
      {
        rc = ShowElement2(lIdc,PS_SF_FLASH,0,0,0, sEle); // Flash
      }
      else
      {
        rc = ShowElement2(lIdc,PS_SF_ENABLE,0,0,0, sEle); // no Flash
      }
    	
      return CheckRC(__LINE__,C_FATAL,szWho,rc,lIdc,_T(""));
#endif
return 0;
    }
    
    
    ///////////////////////////////////////////////////////////////////////
    long PSInterface::ButtonDisable(const long lIdc) // disable a button
    {
#ifndef NewUI
      _WHO(ButtonDisable);
      SELEMENT sEle = {0};
      PSRC rc = 0;
    	
      if (!hWnd) return 0;              // not loaded
    	
      if (lIdc>=IDC_BUTTON1 && lIdc<=IDC_BUTTON8)
      {
        if (fButton[lIdc-IDC_BUTTON1])
        {
          rc = ShowElement(lIdc,PS_SF_DISABLE,NULL,0,0,sEle);
        }
      }
      else
      {
        rc = ShowElement(lIdc,PS_SF_DISABLE,NULL,0,0,sEle);
      }
      return CheckRC(__LINE__,C_FATAL,szWho,rc,lIdc,_T("DisableButton"));
#endif
return 0;
    }

    long PSInterface::GraphicButtonDisable(const long lIdc,const long lIndex) // disable a button
    {
#ifndef NewUI
      _WHO(ButtonDisable);
      SELEMENT sEle = {0};
      PSRC rc = 0;
    	
      if (!hWnd) return 0;              // not loaded

      rc = ShowElement(lIdc,PS_SF_DISABLE,NULL,0,lIndex,sEle);
      return CheckRC(__LINE__,C_FATAL,szWho,rc,lIdc,_T("ButtonDisable"));
#endif
return 0;
    }
    
    long PSInterface::GraphicButtonEnable(const long lIdc,const long lIndex) // disable a button
    {
#ifndef NewUI
      _WHO(ButtonEnable);
      SELEMENT sEle = {0};
      PSRC rc = 0;
    	
      if (!hWnd) return 0;              // not loaded

      rc = ShowElement(lIdc,PS_SF_ENABLE,NULL,0,lIndex,sEle);
      return CheckRC(__LINE__,C_FATAL,szWho,rc,lIdc,_T("ButtonEnable"));
#endif
return 0;
    }
	
    long PSInterface::GraphicButtonHide(const long lIdc,const long lIndex) // hide a button
    {
#ifndef NewUI
      _WHO(ButtonEnable);
      SELEMENT sEle = {0};
      PSRC rc = 0;
    	
      if (!hWnd) return 0;              // not loaded

      rc = ShowElement(lIdc,PS_SF_HIDE,NULL,0,lIndex,sEle);
      return CheckRC(__LINE__,C_FATAL,szWho,rc,lIdc,_T("ButtonHide"));
#endif
return 0;
    }
   
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    
   PSInterfaceList1::PSInterfaceList1(LPCTSTR szWName,
                                      const long lIdc)
	                : PSInterface(szWName)
    {
      ListIdc       = lIdc;
      lCountList    = 0;
      lLastSelected = 0;
      m_nTwoLine    = 0;
      bStrikethru   = 0;
	  haveSelectableSalesItems = 0;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceList1::Add(LPCTSTR szDesc,const bool fHighLight)
    {
#ifndef NewUI
      _WHO(Add);
      SELEMENT sEle = {0};
      PSRC rc = 0;
      long nFun;
    	
      if (!hWnd) return 0;              // not loaded
    	
      nFun = PS_SF_ADD;
      if (fHighLight)
      {
        nFun |= PS_SF_HIGHLIGHT;
      }
     	
      ++nLocalXtraValue; // increment this irrespective of showelement result, no harm

      sEle.nItemLength = -1;

	  // create new unique index for this item
      rc = ShowElement(ListIdc,nFun,szDesc,0,0,sEle);
      if (CheckRC(__LINE__,C_FATAL,szWho,rc,ListIdc,szDesc)) return rc;

      lCountList++;	

      csItemDesc.Add(szDesc);
      return 0;
#endif
return 0;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceList1::Add(const PSTEXT nTxt, // ps text number
                               const bool fHighLight)
    {
#ifndef NewUI
      return Add(ps.GetPSText(nTxt),fHighLight);
#endif
return 0;
    }
    

    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceList1::Update(const long index,
		                          LPCTSTR szDesc, 
								  const bool fHighLight)
    {
#ifndef NewUI
      _WHO(Update);
      SELEMENT sEle = {0};
      PSRC rc = 0;
      long nFun;
    	
      if (!hWnd) return 0;              // not loaded
    	
      nFun = PS_SF_MODIFY;
      if (fHighLight)
      {
        nFun |= PS_SF_HIGHLIGHT;
      }
      sEle.nItemLength = -1;

	  // update entry specified

	  // TAR 214520 - Update the item w/ specified index
      // rc = ShowElement(ListIdc,nFun,szDesc,index,0,sEle); 
      rc = ShowElement(ListIdc,nFun,szDesc,0,index,sEle);

      if (CheckRC(__LINE__,C_FATAL,szWho,rc,ListIdc,szDesc)) return rc;
      return 0;
#endif
return 0;
    }
    


    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceList1::Reset(void)  // reset and empty a receipt or list
    {
#ifndef NewUI
      _WHO(Reset);
      SELEMENT sEle = {0};
      PSRC rc = 0;
    	
      if (!hWnd)
		return 0;              // not loaded
      rc = ShowElement(ListIdc,PS_SF_RESET,0,0,0,sEle);
      if (CheckRC(__LINE__,C_FATAL,szWho,rc,ListIdc,_T("Reset")))
		return rc;
      lCountList = 0;
	  haveSelectableSalesItems = 0;
      csItemDesc.RemoveAll();
	  nLocalXtraValue = 0;
      return 0;
#endif
return 0;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceList1::Get(CString &csDesc)
    {
#ifndef NewUI
      _WHO(Get);
      SELEMENT sEle = {0};
      PSRC rc = 0;
      static _TCHAR szBuff[300];
    	
      if (!hWnd) return 0;						  // not loaded
    	
      // get the current data
      sEle.nIndex   = PS_CURRENTSELECTION;
      sEle.pchBuffer = szBuff;
      sEle.lBufferSize = 300;
      rc = PSGetElement(hDC, hWnd, ListIdc, &sEle);
        	
      if (rc==PS_SUCCESS)
      {
		//T.D 05/06/02 TAR 203386 Beginning 
		trace(L6, _T("rc is PS_SUCCESS: %d"), rc);
		trace(L6, _T("Get: sEle.lExtraData=%d, sEle.nIndex:%d"), sEle.lExtraData, sEle.nIndex);
		// AKR: Do not use sEle.lExtraData as it doesn't need to match with index since
		// SCOT allows now insertions and deletions in addition to just add operation on receipt
		if (csItemDesc.GetSize()>0)
		{
			if (PSInterfaceList1::GetAt(sEle.nIndex+1, csDesc)) //tar 204295 - added +1
			{
				trace(L6, _T("Size of csItemDesc: %d, Index: %d"), csItemDesc.GetSize(), sEle.nIndex);
				rc = sEle.nIndex+1; // make it one based index as it is expected and working before
				trace(L6, _T("rc: %d"), rc);

			}
			else
				rc = -1;  // Make sure we return failure if PSGetElement fails.  ncr_barn	
		}
      }
      else
        rc = -1;  // Make sure we return failure if PSGetElement fails.  ncr_barn
    	
      // if no selection is current on this list avoid the -51 error
      return rc;
#endif
return 0;
    }
  	//T.D 05/06/02 TAR 203386 End
  
    //////////////////////////////////////////////////////////////////////
    long PSInterfaceList1::GetAt(const long nDx, // get info, index 1,2,3...
                                 CString &csDesc)
    {
#ifndef NewUI
      _WHO(GetAt);
    	
      if (!hWnd) return 0;              // not loaded
    	
      if (nDx>=1 && nDx<=csItemDesc.GetSize())
      {
        csDesc = csItemDesc.GetAt(nDx-1);
        return nDx;
      }
      return -1;
#endif
return -1;
    }
    
	///////////////////////////////////////////////////////////////////////
	//tar 182942
    long PSInterfaceList1::SetTopIndex(void) 
    {
#ifndef NewUI
      _WHO(SetTopIndex);
      PSRC rc = 0;
      SELEMENT sEle = {0};
    	
      if (!hWnd) return 0;              // not loaded
      rc = ShowElement(ListIdc,PS_SF_TOPINDEX,0,0,0,sEle);
      if (CheckRC(__LINE__,C_FATAL,szWho,rc,ListIdc,_T(""))) return rc;

      return 0;
#endif
return 0;
    }

    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceList1::Select(const long lSelect)// index, 1,2,3...
    {
#ifndef NewUI
      _WHO(Select);
      PSRC rc = 0;
      SELEMENT sEle = {0};
    	
      if (!hWnd) return 0;              // not loaded
      rc = ShowElement(ListIdc,PS_SF_SELECT,0,0,lSelect-1,sEle);
      if (CheckRC(__LINE__,C_FATAL,szWho,rc,ListIdc,_T(""))) return rc;

      lLastSelected = lSelect;
      return 0;
#endif
return 0;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceList1::UnSelect(void) // un-highlight the last highlighted item
    {
#ifndef NewUI
      _WHO(UnSelect);
      PSRC rc = 0;
      SELEMENT sEle = {0};
    	
      if (!hWnd) return 0;              // not loaded
      if (!lLastSelected) return 0;
      rc = ShowElement(ListIdc,
                       PS_SF_UNSELECT,0,0,lLastSelected-1,sEle);
    	
      if (CheckRC(__LINE__,C_FATAL,szWho,rc,ListIdc,_T(""))) return rc;

      lLastSelected = 0;
      return 0;
#endif
return 0;
    }
    
    // pointer this window's name                                               
    // ... Idc of list
    ///////////////////////////////////////////////////////////////////////
    void PSInterfaceList1::Init(LPCTSTR szWname, const long lIdc) 
    {
#ifndef NewUI
      ListIdc = lIdc;
      lCountList = 0;
	  haveSelectableSalesItems = 0;
      lLastSelected = 0;
      PSInterface::Init(szWname);
#endif
    }

	long PSInterfaceList1::GetListCount() 
    {
      return lCountList;
    }


    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    
     PSInterfaceList2::PSInterfaceList2(LPCTSTR szWName,
                                       const long lIdc)
                                       : PSInterfaceList1(szWName,lIdc)
    {
    	lLastNonInfo = 0;
    }
    
   
    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceList2::Add(LPCTSTR szDesc,
                               LPCTSTR szCode,
                               const bool fHighLight)
    {
      
#ifndef NewUI
      LPCTSTR dot = _T(".");
      // Check data for font changes...
	  if (*szCode!=INFO_ITEM && *szCode!=LINK_ITEM)
      { 
          if (*szCode == *dot)
            m_nTwoLine = -1;
		  else
             m_nTwoLine = 0;
    	  lLastNonInfo = lCountList+1;
      }  
	  else
      {
         if (*szCode==LINK_ITEM) 
           m_nTwoLine = 1;
         else
           m_nTwoLine = 2;
      }
      csItemCode.Add(szCode);
      return PSInterfaceList1::Add(szDesc,fHighLight);
#endif
return 0;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceList2::Reset(void)  // reset and empty a receipt or list
    {
#ifndef NewUI
       lLastNonInfo = 0;
       csItemCode.RemoveAll();
       return PSInterfaceList1::Reset();
#endif
return 0;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceList2::Get(CString &csDesc,
                               CString &csCode)
    {
#ifndef NewUI
      _WHO(Get);
    	
      long lNdx = PSInterfaceList1::Get(csDesc);
      if (lNdx>0)
      {
        csCode = csItemCode.GetAt(lNdx-1);
        return lNdx;
      }
      return -1;
#endif
return -1;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceList2::GetAt(const long nDx, // get info, index 1,2,3...
                                 CString &csDesc,
                                 CString &csCode)
    {
#ifndef NewUI
      _WHO(GetAt);
    	
      if (PSInterfaceList1::GetAt(nDx,csDesc))
      {
        csCode = csItemCode.GetAt(nDx-1);
        return nDx;
      }
      return -1;
#endif
return -1;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceList2::ScrollUp(const bool fHunt) // scroll list
    {
#ifndef NewUI
      _WHO(ScrollUp);
      SELEMENT sEle = {0};
      PSRC rc = 0;
    	
      if (!hWnd) return 0;          // not loaded
    
      if (!lCountList) return 0;	// no items to scroll
    
    	// the receipt may be on a data or info line, not the actual item desc line
    	// scroll up until the we are on the actual item desc line
    	// data lines have a 'code' starting with a byte 0x01...0x03
    	// so if we are on such a line we must keep scrolling up
    
    	CString csDesc,csCode;
    	long lOrg,lNdx;
    	lOrg = lNdx = Get(csDesc,csCode);	// get current index
    	while (lNdx>1) {                  // don't loop if already at the top
    
    		rc = ShowElement(ListIdc,PS_SF_SCROLLUP,0,0,0,sEle);
    		if (rc!=PS_SUCCESS)
    			return CheckRC(__LINE__,C_FATAL,szWho,rc,ListIdc,_T(""));

    		lNdx = Get(csDesc,csCode);
    
    		if (lNdx>0)
    		{
    			if (*csCode!=INFO_ITEM && *csCode!=VOID_ITEM && *csCode!=LINK_ITEM)
    				break;
    		}
    	}
    	
      // if not on an info line, ok, return now
      if (lNdx>0)
      {
    		if (*csCode!=INFO_ITEM && *csCode!=VOID_ITEM && *csCode!=LINK_ITEM)
        {
          SetScrolling();
    			return lNdx;
        }
      }
    	
      // if we top out with no valid item lines return to the original item
      if (lNdx<lOrg)
      {
    		Select(lOrg);
    		return lOrg;
      }
    	
      if (!fHunt)
      {
        SetScrolling();
        return lNdx;                    // no hunting allowed, exit
      }
    	
      // scroll down to next valid item line or the bottom
      return ScrollDown(false);
#endif
return 0;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceList2::ScrollDown(const bool fHunt) // scroll receipt down
    {
#ifndef NewUI
      _WHO(ScrollDown);
      SELEMENT sEle = {0};
      PSRC rc = 0;
    	
      if (!hWnd) return 0;              // not loaded
    	
      if (!lCountList) return 0;	// no items to scroll
    	
      // the receipt may be on a data or info line, not the actual item desc line
      // scroll down until the we are on the actual item desc line
      // data lines have a 'code' starting with a byte 0x01...0x03
      // so if we are on such a line we must keep scrolling down
      // if we reach the bottom without finding a non-voided line, scroll up
      // until we find one
      // if we reach the top without finding a non-voided item, return to the
      // item we started from
    	
      CString csDesc,csCode;
      long lOrg,lNdx;
    	
      lOrg = lNdx = Get(csDesc,csCode);	// get current item desc
    	
      while (lNdx<lCountList) { // don't loop if already at the end
    
    		rc = ShowElement(ListIdc,PS_SF_SCROLLDOWN,0,0,0,sEle);
    	
    		if (rc!=PS_SUCCESS)
    			return CheckRC(__LINE__,C_FATAL,szWho,rc,ListIdc,_T(""));

    
        lNdx = Get(csDesc,csCode);
    
        if (lNdx>0)
        {
    			if (*csCode!=INFO_ITEM && *csCode!=VOID_ITEM && *csCode!=LINK_ITEM)
            break;
        }
      }
    	
      // if we find a non-info line stop here
      if (lNdx>0)
      {
    		if (*csCode!=INFO_ITEM && *csCode!=VOID_ITEM && *csCode!=LINK_ITEM)
        {
          SetScrolling();
          return lNdx;
        }
      }
    	
      // if we bottom out with no non-voided lines return to the original item
      if (lNdx>lOrg)
      {
        Select(lOrg);
    		return lOrg;
      }
    	
      // if we are not allowed to go up, exit here
      if (!fHunt)
      {
        SetScrolling();
        return lNdx;
      }
    	
      // try to scroll up
      return ScrollUp(false);
#endif
return 0;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceList2::SetScrolling(void)
    {
#ifndef NewUI
      _WHO(SetScrolling);
    	
    	// John Gatto says to leave scroll buttons on all the time 98/11/06
      Button(IDC_RECEIPTUP,PS_BLANK,true);
      Button(IDC_RECEIPTDOWN,PS_BLANK,true);
    	/*
      // need to turn off scroll buttons on receipts
      // with just 0 or 1 lines
      if (lCountList<2)
      {
        Button(IDC_RECEIPTUP,PS_NOTEXT,false);
        Button(IDC_RECEIPTDOWN,PS_NOTEXT,false);
      }
      else
      {
        // pd981009 requested by NCR to not gray out buttons
        Button(IDC_RECEIPTUP,PS_BLANK,true);
        Button(IDC_RECEIPTDOWN,PS_BLANK,true);
    		// re-activated for revison E UI test pd981001
        CString csCode,csDesc;
        int nIndex = Get(								  // get current index, 1,2,3...
    			csCode,												  // return the list item code
    			csDesc);											  // return the item desc
        if (nIndex<=1)
        {
    			Button(IDC_RECEIPTUP,PS_BLANK,false);
    			Button(IDC_RECEIPTDOWN,PS_BLANK,true);
        }
        else if (nIndex>=lLastNonInfo)
        {
    			Button(IDC_RECEIPTUP,PS_BLANK,true);
    			Button(IDC_RECEIPTDOWN,PS_BLANK,false);
        }
        else
        {
    			Button(IDC_RECEIPTUP,PS_BLANK,true);
    			Button(IDC_RECEIPTDOWN,PS_BLANK,true);
        }
      }
    	*/
    	
      return 0;
#endif
return 0;
    }

    
    // pointer this window's name                                               
    // ... Idc of list
    ///////////////////////////////////////////////////////////////////////
    void PSInterfaceList2::Init(LPCTSTR szWname, const long lIdc) 
    {
#ifndef NewUI
      lLastNonInfo = 0;
      PSInterfaceList1::Init(szWname, lIdc);
#endif
    }

    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceList2::ShowTopList() 
    {
#ifndef NewUI
      _WHO(ShowMostCurrent);
      SELEMENT sEle = {0};
      PSRC rc = 0;
    	
      if (!hWnd) return 0;              // not loaded

      Show(); 	
      if (!lCountList) return 0;	    // no items to scroll
         
      rc = ShowElement(ListIdc,PS_SF_SCROLLUP,0,0,0,sEle);
      if (rc!=PS_SUCCESS)
    	return CheckRC(__LINE__,C_FATAL,szWho,rc,ListIdc,_T(""));
      return 0; 
#endif
return 0;
    }

    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceList2::ShowMostCurrent() 
    {
#ifndef NewUI
      _WHO(ShowMostCurrent);
      SELEMENT sEle = {0};
      PSRC rc = 0;
    	
      if (!hWnd) return 0;              // not loaded

      Show(); 	
      if (!lCountList) return 0;	    // no items to scroll
         
      rc = ShowElement(ListIdc,PS_SF_SCROLLDOWN,0,0,0,sEle);
      if (rc!=PS_SUCCESS)
    	return CheckRC(__LINE__,C_FATAL,szWho,rc,ListIdc,_T(""));
      return 0; 
#endif
return 0;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////

    
     PSInterfaceGraphicList::PSInterfaceGraphicList(LPCTSTR szWName,
                                       const long lIdc)
                                       : PSInterfaceList2(szWName,lIdc)
    {
    	lLastNonInfo = 0;
    }
    
   
    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceGraphicList::Add(LPCTSTR szDesc,
                               LPCTSTR szCode,
                               const bool fHighLight,
							   HBITMAP hBmp)
    {
#ifndef NewUI
      
      LPCTSTR dot = _T(".");
      // Check data for font changes...
	  if (*szCode!=INFO_ITEM && *szCode!=LINK_ITEM)
      { 
          if (*szCode == *dot)
            m_nTwoLine = -1;
		  else
             m_nTwoLine = 0;
    	  lLastNonInfo = lCountList+1;
      }  
	  else
      {
         if (*szCode==LINK_ITEM) 
           m_nTwoLine = 1;
         else
           m_nTwoLine = 2;
      }
      csItemCode.Add(szCode);

  			
			caHBmp.Add(hBmp);

      
      _WHO(Add);
      SELEMENT sEle = {0};
      PSRC rc = 0;
      long nFun;
    	
      if (!hWnd) return 0;              // not loaded
    	
      nFun = PS_SF_ADD;
      if (fHighLight)
      {
        nFun |= PS_SF_HIGHLIGHT;
      }
     	
      lCountList++;	

      sEle.nItemLength = -1;
		 
      rc = ShowElement(ListIdc,nFun,szDesc,(long)hBmp,lCountList,sEle);
      if (CheckRC(__LINE__,C_FATAL,szWho,rc,ListIdc,szDesc)) return rc;
    	
      csItemDesc.Add(szDesc);
      return 0;
#endif
return 0;

    }
    
    ///////////////////////////////////////////////////////////////////////
	 long PSInterfaceGraphicList::Reset(void)  // reset and empty a receipt or list
    {
#ifndef NewUI
       caHBmp.RemoveAll();
       return PSInterfaceList2::Reset();
#endif
return 0;
    }
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////

    
    PSInterfaceErcpt::PSInterfaceErcpt(LPCTSTR szWName,
                                       const long lIdc)
                                       : PSInterfaceList2(szWName,lIdc)
    {
		bReceipt = true;
    }
     
    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceErcpt::Add(const bool fHighLight,
    						               LPCTSTR szDesc,
                               LPCTSTR szCode,
                               const long lQty,
                               const long lWgt,
                               const long lTcd,
                               const long lPrc,
                               const long lUPrc,	// lUnitPrice
							                 const long lBarcodeType,
											 const long lEntryid,
							                 const long lResAge,
								               LPCTSTR szCat,
											   const bool bSecTag)     //exception category
    {
#ifndef NewUI
      lQuantity.Add(lQty);
      lWeight.Add(lWgt);
      lPrice.Add(lPrc);
      lUnitPrice.Add(lUPrc);
      lTare.Add(lTcd);
	  lRestrictedAge.Add(lResAge);
	  lScanType.Add(lBarcodeType);
	  lEntryIDList.Add(lEntryid);
      bAutomaticApproval.Add(false);
      csExceptionCategory.Add(szCat); 
	  bHasSecurityTag.Add(bSecTag);
      return PSInterfaceList2::Add(szDesc,szCode,fHighLight);
#endif
return 0;
    }
    

    ///////////////////////////////////////////////////////////////////////
	// nOperation = 0 - add, 1- insert, 2 - delete
    long PSInterfaceErcpt::Column2(LPCTSTR szDesc, long nIndex, short nOperation) // formatted price
    {
#ifndef NewUI
      _WHO(Column2);
      SELEMENT sEle = {0};
      PSRC rc = 0;
      //long lNew;
      long nFun;
	  --nIndex ; // make this index zero based
    	
      if (!hWnd) return 0;              // not loaded
	  nFun = PS_SF_ADD;  
    
      rc = ShowElement(IDC_RECEIPTCOL2,nFun,szDesc,nLocalXtraValue,nIndex,sEle);
      if (CheckRC(__LINE__,C_FATAL,szWho,rc,IDC_RECEIPTCOL2,szDesc)) return rc;
      return 0;
#endif
return 0;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceErcpt::Reset(void)  // reset and empty a receipt or list
    {
#ifndef NewUI
      lQuantity.RemoveAll();
      lWeight.RemoveAll();
      lPrice.RemoveAll();
      lUnitPrice.RemoveAll();
      lTare.RemoveAll();
	  lRestrictedAge.RemoveAll();
	  lScanType.RemoveAll();
      bAutomaticApproval.RemoveAll();
      csExceptionCategory.RemoveAll(); 
	  lEntryIDList.RemoveAll();
	  bHasSecurityTag.RemoveAll();
	  return PSInterfaceList2::Reset();
#endif
return 0;
    }
    
    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceErcpt::Get(CString &csDesc,
                               CString &csCode,
                               long &lQty,
                               long &lWgt,
                               long &lPrc,
                               long &lUPrc,
                               long &lTcd,
							   long &lBarcodeType)
    {
#ifndef NewUI
      _WHO(Get);
    	
      long lNdx = PSInterfaceList2::Get(csDesc,csCode);
      if (lNdx>0)
      {
        lQty = lQuantity.GetAt(lNdx-1);
        lWgt = lWeight.GetAt(lNdx-1);
        lPrc = lPrice.GetAt(lNdx-1);
        lUPrc = lUnitPrice.GetAt(lNdx-1);
        lTcd = lTare.GetAt(lNdx-1);
		lBarcodeType = lScanType.GetAt(lNdx-1);
        return lNdx;
      }
      return -1;
#endif
return -1;
    }

    long PSInterfaceErcpt::GetEntryIDAt(const long nDx, // get info, index 1,2,3...
										long  &lCurrentEntryID)
	{
#ifndef NewUI
      _WHO(GetEntryIDAt);
    	
      if (!hWnd) return 0;              // not loaded
    	
      if (nDx>=1 && nDx<=lEntryIDList.GetSize())
      {
        lCurrentEntryID = lEntryIDList.GetAt(nDx-1);
        return nDx;
      }
      return -1;
#endif
return -1;
	}

	long PSInterfaceErcpt::GetVoidItemIndex()
	{
#ifndef NewUI
		CString csDesc,csCode,csCat;
		long lQty,lWgt,lPrc,lUPrc,lTcd, lBarcodeType;
		long selectedIndex = Get(csDesc,csCode,lQty,lWgt,lPrc,lUPrc,lTcd,lBarcodeType); // get selected item 
		int voidIndex = -1;
		bool bItemCodeMatches = false;
		{
			CString csCode1 = csCode;
			if (csCode1.GetLength() && !_istdigit(csCode1[0]))
			{
				csCode1.SetAt(0, _T(' '));
				csCode1.TrimLeft();
			}
			CString csCode2 = io.csItemCode;
			if (csCode2.GetLength() && !_istdigit(csCode2[0]))
			{
				csCode2.SetAt(0, _T(' '));
				csCode2.TrimLeft();
			}
			if (csCode1 == csCode2)
				bItemCodeMatches = true; // item code is the same
		}
		ASSERT(bItemCodeMatches);
		if (bItemCodeMatches)
		{
			// if not the current item
			if ((csItemCode.GetSize() >= 1) && selectedIndex != -1 )
			{
				voidIndex = 0;
				for (int k=csItemCode.GetSize() -1; k >= (selectedIndex-1); k--)
				{
					CString csCode = csItemCode.GetAt(k);
					if (*csCode!=INFO_ITEM && *csCode!=VOID_ITEM && *csCode!=LINK_ITEM)
						voidIndex = voidIndex + 1;
				}
			}
			else if (csItemCode.GetSize() == 1)
			{
				voidIndex = selectedIndex;
			}
		}
		else
		{
			ASSERT(0);
		}
		return voidIndex;
#endif
return 0;
	}

    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceErcpt::GetAt(const long nDx, // get info, index 1,2,3...
                                 CString &csDesc,
                                 CString &csCode,
                                 long &lQty,
                                 long &lWgt,
                                 long &lPrc,
                                 long &lUPrc,
                                 long &lTcd,
							     long &lBarcodeType,
				                 CString &csCat)
    {
#ifndef NewUI
      _WHO(GetAt);
    	
      if (PSInterfaceList2::GetAt(nDx,csDesc,csCode))
      {
        lQty = lQuantity.GetAt(nDx-1);
        lWgt = lWeight.GetAt(nDx-1);
        lPrc = lPrice.GetAt(nDx-1);
        lUPrc = lUnitPrice.GetAt(nDx-1);
        lTcd = lTare.GetAt(nDx-1);
		lBarcodeType = lScanType.GetAt(nDx-1);
        csCat = csExceptionCategory.GetAt(nDx-1); 
        return nDx;
      }
      return -1;
#endif
return -1;
    }
    
    ///////////////////////////////////////////////////////////////////////
	long PSInterfaceErcpt::GetResAgeAt(const long nDx,  //index
									long &lResAge)
	{
#ifndef NewUI
		_WHO(GetResAgeAt);
		lResAge = lRestrictedAge.GetAt(nDx-1);
		return nDx;
#endif
return 0;
	}

	///////////////////////////////////////////////////////////////////////
	long PSInterfaceErcpt::GetSecurityTagItem(const long nDx,  //index
									CString &csDesc,
									CString &csCode,
									bool &bSecTag)
	{
#ifndef NewUI
		_WHO(GetSecurityTagItem);
		if (PSInterfaceList2::GetAt(nDx,csDesc,csCode))
		{
			if(bHasSecurityTag.GetAt(nDx-1) == 0)
				bSecTag = false;
			else
				bSecTag = true;
			return nDx;
		}
		else
		{
			bSecTag = false;
			return -1;
		}
#endif
return -1;
	}

    ///////////////////////////////////////////////////////////////////////
	long PSInterfaceErcpt::GetCurrentItemAutoApprovalFlag(bool &bAutoApproval)
	{
#ifndef NewUI
		_WHO(GetCurrentItemAutoApprovalFlag);
        CString csDesc, csCode;
        long lNdx = PSInterfaceList2::Get(csDesc,csCode);
        if (lNdx>0)
        {
		    if (bAutomaticApproval.GetAt(lNdx-1) == 0)
                bAutoApproval = false;
            else bAutoApproval = true;
            return lNdx;
        }
        else
        {
            bAutoApproval = false;
            return 0;
        }
#endif
	return 0;
	}
    ///////////////////////////////////////////////////////////////////////
	long PSInterfaceErcpt::GetAutoApprovalFlagAt(const long lNdx, bool &bAutoApproval)
	{
#ifndef NewUI
		_WHO(GetAutoApprovalFlagAt);
        CString csDesc, csCode;
        if (PSInterfaceList2::GetAt(lNdx, csDesc,csCode))
        {
		    if (bAutomaticApproval.GetAt(lNdx-1) == 0)
                bAutoApproval = false;
            else bAutoApproval = true;
            return lNdx;
        }
        else
        {
            bAutoApproval = false;
            return 0;
        }
#endif
return 0;
	}

	///////////////////////////////////////////////////////////////////////
	long PSInterfaceErcpt::SetAutoApprovalFlagAt(const long lNdx, const bool bAutoApproval)
	{
		_WHO(SetAutoApprovalFlagAt);
		bAutomaticApproval.SetAt(lNdx-1, bAutoApproval);
		return lNdx;
       
	}

    ///////////////////////////////////////////////////////////////////////
	long PSInterfaceErcpt::SetCurrentItemAutoApprovalFlag(const bool bAutoApproval)
	{
		_WHO(SetCurrentItemAutoApprovalFlag);
        CString csDesc, csCode;
        long lNdx = PSInterfaceList2::Get(csDesc,csCode);
        if (lNdx>0)
        {
		    bAutomaticApproval.SetAt(lNdx-1, bAutoApproval);
            return lNdx;
        }
        else
        {
            return 0;
        }
	}

    ///////////////////////////////////////////////////////////////////////
	//tar 182942
	long PSInterfaceErcpt::SetTopIndex(void) 
	{
		_WHO(SetTopIndex);
        SELEMENT sEle = {0};
        PSRC rc = 0;
        PSInterfaceList1::SetTopIndex();

		int nLength = csItemDesc.GetSize(); 		
		const int nListLength = nLength;
		--nLength;
		CString csCurrentItemCode;
		while (nLength >= 0)
		{
			csCurrentItemCode = csItemCode.GetAt(nLength);
			bool bSelectableItem = false;
			if (csCurrentItemCode.GetLength())
			{
				bSelectableItem =  ((*csCurrentItemCode != INFO_ITEM) && (*csCurrentItemCode != VOID_ITEM) && (*csCurrentItemCode != LINK_ITEM));
			}
			if (bSelectableItem)
			{
				// a selectabe item was found from the bottom, make it selected
				trace(L7, _T("Select (zero based index) : %d"), nLength);
				PSInterfaceList1::Select(nLength+1);
				break;
			}
			else
			{
				--nLength;
			}
		}
		return 0;
	}

    ///////////////////////////////////////////////////////////////////////
    long PSInterfaceErcpt::Select (const long lSelect,// index, 1,2,3...
                                   const bool fVoid)// void this item


    {
      _WHO(Select);
      SELEMENT sEle = {0};
      PSRC rc = 0;
    	
      PSInterfaceList1::Select(lSelect);
    	
      if (fVoid)
      {
        
  	    sEle.nItemLength = -1;

        rc = ShowElement(ListIdc,
          PS_SF_MODIFY | PS_SF_STRIKETHRU,0,0,lSelect-1,sEle);
    		
   		CString csCode = VOID_ITEM;
   		csCode += csItemCode.GetAt(lSelect-1);
        csItemCode.SetAt(lSelect-1,csCode);
        lQuantity.SetAt(lSelect-1,-1);
        lWeight.SetAt(lSelect-1,-1);
        lPrice.SetAt(lSelect-1,-1);
        lUnitPrice.SetAt(lSelect-1,-1);
        lTare.SetAt(lSelect-1,-1);
		lRestrictedAge.SetAt(lSelect-1, -1);
		lScanType.SetAt(lSelect-1, -1);
        csExceptionCategory.SetAt(lSelect-1, -1); 
		bHasSecurityTag.SetAt(lSelect-1, -1);
        UnSelect();
      }
    	
      return 0;
    }

    // pointer this window's name                                               
    // ... Idc of list
    ///////////////////////////////////////////////////////////////////////
    void PSInterfaceErcpt::Init(LPCTSTR szWname, const long lIdc) 
    {      
      PSInterfaceList2::Init(szWname, lIdc);                         
	  bReceipt = true;
    }

       
    ///////////////////////////////////////////////////////////////////////
    PSFrame::PSFrame(LPCTSTR szWName) : PSInterface(szWName)
	{
	  m_NumButtons = 0;
      m_bHasHelpButton = false;
	}

    ///////////////////////////////////////////////////////////////////////
    PSCMFrame::PSCMFrame(LPCTSTR szWName) : PSFrame(szWName)
	{

      m_NumButtons = 4;
      m_bHasHelpButton = true;
	}

    ///////////////////////////////////////////////////////////////////////
    long PSCMFrame::ClearMessages(void)
	{
#ifndef NewUI
      CMEcho.Content(IDC_ECHO, _T(""));
#endif
      return 0;
	}


    ///////////////////////////////////////////////////////////////////////
    long PSFrame::HideControl(const long lIdc) // control ID
    { 
        SELEMENT sEle = {0};
        PSRC rc = 0;
        _WHO(HideControl);

		rc = ShowElement(lIdc, PS_SF_HIDE,_T(""),0,0,sEle);
        return CheckRC(__LINE__,C_FATAL,szWho,rc,lIdc,_T(""));
	}

    ///////////////////////////////////////////////////////////////////////
    long PSCMFrame::HideWindows(void)
	{
      //remove code here to PSProceduresBase::SetFrame(int nFrame)
	  //to improve screen display
      return 0;
	}

    /////////////////////////////////////////////////////////////////////// 
    PSSMFrame::PSSMFrame(LPCTSTR szWName) : PSFrame(szWName)
	{
      m_NumButtons = 8;
	}

    ///////////////////////////////////////////////////////////////////////
    long PSSMFrame::ClearMessages(void)
	{
      Content(IDC_ECHO,_T(""));
#ifndef NewUI
	  CMEcho.Content(IDC_ECHO, _T(""));
#endif
      return 0;
	}

    ///////////////////////////////////////////////////////////////////////
    long PSSMFrame::HideWindows(void)
	{
#ifndef NewUI
      SMSecList.Hide();
      SMMediaList.Hide();
      SMSMMItemList.Hide();
	  SMReceipt.Hide();
      SMReceiptRestricted.Hide();
      SMReceiptVoid.Hide();
      SMReceiptCoupon.Hide();
      SMReceiptVisualItem.Hide();
      SMInfoBox.Hide();
	  SMTBText.Hide();

      // Need to hide live video here also in case we scan directly
      // into store mode - we need to shut down live video in this case.
      CMVideoLarge.Hide();
      CMVideoSmall.Hide();
#endif
      return 0;
	}

    ///////////////////////////////////////////////////////////////////////
    PSAMFrame::PSAMFrame(LPCTSTR szWName) : PSFrame(szWName)
	{
      m_NumButtons = 2;
	}

    ///////////////////////////////////////////////////////////////////////
    long PSAMFrame::ClearMessages(void)
	{
      Content(IDC_ECHO,_T(""));
      Content(IDC_ECHO1,_T(""));
      return 0;
	}

    ///////////////////////////////////////////////////////////////////////
    long PSAMFrame::HideWindows(void)
	{
      return 0;
	}

    ///////////////////////////////////////////////////////////////////////
    long PSFrame::HideButtons()
	{
      int i;
      long rc = 0;

      for (i = 1; i <= m_NumButtons; i++)
	  {
        if (ButtonValid(i))
		{
          rc = Button(IDC_BUTTON1,PS_NOTEXT,false);
          if (rc != 0) return rc;
		}
	  }
	  
      return rc;
	}


    ///////////////////////////////////////////////////////////////////////
    long PSFrame::HideEcho(void)
    {
#ifndef NewUI
      CMEcho.Hide();
      CMPopupEcho.Hide();
#endif
      return 0;
    }

    ///////////////////////////////////////////////////////////////////////
    void PSFrame::SetBackground(const CString nBmp)
    {
      _TCHAR szWork[_MAX_PATH];
      _tmakepath(szWork,SCOTDRIVE,SCOTPATH SCOTIMAGE,nBmp,BMP_EXT);
      Content(IDC_BACKGROUND,szWork);
    }

    ///////////////////////////////////////////////////////////////////////
    long PSFrame::ShowFrame(LPCTSTR weight)
	{

      ClearMessages();
      HideWindows();
	  if (co.fStateDisplayWeightsAndMeasures)
      { 
        if (weight)                             // make sure frame has weight field. 
          Content(IDC_WEIGHT, weight, false);   // show weight
      } 
	  else
      {
		  if (0 != _tcscmp(szWindow, _T("wCMAlphaNumericFrame")))
		  {
			 if (0 != _tcscmp(szWindow, _T("wCMFrame")) )             // make sure frame has weight field.
			  Content(IDC_WEIGHT, NULL, true);      // hide weight
		  }
	  //here is not a good place to enable the help button
	  //now it's handled in PSProceduresBase::ShowFrame(bool bHelpState)
      //if (m_bHasHelpButton) 

        //Button(IDC_BUTTONHELP,BTT_HELP,true);

	  }
      return Show();
	}



long PSInterfaceErcpt::Remove(const long nIndex) // index is 1 based
{
	long lIndex = nIndex;
	--lIndex; // make it zero based index as required by ASUI receipt control
	lQuantity.RemoveAt(lIndex); // these lists are zero based index
	lWeight.RemoveAt(lIndex);
	lPrice.RemoveAt(lIndex);
	lUnitPrice.RemoveAt(lIndex);
	lTare.RemoveAt(lIndex);
	lRestrictedAge.RemoveAt(lIndex);
	lScanType.RemoveAt(lIndex);
	bAutomaticApproval.RemoveAt(lIndex);
	csExceptionCategory.RemoveAt(lIndex);
	bHasSecurityTag.RemoveAt(lIndex);
	int x1 = csItemCode.GetSize();
	csItemCode.RemoveAt(lIndex);
	int x2 = csItemCode.GetSize();
	trace(L7, _T("csItemCode Array size, before remove:%d, after remove:%d"), x1, x2);
	csItemDesc.RemoveAt(lIndex);
	lEntryIDList.RemoveAt(lIndex);
	long rc = PSInterfaceList1::Remove(lIndex); // call interfacelist routine with zero based index
	return rc;
} // of function


long PSInterfaceErcpt::InsertAt(
							   const long nIndex, // this index is one based
							   const bool fHighLight,
    						   LPCTSTR szDesc,
                               LPCTSTR szCode,
                               const long lQty,
                               const long lWgt,
                               const long lTcd,
                               const long lPrc,
                               const long lUPrc,	// lUnitPrice
							   const long lBarcodeType,
							   const long lEntryid,
							   const long lResAge,
							   LPCTSTR szCat,	//exception category
							   const bool bSecTag)     
{
	bool bTempApproval = false;
	long lIndex = nIndex;
	CString csTmp;
	--lIndex; // make it zero based index as required by ASUI receipt control
	lQuantity.InsertAt(lIndex, lQty); // these lists have zero based indexing
	lWeight.InsertAt(lIndex, lWgt);
	lPrice.InsertAt(lIndex,  lPrc);
	lUnitPrice.InsertAt(lIndex, lUPrc );
	lTare.InsertAt(lIndex, lTcd);
	lRestrictedAge.InsertAt(lIndex,  lResAge);
	lScanType.InsertAt(lIndex, lBarcodeType);
	bAutomaticApproval.InsertAt(lIndex, (BYTE) false);
	csExceptionCategory.InsertAt(lIndex, szCat);
	bHasSecurityTag.InsertAt(lIndex, (BYTE) bSecTag);
	int x1 = csItemCode.GetSize();
	csItemCode.InsertAt(lIndex, szCode);
	int x2 = csItemCode.GetSize();
	trace(L7, _T("csItemCode Array size, before insert:%d, after insert:%d"), x1, x2);
	csItemDesc.InsertAt(lIndex, szDesc);
	lEntryIDList.InsertAt(lIndex, lEntryid);
	long rc = PSInterfaceList1::InsertAt(lIndex, szDesc, fHighLight); // call interfacelist routine with zero based index
	//if (rc == PS_SUCCESS) // add needs a scroll up!
	//tar 182942
/*	{
		trace(L5, "Insert line at %d succeeded", lIndex);
		int nLength = csItemDesc.GetSize(); 
		const int nListLength = nLength;
		--nLength;
		CString csCurrentItemCode;
		while (nLength >= 0)
		{
			csCurrentItemCode = csItemCode.GetAt(nLength);
			bool bSelectableItem = false;
			if (csCurrentItemCode.GetLength())
			{
				bSelectableItem =  ((*csCurrentItemCode != INFO_ITEM) && (*csCurrentItemCode != VOID_ITEM) && (*csCurrentItemCode != LINK_ITEM));
			}
			if (bSelectableItem)
			{
				// a selectabe item was found from the bottom, make it selected
				trace(L7, "Select (zero based index) : %d", nLength);
				PSInterfaceList1::Select(nLength+1);
				break;
			}
			else
			{
				--nLength;
			}
		}				// of while
	} */ 					// if PS_SUCCESS
	return rc;
} // of function


long PSInterfaceList1::InsertAt(long nIndex, LPCTSTR szDesc,const bool fHighLight)
{
#ifndef NewUI
	_WHO(InsertAt);
	SELEMENT sEle = {0};
	PSRC rc = 0;
	long nFun;

	if (!hWnd) return 0;              // not loaded

	if (nIndex >= lCountList)
	{
		// add at the end
		nFun = PS_SF_ADD;
	}
	else
	{
		nFun = PS_SF_INSERT;
	}
	if (fHighLight)
	{
		nFun |= PS_SF_HIGHLIGHT;
	}

	sEle.nItemLength = -1;
    ++nLocalXtraValue; // increment this irrespective of showelement result, no harm
	// create new unique index for this item
	if (nFun == PS_SF_ADD)
	{
		rc = ShowElement(ListIdc,nFun,szDesc,0,0,sEle);
	}
	else if (nFun == PS_SF_INSERT)
	{
		rc = ShowElement(ListIdc,nFun,szDesc,0,nIndex,sEle);
	}

	lCountList++;	
	return rc;
#endif
return 0;
}


long PSInterfaceList1::Remove(const long nIndex)
{
#ifndef NewUI
	_WHO(Remove);
	SELEMENT sEle = {0};
	PSRC rc = 0;

	if (!hWnd) return 0;              // not loaded

	sEle.nItemLength = -1;
	long nFun = PS_SF_REMOVE;

	// create new unique index for this item
	rc = ShowElement(ListIdc,nFun,_T("Remove"),0,nIndex,sEle);
	if (CheckRC(__LINE__,C_FATAL,szWho,rc,ListIdc,_T("Remove"))) return rc;

	lCountList--;	
	return rc;
#endif
return 0;
}
#endif
