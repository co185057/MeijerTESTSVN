//////////////////////////////////////////////////////////////////////////////
//
// File:  AppRestartCount.cpp
//
// Description: This is an entirely static class, i.e. there are all
//              class variables and functions, no instance variables or
//              functions, so no object is required.  This class can be
//              used to save a value in the registry that is stored across
//              invocations of SCOTAPP. In this way we can keep track of
//              the number of times SCOTAPP made an attempt to restart
//              itself in case of an exception.
//
//////////////////////////////////////////////////////////////////////////////
 
#include "stdafx.h"
#include "AppRestartCount.h"

// internal static constants
const HKEY        AppRestartCount::APPRESTARTCOUNT_KEY = HKEY_LOCAL_MACHINE;
LPCTSTR const AppRestartCount::APPRESTARTCOUNT_SUBKEY = _T("SOFTWARE\\NCR\\SCOTAPP");
LPCTSTR const AppRestartCount::APPRESTARTCOUNT_VALUE = _T("RestartAttempts");

//------------------------------------------------------------
// reset()
//
// Set the registry restart value to 0. Returns ERROR_SUCCESS
// normally, or other rc from Reg call on error.
//------------------------------------------------------------
long AppRestartCount::reset()
{
  HKEY  hKey;				 // handle of subkey
  DWORD disposition; // like a return code
  long  rc;

  // Create this subkey, or just open if it exists.  It's volatile, so
  // it won't exist across system reboots.  That won't matter if all
  // works correctly, but in case for some reason the restart value
  // doesn't get reset, at least a reboot will wipe it out.
  rc = RegCreateKeyEx( APPRESTARTCOUNT_KEY,   // handle of main key
		                   APPRESTARTCOUNT_SUBKEY,// addr of subkey name
		                   0,                     // reserved
		                   0,                     // no type for subkey value?
		                   REG_OPTION_VOLATILE,   // won't be saved across system reboots
		                   KEY_ALL_ACCESS,        // desired security access
		                   NULL,                  // default security, don't care
		                   &hKey,                 // subkey handle is returned
		                   &disposition );        // returns either REG_CREATED_NEW_KEY or
	                                            // REG_OPENED_EXISTING_KEY, we don't care
  if (rc != ERROR_SUCCESS) return rc;
	
  // Initialize the restart value to 0
  DWORD data = 0;
  rc = RegSetValueEx( hKey,                  // subkey handle from create/open
		                  APPRESTARTCOUNT_VALUE, // addr of value name
		                  0,                     // reserved
		                  REG_DWORD,             // value type
		                  (LPBYTE)&data,         // must cast to this type
		                  sizeof(data));         // data length

  if (rc != ERROR_SUCCESS) return rc;
	
  rc = RegCloseKey(hKey);
	
  return rc;
}

