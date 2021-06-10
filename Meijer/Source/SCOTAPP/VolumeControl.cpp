// VolumeControl.cpp: implementation of the CVolumeControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#ifndef _CPPUNIT
#include "Common.h"
#endif //_CPPUNIT
#include "VolumeControl.h"

#include <assert.h>

//redefine.
//in mmsystem.h, there is a definition
//#ifdef UNICODE
//#define PlaySound  PlaySoundW
//#else
//#define PlaySound  PlaySoundA
//#endif // !UNICODE

#ifdef PlaySound
#undef PlaySound
#endif
#define PlaySound PlaySound

#define COMP_ID   ID_SM
#define T_ID      _T("VolumeControl")

const UINT MUTE			= 0;		//mute
const UINT MAX_VOLUME	= 100;		//full volume

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CVolumeControl::CVolumeControl(VOLUMEFUNCTIONS volumefunctions, HANDLE hSignal, int nNewLevel)
#ifndef _CPPUNIT
    : m_func(volumefunctions), m_hSignal(hSignal), m_hTerminate(NULL), 
      m_pThread(NULL),
	  m_nVolumeLevel(nNewLevel) // SR815.5 Personalized Volume
#endif
{
#ifndef _CPPUNIT
    // +TAR 351654:  
    if(m_hSignal)
    {
        // If the client wishes to delay function processing,
        // then process the call in a thread that will wait until the event is 
        // signalled.

        m_hTerminate = CreateEvent(NULL, TRUE, FALSE, NULL);
        m_pThread = AfxBeginThread(SigHandler, (LPVOID)this,  
                                   THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);

        assert(m_pThread != NULL);
        if(! m_pThread)
        {
            trace(L2, _T("ERROR: Failed to create delayed processing thread!"));
            return;
        }

        // We want to be able to control when the thread is destroyed in case
        // this object is deleted before the thread completes its task.
        m_pThread->m_bAutoDelete = FALSE;
        (void)m_pThread->ResumeThread();
    }
    else
    {
        // Immediately handle the request the normal way.
        HandleFunction();
    }
    // -TAR 351654:  
#endif //_CPPUNIT
}

/**
 * Execute specified VOLUMEFUNCTION
 * Moved from constructor for TAR 351654 
 */
#ifndef _CPPUNIT
void CVolumeControl::HandleFunction(void)
{
    switch(m_func)
    {
    case NOFUNCTION:
    case SHOWLOGO:
        ShowVolumeLogo();
        break;
    case SETDEFAULT:
        SetDefaultVolume();
        break;
    case SETVOLUME:
        SetVolume();
        break;
	//+SR815.5 Personalized Volume
    case SETVOLUMELEVEL:
        SetVolumeLevel();
        break;
    //-SR815.5 Personalized Volume
    default:
        assert(false);
    }
}

/**
 * Thread to handle delayed function processing.
 * This thread sleeps until the m_hSignal event is set.
 * This thread exits when m_hTerminate is set.
 *
 * \param[in] param - Should be an instance of the CVolumeControl class.
 * \return 0 on success, non-zero on failure.
 */
UINT CVolumeControl::SigHandler(LPVOID param)
{
    assert(param != NULL);

    CVolumeControl *vc = static_cast<CVolumeControl *>(param);
    UINT rc = 0;
    HANDLE aEvents[2];
    DWORD evrc;

    aEvents[0] = vc->m_hTerminate;
    aEvents[1] = vc->m_hSignal;

    trace(L6, _T("Delayed volume control thread started"));
    do
    {
        evrc = WaitForMultipleObjects(2, aEvents, FALSE, INFINITE);
        switch(evrc)
        {
        case WAIT_FAILED:
            assert(false);
            rc = GetLastError();
            trace(L2, _T("ERROR: WaitForMultipleObj returned error [%d]"), rc);
            break;

        case (WAIT_OBJECT_0 + 1):         
            trace(L6, _T("Event signaled. Calling volume control function.")); 
            vc->HandleFunction();   // Handle request.
            break;

        case WAIT_OBJECT_0:         // Thread exiting.      
            trace(L6, _T("Delayed volume control thread exiting ..."));
            break;

        case WAIT_TIMEOUT:          // Timed out. Shouldn't be possible here. 
        default:                    
            assert(false);          
            break;
        }
    }
    while(evrc != WAIT_OBJECT_0);

    return rc;
}
#endif //_CPPUNIT

