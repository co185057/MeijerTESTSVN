Call SendMail(Wscript.Arguments(0), Wscript.Arguments(1), Wscript.Arguments(2), Wscript.Arguments(3), Wscript.Arguments(4), Wscript.Arguments(5), Wscript.Arguments(6))



Sub SendMail(strFrom, strTo, strCC, strBCC, strSubject, strBody, strFiles)

	Dim iMsg, iConf, aFile, varFiles, Flds

	


	set imsg = createobject("cdo.message")

	set iconf = createobject("cdo.configuration")


	Set Flds = iConf.Fields

	With Flds

	    .Item("http://schemas.microsoft.com/cdo/configuration/sendusing") = 2

	    .Item("http://schemas.microsoft.com/cdo/configuration/smtpserver") = "ngdomain.blg982.meijer.com"

	    .Update

	End With



	With iMsg

	    Set .Configuration = iConf


	    .To = Trim(strTo)

	    .Cc = Trim(strCc)

	    .Bcc = Trim(strBcc)

	    .From = Trim(strFrom)

	    .Subject = Trim(strSubject)

	    .TextBody = Trim(strBody)



	     varFiles = Split ( strFiles, ";" )

	     For Each aFile in varFiles

                If ( Trim(aFile) <> "" ) Then

		  .AddAttachment Trim(aFile)

		End If

	     Next


	    .fields.update

	    .Send

	End With



	set imsg = nothing

	set iconf = nothing


End Sub

