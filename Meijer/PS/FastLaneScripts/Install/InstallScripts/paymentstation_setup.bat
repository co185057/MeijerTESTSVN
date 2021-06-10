rem ---------------------------------------------------------------------------------
rem paymentstation_setup.bat
rem
rem Purpose: Convert FastLane from a Scan&Bag type lane to a Mobile Payment Station
rem          type lane.
rem
rem History:
rem POS77412  Work Request:58657  Name:Matt Condra         Date: September 24, 2013
rem
rem ---------------------------------------------------------------------------------
regsvr32 /s c:\scot\controls\NCRNullDevice.dll
regedit /s c:\scot\reg\NoScale.reg
