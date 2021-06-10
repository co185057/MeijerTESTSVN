// HeaderHandler.cpp: implementation of the CHeaderHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HeaderGenerator.h"
#include "HeaderHandler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHeaderHandler::CHeaderHandler(CString csFileName)
{
    csFileName.Replace(_T(".dll"), _T(""));
    m_csFileName = csFileName;
    OpenHeaderFiles(m_csFileName);
    if (!m_csPath.IsEmpty())
    {
        MakeNewFile();
    }
}

CHeaderHandler::~CHeaderHandler()
{
}

void CHeaderHandler::OpenHeaderFiles(CString csFileName)
{
    m_csHeaderFileInName = m_csFileName + _T(".tlh");
    m_csHeaderFileOutName = m_csFileName + _T(".temp");
    CFileException Ex;
    m_csPath.Empty();
    if (m_fHeaderFileIn.Open(m_csHeaderFileInName, CFile::modeRead |CFile::typeText, &Ex))
    {
        if(m_fHeaderFileOut.Open(m_csHeaderFileOutName, CFile::modeCreate | CFile::modeWrite | CFile::typeText, &Ex))
        {
            m_csPath = m_fHeaderFileIn.GetFilePath();
            m_csPath.Replace(m_csHeaderFileInName, "");
            m_csPath.MakeLower();
        }
        else
        {
            m_fHeaderFileIn.Close();
            _tprintf(_T("Unable to open output file %s\n"), m_csHeaderFileOutName);
        }
    }
    else
    {
        _tprintf(_T("Unable to open input file %s\n"), m_csHeaderFileInName);
    }
}

void CHeaderHandler::MakeNewFile()
{
    CFileException Ex;

    CString csLine;
    m_csFileContents.Empty();
    m_fHeaderFileIn.SeekToBegin();
    m_fHeaderFileOut.SetLength(0);
    m_fHeaderFileOut.SeekToBegin();
    int nCharsReplaced = 0;
    while (m_fHeaderFileIn.ReadString(csLine))
    {
       nCharsReplaced += ReplaceTLIInclude(csLine);
       m_fHeaderFileOut.WriteString(csLine + _T("\n"));
    }

    m_fHeaderFileIn.Close();
    m_fHeaderFileOut.Flush();
    m_fHeaderFileOut.Close();
    if (nCharsReplaced)
    {
        _tprintf(_T("Updating file %s\n"), m_csHeaderFileInName );
        m_fHeaderFileIn.Remove(m_csHeaderFileInName);
        m_fHeaderFileOut.Rename(m_csHeaderFileOutName, m_csHeaderFileInName);
    }
    else
    {
        _tprintf(_T("File %s not modified\n"), m_csHeaderFileInName);
        m_fHeaderFileOut.Remove(m_csHeaderFileOutName);
    }

   // delete pbufRead;
}

int CHeaderHandler::ReplaceTLIInclude(CString &csLine)
{
    CString csOldString = "\"" + m_csPath + m_csFileName + ".tli\"";
    CString csNewString = "<" + m_csFileName + ".tli>";
    return csLine.Replace(csOldString, csNewString);
}
