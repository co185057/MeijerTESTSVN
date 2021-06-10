var logFile = "C:\\SCOT\\LOGS\\SSCO_OPTIONS.LOG";
var destDir = "C:\\scot\\config";
var sourceDir = "C:\\ACS\\Server\\Data\\SSCO";

var commonSource = sourceDir + "\\Common";
var r4Source = sourceDir + "\\R4";
var r5Source = sourceDir + "\\R5";
var r6Source = sourceDir + "\\R6";
var SS90Source = sourceDir + "\\SS90";
var pcSource = sourceDir + "\\PC";
var termSource = sourceDir + "\\Terminal";

var manifestFile = "manifest.dat";
var manifestSource = sourceDir + "\\" + manifestFile;
var manifestDest = destDir + "\\" + manifestFile;

var splash = null;
var empty = "";
var ForReading = 1, ForWriting = 2, ForAppending = 8;

var ldr = new ActiveXObject("LoadCtrl.Loader");

if (ldr)
{
    splash = ldr.GetSplash();
    
    if (splash)
    {
        splash.Title = "SSCO Options Loader";
        splash.BackVisible = false;
        splash.LogVisible = false;
        splash.StatusVisible = false;
        splash.SetLogFile(logFile, 0);
        
        ldr.Application = "acsapldr";
        ldr.Port = 889;
    
        var now = new Date();
        splash.AddLogMessage(now.toLocaleString());
        splash.AddLogMessage("Searching for server...");
        
	// Register the ACS Active Server
        var server = ldr.RegisterID(empty, empty, false);        		
	
        if (server)
        {
	    if( server.ActiveServer )
	    {
                var status = server.LastStatus;
    	        splash.AddLogMessage("server is active...with status=" + status);
	    }		
	    else
	    {
                var status = server.LastStatus;
	        splash.AddLogMessage("server is not active...with status=" + status);
	    }

            var serverName = server.Name;
            var serverSite = server.Site;
            var serverPort = server.port;
            splash.AddLogMessage("Connecting to server " + serverName + "... Site " + serverSite + " Port " + serverPort);

	if (server.Server)
	{
	  splash.AddLogMessage("Server application is in a primary or secondary role");
	}
	else
	{
	  splash.AddLogMessage("Server application is not acting in a primary or secondary role");
	}

	    // Connect to the active ACS Server
            var connection = server.Connect(empty, empty);

            if (connection)
            {
                splash.AddLogMessage("Connected.");
                splash.AddLogMessage("Getting client info...");
                
                connection.FileEvents = true;
                WScript.ConnectObject(connection, "connection_");
                
                var client = server.GetClientInfo();
                
                if (client)
                {
                    splash.AddLogMessage("Got client info.");
                    splash.AddLogMessage("Downloading common SSCO files from " + commonSource + " to " + destDir);
                    
                    //var errorCode = connection.DownloadDirectory(commonSource, destDir, false);
		    var errorCode = 0
                    
                    if (errorCode == 0)
                    {
			// Get the HW type, R4, R5 or PC (Dev Env)
                        var shell = new ActiveXObject("WScript.Shell");
                        var hwType = shell.RegRead("HKLM\\SOFTWARE\\NCR\\SCOT - Platform\\ObservedOptions\\HWType");
                        var hwSourceDir = null;
                        
                        if (hwType == "SCOT4")
                        {
                            hwSourceDir = r4Source;
                        }
                        else if (hwType == "SCOT5")
                        {
                            hwSourceDir = r5Source;
                        }
			else if (hwType == "SCOT6")
                        {
                            hwSourceDir = r6Source;
                        }
			else if (hwType == "SS90")
                        {
                            hwSourceDir = SS90Source;
                        }
                        else
                        {
                            hwType = "PC"
                            hwSourceDir = pcSource;
                        }
                        
                        splash.AddLogMessage("Hardware type is " + hwType);
                        splash.AddLogMessage("Downloading hardware-specific files from " + hwSourceDir + " to " + destDir);
                        // errorCode = connection.DownloadDirectory(hwSourceDir, destDir, false);
                        
                        if (errorCode == 0)
                        {
                            splash.AddLogMessage("Downloading manifest from " + manifestSource + " to " + manifestDest);
                            errorCode = connection.DownloadFile(manifestSource, manifestDest);
                            
                            if (errorCode == 0)
                            {
                                splash.AddLogMessage("Downloading terminal-specific files from " + termSource + " to " + destDir);
                                
                                var pad = "000";
                                var node = client.ID;
                                node = node.toString();
                                var termID = pad.substr(0, 3 - node.length);
                                var ext = "." + termID + node;

                                splash.AddLogMessage("Extension is " + ext);

                                var fso = new ActiveXObject("Scripting.FileSystemObject");
                                var f = fso.OpenTextFile(manifestDest, ForReading);

                                // Read from the file and display the results.
                                while (!f.AtEndOfStream)
                                {
                                    var r = f.ReadLine();
                                   
                                    /* Null strings, or strings that begin with #, carriage return, line feed, or space, will be ignored. */
                                    if (r && r.charAt(0) != '#' && r.charAt(0) != ' ' && r.charAt(0) != '\r' && r.charAt(0) != '\n' && r.charAt(0) != '\t')
                                    {
					// get the ssco config files with .DAT extension from COMMON folder
					var fileName = r + ".dat";
					var commonFileSource = commonSource + "\\" + fileName;
                                        var fileDest = destDir + "\\" + fileName;
                                        
					errorCode = connection.DownloadFile(commonFileSource, fileDest);
                                        
                                        if (errorCode != 0 && errorCode != 2)
                                        {
                                            splash.AddLogMessage("Failed to get terminal file " + commonFileSource + " with error " + errorCode);
                                        }
                                        else if (errorCode != 2)
                                        {
                                            splash.AddLogMessage("Updated " + fileDest + " from " + commonFileSource);
                                        }
					else
					{
					    splash.AddLogMessage("Terminal file " + commonFileSource + " does not exist with error " + errorCode);
					}

					
					// get the ssco config files with .DAT extension from HW folder
                                        var hwFileSource = hwSourceDir + "\\" + fileName;

					errorCode = connection.DownloadFile(hwFileSource, fileDest);
                                        
                                        if (errorCode != 0 && errorCode != 2)
                                        {
                                            splash.AddLogMessage("Failed to get terminal file " + hwFileSource + " with error " + errorCode);
                                        }
                                        else if (errorCode != 2)
                                        {
                                            splash.AddLogMessage("Updated " + fileDest + " from " + commonFileSource);
                                        }
					else
					{
					    splash.AddLogMessage("Terminal file " + hwFileSource + " does not exist with error " + errorCode);
					}

					// get the ssco config files with .000 extension from COMMON folder
                                        var fileName = r + ".000";
					var commonFileSource = commonSource + "\\" + fileName;
                                        var fileDest = destDir + "\\" + fileName;
                                                                                
					errorCode = connection.DownloadFile(commonFileSource, fileDest);
                                        
                                        if (errorCode != 0 && errorCode != 2)
                                        {
                                            splash.AddLogMessage("Failed to get terminal file " + commonFileSource + " with error " + errorCode);
                                        }
                                        else if (errorCode != 2)
                                        {
                                            splash.AddLogMessage("Updated " + fileDest + " from " + commonFileSource);
                                        }
					else
					{
					    splash.AddLogMessage("Terminal file " + commonFileSource + " does not exist with error " + errorCode);
					}

					// get the ssco config files with .000 extension from HW folder					
                                        var hwFileSource = hwSourceDir + "\\" + fileName;

					errorCode = connection.DownloadFile(hwFileSource, fileDest);
                                        
                                        if (errorCode != 0 && errorCode != 2)
                                        {
                                            splash.AddLogMessage("Failed to get terminal file " + hwFileSource + " with error " + errorCode);
                                        }
                                        else if (errorCode != 2)
                                        {
                                            splash.AddLogMessage("Updated " + fileDest + " from " + commonFileSource);
                                        }
					else
					{
					    splash.AddLogMessage("Terminal file " + hwFileSource + " does not exist with error " + errorCode);
					}


					// get the ssco config files with .XXX extension from TERMINAL folder
                                        fileName = r + ext;
                                        var fileSource = termSource + "\\" + fileName;
                                        fileDest = destDir + "\\" + fileName;

                                        errorCode = connection.DownloadFile(fileSource, fileDest);
                                        
                                        if (errorCode != 0 && errorCode != 2)
                                        {
                                            splash.AddLogMessage("Failed to get terminal file " + fileName + " with error " + errorCode);
                                        }
                                        else if (errorCode != 2)
                                        {
                                            splash.AddLogMessage("Updated " + fileDest + " from " + commonFileSource);
                                        }
					else
					{
					    splash.AddLogMessage("Terminal file " + fileSource + " does not exist with error " + errorCode );
					}
					splash.AddLogMessage("...");
                                    }
                                }
                                
                                f.Close();
                                
                                splash.AddLogMessage("Completed successfully");
                                //splash.MessageBox("Completed successfully", "Complete", 0, 2);
                                
                                splash = null;
                            }
                            else
                            {
                                var msg = "Failed to get manifest file with error " + errorCode;
                                ReportError(msg);
                            }
                        }
                        else
                        {
                            var msg = "Failed to get hardware-specific files with error " + errorCode;
                            ReportError(msg);
                        }
                    }
                    else
                    {
                        var msg = "Failed to get common files with error " + errorCode;
                        ReportError(msg);
                    }

                    client = null;
                }
                else
                {
                    var msg = "Failed to get client information";
                    ReportError(msg);
                }

                connection.Disconnect(0);
                connection = null;
            }
            else
            {
                var msg = "Connection failed";
                ReportError(msg);
            }
            
            server = null;
        }
        else
        {
            var msg = "Registration failed";
            ReportError(msg);
        }
    }
}

splash = null;

function connection_CopyProgress(cur, tot)
{
    if (!splash)
    {
        return;
    }
    
    if (cur <= tot)
    {
        if (cur == 0)
        {
            if (tot == 0)
            {
                splash.ClearProgress();
            }
            else
            {
                splash.SetProgress(0, tot);
            }
        }
        else
        {
            splash.UpdateProgress(cur);
        }
    }
    else
    {
        splash.ClearProgress();
    }
}

function connection_FileProgress(Name)
{
    if (!splash)
    {
        return;
    }
    
    if (Name && (Name != ""))
    {
        var msg = "Updated " + Name;
        splash.AddLogMessage(msg);
        splash.Status = msg;
    }
  
    return 0;
}

function ReportError(msg)
{
    if (splash)
    {
        splash.AddLogMessage("Error: " + msg);
        splash.MessageBox(msg, "Error", 0, 5);
    }
}
