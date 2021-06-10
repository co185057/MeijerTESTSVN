using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices;
using System.IO;
using System.Threading;
using System.Net;
using System.Xml;
using System.Text.RegularExpressions;

namespace GetDiagFiles
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class GetDiagFiles : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label Title;
		private System.Windows.Forms.Label Info;
		private System.Windows.Forms.ProgressBar progressBar1;

		private System.Threading.Timer timer;
        private ArrayList memBuffList = new ArrayList() ;

		bool bProcessed = false;

		public GetDiagFiles()
		{
			bProcessed = false;
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			this.Show();
		
			timer = new System.Threading.Timer( new TimerCallback( ProcessDiags ), null , 3000, 5000 );
		}

        /// <summary>
        /// ProcessMemoryMapFiles:  This function is to read settings from Registry to
        ///     get buffer names, buffer sizes and extract each memory buffer to a file.
        ///     If DiagFile.xml file is not specified, extracting default memory buffers:
        ///     LaunchPadNet, RapNet, Psx_LaunchPadNet, and Psx_RapNet
        /// Params: xmlDiagFilePath - DiagFile.xml file path name
        /// </summary>
        private void ProcessMemoryMapFiles(string xmlDiagFilePath)
        {
            try 
            {
                // Init
                memBuffList.Clear() ;

                // If DiagFile.xml is not found, process these registries by default
                if (!File.Exists(xmlDiagFilePath))
                {
                    ProcessMemoryMapFile(regLaunchPadNetPath) ;    // LaunchPadNet
                    ProcessMemoryMapFile(regRapNetPath) ;          // RapNet
                    ProcessMemoryMapFile(regPsxLaunchPadNetPath) ; // Psx_LaunchPadNet
                    ProcessMemoryMapFile(regPsxRapNetPath) ;       // Psx_RapNet
                }
                else // process registries specified in DiagFile.xml file
                {
                    XmlDocument myXmlDocument = new XmlDocument();
                    myXmlDocument.Load(xmlDiagFilePath);

                    XmlNodeList nodeList = myXmlDocument.GetElementsByTagName( "Registry" );
                    foreach( XmlNode node in nodeList )
                    {
                        ProcessMemoryMapFile(node.InnerText) ;
                    }
                }

                // Extract default NCRDCapAppBuffer
                ExtractMemoryMapFile("NCRDCapAppBuffer", "\\scot\\bin\\HHRapMemoryMapFile.log", 100) ;

                // Clear
                memBuffList.Clear() ;
            }
            catch (Exception ex )
            {
                //MessageBox.Show( ex.Message );
            }
        }

        /// <summary>
        /// ProcessMemoryMapFile: This function is to read settings from Registry to
        ///      get buffer names, buffer sizes and extract each memory buffer to a file
        /// Params: registryPathName - a registry pathname 
        /// </summary>
        private void ProcessMemoryMapFile(string registryPathName)
        {
            if ((registryPathName == null) || (registryPathName.Length <= 0))
                return ;
            
            try 
            {
                // Read memory buffer's settings on each registry
                string strMemBufferPathName = GetRegistryKeyValue(registryPathName, "DcapFile") ;
                string strMemBufferControl  = GetRegistryKeyValue(registryPathName, "DcapControl") ;
                string strMemBufferMaxSize  = GetRegistryKeyValue(registryPathName, "DcapFileMax") ;
                string strMemBufferPrefix   = GetRegistryKeyValue(registryPathName, "DcapLinePrefix") ;

                if ((strMemBufferPathName != null) &&
                    (strMemBufferControl  != null) &&
                    (strMemBufferMaxSize  != null))
                {
                    int nDcapControl = ParseInteger(strMemBufferControl) ;
                    int nDcapFileMax = ParseInteger(strMemBufferMaxSize) ;

                    // 1:  Memory buffer; 4: Physical log file
                    if ((1 == nDcapControl) && (nDcapFileMax > 0))
                    {
                        // Parse for file name
                        FileInfo bufferInfo        = new FileInfo(strMemBufferPathName) ;
                        string strMemoryBufferName = bufferInfo.Name ;

                        if (strMemoryBufferName != null)
                        {
                            ExtractMemoryMapFile(strMemoryBufferName, strMemBufferPathName, nDcapFileMax) ;
                        }
                    }
                }
            }
            catch (Exception ex )
            {
                //MessageBox.Show( ex.Message );
            }
        }

        /// <Summary>
        /// ParseInteger: Helper function to parse int from input string
        /// </Summary>
        private int ParseInteger(string inputString)
        {
            if (inputString.Length <= 0)
            {
                return -1 ;
            }

            inputString.Trim() ;

            for(int i = 0 ; i < inputString.Length; i++)
            {
                if (!Char.IsDigit(inputString[i]))
                {
                    if (i > 0)
                    {
                        char[] trimChar = { '\0' };
                        string intString = inputString.Substring(0, i) ;
                        intString.TrimEnd(trimChar) ;

                        return (Convert.ToInt32(intString)) ;
                    }
                }
            }

            return (-1) ;
        }

        /// <Summary>
        /// GetRegistryKeyValue:  Get value of a key in registry
        /// In Params:  - registryPathName: pathname to a registry to open
        ///             - keyName: name of key to be queried
        /// Out Params: None
        /// Return:     key value upon success; otherwise, return null.
        /// </Summary>
        private string GetRegistryKeyValue(string registryPathName,
                                           string keyName) 
        {
            uint HKEY_LOCAL_MACHINE = 0x80000002;
            uint KEY_READ           = 0x00020019;
            uint REG_SZ             = 1;
            uint hkey               = 0;
            string keyValue         = null ;

            try
            {
                // Open registry 
                if (0 == RegOpenKeyExW( HKEY_LOCAL_MACHINE, registryPathName, 0, KEY_READ, ref hkey))
                {
                    char[] trimChar = { '\0' };
                    uint type = 0;
                    byte[] data = new byte[1024];

                    // Query Key Value 
                    uint len = (uint)data.Length;
                    if (0 == RegQueryValueExW( hkey, keyName, 0, ref type, data, ref len))
                    {
                        if (type == REG_SZ)
                        {
                            keyValue = System.Text.UnicodeEncoding.Unicode.GetString( data, 0, (int)len );
                            keyValue = keyValue.TrimEnd( trimChar );
                        }
                    }

                    // close registry
                    RegCloseKey( hkey );
                }
            }
            catch (Exception ex )
            {
                //MessageBox.Show( ex.Message );
            }

            return (keyValue) ;
        }

        /// <summary>
        /// ExtractMemoryMapFile: This function is to extract contain of memory map file
        ///                       and write to a physical file
        /// In Params: memoryMapFileName  - name of shared memory map file
        ///            outputFilePathName - output file resides on physical disk
        ///            bufferMaxSize      - maximum size of memory buffer in Kb
        /// </summary>
        private void ExtractMemoryMapFile(string memoryMapFileName,
                                          string outputFilePathName,
                                          int    bufferMaxSize)
        {
            try
            {
                // Check if memoryBuffer was already processed
                if (memBuffList.Contains(memoryMapFileName))
                {
                    return ;
                }
                
                // Open memory map file
                IntPtr mappedFileHandle = CreateFileMapping(
                    0xFFFFFFFF,                 // Handle
                    IntPtr.Zero,                // Security Attribute
                    (uint)FileAccess.ReadWrite, // Access Protection
                    0,                          // High-order 32 bits
                    0,                          // Low-order 32 bits
                    memoryMapFileName);         // Name of MMF

                if (mappedFileHandle == IntPtr.Zero)
                {
                    return ;
                }
                
                // Map file to a view
                IntPtr mappedViewHandle = MapViewOfFile(
                    mappedFileHandle,    // Handle to a file-mapping object
                    (uint)FILE_MAP_READ, // File access type
                    0,                   // High-order 32 bits
                    0,                   // Low-order 32 bits
                    0);                  // Number of bytes; zero for entire file
                    
                if (mappedViewHandle == IntPtr.Zero)
                {
                    return ;
                }

                // Delete old file
                if (File.Exists(outputFilePathName))
                {
                    File.Delete(outputFilePathName);
                }

                // Create output file
                StreamWriter fWriteHandle = new StreamWriter(outputFilePathName, false, System.Text.Encoding.Unicode) ;

                int iOffset        = 4 ;                      // Offset first four bytes
                int MAPPING_SIZE   = bufferMaxSize * 1024 ;   // Size of Memory Map File
                byte[] byteContent = new byte[MAPPING_SIZE] ; // array to hold memory map file content

                // Copy Memory Map File content 
                Marshal.Copy(mappedViewHandle, byteContent, 0, MAPPING_SIZE);
                
                // Decode Memory Map File Content
                string stringContent = System.Text.Encoding.Unicode.GetString(byteContent, iOffset, byteContent.Length - iOffset) ;
                
                // Filter garbage characters: first two characters reserved for Datacapture Viewer
                for (int i = 3; i < stringContent.Length; i++)
                {
                    fWriteHandle.Write(stringContent[i]) ;

                    // Remove first two characters on each line used by NCRDataCap
                    if (stringContent[i] == '\n') {i+=2 ;}
                }
    
                // Close file handle
                fWriteHandle.Close() ;

                // Unmap and close memory map file handles
                UnmapViewOfFile(mappedViewHandle);
                CloseHandle((uint)mappedFileHandle);

                // Add processed memorybuffer to the list
                memBuffList.Add(memoryMapFileName) ;
            } 
            catch 
            {
                // Silently exit
                return ;
            }
        }

		private void ProcessDiags( object o )
		{
			if	( bProcessed )
			{
				return; 
			}
			timer.Change( Timeout.Infinite, Timeout.Infinite );
			timer.Dispose();
			timer = null;

			bProcessed = true;
			progressBar1.Value = 10;
			progressBar1.Refresh();

			string binPath = "\\scot\\bin";
			string configPath = "\\scot\\config";
			string logPath = "\\scot\\logs";
			string tempPath="\\temp";

			if	( !Directory.Exists( logPath ) )
			{
				Directory.CreateDirectory( logPath );
			}
            /* TAR 412468 - GetDiagFiles on MC55 failed
             * The reason: \scot\logs\ contains log files
             * of some running processes that can not be deleted,
             * cegl.log.txt, for example.
             * We just create the folder if it does not exists
             */
            /*
			else
			{
				Directory.Delete( logPath, true );
				Directory.CreateDirectory( logPath );
			}
            */

            // Process memory buffers
            ProcessMemoryMapFiles(configPath + "\\DiagFile.xml") ;

			string [] files = Directory.GetFiles( binPath, "*.log" );
			foreach( string fileName in files )
			{ 
				File.Copy( fileName, fileName.Replace( binPath, logPath ), true );
			}

			files = Directory.GetFiles( tempPath, "*.log" );
			foreach ( string fileName in files )
			{
				File.Copy( fileName, fileName.Replace( tempPath,  logPath ), true );
			}


			progressBar1.Value = 20;
			progressBar1.Refresh();

			string hostName = Dns.GetHostName();
			DateTime dt = DateTime.Now;
			string zipFileName = String.Format( "{0}-{1:D2}{2:D2}{3:D2}-{4:D2}{5:D2}.zip", hostName, dt.Year % 100, dt.Month, dt.Day, dt.Hour, dt.Minute );
			string zipFileFull = "\\temp\\" + zipFileName;
			string cmdLine = String.Format( "--create {0} {1}\\*.log {2}\\*.bak {3}\\*.xml {4}\\*.dat",
											zipFileFull, logPath, binPath, configPath, configPath );
						
			if	( File.Exists( zipFileFull ) )
			{
				File.Delete( zipFileFull );
			}

			PROCESS_INFORMATION pi = new PROCESS_INFORMATION();
			CreateProcess( binPath + "\\ZipShell.exe", cmdLine,
				IntPtr.Zero, IntPtr.Zero, false, 0, IntPtr.Zero, null, IntPtr.Zero, out pi );

			Thread.Sleep( 500 );

			int tc = 0;
			int oldValue = progressBar1.Value;

			while ( (!File.Exists( zipFileFull ) ) && ( tc < 50 ) )
			{
				Thread.Sleep( 250 );
				tc++;
				progressBar1.Value = oldValue + ( tc * 80 ) / 50;
				progressBar1.Refresh();
			}

			progressBar1.Value = progressBar1.Maximum;
			progressBar1.Refresh();

			if	( File.Exists( zipFileFull ) )
			{
				MessageBox.Show( "Created file: " + zipFileName );
			}
			else
			{
				MessageBox.Show( "Unable to Create file: " + zipFileName );
			}
			Application.Exit();
		}


		private bool HasExited( IntPtr handleProc )
		{
			bool bResult = false;

			try
			{
				if	( handleProc != IntPtr.Zero )
				{
					uint exitCode = STILL_ACTIVE;
					bool bResultEP = GetExitCodeProcess( handleProc, out exitCode );
					if	( bResultEP == true )
					{
						if	( exitCode != STILL_ACTIVE )
						{
							bResult = true; 
						}
					}
				}
				else
				{
					bResult = true;
				}
			}	
			catch(Exception ex )
			{
				MessageBox.Show( ex.Message );
			}

			return bResult;
		}

		public const uint STILL_ACTIVE = 259;

		[DllImport( "coredll.dll", CharSet=CharSet.Unicode, SetLastError=false)]
		public static extern bool GetExitCodeProcess( IntPtr handle, out uint exitCode );

		[StructLayout( LayoutKind.Sequential )] public struct PROCESS_INFORMATION 
		{
			public IntPtr hProcess;
			public IntPtr hThread;		
			public Int32 dwProcessId;	
			public Int32 dwThreadId;	
		}

		[ DllImport( "coredll.dll" ) ]
		public static extern bool CreateProcess(string lpApplicationName,
			string lpCommandLine, IntPtr lpProcessAttributes, IntPtr lpThreadAttributes,
			bool bInheritHandles, uint dwCreationFlags, IntPtr lpEnvironment,
			string lpCurrentDirectory, IntPtr lpStartupInfo,
			out PROCESS_INFORMATION lpProcessInformation);

		/// <summary>
		/// Memory Map File
		/// </summary>
		private const int FILE_MAP_WRITE = 0x2;
		private const int FILE_MAP_READ  = 0x0004;
		
		[DllImport("coredll.dll",EntryPoint="OpenFileMapping",SetLastError=true, CharSet=CharSet.Unicode) ]
		private static extern IntPtr OpenFileMapping (int dwDesiredAccess, bool bInheritHandle,String lpName );

		[DllImport("coredll.dll",EntryPoint="CreateFileMapping",SetLastError=true,CharSet=CharSet.Unicode)]
		private static extern IntPtr CreateFileMapping(uint hFile, IntPtr lpAttributes, uint flProtect,uint dwMaximumSizeHigh, uint dwMaximumSizeLow, string lpName);
	
		[DllImport("coredll.dll",EntryPoint="MapViewOfFile",SetLastError=true,CharSet=CharSet.Unicode)]
		private static extern IntPtr MapViewOfFile(IntPtr hFileMappingObject,uint dwDesiredAccess, uint dwFileOffsetHigh,uint dwFileOffsetLow, uint dwNumberOfBytesToMap);
	
		[DllImport("coredll.dll",EntryPoint="UnmapViewOfFile",SetLastError=true,CharSet=CharSet.Unicode)]
		private static extern bool UnmapViewOfFile(IntPtr lpBaseAddress);
		
		[DllImport("coredll.dll",EntryPoint="CloseHandle",SetLastError=true,CharSet=CharSet.Unicode)]
		private static extern bool CloseHandle(uint hHandle);

		[DllImport("coredll.dll",EntryPoint="GetLastError",SetLastError=true,CharSet=CharSet.Unicode)]
		private static extern uint GetLastError();

		public enum FileAccess : int
		{
			ReadOnly  = 2,
			ReadWrite = 4
		}

        /// <summary>
        /// Registry
        /// </summary>
        [DllImport("coredll.dll", CharSet=CharSet.Unicode, SetLastError=false)]
        private static extern int RegOpenKeyExW( uint hkey, string subKey, uint options, uint sam, ref uint phkResult );

        [DllImport("coredll.dll", CharSet=CharSet.Unicode, SetLastError=false)]
        private static extern int RegQueryValueExW( uint hkey, string valueName, int reserved, ref uint type, byte[] data, ref uint len );

        [DllImport("coredll.dll", CharSet=CharSet.Unicode, SetLastError=false)]
        private static extern int RegCloseKey( uint hkey );

        /// <summary>
        /// LaunchPadNet
        /// </summary>
        private const string regLaunchPadNetPath = "SOFTWARE\\NCR\\LaunchPadNet\\DataCapture";

        /// <summary>
        /// RapNet
        /// </summary>
        private const string regRapNetPath = "SOFTWARE\\NCR\\RapNet\\DataCapture";

        /// <summary>
        /// Psx_LaunchPadNet
        /// </summary>
        private const string regPsxLaunchPadNetPath = "SOFTWARE\\NCR\\PSX\\DataCapture\\LaunchPadNet.exe";

        /// <summary>
        /// Psx_RapNet
        /// </summary>
        private const string regPsxRapNetPath = "SOFTWARE\\NCR\\PSX\\DataCapture\\RapNet.exe";

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if	( timer != null )
			{
				timer.Dispose();
			}
			base.Dispose( disposing );
		}
		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.Title = new System.Windows.Forms.Label();
			this.Info = new System.Windows.Forms.Label();
			this.progressBar1 = new System.Windows.Forms.ProgressBar();
			// 
			// Title
			// 
			this.Title.Font = new System.Drawing.Font("Times New Roman", 12F, System.Drawing.FontStyle.Bold);
			this.Title.Size = new System.Drawing.Size(237, 20);
			this.Title.Text = "Diagnostic Files";
			// 
			// Info
			// 
			this.Info.Location = new System.Drawing.Point(0, 31);
			this.Info.Size = new System.Drawing.Size(239, 20);
			this.Info.Text = "Please Wait";
			// 
			// progressBar1
			// 
			this.progressBar1.Location = new System.Drawing.Point(12, 138);
			this.progressBar1.Size = new System.Drawing.Size(210, 20);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			// 
			// GetDiagFiles
			// 
			this.ClientSize = new System.Drawing.Size(240, 320);
			this.ControlBox = false;
			this.Controls.Add(this.progressBar1);
			this.Controls.Add(this.Info);
			this.Controls.Add(this.Title);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Text = "Diagnostic Files";

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>

		static void Main() 
		{
			Application.Run(new GetDiagFiles());
		}
	}
}
