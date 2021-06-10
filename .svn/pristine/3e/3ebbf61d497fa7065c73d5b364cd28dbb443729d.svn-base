/// 
/// The Process class. The class System.Diagnostics.Process was
/// not available in Windows CE. Instead, this Process class will be used
/// for Windows CE.
/// 

using System;
//using System.Collections;
//using System.Drawing;
//using System.Threading;
//using PsxNet;using System;
using System.Text;
using System.Collections;
using System.Runtime.InteropServices;
using System.Threading;
using RPSWNET;

namespace LaunchPadNet
{
	/// <summary>
	/// The Process class. 
	/// </summary>
	public class Process
	{

		/// <summary>
		/// The class constructor.
		/// </summary>
		public Process()
		{
		}
        
		public const uint TH32CS_SNAPPROCESS    = 0x00000002;
        
		public IntPtr Handle
		{
			get
			{
				return handle;
			}
		}

		public static Process GetCurrentProcess()
		{
			int processId = GetCurrentProcessId();
			CmDataCapture.Capture( CmDataCapture.MaskApi, String.Format ("Process::GetCurrentProcess - GetCurrentProcessId returns [{0:X}]", processId ));
			Process process = Process.GetProcessById( processId );
			return process;
		}

		/// <summary>
		/// Creates an array of new Process components and associates them with the existing process resources that all share the specified process name.
		/// </summary>  
		public static Process[] GetProcessesByName( string processName )
		{
			ArrayList processes = new ArrayList();
			IntPtr snapshot = IntPtr.Zero; 
			try
			{
				snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
				if( (int)snapshot > 0 )
				{
					string process = processName.ToUpper();                
					PROCESSENTRY32 peCurrent;
					PROCESSENTRY32 pe32 = new PROCESSENTRY32();
					byte[] peBytes = pe32.ToByteArray();
					bool b = Process32First( snapshot, peBytes );
					while( b )
					{
						peCurrent = new PROCESSENTRY32( peBytes );
						if( peCurrent.Name.ToUpper().Equals( process ) )
						{
							IntPtr handle = OpenProcess( 0, false, peCurrent.PID );
							CmDataCapture.Capture( CmDataCapture.MaskError, String.Format ("Process::GetProcessesByName - OpenProcess returned [{0:X}]", handle ));
							Process proc = new Process( handle, ( int ) peCurrent.PID, peCurrent.Name, (int)peCurrent.ThreadCount, (int)peCurrent.BaseAddress );
							processes.Add( proc );
						}
						b = Process32Next( snapshot, peBytes );
					}                
				}                
			}
			finally
			{
				CloseToolhelp32Snapshot( snapshot );                
			}
			return (Process[])processes.ToArray( typeof( Process ) );
		}

		/// <summary>
		/// Gets the name of the process.
		/// </summary>
		public string ProcessName
		{
			get
			{
				return processName;
			}
		}
        
		private Process( IntPtr handle, int processId, string processName, int threadCount, int baseAddress )
		{
			this.handle = handle;
			this.id = processId;
			this.isExiting = false;  // SSCOB-795
			this.processName = processName;
			this.threadCount = threadCount;
			this.baseAddress = baseAddress;
		}

		private int baseAddress = 0;
		private IntPtr handle = IntPtr.Zero;
		private string processName = String.Empty;
		private int threadCount = 0;
        
		private class PROCESSENTRY32
		{
			public PROCESSENTRY32()
			{
			}            
			public PROCESSENTRY32( byte[] aData )
			{
				dwSize = GetUInt( aData, SizeOffset );
				cntUsage = GetUInt( aData, UsageOffset );
				th32ProcessID = GetUInt( aData, ProcessIDOffset );
				th32DefaultHeapID = GetUInt( aData, DefaultHeapIDOffset );
				th32ModuleID = GetUInt( aData, ModuleIDOffset );
				cntThreads = GetUInt( aData, ThreadsOffset );
				th32ParentProcessID = GetUInt( aData, ParentProcessIDOffset );
				pcPriClassBase = (int)GetUInt( aData, PriClassBaseOffset );
				dwFlags = GetUInt( aData, dwFlagsOffset );
				szExeFile = GetString( aData, ExeFileOffset, (int)PInvoke.MAX_PATH );
				th32MemoryBase = GetUInt( aData, MemoryBaseOffset );
				th32AccessKey = GetUInt( aData, AccessKeyOffset );
			}

			public string Name
			{
				get
				{
					return szExeFile.Substring( 0, szExeFile.IndexOf('\0') );
				}
			}
			public uint PID
			{
				get
				{
					return th32ProcessID;
				}
			}
			public ulong BaseAddress
			{
				get
				{
					return th32MemoryBase;
				}
			}
			public ulong ThreadCount
			{
				get
				{
					return cntThreads;
				}
			}

