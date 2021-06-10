// FLReportTextFile.cpp: implementation of the FLReportTextFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FLReportTextFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace std;

FLReportTextFile::FLReportTextFile()
: m_bOpened(false),
  m_pxmlCurrentNode(NULL),
  m_pxmlTextDocument(NULL),
  m_pxmlCurrentTextEntry(NULL)
{

}

FLReportTextFile::~FLReportTextFile()
{
    if (NULL != m_pxmlCurrentNode)
    {
        m_pxmlCurrentNode = NULL;
    }
    if (NULL != m_pxmlTextDocument)
    {
        m_pxmlTextDocument = NULL;
    }
    if (NULL != m_pxmlCurrentTextEntry)
    {
        m_pxmlCurrentTextEntry = NULL;
    }

}

long FLReportTextFile::Open(LPCTSTR lpFileName)
{
	BOOL isSuccessful;
    CFileFind finder;
    long lRetCode = 0;
    HRESULT hr;
    MSXML2::IXMLDOMNodePtr pxmlLanguageAttributesNode;
    try
    {
        if (finder.FindFile(lpFileName))
        {

			hr = m_pxmlTextDocument.CreateInstance(__uuidof(MSXML2::DOMDocument));
		    // Assuming coinitialized
            //Make sure that the DOM document object was successfully created
            if (FAILED(hr))
            {
                cout << _T("Text object CreateInstance Failure") << endl;
    		    return -1;
            }

		    variant_t vFileName = lpFileName;

		    isSuccessful = m_pxmlTextDocument->load(vFileName);
            if(FALSE == isSuccessful)
            {

			    long errLine = m_pxmlTextDocument->parseError->line;
			    long errPos =m_pxmlTextDocument->parseError->linepos;
			    _bstr_t errReason = m_pxmlTextDocument->parseError->reason;
			    _bstr_t errDescription = vFileName;
			    errDescription += _T(": ");
			    errDescription += errReason;
			    CString csDescription = (wchar_t*)errReason;
                CString csErrorLine;
                csErrorLine.Format(_T("--FLReportTextFile::Open> (FLRPTERROR_CANTLOAD) line [%d] pos [%d] Description: %s"), errLine, errPos, (LPCTSTR) csDescription);
                cout << (LPCTSTR)csErrorLine << endl;

                return -1;                 
            }

		    // Now set the current node pointer to the top of the list
		    m_pxmlCurrentNode = m_pxmlTextDocument->GetdocumentElement()->GetfirstChild();
            while ((m_pxmlCurrentNode != NULL)  && (m_pxmlCurrentNode->nodeType == NODE_COMMENT))
            {
                m_pxmlCurrentNode = m_pxmlCurrentNode->GetnextSibling();
            }
            if (NULL == m_pxmlCurrentNode)
            { 
                // No first language
                return -1;
            }
            pxmlLanguageAttributesNode = m_pxmlCurrentNode->Getattributes()->getNamedItem(_T("LanguageName"));
            if (NULL == pxmlLanguageAttributesNode)
            {
                cout << _T("FLReportTextFile::Open - No language name") << endl;
                return -1;
            }
            else
            {
                m_csCurrentLanguage = (LPCTSTR)((_bstr_t)pxmlLanguageAttributesNode->GetnodeValue());
                m_pxmlCurrentTextEntry = m_pxmlCurrentNode->GetfirstChild();
            }
    		m_bOpened = true;  // Set flag to show we've successfully loaded the menu
        }
        else
        {
            lRetCode = -1;
        }

        return lRetCode;
    }
    catch (_com_error e)
    {
        cout << _T("-FLReportTextFile::Open COM Error") << endl;
        return -1;                   
    }
    catch (...)
    {
        cout << _T("-FLReportTextFile::Open Generic Exception") << endl;
        return -1;                   
    }

}

long FLReportTextFile::NextLanguage()
{
    MSXML2::IXMLDOMNodePtr pxmlLanguageAttributesNode;
    try
    {
        m_pxmlCurrentNode = m_pxmlCurrentNode->GetnextSibling();
        while ((m_pxmlCurrentNode != NULL)  && (m_pxmlCurrentNode->nodeType == NODE_COMMENT))
        {
            m_pxmlCurrentNode = m_pxmlCurrentNode->GetnextSibling();
        }
        if (m_pxmlCurrentNode == NULL)
        {
            // Last Language processed
            return 0;
        }
        else
        {
            // Get the current language name and save it
            pxmlLanguageAttributesNode = m_pxmlCurrentNode->Getattributes()->getNamedItem(_T("LanguageName"));
            if (NULL == pxmlLanguageAttributesNode)
            {
                cout << _T("FLReportTextFile::Open - No language name") << endl;
                return -1;
            }
            else
            {
                m_csCurrentLanguage = (LPCTSTR)((_bstr_t)pxmlLanguageAttributesNode->GetnodeValue());
                m_pxmlCurrentTextEntry = m_pxmlCurrentNode->GetfirstChild();
            }
            return 1;
        }

    }
    catch (_com_error e)
    {
        cout << _T("-FLReportTextFile::NextLanguage COM Error") << endl;
        return -1;                   
    }
    catch (...)
    {
        cout << _T("-FLReportTextFile::NextLanguage Generic Exception") << endl;
        return -1;                   
    }
}

long FLReportTextFile::GetNextEntry(CString &csLanguage, CString &csFieldName, CString &csFieldValue)
{
    MSXML2::IXMLDOMNodePtr pxmlAttributesNode;

    try
    {
        while ((m_pxmlCurrentTextEntry != NULL)  && (m_pxmlCurrentTextEntry->nodeType == NODE_COMMENT))
        {
            m_pxmlCurrentTextEntry = m_pxmlCurrentTextEntry->GetnextSibling();
        }
        if (m_pxmlCurrentTextEntry == NULL)
        {
            // Last Language processed
            return 0;
        }
        else
        {
            // Get the current entry and save it
            csLanguage = m_csCurrentLanguage;
            pxmlAttributesNode = m_pxmlCurrentTextEntry->Getattributes()->getNamedItem(_T("ID"));
            if (NULL == pxmlAttributesNode)
            {
                cout << _T("FLReportTextFile::GetNextEntry - No ID") << endl;
                return -1;
            }
            csFieldName = (LPCTSTR)((_bstr_t)pxmlAttributesNode->GetnodeValue());
            pxmlAttributesNode = m_pxmlCurrentTextEntry->Getattributes()->getNamedItem(_T("EntryText"));
            if (NULL == pxmlAttributesNode)
            {
                cout << _T("FLReportTextFile::GetNextEntry - No EntryText") << endl;
                return -1;
            }
            csFieldValue = (LPCTSTR)((_bstr_t)pxmlAttributesNode->GetnodeValue());
            m_pxmlCurrentTextEntry = m_pxmlCurrentTextEntry->GetnextSibling();
            return 1;
        }

    }
    catch (_com_error e)
    {
        cout << _T("-FLReportTextFile::NextLanguage COM Error") << endl;
        return -1;                   
    }
    catch (...)
    {
        cout << _T("-FLReportTextFile::NextLanguage Generic Exception") << endl;
        return -1;                   
    }
}
