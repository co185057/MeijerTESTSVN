using System;
using System.Text;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Data;
using System.IO;
using System.Runtime.InteropServices;
using System.Net;
using System.Xml;

namespace TerminalInfoCE
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class TerminalInfo : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox textBox;
		private System.Windows.Forms.Label Title;
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.Button Ok;

		private delegate string DataFunction();
       
		// The types of data we will collect 
		enum DataType
		{
			ComputerName, 
			ProcessorType,
			ProcessorSpeed,
			Motherboard,
			SystemMemory,
			FreeMemory,
			Storage,
			FreeStorage, 
			BatteryStatus, 
			IPAddress, 
			EndList
		};

		string promptComputerName		= "Computer Name ";
		string promptProcessorType		= "Processor Type";
		string promptProcessorSpeed		= "Processor Speed  ";
		string promptMotherboard		= "Motherboard Type ";
		string promptSystemMemory		= "System Memory ";
		string promptFreeMemory			= "Free Memory   ";
		string promptStorage			= "System Storage";
		string promptFreeStorage		= "Free Storage  ";
		string promptBatteryStatus		= "Battery Status";
		string promptIPAddress			= "IP Address    ";

		string textKeyComputerName		= "ComputerNameText";
		string textKeyProcessorType		= "ProcessorTypeText";
		string textKeySystemMemory		= "SystemMemoryText";
		string textKeyFreeMemory		= "FreeMemoryText";
		string textKeySystemStorage		= "SystemStorageText";
		string textKeyFreeStorage		= "FreeStorageText";
		string textKeyBatteryStatus		= "BatteryText";
		string textKeyIPAddress			= "IPText";
		string textKeyOkButton			= "OkButtonText";
		string textKeyTitle				= "TerminalTitleText";
		string textKeyBatteryCharge		= "BatteryChargeText";
		string textKeyMotherBoard		= "MotherboardText";
		string textKeyProcessorSpeed	= "ProcessorSpeedText";

		static string xmlConfigLanguageFileName = "ConfigLanguage.xml";
		static string xmlMsgFileName  = "TerminalInfoMsg0409.xml";
		// The path of the data file
		static string filePath = "\\scot\\config";

		// The file name of the data. 
		static string fileName = "TerminalInfo.dat";

		string promptTextBatteryCharge = "Charging";

		public TerminalInfo()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			Hashtable dataPrompts = this.GetDataPrompts();
			Hashtable textKeys    = this.GetTextKeys();
			Hashtable dataFunctions = this.GetDataFunctions();

			xmlMsgFileName = GetLanguageFileName( filePath, xmlConfigLanguageFileName, xmlMsgFileName );

			dataPrompts = this.ReadXmlTextPrompts( dataPrompts, textKeys, filePath, xmlMsgFileName );

			ArrayList list = CollectInfo( dataPrompts, dataFunctions );
			StoreInfo( list, filePath, fileName );
			ShowInfo( filePath, fileName );
		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			base.Dispose( disposing );
		}
		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.textBox = new System.Windows.Forms.TextBox();
			this.Title = new System.Windows.Forms.Label();
			this.Ok = new System.Windows.Forms.Button();
			// 
			// textBox
			// 
			this.textBox.BackColor = System.Drawing.Color.White;
			this.textBox.Font = new System.Drawing.Font("Courier New", 11.25F, System.Drawing.FontStyle.Regular);
			this.textBox.ForeColor = System.Drawing.Color.Black;
			this.textBox.Location = new System.Drawing.Point(0, 31);
			this.textBox.Multiline = true;
			this.textBox.Size = new System.Drawing.Size(235, 218);
			this.textBox.Text = "";
			// 
			// Title
			// 
			this.Title.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Bold);
			this.Title.Size = new System.Drawing.Size(231, 16);
			this.Title.Text = "Terminal Information";
			// 
			// Ok
			// 
			this.Ok.Location = new System.Drawing.Point(83, 261);
			this.Ok.Size = new System.Drawing.Size(72, 29);
			this.Ok.Text = "OK";
			this.Ok.Click += new System.EventHandler(this.Ok_Click);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			// 
			// TerminalInfo
			// 
			this.ClientSize = new System.Drawing.Size(240, 320);
			this.ControlBox = false;
			this.Controls.Add(this.Ok);
			this.Controls.Add(this.Title);
			this.Controls.Add(this.textBox);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Text = "TerminalInfo";
			this.WindowState = System.Windows.Forms.FormWindowState.Maximized; //Fix TAR 414649

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>

		static void Main() 
		{
			TerminalInfo terminalInfo = new TerminalInfo();
			Application.Run( terminalInfo );
		}

		private void Ok_Click(object sender, System.EventArgs e)
		{
			Application.Exit();
		}

		private void GoBack_Click(object sender, System.EventArgs e)
		{
			Application.Exit();
		}

		// Construct a HashTable of our default prompts.  
		private Hashtable GetDataPrompts()
		{
			Hashtable prompts = new Hashtable();

			prompts[ DataType.ComputerName ]		= promptComputerName;
			prompts[ DataType.ProcessorType ]		= promptProcessorType;
			// prompts[ DataType.ProcessorSpeed ]		= promptProcessorSpeed;
			// prompts[ DataType.Motherboard ]			= promptMotherboard;
			prompts[ DataType.SystemMemory ]		= promptSystemMemory;
			prompts[ DataType.FreeMemory ]			= promptFreeMemory;
			prompts[ DataType.Storage ]				= promptStorage;
			prompts[ DataType.FreeStorage ]			= promptFreeStorage;
			prompts[ DataType.BatteryStatus ]		= promptBatteryStatus;
			prompts[ DataType.IPAddress ]			= promptIPAddress;

			return prompts;
		}

		private Hashtable GetTextKeys()
		{
			Hashtable textKeys = new Hashtable();
			
			textKeys[  DataType.ComputerName ]		= textKeyComputerName;
			textKeys[  DataType.ProcessorType ]		= textKeyProcessorType;
			// textKeys[  DataType.ProcessorSpeed ]	= textKeyProcessorSpeed;
			// textKeys[  DataType.Motherboard ]	= textKeyMotherBoard;
			textKeys[  DataType.SystemMemory ]		= textKeySystemMemory;
			textKeys[  DataType.FreeMemory ]		= textKeyFreeMemory;
			textKeys[  DataType.Storage ]			= textKeySystemStorage;
			textKeys[  DataType.FreeStorage ]		= textKeyFreeStorage;
			textKeys[  DataType.BatteryStatus ]		= textKeyBatteryStatus;
			textKeys[  DataType.IPAddress ]			= textKeyIPAddress;
			
			return textKeys;
		}

		// Get the file name of our language file 
		private string GetLanguageFileName( string filePath, string fileConfigName, string fileNameDefault )
		{
			XmlTextReader xmlReader = null;
			string fileName = null; 

			try
			{
				xmlReader = new XmlTextReader( filePath + "\\" + fileConfigName );                

				while( xmlReader.Read() && ( fileName == null ) )
				{
					switch( xmlReader.NodeType )
					{
						case XmlNodeType.Element :
							if	( xmlReader.Name == "Display" )
							{
								xmlReader.MoveToFirstAttribute();
								fileName = fileNameDefault.Replace( "0409", xmlReader.Value );
							}
							break;
						default :
							break;
					}
				}			
			}
			catch
			{
			}
			finally
			{
				if ( xmlReader != null )
				{
					xmlReader.Close();
				}

				if	( fileName == null ) 
				{
					fileName = fileNameDefault;
				}
			}


			return fileName;
		}


		// Read the xml of string values 
		// replace our internal values with the ones from the xml
		private Hashtable ReadXmlTextPrompts( Hashtable prompts, Hashtable textKeys, string filePath, string fileNameMsg )
		{
			XmlTextReader xmlReader = null;

			try
			{
				xmlReader = new XmlTextReader( filePath + "\\" + fileNameMsg );                

				while( xmlReader.Read() )
				{
					string elementName = xmlReader.Name;

					switch( xmlReader.NodeType )
					{
						case XmlNodeType.Element :
							// Do we recognize the element name as one of our text keys? 
							if	( textKeys.ContainsValue( elementName ) ) 
							{
								// we have to search our hash to find which one matches
								IDictionaryEnumerator e = textKeys.GetEnumerator(); 
								while ( e.MoveNext() )
								{
									DataType type		= ( DataType ) e.Key;
									string	 textKey	= ( string ) e.Value;
									if	( textKey == elementName )
									{
										string elementValue = xmlReader.ReadInnerXml();
										if	( fileNameMsg.IndexOf( "Msg0409.",0) > 0 ) 
										{
											prompts[ type ] = elementValue.PadRight(14, ' ');
										}
										else
										{
											prompts[ type ] = elementValue;
										}
									}
								}
							}
							else if ( xmlReader.Name == textKeyOkButton )
							{
								this.Ok.Text = xmlReader.ReadInnerXml();
							}
							else if ( xmlReader.Name == textKeyTitle )
							{
								this.Title.Text = xmlReader.ReadInnerXml();
							}
							else if ( xmlReader.Name == textKeyBatteryCharge )
							{
								promptTextBatteryCharge = xmlReader.ReadInnerXml();
							}

							break;
						default :
							break;
					}
				}			
			}
			catch
			{
			}
			finally
			{
				if ( xmlReader != null )
				{
					xmlReader.Close();
				}
			}

			return prompts;
		}

		/// <summary>
		/// Collect the data to show on the Terminal Info 
		/// </summary>
		private ArrayList  CollectInfo( Hashtable dataPrompts, Hashtable dataFunctions )
		{
			ArrayList list = new ArrayList();

			IDictionaryEnumerator myEnumerator = dataPrompts.GetEnumerator();
			while ( myEnumerator.MoveNext() )
			{
				DataType type = ( DataType ) myEnumerator.Key;
				string	 prompt = ( string ) myEnumerator.Value;

				try
				{
					DataFunction dataFunction = ( DataFunction ) dataFunctions[ type ];

					string data = dataFunction();

					StringBuilder val = new StringBuilder(( String.Format( "{0} : {1}", prompt, data ) ) );
					list.Add( val.ToString() );
				}
				catch( Exception e )
				{
					MessageBox.Show( e.Message );
				}
			}
		
			return list;
		}


		// Store the array of strings we've collected 
		// into the TerminalInfo text file 
		private void StoreInfo( ArrayList list, string filePath, string fileName )
		{
			string file = filePath + "\\" + fileName;
			if	( File.Exists( file ) )
			{
				File.Delete( file );
			}

			using(TextWriter textWriter = File.CreateText( file ) )
			{
				foreach( string line in list )
				{
					textWriter.WriteLine( line );
				}
			}
		}

		// Show the contents of the TerminalInfo.dat file 
		private void ShowInfo( string filePath, string fileName)
		{
			// Read in the TerminalInfo.dat 
			using(TextReader textReader = 
					  new StreamReader( filePath + "\\" + fileName ))
			{
				textBox.Text = textReader.ReadToEnd();
			}
		}


		private Hashtable GetDataFunctions()
		{
			Hashtable dataFunctions = new Hashtable();

			dataFunctions[ DataType.ComputerName ]		= new DataFunction( GetComputerName );
			dataFunctions[ DataType.ProcessorType ]		= new DataFunction( GetCPU_Type     );
			dataFunctions[ DataType.ProcessorSpeed ]	= new DataFunction( NotImplemented );
			dataFunctions[ DataType.Motherboard ]		= new DataFunction( GetMotherBoard );
			dataFunctions[ DataType.SystemMemory ]		= new DataFunction( GetTotalMemory );
			dataFunctions[ DataType.FreeMemory ]		= new DataFunction( GetFreeMemory  );
			dataFunctions[ DataType.Storage ]			= new DataFunction( GetDiskTotalSpace );
			dataFunctions[ DataType.FreeStorage ]		= new DataFunction( GetDiskFreeSpace );
			dataFunctions[ DataType.BatteryStatus ]		= new DataFunction( GetBatteryStatus );
			dataFunctions[ DataType.IPAddress ]			= new DataFunction( GetIpAddress );

			return dataFunctions;
		}

		string NotImplemented()
		{
			return "Not Implemented";
		}

		const string coreDll = "coredll.dll";

		#region Battery

		/// <summary>
		/// This structure contains information about the power status of the system.
		/// </summary>
		public class SYSTEM_POWER_STATUS_EX
		{
			/// <summary>
			/// AC power status.
			/// </summary>
			public byte ACLineStatus;
			/// <summary>
			/// Battery charge status.
			/// </summary>
			public byte BatteryFlag;
			/// <summary>
			/// Percentage of full battery charge remaining. This member can be a
			/// value in the range 0 to 100, or 255 if status is unknown. All other
			/// values are reserved.
			/// </summary>
			public byte BatteryLifePercent;
			/// <summary>
			/// Reserved; set to zero.
			/// </summary>
			public byte Reserved1;
			/// <summary>
			/// Number of seconds of battery life remaining, or 0xFFFFFFFF if
			/// remaining seconds are unknown.
			/// </summary>
			public uint BatteryLifeTime;
			/// <summary>
			/// Number of seconds of battery life when at full charge, or 0xFFFFFFFF
			/// if full lifetime is unknown.
			/// </summary>
			public uint BatteryFullLifeTime;
			/// <summary>
			/// Reserved; set to zero.
			/// </summary>
			public byte Reserved2;
			/// <summary>
			/// Backup battery charge status.
			/// </summary>
			public byte BackupBatteryFlag;
			/// <summary>
			/// Percentage of full backup battery charge remaining. Must be in the
			/// range 0 to 100, or BATTERY_PERCENTAGE_UNKNOWN.
			/// </summary>
			public byte BackupBatteryLifePercent;
			/// <summary>
			/// Reserved; set to zero
			/// </summary>
			public byte Reserved3;
			/// <summary>
			/// Number of seconds of backup battery life remaining, or
			/// BATTERY_LIFE_UNKNOWN if remaining seconds are unknown.
			/// </summary>
			public uint BackupBatteryLifeTime;
			/// <summary>
			/// Number of seconds of backup battery life when at full charge, or
			/// BATTERY_LIFE_UNKNOWN if full lifetime is unknown.
			/// </summary>
			public uint BackupBatteryFullLifeTime;
		}

		/// <summary>
		/// This function retrieves the power status of the system. The status indicates
		/// whether the system is running on AC or DC power, whether or not the batteries
		/// are currently charging, and the remaining life of main and backup batteries. 
		/// A remote application interface (RAPI) version of this function exists, and
		/// it is called CeGetSystemPowerStatusEx (RAPI).
		/// </summary>
		/// <param name="lpSystemPowerStatus">[out] Pointer to the SYSTEM_POWER_STATUS_EX
		/// structure receiving the power status information.</param>
		/// <param name="fUpdate">[in] If this Boolean is set to TRUE,
		/// GetSystemPowerStatusEx gets the latest information from the device driver,
		/// otherwise it retrieves cached information that may be out-of-date by
		/// several seconds.</param>
		/// <returns></returns>
		[DllImport(coreDll)]
		public static extern bool GetSystemPowerStatusEx(SYSTEM_POWER_STATUS_EX lpSystemPowerStatus, bool fUpdate);

		// get the battery status 
		private string GetBatteryStatus()
		{
			SYSTEM_POWER_STATUS_EX status = new SYSTEM_POWER_STATUS_EX();

			if (GetSystemPowerStatusEx(status, false) == true )
			{
				if	( status.BatteryLifePercent != 255 )
				{
					return ( String.Format("{0}%", status.BatteryLifePercent ) );
				}
				else 
				{
					if	( ( status.BatteryFlag & 8 ) == 8 )
					{
						return promptTextBatteryCharge;
					}
				}
			} 

			return "Unknown";
		}

		#endregion

		#region Memory
		/// <summary>
		/// This structure contains information about current memory availability.
		/// The GlobalMemoryStatus function uses this structure. 
		///		typedef struct _MEMORYSTATUS 
		///		{ 
		///			DWORD dwLength; 
		///			DWORD dwMemoryLoad; 
		///			DWORD dwTotalPhys; 
		///			DWORD dwAvailPhys; 
		///			DWORD dwTotalPageFile; 
		///			DWORD dwAvailPageFile; 
		///			DWORD dwTotalVirtual; 
		///			DWORD dwAvailVirtual; 
		///		} MEMORYSTATUS, *LPMEMORYSTATUS; 
		/// </summary>
		public class MEMORYSTATUS
		{
			/// <summary>
			/// Initialize an instance of MEMORYSTATUS by setting the
			/// size parameter.
			/// </summary>
			public MEMORYSTATUS()
			{
				dwLength = (uint)Marshal.SizeOf(this);
			}

			/// <summary>
			/// Specifies the size, in bytes, of the MEMORYSTATUS structure. Set
			/// this member to sizeof(MEMORYSTATUS) when passing it to the
			/// GlobalMemoryStatus function.
			/// </summary>
			public uint dwLength;
			/// <summary>
			/// Specifies a number between 0 and 100 that gives a general idea of
			/// current memory utilization, in which 0 indicates no memory use and
			/// 100 indicates full memory use.
			/// </summary>
			public uint dwMemoryLoad;
			/// <summary>
			/// Indicates the total number of bytes of physical memory. 
			/// </summary>
			public uint dwTotalPhys;
			/// <summary>
			/// Indicates the number of bytes of physical memory available.
			/// </summary>
			public uint dwAvailPhys;
			/// <summary>
			/// Indicates the total number of bytes that can be stored in the
			/// paging file. Note that this number does not represent the actual
			/// physical size of the paging file on disk.
			/// </summary>
			public uint dwTotalPageFile;
			/// <summary>
			/// Indicates the number of bytes available in the paging file.
			/// </summary>
			public uint dwAvailPageFile;
			/// <summary>
			/// Indicates the total number of bytes that can be described in the user
			/// mode portion of the virtual address space of the calling process.
			/// </summary>
			public uint dwTotalVirtual;
			/// <summary>
			/// Indicates the number of bytes of unreserved and uncommitted memory
			/// in the user mode portion of the virtual address space of the calling
			/// process.
			/// </summary>
			public uint dwAvailVirtual;
		}

		/// <summary>
		/// This function gets information on the physical and virtual memory of
		/// the system.
		/// </summary>
		/// <param name="lpBuffer">[out] Pointer to a MEMORYSTATUS structure. The
		/// GlobalMemoryStatus function stores information about current memory
		/// availability in this structure.</param>
		[DllImport(coreDll)]
		public static extern void GlobalMemoryStatus(MEMORYSTATUS lpBuffer);

		// Get the amount of free memory 
		private string GetFreeMemory()
		{
			MEMORYSTATUS memStatus = new MEMORYSTATUS();
			GlobalMemoryStatus(memStatus);

			return (String.Format("{0:F2} MB", ( double ) memStatus.dwAvailPhys / ( double ) (1024*1024)));
		}

		// Get the amount of total memory 
		private string GetTotalMemory()
		{
			MEMORYSTATUS memStatus = new MEMORYSTATUS();
			GlobalMemoryStatus(memStatus);

			return (String.Format("{0:F2} MB", ( double ) memStatus.dwTotalPhys / ( double ) (1024*1024)));
		}

		#endregion 

		#region storage 
		// Get the free storage card space 
		[DllImport(coreDll)]
		extern static bool GetDiskFreeSpaceEx( IntPtr dir, out ulong lpFreeBytesAvailableToCaller, 
											 out ulong lpTotalNumberOfBytes, 
											 out ulong lpTotalNumberOfFreeBytes );

		// Get the amount of storage space free 
		private string GetDiskFreeSpace()
		{
			ulong Free1 = 0, Total = 0, Free = 0;

			if	( GetDiskFreeSpaceEx( IntPtr.Zero, out Free1, out Total, out Free ) == true ) 
			{
				return String.Format( "{0:F2} MB", ( double ) Free / ( double ) ( 1024 * 1024 ) );
			}
			else 
			{
				return "Unavailable";
			}
		}

		// Get the amount of storage space total
		private string GetDiskTotalSpace()
		{
			ulong Free1 = 0, Total = 0, Free = 0;

			if	( GetDiskFreeSpaceEx( IntPtr.Zero, out Free1, out Total, out Free ) == true )
			{
				return String.Format( "{0:F2} MB", ( double ) Total / ( double ) ( 1024 * 1024 ) );
			}
			else
			{
				return "Unavailable";
			}
		}

		#endregion 

		#region network 

		const string winsockDll = "winsock.dll";

		[DllImport(winsockDll)]
		extern static int gethostname( byte [] data, int length );

		[DllImport(winsockDll)]
		extern static IntPtr gethostbyname( byte [] data );

		[DllImport(winsockDll)]
		extern static int WSAStartup( short wVersionRequested, byte [] data ); 

		[DllImport(winsockDll)]
		extern static int WSACleanup();

		private string GetIpAddress()
		{
			string hostName = Dns.GetHostName();
			IPAddress address = Dns.Resolve( hostName ).AddressList[0]; 
			// IPAddress address2 = Dns.Resolve( hostName ).AddressList[1]; 
			return address.ToString();	// + " " + address2.ToString();
		}

		/// <summary>
		/// HKEY_LOCAL_MACHINE
		/// </summary>
		private static UIntPtr HKLM = new UIntPtr(0x80000002);

		/// <summary>
		/// Key access types
		/// </summary>
		private enum KeyAccess : uint
		{
			None = 0x0000,
			QueryValue = 0x0001,
			SetValue   = 0x0002,
			CreateSubKey = 0x0004,
			EnumerateSubKeys = 0x0008,
			Notify = 0x0010,
			CreateLink = 0x0020
		}

		[DllImport(registryDll, SetLastError=true)]
		private static extern int RegOpenKeyEx
			(
			UIntPtr hkey,
			String lpSubKey,
			uint ulOptions,
			KeyAccess samDesired,
			ref UIntPtr phkResult
			);

		[DllImport(registryDll, SetLastError=true)]
		private static extern int RegQueryValueEx
			(
			UIntPtr hkey,
			String lpValueName,
			IntPtr lpReserved,
			ref uint lpType,
			byte[] lpData,
			ref uint lpcbData
			);

		private const string registryDll = "coredll.dll";

		// Get the computer name 
		private string GetComputerName()
		{
			string hostName = Dns.GetHostName();
			return hostName;
			/*	
			UIntPtr hKey = new UIntPtr(0);
			RegOpenKeyEx( HKLM, "Ident", 0, KeyAccess.QueryValue, ref hKey );
			byte [] computerName = new byte[ 256 ];
			uint length = 256;
			uint keyType = 0; 
			RegQueryValueEx( hKey, "Name", IntPtr.Zero, ref keyType, computerName, ref length );
			return Encoding.Unicode.GetString( computerName, 0, computerName.Length);
			*/
		}

		#endregion 

		#region CPU 
		public struct SYSTEM_INFO 
		{ 
			public ushort wProcessorArchitecture; 
			public ushort wReserved; 
			public uint dwPageSize; 
			public int lpMinimumApplicationAddress; 
			public int lpMaximumApplicationAddress; 
			public uint dwActiveProcessorMask; 
			public uint dwNumberOfProcessors; 
			public uint dwProcessorType; 
			public uint dwAllocationGranularity; 
			public ushort wProcessorLevel; 
			public ushort wProcessorRevision; 
		} 

		const int PROCESSOR_ARCHITECTURE_INTEL =0; 
		const int PROCESSOR_ARCHITECTURE_MIPS  =1; 
		const int PROCESSOR_ARCHITECTURE_ALPHA =2; 
		const int PROCESSOR_ARCHITECTURE_PPC   =3; 
		const int PROCESSOR_ARCHITECTURE_SHX   =4; 
		const int PROCESSOR_ARCHITECTURE_ARM   =5; 
		const int PROCESSOR_ARCHITECTURE_IA64  =6; 
		const int PROCESSOR_ARCHITECTURE_ALPHA64 =7; 
		const int PROCESSOR_ARCHITECTURE_UNKNOWN =0xFFFF; 

		[DllImport(coreDll)] 
		public static extern void GetSystemInfo( ref SYSTEM_INFO SystemInfo); 

		// Get Processor Type 
		private static string GetCPU_Type() 
		{ 
			SYSTEM_INFO si = new SYSTEM_INFO(); 
			GetSystemInfo(ref si); 
			switch(si.wProcessorArchitecture) 
			{ 
				case PROCESSOR_ARCHITECTURE_ARM: 
					return "ARM";
				case PROCESSOR_ARCHITECTURE_SHX: 
					return "SH3"; 
				case PROCESSOR_ARCHITECTURE_INTEL: 
					return "X86"; 
				case PROCESSOR_ARCHITECTURE_MIPS: 
					return "MIPS"; 
				default: 
					return "Unknown";
			} 
		} 


		class PROCESSOR_INFO 
		{ 
			byte [] data; 

			public PROCESSOR_INFO() 
			{ 
				data = new byte[574]; 
				wVersion = 1; 
			} 

			public byte[] DataBuffer { get { return data; } } 

			public uint DataSize { get { return ( uint ) data.Length; } }

			public ushort wVersion 
			{ 
				get { return (ushort)BitConverter.ToInt16(data, 0); } 
				set { BitConverter.GetBytes(value).CopyTo(data, 0); } 
			} 

			public string szProcessorCore 
			{ 
				get 
				{
					byte[] ret = new byte[80]; Buffer.BlockCopy(data, 2, ret, 0, 80); 
					return Encoding.Unicode.GetString( ret, 0, ret.Length ).TrimEnd('\0'); 
				} 
				set 
				{
					Buffer.BlockCopy(Encoding.Unicode.GetBytes( value ), 0, data, 2, 80); 
				} 
			} 

			public ushort wCoreRevision 
			{ 
				get { return (ushort)BitConverter.ToInt16(data, 82); } 
				set { BitConverter.GetBytes(value).CopyTo(data, 82); } 
			} 


			public string szProcessorName 
			{ 
				get 
				{
					byte[] ret = new byte[80]; Buffer.BlockCopy(data, 84, ret, 0, 80); 
					return Encoding.Unicode.GetString( ret, 0, ret.Length ).TrimEnd('\0'); 
				} 
				set 
				{
					Buffer.BlockCopy(Encoding.Unicode.GetBytes( value ), 0, data, 84, 80); 
				} 
			} 

			public ushort wProcessorRevision 
			{ 
				get { return (ushort)BitConverter.ToInt16(data, 164); } 
				set { BitConverter.GetBytes(value).CopyTo(data, 164); } 
			} 


			public string szCatalogNumber/*[100]*/ 
			{ 
				get 
				{
					byte[] ret = new byte[200]; 
					Buffer.BlockCopy(data, 166, ret, 0, 200);  
					return Encoding.Unicode.GetString( ret, 0, ret.Length ).TrimEnd('\0'); 
				} 
				set 
				{
					Buffer.BlockCopy(Encoding.Unicode.GetBytes( value ), 0, data, 166, 200); 
				} 
			} 

			public string szVendor /*[100];*/ 
			{ 
				get 
				{
					byte[] ret = new byte[200]; Buffer.BlockCopy(data, 366, ret, 0, 200);  
					return Encoding.Unicode.GetString(ret, 0,ret.Length).TrimEnd('\0'); 
				} 
				set 
				{
					Buffer.BlockCopy(Encoding.Unicode.GetBytes( value ), 0, data, 366, 200); 
				} 
			} 

			public uint dwInstructionSet 
			{ 
				get { return (uint)BitConverter.ToInt32(data, 566); } 
				set { BitConverter.GetBytes(value).CopyTo(data, 566); } 
			} 

			public uint dwClockSpeed 
			{ 
				get { return (uint)BitConverter.ToInt32(data, 570); } 
				set { BitConverter.GetBytes(value).CopyTo(data, 570); } 
			} 
		} 

		[DllImport(coreDll)] 
		private static extern bool KernelIoControl( uint dwIoControlCode, IntPtr lpInBuf, uint nInBufSize, byte[] buf, uint nOutBufSize, out uint lpBytesReturned); 

		const uint IOCTL_PROCESSOR_INFORMATION = 0x01010064;

		// Get the motherboard name 
		private string GetMotherBoard()
		{
			PROCESSOR_INFO pi = new PROCESSOR_INFO();
			uint iRet = 0;
            
			if	( KernelIoControl( IOCTL_PROCESSOR_INFORMATION, IntPtr.Zero, 0, pi.DataBuffer, pi.DataSize, out iRet ) == true )
			{
				return pi.szVendor;
			}

			return "Unknown";
		}

		// Get Processor Name 
		private string GetProcessorType()
		{
			PROCESSOR_INFO pi = new PROCESSOR_INFO();
			uint iRet = 0;

			if	( KernelIoControl( IOCTL_PROCESSOR_INFORMATION, IntPtr.Zero, 0, pi.DataBuffer, pi.DataSize, out iRet ) == true )
			{
				return pi.szVendor;
			}

			return "Unavailable";
		}
		#endregion 

		
	}
}
