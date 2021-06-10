/// <summary>
/// Class: EditLane
/// Description: This class contains EditLane.cs file
///     This class is to:
///         - Initialize Edit Lane Screen with lane's current info
///         - Or, initialize Add Lane Screen with default settings
///         - Provide GUIs for Delete, OK, Cancel operations
///         - Provide capabilities for user to change lane's configuration
/// Author: Nhuan Nguyen
/// Title:  NCR - Software Engineer
/// Email:  Nhuan.Nguyen@NCR.com
/// Phone:  (770) 813-3741
/// Date:   March 19, 2008
/// Version: 1.0
/// Revised: April 12, 2008
///		   : Edit for Desktop RAP
///		   : Maribeth Villanueva
/// </summary>
using System ;
using System.Drawing ;
using System.Collections ;
using System.Windows.Forms ;
using System.Data ;
using System.Runtime.InteropServices ;
using System.IO ;
using System.Threading ;
using System.Net ;
using System.Xml ;
using System.Text.RegularExpressions ;

namespace RapConfigure
{
    public class EditLane : System.Windows.Forms.Form
    {

#if ( WINCE )
        private int DISPLAY_WIDTH        = 240 ; // default - overrided by parentForm's dimension
        private int DISPLAY_HEIGHT       = 320 ; // default - overrided by parentForm's dimension
        private const int buttonWidth   = 75 ;
        private const int buttonHeight  = 30 ;
        private const int defaultHeight = 50 ;
        private const int inputHeight   = 20 ;
        private const int xOffset       = 5 ;
        private const int yOffset       = 8 ;
#else
		private int DISPLAY_WIDTH        = 1024 ; // default - overrided by parentForm's dimension
		private int DISPLAY_HEIGHT       = 768 ;  // default - overrided by parentForm's dimension
		private const int buttonWidth   = 180 ;
		private const int buttonHeight  = 60 ;
		private const int defaultHeight = 50 ;
		private const int inputHeight   = 30 ;
		private const int xOffset       = 10 ;
		private const int yOffset       = 10 ;
		private const int vKeyWidth		= 70 ;
		private const int vKeyHeight	= 70 ;

		//keyboard strings
		private string[] alphanumeric1 = {"1","2","3","4","5","6","7","8","9","0","-"};
		private string[] alphanumeric2 = {"Q","W","E","R","T","Y","U","I","O","P"};
		private string[] alphanumeric3 = {"A","S","D","F","G","H","J","K","L"};
		private string[] alphanumeric4 = {"Z","X","C","V","B","N","M","."};

#endif
        // private members
        private CONFIGTYPE configType = CONFIGTYPE.HOSTNAME ;
        private CONFIGTYPE origConfigType = CONFIGTYPE.HOSTNAME ;
#if ( WINCE )
		private System.Windows.Forms.MainMenu keyPadMenu ;
		private System.Windows.Forms.Button okButton ;
		private System.Windows.Forms.Label laneIDLabel ;
		private System.Windows.Forms.Button deleteButton ;
#endif
        private System.Windows.Forms.Label editLaneTitle ;
        private System.Windows.Forms.Label laneLabel ;
        private System.Windows.Forms.Button cancelButton ;
        private System.Windows.Forms.Button laneButton ;
#if (!WINCE )
		// for keyboard button
		private System.Windows.Forms.Button[] vKey1 = new Button[11];
		private System.Windows.Forms.Button[] vKey2 = new Button[10];
		private System.Windows.Forms.Button[] vKey3 = new Button[9];
		private System.Windows.Forms.Button[] vKey4 = new Button[8];
		private System.Windows.Forms.Button backspaceButton ;
		private System.Windows.Forms.Button enterButton ;
		private System.Windows.Forms.ContainerControl vKeyContainerControl ;
#endif	
        private System.Windows.Forms.TextBox[] octetArray = new TextBox[4] ;
        private System.Windows.Forms.TextBox hostNameTextBox ;
#if ( WINCE )
		private System.Windows.Forms.Label laneConfigTypeLabel ;

        private System.Windows.Forms.RadioButton ipAddressRadioButton ;
        private System.Windows.Forms.RadioButton ipOctetRadioButton ;
        private System.Windows.Forms.RadioButton hostNameRadioButton ;
        private System.Windows.Forms.RadioButton emptyRadioButton ;
#else
		private System.Windows.Forms.Button ipAddressButton ;
		private System.Windows.Forms.Button ipOctetButton ;
		private System.Windows.Forms.Button hostNameButton ;
		private System.Windows.Forms.Button emptyButton ;
		private System.Windows.Forms.Label instructionLabel ;
#endif
        private System.Windows.Forms.ContainerControl laneInfoContainerControl ;
        private System.Windows.Forms.ContainerControl configTypeContainerControl ;

        private RapConfigure parentForm ;
        private bool bNewLane = false ;
        private ConfigData configData = null ;
		
		//octetArray
		private int octetBox = 1 ;

        /// <summary>
        /// EditLane: Edit and/or Add Lane Screen
        /// </summary>
        /// <param name="parentForm"></param>
        /// <param name="laneButton"></param>
        /// <param name="configType"></param>
        /// <param name="bNewLane"></param>
        public EditLane( RapConfigure parentForm, Button laneButton, CONFIGTYPE configType, bool bNewLane )
        {
            // Get ConfigData
            this.configData = ConfigData.GetConfigData() ;

            // Init & override class's members
            this.laneButton = laneButton ;
            this.configType = configType ;
            this.origConfigType = configType ;
            this.parentForm = parentForm ;
            this.bNewLane = bNewLane ;
            this.DISPLAY_WIDTH = parentForm.DISPLAY_WIDTH ;   // Overrided from parent form
            this.DISPLAY_HEIGHT = parentForm.DISPLAY_HEIGHT ; // Overrided from parent form

            TRACE( MASK.API, "++EditLane()" ) ;
            TRACE( MASK.INFO, "bNewLane = " + bNewLane.ToString() ) ;
            TRACE( MASK.INFO, ( bNewLane ? "<New lane>" : "Edit Lane: " + laneButton.Text ) ) ;
            TRACE( MASK.INFO, "Configuration type: " + this.GetConfigTypeString( this.configType ) ) ;
            
            // Init
            InitializeComponent() ;

            // Set focus
			if ( this.configType == CONFIGTYPE.IPADDRESS )
			{
				this.octetArray[0].Focus() ;
				this.octetBox = 1;
			}
			else if ( this.configType == CONFIGTYPE.IPOCTET )
			{
				this.octetArray[3].Focus() ;
				this.octetBox = 4;
			}
			else if ( this.configType == CONFIGTYPE.HOSTNAME )
			{
				this.hostNameTextBox.Focus() ;
			}
			
#if ( !WINCE )
			this.BackColor = System.Drawing.Color.FromArgb(0,45,98);
			try
			{
				if( 800 == this.DISPLAY_WIDTH )
					this.BackgroundImage = System.Drawing.Image.FromFile("c:\\scot\\image\\8x6_Background2.png", true);
				else
					this.BackgroundImage = System.Drawing.Image.FromFile("c:\\scot\\image\\10x7_Background2.png", true);
			}
			catch (IOException e)
			{
				TRACE(MASK.ERROR, e.ToString());
			}
#endif

            this.ShowWindowTaskBar(true) ;

            this.Show() ;
        }

        /// <summary>
        /// Init components
        /// </summary>
        private void InitializeComponent()
        {
            TRACE( MASK.API, "++InitializeComponent()" ) ; 
            // Screen Title
            this.DrawTitle() ;

            // Lane Info:
            // this.DrawLaneLabel() ;

            // Lane Config Type
#if ( WINCE )
            this.DrawConfigType() ;
#else
			this.DrawDTRAPConfigType();
#endif

            // 
            this.DrawDynamicInput( this.configType, true ) ;

            // Button Group
            this.DrawButtonGroup() ;

#if ( WINCE )
			// Built-in keyboard
			this.keyPadMenu = new System.Windows.Forms.MainMenu() ;
			this.Menu = this.keyPadMenu ;
#endif
            TRACE( MASK.API, "--InitializeComponent()" ) ;
        }

        /// <summary>
        /// Cancel form and discard change( s )
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CancelButton_Click( object sender, EventArgs e )
        {
            TRACE( MASK.API, "++CancelButton_Click()" ) ;
            TRACE( MASK.API, "--CancelButton_Click()" ) ;
            this.Exit() ;
        }

