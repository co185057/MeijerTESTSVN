#ifndef _CSTRING_H
#define _CSTRING_H

#include <string.h>
#include <tchar.h>

#ifndef ASSERT
#define ASSERT(e, m) _ASSERT(e)
#endif

struct CStringDataX
{
   long nRefs;     // reference count
   int nDataLength;
   int nAllocLength;

   TCHAR* data()
      { return (TCHAR*)(this+1); }
};

class CString
{
public:
	// Constructors
	CString();
	CString(const CString& stringSrc);
	CString(TCHAR ch, int nRepeat = 1);
	CString(LPCTSTR lpsz);
	CString(LPCWSTR lpsz);
	CString(LPCTSTR lpch, int nLength);
	
	CString(const _TUCHAR* psz);

	int GetLength() const;
	BOOL IsEmpty() const;
	void Empty();                       // free up the data

	TCHAR GetAt(int nIndex) const;      // 0 based
	TCHAR operator[](int nIndex) const; // same as GetAt
	void SetAt(int nIndex, TCHAR ch);
	operator LPCTSTR() const;           // as a C string

	// overloaded assignment
	const CString& operator=(const CString& stringSrc);
	const CString& operator=(TCHAR ch);
	const CString& operator=(LPCTSTR lpsz);
	const CString& operator=(LPCWSTR lpsz);
	const CString& operator=(const _TUCHAR* psz);

	// string concatenation
	const CString& operator+=(const CString& string);
	const CString& operator+=(TCHAR ch);
	const CString& operator+=(LPCTSTR lpsz);

	// string comparison
	int Compare(LPCTSTR lpsz) const;         // straight character
	int CompareNoCase(LPCTSTR lpsz) const;   // ignore case

	// simple sub-string extraction
	CString Mid(int nFirst, int nCount) const;
	CString Mid(int nFirst) const;
	CString Left(int nCount) const;
	CString Right(int nCount) const;

	// upper/lower/reverse conversion
	void MakeUpper();
	void MakeLower();
	void MakeReverse();

	// trimming whitespace (either side)
	void TrimRight();
	void TrimLeft();

	// searching (return starting index, or -1 if not found)
	// look for a single character match
	int Find(TCHAR ch) const;               // like "C" strchr
	int ReverseFind(TCHAR ch) const;
	int FindOneOf(LPCTSTR lpszcharSet) const;

	// look for a specific sub-string
	int Find(LPCTSTR lpszSub) const;        // like "C" strstr

	// Access to string implementation buffer as "C" character array
	LPTSTR GetBuffer(int nMinBufLength);
	void ReleaseBuffer(int nNewLength = -1);
	LPTSTR GetBufferSetLength(int nNewLength);
	void FreeExtra();

	//
	// make BSTR
	//
	BSTR AllocSysString() const;

	// Use LockBuffer/UnlockBuffer to turn refcounting off
	LPTSTR LockBuffer();
	void UnlockBuffer();

// Implementation
public:
	~CString();
	int GetAllocLength() const;
	void Init();

protected:
	LPTSTR m_pchData;   // pointer to ref counted string data

	// implementation helpers
	CStringDataX* GetData() const;
	void AllocCopy(CString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
	void AllocBuffer(int nLen);
	void AssignCopy(int nSrcLen, LPCTSTR lpszSrcData);
	void ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, int nSrc2Len, LPCTSTR lpszSrc2Data);
	void ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData);
	void CopyBeforeWrite();
	void AllocBeforeWrite(int nLen);
	void Release();
	static void PASCAL Release(CStringDataX* pData);
	static int PASCAL Safestrlen(LPCTSTR lpsz);
};

extern TCHAR afxChNil;
const CString& AfxGetEmptyString();
#define afxEmptyString AfxGetEmptyString()

// conversion helpers
int __cdecl _wcstombsz(LPTSTR mbstr, const wchar_t* wcstr, size_t count);
int __cdecl _mbstowcsz(wchar_t* wcstr, LPCTSTR mbstr, size_t count);

