@echo off
set HHRAPDIR=%1\HHRAP\build
set buildDIR=%1
set HHInstDIR=%1\HHRAP\Installation

if exist "c:\temp\meijer" rmdir /s /q c:\temp\meijer

md c:\temp\meijer\hhrap

md c:\temp\meijer\hhrap\source

copy /Y %HHRAPDIR% c:\temp\meijer\hhrap
copy /Y %HHRAPDIR%\source c:\temp\meijer\hhrap\source

echo ---------------------------------------------------------------
echo =
echo = Building Version String file for HandHeld RAP
echo =
echo ---------------------------------------------------------------

call %buildDIR%\build\CreateVersionInfoResourceFile.bat %buildDIR%

echo ---------------------------------------------------------------
echo =
echo = Building CAB file for HandHeld RAP
echo =
echo ---------------------------------------------------------------


pushd %CD%

cd c:\temp\meijer\hhrap
call c:\temp\meijer\hhrap\buildcab.bat

popd

copy c:\temp\meijer\hhrap\"NCR Meijer HandHeld Rap.ARM.CAB" %HHInstDIR%

