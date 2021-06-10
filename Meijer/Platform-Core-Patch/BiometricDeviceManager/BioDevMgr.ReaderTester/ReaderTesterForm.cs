using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace BioDevMgr.ReaderTester
{
    public partial class ReaderTesterForm : Form, DPFP.Capture.EventHandler
    {
        private DPFP.Capture.Capture capturer;

        public ReaderTesterForm()
        {
            InitializeComponent();
        }

        private void ReaderTesterForm_Load(object sender, EventArgs e)
        {
            capturer = new DPFP.Capture.Capture(DPFP.Capture.Priority.High);
            capturer.EventHandler = this;
            capturer.StartCapture();
        }

        public void OnComplete(object Capture, string ReaderSerialNumber, DPFP.Sample Sample)
        {
            System.Diagnostics.Trace.TraceInformation("OnComplete: " + ReaderSerialNumber);
        }

        public void OnFingerGone(object Capture, string ReaderSerialNumber)
        {
            System.Diagnostics.Trace.TraceInformation("OnFingerGone: " + ReaderSerialNumber);
        }

        public void OnFingerTouch(object Capture, string ReaderSerialNumber)
        {
            System.Diagnostics.Trace.TraceInformation("OnFingerTouch: " + ReaderSerialNumber);
        }

        public void OnReaderConnect(object Capture, string ReaderSerialNumber)
        {
            string s = ReaderSerialNumber;
            System.Diagnostics.Trace.TraceInformation("connect: " + s);
        }

        public void OnReaderDisconnect(object Capture, string ReaderSerialNumber)
        {
            string s = ReaderSerialNumber;
            System.Diagnostics.Trace.TraceInformation("disconnect: " + s);
        }

        public void OnSampleQuality(object Capture, string ReaderSerialNumber, DPFP.Capture.CaptureFeedback CaptureFeedback)
        {
            System.Diagnostics.Trace.TraceInformation("OnSampleQuality: " + ReaderSerialNumber);
        }

        private void ReaderTesterForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            capturer.StopCapture();
        }
    }
}