        /// <summary>
        /// Delete a lane
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void DeleteButton_Click( object sender, EventArgs e )
        {
            TRACE( MASK.API, "++DeleteButton_Click()" ) ;
            string message = configData.STRConfirmDeleteText + " " + this.laneButton.Text +"?" ; 

            if ( DialogResult.Yes == MessageBox.Show( message, configData.STRConfirmDeleteTitle, 
                MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1 ) )
            {
                this.parentForm.DeleteLane( this.laneButton ) ;
                TRACE( MASK.API, "--DeleteButton_Click()" ) ;
                this.Exit() ;
            }
            else
            {
                TRACE( MASK.INFO, "User cancels deletion" ) ;
                TRACE( MASK.API, "--DeleteButton_Click()" ) ;
            }
        }

        /// <summary>
        /// Display Host Name
        /// </summary>
        private void DisplayHostName()
        {
            TRACE( MASK.EXTENSIVE, "++DisplayHostName()" ) ;
			// Input Box
#if ( WINCE )
			int x = xOffset ;
			int y = this.laneIDLabel.Location.Y + this.laneIDLabel.Height + ( int ) ( 0.5*yOffset ) ;
			int width = this.DISPLAY_WIDTH - 2*xOffset ;
#else
			int x = this.ipAddressButton.Location.X - 3 * xOffset;
			int y = 0 ;			
			int width = this.DISPLAY_WIDTH/3  ;
#endif

			int height = inputHeight ;
            this.hostNameTextBox = new System.Windows.Forms.TextBox() ;
            this.hostNameTextBox.Location = new System.Drawing.Point( x, y ) ;
            this.hostNameTextBox.Size = new System.Drawing.Size( width, height ) ;
#if ( !WINCE )
			this.hostNameTextBox.Font = new System.Drawing.Font( "Arial", 12F ) ;
#endif
            this.hostNameTextBox.Text = this.origConfigType == CONFIGTYPE.HOSTNAME ? this.laneButton.Text : "" ;
            this.hostNameTextBox.Focus() ;
            this.laneInfoContainerControl.Controls.Add( this.hostNameTextBox ) ;
            TRACE( MASK.EXTENSIVE, "--DisplayHostName()" ) ;
        }

        /// <summary>
        /// Display IP Address
        /// </summary>
        private void DisplayIPAddress()
        {
            TRACE( MASK.EXTENSIVE, "++DisplayIPAddress()" ) ;
		
            int height = inputHeight ;
#if ( WINCE ) 
			int x = xOffset ;
            int y = this.laneIDLabel.Location.Y + this.laneIDLabel.Height + ( int )( 0.5 * yOffset ) ;
            int width = ( int )( ( this.DISPLAY_WIDTH - 2*xOffset ) / 4 ) ;
#else
			int x = this.ipAddressButton.Location.X - 3 *xOffset;
			int y = 0 ;
			int width = (this.DISPLAY_WIDTH/3) / 4 ;
#endif
            bool bEditable = ( this.configType == CONFIGTYPE.IPADDRESS ) ? true : false ;
            string strIPAddress = "" + this.laneButton.Text ;
            string[] strOctetArray = new string[4] {"", "", "", ""} ;

            if ( ( strIPAddress != "" ) && ( strIPAddress.IndexOf( '.' ) != -1 ) )
            {
                char[] separator = {'.'} ;
                strOctetArray = strIPAddress.Split( separator ) ;

                if ( ( strOctetArray == null )  || ( strOctetArray.Length != 4 ) )
                {
                    strOctetArray = new string[4] {"", "", "", ""} ;
                }
            }

            if ( ( this.configType == CONFIGTYPE.IPOCTET ) && 
                 ( this.origConfigType != CONFIGTYPE.IPOCTET ) )
            {
                this.parentForm.strIPOctetArray.CopyTo( strOctetArray, 0 ) ;
            }

            for ( int i = 0 ; i < 4 ; i++ )
            {
                // Octet
                if ( ( !bEditable ) && ( i == 3 ) )
                {
                    DisplayIPOctet( i, strOctetArray[i], x, y, width, height, true ) ;
                }
                else
                {
                    DisplayIPOctet( i, strOctetArray[i], x, y, width, height, bEditable ) ;
                }

                x += width ;
            }
            TRACE( MASK.EXTENSIVE, "--DisplayIPAddress()" ) ;
        }

        /// <summary>
        /// Display IP Octet
        /// </summary>
        /// <param name="octetIndex"></param>
        /// <param name="preText"></param>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <param name="bEditable"></param>
        private void DisplayIPOctet( int octetIndex, string preText, int x, int y, int width, int height, bool bEditable )
        {
            TRACE( MASK.EXTENSIVE, "++DisplayIPOctet()" ) ;
            
            this.octetArray[octetIndex] = new System.Windows.Forms.TextBox() ;
            this.octetArray[octetIndex].Location = new System.Drawing.Point( x, y ) ;
            this.octetArray[octetIndex].Size = new System.Drawing.Size( width, height ) ;
#if ( !WINCE )
			this.octetArray[octetIndex].Font = new System.Drawing.Font( "Times New Roman", 12F ) ;
#endif
            this.octetArray[octetIndex].Text = preText ;
			this.octetArray[octetIndex].MaxLength = 3;
            this.octetArray[octetIndex].Enabled = bEditable ;

            if ( !bEditable )
            {
                this.octetArray[octetIndex].BackColor = System.Drawing.Color.LightGray ;
            }
            else
            {
				if ( octetIndex == 0 )
				{
					this.octetArray[octetIndex].KeyPress +=new KeyPressEventHandler( IPOctet1_KeyPress ) ;
					//this.octetArray[octetIndex].TextChanged += new EventHandler( IPOctet1_TextChanged );
					this.octetArray[octetIndex].Click +=new EventHandler(octetArray1Click);
				}
				else if ( octetIndex == 1 )
				{
					this.octetArray[octetIndex].KeyPress +=new KeyPressEventHandler( IPOctet2_KeyPress ) ;
					//this.octetArray[octetIndex].TextChanged += new EventHandler( IPOctet2_TextChanged );
					this.octetArray[octetIndex].Click +=new EventHandler(octetArray2Click);
				}
				else if ( octetIndex == 2 )
				{
					this.octetArray[octetIndex].KeyPress +=new KeyPressEventHandler( IPOctet3_KeyPress ) ;
					//this.octetArray[octetIndex].TextChanged += new EventHandler( IPOctet3_TextChanged );
					this.octetArray[octetIndex].Click +=new EventHandler(octetArray3Click);
				}
				else
				{
					this.octetArray[octetIndex].KeyPress +=new KeyPressEventHandler( IPOctet4_KeyPress ) ;
					//this.octetArray[octetIndex].TextChanged += new EventHandler( IPOctet4_TextChanged );
					this.octetArray[octetIndex].Click +=new EventHandler(octetArray4Click);
				}
            }
            this.laneInfoContainerControl.Controls.Add( this.octetArray[octetIndex] ) ;
            
            TRACE( MASK.EXTENSIVE, "--DisplayIPOctet()" ) ;
        }

