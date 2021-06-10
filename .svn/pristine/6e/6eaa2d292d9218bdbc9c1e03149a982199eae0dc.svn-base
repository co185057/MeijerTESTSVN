md c:\scot\bin
md c:\scot\dll
md c:\scot\config
xcopy ..\Data\PLufile.txt     c:\scot\config 	/R
xcopy ..\Data\DisplayText.txt c:\scot\config 	/R
xcopy ..\Bin\acscom32.dll     c:\scot\bin	/R
xcopy ..\Bin\acsosext.dll     c:\scot\bin	/R
xcopy ..\config\*.* c:\scot\config /R

set path=%PATH%;c:\scot\bin;c:\scot\dll
regedit /s .\TBRegSettings.reg .\CoreApp.reg .\SCOTTB.reg .\SCOTAPPEventLog.reg