CVolumeControl::~CVolumeControl()
{
#ifndef _CPPUNIT
    // +TAR 351654:  Terminate delayed-processing thread if necessary.
    if(m_pThread != NULL)
    {
        assert(m_hSignal != NULL && m_hTerminate != NULL);

        DWORD rc;

        try
        {
            (void)SetEvent(m_hTerminate);

            // TAR 393382 - Change wait from infinite to 3 seconds and 
            //              terminate the thread if it does not exit..
            rc = WaitForSingleObject(m_pThread->m_hThread, 3000);

            assert(rc == WAIT_OBJECT_0);
            if(rc == WAIT_TIMEOUT)
            {
                trace(L5, _T("Timed-out at volume control thread exit."));
                TerminateThread(m_pThread->m_hThread, 1); 
            }

            delete m_pThread;
            (void)CloseHandle(m_hTerminate);
        }
        catch(...)
        {
            // Destructor must not throw an exception.
            assert(false);
        }
    }
    // -TAR 351654
#endif //_CPPUNIT
}

#ifndef _CPPUNIT
//******************************************************
//Method : SetDefaultVolume
//Purpose: Sets volume to the default volume.
//		   The default volume is set through the volume control of launchpad
//Parameters:
//Return Values:
//******************************************************
void CVolumeControl::SetDefaultVolume()
{
		//read the default volume from the registry
		int nDefaultVolume = 0;
		HKEY hKey;
		unsigned long lLength   = sizeof(nDefaultVolume);
		if ( ERROR_SUCCESS == RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\LaunchPadNet\\"), \
				0, KEY_QUERY_VALUE, &hKey ) )
		{
			if ( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("VolumeLevel"), NULL, NULL, \
					(LPBYTE)&nDefaultVolume, &lLength  ) )
			{
				if (( co.fOperationsVolumeControlMuteBypass ) && (nDefaultVolume == 0))
				{
					trace(L6, _T("CVolumeControl::SetDefaultVolume - CPSX::Volume set to minimum level = %d. VolumeControlMuteBypass is set to Y and Launchpad volume is 0"), co.GetVolumeControlIncrementLevel());
					nDefaultVolume = co.GetVolumeControlIncrementLevel();
				}
				if 	( !IsRCError( CPSX::SetVolume(nDefaultVolume, nDefaultVolume) ) )
				{
					trace(L6, _T("CVolumeControl::SetDefaultVolume - CPSX::SetVolume level = %d"), nDefaultVolume); // SR815.5 Personalized Volume
					ShowVolumeLogo();
				}else
				{
					trace(L6, _T("CVolumeControl::SetDefaultVolume - CPSX::SetVolume returns false. Display Mute") );
					showMuteLogo();
				}

			}
		}
		 RegCloseKey( hKey );
}
//******************************************************
//Method : SetVolume
//Purpose: Increments the current volume
//         Once the current volume reaches maximum volume, cycles back to lowest value.
//Parameters:
//Return Values:
//******************************************************
void CVolumeControl::SetVolume()
{
		int nLeftVolume = MUTE;
		int nRightVolume = MUTE;
		if ( !IsRCError(CPSX::GetVolume( nLeftVolume, nRightVolume ) ))
		{
			trace(L6, _T("CVolumeControl::SetVolume - CPSX::GetVolume level = %d"), nLeftVolume);

			// change to MUTE if current volume is MAX_VOLUME
			if ( MAX_VOLUME == nLeftVolume )
			{
				nLeftVolume  = MUTE;
				
				//  +SR719
				//  Bypass the "MUTE" state if VolumeCotrolMuteBypass in ScotOpts is set to Y
				if ( co.fOperationsVolumeControlMuteBypass )
				{
					nLeftVolume  += co.GetVolumeControlIncrementLevel();
				}
				//  -SR719
			}else
			{
				nLeftVolume  += co.GetVolumeControlIncrementLevel();
				if ( MAX_VOLUME < nLeftVolume )
				{
					nLeftVolume = MAX_VOLUME;
				}
			}
			nRightVolume = nLeftVolume;
			if 	( !IsRCError(CPSX::SetVolume(nLeftVolume, nRightVolume ) ))
			{
				trace(L6, _T("CVolumeControl::SetVolume - CPSX::SetVolume level = %d"), nLeftVolume);
				playSound();
				ShowVolumeLogo();
			}else
			{
				trace(L6, _T("CVolumeControl::SetVolume - CPSX::SetVolume returns false. Display Mute") );
				showMuteLogo();
			}
		}else
		{
			trace(L6, _T("CVolumeControl::GetVolume - CPSX::GetVolume returns false. Display Mute") );
			showMuteLogo();
		}
}