        /// <summary>
        /// Dynamically display config's input boxes
        /// </summary>
        /// <param name="configType"></param>
        /// <param name="bShow"></param>
        private void DrawDynamicInput( CONFIGTYPE configType, bool bShow )
        {
            TRACE( MASK.EXTENSIVE, "++DrawDynamicInput()" ) ;
            if ( this.laneInfoContainerControl != null )
            {
                this.laneInfoContainerControl.Controls.Clear() ;
                this.Controls.Remove( this.laneInfoContainerControl ) ;
#if ( !WINCE )
				this.Controls.Remove( this.vKeyContainerControl );
#endif
            }

            // Set Config Type
            this.configType = configType ;

            int y = this.configTypeContainerControl.Location.Y + this.configTypeContainerControl.Height + ( int )( 1.5 * yOffset ) ;

            // Container Control
            this.laneInfoContainerControl = new System.Windows.Forms.ContainerControl() ;
            this.laneInfoContainerControl.Location = new System.Drawing.Point( 0, y ) ;

            this.laneInfoContainerControl.Size = new System.Drawing.Size( this.DISPLAY_WIDTH, defaultHeight+inputHeight) ;
#if ( !WINCE )
			
			this.laneInfoContainerControl.BackColor = System.Drawing.Color.FromArgb(0,45,98);
#endif
            this.Controls.Add( this.laneInfoContainerControl ) ;

#if ( WINCE )
			// Label
			this.laneIDLabel = new System.Windows.Forms.Label() ;
            int x = xOffset ;
            this.laneIDLabel.Location = new System.Drawing.Point( x, 0 ) ;
			this.laneIDLabel.Size = new System.Drawing.Size( this.DISPLAY_WIDTH, 2*inputHeight ) ;
			this.laneInfoContainerControl.Controls.Add( this.laneIDLabel ) ;
#endif      

			if ( ( this.configType == CONFIGTYPE.HOSTNAME ) ||
				( this.configType == CONFIGTYPE.EMPTY ) )
			{
#if ( WINCE )
                // On HHRAP, we do not display anything on EMPTY configuration type
                if ( this.configType == CONFIGTYPE.EMPTY )
                {
                    return;
                }
 
                this.laneIDLabel.Text = configData.STREnterHostNameText  ;
#else
				if(this.configType == CONFIGTYPE.HOSTNAME )
				{
					this.hostNameButton.BackColor = System.Drawing.Color.Gray;
				}
				this.instructionLabel.Text = configData.STREnterHostNameInstruction ;
#endif
				DisplayHostName() ;
#if (!WINCE)
				// Make sure to disable and clear contain of the echo box
				if ( this.configType == CONFIGTYPE.EMPTY )
				{

					this.emptyButton.BackColor = System.Drawing.Color.Gray ;
					this.instructionLabel.Text = configData.STREmptyText;
					this.hostNameTextBox.Enabled = false ;
					this.hostNameTextBox.Text = "" ;
				}
#endif
			}
			else
			{

                if ( this.configType == CONFIGTYPE.IPADDRESS )
                {
					
#if ( WINCE)
					this.laneIDLabel.Text = configData.STREnterIPAddressText  ;
#else
					this.ipAddressButton.BackColor = System.Drawing.Color.Gray;
					this.instructionLabel.Text = configData.STREnterIPAddressInstruction ;
#endif
                }
                else if ( this.configType == CONFIGTYPE.IPOCTET )
                {
					
#if ( WINCE)
					this.laneIDLabel.Text = configData.STREnterIPOctetText ;
#else
					this.ipOctetButton.BackColor = System.Drawing.Color.Gray;
					this.instructionLabel.Text = configData.STREnterIPOctetInstruction ;
#endif
                    
                }
             
                DisplayIPAddress() ;
            }
#if ( !WINCE )
			DisplayKeyboard( this.configType );
#endif
            if ( bShow )
            {
                this.Show() ;
            }

			TRACE( MASK.EXTENSIVE, "--DrawDynamicInput()" ) ;

        }

#if ( !WINCE )
		/// <summay>
		/// DisplayKeyboard()
		/// </summay>
		/// <param name="configType"/>
		private void DisplayKeyboard( CONFIGTYPE configType )
		{
			TRACE( MASK.EXTENSIVE, "++DisplayKeyboard()");
			
			this.configType = configType;

			drawKeyboard(this.configType);

			if( this.configType == CONFIGTYPE.HOSTNAME )
			{
				TRACE( MASK.INFO, "Virtual Keyboard. All keys enabled.");
				
				//-
				this.vKey1[alphanumeric1.Length -1].Enabled = true;
				this.vKey1[alphanumeric1.Length -1].BackColor = System.Drawing.Color.WhiteSmoke ;

				//qwertyuiop
				for(int i = 0 ; i < alphanumeric2.Length; i++)
				{
					this.vKey2[i].Enabled = true;
					this.vKey2[i].BackColor = System.Drawing.Color.WhiteSmoke ;
				}

				//asdfghjkl
				for(int i = 0 ; i < alphanumeric3.Length; i++)
				{
					this.vKey3[i].Enabled = true;
					this.vKey3[i].BackColor = System.Drawing.Color.WhiteSmoke ;
				}

				//zxcvbnm
				for(int i = 0 ; i < alphanumeric4.Length - 1; i++)
				{
					this.vKey4[i].Enabled = true;
					this.vKey4[i].BackColor = System.Drawing.Color.WhiteSmoke ;
				}
				//.
				this.vKey4[alphanumeric4.Length-1].Enabled = false;

			}
			else if( this.configType == CONFIGTYPE.IPADDRESS || this.configType == CONFIGTYPE.IPOCTET)
			{
				//dot (.)
				this.vKey4[alphanumeric4.Length-1].Enabled = true;
				this.vKey4[alphanumeric4.Length-1].BackColor = System.Drawing.Color.WhiteSmoke ;
			}
			else
			{
				//numbers
				for(int i = 0 ; i < alphanumeric1.Length - 1; i++)
				{
					this.vKey1[i].Enabled = false;
					this.vKey1[i].BackColor = System.Drawing.Color.Gray ;
				}
				//backspace
				this.backspaceButton.Enabled = false;
				this.backspaceButton.BackColor = System.Drawing.Color.Gray ;
				
			}
			this.Controls.Add(this.vKeyContainerControl);
			TRACE( MASK.EXTENSIVE, "--DisplayKeyboard()");
		}

		/// <summary>
		/// drawKeyboard- set position of the virtual keyboard
		/// </summary>
		/// <param name="configType"></param>
		private void drawKeyboard(CONFIGTYPE configType)
		{
			int x = 5 * xOffset;
			int y = this.laneInfoContainerControl.Location.Y + this.laneInfoContainerControl.Height;
		
			int vKeyNewWidth = vKeyWidth ;
			int vKeyNewHeight = vKeyHeight ;

			if( 800 == this.DISPLAY_WIDTH )
			{
				vKeyNewWidth = 50 ;
				vKeyNewHeight = 50 ;
			}
			int areaWidth = this.DISPLAY_WIDTH - 4*xOffset ;
			int areaHeight = 4 * vKeyNewHeight + 5*yOffset ;

			this.vKeyContainerControl = new System.Windows.Forms.ContainerControl();
			this.vKeyContainerControl.Location = new System.Drawing.Point(x,y);
			this.vKeyContainerControl.Size = new System.Drawing.Size(areaWidth, areaHeight);
			this.vKeyContainerControl.BackColor = System.Drawing.Color.FromArgb(0,45,98);
			y = 1;

			this.configType = configType;
			//numbers
			x = xOffset;
			for(int i = 0 ; i < alphanumeric1.Length; i++)
			{
				this.vKey1[i] = new System.Windows.Forms.Button();
				this.vKey1[i].Location = new System.Drawing.Point( x, y ) ;
				this.vKey1[i].Size = new System.Drawing.Size( vKeyNewWidth, vKeyNewHeight ) ;

				this.vKey1[i].Text = alphanumeric1[i];
				this.vKey1[i].BackColor = System.Drawing.Color.WhiteSmoke ;
				this.vKey1[i].FlatStyle = System.Windows.Forms.FlatStyle.Popup ;
				this.vKey1[i].Font = new System.Drawing.Font( "Arial", 18F, System.Drawing.FontStyle.Bold ) ;

				this.vKey1[i].Click += new EventHandler( vKeyButton_Click ) ;
				this.vKeyContainerControl.Controls.Add(this.vKey1[i]);
				x += vKeyNewWidth + xOffset ;
			}
			//- disable
			this.vKey1[alphanumeric1.Length -1].Enabled = false;
			this.vKey1[alphanumeric1.Length -1].BackColor = System.Drawing.Color.Gray ;

			//qwertyuiop
			y += vKeyNewHeight + yOffset ;
			x = 0 ;
			for(int i = 0 ; i < alphanumeric2.Length; i++)
			{
				this.vKey2[i] = new System.Windows.Forms.Button();
				this.vKey2[i].Location = new System.Drawing.Point( x, y ) ;
				this.vKey2[i].Size = new System.Drawing.Size( vKeyNewWidth, vKeyNewHeight ) ;

				this.vKey2[i].Text = alphanumeric2[i];
				this.vKey2[i].BackColor = System.Drawing.Color.Gray ;
				this.vKey2[i].Enabled = false;
				this.vKey2[i].FlatStyle = System.Windows.Forms.FlatStyle.Popup ;
				this.vKey2[i].Font = new System.Drawing.Font( "Arial", 18F, System.Drawing.FontStyle.Bold ) ;

				this.vKey2[i].Click += new EventHandler( vKeyButton_Click ) ;
				this.vKeyContainerControl.Controls.Add(this.vKey2[i]);
				x += vKeyNewWidth + xOffset ;
			}

			//backspace
			this.backspaceButton = new System.Windows.Forms.Button();
			this.backspaceButton.Location = new System.Drawing.Point(x,y);
			this.backspaceButton.Size = new System.Drawing.Size( 2*vKeyNewWidth+xOffset, vKeyNewHeight );

			this.backspaceButton.Text = configData.STRBackspace ;
			this.backspaceButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.backspaceButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup ;
			this.backspaceButton.Font = new System.Drawing.Font( "Arial", 12F, System.Drawing.FontStyle.Bold ) ;

			this.backspaceButton.Click += new EventHandler( vKeyButton_Click ) ;
			this.vKeyContainerControl.Controls.Add(this.backspaceButton);

			//asdfghjkl
			y += vKeyNewHeight + yOffset ;
			x = 4 * xOffset ;
			for(int i = 0 ; i < alphanumeric3.Length; i++)
			{
				this.vKey3[i] = new System.Windows.Forms.Button();
				this.vKey3[i].Location = new System.Drawing.Point( x, y ) ;
				this.vKey3[i].Size = new System.Drawing.Size( vKeyNewWidth, vKeyNewHeight ) ;

				this.vKey3[i].Text = alphanumeric3[i];
				this.vKey3[i].BackColor = System.Drawing.Color.Gray ;
				this.vKey3[i].Enabled = false;
				this.vKey3[i].FlatStyle = System.Windows.Forms.FlatStyle.Popup ;
				this.vKey3[i].Font = new System.Drawing.Font( "Arial", 18F, System.Drawing.FontStyle.Bold ) ;

				this.vKey3[i].Click += new EventHandler( vKeyButton_Click ) ;
				this.vKeyContainerControl.Controls.Add(this.vKey3[i]);
				x += vKeyNewWidth + xOffset ;
			}

			//enter
			this.enterButton = new System.Windows.Forms.Button();
			this.enterButton.Location = new System.Drawing.Point(this.backspaceButton.Location.X-2*xOffset,y);
			this.enterButton.Size = new System.Drawing.Size( 2 * vKeyNewWidth+3*xOffset, 2*vKeyNewHeight + yOffset);

			this.enterButton.Text = configData.STREnter ;
			this.enterButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.enterButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup ;
			this.enterButton.Font = new System.Drawing.Font( "Arial", 18F, System.Drawing.FontStyle.Bold ) ;

			this.enterButton.Click += new EventHandler( EnterButton_Click ) ;
			this.vKeyContainerControl.Controls.Add(this.enterButton);
			

			//zxcvbnm.
			y += vKeyNewHeight + yOffset ;
			x = 7*xOffset ;
			for(int i = 0 ; i < alphanumeric4.Length ; i++)
			{
				this.vKey4[i] = new System.Windows.Forms.Button();
				this.vKey4[i].Location = new System.Drawing.Point( x, y ) ;
				this.vKey4[i].Size = new System.Drawing.Size( vKeyNewWidth, vKeyNewHeight ) ;

				this.vKey4[i].Text = alphanumeric4[i];
				this.vKey4[i].BackColor = System.Drawing.Color.Gray ;
				this.vKey4[i].Enabled = false;
				this.vKey4[i].FlatStyle = System.Windows.Forms.FlatStyle.Popup ;
				this.vKey4[i].Font = new System.Drawing.Font( "Arial", 18F, System.Drawing.FontStyle.Bold ) ;

				this.vKey4[i].Click += new EventHandler( vKeyButton_Click ) ;
				this.vKeyContainerControl.Controls.Add(this.vKey4[i]);
				x += vKeyNewWidth + xOffset ;
			}
		}

