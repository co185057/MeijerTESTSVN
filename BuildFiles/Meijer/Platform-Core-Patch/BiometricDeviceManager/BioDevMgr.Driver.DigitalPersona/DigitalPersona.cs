using System;


namespace BioDevMgr
{
    /// <summary>
    /// Plugin that implements the digitalPersona device driver
    /// It uses the latest One Touch for Windows v1.3.0 .NET components 
    /// supplied and supported by digitalPersona.
    /// </summary>
    public class DigitalPersona : PluginDriver, DPFP.Capture.EventHandler
    {

        private TraceHelper trace = new TraceHelper("DigitalPersona");
        private DPFP.Capture.Capture capturer;

        protected override void Load()
        {
            try
            {
                // An digitalPersona specific component initialization
                // This method will not be used to load and claim the device.
                // Loading/claiming will be performed in the Start method.
                capturer = new DPFP.Capture.Capture(DPFP.Capture.Priority.High);					// Create a capture operation.
                capturer.EventHandler = this;
                base.Load();
            }
            finally
            {
                PluginActivity = PluginActivityTypes.Idle;
            }
        }


        public override void Start()
        {
            try
            {
                PluginActivity = PluginActivityTypes.Processing;
                // Load & claim the fingerprint capture device
                capturer.StartCapture();
                base.Start();
            }
            finally
            {
                PluginActivity = PluginActivityTypes.Idle;
            }
        }


        public override void Stop()
        {
            try
            {
                PluginActivity = PluginActivityTypes.Processing;
                capturer.StopCapture();
                base.Stop();
            }
            finally
            {
                PluginActivity = PluginActivityTypes.Idle;
            }
        }

        #region EventHandler Members

        public void OnComplete(object Capture, string ReaderSerialNumber, DPFP.Sample Sample)
        {
            try
            {
                trace.Info("The fingerprint was captured successfully. Serial # " + ReaderSerialNumber);

                DPFP.Capture.CaptureFeedback feedback = DPFP.Capture.CaptureFeedback.None;
                DPFP.FeatureSet featureSet = new DPFP.FeatureSet();
                DPFP.Processing.FeatureExtraction extract = new DPFP.Processing.FeatureExtraction();
                extract.CreateFeatureSet(Sample, DPFP.Processing.DataPurpose.Verification, ref feedback, ref featureSet);
                byte[] featureData = featureSet.Bytes;

                if (feedback == DPFP.Capture.CaptureFeedback.Good)
                {
                    OnPluginEvent(new PluginEventArgs(PluginEventArgs.PluginEventTypes.OnData, featureData, null));
                }
                else
                {
                    trace.Info("Fingerprint feature set was not good. Must scan again");
                }
            }
            finally
            {
                PluginActivity = PluginActivityTypes.Idle;
            }
        }

        public void OnFingerGone(object Capture, string ReaderSerialNumber)
        {
            trace.Info("Detected finger has been removed from the reader. Serial # " + ReaderSerialNumber);
        }

        public void OnFingerTouch(object Capture, string ReaderSerialNumber)
        {
            trace.Info("Detected finger has been placed on the reader. Serial # " + ReaderSerialNumber);
        }

        public void OnReaderConnect(object Capture, string ReaderSerialNumber)
        {
            if (DriverStatus == DriverStatusTypes.Undefined)
            {
                // startup connection
                trace.Warning("Fingerprint reader is connected.");
            }
            else
            {
                trace.Info("The fingerprint reader was reconnected. Serial # " + ReaderSerialNumber);
            }

            driverStatus = DriverStatusTypes.Connected;
        }

        public void OnReaderDisconnect(object Capture, string ReaderSerialNumber)
        {
            if (DriverStatus == DriverStatusTypes.Undefined)
            {
                // startup connection
                trace.Warning("The fingerprint reader is not connected.");
            }
            else
            {
                trace.Info("The fingerprint reader was disconnected. Serial # " + ReaderSerialNumber);
            }

            driverStatus = DriverStatusTypes.Disconnected;
        }

        public void OnSampleQuality(object Capture, string ReaderSerialNumber, DPFP.Capture.CaptureFeedback CaptureFeedback)
        {
            if (CaptureFeedback == DPFP.Capture.CaptureFeedback.Good)
            {
                trace.Info("The quality of the fingerprint sample is good.");
            }
            else
            {
                trace.Warning("The quality of the fingerprint sample is poor.");
            }
        }

        #endregion
    }
}
