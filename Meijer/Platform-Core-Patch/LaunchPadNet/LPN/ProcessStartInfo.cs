/// 
/// The Process class. The class System.Diagnostics.Process was
/// not available in Windows CE. Instead, this ProcessStartInfo class will be used
/// for Windows CE.
/// 

//using System;
//using System.Collections;
//using System.Drawing;
//using System.Threading;
//using PsxNet;
//using RPSWNET;

/// <summary>
/// The SetWindowPosInsertAfter enumeration corresponds to the
/// SetWindowPosInsertAfter Constants of the Windows API.
/// </summary>
public enum ProcessWindowStyle : int
{
	Hidden = 0,		// The hidden window style
	Maximized,		// The maximized window style.
	Minimized,		// The minimized window style.
	Normal
}

namespace LaunchPadNet
{
	/// <summary>
	/// The ProcessStartInfo class. 
	/// </summary>
	public class ProcessStartInfo
	{
		/// <summary>
		/// The class constructor.
		/// </summary>
		public ProcessStartInfo()
		{
		}

		/// <summary>
		/// Initializes a new instance of the ProcessStartInfo class and specifies a file name such as an application or document with which to start the process.
		/// </summary>
		public ProcessStartInfo( string fileName )
		{
			this.FileName = fileName;
		}

		/// <summary>
		/// The class destructor.
		/// </summary>
		~ProcessStartInfo()
		{
		}
		
		/// <summary>
		/// Gets or sets the set of command line arguments to use when starting the application.
		/// </summary>
		public string Arguments
		{
			get
			{
				return arguments;
			}
			
			set
			{
				arguments = value;
			}
		}

		/// <summary>
		/// Gets or sets the application or document to start.
		/// </summary>
		public string FileName
		{
			get
			{
				return fileName;
			}
			
			set
			{
				fileName = value;
			}
		}

		/// <summary>
		/// Gets or sets the set of command line arguments to use when starting the application.
		/// </summary>
		internal PInvoke.PROCESS_INFORMATION ProcessInformationStruct
		{
			get
			{
				return pi;
			}
			set
			{
				pi = value;
			}
		}

		/// <summary>
		/// Gets or sets the set of command line arguments to use when starting the application.
		/// </summary>
		public ProcessWindowStyle WindowStyle
		{
			get
			{
				return windowStyle;
			}
			
			set
			{
				windowStyle = value;
			}
		}

		private PInvoke.PROCESS_INFORMATION pi;

		private string arguments = "";

		private string fileName = "";

		private ProcessWindowStyle windowStyle = ProcessWindowStyle.Normal;
	}
}