		/// <summary>
		/// enter Button Click Event: 
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void EnterButton_Click( object sender, EventArgs e )
		{
			this.OkButton_Click	( sender, e );

		}
		/// <summary>
		/// key Button Click Event: 
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void vKeyButton_Click( object sender, EventArgs e )
		{
			TRACE(MASK.API, "++vKeyButton_Click()");
			string buttonText = ( ( Button )sender ).Text ;
			TRACE( MASK.INFO, "Virtual Keyboard: " + buttonText);
			try
			{
				//backspace
				if(string.Compare(buttonText, configData.STRBackspace) == 0 )
				{
					//hostname backspace
					if(this.configType == CONFIGTYPE.HOSTNAME)
					{
						ProcessBackspace(this.hostNameTextBox);
					}
					//ip octet
					else
					{
						TRACE(MASK.INFO, "octetBox: " + octetBox.ToString());
						ProcessBackspace(octetArray[octetBox-1]);
					}
				}
				// !backspace
				//.
				else if( string.Compare(buttonText, ".") == 0 
						&& (this.configType == CONFIGTYPE.IPADDRESS || this.configType== CONFIGTYPE.IPOCTET)
						)
				{
					//move to the next octetArray
					if(octetBox < 4)
					{
						this.octetArray[octetBox].Focus() ;
						this.octetBox += 1;
					}
				}
				//other keys
				else
				{					
					
					//hostname
					if(this.configType == CONFIGTYPE.HOSTNAME)
					{
						ProcessHostnameKeys(this.hostNameTextBox, buttonText);
					}
					//ip octet
					else
					{
						TRACE(MASK.INFO, "octetBox: " + octetBox.ToString());
						ProcessIPKeys(octetArray[octetBox-1], buttonText);

					}//else ip octet
						
				}//else other keys
			}catch(Exception err)
			{
				TRACE(MASK.ERROR, "ERROR: " + err.ToString());
			}
			finally
			{
				TRACE(MASK.API, "--vKeyButton_Click()");
			}
		}
		/// <summary>
		/// Handles backspace button 
		/// </summary>
		/// <param name="editTextBox"></param>
		private void ProcessBackspace(TextBox editTextBox)
		{

			// If TextBox is empty
			if (editTextBox.Text.Length == 0)
			{
				if(this.configType == CONFIGTYPE.IPADDRESS && octetBox != 1)
				{
					octetBox -= 1;
					editTextBox = octetArray[octetBox-1];
				}
		
				editTextBox.Focus() ;
				return ;
			}

			// Find the selected character(s)
			int startIndex = editTextBox.SelectionStart;
			int delLen      = editTextBox.SelectionLength ;

			// If no character(s) is selected, then just delete the character before the cursor
			if ((delLen == 0) && (startIndex >= 0)) 
			{
				delLen       = 1 ;
				if(startIndex != 0)
				{
					startIndex -= 1 ;
				}
				
			}
			editTextBox.Text = editTextBox.Text.Remove(startIndex, delLen) ;
			editTextBox.SelectionStart = startIndex ;
			editTextBox.Focus() ;
		}

