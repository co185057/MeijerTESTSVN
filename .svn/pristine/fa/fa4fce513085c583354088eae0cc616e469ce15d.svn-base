'******************************************************************************* 
'Purpose: Script to setup the UPCON.dat file on the UpCon Server.
'Author:  Dennis Shovelin
'Created: 8/2005
'*******************************************************************************



Set oShell = Wscript.CreateObject("Wscript.Shell")

Do 
GoodInput= "Y"
STORENO = InputBox("Enter Store Number: ", "Registry Setup - Store Number", "")
TESTLEN = len(STORENO)
	
	if TESTLEN = 0 then 
	   	wscript.quit
               
	end if

	if STORENO = "" then
		Error1=MsgBox("Store Number CANNOT be blank", 48, "Error!")
		GoodInput = "N"

	end if
	if TESTLEN < "4" then
		Error1=MsgBox("Store Number must be 4 characters leading 0's accepted", 48, "Error!")
		GoodInput = "N"

	end if
	if TESTLEN > "4" then
		Error1=MsgBox("Store Number must be 4 characters leading 0's accepted", 48, "Error!")
		GoodInput = "N"

	end if

Loop Until GoodInput = "Y"

sCCS = "HKLM\SYSTEM\CurrentControlSet\"
sTcpipParamsRegPath = sCCS & "Services\Tcpip\Parameters\"
sCompNameRegPath = sCCS & "Control\ComputerName\"

name = oshell.Regread (sCompNameRegPath & "ComputerName\ComputerName") 

Const ForReading = 1, ForWriting = 2, ForAppending = 8
  Dim fso, f, Msg
  Set fso = wscript.CreateObject("Scripting.FileSystemObject")   
  
  
  Set f = fso.OpenTextFile("c:\SCOT\UPCON\emptyupcon.dat", ForAppending, True)
  f.Writeline "STORE" & STORENO & "POS121//Lane" 
  f.writeline "STORE" & STORENO & "POS122//Lane"
  f.writeline "STORE" & STORENO & "POS123//Lane"
  f.writeline "STORE" & STORENO & "POS124//Lane"
  f.writeline "STORE" & STORENO & "POS125//Lane"
  f.writeline "STORE" & STORENO & "POS126//Lane"
  f.writeline "STORE" & STORENO & "RAP1//RAP"
  f.writeline "STORE" & STORENO & "RAP1//Security"
  f.writeline "STORE" & STORENO & "RAP1//Report"
  f.writeline "STORE" & STORENO & "RAP1//SSDB"
  f.writeline "/"

'touch = msgbox(readtextfiletest,65,"MSG")

	Wscript.quit
	
 

'End sc_setup.vbs