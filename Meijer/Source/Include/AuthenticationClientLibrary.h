//
// AuthenticationClientLibrary.h : Defines the initialization routines for the DLL.
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "AuthenticationClientLibraryAPI.h"
#include "FTConnect.h"
#include "ASAConnect.h"

//
// CAuthenticationClientLibraryApp
// See AuthenticationClientLibrary.cpp for the implementation of this class
//

class CAuthenticationClientLibraryApp : public CWinApp
{

public:
	CAuthenticationClientLibraryApp();
	virtual ~CAuthenticationClientLibraryApp();

protected:
	CString m_strModuleName;

// Overrides
public:
	virtual BOOL InitInstance();
	const CString & ModuleName() { return m_strModuleName; }

	BOOL Open(FT_GET_ACTION func);								// This method is called when the client application is started.
	BOOL Open_Net(FT_GET_ACTION_NET func);						// This method is called when the client application is started. .Net support
	BOOL GetFingerprint();										// This method uses the Digital Persona fingerprint reader API in order to collect 1 fingerprint for verification
	BOOL ConnectDevice();
	BOOL DisconnectDevice();
	BOOL RegisterFingerprint();									// This method uses the Digital Persona fingerprint reader API in order to collect 4 fingerprints for registration
	BOOL SaveUser(FT_SAVEUSER_PT pFPData);						// This method uses the ASAInterface to store the users information to the ASA server.
	BOOL DeleteUser(LPCTSTR pUserID);							// This method uses the ASAInterface to delete the users information from the ASA server.
	BOOL VerifyUser(FT_BYTE* pFeatures, int RecommendedFtrLen);	// This method uses the ASAInterface to verify a fingerprint with the ASA server. If the fingerprint is verified the user data is returned to the client application..
	BOOL GetUsers();											// This method uses the ASAInterface to get all the users that are stored on the ASA server.
	BOOL Abort();												// This method is used in order to terminate the getFingerprint/registerFingerprint/saveUser/deleteUser/verifyUser/getUsers methods.
	BOOL Purge();												// This method is used to purge old records in the database.
	BOOL Close();												// This method is called when the client application is exiting.
	BYTE* getBPBuffer(void);									// This method returns the display buffer. ORU uses the display buffer to draw the fingerprint.
	BOOL setRectDraw(int nWidth, int nHeight);					// This method sets width and height of display buffer size.
	unsigned int getNumberOfDevices(void) const;				// This method resturns the number of fingerprint devices connected to the machine.  This is called by ORU to determine if fingerprint device is present.
private:
	FTConnect	*m_pFTConnect;
	ASAConnect	*m_ASAConnection;
};