//******************************************************
//Method : ShowVolumeLogo
//Purpose: Displays the volume level or mute logo.
//Parameters:
//Return Values:
//******************************************************
void CVolumeControl::ShowVolumeLogo( )
{
	int nLeftVolume = MUTE;
	int nRightVolume = MUTE;
	if ( !IsRCError(CPSX::GetVolume( nLeftVolume, nRightVolume ) ))
	{
		if (MUTE == nLeftVolume)
		{
			showMuteLogo();
		}else
		{
			CString csPosition, csImageFileName;
			int nIndex = ((double)nLeftVolume / MAX_VOLUME) * 5;
			if ( nIndex < 1 )
			{
				nIndex = 1;
			}
			ps.GetPSXObject()->GetCustomDataVar(_T("LogoPosition-VOLUME"),csPosition, \
				_T("ButtonVolumeControl"));
			CString csValue;
			csValue.Format( _T("VolumeLogo%d"), nIndex );
			ps.GetPSXObject()->GetCustomDataVar(csValue, csImageFileName, \
				_T("ButtonVolumeControl"));
			trace(L6, _T("CVolumeControl::ShowVolumeLogo Position = %s, ImageFileName = %s"), csPosition, csImageFileName);
			ps.GetPSXObject()->SetConfigProperty(_T("ButtonVolumeControl"), _T(""), UI::PROPERTYLOGO, csImageFileName );
			ps.GetPSXObject()->SetConfigProperty(_T("ButtonVolumeControl"), _T(""), UI::PROPERTYLOGOPOSITION, csPosition);
			
			// +RFQ 7477 & SSCOI-44973
			//SOTF 7745
            //On certain screens Meijer already uses the button 7 (volume control)
            //for something else.  A new control was added to replace volume control
            ps.GetPSXObject()->GetCustomDataVar(_T("LogoPosition-VOLUME"),csPosition, \
				_T("CmdBtn_ButtonVolumeControl"));
			csValue.Format( _T("VolumeLogo%d"), nIndex );
			ps.GetPSXObject()->GetCustomDataVar(csValue, csImageFileName, \
				_T("CmdBtn_ButtonVolumeControl"));
			trace(L6, _T("CVolumeControl::ShowVolumeLogo Position = %s, ImageFileName = %s"), csPosition, csImageFileName);
			ps.GetPSXObject()->SetConfigProperty(_T("CmdBtn_ButtonVolumeControl"), _T(""), UI::PROPERTYLOGO, csImageFileName );
			ps.GetPSXObject()->SetConfigProperty(_T("CmdBtn_ButtonVolumeControl"), _T(""), UI::PROPERTYLOGOPOSITION, csPosition);
            //sotf
			// -RFQ 7477 & SSCOI-44973

		}
	}else
	{
		trace(L6, _T("CVolumeControl::ShowVolumeLogo - CPSX::GetVolume returns false. Display Mute") );
		showMuteLogo();
	}

}



//******************************************************
//Method : playSound
//Purpose: Play sound
//Parameters:
//Return Values:
//******************************************************
void CVolumeControl::playSound()
{
	_TCHAR  szSoundPath[512];
	_TCHAR szPath[_MAX_PATH];
	if ( dm.SoundFindSoundFilePath(szSoundPath, sizeof(szSoundPath)) )
	{
		_tmakepath(szPath,NULL, szSoundPath, ps.GetSoundName(BTTNVALID), _T("WAV"));
		CString csSoundFile = szPath;
		GET_PARTITION_PATH(csSoundFile , csSoundFile.GetBuffer(_MAX_PATH));
		csSoundFile.ReleaseBuffer();
		ps.GetPSXObject()->PlaySound( csSoundFile );
	}

}