			public byte[] ToByteArray()
			{
				byte[] aData;
				aData = new byte[ Size ];
				SetUInt( aData, SizeOffset, Size );
				return aData;
			}

			private static uint GetUInt( byte[] aData, int Offset )
			{
				return BitConverter.ToUInt32( aData, Offset );
			}
			private static void SetUInt( byte[] aData, int Offset, int Value )
			{
				byte[] buint = BitConverter.GetBytes( Value );
				Buffer.BlockCopy( buint, 0, aData, Offset, buint.Length );
			}
			private static ushort GetUShort( byte[] aData, int Offset )
			{
				return BitConverter.ToUInt16( aData, Offset );
			}
			private static void SetUShort( byte[] aData, int Offset, int Value )
			{
				byte[] bushort = BitConverter.GetBytes( (short)Value );
				Buffer.BlockCopy( bushort, 0, aData, Offset, bushort.Length );
			}
			private static string GetString( byte[] aData, int Offset, int Length )
			{
				String sReturn = Encoding.Unicode.GetString( aData, Offset, Length );
				return sReturn;
			}
			private static void SetString( byte[] aData, int Offset, string Value )
			{
				byte[] arr = Encoding.ASCII.GetBytes( Value );
				Buffer.BlockCopy( arr, 0, aData, Offset, arr.Length );
			}
            
			private const int SizeOffset = 0;
			private const int UsageOffset = 4;
			private const int ProcessIDOffset=8;
			private const int DefaultHeapIDOffset = 12;
			private const int ModuleIDOffset = 16;
			private const int ThreadsOffset = 20;
			private const int ParentProcessIDOffset = 24;
			private const int PriClassBaseOffset = 28;
			private const int dwFlagsOffset = 32;
			private const int ExeFileOffset = 36;
			private const int MemoryBaseOffset = 556;
			private const int AccessKeyOffset = 560;
			private const int Size = 564;

			private uint dwSize;
			private uint cntUsage; 
			private uint th32ProcessID; 
			private uint th32DefaultHeapID; 
			private uint th32ModuleID; 
			private uint cntThreads; 
			private uint th32ParentProcessID; 
			private int pcPriClassBase; 
			private uint dwFlags; 
			private string szExeFile;
			private uint th32MemoryBase;
			private uint th32AccessKey;
		}
        
		private const string kernelApiDll = "coredll.dll";
		private const string toolhelpDll = "toolhelp.dll";

		[DllImport(toolhelpDll, CharSet=CharSet.Unicode, SetLastError=false)]
		static extern IntPtr CreateToolhelp32Snapshot( uint dwFlags, uint th32ProcessID );

		[DllImport(toolhelpDll, CharSet=CharSet.Unicode, SetLastError=false)]
		static extern bool CloseToolhelp32Snapshot( IntPtr hSnapshot );
        
