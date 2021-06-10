using System;
using System.Text;
using System.Runtime.InteropServices;
using RPSWNET;

namespace LaunchPadNet
{
	/// <summary>
	/// PInvoke definitions used by the application.
	/// </summary>
	internal class PInvoke
	{
        public const uint HKEY_LOCAL_MACHINE = 0x80000002;
        public const uint KEY_READ = 0x00020019;
        public const uint REG_DWORD = 4;
        public const uint REG_SZ = 1;
		public const uint WAIT_TIMEOUT = 258;
		public const uint WAIT_OBJECT_0 = 0;
		public const uint INFINITE = 0xFFFFFFFF;  // Infinite timeout
		public const uint WM_USER = 0x0400;
		public const uint WM_CLOSE = 0x0010;
		public const uint MAX_PATH = 260;
		public const uint MB_YESNO = 4;
		public const uint IDYES = 6;
		public const uint MB_ICONQUESTION = 0x00000020;
		public const uint STILL_ACTIVE = 259;
		public const uint KEY_SET_VALUE = 0x0002;  //Required to create, delete, or set a registry value.

        [StructLayout( LayoutKind.Sequential )] public struct SYSTEMTIME
        {
            public short Year;
            public short Month;
            public short DayOfWeek;
            public short Day;
            public short Hour;
            public short Minute;
            public short Second;
            public short Milliseconds;
		}

		[StructLayout( LayoutKind.Sequential )] 
			public struct POINT
		{
			public int x;
			public int y;
		}


		[StructLayout( LayoutKind.Sequential )] 
			public struct MSG 
		{
			IntPtr	hWnd;
			UIntPtr wParam;
			IntPtr	lParam;
			uint	time;
			POINT	point;
		}


		[StructLayout( LayoutKind.Sequential )] public struct RECT
		{
			public int left; 
			public int top; 
			public int right; 
			public int bottom;
		}

		public struct WINDOWPLACEMENT 
		{ 
			public uint  length; 
			public uint  flags; 
			public uint  showCmd; 
			public POINT ptMinPosition; 
			public POINT ptMaxPosition; 
			public RECT  rcNormalPosition; 
		}

		[StructLayout( LayoutKind.Sequential )] public struct MEMORYSTATUS
		{
			public int Length;
			public int MemoryLoad;
			public UIntPtr TotalPhysical;
			public UIntPtr AvailablePhysical;
			public UIntPtr TotalPageFile;
			public UIntPtr AvailablePageFile;
			public UIntPtr TotalVirtual;
			public UIntPtr AvailableVirtual;
		}

		/// <summary>
		/// Structure containing process and thread information.
		/// </summary>
		[StructLayout( LayoutKind.Sequential )] public struct PROCESS_INFORMATION 
		{
			public IntPtr hProcess;
			public IntPtr hThread;		
			public Int32 dwProcessId;	
			public Int32 dwThreadId;	
		}

		/// <summary>
		/// The SetWindowPosFlags enumeration corresponds to the SetWindowPos
		/// Constants of the Windows API.
		/// </summary>
		internal enum SetWindowPosFlags : uint
		{
			SWP_NOSIZE = 0x1,
			SWP_NOMOVE = 0x2,
			SWP_NOZORDER = 0x4,
			SWP_NOREDRAW = 0x8,
			SWP_NOACTIVATE = 0x10,
			SWP_FRAMECHANGED = 0x20, // The frame changed: send WM_NCCALCSIZE
			SWP_SHOWWINDOW = 0x40,
			SWP_HIDEWINDOW = 0x80,
			SWP_NOCOPYBITS = 0x100,
			SWP_NOOWNERZORDER = 0x200, //     '  Don't do owner Z ordering
			SWP_DRAWFRAME = SWP_FRAMECHANGED,
			SWP_NOREPOSITION = SWP_NOOWNERZORDER,
		}  /// <summary>

		/// The SetWindowPosInsertAfter enumeration corresponds to the
		/// SetWindowPosInsertAfter Constants of the Windows API.
		/// </summary>
		internal enum SetWindowPosInsertAfter : int
		{
			HWND_TOP = 0,
			HWND_BOTTOM = 1,
			HWND_TOPMOST = -1,
			HWND_NOTOPMOST = -2
		}

