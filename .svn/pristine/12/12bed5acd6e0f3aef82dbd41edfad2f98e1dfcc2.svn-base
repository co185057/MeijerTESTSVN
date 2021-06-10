#undef WINCE

using System;
using System.Runtime.InteropServices;
using System.Threading;
using System.Text;
using PsxNet;
using RPSWNET;

namespace CommonLibsNet
{
    /// <summary>
    /// This class is a replacement for the existing ScannerThread class, containing the scanner
    /// event management as well as encapsulating a scanner sharing mechanism that allows the 
    /// scanner to be available to RapNet, LaunchPad and ScotApp when required.  RapNet and SCOTApp
    /// will maintain ownership of the scanner as is done currently and LaunchPad will request the
    /// use of the scanner on demand when it's in focus.  The RemoteMessage mechanism will be used
    /// to synchronize between the applications to avoid device acquisition contention.
    /// </summary>
    public class ScannerShared : ScannerGeneric
    {
        /// <summary>
        /// The class constructor.
        /// </summary>
        /// <param name="paramPsx">
        /// Psx instance used to host the scanner.
        /// </param>
        /// <param name="configData">
        /// Configuration data 
        /// </param>
        public ScannerShared(Psx paramPsx, bool encryptScanData, ConfigData configData)
            : base(paramPsx, encryptScanData, configData)
        {

        }

