'******************************************************************************* 
'Customer: Savemart
'Script: restart.vbs
'Purpose: Script to reboot Fastlane.
'Author:  Dennis Shovelin
'Created: 1/2006
'*******************************************************************************

Set oShell = CreateObject ("Wscript.shell")

ianswer = MsgBox("Restart Windows?", vbQuestion + vbOKCancel, "Restart?")

If ianswer = vbOK Then 'If OK, reboot



   oShell.run "c:\scot\bin\reboot.exe"
   
Else                   ' user selected cancel

   WScript.quit
   
End If

'End restart.vbs