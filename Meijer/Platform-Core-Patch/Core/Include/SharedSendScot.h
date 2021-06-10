
#ifndef __SHAREDSENDSCOT_H__ 
#define __SHAREDSENDSCOT_H__

#define SECTION1 _T("Messages")
#define SECTION2 _T("WindowName")
#define SECTION3 _T("Responses") //dp185016 tar 287778

#define KEY1 _T("SCOTShutdown")
#define KEY2 _T("NoRespond")
#define KEY3 _T("NotRunning")
#define KEY4 _T("ForceStop")
#define KEY5 _T("WaitForSwitchToDie")
#define KEY6 _T("PerformOrderlyShutdown")
#define KEY7 _T("SCOTAlreadyUp")
#define KEY8 _T("FailRestartSCOT")
#define KEY9 _T("FailRestartSwitch")
#define KEY10 _T("FailReboot")
#define KEY11 _T("SendLaneClose")
#define KEY12 _T("QueryState")
#define KEY13 _T("StopSwitchApp")
#define KEY14 _T("FailStartBatch")
#define KEY15 _T("NoBatch")
#define KEY16 _T("KillPopup")
#define KEY17 _T("RebootMsg")
#define KEY18 _T("DisplayPopup")
#define KEY19 _T("LoadController")
#define KEY20 _T("Switch")
#define KEY21 _T("Timeout")
#define KEY22 _T("PowerDown") //dp185016 rfc 265463

#define KEY25 _T("ForceStopRAP")
#define KEY30 _T("RAPNotRunning")
#define KEY35 _T("RAPShutdown")
#define KEY40 _T("RAPNoRespond")
#define KEY45 _T("FailRestartRAP")
#define KEY50 _T("RAPAlreadyUp")
#define KEY55 _T("RAPRebootMsg")
#define KEY60 _T("FailRebootRAP")
#define KEY65 _T("RAPKillPopup")
#define KEY66 _T("PowerStopRAP") //+dp185016 rfc 265463
#define KEY67 _T("PowerDownRAP")
#define KEY68 _T("RAPRunning")   //-dp185016

#define KEY70 _T("StopSecurity")
#define KEY75 _T("StartSecurity")

//+dp185016 tar 287778
//Fastlane responses
#define KEY80 _T("LaneClosed")
#define KEY81 _T("NotInTransaction")
#define KEY82 _T("InTransaction")
#define KEY83 _T("CurrentState")
#define KEY84 _T("GetTextOutput")
//-dp185016

#define szIniFilePath _T("c:\\scot\\config\\SENDScot.ini")

#endif