		/// <summary>
		/// Handles other button (0-9, a-z)
		/// </summary>
		/// <param name="editTextBox"></param>
		private void ProcessIPKeys(TextBox editTextBox, string buttonText)
		{

			// Find the selected character(s)
			int startIndex = editTextBox.SelectionStart;
			int delLen  = editTextBox.SelectionLength ;
			int length = editTextBox.Text.Length;
			string textContent = editTextBox.Text;
			string substr = "";
			string substr1 = "";
			bool gotoNext = false;

			if (delLen > 0)
			{
				editTextBox.SelectedText = buttonText ;
				startIndex++;
			}
			else //no highlighted text, append/insert button text at cursor position
			{
				int y = 0; //value of the textbox

				TRACE(MASK.INFO, "Octet Length: " + length.ToString());

				if(length == 0) //empty
				{
					editTextBox.Text += buttonText;
				}
				else if(length == 3) //full
				{
					if(octetBox < 4) 
					{
						//go to the next array
						octetBox += 1;
						editTextBox = octetArray[octetBox - 1];
						editTextBox.Focus() ;
						length = editTextBox.Text.Length;

						if(length == 0) //next array is empty, add the buttonText
						{
							editTextBox.Text += buttonText ;
						}
						else
						{ //check if can still add
							y = int.Parse(editTextBox.Text + buttonText);
							//if the greater than 255
							if(y >= 0 && y < 256)
							{
								editTextBox.Text = editTextBox.Text + buttonText;
							}
						}
					}
					else //octetBox = 4
					{
						editTextBox.Focus();
						return; //do nothing	
					}

				}
				else // !empty !full
				{ //1-2 chars
					y = int.Parse(editTextBox.Text);
					switch(startIndex)
					{
						case 0:
						{
							y = int.Parse(buttonText + editTextBox.Text);
							
							//if the greater than 255
							if(y >= 0 && y < 256)
							{
								editTextBox.Text = buttonText + editTextBox.Text ;
							}
							else //cant insert anymore
							{
								gotoNext = true;
							}
							break;
						}
						case 1:
						{
							if(length == startIndex) //1 chars
							{
								y = int.Parse(editTextBox.Text + buttonText);
							
								//if the greater than 255
								if(y >= 0 && y < 256)
								{
									editTextBox.Text = editTextBox.Text + buttonText;
									if(y > 25)
									{
										gotoNext = true;
									}
								}
							}
							else //2 chars
							{
								substr = textContent.Substring(0,startIndex);
								substr1 =textContent.Substring(startIndex,length-startIndex);
								y = int.Parse(substr + buttonText + substr1) ; 
								//if the greater than 255
								if(y >= 0 && y < 256)
								{
									editTextBox.Text = substr + buttonText + substr1;
									gotoNext = true; //go to next array after
								}
								else
								{
									gotoNext = true;
								}
							}
							break;
						}
						case 2:
						{
							y = int.Parse(editTextBox.Text);
							
							//if y < 26 can still insert
							if( y < 26 )
							{
								if( 5 >= int.Parse(buttonText) )
								{
									editTextBox.Text = editTextBox.Text + buttonText;
								}
								else
								{
									gotoNext = true; //go to the next array
								}
							}
							break;
						}
						default:
						{
							editTextBox.Focus();
							return;//do nothing
						}
					} 
					
				}
				startIndex++; //add 1 to startIndex after adding, cursor will go position to the newly added char

				if(gotoNext) //go to next array
				{
					this.octetBox += 1;
					editTextBox = octetArray[octetBox - 1];
					startIndex = editTextBox.Text.Length;
				}
			}//no selected text

			editTextBox.SelectionStart = startIndex;
			editTextBox.Focus() ;

		}
		/// <summary>
		/// Handles other button (0-9, a-z)
		/// </summary>
		/// <param name="editTextBox"></param>
		private void ProcessHostnameKeys(TextBox editTextBox, string buttonText)
		{

			// Find the selected character(s)
			int startIndex = editTextBox.SelectionStart;
			int delLen  = editTextBox.SelectionLength ;
			int length = editTextBox.Text.Length;
			string textContent = editTextBox.Text;
			string substr = "";
			string substr1 = "";

			if (delLen > 0)
			{
				editTextBox.SelectedText = buttonText ;
				startIndex++;
			}
			else //no highlighted text, append/insert button text at cursor position
			{
				if(length == 0 || startIndex==length)
					{
						editTextBox.Text += buttonText;
					}
					else if(startIndex == 0)
					{
						editTextBox.Text = buttonText + editTextBox.Text;
					}
					else
					{
						if(startIndex>=1)
						{
							substr = textContent.Substring(0,startIndex);
							substr1 =buttonText+ textContent.Substring(startIndex,length-startIndex);
							this.hostNameTextBox.Text = substr + substr1 ; 
						}
					}
				startIndex++; //position cursor to the newly added character/number
				editTextBox.SelectionStart = startIndex;
				editTextBox.Focus() ;
			}//no selected text

		}

#endif
        /// <summary>
        /// Dispose()
        /// </summary>
        /// <param name="disposing"></param>
        protected override void Dispose( bool disposing )
        {
            base.Dispose( disposing ) ;
        }

        /// <summary>
        /// Draw Delete, Cancel, OK button group
        /// </summary>
        private void DrawButtonGroup()
        {
            TRACE( MASK.EXTENSIVE, "++DrawButtonGroup()" ) ;
			
			int buttonNewWidth = buttonWidth ;			
            int buttonNewHeight = buttonHeight; 

#if ( WINCE )            
			int x = ( int )( ( this.DISPLAY_WIDTH - 2*buttonNewWidth - xOffset )/2 ) ;
            int y = this.laneInfoContainerControl.Location.Y + this.laneInfoContainerControl.Height + 2*yOffset ;
#else		
			
			if( 800 == DISPLAY_WIDTH )
			{
				buttonNewWidth = 120 ;
				buttonNewHeight = 50 ; 
			}
			int x = this.DISPLAY_WIDTH - 3*xOffset - buttonNewWidth ;
			int y = this.DISPLAY_HEIGHT - ( yOffset + buttonNewHeight) ; 
#endif

#if ( WINCE )
            if ( !this.bNewLane )
            {
				// Delete
                x = ( int )( ( this.DISPLAY_WIDTH - 3*buttonWidth - 2*xOffset )/2 ) ;
                this.deleteButton = new System.Windows.Forms.Button() ;
                this.deleteButton.Location = new System.Drawing.Point( x, y ) ;
                this.deleteButton.Size = new System.Drawing.Size( buttonNewWidth, buttonHeight ) ;
                this.deleteButton.Text = configData.STRDeleteButtonText ;
                this.deleteButton.Click += new EventHandler( DeleteButton_Click ) ;
                this.Controls.Add( this.deleteButton ) ;
                x += buttonNewWidth + xOffset ;
                TRACE( MASK.EXTENSIVE, "Delete button: " + this.deleteButton.Text ) ;
            }

#endif
            // Cancel
            this.cancelButton = new System.Windows.Forms.Button() ;
            this.cancelButton.Location = new System.Drawing.Point( x, y ) ;
            this.cancelButton.Size = new System.Drawing.Size( buttonNewWidth, buttonNewHeight ) ;
            this.cancelButton.Text = configData.STRCancelButtonText ;
#if ( !WINCE )
            this.cancelButton.Size = new System.Drawing.Size( buttonNewWidth-xOffset, buttonNewHeight ) ;
			this.cancelButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.cancelButton.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold) ;
#endif
            this.cancelButton.Click += new EventHandler( CancelButton_Click ) ;
            TRACE( MASK.EXTENSIVE, "Cancel button: " + this.cancelButton.Text ) ;

#if ( WINCE )
            // OK
            x += buttonNewWidth + xOffset ;
            this.okButton = new System.Windows.Forms.Button() ;
            this.okButton.Location = new System.Drawing.Point( x, y ) ;
            this.okButton.Size = new System.Drawing.Size( buttonNewWidth, buttonHeight ) ;
            this.okButton.Text = configData.STROkButtonText ;
            this.okButton.Click += new EventHandler( OkButton_Click ) ;
            TRACE( MASK.EXTENSIVE, "OK button: " + this.okButton.Text ) ;
            this.Controls.Add( this.okButton ) ;
#endif
            this.Controls.Add( this.cancelButton ) ;
            //this.Controls.Add( this.deleteButton ) ;
            this.ControlBox = false ;
            this.MaximizeBox = false ;
            this.MinimizeBox = false ;

            TRACE( MASK.EXTENSIVE, "--DrawButtonGroup()" ) ;
        }
#if ( WINCE )

