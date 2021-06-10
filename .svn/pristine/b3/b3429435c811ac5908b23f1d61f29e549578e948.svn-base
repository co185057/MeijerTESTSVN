// CustomerMHMessages.h: interface for the CBaseMessages class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BaseMessages_H__INCLUDED_)
#define AFX_BaseMessages_H__INCLUDED_

#include "SingleInstance.h"
#include "OverrideScotStringList.h"
#include "ScotStringList.h"

#ifndef SCUD_BASE
/// \def SC.ot CU.stomer D.efined string constants or IDs start with 5000
#define SCUD_BASE 9000			
#endif

#ifndef BEST_BASE
/// \def SC.ot CU.stomer D.efined base entry state IDs start with 5000
#define BEST_BASE 5000			
#endif

#include "scudefs.h"  // contains definitions for SCUDMSGS

/// \def Maximum supported message list
/// \brief 2 for now - scot messaged and scud messages.
//+RFQ 3267 and 3265
#define MAXSUPPORTEDLIST 5 

/// /enum Message File Types
/// /brief Type of Message files used by Transaction Broker
const enum MsgFileType
{
	SCOTMSGS, /// < enum SCOTMSGS  value 0
    SCUDMSGS, /// < enum SCUDMSGS  value 1
    SCUDMSGS2, /// < enum SCUDMSGS2 value 2
    SCUDMSGS3, /// < enum SCUDMSGS2 value 3 //+RFQ 3267 and 3265
    SCUDMSGS4, /// < enum SCUDMSGS2 value 4 //+RFQ 3267 and 3265
};

//////////////////////////////////////////////////////////////////////
///
/// CBaseMessages is a Singleton class that responsible to manage all 
/// the message files used by Transaction Broker.
///
///	@author:
///
//////////////////////////////////////////////////////////////////////
class CBaseMessages : public CSingleInstance<CBaseMessages>
{
    friend class CSingleInstance<CBaseMessages>;

public:
	///////////////////////////////////////////////////////////////////
	/// Destructor
	///////////////////////////////////////////////////////////////////
    virtual ~CBaseMessages();

	///////////////////////////////////////////////////////////////////
	/// ReadInSCOTMessageFiles, reads in FL msg files enabling shared 
	/// us by TB defined in TBBaseOpts.dat.
	///
	/// @param NONE
	/// @return VOID
	///////////////////////////////////////////////////////////////////
    virtual void ReadInSCOTMessageFiles(void);

	///////////////////////////////////////////////////////////////////
	/// GetStringValueFromID, given a string id on the scudefs file, 
	///		lookup and return the string associated with it..
	///
	/// @param int nStringID - the scudefs.h defined string ID
	/// @param bool bForceTranslate - will translate the string regardless of whether or not we're currently in store or assist mode.
	/// @return CString - the string value, empty if ID not found
	///////////////////////////////////////////////////////////////////
    virtual CString GetStringValueFromID(int nStringID, bool bForceTranslate = false);

protected:
    CBaseMessages();  // should not be called outside of this class (instance function)
    
private:
	///////////////////////////////////////////////////////////////////
	/// Constructor
	///
	///	@param CBaseMessages - intentionally hide copy const
	///////////////////////////////////////////////////////////////////	
    CBaseMessages(const CBaseMessages &);              

	///////////////////////////////////////////////////////////////////
	/// Override for '=' operator
	///
	///	@param CBaseMessages - reference to CBaseMessages
	///////////////////////////////////////////////////////////////////	
    CBaseMessages & operator =(const CBaseMessages &); 

	///////////////////////////////////////////////////////////////////
	/// ReadInMainFiles, This function does the actual work reading in 
	///		the files and saving the data to a list.
	///
	/// @param CString csFileName - the name of the file to read in (full path info)
	/// @param MsgFileType nMsgType - the enum value - used to differentiate between lists
	///
	/// @return VOID
	///////////////////////////////////////////////////////////////////
    virtual void ReadInMainFiles(CString csFileName, MsgFileType nMsgType);
    
	//////////////////////////////////////////////////////////////////////
	/// ReadInSCOTMessageOverrideFiles, after having read in the main option
	///      file, this function attempts to load the override files (the
	///      .000 and .xxx files).
	///
	/// @param CString csFileName - the name of the file to read in (full path info)
	/// @param MsgFileType nMsgType - the enum value - used to differentiate between lists
	/// 
	/// @return BOOL - TRUE=success; FALSE=failure
	/////////////////////////////////////////////////////////////////////////////
	virtual BOOL ReadInSCOTMessageOverrideFiles(CString csFileName, MsgFileType nMsgType);

	/// COverrideSCOTStringList var to store the message lists.
	///	The list is an array with the size of #MAXSUPPORTEDLIST.
    COverrideSCOTStringList* m_pSCOTStrTableList[MAXSUPPORTEDLIST];

    bool m_bMultipleSecondaryLanguageSupport;
};

#endif 

