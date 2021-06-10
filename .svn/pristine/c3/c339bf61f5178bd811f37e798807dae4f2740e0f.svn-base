#ifndef __SCOTPRICESOUND_H
#define __SCOTPRICESOUND_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef SCOTPriceSound_BUILD_DLL

#define PriceSound_EXPORT dllexport

// CSCOTPriceSoundPSXApp
// See SCOTPriceSoundPSX.cpp for the implementation of this class
//

class CSCOTPriceSoundPSXApp : public CWinApp
{
public:
	CSCOTPriceSoundPSXApp();
    ~CSCOTPriceSoundPSXApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	static UINT ConvertFastlaneLanguageToLCID( const CString& str );
	BOOL ScotSoundPlaySound( const CString& strSound, DWORD nInterrupt, DWORD nInterruptible, HANDLE hWaitEvent=0 );	

    BOOL m_bRemote;
    CmDataCapture m_dataCapture;
	UINT m_nDefaultLCID;
	DWORD m_nDisplayTargetMask;
	UINT m_nLCID;
	DWORD m_nMessageInterrupt;
	DWORD m_nMessageInterruptible;
	DWORD m_nPriceInterrupt;
	DWORD m_nPriceInterruptible;
	DWORD m_nSecurityInterrupt;
	DWORD m_nSecurityInterruptible;
    CPSX *m_pPSX;
	CStringArray m_strExtensions;
    CString m_strSoundPath;
    CMap<UINT,UINT,CString,LPCTSTR> m_currencyCodeMap;
	        
	DECLARE_MESSAGE_MAP()
};

#else

#define PriceSound_EXPORT dllimport

#endif

extern __declspec(PriceSound_EXPORT) BOOL SCOTPriceSoundSayPrice( DWORD );
extern __declspec(PriceSound_EXPORT) BOOL SCOTPriceSoundSayMessage( LPCTSTR );
extern __declspec(PriceSound_EXPORT) BOOL SCOTPriceSoundSaySecurity( long, LPCTSTR, HANDLE hWaitEvent=0 );
extern __declspec(PriceSound_EXPORT) BOOL SCOTPriceSoundSayMidiSound( LPCTSTR );
extern __declspec(PriceSound_EXPORT) BOOL SCOTPriceSoundSaySignal( long );
extern __declspec(PriceSound_EXPORT) BOOL SCOTPriceSoundInit( LPCTSTR );
extern __declspec(PriceSound_EXPORT) BOOL SCOTPriceSoundFindSoundFilePath( LPTSTR, unsigned long );
extern __declspec(PriceSound_EXPORT) BOOL SCOTPriceSoundSetLanguage( LPCTSTR, LPCTSTR );
extern __declspec(PriceSound_EXPORT) BOOL SCOTSaySecurityAllowInterrupt( bool );
extern __declspec(PriceSound_EXPORT) BOOL SCOTPriceSoundConnectToServer( LPCTSTR, int, int );
extern __declspec(PriceSound_EXPORT) BOOL SCOTPriceSoundDisconnectFromServer( BOOL );
extern __declspec(PriceSound_EXPORT) BOOL SCOTPriceSoundInitializeRemote( bool );
extern __declspec(PriceSound_EXPORT) long SCOTPriceSoundUninitializeRemote();


#ifdef __cplusplus
}
#endif

#endif