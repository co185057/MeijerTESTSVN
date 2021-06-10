//
// ChangePasswordState.cs
//
// Change history:
// POS151582 Work Request:66559 Name:Saiprasad Srihasam Date:December 26, 2014
// POS151582 Work Request:66559 Name:Saiprasad Srihasam Date:December 26, 2014
// POS105314 Work Request:61233 Name:Saiprasad Srihasam Date:August 07, 2014
// POS105314 Work Request:61233 Name:Saiprasad Srihasam Date:August 05, 2014
// POS107504 Work Request:61233  Name: Saiprasad Srihasam Date: July 25, 2014
// POS105305 Work Request:61233  Name: Saiprasad Srihasam Date: July 15, 2014
// POS101386 Work Request:61233  Name: Saiprasad Srihasam Date: June 26, 2014
// POS101386 Work Request:61233  Name: Saiprasad Srihasam Date: June 25, 2014
//////////////////////////////////////////////////////////////////////
using System;
using System.Threading;
using System.Runtime.InteropServices;  //SR93- Biometrics
using PsxNet;
using RPSWNET;
using System.Net;
using System.Linq;

#if( !WINCE )
using System.Windows.Forms;
using System.Net.Sockets;
#endif

namespace RapNet
{
    /// <summary>
    /// 
    /// </summary>
    internal class ChangePasswordState : RapNet.State
    {

        string m_OperId;
        static string m_prevOperId;
        string m_userPwd;
        string m_userNewPwd;
        string m_NewPwdCnfrm;
        int m_MaxtryPwd;
        int nCountInvalidPwd;

        /// <summary>
        /// The class constructor.  The CheckIn State is a class to handle the check in operation when attendant pressing the Check in button at the RAP station.
        /// It would send the "check-in" message to the SSCO through RCM and the SSCO would send back the response to this class again through RCM.
        /// The response from SSCO would be processed in the state_psxevent function.
        /// </summary>
        /// <param name="paramPsx">
        /// SharedPsx instance used to connect.
        /// </param>
        public ChangePasswordState(SharedPsx paramPsx, IPsx psx, bool bScanHndlr)
            : base(paramPsx, psx, bScanHndlr)
        {
            ConfigData configData = ConfigData.GetConfigData();
            //POS105314
            if (configData.AllowEmplIdSignOn)
            {
                contextEnterId = myPsx.GetExpectedEnterIDContext();
            }
            else
            {
                contextEnterId = "ChangePWD";
            }
            contextEnterPassword = "EnterOldPWD";
            //POS105314
            m_OperId = "";
            m_prevOperId = "";
            m_userPwd = "";
            m_userNewPwd = "";
            m_NewPwdCnfrm = "";
            m_MaxtryPwd = 0;
            nCountInvalidPwd = 0;

            GetMaxTryInvalidPassword();

            ResetCheckInContexts(false);

            myPsx.ShowLanes(false, 0);
        }

        /// <summary>
        /// Releases the resources used by the CheckIn instance.
        /// </summary>
        public override void Dispose()
        {

            if (bEmplPwdExpired)
            {
                bEmplPwdExpired = false;
            }
            ResetCheckInContexts(false);


            if (checkInTimer != null)
            {
                checkInTimer.Dispose();
                checkInTimer = null;
            }

            myPsx.ChangePwd = false;


            base.Dispose();
        }


