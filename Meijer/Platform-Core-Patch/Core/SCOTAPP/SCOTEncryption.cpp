// SCOTEncryption.cpp: implementation of the SCOTEncryption class.
//
//////////////////////////////////////////////////////////////////////

#include "dll.h"

//#include "stdafx.h"
#include <afxwin.h>                 // MFC core and standard components

#include "TraceObject.h"
#include "CommonServices.h"
#include "SCOTEncryption.h"
//#include <iostream>

#include "ErrorObject.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SCOTEncryption")



USING_NAMESPACE(CryptoPP)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SCOTEncryption::SCOTEncryption()
{
}

SCOTEncryption::~SCOTEncryption()
{
}

void SCOTEncryption::Initialize()
{
    trace(L6, _T("+SCOTEncryption::Initialize"));
}


CString SCOTEncryption::ByteToHexString(unsigned char in[], int length)
{
	trace(L6, _T("+SCOTEncryption::ByteToHexString"));

	unsigned char ch = 0x00;

	if(in==NULL)
		return _T("");
	
	const CString pseudo[] = {_T("0"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("F")};

	CString result = _T("");

	int i=0;
	while(i < length)
	{
		ch = (in[i] & 0xF0);
		ch = (ch >> 4);
		ch = (ch & 0x0F);

		result += pseudo[(int)ch];

		ch = (in[i] & 0x0F);

		result += pseudo[(int)ch];
        
		i++;
	}
	trace(L6, _T("-SCOTEncryption::ByteToHexString"));
	return result;
}


//use the SHA-256 encryption calls to create a hashed version of a string

CString SCOTEncryption::ComputeSHA256( CString& message )
{
	trace(L6, _T("+SCOTEncryption::ComputeSHA256"));
	SHA256 sha;
	byte digest[32];

	int length = message.GetLength();
	LPTSTR p = message.GetBuffer(length);

	sha.Update((BYTE*)p, length * sizeof(TCHAR) );

    sha.Final(digest);


	if (!sha.VerifyDigest(digest, (BYTE*)p, length * sizeof(TCHAR)))
	{
		trace(L6, _T("SHA-256 hash failed"));
		message.ReleaseBuffer();
		return _T("");
	}
	trace(L6, _T("SHA-256 hash succeeded"));
	message.ReleaseBuffer();
	
	trace(L6, _T("-SCOTEncryption::ComputeSHA256"));
	
	return ByteToHexString(digest, sizeof(digest));
	
}

