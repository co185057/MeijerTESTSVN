using System;
using System.Data;
using System.Runtime.InteropServices;

namespace Shutdown
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class Class1
	{
		const string coreDll = "coredll.dll";

		[DllImport(coreDll, SetLastError = true)]
		static extern int SetSystemPowerState(string psState, int StateFlags, int Options);

		public enum PowerState
		{
			POWER_STATE_ON = 0x00010000,
			POWER_STATE_OFF = 0x00020000,
			POWER_STATE_CRITICAL = 0x00040000,
			POWER_STATE_BOOT = 0x00080000,
			POWER_STATE_IDLE = 0x00100000,
			POWER_STATE_SUSPEND = 0x00200000,
			POWER_STATE_UNATTENDED = 0x00400000,
			POWER_STATE_RESET = 0x00800000,
			POWER_STATE_USERIDLE = 0x01000000,
			POWER_STATE_PASSWORD = 0x10000000
		}

		public enum PowerRequirement
		{
			POWER_NAME = 0x00000001,
			POWER_FORCE = 0x00001000,
			POWER_DUMPDW = 0x00002000
		}

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		static void Main(string[] args)
		{
			int iRet = SetSystemPowerState(null, (int)PowerState.POWER_STATE_OFF, (int)PowerRequirement.POWER_FORCE);
		}
	}
}

