/* +
-------------------------------------------------------------------------------

 Module Name     : SCOTPSApp.H
 Type            : Application include file for SCOTPS

 Language        : MSVC++
 Operating System: Win32
 Author/Location : SCOT 1.0 Team, RSG-Atlanta

 Copyright (C) NCR Corporation 1997, 1998

-------------------------------------------------------------------------------
 Date    | Changes Made                                      | Version | By
-------------------------------------------------------------------------------
 09/11/97  Major change in the ShowFlag bits. The PS_SF_HIDE   01.00.03  KSo
           now has bit shifted one nibble to the right to
           allow more bits for Visual Modification Flag.
           This is in response to an RFC to add support for
           another attribute in the Receipt.
           SF_HIGHLIGHT is no longer the same as SF_SELECT.
 02/11/98  Add support for PSEnableNotification.                         KSo
-------------------------------------------------------------------------------

 This is an application include file, to be distributed with SCOTPS SDK.
 
-------------------------------------------------------------------------------
$Archive: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/core/Include/SCOTPSApp.h $
$Revision: 3 $
$Date: 10/29/08 4:52p $
-------------------------------------------------------------------------------
*/

#ifndef  _SCOTPSApp_H
#define  _SCOTPSApp_H

#ifdef   __cplusplus
extern   "C" {
#endif   /* __cplusplus */

#ifdef  _SCOTPSDEV_H
   #define  APIExpose   __declspec(dllexport)
#else
   #define  APIExpose   __declspec(dllimport)
#endif

#include "RemoteObjectDefines.h"
/*
** Typedefs
*/
typedef long   PSRC;             /* PS return code, do not reduce #bits! ever */
typedef long   DEVID;            /* Device ID */
typedef long   HDEVCONTEXT;      /* Device context, no bit reduction! */
typedef bool   (CALLBACK *FPEVENTHANDLER)(UINT, WPARAM, LPARAM);
typedef unsigned short USHORT;
typedef unsigned int   UINT;
typedef unsigned long  ULONG;
typedef unsigned long  PSSHOWFLAG;
typedef unsigned long  PSELEMENTTYPE;

typedef struct {
   union {
   LPCTSTR  szPSName;         /* InParam for PSShowPresentationElement */
   LPCTSTR  szContent;        /* InParam for PSShowElement */
   LPTSTR   pchBuffer;        /* OutParam for PSGetElement */
   };
   long     lExtraData;       /* InParam for PSShowElement */
                              /* OutParam for PSGetElement */
   long     lBufferSize;      /* InParam for PSGetElement  */
   HWND     hWindow;          /* In and Out Param */
   int      nIndex;           /* In and Out Param */
   int		nItemLength;      /* Support receipt font changes */  
   long		lBitmapData;	  /* InParam for PSShowElement */
   long		lImageType;		  /* InParam for PSShowElement depends on Produce or Tender screen */
   PSSHOWFLAG   fShow;        /* In and Out Param */
} SELEMENT, *PSELEMENT;


/*
** Return codes
** -001 to -099 Run-time errors
** -100 to -199 Programming errors
** -200 to -299 Configuration errors
** -300 to -349 
** -350 to -399 System errors (out of memory, etc)
** -400 to -499 Printer PSM
** -500 to -599 Display PSM
** -600 to -699 Sound PSM
*/
const    PSRC  PS_SUCCESS              =    0;
const    PSRC  PS_ALREADYINITIALIZED   =    1;     /* SCOTPS has already been initialized */
const    PSRC  PS_NOSELECTION          =    2;     /* No item is selected */

const    PSRC  PS_E_BUFFERTOOSMALL     =  -50;     /* Buffer is too small for data copying */
const    PSRC  PS_E_INVALIDINDEX       =  -51;     /* Index passed is not valid */

const    PSRC  PS_E_NOTSUPPORTED       = -100;     /* Command is not supported */
const    PSRC  PS_E_INVALIDCALLBACK    = -101;     /* Callback function is not valid */
const    PSRC  PS_E_INVALIDDEVCONTEXT  = -102;     /* The hDC passed is not valid */
const    PSRC  PS_E_INVALIDPARAM       = -103;     /* At least one of the parameters is invalid */
const    PSRC  PS_E_INVALIDCOMMAND     = -104;     /* Command is not valid in the usage context */
const    PSRC  PS_E_INVALIDSHOWFLAG    = -105;     /* Specified show flag is invalid */
const    PSRC  PS_E_INVALIDPOINTER     = -106;     /* Specified pointer is invalid */

const    PSRC  PS_E_DEVICENOTSUPPORTED = -200;     /* No PSM configured for this device */
const    PSRC  PS_E_INVALIDPSM         = -201;     /* Invalid or non-existence PSM */
const    PSRC  PS_E_INVALIDPSFILE      = -202;     /* Invalid or non-existence PS or related files */
const    PSRC  PS_E_INVALIDPS          = -203;     /* Invalid or non-existence PS */
const    PSRC  PS_E_INVALIDTEMPLATE    = -204;     /* Invalid or non-existence template */
const    PSRC  PS_E_INVALIDWINDOW      = -205;     /* Invalid or non-existence window */
const    PSRC  PS_E_INVALIDELEMENT     = -206;     /* Invalid or non-existence element */
const    PSRC  PS_E_INVALIDMEDIA       = -207;     /* Invalid or non-existence media for use in PS */

const    PSRC  PS_E_OUTOFMEMORY        = -350;     /* Memory allocation error */
const    PSRC  PS_E_CREATEFAIL         = -351;     /* Creation of window failed */
const    PSRC  PS_E_LIVEVIDEO          = -352;     /* LiveVideo Message */
const    PSRC  PS_E_REMOTEINVALIDPARMS = -353;     /* Invalid number of remote parameters */
const    PSRC  PS_E_UNEXPECTED         = -399;     /* Really unexpected error */

/*
** fShow - Show Flags
** HIWORD(fShow) = Common show flag for all PSM
** LOWORD(fShow) = PSM-specific show flag
** 
*/
                                             /* VSF - Visual State Flags */
const PSSHOWFLAG    PS_SF_SHOW      = 0x00000000L;
const PSSHOWFLAG    PS_SF_HIDE      = 0x00010000L;
const PSSHOWFLAG    PS_SF_REFRESH   = 0x00020000L;

const PSSHOWFLAG    PS_SF_VSF_MASK  = 0x000F0000L;

                                             /* VMF - Visual Modifier Flags */
const PSSHOWFLAG    PS_SF_ENABLE    = 0x00000000L;
const PSSHOWFLAG    PS_SF_HIGHLIGHT = 0x00100000L;
const PSSHOWFLAG    PS_SF_DISABLE   = 0x00200000L;
const PSSHOWFLAG    PS_SF_STRIKETHRU= 0x00200000L;
const PSSHOWFLAG    PS_SF_FLASH     = 0x00400000L;

const PSSHOWFLAG    PS_SF_VMF_MASK  = 0x00F00000L;

                                             /* CMF - Content Modifier Flags */
const PSSHOWFLAG    PS_SF_ADD       = 0x00000000L;
const PSSHOWFLAG    PS_SF_INSERT    = 0x01000000L;
const PSSHOWFLAG    PS_SF_RESET     = 0x02000000L;
const PSSHOWFLAG    PS_SF_MODIFY    = 0x03000000L;
const PSSHOWFLAG    PS_SF_REMOVE    = 0x04000000L;
const PSSHOWFLAG    PS_SF_TOPINDEX  = 0x05000000L; //tar182942

const PSSHOWFLAG    PS_SF_CMF_MASK  = 0x0F000000L;

                                             /* NMF - Navigation Modifier Flags */
const PSSHOWFLAG    PS_SF_SELECT    = 0x10000000L;
const PSSHOWFLAG    PS_SF_UNSELECT  = 0x20000000L;
const PSSHOWFLAG    PS_SF_PAGEUP    = 0x30000000L;
const PSSHOWFLAG    PS_SF_PAGEDOWN  = 0x40000000L;
const PSSHOWFLAG    PS_SF_SCROLLUP  = 0x50000000L;
const PSSHOWFLAG    PS_SF_SCROLLDOWN= 0x60000000L;

const PSSHOWFLAG    PS_SF_NMF_MASK  = 0xF0000000L;

                                             /* DPS-specific Flags */
const PSSHOWFLAG    PS_SF_ALLPOPUP  = 0x00000001L;

const int      PS_CURRENTSELECTION = -1;
const int      PS_ALLENTRIES       = -1;
const int      PS_LASTENTRY        = -2;

const PSELEMENTTYPE  PS_ETYPE_NONE     = 0x00000000L;
const PSELEMENTTYPE  PS_ETYPE_ALL      = 0xFFFFFFFFL;
const PSELEMENTTYPE  PS_ETYPE_TOUCH    = 0x00000001L;
const PSELEMENTTYPE  PS_ETYPE_KEYBOARD = 0x00000002L;
const PSELEMENTTYPE  PS_ETYPE_VIDEO    = 0x00000004L;

/*
** SCOTPS APIs
*/
                        /* PSAPI Management Services */
APIExpose   PSRC        PSInit(HINSTANCE hAppInst, HWND hAppWnd, LPCTSTR szAppRegRoot);
APIExpose   PSRC        PSUninit(void);
APIExpose   HDEVCONTEXT PSCreateDC(LPCTSTR szDevClass, DEVID DevID, FPEVENTHANDLER fpEventHandler);
APIExpose   PSRC        PSDeleteDC(HDEVCONTEXT hDC);

                        /* PSAPI Special DPS Services */
APIExpose   HWND        PSGetWindowHandle(HDEVCONTEXT hDC, LPCTSTR szWindowName);
APIExpose   int         PSGetElementID(HDEVCONTEXT hDC, HWND hWindow, LPCTSTR szElementName);
APIExpose   PSRC        PSShowWindow(HDEVCONTEXT hDC, HWND hWindow, PSSHOWFLAG fShow);
APIExpose   PSRC        PSShowWindowEx(HDEVCONTEXT hDC, LPCTSTR szWindowName, PSSHOWFLAG fShow);

                        /* PSAPI Presentation Services */
APIExpose   PSRC        PSGetElement(HDEVCONTEXT hDC, HWND hWindow, INT nElementID, PSELEMENT psElement);
APIExpose   PSRC        PSShowElement(HDEVCONTEXT hDC, HWND hWindow, INT nElementID, PSELEMENT psElement);
APIExpose   PSRC        PSShowPresentation(HDEVCONTEXT hDC,LPCTSTR szPSName, PSSHOWFLAG fShow);
APIExpose   PSRC        PSFlush(HDEVCONTEXT hDC);
APIExpose   PSRC        PSEnableNotification(HDEVCONTEXT hDC, BOOL fEnable, PSELEMENTTYPE lElementTypes);

                        /* PSAPI Resource Management */
APIExpose   USHORT      PSGetPresentationFile(HDEVCONTEXT hDC, LPTSTR pBuffer, USHORT BufferLength);
APIExpose   PSRC        PSSetPresentationFile(HDEVCONTEXT hDC, LPCTSTR szFilename);

								/* PSAPI Remote Connection Management */
APIExpose	PSRC			PSInitializeRemote(HDEVCONTEXT hDC, PSRemoteParms psRemoteParms, BOOL bServer);
APIExpose	PSRC			PSConnectToRemote(HDEVCONTEXT hDC, PSRemoteParms psRemoteParms);
APIExpose	PSRC			PSDisconnectFromRemote(HDEVCONTEXT hDC, BOOL bSendDisconnectMsg);
APIExpose   PSRC        PSUninitializeRemote(HDEVCONTEXT hDC);
APIExpose   int         PSGetCurrentDisplay(HDEVCONTEXT hDC);

/* The followings are function derivatives */
                        /* Inline Functions */
inline PSRC PSScrollUp(HDEVCONTEXT hDC, HWND hWindow, int nElementID)
   {
   SELEMENT sElement;
   sElement.fShow = PS_SF_SCROLLUP;
   sElement.szContent = NULL;
   return (PSShowElement(hDC, hWindow, nElementID, &sElement));
   }
inline PSRC PSScrollDown(HDEVCONTEXT hDC, HWND hWindow, int nElementID)
   {
   SELEMENT sElement;
   sElement.fShow = PS_SF_SCROLLDOWN;
   sElement.szContent = NULL;
   return (PSShowElement(hDC, hWindow, nElementID, &sElement));
   }
inline PSRC PSPageUp(HDEVCONTEXT hDC, HWND hWindow, int nElementID)
   {
   SELEMENT sElement;
   sElement.fShow = PS_SF_PAGEUP;
   sElement.szContent = NULL;
   return (PSShowElement(hDC, hWindow, nElementID, &sElement));
   }
inline PSRC PSPageDown(HDEVCONTEXT hDC, HWND hWindow, int nElementID)
   {
   SELEMENT sElement;
   sElement.fShow = PS_SF_PAGEDOWN;
   sElement.szContent = NULL;
   return (PSShowElement(hDC, hWindow, nElementID, &sElement));
   }
inline PSRC PSSelectEntry(HDEVCONTEXT hDC, HWND hWindow, int nElementID, UINT nIndex)
   {
   SELEMENT sElement;
   sElement.fShow  = PS_SF_SELECT;
   sElement.nIndex = nIndex;
   sElement.szContent = NULL;
   return (PSShowElement(hDC, hWindow, nElementID, &sElement));
   }
inline PSRC PSUnselectEntry(HDEVCONTEXT hDC, HWND hWindow, int nElementID)
   {
   SELEMENT sElement;
   sElement.fShow  = PS_SF_UNSELECT;
   sElement.szContent = NULL;
   return (PSShowElement(hDC, hWindow, nElementID, &sElement));
   }
inline PSRC PSAddEntry(HDEVCONTEXT hDC, HWND hWindow, int nElementID, LPCTSTR szContent)
   {
   SELEMENT sElement;
   sElement.fShow     = PS_SF_ADD;
   sElement.szContent = szContent;
   return (PSShowElement(hDC, hWindow, nElementID, &sElement));
   }

#ifdef   __cplusplus
}        /* End of extern "C" { */
#endif	/* __cplusplus */


#endif   /* _SCOTPSApp_H */
