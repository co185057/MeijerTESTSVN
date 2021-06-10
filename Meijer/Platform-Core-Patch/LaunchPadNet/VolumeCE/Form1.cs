using System;
using System.Reflection;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Data;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using System.Xml;


namespace VolumeCE
{

	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TrackBar trackBar1;
		private System.Windows.Forms.Button Ok;
		private System.Windows.Forms.Label Title;
		private System.Windows.Forms.Label Instructions;
		private System.Windows.Forms.Panel panel1;
		static private Image backGroundImage;
		private System.Windows.Forms.Button vol5;
		private System.Windows.Forms.Button vol4;
		private System.Windows.Forms.Button vol3;
		private System.Windows.Forms.Button vol2;
		private System.Windows.Forms.Button vol1;
		private System.Windows.Forms.Button vol0;

		ushort	initialVolume = 0;

		string	textKeyInstructions	=	"InstructionsText";
		string  textKeyOkButton		=	"OkButtonText";
		string  textKeyTitle		=	"TitleText";
		string  textKeyVol5			=	"Button5Text";
		string  textKeyVol4			=	"Button4Text";
		string  textKeyVol3			=	"Button3Text";
		string  textKeyVol2			=	"Button2Text";
		string  textKeyVol1			=	"Button1Text";
		string  textKeyVol0			=	"Button0Text";


		public Form1()
		{
			backGroundImage = ( Image ) new 
								Bitmap( Assembly.GetExecutingAssembly().GetManifestResourceStream("VolumeCE.Volume2.bmp")); 

			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//

			initialVolume = volLeft = GetVolumeRegistry();

			int tb = 3;

			if	( volLeft >= 0xFF )
			{
				tb = 5;
			}
			else if ( volLeft >= 0xCC )
			{
				tb = 4;
			}
			else if ( volLeft >= 0x99 )
			{
				tb = 3;
			}
			else if ( volLeft >= 0x66 )
			{
				tb = 2;
			}
			else if ( volLeft >= 0x33 )
			{
				tb = 1;
			}
			else 
			{
				tb = 0;
			}

			string filePath	= "\\scot\\config";
			string fileConfigLanguageName = "ConfigLanguage.xml";
			string fileLanguageMsgName	= "VolumeMsg0409.xml";

			fileLanguageMsgName = this.GetLanguageFileName( filePath, fileConfigLanguageName, fileLanguageMsgName );
			this.ReadXmlTextPrompts( filePath, fileLanguageMsgName );

			trackBar1.Value = ( int ) ( tb );
			trackBar1.Refresh();
		}

		static string fileNameSound = "\\scot\\sound\\Volume.wav";

		/// <summary>
		/// The main entry point for the application.
		/// </summary>

		static void Main() 
		{
			// If our sound file doesn't exist, write it out.
			// Our sound api can't play from our resource. 
			try
			{
				if	( !File.Exists( fileNameSound ) )
				{
					Stream str = Assembly.GetExecutingAssembly().GetManifestResourceStream("VolumeCE.Volume.wav");
					byte [] bytes = new byte[ str.Length ];
					str.Read( bytes, 0, ( int ) str.Length );
					BinaryWriter binWriter = 
						new BinaryWriter(File.Open( fileNameSound, FileMode.Create));
					binWriter.Write( bytes, 0, bytes.Length );
					binWriter.Close();
				}
			}
			catch
			{
				// Unable to verify or force the sound file,
				// use the fallback
				fileNameSound = "\\Windows\\default.wav";
			}

			Form1 form = new Form1();
			Application.Run( form );
		}

		private void Ok_Click(object sender, System.EventArgs e)
		{
			Application.Exit();
		}


		private void trackBar1_ValueChanged(object sender, System.EventArgs e)
		{
			double	d		= ( double ) trackBar1.Value;
			double	dMax	= ( double ) trackBar1.Maximum;
			d				= d / dMax;
			uint volume		= ( uint ) ( ( double ) uint.MaxValue * d );
			d				= ushort.MaxValue * d;
			volLeft			= ( ushort ) d;
			volRight		= volLeft;

			SetVolumeRegistry( volLeft );
			waveOutSetVolume( IntPtr.Zero, volume );
			
			StringBuilder sb = new StringBuilder( fileNameSound );
			try
			{
				PlaySound( sb, IntPtr.Zero, SND_ASYNC | SND_FILENAME );
			}
			catch
			{

			}
		}