        /// <summary>
        /// Configuration type
        /// </summary>
        private void DrawConfigType()
        {
            TRACE( MASK.EXTENSIVE, "++DrawConfigType()" ) ;
            int x = 0 ;
            int y = this.editLaneTitle.Location.Y + this.editLaneTitle.Height + yOffset ;

			int width = this.DISPLAY_WIDTH - 2*xOffset ;
            int height = inputHeight ;

            this.configTypeContainerControl = new System.Windows.Forms.ContainerControl() ;
            this.configTypeContainerControl.Location = new System.Drawing.Point( x, y ) ;
            this.configTypeContainerControl.Size = new System.Drawing.Size( this.DISPLAY_WIDTH, 2*defaultHeight ) ;
            
            this.laneConfigTypeLabel = new System.Windows.Forms.Label() ;
            this.laneConfigTypeLabel.Location = new System.Drawing.Point( xOffset, 0 ) ;
            this.laneConfigTypeLabel.Size = new System.Drawing.Size( width, height ) ;
            this.laneConfigTypeLabel.Text = configData.STRConfigTypeText ;

            // Host Name
            x = 3*xOffset ;
            y = this.laneConfigTypeLabel.Location.Y + this.laneConfigTypeLabel.Height ;
            this.hostNameRadioButton = new System.Windows.Forms.RadioButton() ;
            this.hostNameRadioButton.Location = new System.Drawing.Point( x, y ) ;
            this.hostNameRadioButton.Size = new System.Drawing.Size( width, height ) ;
            this.hostNameRadioButton.Text = configData.STRHostNameText ;
            this.hostNameRadioButton.Checked = ( this.configType == CONFIGTYPE.HOSTNAME ) ;
            this.hostNameRadioButton.Click += new EventHandler( HostNameRadioButton_Click ) ;

            // IP Address
            y += inputHeight ;
            this.ipAddressRadioButton = new System.Windows.Forms.RadioButton() ;
            this.ipAddressRadioButton.Location = new System.Drawing.Point( x, y ) ;
            this.ipAddressRadioButton.Size = new System.Drawing.Size( width, height ) ;
            this.ipAddressRadioButton.Text = configData.STRIPAddressText ;
            this.ipAddressRadioButton.Checked = ( this.configType == CONFIGTYPE.IPADDRESS ) ;
            this.ipAddressRadioButton.Click +=new EventHandler( IPAddressRadioButton_Click ) ;

            // IP Octet
            y += inputHeight ;
            this.ipOctetRadioButton = new System.Windows.Forms.RadioButton() ;
            this.ipOctetRadioButton.Location = new System.Drawing.Point( x, y ) ;
            this.ipOctetRadioButton.Size = new System.Drawing.Size( width, height ) ;
            this.ipOctetRadioButton.Text = configData.STRIPOctetText ;
            this.ipOctetRadioButton.Checked = ( this.configType == CONFIGTYPE.IPOCTET ) ;
            this.ipOctetRadioButton.Click +=new EventHandler( IPOctetRadioButton_Click ) ;
            this.ipOctetRadioButton.Enabled = this.parentForm.bValidIPAddress ;

            // Empty
            y += inputHeight ;

            this.emptyRadioButton = new System.Windows.Forms.RadioButton() ;
            this.emptyRadioButton.Location = new System.Drawing.Point( x, y ) ;
            this.emptyRadioButton.Size = new System.Drawing.Size( width, height ) ;
            this.emptyRadioButton.Text = configData.STREmptyText ;
            this.emptyRadioButton.Checked = ( this.configType == CONFIGTYPE.EMPTY ) ;
            this.emptyRadioButton.Click +=new EventHandler( EmptyRadioButton_Click ) ;

            this.configTypeContainerControl.Controls.Add( this.laneConfigTypeLabel ) ;
            this.configTypeContainerControl.Controls.Add( this.hostNameRadioButton ) ;
            this.configTypeContainerControl.Controls.Add( this.ipAddressRadioButton ) ;
            this.configTypeContainerControl.Controls.Add( this.ipOctetRadioButton ) ;
            this.configTypeContainerControl.Controls.Add( this.emptyRadioButton ) ;

            this.Controls.Add( this.configTypeContainerControl ) ;
            TRACE( MASK.EXTENSIVE, "--DrawConfigType()" ) ;
        }

#else
		/// <summary>
		/// Configuration type
		/// </summary>
		private void DrawDTRAPConfigType()
		{
			TRACE( MASK.EXTENSIVE, "++DrawConfigType()" ) ;
			int x = 0 ;
			int y = this.editLaneTitle.Location.Y + this.editLaneTitle.Height + inputHeight + 2* yOffset;

			int width = ( int ) ( ( this.DISPLAY_WIDTH/2) /3)  - (3*xOffset) ;
			int height = buttonHeight ;
			if ( 800 == this.DISPLAY_WIDTH )
			{
				height = 50;
			}

			this.configTypeContainerControl = new System.Windows.Forms.ContainerControl() ;
			this.configTypeContainerControl.Location = new System.Drawing.Point( x, y ) ;
			this.configTypeContainerControl.Size = new System.Drawing.Size( this.DISPLAY_WIDTH, buttonHeight + 2*yOffset ) ;
			this.configTypeContainerControl.BackColor = System.Drawing.Color.FromArgb(0,45,98);

			// Host Name
			x = this.DISPLAY_WIDTH/4 - 3*xOffset ;
			y = yOffset ; 
			this.hostNameButton = new System.Windows.Forms.Button() ;
			this.hostNameButton.Location = new System.Drawing.Point( x, y ) ;
			this.hostNameButton.Size = new System.Drawing.Size( width, height ) ;
			this.hostNameButton.Text = configData.STRHostNameText ;
			this.hostNameButton.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Regular) ;
			this.hostNameButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.hostNameButton.Click += new EventHandler( HostNameButton_Click ) ;

			// IP Address
			x += width + xOffset ;
			this.ipAddressButton = new System.Windows.Forms.Button() ;
			this.ipAddressButton.Location = new System.Drawing.Point( x, y ) ;
			this.ipAddressButton.Size = new System.Drawing.Size( width, height ) ;
			this.ipAddressButton.Text = configData.STRIPAddressText ;
			this.ipAddressButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.ipAddressButton.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Regular) ;
			this.ipAddressButton.Click +=new EventHandler( IPAddressButton_Click ) ;

			// IP Octet
			x += width + xOffset ;
			this.ipOctetButton = new System.Windows.Forms.Button() ;
			this.ipOctetButton.Location = new System.Drawing.Point( x, y ) ;
			this.ipOctetButton.Size = new System.Drawing.Size( width, height ) ;
			this.ipOctetButton.Text = configData.STRIPOctetText ;
			this.ipOctetButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.ipOctetButton.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Regular) ;
			this.ipOctetButton.Click +=new EventHandler( IPOctetButton_Click ) ;
			this.ipOctetButton.Enabled = this.parentForm.bValidIPAddress ;

			// Empty
			x += width + xOffset ;

			this.emptyButton = new System.Windows.Forms.Button() ;
			this.emptyButton.Location = new System.Drawing.Point( x, y ) ;
			this.emptyButton.Size = new System.Drawing.Size( width, height ) ;
			this.emptyButton.Text = configData.STREmptyText ;
			this.emptyButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.emptyButton.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Regular) ;
			this.emptyButton.Click +=new EventHandler( EmptyButton_Click ) ;

			this.configTypeContainerControl.Controls.Add( this.hostNameButton ) ;
			this.configTypeContainerControl.Controls.Add( this.ipAddressButton ) ;
			this.configTypeContainerControl.Controls.Add( this.ipOctetButton ) ;
			this.configTypeContainerControl.Controls.Add( this.emptyButton ) ;

			this.Controls.Add( this.configTypeContainerControl ) ;
			TRACE( MASK.EXTENSIVE, "--DrawConfigType()" ) ;
		}

