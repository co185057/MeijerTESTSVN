#include "stdafx.h"

#define ASSERT(e, m) _ASSERT(e)

#include "cstr.h"


TCHAR afxChNil = _T('\0');
static int rgInitData[] = { -1, 0, 0, 0 };
static CStringDataX* afxDataNil = (CStringDataX*)&rgInitData;
static LPCTSTR afxPchNil = (LPCTSTR)(((BYTE*)&rgInitData)+sizeof(CStringDataX));
const CString& AfxGetEmptyString()
	{ return *(CString*)&afxPchNil; }

CString::CString()
{
	Init();
}

CString::CString(const CString& stringSrc)
{
	ASSERT(stringSrc.GetData()->nRefs != 0, _T("nRefs != 0"));
	if (stringSrc.GetData()->nRefs >= 0)
	{
		ASSERT(stringSrc.GetData() != afxDataNil, _T("GetData != Nil"));
		m_pchData = stringSrc.m_pchData;
		InterlockedIncrement(&GetData()->nRefs);
	} else {
		Init();
		*this = stringSrc.m_pchData;
	}
}

void CString::AllocBuffer(int nLen)
{
	ASSERT(nLen >= 0, _T("Invalid length"));
	ASSERT(nLen <= 65534, _T("Invalid length"));    // max size (enough room for 1 extra)

	if (nLen == 0)
		Init();
	else
	{
		CStringDataX* pData =
			(CStringDataX*)new BYTE[sizeof(CStringDataX) + (nLen+1)*sizeof(TCHAR)];
		pData->nRefs = 1;
		pData->data()[nLen] = _T('\0');
		pData->nDataLength = nLen;
		pData->nAllocLength = nLen;
		m_pchData = pData->data();
	}
}

void CString::Release()
{
	if (GetData() != afxDataNil)
	{
		ASSERT(GetData()->nRefs != 0, _T("nRefs != 0"));
		if (InterlockedDecrement(&GetData()->nRefs) <= 0)
			delete[] (BYTE*)GetData();
		Init();
	}
}

void PASCAL CString::Release(CStringDataX* pData)
{
	if (pData != afxDataNil)
	{
		ASSERT(pData->nRefs != 0, _T("nRefs != 0"));
		if (InterlockedDecrement(&pData->nRefs) <= 0)
			delete[] (BYTE*)pData;
	}
}

void CString::Empty()
{
	if (GetData()->nDataLength == 0)
		return;
	if (GetData()->nRefs >= 0)
		Release();
	else
		*this = &afxChNil;
	ASSERT(GetData()->nDataLength == 0, _T("bogus"));
	ASSERT(GetData()->nRefs < 0 || GetData()->nAllocLength == 0, _T("bogus"));
}

void CString::CopyBeforeWrite()
{
	if (GetData()->nRefs > 1)
	{
		CStringDataX* pData = GetData();
		Release();
		AllocBuffer(pData->nDataLength);
		memcpy(m_pchData, pData->data(), (pData->nDataLength+1)*sizeof(TCHAR));
	}
	ASSERT(GetData()->nRefs <= 1, _T("bogus"));
}

void CString::AllocBeforeWrite(int nLen)
{
	if (GetData()->nRefs > 1 || nLen > GetData()->nAllocLength)
	{
		Release();
		AllocBuffer(nLen);
	}
	ASSERT(GetData()->nRefs <= 1, _T("bogus"));
}

CString::~CString()
{
	if (GetData() != afxDataNil)
	{
		if (InterlockedDecrement(&GetData()->nRefs) <= 0)
			delete[] (BYTE*)GetData();
	}
}

void CString::AllocCopy(CString& dest, int nCopyLen, int nCopyIndex,
	 int nExtraLen) const
{
	int nNewLen = nCopyLen + nExtraLen;
	if (nNewLen == 0)
	{
		dest.Init();
	} else {
		dest.AllocBuffer(nNewLen);
		memcpy(dest.m_pchData, m_pchData+nCopyIndex, nCopyLen*sizeof(TCHAR));
	}
}

//////////////////////////////////////////////////////////////////////////////
// More sophisticated construction

CString::CString(LPCTSTR lpsz)
{
	Init();

	int nLen = Safestrlen(lpsz);
	if (nLen != 0)
	{
		AllocBuffer(nLen);
		memcpy(m_pchData, lpsz, nLen*sizeof(TCHAR));
	}
}

CString::CString(LPCWSTR lpsz)
{
	Init();
	int nSrcLen = lpsz != NULL ? wcslen(lpsz) : 0;
	if (nSrcLen != 0)
	{
		AllocBuffer(nSrcLen*2);
		_wcstombsz(m_pchData, lpsz, (nSrcLen*2)+1);
		ReleaseBuffer();
	}
}