        /// <summary>
        /// Releases the resources used by the ScannerShared instance.
        /// </summary>
        public override void Dispose()
        {
            if (ScannerOpened || ScannerEnabled)
                Disable();

            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Starts the scanner processing
        /// </summary>
        public override bool Start()
        {
            return Open(myConfigData.NcrRemoteScanner);
        }

		private void AbortScannerThread()
		{
			CmDataCapture.Capture(CmDataCapture.MaskInfo, "Triggering scanner thread exit event");
			autoEvent.Set(); //trigger the exit event for scanner thread

			CmDataCapture.Capture(CmDataCapture.MaskInfo, "Aborting any scan/read operations on the scanner");
			PPCScanAbort();
			if (threadMethod != null && threadMethod.IsAlive && !threadMethod.Join(2000))
			{
				try
				{
					// thread seems hung - terminate it explicitly.
					CmDataCapture.Capture(CmDataCapture.MaskInfo, "Scanner thread joining timed out. Aborting the thread...");
					threadMethod.Abort();
				}
				catch (ThreadStateException tse)
				{
					CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerShared::Disable() - Thread abort raised exception: " + tse.Message);
				}
			}
			CmDataCapture.Capture(CmDataCapture.MaskInfo, "Disabling the scanner");
			PPCScanDisable();
			CmDataCapture.Capture(CmDataCapture.MaskInfo, "Closing the scanner");
			PPCScanClose();
			ScannerOpened = ScannerEnabled = false;
			threadMethod = null;
			CmDataCapture.Capture(CmDataCapture.MaskInfo, "ScannerShared::Disable() - Done.");
		}


        /// <summary>
        /// Disables the scanner.
        /// </summary>        
        public override void Disable()
        {
            if (ScannerEnabled)
            {
                AbortScannerThread();
            }
            else
            {
                if (ScannerOpened)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "Scanner was not enabled hence, just closing the scanner");
                    PPCScanClose();
                    ScannerOpened = false;
                }
                else
                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "ScannerShared::Disable() - Scanner already closed.");
            }
        }

        /// <summary>
        /// Enables the scanner.
        /// </summary>
        /// <param name="host">
        /// Machine name we are binding the scanner to.
        /// </param>
        /// <returns>
        /// Return true if successful.
        /// </returns>
        public override bool Enable(string host)
        {
            if (ScannerEnabled)
            {
                CmDataCapture.Capture(CmDataCapture.MaskInfo, "Returning as the scanner is already enabled");
                return true;
            }

			//Corner case
			if (threadMethod != null && threadMethod.IsAlive)
			{
				AbortScannerThread();
			}

            bool success = false;
            try
            {
                CmDataCapture.Capture(CmDataCapture.MaskInfo, "Opening the scanner...");
                if (PPCScanOpen(scannerRegKey))
                {
                    ScannerOpened = true;
                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "Enabling the scanner...");
                    success = PPCScanEnable(host);
                    if (!success)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "Failed to enable the scanner");
                        Disable();
                    }
                    else
                    {
                        ScannerEnabled = true;
                        threadMethod = new Thread(new ThreadStart(ThreadMethod));
                        threadMethod.Start();                        
                        CmDataCapture.Capture(CmDataCapture.MaskError, "Successfully enabled the scanner");
                    }
                }
                else
                {
                    CmDataCapture.Capture(CmDataCapture.MaskError, "Failed to open the scanner");
                }
            }
            catch (SystemException e)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerShared::Enable() - SystemException caught: " + e.Message);
                Disable();
            }
            catch (Exception e)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerShared::Enable() - Unknown error: " + e.Message);
                Disable();
            }

            return success;
        }

        /// <summary>
        /// Loads the scanner.
        /// </summary>
        /// <param name="regKey">
        /// Registry key where configuration information is stored.
        /// </param>
        /// <returns>
        /// Return true if successful.
        /// </returns>
        public override bool Open(string regKey)
        {
            // We don't actually open the device until it is to be enabled.
            scannerRegKey = regKey;
            return true;
        }

        /// <summary>
        /// Worker thread method that monitors incoming scanner data.
        /// </summary>
        public void ThreadMethod()
        {
            try
            {
                CmDataCapture.Capture(CmDataCapture.MaskInfo, "ScannerShared::ThreadMethod() - Entering thread.");
                string scanSound = myConfigData.ScanSound.Length > 0 ? myConfigData.AudioPath + "\\" + myConfigData.ScanSound : "";
                int scanResult = 0;
                StringBuilder scanData = new StringBuilder(2000);
                StringBuilder scanDataLabel = new StringBuilder(2000);

                while (ScannerEnabled)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskInfo, "Issuing a read call on the scanner to wait for the scanner event...");
                    scanData.Length = 0;
                    scanDataLabel.Length = 0;
                    scanResult = PPCScanRead(scanData, scanDataLabel, PInvoke.INFINITE);
                    if (scanResult >= 0)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "PPCScanRead succeeded with scan type: " + scanResult.ToString());
                        if (scanSound.Length > 0)
                        {
                            try
                            {
                                myPsx.PlaySound(scanSound);
                            }
                            catch (PsxNet.PsxException psex)
                            {
                                CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerShared::ThreadMethod() - PlaySound failed for file " + scanSound + ". Exception: " + psex.Message);
                            }
                        }
                        FireScannerEvent(this, new ScannerEventArgs(scanData.ToString(), scanDataLabel.ToString(), scanResult));
                    }
                    else if (-1 == scanResult)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "Read operation on the scanner has been timed out");
                    }
                    else
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "Read operation failed with error code: " + scanResult.ToString());
                    }
                    if (autoEvent.WaitOne(500, false))
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "Exiting the ScannerShared::ThreadMethod() - Exit event was triggered externally");
                        break;
                    }
                }                
            }
            catch (ThreadAbortException tae)
            {
                Thread.ResetAbort();
                CmDataCapture.Capture(CmDataCapture.MaskError,
                    "ScannerShared::ThreadMethod() - Abort interruption issued by other thread (likely due to timeout): " + tae.Message);
                autoEvent.Reset(); // exception misses WaitOne() path and requires explicit reset
                CleanUpOnScannerThreadAbort();
            }
            catch (SystemException e)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerShared::ThreadMethod() - SystemException caught: " + e.Message);
                autoEvent.Reset(); // exception misses WaitOne() path and requires explicit reset
                CleanUpOnScannerThreadAbort();
            }

            CmDataCapture.Capture(CmDataCapture.MaskInfo, "ScannerShared::ThreadMethod() - Exiting thread.");                        
        }

        private void CleanUpOnScannerThreadAbort()
        {
            //We're aborting from scanner controlling thread. Clean up goes here - close the scanner and reset the controlling flags
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "Aborting any scan/read operations on the scanner");
            PPCScanAbort();
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "Disabling the scanner");
            PPCScanDisable();
            CmDataCapture.Capture(CmDataCapture.MaskInfo, "Closing the scanner");
            PPCScanClose();
            ScannerOpened = ScannerEnabled = false;
        }

        /// <summary>
        /// ThreadMethod thread 
        /// </summary>
        private Thread threadMethod = null;

        /// <summary>
        /// Event handle used to signal the exit of ThreadMethod.
        /// </summary>
        private AutoResetEvent autoEvent = new AutoResetEvent(false);

        /// <summary>
        /// Store the registry key containing the scanner device settings
        /// </summary>
        private string scannerRegKey = "";
    }
}