//------------------------------------------------------------
// increment()
//
// If the reg subkey doesn't exist, create it and set it to 1.
// If it does, increment it.  Returns ERROR_SUCCESS normally,
// or other rc from Reg call on error.
//------------------------------------------------------------
long AppRestartCount::increment()
{
  HKEY          hKey;							  // handle of subkey
  DWORD         disposition;			  // like a return code
  DWORD         appRestarts = 0;
  unsigned long dataLength = sizeof(appRestarts);
  long          rc;
	
  // Open the subkey (or create it if it doesn't exist)
  rc = RegCreateKeyEx( APPRESTARTCOUNT_KEY,    // handle of main key
		                   APPRESTARTCOUNT_SUBKEY, // addr of subkey name
		                   0,                      // reserved
		                   0,                      // no type for subkey value?
		                   REG_OPTION_VOLATILE,    // won't be saved across system reboots
		                   KEY_ALL_ACCESS,         // desired security access
		                   NULL,                   // default security, don't care
		                   &hKey,                  // subkey handle is returned
		                   &disposition );         // returns either REG_CREATED_NEW_KEY or
	                                             // REG_OPENED_EXISTING_KEY
  if (rc != ERROR_SUCCESS) return rc;
	
  // Treat the case where the subkey did not exist as if the value
  // were 0.  Set it to 1.
  if (disposition == REG_CREATED_NEW_KEY)
  {
    appRestarts = 1;
    rc = RegSetValueEx( hKey,                  // subkey handle from creation/opening
			                  APPRESTARTCOUNT_VALUE, // addr of value name
			                  0,                     // reserved
			                  REG_DWORD,             // value type
			                  (LPBYTE)&appRestarts,  // must cast to this type
			                  sizeof(appRestarts));	 // data length
    if (rc != ERROR_SUCCESS) return rc;
  }
	
  // This is the case we would expect.  Increment the value.
  else if (disposition == REG_OPENED_EXISTING_KEY)
  {
    // Get the restart value
    RegQueryValueEx( hKey,                  // subkey handle
			               APPRESTARTCOUNT_VALUE, // addr of value name
			               NULL,                  // reserved
			               NULL,                  // for returning value type - we already know it (REG_DWORD)
			               (LPBYTE)&appRestarts,  // returns restart value data, parm type casting required
			               &dataLength );         // returns length of data, we don't need, but required parm
		
    // Increment restart attempts
    appRestarts++;
		
    rc = RegSetValueEx( hKey,                  // subkey handle from creation/opening
			                  APPRESTARTCOUNT_VALUE, // addr of value name
			                  0,					 // reserved
			                  REG_DWORD,			 // value type
			                  (LPBYTE)&appRestarts,  // must cast to this type
			                  sizeof(appRestarts));	 // data length
    if (rc != ERROR_SUCCESS) return rc;
  }
	
  rc = RegCloseKey(hKey);
	
  return rc;
}

//-----------------------------------------------------------------
// getCount
//
// If the subkey doesn't exist, create it and return 0. If it does,
// return the value that exists. Set the return code parameter to
// ERROR_SUCCESS normally, or other rc from Reg call on error.
//-----------------------------------------------------------------
int AppRestartCount::getCount(long& rc)
{
  HKEY          hKey;							  // handle of subkey
  DWORD         disposition;			  // like a return code
  DWORD         appRestarts = 0;
  unsigned long dataLength = sizeof(appRestarts);
	
  // Open the subkey (create in case it doesn't exist)
  rc = RegCreateKeyEx( APPRESTARTCOUNT_KEY,    // handle of main key
		                   APPRESTARTCOUNT_SUBKEY, // addr of subkey name
		                   0,											 // reserved
		                   0,											 // no type for subkey value?
		                   REG_OPTION_VOLATILE,    // won't be saved across system reboots
		                   KEY_ALL_ACCESS,				 // desired security access
		                   NULL,									 // default security, don't care
		                   &hKey,									 // subkey handle is returned
		                   &disposition );				 // returns either REG_CREATED_NEW_KEY or
	                                             // REG_OPENED_EXISTING_KEY
  if (rc != ERROR_SUCCESS) return 0;
	
  // Treat the case where the subkey did not exist as if the value
  // were 0.
  if (disposition == REG_CREATED_NEW_KEY)
  {
    appRestarts = 0;
    rc = RegSetValueEx( hKey,                  // subkey handle from creation/opening
			                  APPRESTARTCOUNT_VALUE, // addr of value name
			                  0,					 // reserved
			                  REG_DWORD,			 // value type
			                  (LPBYTE)&appRestarts,  // must cast to this type
			                  sizeof(appRestarts));	 // data length
    if (rc != ERROR_SUCCESS) return 0;
		
    rc = RegCloseKey(hKey);
		
    return 0;
  }
	
  // Get the restart value
  rc = RegQueryValueEx( hKey,                  // subkey handle
		                    APPRESTARTCOUNT_VALUE, // addr of value name
		                    NULL,									 // reserved
		                    NULL,									 // for returning value type - we already know it (REG_DWORD)
		                    (LPBYTE)&appRestarts,  // returns restart value data, parm type casting required
		                    &dataLength );				 // returns length of data, we don't need, but required parm
  if (rc != ERROR_SUCCESS) return 0;
	
  rc = RegCloseKey(hKey);
	
  return (int)appRestarts;
}