void CString::AssignCopy(int nSrcLen, LPCTSTR lpszSrcData)
{
	AllocBeforeWrite(nSrcLen);
	memcpy(m_pchData, lpszSrcData, nSrcLen*sizeof(TCHAR));
	GetData()->nDataLength = nSrcLen;
	m_pchData[nSrcLen] = _T('\0');
}

const CString& CString::operator=(const CString& stringSrc)
{
	if (m_pchData != stringSrc.m_pchData)
	{
		if ((GetData()->nRefs < 0 && GetData() != afxDataNil) ||
			stringSrc.GetData()->nRefs < 0)
		{
			// actual copy necessary since one of the strings is locked
			AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
		}
		else
		{
			// can just copy references around
			Release();
			ASSERT(stringSrc.GetData() != afxDataNil, _T("bogus"));
			m_pchData = stringSrc.m_pchData;
			InterlockedIncrement(&GetData()->nRefs);
		}
	}
	return *this;
}

const CString& CString::operator=(LPCTSTR lpsz)
{
	AssignCopy(Safestrlen(lpsz), lpsz);
	return *this;
}

const CString& CString::operator=(LPCWSTR lpsz)
{
	int nSrcLen = lpsz != NULL ? wcslen(lpsz) : 0;
	AllocBeforeWrite(nSrcLen*2);
	_wcstombsz(m_pchData, lpsz, (nSrcLen*2)+1);
	ReleaseBuffer();
	return *this;
}

void CString::ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, int nSrc2Len, LPCTSTR lpszSrc2Data)
{
  // -- master concatenation routine
  // Concatenate two sources
  // -- assume that 'this' is a new CString object

	int nNewLen = nSrc1Len + nSrc2Len;
	if (nNewLen != 0)
	{
		AllocBuffer(nNewLen);
		memcpy(m_pchData, lpszSrc1Data, nSrc1Len*sizeof(TCHAR));
		memcpy(m_pchData+nSrc1Len, lpszSrc2Data, nSrc2Len*sizeof(TCHAR));
	}
}

void CString::ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData)
{
	if (nSrcLen == 0)
		return;

	if (GetData()->nRefs > 1 || GetData()->nDataLength + nSrcLen > GetData()->nAllocLength)
	{
		CStringDataX* pOldData = GetData();
		ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData);
		ASSERT(pOldData != NULL, _T("bogus"));
		CString::Release(pOldData);
	} else {
		// fast concatenation when buffer big enough
		memcpy(m_pchData+GetData()->nDataLength, lpszSrcData, nSrcLen*sizeof(TCHAR));
		GetData()->nDataLength += nSrcLen;
		ASSERT(GetData()->nDataLength <= GetData()->nAllocLength, _T("bogus"));
		m_pchData[GetData()->nDataLength] = _T('\0');
	}
}

const CString& CString::operator+=(LPCTSTR lpsz)
{
	ConcatInPlace(Safestrlen(lpsz), lpsz);
	return *this;
}

const CString& CString::operator+=(TCHAR ch)
{
	ConcatInPlace(1, &ch);
	return *this;
}

const CString& CString::operator+=(const CString& string)
{
	ConcatInPlace(string.GetData()->nDataLength, string.m_pchData);
	return *this;
}

LPTSTR CString::GetBuffer(int nMinBufLength)
{
	ASSERT(nMinBufLength >= 0, _T("bogus"));

	if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength)
	{
		CStringDataX* pOldData = GetData();
		int nOldLen = GetData()->nDataLength;   // AllocBuffer will tromp it
		if (nMinBufLength < nOldLen)
			nMinBufLength = nOldLen;
		AllocBuffer(nMinBufLength);
		memcpy(m_pchData, pOldData->data(), (nOldLen+1)*sizeof(TCHAR));
		GetData()->nDataLength = nOldLen;
		CString::Release(pOldData);
	}
	ASSERT(GetData()->nRefs <= 1, _T("bogus"));

	ASSERT(m_pchData != NULL, _T("bogus"));
	return m_pchData;
}

void CString::ReleaseBuffer(int nNewLength)
{
	CopyBeforeWrite();  

	if (nNewLength == -1)
		nNewLength = lstrlen(m_pchData); 

	ASSERT(nNewLength <= GetData()->nAllocLength, _T("bogus"));
	GetData()->nDataLength = nNewLength;
	m_pchData[nNewLength] = _T('\0');
}