        /// <summary>
        /// State Psx event handler.
        /// </summary>
        /// <param name="sender">
        /// The source of the event.
        /// </param>		
        /// <param name="e">
        /// A PsxEventArgs that contains the event data.
        /// </param>
        /// <returns>
        /// Return True if we handle the event; otherwise, it should return False.
        /// </returns>
        public override bool State_PsxEvent(object sender, PsxEventArgs e)
        {
            bool handled = false;
            //POS105314
            ConfigData configData = ConfigData.GetConfigData();
            string contextChangepwd;

            CmDataCapture.Capture(CmDataCapture.MaskError, "CheckInState::State_PsxEvent() - EventName =  " + e.EventName + ", EventContext = " + e.ContextName + "EventRAP = " + ConfigData.eventRap);

            if (configData.AllowEmplIdSignOn)
            {
                contextChangepwd = myPsx.GetExpectedEnterIDContext();
            }
            else
            {
                contextChangepwd = "ChangePWD";
            }

            m_OperId = (string)myPsx.psx.GetConfigProperty("Echo", contextChangepwd, Psx.PropertyText);
            //POS105314
            m_userPwd = (string)myPsx.psx.GetConfigProperty("Echo", "EnterOldPWD", Psx.PropertyText);
            m_userNewPwd = (string)myPsx.psx.GetConfigProperty("Echo", "EnterNewPWD", Psx.PropertyText);
            m_NewPwdCnfrm = (string)myPsx.psx.GetConfigProperty("Echo", "ConfirmNewPWD", Psx.PropertyText);
            CmDataCapture.Capture(CmDataCapture.MaskError, "CheckInState::State_PsxEvent() - ContextEnterID = " + contextEnterId + "User ID =  " + m_OperId);

            if (e.EventName.Equals(Psx.EventClick))
            {
                if (e.ContextName.Equals("EnterOldPWD") && (e.ControlName.Equals("KeyBoardP4") || e.ControlName.Equals("AlphaNumKey3")))
                {
                    CmDataCapture.Capture(CmDataCapture.MaskError, "CheckInState::State_PsxEvent() - ContextEnterID = " + contextEnterId + "User ID =  " + m_OperId);

                    string strURL = "";

                    string userPwd = (string)myPsx.psx.GetConfigProperty("Echo", contextEnterPassword, Psx.PropertyText);

                    try
                    {
                        strURL = GetWebServiceURL(201);
                        ValidateEmplPwd(strURL);

                        handled = true;
                    }
                    catch (Exception ex)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "POSFileAccessService Exception for " + strURL + " server: " + ex.Message); //POS101386
                        try
                        {
                            strURL = GetWebServiceURL(202);
                            ValidateEmplPwd(strURL);
                            handled = true;
                        }
                        catch (Exception ex1)
                        {
                            CmDataCapture.Capture(CmDataCapture.MaskError, "POSFileAccessService Exception for " + strURL + " server: " + ex1.Message); //POS101386
                            myPsx.psx.SetContext("UnableToConnectToServer", Psx.DisplayStandard, false);
                        }
                    }

                }
                //POS105314
                else if (e.ContextName.Equals("EnterOldPWD") && e.ControlName.Equals("ButtonCommand0"))
                {
                    ResetCheckInContexts(false);
                    myPsx.psx.SetContext(contextChangepwd, Psx.DisplayStandard, false);
                    handled = true;

                }
                else if (e.ContextName.Equals("PWDExpired") && e.ControlName.Equals("OKButton"))
                {
                    ResetCheckInContexts(false);
                    myPsx.psx.SetContext(contextChangepwd, Psx.DisplayStandard, false);
                    handled = true;
                }
                //POS105314
                else if (e.ContextName.Equals("ConfirmNewPWD"))
                {
                    string strURL = "";
                    if (m_userNewPwd == m_NewPwdCnfrm)
                    {

                        try
                        {
                            strURL = GetWebServiceURL(201);
                            ChangeEmployeePwd(strURL);
                        }
                        catch (Exception ex)
                        {
                            CmDataCapture.Capture(CmDataCapture.MaskError, "POSFileAccessService Exception for " + strURL + " server: " + ex.Message); //POS101386
                            try
                            {
                                strURL = GetWebServiceURL(202);
                                ChangeEmployeePwd(strURL);
                            }
                            catch (Exception ex1)
                            {
                                CmDataCapture.Capture(CmDataCapture.MaskError, "POSFileAccessService Exception for " + strURL + " server: " + ex1.Message); //POS101386
                                myPsx.psx.SetContext("UnableToConnectToServer", Psx.DisplayStandard, false);
                            }
                        }

                    }
                    else
                    {
                        myPsx.psx.SetContext("PWDDoNotMatch", Psx.DisplayStandard, false);
                    }

                    handled = true;
                }

            }
            else if (e.EventName.Equals(Psx.EventChangeContext))
            {
                if (e.ContextName.Equals("ChangePWD") ||
                    e.ContextName.Equals("EnterOldPWD") ||
                    e.ContextName.Equals("EnterNewPWD") ||
                    e.ContextName.Equals("ConfirmNewPWD"))
                {
                    myPsx.psx.SetConfigProperty("Echo", e.ContextName, Psx.PropertyTextFormat, String.Empty);

                    // (+) POS151582
                    if (e.ContextName.Equals("ConfirmNewPWD"))
                    {
                        string strTemp = m_userNewPwd;
                        strTemp = strTemp.Replace('0', ' ');
                        strTemp = strTemp.Trim();

                        if (strTemp == string.Empty)
                        {
                            myPsx.psx.SetContext("InvalidPassword", Psx.DisplayStandard, false);
                        }
                    }
                    // (-) POS151582
                    handled = true;
                }
                else if (e.ContextName.Equals("ScanIDOnlyChangePWD") ||
                        e.ContextName.Equals("ScanIDOrKeyChangePWD"))
                {
                    handled = true;
                }


            }
            //POS105314

#if( !WINCE )
            else if (e.EventName.Equals(Psx.EventKeyDown))
            {
                if (Keys.Enter == Psx.ObjectToKeys(e.Param))
                {
                    if (Psx.State.Normal == (Psx.State)Convert.ToInt32(myPsx.psx.GetControlConfigProperty(configData.AlphaNumericKeyboard ? "AlphaNumKey3" : "KeyBoardP4", Psx.PropertyState)))
                    {
                        if (e.ContextName.Equals(contextEnterId))
                        {
                            myPsx.psx.SetContext(contextEnterPassword, Psx.DisplayStandard, false);
                            handled = true;
                        }

                    }
                }
            }
#endif
            //POS105314

