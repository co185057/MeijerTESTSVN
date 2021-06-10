// FLReportTextFile.h: interface for the FLReportTextFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLREPORTTEXTFILE_H__C015B214_5E70_4B21_BEC9_14161A455E30__INCLUDED_)
#define AFX_FLREPORTTEXTFILE_H__C015B214_5E70_4B21_BEC9_14161A455E30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#import <msxml4.dll> 
using namespace MSXML2;

class FLReportTextFile  
{
public:
	long GetNextEntry(CString &csLanguage, CString &csFieldName, CString &csFieldValue);
	CString m_csCurrentLanguage;
	long NextLanguage();
	long Open(LPCTSTR lpFileName);
	FLReportTextFile();
	virtual ~FLReportTextFile();
private:
   	MSXML2::IXMLDOMDocumentPtr m_pxmlTextDocument;
	MSXML2::IXMLDOMNodePtr m_pxmlCurrentNode;
	MSXML2::IXMLDOMNodePtr m_pxmlCurrentTextEntry;
	bool m_bOpened;

};

#endif // !defined(AFX_FLREPORTTEXTFILE_H__C015B214_5E70_4B21_BEC9_14161A455E30__INCLUDED_)