LPTSTR CString::GetBufferSetLength(int nNewLength)
{
	ASSERT(nNewLength >= 0, _T("bogus"));

	GetBuffer(nNewLength);
	GetData()->nDataLength = nNewLength;
	m_pchData[nNewLength] = _T('\0');
	return m_pchData;
}

void CString::FreeExtra()
{
	ASSERT(GetData()->nDataLength <= GetData()->nAllocLength, _T("bogus"));
	if (GetData()->nDataLength != GetData()->nAllocLength)
	{
		CStringDataX* pOldData = GetData();
		AllocBuffer(GetData()->nDataLength);
		memcpy(m_pchData, pOldData->data(), pOldData->nDataLength*sizeof(TCHAR));
		ASSERT(m_pchData[GetData()->nDataLength] == _T('\0'), _T("bogus"));
		CString::Release(pOldData);
	}
	ASSERT(GetData() != NULL, _T("bogus"));
}

LPTSTR CString::LockBuffer()
{
	LPTSTR lpsz = GetBuffer(0);
	GetData()->nRefs = -1;
	return lpsz;
}

void CString::UnlockBuffer()
{
	ASSERT(GetData()->nRefs == -1, _T("bogus"));
	if (GetData() != afxDataNil)
		GetData()->nRefs = 1;
}
          
BSTR AllocSysString()
{
	return NULL;
}
          
BSTR CString::AllocSysString() const
{
#if defined(_UNICODE) || defined(OLE2ANSI)
	BSTR bstr = ::SysAllocStringLen(m_pchData, GetData()->nDataLength);
	if(bstr == NULL)
		return NULL;
	return bstr;
#else
#if defined(_WIN32)
	int nLen = MultiByteToWideChar(CP_ACP, 0, m_pchData, GetData()->nDataLength, NULL, NULL);
	BSTR bstr = ::SysAllocStringLen(NULL, nLen);
	if (bstr != NULL)
	   MultiByteToWideChar(CP_ACP, 0, m_pchData, GetData()->nDataLength, bstr, nLen);
	return bstr;
#else
	return ::SysAllocStringLen(m_pchData, GetData()->nDataLength);
#endif	
#endif

}

int CString::Find(TCHAR ch) const
{
	// find first single character
	LPTSTR lpsz = _tcschr(m_pchData, (_TUCHAR)ch);

	// return -1 if not found and index otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

int CString::FindOneOf(LPCTSTR lpszcharSet) const
{
	LPTSTR lpsz = _tcspbrk(m_pchData, lpszcharSet);
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

void CString::MakeUpper()
{   
	CopyBeforeWrite();
	_tcsupr(m_pchData);
}

void CString::MakeLower()
{
	CopyBeforeWrite();
	_tcslwr(m_pchData);
}

void CString::SetAt(int nIndex, TCHAR ch)
{
	ASSERT(nIndex >= 0, _T("bogus"));
	ASSERT(nIndex < GetData()->nDataLength, _T("bogus"));

	CopyBeforeWrite();
	m_pchData[nIndex] = ch;
}

int __cdecl _wcstombsz(LPTSTR mbstr, const wchar_t* wcstr, size_t count)
{
	if (count == 0 && mbstr != NULL)
		return 0;

	int result = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1,
		mbstr, count, NULL, NULL);
	ASSERT(mbstr == NULL || result <= (int)count, _T("bogus"));
	if (result > 0)
		mbstr[result-1] = 0;
	return result;
}

int __cdecl _mbstowcsz(wchar_t* wcstr, LPCTSTR mbstr, size_t count)
{
	if (count == 0 && wcstr != NULL)
		return 0;

	int result = ::MultiByteToWideChar(CP_ACP, 0, mbstr, -1,
		wcstr, count);
	ASSERT(wcstr == NULL || result <= (int)count, _T("bogus"));
	if (result > 0)
		wcstr[result-1] = 0;
	return result;
}

LPWSTR AfxA2WHelper(LPWSTR lpw, LPCTSTR lpa, int nChars)
{
	if (lpa == NULL)
		return NULL;
	ASSERT(lpw != NULL, _T("bogus"));
	lpw[0] = _T('\0');
	MultiByteToWideChar(CP_ACP, 0, lpa, -1, lpw, nChars);
	return lpw;
}

LPTSTR AfxW2AHelper(LPTSTR lpa, LPCWSTR lpw, int nChars)
{
	if (lpw == NULL)
		return NULL;
	ASSERT(lpa != NULL, _T("bogus"));
	lpa[0] = _T('\0');
	WideCharToMultiByte(CP_ACP, 0, lpw, -1, lpa, nChars, NULL, NULL);
	return lpa;
}