		private void vol5_Click(object sender, System.EventArgs e)
		{
			trackBar1.Value = 5;		
		}

		private void vol4_Click(object sender, System.EventArgs e)
		{
			trackBar1.Value = 4;		
		}

		private void vol3_Click(object sender, System.EventArgs e)
		{
			trackBar1.Value = 3;		
		}

		private void vol2_Click(object sender, System.EventArgs e)
		{
			trackBar1.Value = 2;		
		}

		private void vol1_Click(object sender, System.EventArgs e)
		{
			trackBar1.Value = 1;		
		}

		private void vol0_Click(object sender, System.EventArgs e)
		{
			trackBar1.Value = 0;		
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
								do
								{
									if(xmlReader.Name == "Language") break;
								} while( xmlReader.MoveToNextAttribute() );
								
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
		private void ReadXmlTextPrompts( string filePath, string fileNameMsg )
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
							string name = xmlReader.Name;
													
							// Do we recognize the element name as one of our text keys? 
							if( name == "String" )
							{
								xmlReader.MoveToFirstAttribute();
								do
								{
									if( xmlReader.Name == "Name" ) break;
								} while( xmlReader.MoveToNextAttribute() );

								name = xmlReader.Value;
								xmlReader.MoveToElement();
								
								if( name == textKeyInstructions )
								{
									this.Instructions.Text = xmlReader.ReadInnerXml();
								}
								else if ( name == textKeyOkButton )
								{
									this.Ok.Text = xmlReader.ReadInnerXml();
								}
								else if ( name == textKeyTitle )
								{
									this.Title.Text = xmlReader.ReadInnerXml();
								}
								else if ( name == textKeyVol5 )
								{
									this.vol5.Text = xmlReader.ReadInnerXml();
								}
								else if ( name == textKeyVol4 )
								{
									this.vol4.Text = xmlReader.ReadInnerXml();
								}
								else if ( name == textKeyVol3 )
								{
									this.vol3.Text = xmlReader.ReadInnerXml();
								}
								else if ( name == textKeyVol2 )
								{
									this.vol2.Text = xmlReader.ReadInnerXml();
								}
								else if ( name == textKeyVol1 )
								{
									this.vol1.Text = xmlReader.ReadInnerXml();
								}
								else if ( name == textKeyVol0 )
								{
									this.vol0.Text = xmlReader.ReadInnerXml();
								}
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
		}




		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			IntPtr hWnd = FindWindow( "#NETCF_AGL_BASE_", "Volume Control" );

			base.Dispose( disposing );
		}

		protected override void OnPaint(PaintEventArgs e)
		{
			if	( backGroundImage != null )
				e.Graphics.DrawImage( backGroundImage, 0, 0 );
			else
				base.OnPaint( e );
		}


