//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SCOTOpts.h
//
// TITLE:	Class header for SCOTOpts
//
// AUTHOR:	John Z. Bush
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 11/21/2002,	jzb.10.x,	Lowering of Security for Item Removals
//							jzb.10.3, SCOTOpts allows access to FastLane option settings
//
//////////////////////////////////////////////////////////////////////////////////////////////////

// ***** jzb.10.3
#if !defined(AFX_SCOTOPTS_H__E88FD816_EE44_4E77_91ED_EBA7A9BB784F__INCLUDED_)
#define AFX_SCOTOPTS_H__E88FD816_EE44_4E77_91ED_EBA7A9BB784F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SCOTOpts  
{
public:
	bool GetOptionStr( CString &sOption, CString &sValue, CString &sDefault);
	bool GetOptionFlag( CString &sOption, bool bDefault);
	SCOTOpts();
	virtual ~SCOTOpts();

private:
	void GetOption( CString &sOption, CString &sValue, bool bStoreProfile);

	CString m_sFastLaneConfigPath;
	CString m_sDefaultProfile;
	CString m_sStoreLevelProfile;
};

#endif // !defined(AFX_SCOTOPTS_H__E88FD816_EE44_4E77_91ED_EBA7A9BB784F__INCLUDED_)
// *****
