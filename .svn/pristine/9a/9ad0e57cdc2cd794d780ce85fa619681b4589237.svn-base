using System;
using System.IO;
using System.Text;
using System.Net;
using System.Diagnostics;
using System.Threading;
using System.Net.NetworkInformation;
using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;
using Newtonsoft.Json.Converters;
using RPSWNET;

namespace SSCOGoalClientNet
{
    public class WalmartSSCOGoals
    {
        private const int nEventID = 9005; //SSCOGoal Event log id
        public static bool pingFTP = false;

        /* This is the public interface */
        public static SSCOGoal GetWalmartSSCOGoals(string sURL, string sStoreNumber, string sCountryCode, string dtRetrievalDate)
        {
            return ReadSCOGoals(sURL, sStoreNumber, sCountryCode, dtRetrievalDate);
        }

        public static SSCOGoal GetWalmartSSCOGoalsFTP(string sStoreNumber, string sCountryCode, string sISPFTPServerIP)
        {
            // expected ftp server filename: isp.s05501.us.wal-mart.com
            string ftpServer = "isp.s" + sStoreNumber + "." + sCountryCode + ".wal-mart.com";

            // expected filename formet: POS-SCO-GOALS.05501.US
            string ftpFilename = "POS-SCO-GOALS." + sStoreNumber + "." + sCountryCode;

            CmDataCapture.Capture(CmDataCapture.MaskInfo, "SSCOGoal: FTPServer " + ftpServer + " Filename: " + ftpFilename);
            return ExecuteCommand(ftpServer, ftpFilename, sISPFTPServerIP); //get file from the ftpserver
        }

        public static bool PingHost(string sServerName)
        {

            Ping checkPing = new Ping();

            try
            {
                PingReply reply = checkPing.Send(sServerName, 500);
                pingFTP = reply.Status == IPStatus.Success;
            }
            catch (PingException)
            {
                pingFTP = false;
                CmDataCapture.Capture(CmDataCapture.MaskInfo, "Unable to ping using " + sServerName);
            }

            return pingFTP;
        }