		protected override void OnPaintBackground( PaintEventArgs e )
		{
			if	( backGroundImage != null )
				e.Graphics.DrawImage( backGroundImage, 0, 0 );
			else
				base.OnPaintBackground( e );
		}



		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.trackBar1 = new System.Windows.Forms.TrackBar();
			this.Ok = new System.Windows.Forms.Button();
			this.Title = new System.Windows.Forms.Label();
			this.Instructions = new System.Windows.Forms.Label();
			this.panel1 = new System.Windows.Forms.Panel();
			this.vol5 = new System.Windows.Forms.Button();
			this.vol4 = new System.Windows.Forms.Button();
			this.vol3 = new System.Windows.Forms.Button();
			this.vol2 = new System.Windows.Forms.Button();
			this.vol1 = new System.Windows.Forms.Button();
			this.vol0 = new System.Windows.Forms.Button();
			// 
			// trackBar1
			// 
			this.trackBar1.LargeChange = 1;
			this.trackBar1.Location = new System.Drawing.Point(71, 82);
			this.trackBar1.Maximum = 5;
			this.trackBar1.Orientation = System.Windows.Forms.Orientation.Vertical;
			this.trackBar1.Size = new System.Drawing.Size(24, 159);
			this.trackBar1.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
			// 
			// Ok
			// 
			this.Ok.Location = new System.Drawing.Point(87, 262);
			this.Ok.Size = new System.Drawing.Size(66, 29);
			this.Ok.Text = "Ok";
			this.Ok.Click += new System.EventHandler(this.Ok_Click);
			// 
			// Title
			// 
			this.Title.Font = new System.Drawing.Font("Tahoma", 12F, System.Drawing.FontStyle.Bold);
			this.Title.Location = new System.Drawing.Point(0, 2);
			this.Title.Size = new System.Drawing.Size(232, 20);
			this.Title.Text = "Volume Control";
			// 
			// Instructions
			// 
			this.Instructions.Location = new System.Drawing.Point(0, 29);
			this.Instructions.Size = new System.Drawing.Size(238, 20);
			this.Instructions.Text = "Adjust volume using control";
			// 
			// panel1
			// 
			this.panel1.BackColor = System.Drawing.Color.Black;
			this.panel1.Location = new System.Drawing.Point(3, 254);
			this.panel1.Size = new System.Drawing.Size(235, 1);
			// 
			// vol5
			// 
			this.vol5.Location = new System.Drawing.Point(144, 60);
			this.vol5.Size = new System.Drawing.Size(41, 25);
			this.vol5.Text = "5";
			this.vol5.Click += new System.EventHandler(this.vol5_Click);
			// 
			// vol4
			// 
			this.vol4.Location = new System.Drawing.Point(144, 92);
			this.vol4.Size = new System.Drawing.Size(41, 25);
			this.vol4.Text = "4";
			this.vol4.Click += new System.EventHandler(this.vol4_Click);
			// 
			// vol3
			// 
			this.vol3.Location = new System.Drawing.Point(144, 124);
			this.vol3.Size = new System.Drawing.Size(41, 25);
			this.vol3.Text = "3";
			this.vol3.Click += new System.EventHandler(this.vol3_Click);
			// 
			// vol2
			// 
			this.vol2.Location = new System.Drawing.Point(144, 156);
			this.vol2.Size = new System.Drawing.Size(41, 25);
			this.vol2.Text = "2";
			this.vol2.Click += new System.EventHandler(this.vol2_Click);
			// 
			// vol1
			// 
			this.vol1.Location = new System.Drawing.Point(144, 188);
			this.vol1.Size = new System.Drawing.Size(41, 25);
			this.vol1.Text = "1";
			this.vol1.Click += new System.EventHandler(this.vol1_Click);
			// 
			// vol0
			// 
			this.vol0.Location = new System.Drawing.Point(144, 220);
			this.vol0.Size = new System.Drawing.Size(41, 25);
			this.vol0.Text = "0";
			this.vol0.Click += new System.EventHandler(this.vol0_Click);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			// 
			// Form1
			// 
			this.ClientSize = new System.Drawing.Size(240, 320);
			this.ControlBox = false;
			this.Controls.Add(this.vol0);
			this.Controls.Add(this.vol1);
			this.Controls.Add(this.vol2);
			this.Controls.Add(this.vol3);
			this.Controls.Add(this.vol4);
			this.Controls.Add(this.vol5);
			this.Controls.Add(this.panel1);
			this.Controls.Add(this.Instructions);
			this.Controls.Add(this.Title);
			this.Controls.Add(this.Ok);
			this.Controls.Add(this.trackBar1);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Text = "Volume Control";
			this.WindowState = System.Windows.Forms.FormWindowState.Maximized; //Fix TAR 414649
		}
		#endregion

		const uint SND_FILENAME	= 0x00020000;
		const uint SND_ASYNC	= 0x00000001;

		const string kernelApi = "coredll.dll";

		[ DllImport( kernelApi ) ]
		public extern static bool PlaySound( StringBuilder fileName, IntPtr hMod, uint Flags );

		[ DllImport( kernelApi ) ]
		public extern static bool waveOutSetVolume( IntPtr handle, uint Volume );
        
		void SetVolumeRegistry( ushort VolLeft )
		{
			uint volume = ( uint ) ( ( ( uint ) VolLeft ) | ( ( ( uint ) VolLeft ) << 16 ) );
			int result = RegCreateValueDWORD( HKCU, "ControlPanel\\Volume", "Volume", volume );
			uint volume2 = 0; 
			int result2 = RegGetDWORDValue( HKCU, "ControlPanel\\Volume", "Volume", ref volume2 );
		}

		ushort GetVolumeRegistry()
		{
			uint volume = 0;
			int  result = RegGetDWORDValue( HKCU, "ControlPanel\\Volume", "Volume", ref volume );
			return ( ( ushort ) ( volume & 0xFF ) );
		}

		/*
		#define SHFS_SHOWTASKBAR            0x0001
		#define SHFS_HIDETASKBAR            0x0002
		#define SHFS_SHOWSIPBUTTON          0x0004
		#define SHFS_HIDESIPBUTTON          0x0008
		#define SHFS_SHOWSTARTICON          0x0010
		#define SHFS_HIDESTARTICON          0x0020
		*/

		const uint	SHFS_HIDETASKBAR		= 0x002;
		const uint  SHFS_HIDESIPBUTTON		= 0x008;
		const uint	SHFS_HIDESTARTICON		= 0x020;
		const uint	SHFS_SHOWTASKBAR		= 0x001;
		const uint	SHFS_SHOWSIPBUTTON		= 0x004;
		const uint	SHFS_SHOWSTARTICON		= 0x010;

		// we use this api to hide the task bar / start menu 
		[ DllImport( "aygshell.dll" ) ]
		extern static bool SHFullScreen( IntPtr hWnd, uint settings );

		[ DllImport( userDll ) ]
		extern static IntPtr FindWindow( string classname, string title );
		
		#region REGISTRY
		const uint MB_ICONASTERISK = 0x040; 

		const string userDll = "coredll.dll";

		[ DllImport( userDll ) ]
		static extern bool MessageBeep( uint type );

		ushort volLeft = 0, volRight = 0;

		/// <summary>
		/// Create a DWORD value in the specified registry key
		/// </summary>
		/// <param name="keyName">Name of key</param>
		/// <param name="valueName">Name of value</param>
		/// <param name="dwordData">Value data</param>
		/// <returns>ERROR_SUCCESS if successful</returns>
		public static int RegCreateValueDWORD( UIntPtr hKeyRoot, string keyName, string valueName, uint dwordData)
		{
			UIntPtr hkey = new UIntPtr( 0 );
			try
			{
				int result = RegOpenKeyEx( hKeyRoot, keyName, 0, KeyAccess.None, ref hkey);
				if (ERROR_SUCCESS != result)
					return result;

				byte[] bytes = BitConverter.GetBytes(dwordData);
				return RegSetValueEx(hkey, valueName, 0, KeyType.Dword,
					bytes, (uint)bytes.Length);
			}
			finally
			{
				if (UIntPtr.Zero != hkey)
				{
					RegCloseKey(hkey);
				}
			}
		}

		/// <summary>
		/// Get the specified DWORD value registry entry.
		/// </summary>
		/// <param name="keyName">Key name</param>
		/// <param name="valueName">Value name</param>
		/// <param name="dwordResult">Value data</param>
		/// <returns>ERROR_SUCCESS if successful</returns>
		public static int RegGetDWORDValue( UIntPtr hKeyRoot, string keyName, string valueName, ref uint dwordResult)
		{
			UIntPtr hkey = UIntPtr.Zero;

			try
			{
				int result = RegOpenKeyEx( hKeyRoot, keyName, 0, KeyAccess.None, ref hkey);
				if (ERROR_SUCCESS != result)
					return result;

				byte[] bytes = null;
				uint length = 0;
				KeyType keyType = KeyType.None;
                    
				result = RegQueryValueEx(hkey, valueName, IntPtr.Zero, ref keyType,
					null, ref length);

				bytes = new byte[Marshal.SizeOf(typeof(uint))];
				length = (uint)bytes.Length;
				keyType = KeyType.None;
                    
				result = RegQueryValueEx(hkey, valueName, IntPtr.Zero, ref keyType,
					bytes, ref length);

				if (ERROR_SUCCESS != result)
					return result;

				dwordResult = BitConverter.ToUInt32(bytes, 0);

				return ERROR_SUCCESS;
			}
			finally
			{
				if (UIntPtr.Zero != hkey)
				{
					RegCloseKey(hkey);
				}
			}
		}

		/// <summary>
		/// Key value types
		/// </summary>
		public enum KeyType : uint
		{
			None = 0,
			String = 1,
			Dword = 4,
		}

		/// <summary>
		/// Key access types
		/// </summary>
		public enum KeyAccess : uint
		{
			None = 0x0000,
			QueryValue = 0x0001,
			SetValue   = 0x0002,
			CreateSubKey = 0x0004,
			EnumerateSubKeys = 0x0008,
			Notify = 0x0010,
			CreateLink = 0x0020
		}

		/// <summary>
		/// HKEY_CLASSES_ROOT
		/// </summary>
		public static UIntPtr HKCR = new UIntPtr(0x80000000);
		/// <summary>
		/// HKEY_CURRENT_USER
		/// </summary>
		public static UIntPtr HKCU = new UIntPtr(0x80000001);
		/// <summary>
		/// HKEY_LOCAL_MACHINE
		/// </summary>
		public static UIntPtr HKLM = new UIntPtr(0x80000002);

		/// <summary>
		/// HKEY_USERS
		/// </summary>
		public static UIntPtr HKU = new UIntPtr(0x80000003);

		/// <summary>
		/// Successful return value from Registry API
		/// </summary>
		public const int ERROR_SUCCESS = 0;

		public const string registryDll = "coredll.dll";

		/// <summary>
		/// This function creates the specified key. If the key already exists in
		/// the registry, the function opens it. A remote application interface
		/// (RAPI) version of this function exists, and it is called
		/// CeRegCreateKeyEx.
		/// </summary>
		/// <param name="hkey">[in] Handle to a currently open key or one of:
		/// HKCR, HKCU, HKLM.</param>
		/// <param name="lpSubKey">[in] Pointer to a null-terminated string specifying
		/// the name of a subkey that this function opens or creates. The subkey
		/// specified must be a subkey of the key identified by the hKey parameter.
		/// This subkey must not begin with the backslash character (‘\’). This
		/// parameter cannot be NULL. In Windows CE, the maximum length of a key
		/// name is 255 characters, not including the terminating NULL character.
		/// You can also only nest 16 levels of sub-keys in Windows CE.</param>
		/// <param name="Reserved">[in] Reserved; set to 0.</param>
		/// <param name="lpClass">[in] Pointer to a null-terminated string that
		/// specifies the class (object type) of this key. This parameter is ignored
		/// if the key already exists. In Windows CE, the maximum length of a class
		/// string is 255 characters, not including the terminating NULL
		/// character.</param>
		/// <param name="dwOptions">[in] Ignored; set to 0 to ensure compatibility
		/// with future versions of Windows CE.</param>
		/// <param name="samDesired">[in] Ignored; set to 0 to ensure compatibility
		/// with future versions of Windows CE.</param>
		/// <param name="lpSecurityAttributes">[in] Set to NULL. Windows CE
		/// automatically assigns the key a default security descriptor.</param>
		/// <param name="phkResult">[out] Pointer to a variable that receives a
		/// handle to the opened or created key. When you no longer need the
		/// returned handle, call the RegCloseKey function to close it. </param>
		/// <param name="lpdwDisposition">out] Pointer to a variable that receives
		/// one of the following disposition values: REG_CREATED_NEW_KEY or
		/// REG_OPENED_EXISTING_KEY</param>
		/// <returns>ERROR_SUCCESS indicates success. A nonzero error code defined
		/// in Winerror.h indicates failure. To get a generic description of the error,
		/// call FormatMessage with the FORMAT_MESSAGE_FROM_SYSTEM flag set. The
		/// message resource is optional; therefore, if you call FormatMessage it
		/// could fail.</returns>
		[DllImport(registryDll, SetLastError=true)]
		public static extern int RegCreateKeyEx
		(
			UIntPtr hkey,
			String lpSubKey,
			uint Reserved,
			StringBuilder lpClass,
			uint dwOptions,
			KeyAccess samDesired,              
			IntPtr lpSecurityAttributes,
			ref UIntPtr phkResult,
			ref uint lpdwDisposition
		);

		/// <summary>
		/// This function deletes a named subkey from the specified registry key. 
		/// A remote application interface (RAPI) version of this function exists,
		/// and it is called CeRegDeleteKey.
		/// </summary>
		/// <param name="hkey">[in] Handle to a currently open key or one of:
		/// HKCR, HKCU, HKLM.</param>
		/// <param name="subkeyName">[in] Pointer to a null-terminated string
		/// specifying the name of the key to delete. This parameter cannot
		/// be NULL.</param>
		/// <returns>ERROR_SUCCESS indicates success. A nonzero error code defined
		/// in Winerror.h indicates failure. To get a generic description of the
		/// error, call FormatMessage with the FORMAT_MESSAGE_FROM_SYSTEM flag set.
		/// The message resource is optional; therefore, if you call FormatMessage
		/// it could fail.</returns>
		[DllImport(registryDll, SetLastError=true)]
		public static extern int RegDeleteKey
		(
			UIntPtr hkey,
			string subkeyName
		);

		/// <summary>
		/// This function opens the specified key.
		/// </summary>
		/// <param name="hkey">[in] Handle to a currently open key or one of:
		/// HKCR, HKCU, HKLM.</param>
		/// <param name="lpSubKey">[in] Pointer to a null-terminated string
		/// containing the name of the subkey to open. If this parameter is NULL
		/// or a pointer to an empty string, the function will open a new handle
		/// to the key identified by the hKey parameter. In this case, the function
		/// will not close the handles previously opened.</param>
		/// <param name="ulOptions">[in] Reserved; set to 0.</param>
		/// <param name="samDesired">[in] Not supported; set to 0.</param>
		/// <param name="phkResult">[out] Pointer to a variable that receives
		/// a handle to the opened key. When you no longer need the returned
		/// handle, call the RegCloseKey function to close it. </param>
		/// <returns>ERROR_SUCCESS indicates success. A nonzero error code defined
		/// in Winerror.h indicates failure. To get a generic description of the
		/// error, call FormatMessage with the FORMAT_MESSAGE_FROM_SYSTEM flag
		/// set. The message resource is optional; therefore, if you call
		/// FormatMessage it could fail.</returns>
		[DllImport(registryDll, SetLastError=true)]
		public static extern int RegOpenKeyEx
		(
			UIntPtr hkey,
			String lpSubKey,
			uint ulOptions,
			KeyAccess samDesired,
			ref UIntPtr phkResult
		);

		/// <summary>
		/// This function retrieves the type and data for a specified value
		/// name associated with an open registry key.
		/// </summary>
		/// <param name="hkey">[in] Handle to a currently open key or one of:
		/// HKCR, HKCU, HKLM.</param>
		/// <param name="lpValueName">[in] Pointer to a string containing the
		/// name of the value to query. If this parameter is NULL or an empty
		/// string, the function retrieves the type and data for the key’s
		/// unnamed value. A registry key does not automatically have an unnamed
		/// or default value. Unnamed values can be of any type.</param>
		/// <param name="lpReserved">[in] Reserved; set to NULL.</param>
		/// <param name="lpType">[out] Pointer to a variable that receives the
		/// type of data associated with the specified value.</param>
		/// <param name="lpData">[out] Pointer to a buffer that receives the value’s
		/// data. This parameter can be NULL if the data is not required.</param>
		/// <param name="lpcbData">[in/out] Pointer to a variable that specifies the
		/// size, in bytes, of the buffer pointed to by the lpData parameter. When
		/// the function returns, this variable contains the size of the data copied
		/// to lpData. If the data has the REG_SZ, REG_MULTI_SZ or REG_EXPAND_SZ type,
		/// then lpcbData will also include the size of the terminating null character.
		/// The lpcbData parameter can be NULL only if lpData is NULL. 
		/// <returns>ERROR_SUCCESS indicates success. A nonzero error code defined
		/// in Winerror.h indicates failure. To get a generic description of the
		/// error, call FormatMessage with the FORMAT_MESSAGE_FROM_SYSTEM flag set.
		/// The message resource is optional; therefore, if you call FormatMessage
		/// it could fail.</returns>
		[DllImport(registryDll, SetLastError=true)]
		public static extern int RegQueryValueEx
		(
			UIntPtr hkey,
			String lpValueName,
			IntPtr lpReserved,
			ref KeyType lpType,
			byte[] lpData,
			ref uint lpcbData
		);

		/// <summary>
		/// This function stores data in the value field of an open registry key.
		/// It can also set additional value and type information for the
		/// specified key.
		/// </summary>
		/// <param name="hkey">[in] Handle to a currently open key or one of:
		/// HKCR, HKCU, HKLM.</param>
		/// <param name="lpValueName">[in] Pointer to a string containing the
		/// name of the value to set. If a value with this name is not already
		/// present in the key, the function adds it to the key. If this parameter
		/// is NULL or an empty string, the function sets the type and data for the
		/// key’s unnamed value. Registry keys do not have default values, but they
		/// can have one unnamed value, which can be of any type. The maximum length
		/// of a value name is 255, not including the terminating NULL
		/// character. </param>
		/// <param name="Reserved">[in] Reserved; must be zero.</param>
		/// <param name="dwType">[in] Specifies the type of information to be stored
		/// as the value’s data.</param>
		/// <param name="lpData">[in] Pointer to a buffer containing the data to
		/// be stored with the specified value name.</param>
		/// <param name="cbData">[in] Specifies the size, in bytes, of the
		/// information pointed to by the lpData parameter. If the data is of
		/// type REG_SZ, REG_EXPAND_SZ, or REG_MULTI_SZ, cbData must include the
		/// size of the terminating null character. The maximum size of data allowed
		/// in Windows CE is 4 KB.</param>
		/// <returns>ERROR_SUCCESS indicates success. A nonzero error code defined
		/// in Winerror.h indicates failure. To get a generic description of the
		/// error, call FormatMessage with the FORMAT_MESSAGE_FROM_SYSTEM flag set.
		/// The message resource is optional; therefore, if you call FormatMessage
		/// it could fail.</returns>
		[DllImport(registryDll, SetLastError=true)]
		public static extern int RegSetValueEx
		(
			UIntPtr hkey,
			String lpValueName,
			uint Reserved,
			KeyType dwType,
			byte[] lpData,
			uint cbData
		);

		/// <summary>
		/// This function removes a named value from the specified registry key.
		/// A remote application interface (RAPI) version of this function exists,
		/// and it is called CeRegDeleteValue. 
		/// </summary>
		/// <param name="hkey">[in] Handle to a currently open key or one of:
		/// HKCR, HKCU, HKLM.</param>
		/// <param name="valueName">[in] Pointer to a null-terminated string
		/// that names the value to remove. If this parameter is NULL or points
		/// to an empty string, the default value of the key is removed. A default
		/// value is create by calling RegSetValueEx with a NULL or empty string
		/// value name.</param>
		/// <returns>ERROR_SUCCESS indicates success. A nonzero error code defined
		/// in Winerror.h indicates failure. To get a generic description of the
		/// error, call FormatMessage with the FORMAT_MESSAGE_FROM_SYSTEM flag set.
		/// The message resource is optional; therefore, if you call FormatMessage
		/// it could fail.</returns>
		[DllImport(registryDll, SetLastError=true)]
		public static extern int RegDeleteValue
		(
			UIntPtr hkey,
			string valueName
		);

		/// <summary>
		/// This function releases the handle of the specified key. A remote
		/// application interface (RAPI) version of this function exists, and
		/// it is called CeRegCloseKey.
		/// </summary>
		/// <param name="hkey">[in] Handle to the open key to close.</param>
		/// <returns>ERROR_SUCCESS indicates success. A nonzero error code
		/// defined in Winerror.h indicates failure. To get a generic description
		/// of the error, call FormatMessage with the FORMAT_MESSAGE_FROM_SYSTEM
		/// flag set. The message resource is optional; therefore, if you call
		/// FormatMessage it could fail.</returns>
		[DllImport(registryDll, SetLastError=true)]
		public static extern int RegCloseKey
		(
			UIntPtr hkey
		);

		#endregion

	}
}



