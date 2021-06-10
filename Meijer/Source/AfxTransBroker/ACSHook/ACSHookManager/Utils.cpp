#include "StdAfx.h"
#include "Utils.h"
#include "ACSHook.h"
#include "TraceImpl.h"

CUtils::CUtils() : m_pContexts(NULL), m_pControls(NULL), m_pProps(NULL) 
{
}


CUtils::~CUtils()
{

}

double CUtils::GetMS()
{
	static BOOL bOnce = FALSE;
	static LARGE_INTEGER liFreq = { 0 };
	if( bOnce )
	{
		bOnce = FALSE;
		::QueryPerformanceFrequency(&liFreq);	// counts per second
	}
	LARGE_INTEGER liCounter = { 0 };
	::QueryPerformanceCounter(&liCounter);		// counts

	return ((double)liCounter.QuadPart / (double)liFreq.QuadPart) / (double)1000;
}
double CUtils::GetMSDiff(double dLastMS)
{
	return dLastMS - GetMS();
}

CString CUtils::GetLastErrorString()
{
    return GetLastErrorString(GetLastError());
}

CString CUtils::GetLastErrorString(DWORD dwError)
{
   Trace trace(_T("CUtils::GetLastErrorString"), false);

    LPVOID pMsg;

    if( FormatMessage( 
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &pMsg,
        0,
        NULL ) )
    {
        CString str = (LPCTSTR)pMsg;
        LocalFree(pMsg);
        return str;
    }

    trace(DCM_ERROR, _T("There was an error formatting the last system error for display."));
    return _T("");
}

void CUtils::HexTrace(DWORD dcMask, const BYTE * data, long length)
{
    Trace trace(_T("HexTrace"));
    const long CHARS_PER_LINE = 8;
    trace(dcMask, _T("Hex dump length: %d"), length);

    if (length==0) return;

    long lines = length/CHARS_PER_LINE + 1;

    LPTSTR szWork = new TCHAR[  length*3 + 
                                + lines*(3+1+CHARS_PER_LINE) + //00 00 00 00   ....\n
                                100];  
    LPTSTR p;
    const BYTE * b;
    long len = length;

    p = szWork;
    b = data;

    long lStart = 0;
    *p++ = _T('\n');
    while(lStart < length)
    {
        long lEnd = min(lStart+CHARS_PER_LINE, length);
        for(long l = lStart; l < lEnd; l++)
        {
            _stprintf(p,_T(" %02x"),b[l-lStart]);
            p += 3;
        }
        _tcscat(p, _T("   "));
        p += 3;
        for(long l = lStart; l < lEnd; l++)
        {
            if( _istcntrl(*b) )
            {
                _tcscat(p++, _T("."));
                b++;
            }
            else
            {
                _stprintf(p++,_T("%c"),*b++);
            }
        }
        _tcscat(p++, _T("\n"));
        lStart = lEnd;
    }

    // Replace format specifiers and special commands with literal text
	 trace(dcMask) << szWork << NCR::Util::endl;
    delete [] szWork;
}

CUtils::CACSHookStub::CACSHookStub() 
	: m_pACSHook(NULL)
{

}

CUtils::CACSHookStub::~CACSHookStub()
{
	m_pACSHook = NULL;
}

bool CUtils::CACSHookStub::SendInput(LPCTSTR pszInput)
{
	return m_pACSHook && SUCCEEDED(m_pACSHook->SendInput(_bstr_t(pszInput)));
}


bool CUtils::CACSHookStub::SendControlToTB(LPCTSTR pszControl)
{
	return m_pACSHook && m_pACSHook->SendControlToTB(pszControl);
}

bool CUtils::CACSHookStub::SendControlToApp(LPCTSTR pszControl)
{
	return m_pACSHook && m_pACSHook->SendControlToApp(pszControl);
}


void CUtils::CACSHookStub::FireEvent(long lEvent, long lData, LPCTSTR szData1, LPCTSTR szData2)
{
	if ( m_pACSHook )
	{
		m_pACSHook->FireEvent(lEvent, lData, szData1, szData2 );
	}
}

CUtils::CAppModelStub::CAppModelStub() 
	: m_pAppModel(NULL)
{

}

CUtils::CAppModelStub::~CAppModelStub()
{
	m_pAppModel = NULL;
}