            return handled;
        }

        /// <summary>
        /// Occurs when Scanner fires an event.
        /// </summary>
        /// <param name="sender">
        /// The source of the event.
        /// </param>		
        /// <param name="e">
        /// A ScannerEventArgs that contains the event data.
        /// </param>
        public override void State_ScannerEvent(object sender, ScannerEventArgs e)
        {
            //POS105314
            ConfigData configData = ConfigData.GetConfigData();
            // tell launchpad to reset the auto sign off counter
            Remote.SendMessage(Remote.LaunchpadApplication, "operation=reset-autosignoff");


            if (configData.AllowEmplIdSignOn)
            {
                string barcode = e.ScanDataLabel;
                char[] ctrlChars = new char[] { '\n', '\r', ' ' };
                barcode = barcode.Trim(ctrlChars);
                configData.SavedScannerEventArgs = new ScannerEventArgs(e.ScanData, barcode, e.ScanType);
                myPsx.psx.SetConfigProperty("Echo", contextEnterId, Psx.PropertyTextFormat, barcode);
                myPsx.psx.SetContext("EnterOldPWD", Psx.DisplayStandard, false);
            }
            //POS105314			

        }

        /// <summary>
        /// Returns a string representation of the object.
        /// </summary>
        public override string ToString()
        {
            return "ChangePassword";
        }

        /// <summary>
        /// Resets Id and password contexts.
        /// </summary>
        /// <param name="retry">
        /// True if this is after a failed sign on.
        /// </param>				
        private void ResetCheckInContexts(bool retry)
        {
            try
            {
                string leadthruText = myPsx.GetExpectedEnterIDLeadthru();
                string leadthruPwdText = myPsx.GetExpectedPasswordLeadthru();
                if (retry)
                {
                    leadthruText = "Re" + leadthruText;
#if( HH_UI )
					leadthruText = "HH_" + leadThruText;
#endif
                }

                myPsx.psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, true);
                myPsx.psx.SetConfigProperty("Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.psx.GetString(leadthruText));
                myPsx.psx.SetConfigProperty("Leadthru", contextEnterPassword, Psx.PropertyTextFormat, myPsx.psx.GetString(leadthruPwdText));
                myPsx.psx.SetConfigProperty("Leadthru", "ChangePWD", Psx.PropertyTextFormat, myPsx.psx.GetString(leadthruText));
                myPsx.psx.SetConfigProperty("Echo", "ChangePWD", Psx.PropertyTextFormat, String.Empty);
                myPsx.psx.SetConfigProperty("Echo", contextEnterId, Psx.PropertyTextFormat, String.Empty);   //POS105314
                myPsx.psx.SetConfigProperty("Echo", "EnterOldPWD", Psx.PropertyTextFormat, String.Empty);
                myPsx.psx.SetConfigProperty("Echo", "EnterNewPWD", Psx.PropertyTextFormat, String.Empty);
                myPsx.psx.SetConfigProperty("Echo", "ConfirmNewPWD", Psx.PropertyTextFormat, String.Empty);

            }
            catch (PsxException caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "CheckInState::ResetSignOnContexts() - PsxException: " + caught.Message);
            }
            catch (Exception caught)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "CheckInState::State_PsxEvent() - Exception: " + caught.Message);
            }
        }

        /// <summary>
        /// Timeout callback for sign on.
        /// </summary>
        private void CheckInTimeout(object state)
        {
            if (checkInTimer != null)
            {
                myPsx.psx.SetContext("Startup", Psx.DisplayStandard, false);
                myPsx.psx.SetControlProperty(Psx.ControlDisplay, Psx.PropertyUserInput, true);
            }
        }


        public string GetWebServiceURL(int v_intServer)
        {
            // (+) POS101386 - Modified code to build URL using ip address rather than using the DNS.
            string strIPaddr = string.Empty;
            IPHostEntry ipEntry = Dns.GetHostEntry(string.Empty);
            IPAddress ipAddress = ipEntry.AddressList
                .FirstOrDefault(a => a.AddressFamily == AddressFamily.InterNetwork);
            if (ipAddress != null)
                strIPaddr = ipAddress.ToString();

            // (+) POS107504 - Modified logic to replace the 4th Octet with "201"/"202" to connect to the server.
            int nIndex = strIPaddr.LastIndexOf('.');
            strIPaddr = strIPaddr.Substring(0, nIndex + 1);
            strIPaddr += v_intServer.ToString();
            // (-) POS107504 - Modified logic to replace the 4th Octet with "201"/"202" to connect to the server.


            string strURL = cAppConfigObj.propWebServiceTemplate;
            strURL = strURL.Replace("STORE_SERVER_IP_ADDRESS", strIPaddr);
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "Server Address=" + strURL);
            // (-) POS101386 - Modified code to build URL using ip address rather than using the DNS.
            return strURL;
        }


        public void ValidateEmplPwd(string strURL)
        {
            int nRes =0;
            bool bRes;


            client.Url = strURL;
            client.ValidateEmplPwd_ws(m_OperId, m_userPwd, out nRes, out bRes);

            if (nRes == 0)
            {
                if (m_prevOperId == m_OperId)
                {
                    if (nCountInvalidPwd == m_MaxtryPwd)
                    {
                        myPsx.psx.SetContext("PWDLimitExceeded", Psx.DisplayStandard, false);
                        nCountInvalidPwd = 0;
                    }
                    else
                    {
                        //myPsx.SetContext( "ReEnterID", Psx.DisplayStandard, false );
                        //POS105314
                        //						myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.GetString( "ReEnterIDLeadthru" ) );
                        ResetCheckInContexts(true);
                        myPsx.psx.SetContext(contextEnterId, Psx.DisplayStandard, false);
                        //POS105314
                        m_prevOperId = m_OperId;
                        nCountInvalidPwd++;
                    }
                }
                else
                {
                    //myPsx.SetContext( "ReEnterID", Psx.DisplayStandard, false );
                    //POS105314
                    //					myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.GetString( "ReEnterIDLeadthru" ) );
                    ResetCheckInContexts(true);
                    myPsx.psx.SetContext(contextEnterId, Psx.DisplayStandard, false);
                    //POS105314
                    m_prevOperId = m_OperId;
                    nCountInvalidPwd = 1;
                }
            }
            else if (nRes == -1)
            {
                //myPsx.SetContext( "ReEnterID", Psx.DisplayStandard, false );
                //POS105314
                //				myPsx.SetConfigProperty( "Leadthru", contextEnterId, Psx.PropertyTextFormat, myPsx.GetString( "ReEnterIDLeadthru" ) );
                ResetCheckInContexts(true);
                myPsx.psx.SetContext(contextEnterId, Psx.DisplayStandard, false);
                //POS105314
            }
            else
            {
                myPsx.psx.SetContext("EnterNewPWD", Psx.DisplayStandard, false);
            }

        }

        public void ChangeEmployeePwd(string strURL)
        {
            int nRes = 0;
            bool bRes;

            client.Url = strURL;
            client.ChangeEmplPwd_ws(m_OperId, m_userPwd, m_userNewPwd, out nRes, out bRes);

            if (nRes == 1)
            {
                if (bEmplPwdExpired)
                {
                    myPsx.psx.SetConfigProperty("ConfirmMessage", "PWDChangeSuccessfull", Psx.PropertyTextFormat, myPsx.psx.GetString("PWDExpiredAndChangedSuccessfullyLeadthru"));
                    myPsx.psx.SetContext("PWDChangeSuccessfull", Psx.DisplayStandard, false);
                }
                else
                {
                    myPsx.psx.SetConfigProperty("ConfirmMessage", "PWDChangeSuccessfull", Psx.PropertyTextFormat, myPsx.psx.GetString("PWDChangeSuccessfullLeadthru"));
                    myPsx.psx.SetContext("PWDChangeSuccessfull", Psx.DisplayStandard, false);
                }
            }
            else if (nRes == -2)
            {
                myPsx.psx.SetContext("SamePasswordEntered", Psx.DisplayStandard, false);
            }
            // (+) POS151582.
            else if (nRes == -3)
            {
                myPsx.psx.SetContext("InvalidPassword", Psx.DisplayStandard, false);
            }
            // (-) POS151582.
        }


        public void GetMaxTryInvalidPassword()
        {
            bool bRes;
            try
            {
                client.Url = GetWebServiceURL(201);
                client.GetMaxInvalidPwdAttempts_ws(out m_MaxtryPwd, out bRes);
            }
            catch (Exception ex)
            {

                CmDataCapture.Capture(CmDataCapture.MaskError, "POSFileAccessService Exception for " + client.Url + " server: " + ex.Message);  //POS101386
                try
                {
                    client.Url = GetWebServiceURL(202);
                    client.GetMaxInvalidPwdAttempts_ws(out m_MaxtryPwd, out bRes);

                }
                catch (Exception ex1)
                {

                    CmDataCapture.Capture(CmDataCapture.MaskError, "POSFileAccessService Exception for " + client.Url + " server: " + ex1.Message); //POS101386
                    myPsx.psx.SetContext("UnableToConnectToServer", Psx.DisplayStandard, false);
                }
            }
        }



        /// <summary>
        /// EnterId Psx Context name.
        /// </summary>		
        private readonly string contextEnterId;

        /// <summary>
        /// EnterPassword Psx Context name.
        /// </summary>		
        private readonly string contextEnterPassword;


        /// <summary>
        /// Timer that waits for sign on results.
        /// </summary>
        private System.Threading.Timer checkInTimer = null;
    }
}