		[DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
		static extern uint GetModuleFileName( IntPtr hModule, StringBuilder lpFilename, uint nSize );
        
		[DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
		static extern IntPtr GetModuleHandle( string lpModuleName );

		[DllImport(toolhelpDll, CharSet=CharSet.Unicode, SetLastError=false)]
		static extern bool Process32First( IntPtr hSnapshot, byte[] pe );

		[DllImport(toolhelpDll, CharSet=CharSet.Unicode, SetLastError=false)]
		static extern bool Process32Next( IntPtr hSnapshot, byte[] pe );

		[DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
		static extern IntPtr OpenProcess( int dwAccess, bool bInherit, uint processID ); 

		[DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
		static extern bool CloseHandle( IntPtr handle );

		[DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
		static extern int GetCurrentProcessId();
		
		/// <summary>
		/// Gets a value indicating whether the associated process has been terminated.
		/// </summary>
		public bool HasExited
		{
			get
			{
				bool bResult = false;
				try
				{
					IntPtr handleProc = OpenProcess( 0, false, ( uint ) id );
					if	( handleProc != IntPtr.Zero )
					{
						uint exitCode = PInvoke.STILL_ACTIVE;
						bool bResultEP = PInvoke.GetExitCodeProcess( handleProc, out exitCode );
						if	( bResultEP == true )
						{
							if	( exitCode != PInvoke.STILL_ACTIVE )
							{
								bResult = true; 
							}
						}
						else
						{
							CmDataCapture.Capture( CmDataCapture.MaskError, String.Format ("Process::HasExited - GetExitCodeProcess returned false" ));
						}
						CloseHandle( handleProc );
					}
					else
					{
						bResult = true;
					}
				}    
				catch(Exception ex)
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, String.Format ("Process::HasExited(get) - Exception: {0}", ex.Message ));
				}
				//InvalidOperationException There is no process associated with the object. 
				//Win32Exception The exit code for the process could not be retrieved. 

				// CmDataCapture.Capture( CmDataCapture.MaskError, String.Format ("Process::HasExited(get) - returns [{0}]", bResult ));

				if	( bResult == true ) 
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, String.Format ("Process::HasExited(get) = true [{0:X}][{1}]", this.Id, this.ProcessName ));
				}
				
				return bResult;
			}
		}


		/// <summary>
		/// Gets an indication if the associated process is in the process of terminating.
		/// Added for SSCOB-795 to allow wait for RPSW termination.
		/// </summary>
		public bool IsExiting
		{
			get
			{
				return isExiting;
			}
		}


		/// <summary>
		/// Gets the unique identifier for the associated process.
		/// </summary>
		public int Id
		{
			get
			{
				return id;
			}
		}

		/// <summary>
		/// Closes a process that has a user interface by sending a close message to its main window.
		/// </summary>
		public bool CloseMainWindow()
		{
			IntPtr hWnd = MainWindowHandle;

			if	( hWnd.ToInt32() == 0 ) 
			{
				return true;
			}

			UIntPtr wParam = UIntPtr.Zero;
			IntPtr lParam = IntPtr.Zero;
			uint message = PInvoke.WM_CLOSE;

			if ( PInvoke.PostMessage(hWnd, message, wParam, lParam) )
			{
				CmDataCapture.Capture( CmDataCapture.MaskExtensive, "Stop() GENTLE: ");
				isExiting = true; // SSCOB-795
				int elapsedMS = 0;
				while ( !HasExited && elapsedMS < 2000 )	// 2000 ms is 2 seconds
				{
					Thread.Sleep(100);
					elapsedMS += 100;
				}
			}

			return true;
		}

		/// <summary>
		/// Creates a new Process component, and associates it with the existing process resource that you specify.
		/// </summary>
		public static Process GetProcessById( int processID )
			// OVERLOAD public static Process GetProcessById(int, string);
		{			
			Process process = null; 
			IntPtr snapshot = IntPtr.Zero;
			try
			{
				snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
				if( (int)snapshot > 0 )
				{
					PROCESSENTRY32 peCurrent;
					PROCESSENTRY32 pe32 = new PROCESSENTRY32();
					byte[] peBytes = pe32.ToByteArray();
					bool b = Process32First( snapshot, peBytes );
					while( b )
					{
						peCurrent = new PROCESSENTRY32( peBytes );
						if( peCurrent.PID == processID )
						{
							IntPtr handle = OpenProcess( 0, false, peCurrent.PID );
							CmDataCapture.Capture( CmDataCapture.MaskError, String.Format ("Process::GetProcessById - OpenProcess returned [{0:X}]", handle ));
							process = new Process( handle, ( int ) peCurrent.PID, peCurrent.Name, (int)peCurrent.ThreadCount, (int)peCurrent.BaseAddress );
							break;
						}
						b = Process32Next( snapshot, peBytes );
					}                
				}
			}
			finally
			{
				CloseToolhelp32Snapshot( snapshot );                
			}
			return process;
		}

		/// <summary>
		/// Immediately stops the associated process.
		/// </summary>
		public void Kill()
		{
			try
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, String.Format ("Process::Kill() - PID [{0:X}][{1}]", this.Id, this.ProcessName ));
				isExiting = true; // SSCOB-795
				PInvoke.TerminateProcess( handle, 0 );
			}    
			catch(Exception ex)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, String.Format ("Process::Kill() - Exception: ", ex.Message ));
			}
			//Win32Exception The associated process could not be terminated.
			//SystemException	No process Id has been set, and a Handle from which the Id property can be determined does not exist.
			//				or	There is no process associated with this Process object.
			//					You are attempting to call Kill for a process that is running on a remote computer. The method is available only for processes running on the local computer.
			//InvalidOperationException The process has already exited.
		}

		/// <summary>
		/// Starts a process resource and associates it with a Process component.
		/// </summary>
		public bool Start()
		{
			bool res = false;
			try
			{
				res = PInvoke.CreateProcess(StartInfo.FileName, null, 
					System.IntPtr.Zero, System.IntPtr.Zero, false, 0, System.IntPtr.Zero, null,
					System.IntPtr.Zero, out pi);
				if	( res == true )
				{
					id		= pi.dwProcessId;
					handle	= pi.hProcess;
					threadCount = 1;
					string fileName = StartInfo.FileName;
					int idx = fileName.LastIndexOf( "\\" );
					if	( idx != -1 )
					{
						fileName = fileName.Substring( idx + 1 );
					}
					processName = fileName;
					this.StartInfo.ProcessInformationStruct = pi;
				}
			}    
			catch(Exception ex)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, String.Format ("Process::Start() - CreateProcess Failed [{1}] Exception: [{0}]", ex.Message, StartInfo.FileName ));
			}

			return (res != false);
		}
        
		/// <summary>
		/// Gets or sets the properties to pass to the Start method of the Process.
		/// </summary>
		public ProcessStartInfo StartInfo
		{
			get
			{
				try
				{
				}    
				catch(Exception ex)
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, String.Format ("Process::StartInfo(get) - Exception: {0}", ex.Message ));
				}
				//ArgumentException The value that specifies the StartInfo is a null reference.
				
				return startInfo;
			}
			
			set
			{
				try
				{
					startInfo = value;
				}    
				catch(Exception ex)
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, String.Format ("Process::StartInfo(set)- Exception: {0}", ex.Message ));
				}
				//ArgumentException The value that specifies the StartInfo is a null reference.
			}
		}

		/// <summary>
		/// Causes the Process component to wait for the associated process to enter an idle state.
		/// </summary>
		public bool WaitForInputIdle()
		{
			IntPtr hWnd = MainWindowHandle;

			if	( hWnd.ToInt32() == 0 ) 
			{
				return true;
			}

			CmDataCapture.Capture( CmDataCapture.MaskError, 
				String.Format( "[ Process::WaitForInputIdle] - waiting for Process [{0:X}][{1}]", 
				id, ProcessName ));
			
			PInvoke.MSG msg = new PInvoke.MSG();

			int i = 200; 
			while ( ( PInvoke.PeekMessage( ref msg, hWnd, 0, 0, 0 ) )
					&& ( i > 0 ) )
			{
				Thread.Sleep( 100 );
				i--;
			}

			return true;
		}

		/// <summary>
		/// Causes the Process component to wait for the associated process to exit.
		/// </summary>
		public bool WaitForExit()
		{
			CmDataCapture.Capture( CmDataCapture.MaskError, 
				String.Format( "[ Process::WaitForExit] - waiting for Process [{0:X}][{1}]", 
									id, ProcessName ));

			isExiting = true; // SSCOB-795
			while ( !HasExited )
			{
				Thread.Sleep( 200 );
			}

			return HasExited;
		}

		// Find the window that belongs to 
		// the process id of this process
		private IntPtr FindMainWindowHandle()
		{
			int iHandleCount = 2048; 
			IntPtr [] Handles = new IntPtr [ 2048 ];

			// Get all open Windows 
			// fixed ( IntPtr * pHandles = Handles )
			{
				try
				{
					PInvoke.GetWindowHandles( Handles, ref iHandleCount );
				}
				catch ( Exception e )
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, 
						String.Format( "<Exception> [ Process::FindMainWindowHandles - GetWindowHandles()] [{0}]", 
						e.Message ));
				}
			}

			// Find the window that belongs to 
			// the process id of this process
			int i = 0;
			for( i = 0; i < iHandleCount; i++ )
			{
				int processID = -1;

				IntPtr hWnd = Handles[ i ];
				PInvoke.GetWindowThreadProcessId( hWnd, out processID);
				if	( processID == id )
				{
					StringBuilder windowTitle = new StringBuilder( 256 );
					PInvoke.GetWindowText( hWnd, windowTitle, windowTitle.Capacity );
					CmDataCapture.Capture( CmDataCapture.MaskApi, 
						String.Format( "[ Process::FindMainWindowHandles - Found the window handle [{0:X}][{1}] for process id [{2:X}] [{3}]", 
										hWnd.ToInt32(), windowTitle.ToString(), id, ProcessName ));
					return hWnd;
				}
			}

			// Window not found 
			return new IntPtr(0);
		}

		/// <summary>
		/// Gets the window handle of the main window of the associated process.
		/// </summary>
		public IntPtr MainWindowHandle
		{
			get
			{
				if	( mainWindowHandle.ToInt32() == 0 )
				{
					mainWindowHandle = FindMainWindowHandle();
				}
				return mainWindowHandle;
			}
		}

		// Process ID
		private int id = 0;
		private bool isExiting = false;  // SSCOB-795
		private ProcessStartInfo startInfo;
		private IntPtr mainWindowHandle = new IntPtr(0);
		private PInvoke.PROCESS_INFORMATION pi;

	}
}