// CString
inline CStringDataX* CString::GetData() const
        { ASSERT(m_pchData != NULL, _T("Null Pointer")); return ((CStringDataX*)m_pchData)-1; }
inline void CString::Init()
{ 
	m_pchData = afxEmptyString.m_pchData; 
}
inline CString::CString(const _TUCHAR* lpsz)
        { Init(); *this = (LPCTSTR)lpsz; }
inline const CString& CString::operator=(const _TUCHAR* lpsz)
        { *this = (LPCTSTR)lpsz; return *this; }
inline int CString::GetLength() const
        { return GetData()->nDataLength; }
inline int CString::GetAllocLength() const
        { return GetData()->nAllocLength; }
inline BOOL CString::IsEmpty() const
        { return GetData()->nDataLength == 0; }
inline CString::operator LPCTSTR() const
        { return m_pchData; }
inline int PASCAL CString::Safestrlen(LPCTSTR lpsz)
        { return (lpsz == NULL) ? 0 : lstrlen(lpsz); }

// CString support (windows specific)
inline int CString::Compare(LPCTSTR lpsz) const
        { return _tcscmp(m_pchData, lpsz); }    // MBCS/Unicode aware
inline int CString::CompareNoCase(LPCTSTR lpsz) const
        { return _tcsicmp(m_pchData, lpsz); }   // MBCS/Unicode aware

inline TCHAR CString::GetAt(int nIndex) const
{
        ASSERT(nIndex >= 0, _T("Invalid Index"));
        ASSERT(nIndex < GetData()->nDataLength, _T("Invalid Index"));
        return m_pchData[nIndex];
}
inline TCHAR CString::operator[](int nIndex) const
{
        // same as GetAt
        ASSERT(nIndex >= 0, _T("Invalid Index"));
        ASSERT(nIndex < GetData()->nDataLength, _T("Invalid Index"));
        return m_pchData[nIndex];
}

inline bool operator==(const CString& s1, const CString& s2)
{ return s1.Compare(s2) == 0; }

inline bool operator==(const CString& s1, LPCTSTR s2)
{ return s1.Compare(s1) == 0; }

inline bool operator==(LPCTSTR s1, const CString& s2)
{ return s2.Compare(s1) == 0; }

inline bool operator!=(const CString& s1, const CString& s2)
{ return s1.Compare(s2) != 0; }

inline bool operator!=(const CString& s1, LPCTSTR s2)
{ return s1.Compare(s2) != 0; }

inline bool operator!=(LPCTSTR s1, const CString& s2)
{ return s2.Compare(s1) != 0; }

inline bool operator<(const CString& s1, const CString& s2)
{ return s1.Compare(s2) < 0; }

inline bool operator<(const CString& s1, LPCTSTR s2)
{ return s1.Compare(s2) < 0; }

inline bool operator<(LPCTSTR s1, const CString& s2)
{ return s2.Compare(s1) > 0; }

inline bool operator>(const CString& s1, const CString& s2)
{ return s1.Compare(s2) > 0; }

inline bool operator>(const CString& s1, LPCTSTR s2)
{ return s1.Compare(s2) > 0; }

inline bool operator>(LPCTSTR s1, const CString& s2)
{ return s2.Compare(s1) < 0; }

inline bool operator<=(const CString& s1, const CString& s2)
{ return s1.Compare(s2) <= 0; }

inline bool operator<=(const CString& s1, LPCTSTR s2)
{ return s1.Compare(s2) <= 0; }

inline bool operator<=(LPCTSTR s1, const CString& s2)
{ return s2.Compare(s1) >= 0; }

inline bool operator>=(const CString& s1, const CString& s2)
{ return s1.Compare(s2) >= 0; }

inline bool operator>=(const CString& s1, LPCTSTR s2)
{ return s1.Compare(s2) >= 0; }

inline bool operator>=(LPCTSTR s1, const CString& s2)
{ return s2.Compare(s1) <= 0; }

#endif