		/// <summary>
		/// The ShowWindowFlags enumeration of flags corresponds to the ShowWindow
		/// Constants of the Window API.
		/// </summary>
		internal enum ShowWindowFlags : int
		{
			SW_HIDE             = 0,
			SW_SHOWNORMAL       = 1,
			SW_NORMAL           = 1,
			SW_SHOWMINIMIZED    = 2,
			SW_SHOWMAXIMIZED    = 3,
			SW_MAXIMIZE         = 3,
			SW_SHOWNOACTIVATE   = 4,
			SW_SHOW             = 5,
			SW_MINIMIZE         = 6,
			SW_SHOWMINNOACTIVE  = 7,
			SW_SHOWNA           = 8,
			SW_RESTORE          = 9,
			SW_SHOWDEFAULT      = 10,
			SW_FORCEMINIMIZE    = 11,
			SW_MAX              = 11,
		}
  
		// Use this version when the caller allocates the strings
		// (For example: CreateProcess)
		//[StructLayout(LayoutKind.Sequential, CharSet=CharSet.Auto)]
		[StructLayout( LayoutKind.Sequential )] public struct STARTUPINFO
		{
			public uint    cb;
			public string  lpReserved;
			public string  lpDesktop;
			public string  lpTitle;
			public uint    dwX;
			public uint    dwY;
			public uint    dwXSize;
			public uint    dwYSize;
			public uint    dwXCountChars;
			public uint    dwYCountChars;
			public uint    dwFillAttribute;
			public uint    dwFlags;
			public ushort  wShowWindow;
			public ushort  cbReserved2;
			public IntPtr  lpReserved2;
			public IntPtr  hStdInput;
			public IntPtr  hStdOutput;
			public IntPtr  hStdError;
		} 

#if( WINCE )
        private const string kernelApiDll = "coredll.dll";
        private const string regApiDll = "coredll.dll";
        private const string userDll = "coredll.dll";
        private const string winsockDll = "coredll.dll";
		private const string winmmDll = "coredll.dll";

#if( DEBUG )
		private const string remoteMessageDll = "RemoteMessageUD.dll";
#else
		private const string remoteMessageDll = "RemoteMessageU.dll";
#endif

#else

#if( DEBUG )
		private const string remoteMessageDll = "RemoteMessageUD.dll";
#else
		private const string remoteMessageDll = "RemoteMessageU.dll";
#endif

        private const string kernelApiDll = "kernel32.dll";
        private const string regApiDll = "advapi32.dll";
        private const string userDll = "user32.dll";
		private const string winsockDll = "ws2_32.dll";
		private const string winmmDll = "winmm.dll";
#endif

