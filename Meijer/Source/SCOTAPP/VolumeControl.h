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
	SETVOLUME,
	SETVOLUMELEVEL  // SR815.5 Personalized Volume
	
}VOLUMEFUNCTIONS;

class CVolumeControl 
{
public:
	CVolumeControl(VOLUMEFUNCTIONS volumefunctions = NOFUNCTION, 
                   HANDLE hSignal=0,
				   int nNewLevel=0); // SR815.5 Personalized Volume
	virtual ~CVolumeControl();
#ifndef _CPPUNIT
	void SetDefaultVolume(void);
	void SetVolume(void);
	void ShowVolumeLogo();
	//+SR815.5 Personalized Volume
    int GetDefaultVolume(void); // Retrieve the default volume specified in the registry.
    int GetVolumeLevel(void); // Retrive the volume object on where the volume ended up after the user was finished cycling through the levels.
    void SetVolumeLevel(void); // Set the new volume level.
    int GetMaxVolumeLevel(void); // Get the maximum volume level.
    //-SR815.5 Personalized Volume
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
	
private:
    int m_nVolumeLevel; // SR815.5 Personalized Volume    
#endif //_CPPUNIT	
};

#endif // !defined(AFX_VOLUMECONTROL_H__A7D7C06F_E6EE_4CA2_973F_B80112CAA74E__INCLUDED_)
