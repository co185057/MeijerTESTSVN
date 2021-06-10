Const ForReading = 1    
Const ForWriting = 2

Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objFile = objFSO.OpenTextFile("c:\scot\config\filetransportservice.xml", ForReading)
strText = objFile.ReadAll
objFile.Close

strNewText = Replace(strText, "administrator", "acs")
Set objFile = objFSO.OpenTextFile("c:\scot\config\filetransportservice.xml", ForWriting)
objFile.Write strNewText  'WriteLine adds extra CR/LF

strNewText = Replace(strText, "Unity1", "unity1")
Set objFile = objFSO.OpenTextFile("c:\scot\config\filetransportservice.xml", ForWriting)
objFile.Write strNewText  'WriteLine adds extra CR/LF

objFile.Close