        [DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern bool CloseHandle( IntPtr handle );
      
        [DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern IntPtr CreateEvent( IntPtr lpEventAttributes, bool bManualReset, bool bInitialState, string lpName );

		[DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
		public static extern bool EnableWindow( IntPtr hWnd, bool bEnabled );
        
        [DllImport(userDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern IntPtr FindWindow( string lpClassName, string lpWindowName );

		[DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
		public static extern bool FreeLibrary( IntPtr hModule );

		[DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
		public static extern bool GetExitCodeProcess( IntPtr handle, out uint exitCode );

		[DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
		public static extern void GlobalMemoryStatus( out MEMORYSTATUS lpBuffer );
        
		[DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
		public static extern IntPtr LoadLibrary( string lpFileName );

		[DllImport(userDll, CharSet=CharSet.Unicode, SetLastError=false)]
		public static extern int MessageBox( IntPtr hWnd, string lpText, string lpCaption, uint uType );

        [DllImport(regApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern int RegCloseKey( uint hkey );
        
        [DllImport(regApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern int RegOpenKeyExW( uint hkey, string subKey, uint options, uint sam, ref uint phkResult );

        [DllImport(regApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern int RegQueryValueExW( uint hkey, string valueName, int reserved, ref uint type, byte[] data, ref uint len );

        [DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern bool SetSystemTime( ref SYSTEMTIME time );
        
        [DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern uint WaitForSingleObject( IntPtr handle, uint milliseconds );
		
		[DllImport(kernelApiDll)]
		public static extern uint GetWindowsDirectory(
			StringBuilder Buffer,  // buffer for system directory
			uint uSize      // size of directory buffer
			);

		[DllImport(userDll)]
		public static extern int EnumWindows(
			CallBack x,
			int y);

		// WorkAround for EnumWindows because 
		// the callback delegate doesn't work in CE.NET
		[ DllImport( remoteMessageDll ) ]
		public static extern void GetWindowHandles( 
				[In, Out] IntPtr [] Handles, 
				ref int pCount );

		[DllImport(userDll)]
		public static extern IntPtr GetParent(
			IntPtr hWnd   // handle to child window
			);

		[DllImport(userDll)]
		public static extern bool GetWindowPlacement(
			IntPtr hWnd,                // handle to window
			out WINDOWPLACEMENT lpwndpl     // position data
			);

		[DllImport(userDll)]
		public static extern int GetWindowText(
			IntPtr hWnd,            // handle to window or control
			StringBuilder lpString, // text buffer
			int nMaxCount           // maximum number of characters to copy
			);
		
		[DllImport(userDll)]
		public static extern int GetWindowThreadProcessId(
			IntPtr hWnd,            // handle to window
			out int lpdwProcessId   // process identifier
			);

		[DllImport(userDll)]
		public static extern IntPtr SetFocus(
			IntPtr hWnd   // handle to window
			);
	
		[DllImport(userDll)]
		public static extern bool SetForegroundWindow(
			IntPtr hWnd   // handle to window
			);
		
		/// <summary>
		/// The GetForegroundWindow function returns a handle to the foreground window.
		/// </summary>
		[DllImport(userDll)]
		public static extern IntPtr GetForegroundWindow();

		/// <summary>
		/// The GetActiveWindow function returns a handle to the active window.
		/// </summary>
		[DllImport(userDll)]
		public static extern IntPtr GetActiveWindow();

		/// <summary>
		/// Determines if a window is visible (not hidden, minimized, etc.).
		/// </summary>
		[DllImport(userDll)]
		public static extern bool IsWindowVisible(IntPtr hWnd);

		[DllImport(userDll)]
		public static extern bool SetWindowPos(
			IntPtr hWnd,             // handle to window
			IntPtr hWndInsertAfter,  // placement-order handle
			int X,                   // horizontal position
			int Y,                   // vertical position
			int cx,                  // width
			int cy,                  // height
			uint uFlags              // window-positioning options
			);

		[DllImport(userDll)]
		public static extern bool ShowWindow(
			IntPtr hWnd,    // handle to window
			int nCmdShow    // show state
			);

		[DllImport(userDll)]
		public static extern bool BringWindowToTop(IntPtr hWnd);

		[DllImport(kernelApiDll)]
		public static extern bool CreateProcess(string lpApplicationName,
			string lpCommandLine, IntPtr lpProcessAttributes, IntPtr lpThreadAttributes,
			bool bInheritHandles, uint dwCreationFlags, IntPtr lpEnvironment,
			string lpCurrentDirectory, IntPtr lpStartupInfo,
			out PROCESS_INFORMATION lpProcessInformation);

		[DllImport(kernelApiDll)]
		public static extern void TerminateProcess( IntPtr handle, int exitCode );

		[DllImport(userDll, SetLastError = true)]
		public static extern bool PostMessage(IntPtr hWnd, uint Msg, UIntPtr wParam, IntPtr lParam);

		[DllImport(userDll, SetLastError = true)]
		public static extern bool PeekMessage(  ref MSG pMsg, IntPtr hWnd, uint filterMin, uint filterMax, uint removeMsg );

		public const uint EVENT_SET = 3;

		public static bool SetEvent( IntPtr handle )
		{
#if( WINCE )
			return Convert.ToBoolean( PInvoke.EventModify( handle, 3 ) );
#else
            return PInvoke.SetEventNative( handle );
#endif
		}

#if( WINCE )
        [DllImport(kernelApiDll, SetLastError=false)]
        public static extern int EventModify( IntPtr handle, uint code );
        
        [DllImport(winsockDll, SetLastError=false)]
#else
        [DllImport(kernelApiDll, EntryPoint="SetEvent", CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern bool SetEventNative( IntPtr handle );

		[DllImport(winsockDll, CharSet=CharSet.Ansi, SetLastError=false)]
#endif
        public static extern int gethostname( StringBuilder host, int len );

		// SystemParameterInfo uiAction constants
		public const int SPI_GETFOREGROUNDLOCKTIMEOUT = 0x2000;
		public const int SPI_SETFOREGROUNDLOCKTIMEOUT = 0x2001;

		// SystemParameterInfo uiInit constants
		public const int SPIF_SENDWININICHANGE = 2;
		public const int SPIF_UPDATEINIFILE = 1;

		[DllImport(userDll)]
		public static extern bool SystemParametersInfo(uint uiAction, uint uiParam, IntPtr pvParam, uint uiInit);

		// CURRENTLY NOT USED
		//[DllImport(userDll, EntryPoint="SystemParametersInfo", SetLastError = true)]
		//public static extern bool SystemParametersInfoGet(uint action, uint param, ref uint vparam, uint init);

		[DllImport(userDll, EntryPoint="SystemParametersInfo", SetLastError = true)]
		public static extern bool SystemParametersInfoSet(uint action, uint param, uint vparam, uint init);

#if	( WINCE )

		public const uint SHFS_HIDETASKBAR		= 0x0002;
		public const uint SHFS_HIDESIPBUTTON	= 0x0008;
		public const uint SHFS_HIDESTARTICON	= 0x00020;
		public const uint SHFS_SHOWTASKBAR		= 0x0001;
		public const uint SHFS_SHOWSIPBUTTON	= 0x0004;
		public const uint SHFS_SHOWSTARTICON	= 0x0010;
		public const uint SHFS_HIDEALL			=  SHFS_HIDETASKBAR 
													| SHFS_HIDESIPBUTTON 
													| SHFS_HIDESTARTICON;

		public const uint SHFS_SHOWALL			=  SHFS_SHOWTASKBAR
													| SHFS_SHOWSIPBUTTON	
													| SHFS_SHOWSTARTICON;
					
		[DllImport( "aygshell.dll" ) ]
		public static extern void SHFullScreen( IntPtr hWnd, uint settings );

#else
		internal enum ExitWindows : uint
		{
			LogOff = 0x00,
			ShutDown = 0x01,
			Reboot = 0x02,
			Force = 0x04,
			PowerOff = 0x08,
			ForceIfHung = 0x10
		}

		internal enum ShutdownReason : uint
		{
			MajorApplication = 0x00040000,
			MajorHardware = 0x00010000,
			MajorLegacyApi = 0x00070000,
			MajorOperatingSystem = 0x00020000,
			MajorOther = 0x00000000,
			MajorPower = 0x00060000,
			MajorSoftware = 0x00030000,
			MajorSystem = 0x00050000,

			MinorBlueScreen = 0x0000000F,
			MinorCordUnplugged = 0x0000000b,
			MinorDisk = 0x00000007,
			MinorEnvironment = 0x0000000c,
			MinorHardwareDriver = 0x0000000d,
			MinorHotfix = 0x00000011,
			MinorHung = 0x00000005,
			MinorInstallation = 0x00000002,
			MinorMaintenance = 0x00000001,
			MinorMMC = 0x00000019,
			MinorNetworkConnectivity = 0x00000014,
			MinorNetworkCard = 0x00000009,
			MinorOther = 0x00000000,
			MinorOtherDriver = 0x0000000e,
			MinorPowerSupply = 0x0000000a,
			MinorProcessor = 0x00000008,
			MinorReconfig = 0x00000004,
			MinorSecurity = 0x00000013,
			MinorSecurityFix = 0x00000012,
			MinorSecurityFixUninstall = 0x00000018,
			MinorServicePack = 0x00000010,
			MinorServicePackUninstall = 0x00000016,
			MinorTermSrv = 0x00000020,
			MinorUnstable = 0x00000006,
			MinorUpgrade = 0x00000003,
			MinorWMI = 0x00000015,

			FlagUserDefined = 0x40000000,
			FlagPlanned = 0x80000000
		}

		[DllImport(userDll)]
		public static extern bool ExitWindowsEx(ExitWindows uFlags, ShutdownReason dwReason);
/*
		internal enum SECURITY_IMPERSONATION_LEVEL : int
		{
			SecurityAnonymous,
			SecurityIdentification,
			SecurityImpersonation,
			SecurityDelegation
		}

		[DllImport(regApiDll, SetLastError=true)]
		public extern static bool DuplicateToken(IntPtr ExistingTokenHandle, int
			SECURITY_IMPERSONATION_LEVEL, ref IntPtr DuplicateTokenHandle);

*/
		[DllImport(regApiDll, SetLastError=true)]
		public static extern bool LogonUser(
			string lpszUsername, 
			string lpszDomain, 
			string lpszPassword, 
			int dwLogonType, 
			int dwLogonProvider, 
			ref IntPtr phToken
			);
#endif
	//+4.2 provide subtle on-screen volume control
	[DllImport(regApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
	public static extern int RegSetValueExW( uint hkey, string valueName, int reserved, uint type, byte[] data, uint len );
	//-4.2 provide subtle on-screen volume control
	}
}