//******************************************************
//Method : showMuteLogo
//Purpose: Display mute button
//Parameters:
//Return Values:
//******************************************************
void CVolumeControl::showMuteLogo()
{
	CString csPosition, csImageFileName;
	ps.GetPSXObject()->GetCustomDataVar(_T("LogoPosition-MUTE"), csPosition, \
		_T("ButtonVolumeControl"));
	ps.GetPSXObject()->GetCustomDataVar(_T("MuteLogo"), csImageFileName, \
		_T("ButtonVolumeControl"));
	trace(L6, _T("CVolumeControl::showMute Position = %s, ImageFileName = %s"), csPosition, csImageFileName);
	ps.GetPSXObject()->SetConfigProperty(_T("ButtonVolumeControl"), _T(""), UI::PROPERTYLOGO, csImageFileName );
	ps.GetPSXObject()->SetConfigProperty(_T("ButtonVolumeControl"), _T(""), UI::PROPERTYLOGOPOSITION, csPosition);
	
	// +RFQ 7477 & SSCOI-44973
	//POS36524 - make sure the muted image is displayed also - rrs
    ps.GetPSXObject()->GetCustomDataVar(_T("LogoPosition-MUTE"), csPosition, \
		_T("CmdBtn_ButtonVolumeControl"));
	ps.GetPSXObject()->GetCustomDataVar(_T("MuteLogo"), csImageFileName, \
		_T("CmdBtn_ButtonVolumeControl"));
	trace(L6, _T("CVolumeControl::showMute Position = %s, ImageFileName = %s"), csPosition, csImageFileName);
	ps.GetPSXObject()->SetConfigProperty(_T("CmdBtn_ButtonVolumeControl"), _T(""), UI::PROPERTYLOGO, csImageFileName );
	ps.GetPSXObject()->SetConfigProperty(_T("CmdBtn_ButtonVolumeControl"), _T(""), UI::PROPERTYLOGOPOSITION, csPosition);   
    //e POS36524 - make sure the muted image is displayed also - rrs
	// -RFQ 7477 & SSCOI-44973

} 

//+SR815.5 Personalized Volume
int CVolumeControl::GetDefaultVolume() // Retrieve the default volume level specified in the registry.
{
	int retVal = -1; // error occured
    int nDefaultVolume = 0;
	HKEY hKey;
	unsigned long lLength   = sizeof(nDefaultVolume);
	if ( ERROR_SUCCESS == RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\LaunchPadNet\\"), \
			0, KEY_QUERY_VALUE, &hKey ) )
	{
		if ( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("VolumeLevel"), NULL, NULL, \
				(LPBYTE)&nDefaultVolume, &lLength  ) )
		{
			trace(L6, _T("CVolumeControl::GetDefaultVolume - default volume level = %d"), nDefaultVolume);
            retVal = nDefaultVolume;
		}
	}
	RegCloseKey( hKey );
    return retVal;
}

int CVolumeControl::GetVolumeLevel() // Get the current volume level.
{
    int nLeftVolume = MUTE;
	int nRightVolume = MUTE;

	if ( !IsRCError(CPSX::GetVolume( nLeftVolume, nRightVolume ) ))
	{
        trace(L6, _T("CVolumeControl::GetVolumeLevel - current volume level = %d"), nLeftVolume);
    }

    return nLeftVolume;
}

void CVolumeControl::SetVolumeLevel() // Set the volume level to the integer level specified.
{
    if ((co.fOperationsVolumeControlMuteBypass) && (m_nVolumeLevel == 0))
    {
        m_nVolumeLevel = co.GetVolumeControlIncrementLevel();
    }
	if 	( !IsRCError( CPSX::SetVolume(m_nVolumeLevel, m_nVolumeLevel) ) )
	{
        trace(L6, _T("CVolumeControl::SetVolumeLevel - new volume level = %d"), m_nVolumeLevel);
		ShowVolumeLogo();
	}else
	{
		trace(L6, _T("CVolumeControl::SetVolumeLevel - CPSX::SetVolume returns false. Display Mute") );
		showMuteLogo();
	}
}

int CVolumeControl::GetMaxVolumeLevel() // Get the maximum volume level
{
    return MAX_VOLUME;
}
//-SR815.5 Personalized Volume
#endif //_CPPUNIT