#endif

        /// <summary>
        /// Lane's label to display the edited lane's info
        /// </summary>
        private void DrawLaneLabel()
        {
            TRACE( MASK.EXTENSIVE, "++DrawLaneLabel()" ) ;
            int x = xOffset ;
            int y = this.editLaneTitle.Location.Y + this.editLaneTitle.Height + yOffset ;

            this.laneLabel = new System.Windows.Forms.Label() ;
            this.laneLabel.Location = new System.Drawing.Point( x, y ) ;
            this.laneLabel.Size = new System.Drawing.Size( this.DISPLAY_WIDTH-2*x, inputHeight ) ;
            this.laneLabel.Text = ( this.bNewLane ? "" : configData.STRLaneText + ": " + this.laneButton.Text ) ;
            this.laneLabel.TextAlign = System.Drawing.ContentAlignment.TopLeft ;
            this.Controls.Add( this.laneLabel ) ;
            TRACE( MASK.EXTENSIVE, "--DrawLaneLabel()" ) ;
        }

        /// <summary>
        /// DrawTitle: Edit Lane or Add Lane
        /// </summary>
        private void DrawTitle()
        {
            TRACE( MASK.EXTENSIVE, "++DrawTitle()" ) ;
            this.editLaneTitle = new System.Windows.Forms.Label() ;
#if ( WINCE )
            this.editLaneTitle.Font = new System.Drawing.Font( "Times New Roman", 10F, System.Drawing.FontStyle.Bold ) ;
            this.editLaneTitle.Location = new System.Drawing.Point( 0, yOffset ) ;
#else
			this.editLaneTitle.Font = new System.Drawing.Font( "Times New Roman", 22F, System.Drawing.FontStyle.Bold ) ;
			this.editLaneTitle.ForeColor = System.Drawing.Color.WhiteSmoke;
			this.editLaneTitle.Location = new System.Drawing.Point( xOffset, 2*yOffset ) ;
			this.editLaneTitle.BackColor = System.Drawing.Color.FromArgb(0,45,98);
#endif
            this.editLaneTitle.Size = new System.Drawing.Size( this.DISPLAY_WIDTH, inputHeight ) ;

            if (this.bNewLane)
            {
                this.editLaneTitle.Text = configData.STRAddLaneTitle ;
            }
            else
            {
                this.editLaneTitle.Text = configData.STREditLaneTitle + ": " + this.laneButton.Text ;
            }
			this.editLaneTitle.TextAlign = System.Drawing.ContentAlignment.TopLeft ;
			this.Controls.Add( this.editLaneTitle ) ;

#if ( !WINCE )
			this.instructionLabel = new System.Windows.Forms.Label() ;
			int y = this.editLaneTitle.Location.Y + this.editLaneTitle.Height + xOffset ;
			this.instructionLabel.Location = new System.Drawing.Point(xOffset, y ) ;
			this.instructionLabel.Size = new System.Drawing.Size( this.DISPLAY_WIDTH, inputHeight ) ;
			this.instructionLabel.Font = new System.Drawing.Font( "Arial", 12F, System.Drawing.FontStyle.Bold ) ;
			this.instructionLabel.BackColor = System.Drawing.Color.FromArgb(0,45,98);
			this.instructionLabel.ForeColor = System.Drawing.Color.WhiteSmoke;
			
			this.Controls.Add( this.instructionLabel ) ;
#endif

            TRACE( MASK.EXTENSIVE, "--DrawTitle()" ) ;
        }


        /// <summary>
        /// Exit(): Generate log before closing this form
        /// </summary>
        private void Exit()
        {
            TRACE( MASK.API, "--EditLane()" );
#if ( !WINCE )
			this.parentForm.Show();
#endif
            this.Close() ;

        }
        
        /// <summary>
        /// GetConfigTypeString: returns a string from configuration type
        /// </summary>
        /// <param name="configType"></param>
        /// <returns></returns>
        private string GetConfigTypeString( CONFIGTYPE configType )
        {
            TRACE( MASK.EXTENSIVE, "++GetConfigTypeString()" ) ;
            TRACE( MASK.EXTENSIVE, "Input: " + configType.ToString() ) ;

            string typeString = "Empty" ;
            if ( configType == CONFIGTYPE.HOSTNAME )       { typeString = "Host Name" ; }
            else if ( configType == CONFIGTYPE.IPADDRESS ) { typeString = "IP Address" ; }
            else if ( configType == CONFIGTYPE.IPOCTET )   { typeString = "IP Octet" ; }

            TRACE( MASK.EXTENSIVE, "Output: " + typeString ) ;
            TRACE( MASK.EXTENSIVE, "--GetConfigTypeString()" ) ;

            return ( typeString ) ;
        }

        /// <summary>
        /// GetLaneInfoFromInput: get user's input
        /// </summary>
        /// <returns></returns>
        private string GetLaneInfoFromInput()
        {
            TRACE( MASK.API, "++GetLaneInfoFromInput()" ) ;
            string laneInfo = "" ;
            // IP Address or IP Octet
            if ( ( this.configType == CONFIGTYPE.IPADDRESS ) ||
                ( this.configType == CONFIGTYPE.IPOCTET ) )
            {
                for ( int i = 0; i < 4; i++ )
                {
                    if ( ( this.octetArray[i].Text == "" ) || 
                        ( System.Int32.Parse( this.octetArray[i].Text ) < 0 ) ||
                        ( System.Int32.Parse( this.octetArray[i].Text ) > 255 ) )
                    {
                        laneInfo = "" ;
                        break ;
                    }
                    else
                    {
                        laneInfo += this.octetArray[i].Text ;
                        if ( i < 3 ) laneInfo += "." ;
                    }
                }
            }
            else // Host Name or Empty Lane
            {
                if ( this.configType == CONFIGTYPE.HOSTNAME )
                    laneInfo = this.hostNameTextBox.Text ;
                else
                    laneInfo = "" ;
            }

            if ( laneInfo != null ) { laneInfo = laneInfo.Trim() ; }

            TRACE( MASK.INFO, "Configurtion type: " + this.GetConfigTypeString( this.configType ) ) ;
            TRACE( MASK.INFO, "Lane's info: " + laneInfo ) ;
            TRACE( MASK.API, "--GetLaneInfoFromInput()" ) ;

            return ( laneInfo ) ;
        }

#if ( WINCE )
        /// <summary>
        /// Event handler for Host Name radio button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void HostNameRadioButton_Click( object sender, EventArgs e )
        {
            TRACE( MASK.API, "++HostNameRadioButton_Click()" ) ;
            DrawDynamicInput( CONFIGTYPE.HOSTNAME, true ) ;
            this.hostNameTextBox.Focus() ;
            TRACE( MASK.API, "--HostNameRadioButton_Click()" ) ;
        }

        /// <summary>
        /// Event handler for IP Address
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void IPAddressRadioButton_Click( object sender, EventArgs e )
        {
            TRACE( MASK.API, "++IPAddressRadioButton_Click()" ) ;
            DrawDynamicInput( CONFIGTYPE.IPADDRESS, true ) ;
            this.octetArray[0].Focus() ;
            TRACE( MASK.API, "--IPAddressRadioButton_Click()" ) ;
        }

        /// <summary>
        /// Event handler for IP Octet
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void IPOctetRadioButton_Click( object sender, EventArgs e )
        {
            TRACE( MASK.API, "++IPOctetRadioButton_Click()" ) ;
            this.DrawDynamicInput( CONFIGTYPE.IPOCTET, true ) ;
            this.octetArray[3].Focus() ;
            TRACE( MASK.API, "--IPOctetRadioButton_Click()" ) ;
        }
        
		
        /// <summary>
        /// Empty
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void EmptyRadioButton_Click( object sender, EventArgs e )
        {
            TRACE( MASK.API, "++EmptyRadioButton_Click()" ) ;
            DrawDynamicInput( CONFIGTYPE.EMPTY, true ) ;
            TRACE( MASK.API, "--EmptyRadioButton_Click()" ) ;
        }
#else
		/// <summary>
		/// Event handler for Host Name  button
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void HostNameButton_Click( object sender, EventArgs e )
		{
			TRACE( MASK.API, "++HostNameButton_Click()" ) ;
			DrawDynamicInput( CONFIGTYPE.HOSTNAME, true ) ;
			this.hostNameButton.BackColor = System.Drawing.Color.Gray ;
			this.ipOctetButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.ipAddressButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.emptyButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			//this.laneIDLabel.Text = "";
			this.instructionLabel.Text = configData.STREnterHostNameInstruction ;
			this.hostNameTextBox.Focus() ;
			this.configType = CONFIGTYPE.HOSTNAME ;
			TRACE( MASK.API, "--HostNameButton_Click()" ) ;
		}

		/// <summary>
		/// Event handler for IP Address
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void IPAddressButton_Click( object sender, EventArgs e )
		{
			TRACE( MASK.API, "++IPAddressButton_Click()" ) ;
			DrawDynamicInput( CONFIGTYPE.IPADDRESS, true ) ;
			this.ipAddressButton.BackColor = System.Drawing.Color.Gray ;
			this.hostNameButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.ipOctetButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.emptyButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			//this.laneIDLabel.Text = "";
			this.instructionLabel.Text = configData.STREnterIPAddressInstruction ;
			this.octetArray[0].Focus() ;
			this.octetBox = 1;
			this.configType = CONFIGTYPE.IPADDRESS ;
			TRACE( MASK.API, "--IPAddressButton_Click()" ) ;
		}

		/// <summary>
		/// Event handler for IP Octet
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void IPOctetButton_Click( object sender, EventArgs e )
		{
			TRACE( MASK.API, "++IPOctetButton_Click()" ) ;
			this.DrawDynamicInput( CONFIGTYPE.IPOCTET, true ) ;
			this.ipOctetButton.BackColor = System.Drawing.Color.Gray ;
			this.hostNameButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.ipAddressButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.emptyButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			//this.laneIDLabel.Text = "";
			this.instructionLabel.Text = configData.STREnterIPOctetInstruction ;
			this.octetArray[3].Focus() ;
			this.octetBox = 4;
			this.configType = CONFIGTYPE.IPOCTET ;
			TRACE( MASK.API, "--IPOctetButton_Click()" ) ;
		}

		/// <summary>
		/// Empty
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void EmptyButton_Click( object sender, EventArgs e )
		{
			TRACE( MASK.API, "++EmptyButton_Click()" ) ;
			this.configType = CONFIGTYPE.EMPTY ;
			this.emptyButton.BackColor = System.Drawing.Color.Gray ;
			this.hostNameButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.ipOctetButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.ipAddressButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			//this.laneIDLabel.Text = "";
			this.configType = CONFIGTYPE.EMPTY;
			this.instructionLabel.Text = configData.STREmptyText;
			DrawDynamicInput( CONFIGTYPE.EMPTY, true ) ;
			TRACE( MASK.API, "--EmptyButton_Click()" ) ;
		}
