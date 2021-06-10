using System;

namespace BioDevMgr
{
	/// <summary>
	/// Summary description for PluginDriver.
	/// </summary>
	public abstract class PluginDriver : Plugin
	{


		protected PluginDriver()
		{
			// prevent instantiation
		}

        public enum DriverStatusTypes
        {
            Disconnected,
            Connected,
            Undefined
        }

        protected string GetDriverStatusString(DriverStatusTypes type)
        {
            switch (type)
            {
                case DriverStatusTypes.Disconnected: return "Disconnected";
                case DriverStatusTypes.Connected: return "Connected";
                case DriverStatusTypes.Undefined: return "Undefined";
                default: return "UNKNOWN";
            };
        }

        protected DriverStatusTypes driverStatus = DriverStatusTypes.Undefined;

        /// <summary>
        /// Current driver status
        /// </summary>
        public DriverStatusTypes DriverStatus
        {
            get
            {
                return driverStatus;
            }
        }

        /// <summary>
        /// Driver status in string format
        /// </summary>
        public string DriverStatusString
        {
            get
            {
                return GetDriverStatusString(driverStatus);
            }
        }

		protected override void OnPluginEvent(PluginEventArgs e)
		{
			// Do any Connector specific processing here.

			// Call the base class event invocation method.
			base.OnPluginEvent(e);
		}
	}
}
