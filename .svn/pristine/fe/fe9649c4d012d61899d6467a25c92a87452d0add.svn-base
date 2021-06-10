// HeaderHandler.h: interface for the CHeaderHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEADERHANDLER_H__1849A5AA_E9BD_488A_A559_AAEDADCF1947__INCLUDED_)
#define AFX_HEADERHANDLER_H__1849A5AA_E9BD_488A_A559_AAEDADCF1947__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHeaderHandler  
{
public:
	CHeaderHandler(CString csFileName);
	virtual ~CHeaderHandler();

protected:
	int ReplaceTLIInclude(CString &csLine);
    void OpenHeaderFiles(CString csFileName);
    void MakeNewFile();
    CString m_csFileName;
    CStdioFile m_fHeaderFileIn;
    CStdioFile m_fHeaderFileOut;
    CString m_csHeaderFileInName;
    CString m_csHeaderFileOutName;
    CString m_csPath;
    CString m_csFileContents;
};

#endif // !defined(AFX_HEADERHANDLER_H__1849A5AA_E9BD_488A_A559_AAEDADCF1947__INCLUDED_)
