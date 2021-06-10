using System;
using System.Threading;
using System.Text;
using PsxNet;
using RPSWNET;

namespace CommonLibsNet
{
    /// <summary>
    /// Encapsulates scanner worker thread
    /// </summary>
    public class ScannerThread : ScannerGeneric
    {
        /// <summary>
        /// The class constructor
        /// </summary>
        /// <param name="paramPsx">
        /// Psx instance used to host the scanner
        /// </param>
        /// <param name="encryptScanData">
        /// Flag indicating if scan data should be encrypted
        /// </param>
        /// <param name="configData">
        /// Configuration Data
        /// </param>
        public ScannerThread(Psx paramPsx, bool encryptScanData, ConfigData configData)
            : base(paramPsx, encryptScanData, configData)
        {
        }

        /// <summary>
        /// Releases the resources used by the ScannerThread instance.
        /// </summary>
        public override void Dispose()
        {
            if (null != myPsx)
            {
                PPCScanAbort();
            }
            autoEvent.Set();
            GC.SuppressFinalize(this);
            ScannerEnabled = false;
        }

        public override bool Start()
        {
            if (Open(myConfigData.NcrRemoteScanner))
            {
                Thread t = new Thread(new ThreadStart(ThreadMethod));
                t.Start();
                return true;
            }
            return false;
        }

        /// <summary>
        /// Disables the scanner.
        /// </summary>        
        public override void Disable()
        {
            if (!ConfigData.IsProxyServer)
            {
                if (null != myPsx)
                {
                    CmDataCapture.Capture(CmDataCapture.MaskWarning, "ScannerThread::Disable() - PPCScanDisable is called");
                    PPCScanDisable();
                }
                else
                {
                    CmDataCapture.Capture(CmDataCapture.MaskWarning, "ScannerThread::Disable() - Scanner is not opened");
                }
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
            bool success = false;
            if (null != myPsx)
            {
                CmDataCapture.Capture(CmDataCapture.MaskWarning, "ScannerThread::Enable() - PPCScanEnable is called");
                try
                {
                    success = PPCScanEnable(host); //might caused for a hang
                    if (!success)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerThread::Enable() - PPCScanEnable failed on host: " + host);
                    }
                    else
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerThread::Enable() - PPCScanEnable succeeded on host: " + host);
                    }
                }
                catch (SystemException e)
                {
                    success = false;
                    CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerThread::Enable() - SystemException caught: " + e.Message);
                }
            }
            else
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerThread::Enable() - Scanner is not opened");
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
            try
            {
                if (!PPCScanOpen(regKey))
                {
                    myPsx = null;
                }
            }
            catch (SystemException e)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerThread::Open() - SystemException caught: " + e.Message);
                myPsx = null;
            }
            if (null != myPsx)
            {
                new Thread(new ThreadStart(ThreadMethod)).Start();
                return true;
            }
            return false;
        }

        /// <summary>
        /// Worker thread method that monitors incoming scanner data.
        /// </summary>
        private void ThreadMethod()
        {
            try
            {
                int scanResult = 0;
                StringBuilder scanData = new StringBuilder(2000);
                StringBuilder scanDataLabel = new StringBuilder(2000);
                while (myPsx != null)
                {
                    try
                    {
                        PPCScanSetBinaryConversion(EncryptScanData ? OPOS_BC_DECIMAL : OPOS_BC_NONE);
                    }
                    catch (Exception)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskWarning, "ScannerThread::ThreadMethod() - PPCScanSetBinaryConversion API not supported");
                    }
                    scanResult = PPCScanRead(scanData, scanDataLabel, PInvoke.INFINITE);
                    if (scanResult >= 0)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, "ScannerThread::ThreadMethod() - PPCScanRead succeeded with scan type: " + scanResult.ToString());
                        myPsx.PlaySound(myConfigData.ScanSound);
                        FireScannerEvent(this, new ScannerEventArgs(scanData.ToString(), scanDataLabel.ToString(), scanResult));
                    }
                    else if (-1 == scanResult)
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerThread::ThreadMethod() - PPCScanRead timed out");
                    }
                    else
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskError, "ScannerThread::ThreadMethod() - PPCScanRead failed with error: " +
                            scanResult.ToString());
                    }
                    if (autoEvent.WaitOne(500, false))
                    {
                        myPsx = null;
                        CmDataCapture.Capture(CmDataCapture.MaskWarning, "ScannerThread::ThreadMethod() - PPCScanDisable is called");
                        PPCScanDisable();
                        PPCScanClose();
                    }
                }
            }
            catch (SystemException e)
            {
                CmDataCapture.Capture(CmDataCapture.MaskError, "ThreadMethod::ScannerThread() - SystemException caught: " + e.Message);
                myPsx = null;
            }
        }

        /// <summary>
        /// Event handle used to signal the exit of ThreadMethod.
        /// </summary>
        private AutoResetEvent autoEvent = new AutoResetEvent(false);
    }
}
