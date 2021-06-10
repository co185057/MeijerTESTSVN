using System;
using System.Text;
using System.Collections;
using System.Runtime.InteropServices;
using RPSWNET;

namespace RapNet
{
	/// <summary>
	/// PInvoke definitions used by the application.
	/// </summary>
	public class PInvoke
	{
        public const uint HKEY_LOCAL_MACHINE    = 0x80000002;
        public const uint INFINITE              = 0xFFFFFFFF;
        public const uint KEY_READ              = 0x00020019;
        public const uint MAX_PATH              = 260;
        public const uint REG_DWORD             = 4;
        public const uint REG_SZ                = 1;        

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

#if (WINCE)
        public const string kernelApiDll = "coredll.dll";
        private const string regApiDll = "coredll.dll";
        private const string userDll = "coredll.dll";
        public const string winsockDll = "winsock.dll";
        private const string shellDll = "aygshell.dll";
#else
        public const string kernelApiDll = "kernel32.dll";
        private const string regApiDll = "advapi32.dll";
        private const string userDll = "user32.dll";
        public const string winsockDll = "ws2_32.dll";
#endif

        [DllImport(userDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern IntPtr FindWindow( string lpClassName, string lpWindowName );

        [DllImport(winsockDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern int gethostname( byte[] host, int len );

        [DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern void GlobalMemoryStatus( out MEMORYSTATUS lpBuffer );

        [DllImport(regApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern int RegCloseKey( uint hkey );

        [DllImport(regApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern int RegOpenKeyExW( uint hkey, string subKey, uint options, uint sam, ref uint phkResult );

        [DllImport(regApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern int RegQueryValueExW( uint hkey, string valueName, int reserved, ref uint type, byte[] data, ref uint len );

        [DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern bool SetSystemTime( ref SYSTEMTIME time );

        [DllImport(userDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern bool ShowWindow( IntPtr hWnd, int nCmdShow );
                        
        [DllImport(winsockDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern int WSACleanup();
                
#if (WINCE)
        [DllImport(shellDll, CharSet=CharSet.Unicode, SetLastError = false)]
        public static extern uint SHCloseApps(uint dwMemSought);
        
        public static uint GetCurrentProcess()
        {
            return 66;
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

		[DllImport(kernelApiDll)]
		public static extern bool CreateProcess(string lpApplicationName,
			string lpCommandLine, IntPtr lpProcessAttributes, IntPtr lpThreadAttributes,
			bool bInheritHandles, uint dwCreationFlags, IntPtr lpEnvironment,
			string lpCurrentDirectory, IntPtr lpStartupInfo,
			out PROCESS_INFORMATION lpProcessInformation);
#else
        [DllImport(kernelApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern uint GetCurrentProcess();        
#endif
	}

#if (WINCE)

    /// <summary>
    /// Process class for Compact Framework.
    /// </summary>
    public class Process
    {
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
            IntPtr handle = GetModuleHandle( null );
            string name = String.Empty;
            if( IntPtr.Zero != handle )
            {
                StringBuilder buffer = new StringBuilder( (int)PInvoke.MAX_PATH );
                if( GetModuleFileName( handle, buffer, (uint)buffer.Capacity ) > 0 )
                {
                    name = buffer.ToString();
                    int n = name.LastIndexOf( '\\' );
                    if( -1 != n )
                    {
                        name = name.Substring( n + 1 );
                    }
                }
            }            
            return new Process( handle, name, 0, 0 );
        }

        public static Process[] GetProcessesByName( string processName )
        {
            ArrayList processes = new ArrayList();
            IntPtr snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
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
                        Process proc = new Process( new IntPtr( (int)peCurrent.PID ), peCurrent.Name, (int)peCurrent.ThreadCount, (int)peCurrent.BaseAddress );
                        processes.Add( proc );
                    }
                    b = Process32Next(snapshot, peBytes);
                }                
                CloseToolhelp32Snapshot(snapshot);                
            }                
            return (Process[])processes.ToArray(typeof(Process));
        }

        public string ProcessName
        {
            get
            {
                return processName;
            }
        }
        
        private Process( IntPtr handle, string processName, int threadCount, int baseAddress )
        {
            this.handle = handle;
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
            public ulong PID
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
		private int id = 0;
		private ProcessStartInfo startInfo;
		private PInvoke.PROCESS_INFORMATION pi;
    }

#endif

}

