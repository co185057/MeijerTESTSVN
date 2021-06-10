#if !defined(AFX_REMOTEOBJECTDEFINES_H__B192C94A_298D_4C22_A7A5_C287AB7C5CD0__INCLUDED_)
#define AFX_REMOTEOBJECTDEFINES_H__B192C94A_298D_4C22_A7A5_C287AB7C5CD0__INCLUDED_

// Values for m_Focus
const int NCRRC_LOCALFOCUS  = 0;     //Focus is local
const int NCRRC_REMOTEFOCUS = 1;     //Focus is remote
// Values for m_Display
const int NCRRC_LOCALDISPLAY = 0;    //Display on local screen only
const int NCRRC_REMOTEDISPLAY = 1;   //Display on remote screen only
const int NCRRC_BOTHDISPLAY = 2;     //Display on both the local and remote screens

// The following fit into the table in SCOTPSDev.h.  They
// are included here so the object can remain independent of ps
const    WORD	PSC_DISCONNECT_MSG            = 150;
const    WORD  PSC_CONNECT_MSG               = 151;
const    WORD  PSC_EVENT_MSG                 = 152;
const    WORD  PSC_INITIALIZEREMOTE          = 153;
const    WORD  PSC_CONNECTTOREMOTE           = 154;
const    WORD  PSC_DISCONNECTFROMREMOTE      = 155;
const    WORD  PSC_KEEP_ALIVE                = 156;
const    WORD  PSC_UNINITIALIZEREMOTE        = 157;
const		WORD  PSC_GETCURRENTDISPLAY         = 158;

typedef long   HDEVCONTEXT;      /* Device context, no bit reduction! */

typedef struct {
	LPCTSTR sName;
	LPCTSTR sAssistModeTemplate;
	HDEVCONTEXT hDC;
	int     iDisplay;
	int     iFocus;
	bool    bDualConnection;
} SRemoteParms, *PSRemoteParms;


#endif