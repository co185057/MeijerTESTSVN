    /////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // FILE:  PSProcedures.H
    //
    // TITLE:   Display Presentation Services Interface Procedures (High Level)
    //
    // This module handles all DPS calls for the SCOT application so that no other
    // component need know the details of DPS calls and all other components are
    // insulated from changes to the DPS API.
    //
    // From here we call the PSInterface routines
    //
    //
    // AUTHOR:  Peter Denhoed, Updated DcL-
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    #ifndef _PSPROCEDURES
    #define _PSPROCEDURES
    
  #include "PSProceduresBase.h"

  #ifdef _SCOTSSF_
  class DLLEXPORT
  #else
  class DLLIMPORT
  #endif
   PSProcedures : public PSProceduresBase// presentation services procedures (high level)
    {
    	
    public:

		PSProcedures();
		
 #ifdef _TESTSSF
		void ShowCardAmountList(void);
		void ShowGiftCardActivityList(void);
	private:
		void PutToSafeArray(int nIndex, COleSafeArray& saArray, CString csString);

 #endif

    };
    
    #endif
