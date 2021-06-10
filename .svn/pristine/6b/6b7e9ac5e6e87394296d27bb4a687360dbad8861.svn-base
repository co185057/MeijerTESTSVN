// VolumeControl.h: interface for the CVolumeControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VOLUMECONTROL_H__A7D7C06F_E6EE_4CA2_973F_B80112CAA74E__INCLUDED_)
#define AFX_VOLUMECONTROL_H__A7D7C06F_E6EE_4CA2_973F_B80112CAA74E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mmsystem.h"

typedef enum 
{
	NOFUNCTION,
    SHOWLOGO,
	SETDEFAULT,
	SETVOLUME
}VOLUMEFUNCTIONS;

class CVolumeControl 
{
public:
	CVolumeControl(VOLUMEFUNCTIONS volumefunctions = NOFUNCTION, 
                   HANDLE hSignal=0);
	virtual ~CVolumeControl();
	void SetDefaultVolume(void);
	void SetVolume(void);
	void ShowVolumeLogo();
protected:
	void playSound(void);
	void showMuteLogo(void);

    // +TAR 351654 - Since pointer variables have been added to this class,
    //               add protected copy ctor and assignment operator to 
    //               prevent this object from being copied.
    CVolumeControl(const CVolumeControl &rhs);
    void operator=(const CVolumeControl &rhs);
    // -TAR 351654

    void HandleFunction(void);              // TAR 351654

    static UINT SigHandler(LPVOID parm);    // TAR 351654
    VOLUMEFUNCTIONS m_func;                 // TAR 351654
    HANDLE m_hSignal;                       // TAR 351654
    HANDLE m_hTerminate;                    // TAR 351654
    CWinThread *m_pThread;                  // TAR 351654
};

#endif // !defined(AFX_VOLUMECONTROL_H__A7D7C06F_E6EE_4CA2_973F_B80112CAA74E__INCLUDED_)
