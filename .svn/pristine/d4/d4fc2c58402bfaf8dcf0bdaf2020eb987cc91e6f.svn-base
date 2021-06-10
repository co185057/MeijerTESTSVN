/////////////////////////////////////////////////////////////////////////////////////
//	Header file: PSLoadStrings.h
//	Declaration file included in PSProcedure class declaration in PSProcedures.h
//	
//
/////////////////////////////////////////////////////////////////////////////////////


#ifndef _PSLOADSTRINGS_
#define _PSLOADSTRINGS_

public:

const enum SCOTSTRLIST_TYPE
{
	BMP_SCOTCORE,
	BMP_SCOTUSERDEF,
	// @@@INFOPOINT
	BMP_SCOTMASCOT,	
	// @@@INFOPOINT
    MSG_SCOTCORE_LANG_ONE,
    MSG_SCOTCORE_LANG_TWO,
    MSG_SCOTCORE_LANG_THREE,
    MSG_SCOTCORE_LANG_FOUR,
    MSG_SCOTCORE_LANG_FIVE,
    MSG_SCOTCORE_LANG_MAX,
	MSG_SCOTUSERDEF_LANG_ONE,
	MSG_SCOTUSERDEF_LANG_TWO,
	MSG_SCOTUSERDEF_LANG_THREE,
	MSG_SCOTUSERDEF_LANG_FOUR,
	MSG_SCOTUSERDEF_LANG_FIVE,
	MSG_SCOTUSERDEF_LANG_MAX,
	WAVE_SCOTWAVE,
	MSG_SCOTTB_LANG_ONE,
	MSG_SCOTTB_LANG_TWO,
	MSG_SCOTTB_LANG_THREE,
	MSG_SCOTTB_LANG_FOUR,
	MSG_SCOTTB_LANG_FIVE,
	MSG_SCOTTB_LANG_MAX,
	SCOTSTRLIST_LAST
};

typedef enum SCOTSTRLIST_TYPE tSCOTSTRLIST;

const enum tStringType
{
	BMP_ID,
   //@@@INFOPOINT
	MASCOT_BMP_ID,
	//@@@INFOPOINT
	WAVE_ID,
	MSG_ID
};
	
#ifndef _CPPUNIT
public:
	virtual CString GetStringValueFromID(int nStringID, tStringType nMessageType);
	virtual int		GetListSize(SCOTSTRLIST_TYPE iListType);
	virtual CString GetFileNameFromID(int nStringID, tStringType nMessageType);

// cm150000 - Start FastLane 3.1 Modification
	bool PutTerminalIDInReg(int nTerminalID);
// cm150000 - End FastLane 3.1 Modification

#endif //_CPPUNIT
//private:
protected:
#ifndef _CPPUNIT
    COverrideSCOTStringList* pSCOTStrTableList[SCOTSTRLIST_LAST];
	virtual CString GetSCOTStrListFileExt(SCOTSTRLIST_TYPE iFileType);
#endif //_CPPUNIT
	virtual BOOL	LoadSCOTStringConstants(CString csConfigPath);

	virtual BOOL	UnLoadSCOTStringConstants();
#ifndef _CPPUNIT
	virtual SCOTSTRLIST_TYPE GetCurrentListPtr(int& nCurrentStringID, tStringType nMessageType);
	 //A 000 messages
  virtual BOOL LoadSCOTStringConstantsOverrides(CString &csConfigPath);
  virtual BOOL RefreshStringList(int iListID,
                                 CString &csConfigPath,
                                 CString &csFileName,
                                 int iTerminal);
#endif //_CPPUNIT
  virtual CString GetMSLocaleID(const CString& csLangCode);
  CString GetLanguageCode(const CString& csLangCode); 
  //E 000 messages

#endif // _PSLOADSTRINGS_