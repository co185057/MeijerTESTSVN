// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DEVICEERRORLOOKUP_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DEVICEERRORLOOKUP_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.


#ifdef DEVICEERRORLOOKUP_EXPORTS
#define DEVICEERRORLOOKUP_API __declspec(dllexport)
#else
#define DEVICEERRORLOOKUP_API __declspec(dllimport)
#endif


//Load the device list and initialize logging
DEVICEERRORLOOKUP_API bool DeviceErrorInitialize();
//Unload the device list and clean up
DEVICEERRORLOOKUP_API bool DeviceErrorUnInitialize();

//Set the terminal number in the external dll so that it can read the terminal override message files
DEVICEERRORLOOKUP_API bool SetTerminalNumber(long nTermNumber);

//Creates or returns the html file name based on device class, device model and errors
DEVICEERRORLOOKUP_API bool GetDeviceErrorHTML(LPTSTR sHTML,int DeviceClass, LPTSTR sDeviceModel, LPTSTR sDeviceSubModel,
					int StatusEventId, long ExtendedResultCode, int nDataLength, void* sData, bool* bCheckHealth = NULL, int nContext = -1, LPTSTR sLCID=NULL);
//Is this error ignored?
DEVICEERRORLOOKUP_API bool IsDeviceErrorIgnored(int DeviceClass, LPTSTR sDeviceModel, LPTSTR sDeviceSubModel,int StatusEventId, long ExtendedResultCode);

//Replace the text area in sHTMLFile with input sText
DEVICEERRORLOOKUP_API bool UpdateDeviceErrorHTMLText(LPTSTR sHTMLFile, LPTSTR csText, bool bAppend=false);

//Replace the sToken in the html file sHTMLFile with the string sParms.   For example, insert the amount to the string "Failed to dispense %s" in html file.
DEVICEERRORLOOKUP_API bool ParseDeviceErrorHTML(LPTSTR sHTMLFile, LPTSTR sParms, LPTSTR sToken);

//Get currect title1 text
DEVICEERRORLOOKUP_API bool GetCurrentTitleText(LPTSTR sTitle1);
//Get currect title2 text
DEVICEERRORLOOKUP_API bool GetCurrentTitle2Text(LPTSTR sTitle2);

DEVICEERRORLOOKUP_API  int GetCurrentMessageTextLength();
//Get current message text
DEVICEERRORLOOKUP_API bool GetCurrentMessageText(LPTSTR sMsgText);
//Get current button1 text
DEVICEERRORLOOKUP_API bool GetCurrentButton1Text(LPTSTR sButtonText);

//Replace or update title in the html file sHTMLFile with the string csTitle
DEVICEERRORLOOKUP_API bool UpdateDeviceErrorHTMLTitle(LPTSTR sHTMLFile, LPTSTR csTitle, bool bAppend=false);	//SSCOB-517

//Replace or update sub title in the html file sHTMLFile with the string csSubTitle
DEVICEERRORLOOKUP_API bool UpdateDeviceErrorHTMLSubTitle(LPTSTR sHTMLFile, LPTSTR csSubTitle, bool bAppend=false);	//SSCOB-517


