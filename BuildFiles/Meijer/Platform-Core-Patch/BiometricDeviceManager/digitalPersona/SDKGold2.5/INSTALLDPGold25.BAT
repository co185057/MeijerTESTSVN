@ECHO OFF
REM STEP 1 - Open the C Drive
C:

REM STEP 2 - Install Digital Persona Gold 2.5
ECHO Installing Digital Persona Gold 2.5...
msiexec /i C:\Windows\system32\SetupDPGold25.msi  /qn FT_1_KEY=54df8c46a66ba14fdd8f0152cce9e65f  FT_2_KEY=d6c49aefd2ff0c3e54bf942a588faa7b