        /*This is the private method that will communicate with the webservice */
        //read ssco goal from web service
        private static SSCOGoal ReadSCOGoals(string sURL, string sStoreNumber, string sCountryCode, string dtRetrievalDate)
        {
            SSCOGoal goals = null;
            string responseCodeLog = string.Empty;

            try
            {
                //prepare url with query parameters
                string sResource = "/custexp/api/scoData/v1/getSCOData/";
                string sQuery = string.Format("{0}{1}{2}/{3}?reportDate={4}", sURL, sResource, sCountryCode, sStoreNumber, dtRetrievalDate);

                CmDataCapture.Capture(CmDataCapture.MaskInfo, "SSCOGoal: Checking web service: " + sQuery);

                CmDataCapture.Capture(CmDataCapture.MaskInfo, "SSCOGoal: Before creating the httpwebrequest");
                HttpWebRequest myRequest = (HttpWebRequest)WebRequest.Create(sQuery);
                CmDataCapture.Capture(CmDataCapture.MaskInfo, "SSCOGoal: After creating the httpwebrequest");
                myRequest.Timeout = 10000;

                using (HttpWebResponse myResponse = (HttpWebResponse)myRequest.GetResponse())
                {
                    if (myResponse != null)
                    {
                        try
                        {
                            //Check if url is available and running
                            if (myResponse.StatusCode == HttpStatusCode.OK)
                            {
                                CmDataCapture.Capture(CmDataCapture.MaskInfo, "SSCOGoal: content length is [" + myResponse.ContentLength + "] and content type is [" + myResponse.ContentType + "]");
                                CmDataCapture.Capture(CmDataCapture.MaskInfo, "SSCOGoal: Available: " + (int)myResponse.StatusCode + " Status Description " + myResponse.StatusDescription);

                                // Get the stream associated with the response.
                                using (Stream receiveStream = myResponse.GetResponseStream())
                                {
                                    if (receiveStream != null)
                                    {
                                        try
                                        {
                                            string jsonContentData = string.Empty;

                                            // Pipes the stream to a higher level stream reader with the required encoding format. 
                                            using (StreamReader readStream = new StreamReader(receiveStream, Encoding.UTF8))
                                            {
                                                if (readStream != null)
                                                {
                                                    try
                                                    {
                                                        jsonContentData = readStream.ReadToEnd();
                                                    }
                                                    finally
                                                    {
                                                        readStream.Close();
                                                    }
                                                }
                                            }

                                            CmDataCapture.Capture(CmDataCapture.MaskInfo, "SSCOGoal: Data received " + jsonContentData);

                                            //deserialize the data from the web service
                                            SSCODataResponse sscoGoalDataResponse = JsonConvert.DeserializeObject<SSCODataResponse>(jsonContentData);

                                            if (sscoGoalDataResponse != null)
                                            {
                                                responseCodeLog = "SSCOGoal: StoreGoal: " + sscoGoalDataResponse.Body.DataResponseSCOGoal + " StoreActual: " + sscoGoalDataResponse.Body.DataResponseSCOActual;

                                                goals = new SSCOGoal();

                                                if (sscoGoalDataResponse.Body.DataResponseSCOGoal != null)
                                                {
                                                    goals.Goal = sscoGoalDataResponse.Body.DataResponseSCOGoal;
                                                }
                                                else
                                                {
                                                    goals.Goal = null;
                                                }

                                                if (sscoGoalDataResponse.Body.DataResponseSCOActual != null)
                                                {
                                                    goals.Actual = sscoGoalDataResponse.Body.DataResponseSCOActual;
                                                }
                                                else
                                                {
                                                    goals.Actual = null;
                                                }

                                                responseCodeLog = " SSCOGoal: StoreGoal: " + goals.Goal + " StoreActual: " + goals.Actual;
                                                CmDataCapture.Capture(CmDataCapture.MaskInfo, responseCodeLog);
                                                LogApplicationEvent("SSCOGoalClientNet", "Application", responseCodeLog, EventLogEntryType.Information, nEventID);
                                            }
                                            else
                                            {
                                                responseCodeLog = "SSCOGoal: ReadSCOGoal Webservice did not respond expected datatype for StoreGoal and StoreActual.";
                                                CmDataCapture.Capture(CmDataCapture.MaskInfo, responseCodeLog);
                                            }
                                        }
                                        finally
                                        {
                                            receiveStream.Close();
                                        }
                                    }
                                    else
                                    {
                                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "SSCOGoal: null stream retrieved from response object");
                                    }
                                } // using receiveStream
                            }
                            else
                            {
                                CmDataCapture.Capture(CmDataCapture.MaskInfo, string.Format("SSCOGoal: Unexpected HTTP status of {0} {1}", (int)myResponse.StatusCode, myResponse.StatusDescription));
                            }
                        }
                        finally
                        {
                            myResponse.Close();
                        }
                    } // if myResponse != null
                    else
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "SSCOGoal: response object is null");
                    }
                } // using myResponse
            }
            catch (WebException ex)
            {
                // all exceptions will be logged in RAPNet.log and Application.evt with EVENT ID 9005
                HttpWebResponse resp = ex.Response as HttpWebResponse;

                if (resp != null)
                {
                    if (ex.Status == WebExceptionStatus.ProtocolError && ex.Response != null)
                    {
                        //explicitly idenfifying the http error response for now in case we need to do something
                        if (resp.StatusCode == HttpStatusCode.NotFound) // HTTP 404
                        {
                            //figure out what to do next as soon as the service is running
                        }

                        if (resp.StatusCode == HttpStatusCode.InternalServerError) //HTTP 500
                        {
                            //figure out what to do next as soon as the service is running
                        }
                        if (resp.StatusCode == HttpStatusCode.BadRequest) //HTTP 400
                        {
                            //figure out what to do next as soon as the service is running
                        }

                        CmDataCapture.Capture(CmDataCapture.MaskInfo, responseCodeLog);
                        // -SSCOIA-16809
                    }
                }
                else
                {
                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "SSCOGoal: Null response object");
                }

                if (ex.Status == WebExceptionStatus.NameResolutionFailure)
                {
                    //figure out what to do next as soon as the service is running
                }

                responseCodeLog = "SSCOGoal: Service is not found. Received status: " + ex.Status;
                CmDataCapture.Capture(CmDataCapture.MaskInfo, responseCodeLog);
                LogApplicationEvent("SSCOGoalClientNet", "Application", responseCodeLog, EventLogEntryType.Warning, nEventID);
            }
            catch (Exception ex)
            {
                //  not available at all, for some reason
                responseCodeLog = string.Format("SSCOGoal EXCEPTION: {0}", ex.Message);
                CmDataCapture.Capture(CmDataCapture.MaskInfo, responseCodeLog);
                LogApplicationEvent("SSCOGoalClientNet", "Application", responseCodeLog, EventLogEntryType.Error, nEventID);
            }

            return goals;
        }

        private static SSCOGoal ExecuteCommand(string ftpServer, string ftpFilename, string sISPFTPServerIP)
        {
            SSCOGoal goals = new SSCOGoal();
            if (!PingHost(ftpServer))
            {
                // +SSCOIA-16387
                if (sISPFTPServerIP != String.Empty)
                {
                    ftpServer = sISPFTPServerIP;
                }
                else
                {
                    ftpServer = String.Empty;
                }
                // -SSCOIA-16387

                CmDataCapture.Capture(CmDataCapture.MaskInfo, "Use ip address of the isp server: " + ftpServer);
                LogApplicationEvent("SSCOGoalClientNet FTP Module", "Application", "Use ip address of the isp server: " + ftpServer, EventLogEntryType.Error, nEventID);
            }

            string filePath = Environment.ExpandEnvironmentVariables(@"%APP_DRIVE%\scot\bin\");

            //Write the expected parameters for the ftp command
            String ftpCmnds = "open " + ftpServer + " \r\nuser smart smart \r\nbin \r\ncd /u/spool/01 \r\nlcd " + filePath + "\r\nget " + ftpFilename + "\r\nquit";

            //storage for the ftp commands to be used
            string ftpFile = filePath + "sscogoalcommands.ftp";

            if (File.Exists(ftpFile))
                File.Delete(ftpFile);

            //if (File.Exists(ftpFilename))
            //    File.Delete(ftpFilename);

            //Create the actual file
            using (StreamWriter sw = File.CreateText(ftpFile))
            {
                sw.WriteLine(ftpCmnds);
                sw.Close();
            }

            string filePathCommand = filePath + "SSCOGoalFTP.bat";

            if (File.Exists(filePathCommand))
            {
                Process p = new Process();
                p.StartInfo.FileName = "SSCOGoalFTP.bat";
                p.StartInfo.UseShellExecute = false;
                p.StartInfo.CreateNoWindow = true;
                p.StartInfo.WorkingDirectory = filePath;
                p.Start();
                p.WaitForExit();

                while (!p.HasExited)
                {
                    // Discard cached information about the process
                    p.Refresh();
                    // Print working set to console.
                    // Wait 2 seconds.
                    Thread.Sleep(2000);
                }
                p.Close();
            }
            else
            {
                CmDataCapture.Capture(CmDataCapture.MaskInfo, "FTP Module error: File not found: " + filePathCommand);
                LogApplicationEvent("SSCOGoalClientNet FTP Module", "Application", "FTP Module error: File not found: " + filePathCommand, EventLogEntryType.Error, nEventID);
            }

            return ReadSCOGoalsFTPFile(ftpFilename);

        }

        private static SSCOGoal ReadSCOGoalsFTPFile(string ftpFilename)
        {
            string scoGoalFileLocation = @"c:\scot\bin\"; //SSCOGoal ftp file location
            string jsonPath = string.Format(scoGoalFileLocation + ftpFilename);
            CmDataCapture.Capture(CmDataCapture.MaskInfo, jsonPath);


            SSCOGoal goals = new SSCOGoal();
            try
            {

                if (File.Exists(jsonPath))
                {

                    string jsonData = File.ReadAllText(jsonPath);
                    SSCODataResponse response = JsonConvert.DeserializeObject<SSCODataResponse>(jsonData);

                    if (response != null)
                    {
                        goals.Goal = response.Body.DataResponseSCOGoal;
                        goals.Actual = response.Body.DataResponseSCOActual;
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "FTP Module using " + ftpFilename + ": Goal is " + goals.Goal + ". Actual is " + goals.Actual + ".");
                        LogApplicationEvent("SSCOGoalClientNet FTP Module", "Application", "FTP Module using " + ftpFilename + ": Goal is " + goals.Goal + ". Actual is " + goals.Actual + ".", EventLogEntryType.Information, nEventID);
                    }
                    else
                    {
                        goals.Goal = string.Empty;
                        goals.Actual = string.Empty;
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "FTP Module error: Goal is empty. Actual is empty");
                        LogApplicationEvent("SSCOGoalClientNet FTP Module", "Application", "FTP Module error: Goal is empty. Actual is empty", EventLogEntryType.Information, nEventID);
                    }
                }
                else
                {
                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "FTP Module error: File not found: " + ftpFilename);
                    LogApplicationEvent("SSCOGoalClientNet FTP Module", "Application", "FTP Module error: File not found: " + ftpFilename, EventLogEntryType.Error, nEventID);
                }
            }
            catch (Exception ex)
            {
                CmDataCapture.Capture(CmDataCapture.MaskInfo, "Goal data could not be retrieved using " + ftpFilename + " " + ex.Message);
                LogApplicationEvent("SSCOGoalClientNet FTP Module", "Application", "Goal data could not be retrieved using " + ftpFilename + " " + ex.Message, EventLogEntryType.Error, nEventID);
            }
            return goals;

        }

        private static void LogApplicationEvent(string sSource, string sLog, string sEvent, EventLogEntryType logType, int nEventID)
        {
            try
            {
                if (!EventLog.SourceExists(sSource))
                {
                    EventLog.CreateEventSource(sSource, sLog);
                }

                EventLog.WriteEntry(sSource, sEvent, logType, nEventID);
            }
            catch (Exception ex)
            {
                CmDataCapture.Capture(CmDataCapture.MaskInfo, "Unable to write to event log. " + ex.Message);
            }
        }
    }
}
