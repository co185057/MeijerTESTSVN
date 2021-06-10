using PsxNet;
using System;
using RPSWNET;
using System.Text;
using System.Drawing;
using System.Threading;
using System.Diagnostics;

namespace RapNet
{
    /// <summary>
	/// Encapsulates Psx connection worker thread.
	/// </summary>
	public class ConnectThread : IDisposable
	{
		/// <summary>
		/// The class constructor.
		/// </summary>
		public ConnectThread()
		{			
		}

		/// <summary>
		/// The class destructor.
		/// </summary>
		~ConnectThread()
		{
			Dispose();
		}

		/// <summary>
		/// Releases the resources used by the ConnectThread instance.
		/// </summary>
		public void Dispose()
		{
            autoEvent.Set();
            while (null != lanePsxArray)
            {
                Thread.Sleep(1000);
            }            
            GC.SuppressFinalize(this);            
		}

        /// <summary>
        /// Returns whether the thread is running or not.
        /// </summary>
        public bool IsRunning()
        {
            return lanePsxArray != null;
        }

        /// <summary>
        /// Sets or gets the LanePsxArray.
        /// </summary>
        public LanePsx[] LanePsxArray
        {
            get { return lanePsxArray; }
            set { lanePsxArray = value; }
        }

        protected virtual void GenerateConnectionString(LanePsx lane, StringBuilder connectionString)
        {
            connectionString.Append("HandheldRAP=");
            connectionString.Append(ConfigData.IsMobileUI ? "1;" : "0;");            
            if (lane.Owner.SignedOn)
            {
                connectionString.Append("operation[text]=sign-on-auto;UserId[text]=");
                connectionString.Append(lane.Owner.OperatorId);
                connectionString.Append(";RapValidHeadCashier[text]=");
                connectionString.Append(lane.Owner.RapValidHeadCashier);
                connectionString.Append(";");
            }
            connectionString.Append("RapSingleSize=");
            connectionString.Append(lane.RapSingleContext);
            connectionString.Append(";");

            connectionString.Append("RAPLite=");
            if (ConfigData.GetConfigData().EnableRAPLite)
            {
                connectionString.Append("1;");

                connectionString.Append("DeviceType=");

                if (ConfigData.IsProxyServer)
                {
                    connectionString.Append("XRAPLite;");
                }
                else
                {
                    connectionString.Append(ConfigData.IsMobileUI ? "HHRAPLite;" : "Desktop RAP;");
                }
            }
            else 
            {
                connectionString.Append("0;");

                connectionString.Append("DeviceType=");
                connectionString.Append(ConfigData.IsMobileUI ? "Mobile RAP;" : "Desktop RAP;");
            }
        }

        protected virtual void HandleResponseString(LanePsx lane, String responseString)
        {
            CmDataCapture.Capture(CmDataCapture.MaskInfo,
                String.Format("LanePsx::HandleResponseString() Lane#:{0}, responseString={1}",
                    lane.LaneNumber, responseString));

            lane.LaneIntervention = new Intervention(responseString);

            string str = ConfigData.ExtractRapEventData("acquire", responseString);
            if (str != null)
            {
                lane.DisableContextRemote = Convert.ToBoolean(str);                
            }
            str = ConfigData.ExtractRapEventData("encrypt-scan-data", responseString);
            if (str != null)
            {
                ScannerThread.EncryptScanData = Convert.ToBoolean(str);
            }            
            lane.RefreshContext();

            if (ConfigData.GetConfigData().EnableRAPLite
                && (Intervention.Type.Immediate == lane.LaneIntervention.type) && !ConfigData.IsProxyServer)
            {
                lane.psx.Visible = true;
            }

            lane.Owner.RefreshUi();

            if (!ConfigData.IsProxyServer)
            {
                lane.Owner.TileLanes();
            }

            if (null != lane.Owner.TrxCount)
            {
                lane.Owner.TrxCount.ShowTransactionCount();
            }
            Remote.SendMessage(Remote.LaunchpadApplication, "operation=set-active-host;host=" + ConfigData.ExtractRapEventData("host", lane.HostName));                
        }
						
		/// <summary>
		/// Worker thread method that tries to connect our Psx to it's corresponding FastLane Psx.
		/// </summary>
		internal void ThreadMethod()
		{
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "ConnectThread::ThreadMethod() - Remote connection thread start");            
            ConfigData configData = ConfigData.GetConfigData();            
			StringBuilder connectionString = new StringBuilder();
			bool laneNotConnected = false;
            string hostName = "";
            string service = "";
            LanePsx lane = null;
            while (lanePsxArray != null)
    		{
                laneNotConnected = false;
                for (int n = 0; n < lanePsxArray.Length; n++)
                {
                    if (autoEvent.WaitOne(0, false))
                    {
                        lanePsxArray = null;
                        laneNotConnected = false;
                        break;
                    }
                    lane = LanePsxArray[n];
                    if (null != lane && lane.RemoteConnection.Equals("invalid"))
                    {
                        try
                        {
                            connectionString.Length = 0;
                            GenerateConnectionString(lane, connectionString);
                            object param = connectionString.ToString().Clone();

                            hostName = ConfigData.ExtractRapEventData("host", lane.HostName);
                            service = ConfigData.ExtractRapEventData("service", lane.HostName);
                            if (service == null)
                            {
                                service = Remote.FastlaneService;
                            }
                            lane.psx.ConnectRemote(hostName, service, "RAP." + configData.ComputerName + "." +
                                Process.GetCurrentProcess().ProcessName, ref param, (uint)configData.ConnectRemoteTimeout);
                            HandleResponseString(lane, param.ToString());
                            CmDataCapture.Capture(CmDataCapture.MaskInfo, "ConnectThread::ThreadMethod() - ConnectRemote success with host: " +
                                hostName + ", service:" + service);
				        }
				        catch (PsxException caught)
				        {
                            laneNotConnected = true;
                            lane.ResetUi();
                            CmDataCapture.Capture(CmDataCapture.MaskWarning, "ConnectThread::ThreadMethod() - ConnectRemote failed with host: " +
                                hostName + ", service:" + service + ", error: " + caught.Message);
				        }
				        catch (Exception caught)
				        {
                            laneNotConnected = true;
                            CmDataCapture.Capture(CmDataCapture.MaskError, "ConnectThread::ThreadMethod() - Exception: " + caught.Message);					        
				        }
                    }
                }
                if (!laneNotConnected || autoEvent.WaitOne(configData.ConnectionPeriod, false))
                {
                    lanePsxArray = null;                    
                }
			}
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "ConnectThread::ThreadMethod() - Remote connection thread end");
		}

		/// <summary>
		/// Event used to signal the exit of ThreadMethod.
		/// </summary>
        private AutoResetEvent autoEvent = new AutoResetEvent(false);

        /// <summary>
        /// Array of lane specific Psx instances.
        /// </summary>
        private LanePsx[] lanePsxArray = null;

	}
}