#ifndef _IPSPROCEDURES_H
#define _IPSPROCEDURES_H

#include "ConfigObjectConstants.h"  // Need SCOT_LANGUAGE_TYPE
#include "PSProceduresConstants.h"  // Need CONTEXT_TYPE

class IPSProcedures
{
public:
    IPSProcedures() {}
    virtual ~IPSProcedures() {}
	// general purpose, called by all modes
      // BeginCodeLocale
      typedef enum SCREEN_POSITION {
          SP_OTHER,
          SP_ERECEIPTCOL1,
          SP_ERECEIPTCOL2,
          SP_NONE,
      };

      //dp185016 tar 299311
      typedef enum FORMAT_OPTION {
          DEFAULT = 0,
          REPORTINGFORMAT,
          CASHMANAGEMENTFORMAT,
      };

      //+SR828
      typedef enum HKStatus{
          HK_ACUTEON,
          HK_GRAVEON,
          HK_CIRCUMFLEXON,
          HK_UMLAUTON,
          HK_TILDEON,
          HK_NORMAL
      };

    /****************************************
     * PSX methods
     ****************************************/

    /****************************************
     * PSProcedures methods
     ****************************************/

    virtual void SetLanguage(SCOT_LANGUAGE_TYPE) = 0;
    virtual SCOT_LANGUAGE_TYPE GetLanguage(void) = 0;
    virtual CString GetPSText(const CString csMsg, SCOT_LANGUAGE_TYPE language) = 0;
    virtual CString GetPSText(const int nMsg) = 0;   // return a pointer to the PS text string
    virtual CString GetPSText(const int nMsg, SCOT_LANGUAGE_TYPE language) = 0;   // return a pointer to the PS text string in the given language
    virtual long ConvertSCOTLangID2LCID(const CString& csISOCode, CString& csLCIDCode) = 0;
    virtual bool IsPickListItem(const CString& csItemCode) = 0;
    virtual void SetFoundPickListItemsFromSearch(bool b) = 0;
    virtual bool GetFoundPickListItemsFromSearch() = 0;
    virtual long GetInput(CString &) = 0;
    virtual long GetPicklistItemsSold() = 0;

	
	virtual CString FmtDollar(const long lCents, SCREEN_POSITION sp, FORMAT_OPTION bReporting)=0;
    virtual void ShowChangeDueAmountPaid( const long,const long, bool bChangeDueText = true)=0;   
	
    
};

#endif // _IPSPROCEDURES_H