#endif
		/// <summary>
		/// IP Octet Event Handler - 1
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void IPOctet1_KeyPress( object sender, KeyPressEventArgs e )
		{
			TRACE( MASK.EXTENSIVE, "++IPOctet1_KeyPress()" ) ;
			ValidateIPOctet( 0, e ) ;
			TRACE( MASK.EXTENSIVE, "--IPOctet1_KeyPress()" ) ;
		}

		/// <summary>
		/// IP Octet Event Handler - 2
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void IPOctet2_KeyPress( object sender, KeyPressEventArgs e )
		{
			TRACE( MASK.EXTENSIVE, "++IPOctet2_KeyPress()" ) ;
			ValidateIPOctet( 1, e ) ;
			TRACE( MASK.EXTENSIVE, "--IPOctet2_KeyPress()" ) ;
		}

		/// <summary>
		/// IP Octet Event Handler - 3
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void IPOctet3_KeyPress( object sender, KeyPressEventArgs e )
		{
			TRACE( MASK.EXTENSIVE, "++IPOctet3_KeyPress()" ) ;
			ValidateIPOctet( 2, e ) ;
			TRACE( MASK.EXTENSIVE, "--IPOctet3_KeyPress()" ) ;
		}

		/// <summary>
		/// IP Octet Event Handler - 4
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void IPOctet4_KeyPress( object sender, KeyPressEventArgs e )
		{
			TRACE( MASK.EXTENSIVE, "++IPOctet4_KeyPress()" ) ;
			ValidateIPOctet( 3, e ) ;
			TRACE( MASK.EXTENSIVE, "--IPOctet4_KeyPress()" ) ;
		}

        public void OkButton_Click( object sender, EventArgs e )
        {
            TRACE( MASK.API, "++OkButton_Click()" ) ;
            bool bExit = true ;

            try
            {
                // Get lane info from user's input
                string laneInfo = GetLaneInfoFromInput() ;

                // Check if lane's info is the same as the edited lane
                if ( ( !this.bNewLane && ( laneInfo != null ) ) &&
                    ( ( this.laneButton.Text == laneInfo ) || 
                    ( this.laneButton.Text.ToUpper() == laneInfo.ToUpper() ) ) )
                {
                    TRACE( MASK.INFO, "Lane: " + laneInfo + " has has not been changed" ) ;
                    return ;
                }

                // Check for duplicate
                if ( this.parentForm.IsDuplicate( laneInfo, true ) )
                {
                    bExit = false ;
                    return ;
                }

                this.laneButton.Text = laneInfo ;

                // Add new lane or update laneInfoArray
                if ( this.bNewLane )
                {
                    this.parentForm.AddLane( this.laneButton.Text ) ;
                }
                else
                {
                    this.parentForm.UpdateLaneInfoArray() ;
                    this.parentForm.SetModify( true ) ;
                }
            }
            catch ( Exception ex )
            {
                TRACE( MASK.ERROR, ex.ToString() ) ;
            }
            finally
            {
                TRACE( MASK.API, "--OKButton_Click()" ) ;
                if ( bExit ) { this.Exit() ; }
            }
        }

        /// <summary>
        /// HideWindowTaskBar
        /// </summary>
        private void ShowWindowTaskBar(bool bFullScreen)
        {
            TRACE( MASK.API, "++ShowWindowTaskBar()" ) ;
            // Clear other close/minimize boxes
            this.MaximizeBox = false ;
            this.MinimizeBox = false ;
            this.ControlBox = false ;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None ;
            this.ClientSize = new System.Drawing.Size( DISPLAY_WIDTH, DISPLAY_HEIGHT ) ;
            // Set Size
            this.Location = new System.Drawing.Point( 0, 0 ) ;
            this.Size = new System.Drawing.Size( DISPLAY_WIDTH, DISPLAY_HEIGHT ) ;

            try
            {
                // Set this window to full screen
                IntPtr hMainWnd = RapConfigure.FindWindow( null, this.Text ) ;
                if (hMainWnd != IntPtr.Zero)
                {
                    RapConfigure.SHFullScreen( hMainWnd, bFullScreen ? RapConfigure.SHFS_HIDEALL : RapConfigure.SHFS_SHOWALL ) ;
                }

                // Hide Window Task Bar
                IntPtr hWnd = RapConfigure.FindWindow( "HHTaskBar", null );
                if (hWnd != IntPtr.Zero)
                {
                    RapConfigure.ShowWindow( hWnd, bFullScreen ? (int)RapConfigure.ShowWindowFlags.SW_HIDE : (int) RapConfigure.ShowWindowFlags.SW_SHOW ) ;
                }
            }
            catch (Exception e)
            {
                TRACE( MASK.ERROR, e.ToString() ) ;
            }
            finally
            {
                TRACE( MASK.INFO, "Full Screen: " + ( bFullScreen ? "TRUE" : "FALSE" ) ) ;
                TRACE( MASK.API, "--ShowWindowTaskBar()" ) ;
            }
        }

        /// <summary>
        /// Validate IP Octet Input
        /// </summary>
        /// <param name="octetIndex"></param>
        /// <param name="e"></param>
        private void ValidateIPOctet( int octetIndex, KeyPressEventArgs e )
        {
            TRACE( MASK.EXTENSIVE, "++ValidateIPOctet()" ) ;
            const char Delete = ( char )8 ;

            // Eliminate non-digit character
            e.Handled = ( ( !Char.IsDigit( e.KeyChar ) ) && ( e.KeyChar != Delete ) && ( e.KeyChar != '.' ) ) ;

            // Check input range [0,255]
            if ( Char.IsDigit( e.KeyChar ) )
            {
				// If the octet is currently highlighted - clear its value
				if (this.octetArray[octetBox-1].SelectionLength > 0)
				{
					this.octetArray[octetBox-1].Text = "" ;
				}

                string strInput = "" + this.octetArray[octetIndex].Text + e.KeyChar.ToString() ;
                int octetValue = ( strInput != "" ) ? System.Int32.Parse( strInput ) : -1 ;

                // Value outside [0, 255], automatically move cursor to next octet
				if ( ( octetValue < 0 ) || ( octetValue > 255 ) )
				{
					if ( octetIndex < 3 )
					{
						this.octetArray[octetIndex+1].Focus() ;
						this.octetArray[octetIndex+1].Text = e.KeyChar.ToString() ;
						this.octetArray[octetIndex+1].SelectionStart = 1 ;
						this.octetBox++ ;
					}
					e.Handled = true ;
				}
				else // within [0, 255]
                {
                    // Automaticaly move cursor to the next available octet field
                    if ( ( ( octetValue > 25 ) || ( strInput.Length >= 3 ) ) )
                    {
						if ( octetIndex < 3 )
						{
							this.octetArray[octetIndex+1].Focus() ;
							this.octetBox = octetBox + 1 ;
						}
						else if ( ( octetIndex == 3 ) && ( strInput.Length > 3 ) )
							e.Handled = true ;
                    }
                }
            }
            else if ( e.KeyChar == '.' )
            {
				if ( octetIndex < 3 )
				{
					this.octetArray[octetIndex+1].Focus() ;
					this.octetBox = octetBox + 1 ;
				}
                e.Handled = true ;
            }

            TRACE( MASK.EXTENSIVE, "IP octet index = " + octetIndex ) ;
            TRACE( MASK.EXTENSIVE, "e.KeyChar = " + e.KeyChar ) ;
            TRACE( MASK.EXTENSIVE, "--ValidateIPOctet()" ) ;
        }

        /// <summary>
        /// TRACE: forward log data to Log class
        /// </summary>
        /// <param name="maskLevel"></param>
        /// <param name="logData"></param>
        private void TRACE( MASK maskLevel, string logData )
        {
            DataCapture.TRACE( maskLevel, logData ) ;
		}

		/// <summary>
		/// octetArray1Click - set the octetfield
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void octetArray1Click(object sender, EventArgs e)
		{
			octetBox = 1;
		}

		/// <summary>
		/// octetArray2Click - set the octetfield
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void octetArray2Click(object sender, EventArgs e)
		{
			octetBox = 2;
		}

		/// <summary>
		/// octetArray3Click - set the octetfield
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void octetArray3Click(object sender, EventArgs e)
		{
			octetBox = 3;
		}

		/// <summary>
		/// octetArray4Click - set the octetfield
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void octetArray4Click(object sender, EventArgs e)
		{
			octetBox = 4;
		}
	}
}
