// BaseMHOptions.h: interface for the CBaseMHOptions class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
///
/// \file BaseMHOptions.h
/// \brief interface for the CBaseMHOptions class.
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////////
#if !defined(AFX_BaseMHOptions_H__INCLUDED_)
#define AFX_BaseMHOptions_H__INCLUDED_

#include "afxtempl.h"
#include "transbroker.h"

//////////////////////////////////////////////////////////////////////
///
/// CBaseMHOptions is an implementaion of CBaseMHOptions
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////
class CBaseMHOptions
{
public:
	//////////////////////////////////////////////////////////////////////
	///
	/// Destructor
	///
	//////////////////////////////////////////////////////////////////////
    virtual ~CBaseMHOptions();
    
	//////////////////////////////////////////////////////////////////////
	///
	/// Initialize - reads in the base and customer level option files, sets default
	///       values for options that need values.
	///
	///	@return TBRC
	//////////////////////////////////////////////////////////////////////
    virtual TBRC Initialize();
    
	//////////////////////////////////////////////////////////////////////
	///
	/// IsAllowed - The application can use this function to query TB to see 
	///       whether or not a certain feature is allowable in the current state.
	///
	///	@param TBFEATURE nFeature - feature (TBFEATURE enum)
	///	@return BOOL - TRUE:permitted; FALSE:not permitted
	//////////////////////////////////////////////////////////////////////
    virtual bool IsAllowed(TBFEATURE nFeature);

	//////////////////////////////////////////////////////////////////////
	///
	/// GetTerminalNumber - Return POS client terminal number. FastLane assumes 
	///                       positive terminal numbers only. Permits negative
	///                       numbers to be used to return error status.
	///
	///	@return long - terminal number
	//////////////////////////////////////////////////////////////////////
    virtual long GetTerminalNumber(void);

	//////////////////////////////////////////////////////////////////////
	///
	/// GetVersion - Return the version number and information of the TB.
	///                Version string conforms to the below format:
	///                "FastLane TB Ver xx.xx.xx for Solution Name"
	///                This should be set using the CBaseCurrentOptions::SetVersionString
	///                function. Base TB does not currently set this.
	///
	///	@param LPTSTR szVersionString - Buffer to contain the version string
	///	@param long nBufferLength - Buffer length
	///	@return long - Length of the actual version string, including the NULL 
	///                 character. If this is bigger than nBufferLength then the 
	///                 buffer passed is not long enough to copy the entire version 
	///                 string, but nBufferLength characters are still copied into 
	///                 the buffer.
	//////////////////////////////////////////////////////////////////////
    virtual long GetVersion(LPTSTR szVersionString, long nBufferLength);

	//////////////////////////////////////////////////////////////////////
	///
	/// SetOptionValue - can be used to set the value of an option that was read
	///       in from the option file. When you override it through this function,
	///       the value in the option file will also be changed.
	///
	///	@param CString& strSectionName - name of section in option file
	///	@param CString& strOptName - option name
	///	@param CString & strOptValue - option value
	///	@return bool - true=success; false=failure
	//////////////////////////////////////////////////////////////////////
    bool SetOptionValue(const CString& strSectionName, const CString& strOptName, const CString & strOptValue);

	//////////////////////////////////////////////////////////////////////
	///
	/// DetermineTerminalNumber - Read in Terminal number from registry
	///
	///	@param HKEY hKey - handle to reg key that TB reads from
	///	@return int - terminal number
	//////////////////////////////////////////////////////////////////////
	virtual int DetermineTerminalNumber(HKEY hKey);
    
	//////////////////////////////////////////////////////////////////////
	///
	/// OptionValue - retrieve value of option passed as parameter
	///
	///	@param CString& strOptName - option to lookup in option map
	///	@return CString - value of option
	//////////////////////////////////////////////////////////////////////
    CString OptionValue( const CString& strOptName);

	//////////////////////////////////////////////////////////////////////
	///
	/// BoolOptionValue - just calls BoolVal, which will return a bool value 
	///       for the option passed in as the parameter
	///
	///	@param CString& strOptName - option to lookup in option map
	///	@return bool - value of option
	//////////////////////////////////////////////////////////////////////
    bool BoolOptionValue(const CString& strOptName);

	//////////////////////////////////////////////////////////////////////
	///
	/// IntOptionValue - just calls BoolVal, which will return a bool value 
	///       for the option passed in as the parameter
	///
	///	@param CString& strOptName - option to lookup in option map
	/// @return bool - value of option
	//////////////////////////////////////////////////////////////////////
    int  IntOptionValue(const CString& strOptName);

	//////////////////////////////////////////////////////////////////////
	///
	/// BoolVal - returns a bool value for the option passed in as the parameter
	///
	///	@param CString strVal - option to lookup in option map
	///	@return bool - value of option
	//////////////////////////////////////////////////////////////////////
    bool BoolVal(CString strVal);

	//////////////////////////////////////////////////////////////////////
	///
	/// IntVal - returns an int value for the option passed in as the parameter
	///
	///	@param CString strVal - option to lookup in option map
	///	@return int - value of option
	//////////////////////////////////////////////////////////////////////
    int  IntVal(CString strVal);
    
protected:
	//////////////////////////////////////////////////////////////////////
	///
	/// Constructor
	///
	//////////////////////////////////////////////////////////////////////
    CBaseMHOptions();

	//////////////////////////////////////////////////////////////////////
	///
	/// GetOptionFileName - reads in the base or customer (depending on parameter)
	///       option file name from the registry
	///
	///	@param bool bCustFile - return the customer file? False = base file.
	///	@return CString - full path info of option file name
	//////////////////////////////////////////////////////////////////////
    CString GetOptionFileName(bool bCustFile);

    CMapStringToString m_cmOptionMap;	///< Object for option map
    CMapStringToString m_cmDefaults;	///< Object for default map

private:
	//////////////////////////////////////////////////////////////////////
	///
	/// Hide Copy Const
	///
	//////////////////////////////////////////////////////////////////////
    CBaseMHOptions(CBaseMHOptions &);                    // hide copy const

	//////////////////////////////////////////////////////////////////////
	///
	/// Hide Assignment
	///
	//////////////////////////////////////////////////////////////////////
    CBaseMHOptions & operator =(const CBaseMHOptions &); // hide assignment

	//////////////////////////////////////////////////////////////////////
	///
	/// CreateOptionMap - reads in the .dat, .000, and .xxx option files to create
	///       the option map.
	///
	///	@param CString& szFileName = the filename with the .xxx (terminal num) extension
	//////////////////////////////////////////////////////////////////////
    void CreateOptionMap(const CString& szFileName);
    
	//////////////////////////////////////////////////////////////////////
	///
	/// LoadStandardOptions, loads base options
	///
	//////////////////////////////////////////////////////////////////////
    void LoadStandardOptions();

	//////////////////////////////////////////////////////////////////////
	///
	/// LoadCustomerOptions, loads customer options
	///
	//////////////////////////////////////////////////////////////////////
    void LoadCustomerOptions();
    
    CRITICAL_SECTION m_CritSectOptions; ///< critical section for option handler
 
};

#endif // !defined(AFX_BaseMHOptions_H__D525C84F_D10A_4F2C_93CA_3E26BE313951__INCLUDED_)
