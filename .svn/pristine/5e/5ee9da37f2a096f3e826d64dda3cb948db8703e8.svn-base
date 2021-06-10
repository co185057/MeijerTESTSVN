'******************************************************************************* 
'Customer: Meijer
'Script: UpCON_setup.vbs
'Purpose: Automates the tailoring of UpCon to the specific store environment
'Author:  Dennis Shovelin
'Created: 3/2006
'*******************************************************************************

Set oShell = CreateObject ("Wscript.shell")

'*******************************************************************************
'Setup Variables
'*******************************************************************************
Dim CI, s1, s2, sPath, A1, i, r, A2
Dim upcondat
Dim sampleline
Dim ObjFSO

DIM savedcomputername(10)
dim number_of_PODS

dim savedRAPname1
dim savedRAPname2
dim numberoflanes_POD1
dim numberoflanes_POD2
Dim servermsg
Dim title 
dim style 
dim response 
Dim POS_NO
Dim STORE_NUM
DIM Starting_lane_no_POD1
dim starting_lane_no_POD2
DIM computed_lane_no_POD2
STORE_NUM = ""

'*******************************************************************************
'   Prompt for store number Subroutine
'*******************************************************************************
sub get_store()
Do
GoodInput = "Y"
STORE_NUM = InputBox("Enter Store Number: Must be 4 digits Leading Zeroes", "Store Number", "1")
TESTLEN = len(STORE_NUM)
	
	if TESTLEN = 0 then
		Error1=MsgBox("STORE Number must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	if STORE_NUM = "" then
		 Error1=MsgBox("STORE Number must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	if TESTLEN < 4 then
		Error1=MsgBox("STORE Number must be 4 digits", 48, "Error!")
		GoodInput = "N"
	end if
	if TESTLEN > 4 then
		Error1=MsgBox("Terminal Number must be 2 digits", 48, "Error!")
		GoodInput = "N"
	end if
Loop Until GoodInput = "Y"

end sub

'**************************************************************************
'   GET RAP Information
'**************************************************************************
sub get_Config_info
Do
GoodInput = "Y"
Number_of_PODS = InputBox("How many PODS are in this store", "Number of PODS", "1")
TESTLEN = len(Number_of_PODS)
	
	if TESTLEN = 0 then
		Error1=MsgBox("Number of PODS must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	if Number_of_PODS = "" then
		 Error1=MsgBox("Number of PODS must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	if TESTLEN < 1 then
		Error1=MsgBox("Number of PODS must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	if TESTLEN > 1 then
		Error1=MsgBox("Number of PODS must be 1 digits", 48, "Error!")
		GoodInput = "N"
	end if
Loop Until GoodInput = "Y"

'**************************************************************************
'   GET INFORMATION FOR POD 1
'**************************************************************************


do
GoodInput = "Y"
numberoflanes_POD1 = InputBox("Enter the Number of Lanes in POD 1", "Number of Lanes POD 1", "0")
TESTLEN = len(numberoflanes_POD1)

	if TESTLEN = 0 then
		Error1=MsgBox("Number of Lanes must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	if numberoflanes_POD1 = "" then
		 Error1=MsgBox("Number of Lanes must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	if TESTLEN > 1 then
		Error1=MsgBox("Number of Lanes must be 1 digit", 48, "Error!")
		GoodInput = "N"
	end if
Loop Until GoodInput = "Y"

do
GoodInput = "Y"
Starting_lane_no_POD1 = InputBox("Enter the Starting Lane Number for POD 1: 3 digits", "Starting Lane Number POD 1", "000")
TESTLEN = len(Starting_lane_no_POD1)

	if TESTLEN = 0 then
		Error1=MsgBox("Starting Lane Number must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	if Starting_lane_no_POD1 = "" then
		 Error1=MsgBox("Starting Lane Number must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	
	if TESTLEN < 3 then
		Error1=MsgBox("Starting Number must be 3 digits", 48, "Error!")
		GoodInput = "N"
	end if

	if TESTLEN > 3 then
		Error1=MsgBox("Starting Number must be 3 digits", 48, "Error!")
		GoodInput = "N"
	end if
Loop Until GoodInput = "Y"

'******************************END GET INFORMATION POD 1**************************

'**************************************************************************
'   GET INFORMATION FOR POD 2
'**************************************************************************
if number_of_PODS = "2" then

do
GoodInput = "Y"
numberoflanes_POD2 = InputBox("Enter the Number of Lanes in POD 2", "Number of Lanes", "0")
TESTLEN = len(numberoflanes_POD2)

	if TESTLEN = 0 then
		Error1=MsgBox("Number of Lanes must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	if numberoflanes_POD2 = "" then
		 Error1=MsgBox("Number of Lanes must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	
	

	if TESTLEN > 1 then
		Error1=MsgBox("Number of Lanes must be 1 digit", 48, "Error!")
		GoodInput = "N"
	end if
Loop Until GoodInput = "Y"


do
GoodInput = "Y"
Starting_lane_no_POD2 = InputBox("Enter the Starting Lane Number for POD 2: 3 digits", "Starting Lane Number", "000")
TESTLEN = len(Starting_lane_no_POD2)

	if TESTLEN = 0 then
		Error1=MsgBox("Starting Lane Number must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	if Starting_lane_no_POD2 = "" then
		 Error1=MsgBox("Starting Lane Number must be entered", 48, "Error!")
		GoodInput = "N"
	end if
	
	if TESTLEN < 3 then
		Error1=MsgBox("Starting Number must be 3 digits", 48, "Error!")
		GoodInput = "N"
	end if

	if TESTLEN > 3 then
		Error1=MsgBox("Starting Number must be 3 digits", 48, "Error!")
		GoodInput = "N"
	end if
Loop Until GoodInput = "Y"
end if

end sub

'**********************************************************************
'  UPCON.dat  file write
'**********************************************************************
sub upcondatupdate()
dim i 
dim done
mynum_POD1 = int(numberoflanes_POD1 - 1)
mynum_POD2 = int(numberoflanes_POD2 - 1)
Set objFSO = WScript.CreateObject("Scripting.FileSystemObject")

If objFSO.FileExists("c:\scot\upcon\upcon.dat") Then objFSO.DeleteFile("c:\scot\upcon\upcon.dat")
Set upcondat = objFSO.CreateTextFile("c:\scot\upcon\upcon.dat")
sampleline = "; File: Upcon.dat"
upcondat.writeline(sampleline)
sampleline = "; This file specifies to UpCon the network configuration of a store."
upcondat.writeline(sampleline)
sampleline = "; One or more Sets of Lanes, optional Security Server and optional"
upcondat.writeline(sampleline)
sampleline = "; Rap stand and Report server may be specified.  There may be only one security server"
upcondat.writeline(sampleline)
sampleline = "; defined in each set. There may be only one RAP defined in each set. "
upcondat.writeline(sampleline)
sampleline = "; There may be only one Report server defined in each set."
upcondat.writeline(sampleline)
sampleline = "; Each set description ends with /."
upcondat.writeline(sampleline)
sampleline = "; Lanes, Security servers or Rap stands are specified with the syntax:"
upcondat.writeline(sampleline)
sampleline = ";"
upcondat.writeline(sampleline)
sampleline = ";	Node_Id / [ALIAS] / Rap|Lane|Security|SSDB | Report"
upcondat.writeline(sampleline)
sampleline = ";"
upcondat.writeline(sampleline)
sampleline = "; Node_Id may be either a network name or IP address."
upcondat.writeline(sampleline)
sampleline = "; Alias specifies display string representing the node, or blank."
upcondat.writeline(sampleline)
sampleline = "; Rap | Lane | Security |SSDB| Report must be specified to identify the node type."
upcondat.writeline(sampleline)
sampleline = "; The field separator /is required.  "
upcondat.writeline(sampleline)
sampleline = "; The End of Set identifier / is required."
upcondat.writeline(sampleline)
sampleline = ";"
upcondat.writeline(sampleline)
sampleline = "; The next set might be specified as..."
upcondat.writeline(sampleline)
sampleline = ";153.60.29.240//Security"
upcondat.writeline(sampleline)
sampleline = ";153.60.29.248/rap6/Rap"
upcondat.writeline(sampleline)
sampleline = ";153.60.29.245/testteam9/Lane"
upcondat.writeline(sampleline)
sampleline = ";/"
upcondat.writeline(sampleline)
sampleline = ";"
upcondat.writeline(sampleline)
sampleline = ";"
upcondat.writeline(sampleline)
sampleline = "/"
upcondat.writeline(sampleline)
'**************DO LOOP TO FILL IN


'*****Check if POD 1

i = 0
done = "N"

computed_lane_no = starting_lane_no_POD1

do 
savedcomputername(i) = "Store" & STORE_NUM & "POS" & computed_lane_no

computed_lane_no = computed_lane_no + 1
i = i + 1
if i > mynum_POD1 then
  done = "Y"
end if
loop until done = "Y"
'*********END DO LOPP FILL IN

sampleline = "; POD 1"
upcondat.writeline(sampleline)
'**************DO LOOP FOR WRITING
i = 0
done = "N"
do 
sampleline = savedcomputername(i) & "//LANE"
upcondat.writeline(sampleline)
i = i + 1
if i > mynum_POD1 then
  done = "Y"
end if
loop until done = "Y"
'***************END DO LOOP FOR WRITING
savedRAPname1 = "STORE" & STORE_NUM & "RAP1"
sampleline = savedRAPname1 & "//RAP"
upcondat.writeline(sampleline)
sampleline = savedRAPname1 & "//Security"
upcondat.writeline(sampleline)
sampleline = savedRAPname1 & "//Report"
upcondat.writeline(sampleline)
sampleline = savedRAPname1 & "//SSDB"
upcondat.writeline(sampleline)
sampleline = "/"
upcondat.writeline(sampleline)
sampleline = " "
'******************* END POD 1

'******************  POD 2

if number_of_PODS = "2" then
sampleline = "; POD 2"
upcondat.writeline(sampleline)

i = 0
done = "N"
computed_lane_no = starting_lane_no_POD2
do 
savedcomputername(i) = "Store" & STORE_NUM & "POS" & computed_lane_no
computed_lane_no = computed_lane_no + 1
i = i + 1
if i > mynum_POD2 then
  done = "Y"
end if
loop until done = "Y"
'*********END DO LOPP FILL IN
i = 0
done = "N"
do 
sampleline = savedcomputername(i) & "//LANE"
upcondat.writeline(sampleline)
i = i + 1
if i > mynum_POD2 then
  done = "Y"
end if
loop until done = "Y"
'***************END DO LOOP FOR WRITING
savedRAPname2 = "STORE" & STORE_NUM & "RAP2"
sampleline = savedRAPname2 & "//RAP"
upcondat.writeline(sampleline)
sampleline = savedRAPname2 & "//Security"
upcondat.writeline(sampleline)
sampleline = savedRAPname2 & "//Report"
upcondat.writeline(sampleline)
sampleline = savedRAPname2 & "//SSDB"
upcondat.writeline(sampleline)
sampleline = "/"
upcondat.writeline(sampleline)
sampleline = " "
'******************* END POD 2

end if

end sub
'***************************end Launchpad function******************************



'*************************************************************************
'"*************************************************************************
'  MAIN PROCESSING SECTION
'*************************************************************************
'*************************************************************************
get_store
get_config_Info

'*****************Create upcon.dat************************************

upcondatupdate

 
wscript.quit