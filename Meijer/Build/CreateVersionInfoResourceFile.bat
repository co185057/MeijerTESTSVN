::@echo off
set buildDIR=%1


	
for /F "tokens=1,2 delims=#=" %%A in (%buildDIR%\build\lanesetup.ini) do if %%A.==CUSTOMERVER. echo %%B> lanesetup.txt

copy lanesetup.txt c:\temp\meijer\HHRAP


