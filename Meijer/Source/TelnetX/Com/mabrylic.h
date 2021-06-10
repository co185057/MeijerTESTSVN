#ifdef _WIN32
#ifndef _MABRYLIC_H
#define _MABRYLIC_H

typedef struct ProductInfo {
		SHORT MabryIcon;
		SHORT ProductIcon;
		LPTSTR ProductID;
		LPTSTR SourceID;
		LPTSTR ProductDirectory;
		LPTSTR ProductName;
		LPTSTR ProductDescription;
		LPTSTR ProductVersion;
		LPTSTR ProductHelpFile;
		LPTSTR PackageID;
		LPTSTR PackageDirectory;
		LPTSTR PackageName;
		LPTSTR PackageDescription;
		SHORT CopyrightYear;
		LONG  LicenseFileKey;
		LPTSTR LicenseFileName;
		LONG  Products[ 10 ];
	} PRODUCTINFO;

#define CT_COMOBJECT	1
#define CT_ACTIVEXCTRL 2
#define CT_MFCCTRL 3

#ifdef __cplusplus
#define CFUNC extern _T("C")
#else
#define CFUNC 
#endif

CFUNC void MabryLicenseInitLow( PRODUCTINFO* ppi, int ComponentType, BOOL Sample, BOOL Debug, HINSTANCE hInst, LPTSTR LPSTRDate );
CFUNC void MabryLicenseAbout( void );
CFUNC BOOL MabryLicenseCheck( LPTSTR LicenseCode, BOOL ShowDialog );
CFUNC void MabryLicenseFileKey( LPTSTR LPTSTR );
CFUNC BOOL MabryLicenseCheckRun( LPTSTR LicenseCode );

#if _MSC_VER >= 1100
#define MODULEINSTANCE _Module.GetModuleInstance()
#else
#define MODULEINSTANCE m_hInstance
#endif

#ifdef _DEBUG
#define MabryLicenseInit(PPI,CT) MabryLicenseInitLow(PPI,CT,FALSE,TRUE,MODULEINSTANCE,__DATE__)
#else
#ifdef _SHAREWARE
#define MabryLicenseInit(PPI,CT) MabryLicenseInitLow(PPI,CT,TRUE,FALSE,MODULEINSTANCE,__DATE__)
#else
#define MabryLicenseInit(PPI,CT) MabryLicenseInitLow(PPI,CT,FALSE,FALSE,MODULEINSTANCE,__DATE__)
#endif
#endif

#ifndef _BUILDLICLIB
template <class T, const CLSID* pclsid>
class CMabryLicense
{
public:
	CMabryLicense()
	{
		m_RuncheckPassed = FALSE;
	}
protected:
	BOOL		m_RuncheckPassed;
	CComBSTR	m_LicenseKey;

	class CLicense
	{
	public:
		BOOL VerifyLicenseKey(BSTR bstr)
		{
			USES_CONVERSION;

			return MabryLicenseCheck( OLE2T(bstr), FALSE );
		}
		BOOL GetLicenseKey(DWORD dwReserved, BSTR* pBstr)
		{
			USES_CONVERSION;
			_TCHAR szKey[20];

			if ( pBstr == NULL )
			  return FALSE;

			MabryLicenseFileKey( szKey );
			*pBstr = SysAllocString( T2OLE( szKey ));
			return ( *pBstr != NULL );
		}
		BOOL IsLicenseValid()
		{
			return MabryLicenseCheck( NULL, TRUE );
		}
	};
	HRESULT _RuntimeCheck()
	{
		USES_CONVERSION;
		
		if( m_RuncheckPassed == FALSE )
		{
			if( (m_RuncheckPassed = MabryLicenseCheckRun(OLE2T(m_LicenseKey))) == FALSE )
				return T::Error(_T("Invalid or missing LicenseKey"), *pclsid, CUSTOM_CTL_SCODE(32000));
		}
		return S_OK;
	}
};

#define RuntimeCheck() \
{ \
	HRESULT hr = _RuntimeCheck(); \
	if( hr != S_OK ) \
		return hr; \
}
#endif

#else // _WIN32

#define FORCE_NONE	0
#define FORCE_YES	1
#define FORCE_NO	2

BOOL SharewareDialog( LPTSTR LPSTRAppName, int iReserved );
BOOL CheckLicense( LPTSTR lpstrBaseFileName, long lLicenseKey );


#endif // _MABRYLIC_H
#endif // _WIN32
