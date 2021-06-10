/// <summary>
/// Class: RapConfigure
/// Description: This class contains RapConfigure.cs file
///     This class is to:
///         - Initialize screen with existing data in RapNet.xml
///         - Provide GUIs for Add, Edit, Save, Reposition, Exit operations
/// Author: Nhuan Nguyen
/// Title:  NCR - Software Engineer
/// Email:  Nhuan.Nguyen@NCR.com
/// Phone:  (770) 813-3741
/// Date:   March 19, 2008
/// Version: 1.0
/// Revised: April 12, 2008
///		   : Edited for Desktop RAP
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
    // Lane configuration type
    public enum CONFIGTYPE {HOSTNAME, IPADDRESS, IPOCTET, EMPTY} ;
    // HHRAP model
    public enum HHRAPMODEL {MC50, MC70, OTHER} ;
    // Log level
    public enum MASK : uint
    {
        API         = 0x00000001, // API method entry and exit
        ERROR       = 0x00000002, // Error event - major unrecoverable problem 
        WARNING     = 0x00000004, // Warning event - minor recoverable problem
        INFO        = 0x00000008, // Informational event - trace normal activity
        EXTENSIVE   = 0x00000010, // Trace event - extensive tracing
        PERFORMANCE = 0x00000020, // Performance tracing
        ALL         = 0xFFFFFFFF  // Trace all
    }

    // OEM & Platform
    public enum SystemParametersInfoActions : uint
    {
        SPI_GETPLATFORMTYPE = 257, // Smartphone/PocketPC detection
        SPI_GETOEMINFO      = 258, // OEM to determine device's model MC50/MC70/etc
    }

    /// <summary>
    /// RapConfigure's constructor
    /// </summary>
	public class RapConfigure : System.Windows.Forms.Form
	{
		// Constants
#if ( WINCE )
        public int DISPLAY_WIDTH        = 240 ; // default - overrided by config file
        public int DISPLAY_HEIGHT       = 320 ; // default - overrided by config file
        private const int buttonWidth   = 56 ;
        private const int buttonHeight  = 30 ;
        private const int defaultHeight = 60 ;
        private const int inputHeight   = 20 ;
        private const int xOffset       = 8 ;
        private const int yOffset       = 8 ;
#else
		public int DISPLAY_WIDTH        = 1024 ; // default - overrided by config file
		public int DISPLAY_HEIGHT       = 768 ;  // default - overrided by config file
		private const int buttonWidth   = 180 ;
		private const int buttonHeight  = 60 ;
		private const int defaultHeight = 50 ;
		private const int inputHeight   = 30 ;
		private const int xOffset       = 10 ;
		private const int yOffset       = 10 ;
#endif

        

		private const int MAXLANE       = 10 ; // default - not overrided by config file
        
		// Components
		private System.Windows.Forms.Label      mainTitle ;
		private System.Windows.Forms.Label      repositionLabel ;
		private System.Windows.Forms.Button[]   laneButtonArray ;
		private System.Windows.Forms.Button     addButton ;
		private System.Windows.Forms.Button     saveButton ;
		private System.Windows.Forms.Button     exitButton ;
		private System.Windows.Forms.Button     srcButton ;
#if ( !WINCE )
		private System.Windows.Forms.Button     deleteButton ;
#else
		private System.Windows.Forms.ContainerControl buttonGroupContainerControl ;
#endif
		private System.Windows.Forms.Button     moveButton ;
		private System.Windows.Forms.ContainerControl lanesContainerControl ;
		
        
		// Lane Info
		private string[] laneInfoArray ;
		public string[] strIPOctetArray = new string[4] {"","","",""} ;
		// Default volatile and non-volatile directories
		private string configPath = "\\scot\\config" ;
		private string nonVolatilePath = "\\Application" ;
		// Where to save RapNet.xml for MC50
		private string loadingPath = "" ;
		// If any lanes modified
		private bool bModify = false ;
		// Can get HHRAP's IP address
		public bool bValidIPAddress = false ;
		private int numLanes = 0 ;
		private bool bRepositionMode = false ;
		private bool bDeleteLane = false;
		// Lane in pushdown state
		private bool bPushDown = false ;
		// Model needed for special saving mechanism in HHRAP MC50 
		private HHRAPMODEL deviceModel = HHRAPMODEL.OTHER ;
		ConfigData configData = null ;

		/// <summary>
		/// Constructor
		/// </summary>
		public RapConfigure()
		{
			try
			{
				// Get ConfigData
				this.configData = ConfigData.GetConfigData() ;

				// Override default config path
				this.configPath = this.configData.ConfigPath ;

				// Get loading path
				this.loadingPath = this.configData.LoadingPath ;

				// Get number of lanes
				this.numLanes = this.configData.NumLanes ;

				// Override Display dimension
				this.DISPLAY_WIDTH = this.configData.Width ;
				this.DISPLAY_HEIGHT = this.configData.Height ;

				// Get Lane Info Array
				if ( this.numLanes > 0 )
				{
					this.laneInfoArray = new string[this.numLanes] ;
					( this.configData.GetLaneInfoArray() ).CopyTo( this.laneInfoArray, 0 ) ;
				}
				else
				{
					this.laneInfoArray = new string[0] ;
					this.laneInfoArray.Initialize() ;
				}

				// Init
				InitializeComponent() ;

				// Get HHRAP's IP Address
				this.bValidIPAddress = this.GetIPAddress() ;
#if ( !WINCE )
				this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
#endif
				this.Show() ;
			}
			catch(Exception e)
			{
				TRACE(MASK.ERROR, e.ToString());
			}
		}

		/// <summary>
		/// Init components
		/// </summary>
		private void InitializeComponent()
		{
			TRACE( MASK.API, "++InitializeComponent()" ) ;

			// Title
			this.DisplayTitle() ;

			// Reposition Label
			this.DisplayRepositionLabel() ;

			// Lanes
			this.DisplayLanes( false ) ;

			// Button group
			this.DisplayButtonGroup() ;


			// HideWindow
			this.ShowWindowTaskBar(true) ;

			TRACE( MASK.API, "--InitializeComponent()" ) ;
		}

		private new int Width
		{
			get { return ( configData.Width ) ; }
		}

		private new int Height
		{
			get { return ( configData.Height ) ; }
		}

        /// <summary>
        /// Add new lane
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void AddButton_Click( object sender, EventArgs e )
        {
            TRACE( MASK.API, "++AddButton_Click()" ) ;
            TRACE( MASK.INFO, "Current number of lanes: " + this.numLanes ) ;
            if ( this.numLanes >= MAXLANE )
            {
                TRACE( MASK.API, "--AddButton_Click()" ) ;
                return ;
            }

            TRACE( MASK.API, "--AddButton_Click(): Calling EditLane()" ) ;

            Button newButton = new System.Windows.Forms.Button() ;
            newButton.Text = "" ;
            EditLane editLane = new EditLane( this, newButton, CONFIGTYPE.HOSTNAME, true ) ;
#if ( !WINCE )
			this.Hide();
#endif
            editLane.Show() ;
        }

        /// <summary>
        /// Add new lane
        /// </summary>
        /// <param name="strLaneInfo"></param>
        public void AddLane( string strLaneInfo )
        {
            TRACE( MASK.API, "++AddLane()" ) ;
            TRACE( MASK.INFO, "New lane: " + strLaneInfo ) ;
            TRACE( MASK.EXTENSIVE, "Before, number of lanes: " + this.numLanes ) ;
			this.repositionLabel.Text = "";
   
            if ( this.numLanes >= MAXLANE )
            {
                TRACE( MASK.API, "--AddLane(): it should never be here" ) ;
                return ;
            }

            // Inscrease number of lanes by one
            this.numLanes = System.Math.Min(++this.numLanes, MAXLANE) ;

            try
            {
                if ( ( this.laneInfoArray != null ) && ( this.laneInfoArray.Length >= 1 ) )
                {
                    // Copy laneInfoArray to tempArray
                    string[] tempArray = new string[this.laneInfoArray.Length] ;
                    this.laneInfoArray.CopyTo( tempArray, 0 ) ;

                    // Reset laneInfoArray, increase its size, and copy tempArray back
                    this.laneInfoArray.Initialize() ;
                    this.laneInfoArray = new string[this.numLanes] ;
                    tempArray.CopyTo( this.laneInfoArray, 0 ) ;

                    // Release tempArray
                    tempArray.Initialize() ;
                }
                else
                {
                    this.laneInfoArray = new string[this.numLanes] ;
                }

                // Always add new lane at the end
                this.laneInfoArray[this.numLanes-1] = strLaneInfo ;
            } 
            catch ( Exception e )
            {
                TRACE( MASK.ERROR, e.ToString() + "; numLanes: " + this.numLanes + "; Add New Button: " + strLaneInfo ) ;
                this.UpdateLaneInfoArray() ;  // Update laneInfoArray from actual lanes
            }
            finally
            {
                // Display Lane
                this.SetModify( true ) ;
                this.DisplayLanes( true ) ;

                TRACE( MASK.EXTENSIVE, "Afer, number of lanes: " + this.numLanes ) ;
                TRACE( MASK.API, "--AddLane()" ) ;
            }
        }

        /// <summary>
        /// Delete a lane
        /// </summary>
        /// <param name="delLane"></param>
        public void DeleteLane( Button delLane )
        {
            TRACE( MASK.API, "++DeleteLane()" ) ;
            TRACE( MASK.INFO, "Delete lane: " + delLane.Text ) ;
            TRACE( MASK.EXTENSIVE, "Before deletion: number of lanes: " + this.numLanes ) ;
            this.SetModify( true ) ;

            // Decrease number of lanes by one
            this.numLanes = System.Math.Max(0, --this.numLanes) ;

            if ( this.numLanes == 0 )
            {
                this.laneInfoArray.Initialize() ;
                this.DisplayLanes( true ) ;

                TRACE( MASK.EXTENSIVE, "After deletion: number of lanes: " + this.numLanes ) ;
                TRACE( MASK.API, "--DeleteLane()" ) ;
                return ;
            }

            // Reset laneInfo
            this.laneInfoArray.Initialize() ;
            this.laneInfoArray = new string[this.numLanes] ;

            // Update
            for ( int i = 0, j = 0 ; i < this.laneButtonArray.Length ; i++ )
            {
                if ( ( this.laneButtonArray[i] != delLane ) && ( j < this.numLanes ) )
                {
                    this.laneInfoArray[j++] = this.laneButtonArray[i].Text ;
                }
            }
            
            //Display
            this.DisplayLanes( true ) ;

            TRACE( MASK.EXTENSIVE, "After deletion: number of lanes: " + this.numLanes ) ;
            TRACE( MASK.API, "--DeleteLane()" ) ;
        }

        /// <summary>
        /// Dispose
        /// </summary>
        /// <param name="disposing"></param>
        protected override void Dispose( bool disposing )
        {
            try
            {
                // Dispose lane button group's container
                if ( this.lanesContainerControl != null )
                {
                    this.lanesContainerControl.Controls.Clear() ;
                    this.Controls.Remove( this.lanesContainerControl ) ;
                    this.lanesContainerControl.Dispose() ;
                }
#if ( WINCE )
                // Dispose operation button group's container
                if ( this.buttonGroupContainerControl != null )
                {
                    this.buttonGroupContainerControl.Controls.Clear() ;
                    this.Controls.Remove( this.buttonGroupContainerControl ) ;
                    this.buttonGroupContainerControl.Dispose() ;
                }
#endif
                // Clear lane info array
                if ( this.laneInfoArray != null )
                    this.laneInfoArray.Initialize() ;

                // Remove all controls
                this.Controls.Clear() ;

                base.Dispose( disposing ) ;
            }
            catch ( Exception e )
            {
#if ( DEBUG )
                MessageBox.Show( e.ToString() ) ;
#endif
            }
        }

        /// <summary>
        /// Define Button Group: Add, Save, Exit
        /// </summary>
		private void DisplayButtonGroup()
		{
			TRACE( MASK.API, "++DisplayButtonGroup()" ) ;
			int buttonNewWidth = buttonWidth ;
			int buttonNewHeight = buttonHeight ;

#if ( WINCE )
			int x = ( int )( DISPLAY_WIDTH - 1.5*xOffset - 4*buttonNewWidth )/2 ;
			int y = this.lanesContainerControl.Location.Y + this.lanesContainerControl.Height + 2*yOffset ;
			// Define group's container
			this.buttonGroupContainerControl = new System.Windows.Forms.ContainerControl() ;
			this.buttonGroupContainerControl.Location = new System.Drawing.Point( 0,y ) ;
			this.buttonGroupContainerControl.Size = new System.Drawing.Size( DISPLAY_WIDTH, buttonHeight ) ;
			y = 0;
#else
			if( 800 == DISPLAY_WIDTH )
			{
				buttonNewWidth = 120 ;
				buttonNewHeight = 50 ; 
			}
			int x = ( int )( DISPLAY_WIDTH - 1.5*xOffset - 5*buttonNewWidth )/2 ;
			int y = this.DISPLAY_HEIGHT - ( xOffset + buttonNewHeight) ; 
			this.deleteButton = new System.Windows.Forms.Button() ;
			this.deleteButton.Location = new System.Drawing.Point( x, y ) ;
			this.deleteButton.Size = new System.Drawing.Size( buttonNewWidth, buttonNewHeight ) ;
			this.deleteButton.Text = configData.STRDeleteButtonText ;
			this.deleteButton.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold) ;
			if( this.numLanes < 1 )
			{
				this.deleteButton.Enabled = false;
			}
			this.Controls.Add( this.deleteButton ) ;
			this.deleteButton.Click += new EventHandler( DeleteButton1_Click ) ;
     		x += ( int )( 0.5*xOffset + buttonNewWidth ) ;
			TRACE( MASK.INFO, "Delete button: " + this.deleteButton.Text ) ;

#endif
            // Move
            this.moveButton = new System.Windows.Forms.Button() ;
            this.moveButton.Location = new System.Drawing.Point( x, y ) ;
            this.moveButton.Size = new System.Drawing.Size( buttonNewWidth, buttonNewHeight ) ;
#if ( WINCE )
            this.moveButton.Font = new System.Drawing.Font("Arial", 7.5F, System.Drawing.FontStyle.Regular) ;
#else
            this.moveButton.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold) ;
#endif
            this.moveButton.Text = configData.STRMoveButtonText ;
#if( WINCE )
				this.buttonGroupContainerControl.Controls.Add( this.moveButton ) ;
#else
			this.Controls.Add( this.moveButton ) ;
#endif
            this.moveButton.Click +=new EventHandler( MoveButton_Click ) ;
            this.moveButton.Enabled = ( this.numLanes > 1 ) ;
            TRACE( MASK.EXTENSIVE, "Move button: " + this.moveButton.Text ) ;

            // Add
            x += ( int )( 0.5*xOffset + buttonNewWidth ) ;
            this.addButton = new System.Windows.Forms.Button() ;
            this.addButton.Location = new System.Drawing.Point( x, y ) ;
            this.addButton.Size = new System.Drawing.Size( buttonNewWidth, buttonNewHeight ) ;
#if ( WINCE )
            this.addButton.Font = new System.Drawing.Font("Arial", 7.5F, System.Drawing.FontStyle.Regular) ;
#else
            this.addButton.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold) ;
#endif
            this.addButton.Text = configData.STRAddButtonText ;
#if( WINCE )
			this.buttonGroupContainerControl.Controls.Add( this.addButton ) ;
#else
			this.Controls.Add( this.addButton ) ;
#endif
            this.addButton.Click +=new EventHandler( AddButton_Click ) ;
            this.addButton.Enabled  = ( this.numLanes < MAXLANE ) ;
            TRACE( MASK.EXTENSIVE, "Add button: " + this.addButton.Text ) ;

            // Save
            x += ( int )( 0.5*xOffset + buttonNewWidth ) ;
            this.saveButton = new System.Windows.Forms.Button() ;
            this.saveButton.Location = new System.Drawing.Point( x , y ) ;
            this.saveButton.Size = new System.Drawing.Size( buttonNewWidth, buttonNewHeight ) ;
#if ( WINCE )
            this.saveButton.Font = new System.Drawing.Font("Arial", 7.5F, System.Drawing.FontStyle.Regular) ;
#else
            this.saveButton.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold) ;
#endif
            this.saveButton.Text = configData.STRSaveButtonText ;
            this.saveButton.Click +=new EventHandler( SaveButton_Click );
            this.saveButton.Enabled = this.bModify ;
#if( WINCE )
				this.buttonGroupContainerControl.Controls.Add( this.saveButton ) ;
#else
			this.Controls.Add( this.saveButton ) ;
#endif
            TRACE( MASK.EXTENSIVE, "Save button: " + this.saveButton.Text ) ;

            // Exit
            x += ( int )( 0.5*xOffset + buttonNewWidth ) ;
            this.exitButton = new System.Windows.Forms.Button() ;
            this.exitButton.Location = new System.Drawing.Point( x, y ) ;
            this.exitButton.Size = new System.Drawing.Size( buttonNewWidth, buttonNewHeight ) ;
#if ( WINCE )
            this.exitButton.Font = new System.Drawing.Font("Arial", 7.5F, System.Drawing.FontStyle.Regular) ;
#else
            this.exitButton.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold) ;
#endif
            this.exitButton.Text = configData.STRExitButtonText ;
            this.exitButton.Click += new EventHandler( ExitButton_Click ) ; 
#if( WINCE )
				this.buttonGroupContainerControl.Controls.Add( this.exitButton ) ;
#else
			this.Controls.Add( this.exitButton ) ;
#endif
            TRACE( MASK.EXTENSIVE, "Exit button: " + this.exitButton.Text ) ;

#if ( !WINCE )
			this.deleteButton.BackColor = System.Drawing.Color.WhiteSmoke;
			this.moveButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.addButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.saveButton.BackColor = System.Drawing.Color.WhiteSmoke ;
			this.exitButton.BackColor = System.Drawing.Color.WhiteSmoke ;
#endif				
			// Add this button group to main's control
#if (WINCE)
			this.Controls.Add( this.buttonGroupContainerControl ) ;
#endif	


            TRACE( MASK.API, "--DisplayButtonGroup()" ) ;
        }

        /// <summary>
        /// Display Reposition label: instruction for select
        /// source and destination buttons
        /// </summary>
        private void DisplayRepositionLabel()
        {
            TRACE( MASK.API, "++DisplayRepositionLabel()" ) ;
            int x = 0;
            int y = this.mainTitle.Location.Y + this.mainTitle.Height + yOffset ;

            this.repositionLabel = new System.Windows.Forms.Label() ;
            this.repositionLabel.Location = new System.Drawing.Point( x, y ) ;
            this.repositionLabel.Size = new System.Drawing.Size( DISPLAY_WIDTH, inputHeight ) ;
#if ( !WINCE )
			this.repositionLabel.Font = new System.Drawing.Font( "Arial", 18F, System.Drawing.FontStyle.Italic ) ;
			this.repositionLabel.ForeColor = System.Drawing.Color.Red ;
			this.repositionLabel.BackColor = System.Drawing.Color.FromArgb(215, 215, 215);
#endif
            this.repositionLabel.Text = "" ;
            this.repositionLabel.TextAlign = System.Drawing.ContentAlignment.TopCenter ;
            this.Controls.Add( this.repositionLabel ) ;

            TRACE( MASK.API, "--DisplayRepositionLabel()" ) ;
        }

        /// <summary>
        /// Display Lanes
        /// </summary>
        /// <param name="bUpdateState"></param>
        private void DisplayLanes( bool bUpdateState )
        {
#if (WINCE)
            bool bTwoColumns = false ; // default for HHRAP
#else
			bool bTwoColumns = true ;  // Always for Desktop RAP
#endif
            TRACE( MASK.API, "++DisplayLanes()" ) ;
            TRACE( MASK.INFO, "bUpdateState = " + bUpdateState.ToString() ) ;
            if ( ( this.numLanes < 0 ) || ( this.numLanes > MAXLANE ) )
            {
                return ;
            }

            // Set bTwoColumns = (this.numLanes > 5) too early will break the following line
			int x = bTwoColumns ? buttonWidth/2 : 0 ;
			int y = this.repositionLabel.Location.Y + this.repositionLabel.Height + ( int )( 0.5 * yOffset ) ;
			int areaWidth = DISPLAY_WIDTH - 2*x ;
			int areaHeight = DISPLAY_HEIGHT - y - 2*buttonHeight ;

            // Clear and remove old lanes
            if ( this.lanesContainerControl != null )
            {
                this.lanesContainerControl.Controls.Clear() ;
                this.Controls.Remove( this.lanesContainerControl ) ;
                this.lanesContainerControl.Dispose() ;
            }

            // Define lanes container control
            this.lanesContainerControl = new System.Windows.Forms.ContainerControl() ;
            this.lanesContainerControl.Location = new System.Drawing.Point( x, y ) ; 
            this.lanesContainerControl.Size = new System.Drawing.Size( areaWidth, areaHeight+5 ) ;
#if ( !WINCE )
			this.lanesContainerControl.BackColor = System.Drawing.Color.FromArgb(215, 215, 215);
#endif

            // Init LaneArray
            if ( this.numLanes > 0 )
            {
                this.laneButtonArray = new System.Windows.Forms.Button[this.numLanes] ;
            }

#if ( WINCE )
            // Display Rap Template
            int width = DISPLAY_WIDTH - 2*xOffset ;
            int height = ( this.numLanes > 0 ? ( int )( ( areaHeight ) / numLanes ) : defaultHeight ) ;

            // Half-size button if there are 6 lanes or more
            if ( this.numLanes > 5 )
            {
                width = ( int )( width / 2 ) ;
                height = ( int )( ( 2*areaHeight ) / ( this.numLanes + ( this.numLanes % 2 ) ) ) ;
            }

            // if height too big, set to default
            height = System.Math.Min( height, defaultHeight ) ;

            // Now, determine if lanes should be displayed in two columns on HHRAP
            bTwoColumns = ( bool )( this.numLanes > 5 ) ;
#else
			int width = (DISPLAY_WIDTH / 2) -  (int)(.5 * buttonWidth) - 4*xOffset;
			int height = (areaHeight - 4*yOffset) / 5 ;
#endif       
            // Draw lane buttons
            x = bTwoColumns ? 0 : xOffset ; 
			y = 1 ;
            for ( int i = 0 ; i < this.numLanes ; i++ )
            {
                laneButtonArray[i] = new System.Windows.Forms.Button() ;
#if ( !WINCE ) 
				laneButtonArray[i].BackColor = System.Drawing.Color.White;
				laneButtonArray[i].FlatStyle = System.Windows.Forms.FlatStyle.Flat;
                laneButtonArray[i].Font = new System.Drawing.Font("Arial", 12F);
#endif
                if ( bTwoColumns )
                {
                    if ( ( i % 2 ) == 1 ) // Even lane
                    {   
#if ( WINCE )
                        x = xOffset + width + 1 ;
#else
						x = width + 5*xOffset ; 
#endif
                    }
                    else // Odd lane
                    {
#if ( WINCE )
                        x = xOffset ;
                        y += ( i > 0 ? height + 1 : 0 ) ;
#else
						x = 2*xOffset ;
                        y += ( i > 0 ? height + yOffset : 0 ) ;
#endif
                    }
                }
                else
                {
                    y += ( i > 0 ? height+1 : 0 ) ;
                }
                this.laneButtonArray[i].Location = new System.Drawing.Point( x, y ) ;
                this.laneButtonArray[i].Size = new System.Drawing.Size( width, height ) ;
                this.laneButtonArray[i].Text = ( i < this.laneInfoArray.Length ) ? laneInfoArray[i] : "" ;
                this.lanesContainerControl.Controls.Add( this.laneButtonArray[i] ) ;
                //Edit/reposition lane modes
                this.laneButtonArray[i].Click += new EventHandler( LaneButton_Click ) ;

                TRACE( MASK.INFO, "Lane[" + i + "]=" + this.laneButtonArray[i].Text ) ;
            }

            // Add button group to main control
            this.Controls.Add( this.lanesContainerControl ) ;

            // Update
            if ( bUpdateState )
            {
				this.moveButton.Enabled = ( this.numLanes > 1 ) ;
				this.addButton.Enabled = ( this.numLanes < MAXLANE );
				this.saveButton.Enabled = this.bModify ;
				this.exitButton.Enabled = true ;
#if ( !WINCE )
				this.deleteButton.Enabled = ( this.numLanes > 0 ) ;

				if( this.bDeleteLane )
				{
					this.moveButton.Enabled = false;
					this.addButton.Enabled = false;
					this.saveButton.Enabled = false;
					this.exitButton.Enabled = false;
				}
#endif
				if( this.bRepositionMode )
				{
					this.addButton.Enabled = false;
					this.saveButton.Enabled = false;
					this.exitButton.Enabled = false;			
#if ( !WINCE)
					this.deleteButton.Enabled = false;
#endif
				}

                this.Show() ;
			
            }

            TRACE( MASK.API, "--DisplayLanes()" ) ;
        }

        /// <summary>
        /// Display Main Title
        /// </summary>
        private void DisplayTitle()
        {
            TRACE( MASK.API, "++DisplayTitle()" ) ;
            // mainTitle
            this.mainTitle = new System.Windows.Forms.Label() ;
#if ( WINCE )
            this.mainTitle.Font = new System.Drawing.Font( "Times New Roman", 10F, System.Drawing.FontStyle.Bold ) ;
            this.mainTitle.Location = new System.Drawing.Point( 0, yOffset ) ;
			this.mainTitle.TextAlign = System.Drawing.ContentAlignment.TopCenter ;
			this.mainTitle.Size = new System.Drawing.Size( DISPLAY_WIDTH, inputHeight ) ;
#else
            this.mainTitle.Font = new System.Drawing.Font( "Arial", 32F, System.Drawing.FontStyle.Bold ) ;
            this.mainTitle.Location = new System.Drawing.Point( 3*xOffset, 3*yOffset ) ;
			this.mainTitle.Size = new System.Drawing.Size( DISPLAY_WIDTH, inputHeight*2) ;
#endif
            
            
            this.mainTitle.Text = configData.STRMainTitle ;
#if ( !WINCE )
			this.mainTitle.BackColor = System.Drawing.Color.FromArgb(215, 215, 215 ); 
#endif
            this.Controls.Add( this.mainTitle ) ;
            this.Text = configData.STRMainTitle ;

            TRACE( MASK.EXTENSIVE, "Main title: " + this.mainTitle.Text ) ;
            TRACE( MASK.API, "--DisplayTitle()" ) ;
        }

        /// <summary>
        /// Exit: Cleanup and terminate the application
        /// </summary>
        private void Exit( bool bRestartRap )
        {
            if ( bRestartRap )
            {
                // Ask user to re-start RAP
                MessageBox.Show( configData.STRRestartRAPText, configData.STRRestartRAPTitle, MessageBoxButtons.OK, 
                    MessageBoxIcon.None, MessageBoxDefaultButton.Button1 ) ;

                TRACE( MASK.INFO, "You need to restart RAP!" ) ;
            }
            
            // Close Log
            DataCapture.CloseFile() ;

            this.Dispose() ;
            this.Close() ;
            Application.Exit() ;
        }

        /// <summary>
        /// Exit Button Event handler
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ExitButton_Click( object sender, EventArgs e )
        {
            TRACE( MASK.API, "++ExitButton_Click()" ) ;
            DialogResult confirm = DialogResult.No ;

            if ( this.bModify )
            {
                confirm = MessageBox.Show( configData.STRConfirmExitText,configData.STRConfirmExitTitle, 
                    MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1 ) ;
            }
               
            if ( ( confirm == DialogResult.Yes ) || ( !this.bModify ) )
            {
                TRACE( MASK.API, "--ExitButton_Click()" ) ;
                this.Exit( false ) ;
            }
            else if ( this.bModify )
            {
                TRACE( MASK.INFO, "User cancels Exit operation" ) ;
                TRACE( MASK.API, "--ExitButton_Click()" ) ;
            }
        }

        /// <summary>
        /// Get Handle device IP Address
        /// </summary>
        /// <returns></returns>
        private bool GetIPAddress()
        {
            TRACE( MASK.API, "++GetIPAddress()" ) ;
            try
            {
                string strHostName = System.Net.Dns.GetHostName() ;
                char[] splitter = {'.'} ;
                IPHostEntry ipEntry = System.Net.Dns.GetHostByName( strHostName ) ;
                IPAddress[] ipAddressList = ipEntry.AddressList ;
                this.strIPOctetArray = new string[4] {"", "", "", ""} ;

                for ( int i = 0 ; i < ipAddressList.Length ; i++ )
                {
                    string strIPAddress = ipAddressList[i].ToString() ;
                    this.strIPOctetArray = strIPAddress.Split( splitter ) ;

                    TRACE( MASK.INFO, "Process ipAddressList["+i+"]: " + strIPAddress ) ;

                    if ( ( this.strIPOctetArray != null ) && ( strIPOctetArray.Length == 4 ) )
                    {
                        TRACE( MASK.INFO, "Found HHRAP's IP Address: " + strIPAddress ) ;
                        return ( true ) ;
                    }
                    else
                    {
                        this.strIPOctetArray.Initialize() ;
                        this.strIPOctetArray = new string[4] {"", "", "", ""} ;
                        TRACE( MASK.WARNING, "Unable to get HHRAP's IP address" ) ;
                        return ( false ) ;
                    }
                }
            }
            catch (Exception e)
            {
                TRACE( MASK.ERROR, e.ToString() ) ;
            }
            finally
            {
                TRACE( MASK.API, "--GetIPAddress()" ) ;
            }
            return ( false ) ;
        }

        /// <summary>
        /// GetIPType: IP Address or IP Octet
        /// </summary>
        /// <param name="strIPAddress"></param>
        /// <returns></returns>
        private CONFIGTYPE GetIPType( string strIPAddress )
        {
            TRACE( MASK.API, "++GetIPType()" ) ;
            TRACE( MASK.INFO, "Input IP: " + strIPAddress ) ;
            CONFIGTYPE configType = CONFIGTYPE.EMPTY ;

            if ( strIPAddress == null )
            {
                configType = CONFIGTYPE.EMPTY ;
                goto END_GETIPTYPE ;
            }

            char[] separator = {'.'} ;
            string[] strOctetArray = strIPAddress.Split( separator ) ;

            if ( ( strOctetArray == null ) || ( strOctetArray.Length != 4 ) )
            {
                configType = CONFIGTYPE.HOSTNAME ;
                goto END_GETIPTYPE ;
            }

            // If the first 3 octets match first 3 octets of HHRAP's IP Address
            if ( this.bValidIPAddress && 
                ( strOctetArray[0] == this.strIPOctetArray[0] ) &&
                ( strOctetArray[1] == this.strIPOctetArray[1] ) &&
                ( strOctetArray[2] == this.strIPOctetArray[2] ) )
            {
                configType = CONFIGTYPE.IPOCTET ;
            }
            else
            {
                configType = CONFIGTYPE.IPADDRESS ;
            }

            END_GETIPTYPE:
            TRACE( MASK.INFO, "Output: " + configType.ToString() ) ;
            TRACE( MASK.API, "--GetIPType()" ) ;

            return ( configType ) ;
        }

        /// <summary>
        /// Get Lane Config Type: Host Name, IP Address, IP Octet, or Empty
        /// </summary>
        /// <param name="inputString"></param>
        /// <returns></returns>
        private CONFIGTYPE GetLaneConfigType( string inputString )
        {
            TRACE( MASK.API, "++GetLaneConfigType()" ) ;
            TRACE( MASK.INFO, "Input: " + inputString ) ;
            int numDots = 0 ;
            bool bNonDigit = false ;
            CONFIGTYPE configType = CONFIGTYPE.EMPTY ;
            
            if ( ( inputString == null ) || ( inputString.Length == 0 ) )
            {
                configType = CONFIGTYPE.EMPTY ;
                goto END_GETLANECONFIGTYPE ;
            }

            for ( int i = 0 ; i < inputString.Length ; i++ )
            {
                if ( inputString[i] == '.' )
                    numDots++ ;
                else if ( !Char.IsDigit( inputString[i] ) )
                    bNonDigit = true ;
            }

            if ( !bNonDigit && ( numDots == 3 ) )
            {
                configType = GetIPType( inputString ) ;
            }
            else
            {
                configType = CONFIGTYPE.HOSTNAME ;
            }

            END_GETLANECONFIGTYPE:
            TRACE( MASK.INFO, "Output: " + configType.ToString() ) ;
            TRACE( MASK.API, "--GetLaneConfigType()" ) ;
            return ( configType ) ;
        }

        /// <summary>
        /// InsertLane: Shift buttons in between the source button and the
        /// the destination button up or down.
        /// </summary>
        /// <param name="srcButton"></param>
        /// <param name="destButton"></param>
        private void InsertLane( Button srcButton, Button destButton )
        {
            TRACE( MASK.API, "++InsertLane()" ) ;
            TRACE( MASK.INFO, "Source lane: "+srcButton.Text + "; destination lane: " + destButton.Text ) ;
            
            if ( !this.bRepositionMode )
            {
                TRACE( MASK.INFO, "Reposition mode is not enabled" ) ;
                TRACE( MASK.API, "--InsertLane()" ) ;
                return ;
            }

            try
            {
                // If source and destination are the same, just clear its state ;
                if ( srcButton.Equals( destButton ) )
                {
                    srcButton.BackColor = this.addButton.BackColor ;
                    TRACE( MASK.INFO, "Source lane is the same as destination lane" ) ;
                    return ;
                }

                bool bShiftUp = false ;
                this.laneInfoArray.Initialize() ;

                this.laneInfoArray = new string[this.laneButtonArray.Length] ;

                for ( int i = 0, j = 0; i < this.laneButtonArray.Length && j < this.laneButtonArray.Length; i++ )
                {
                    if ( this.laneButtonArray[i] == srcButton )
                    {
                        bShiftUp = true ;
                        srcButton.BackColor = destButton.BackColor ;
                    }
                    else if ( this.laneButtonArray[i] == destButton )
                    {
                        this.laneInfoArray[j++] = bShiftUp ? destButton.Text : srcButton.Text ;
                        this.laneInfoArray[j++] = bShiftUp ? srcButton.Text : destButton.Text ;

                        TRACE( MASK.INFO, "Lane: " + srcButton.Text + " inserted at lane " + destButton.Text ) ;
                    }
                    else
                    {
                        string strAction = (i == j) ? (" remains at position: " + j) : (" shifted to position: " + j) ;
                        this.laneInfoArray[j++] = this.laneButtonArray[i].Text ;
                        TRACE( MASK.INFO, "Lane["+i+"]="+laneButtonArray[i].Text + strAction ) ;
                    }
                }
            }
            catch (Exception e)
            {
                TRACE( MASK.ERROR, e.ToString() ) ;
                // When it gets here, there is something wrong; 
                // so reset laneInfoArray from actual lane buttons
                this.UpdateLaneInfoArray() ;
            }
            finally
            {
                this.bModify = true ;
                this.DisplayLanes( true ) ;
                TRACE( MASK.API, "--InsertLane()" ) ;
            }
        }

        /// <summary>
        /// IsDuplicate: check for duplicate non-empty lanes
        /// </summary>
        /// <param name="laneInfo"></param>
        /// <returns></returns>
        public bool IsDuplicate( string strLaneInfo, bool bDisplayError )
        {
            TRACE( MASK.API, "++IsDuplicate()" ) ;
            TRACE( MASK.INFO, "Check lane: " + strLaneInfo ) ;

            try
            {
                string checkLane = strLaneInfo.Trim() ;

                // Ignore empty lanes
                if ( ( checkLane == null ) || ( checkLane.Length == 0 ) )
                {
                    return (false) ;
                }

                // Check if it is the same as HHRAP's IP Address
                if ( this.bValidIPAddress && ( this.strIPOctetArray.Length == 4 ) )
                {
                    string strHHRapIPAddress = this.strIPOctetArray[0] + "." + this.strIPOctetArray[1] + "." +
                        this.strIPOctetArray[2] + "." + this.strIPOctetArray[3] ;

                    if ( strLaneInfo == strHHRapIPAddress )
                    {
                        if (bDisplayError)
                        {
#if ( WINCE )
                            string strErrorMsge = configData.STRErrorText + "! " + configData.STRSameHHRapIPAddress.Replace("%s", strLaneInfo) ;
#else
							string strErrorMsge = configData.STRErrorText + "! " + configData.STRSameRapIPAddress.Replace("%s", strLaneInfo) ;
#endif
                            MessageBox.Show( strErrorMsge, configData.STRDuplicateIPTitle, 
                                MessageBoxButtons.OK, MessageBoxIcon.Hand, MessageBoxDefaultButton.Button1 ) ;
                        }
#if ( WINCE )
                        TRACE( MASK.ERROR, "Error: Lane '" + strLaneInfo + "' is the same as HHRAP's IP Address!") ;
#else
						TRACE( MASK.ERROR, "Error: Lane '" + strLaneInfo + "' is the same as RAP's IP Address!") ;
#endif
                        return ( true ) ;
                    }
                }

                // Check for existing lane
                checkLane = checkLane.ToUpper() ;
                for ( int i = 0; i < this.laneButtonArray.Length; i++)
                {
                    if ( this.laneButtonArray[i].Text != "" )
                    {
                        string strLane = this.laneButtonArray[i].Text ;
                        strLane = strLane.Trim() ;
                        strLane = strLane.ToUpper() ;

                        if ( ( strLaneInfo == this.laneButtonArray[i].Text ) ||
                            ( checkLane.CompareTo( strLane ) == 0 ) )
                        {
                            if (bDisplayError)
                            {
                                string strErrorMsge = configData.STRErrorText + ": " + configData.STRDuplicateLaneText.Replace("%s", strLaneInfo) ;
                                MessageBox.Show( strErrorMsge, configData.STRDuplicateLaneTitle, 
                                    MessageBoxButtons.OK, MessageBoxIcon.Hand, MessageBoxDefaultButton.Button1 ) ;
                            }

                            TRACE( MASK.ERROR, "Error: Lane '" + strLaneInfo + "' already exists") ;
                            return (true) ;
                        }
                    }
                }
            }
            catch ( Exception e )
            {
                TRACE( MASK.ERROR, e.ToString() ) ;
            }
            finally
            {
                TRACE( MASK.API, "--IsDuplicate()" ) ;
            }

            return (false) ;
        }

        /// <summary>
        /// Lane Button Click Event: Same event is used for Edit Lane mode
        /// and "Reposition" mode
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void LaneButton_Click( object sender, EventArgs e )
        {
            TRACE( MASK.API, "++LaneButton_Click()" ) ;
            // If it is Reposition Mode
			if ( this.bRepositionMode )
			{
				TRACE( MASK.INFO, "Active mode: Reposition Lane" ) ;
				this.Reposition( sender ) ;
				TRACE( MASK.API, "--LaneButton_Click()" ) ;
			}
#if ( !WINCE )
			else if ( this.bDeleteLane )
			{
				string message = configData.STRConfirmDeleteText + " " + (( Button ) sender).Text +"?" ; 
      
				if ( DialogResult.Yes == MessageBox.Show( message, configData.STRConfirmDeleteTitle, 
					MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1 ) )
				{
					this.DeleteLane( ( Button ) sender ) ;
				}
				if( this.numLanes < 1 )
				{
					this.repositionLabel.Text = configData.STRNoLane;
					MessageBox.Show(configData.STRNoLaneToDelete, configData.STRNoLane);
					this.bDeleteLane = false;
					this.deleteButton.BackColor = this.addButton.BackColor;
					this.deleteButton.Enabled = false;
					this.saveButton.Enabled = this.bModify ;
					this.addButton.Enabled = true;
					this.exitButton.Enabled = true;
				}
			}
#endif
			else
			{
				TRACE( MASK.INFO, "Active mode: Edit Lane" ) ;
				TRACE( MASK.API, "--LaneButton_Click(): Calling EditLane()" ) ;
				EditLane editLane = new EditLane( this, ( Button )sender, GetLaneConfigType( ( ( Button )sender ).Text ), false ) ;
#if ( !WINCE )
				this.Hide();
#endif
				editLane.Show() ;
			}            
        }

		/// <summary>
		/// Delete Button Event handler
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void DeleteButton1_Click( object sender, EventArgs e )
		{
			TRACE( MASK.API, "++DeleteButton_Click()" ) ;
			
			this.bDeleteLane = ( !this.bDeleteLane );

			// Set button state
#if ( !WINCE )
			this.deleteButton.BackColor = ( this.bDeleteLane ? System.Drawing.Color.Gray : this.addButton.BackColor ) ;
#endif
			this.moveButton.Enabled = ( !this.bDeleteLane  && this.numLanes > 1 );
			this.addButton.Enabled = ( this.bDeleteLane ? false : ( this.numLanes < MAXLANE ) ) ;
			this.saveButton.Enabled = ( this.bDeleteLane ? false : this.bModify ) ;
			this.exitButton.Enabled = !this.bDeleteLane ;

			// Init/Clear selected button's state
			this.bPushDown = false ;

			// Update instruction label
			this.repositionLabel.Text = ( this.bDeleteLane ? configData.STRSelectLaneToDelete : "" ) ;

			// If srcButton is still in PushDown state, but user cancels Reposition mode
			// we need to clear and reset the srcButton's state to normal
			if ( ( !this.bDeleteLane ) && ( this.srcButton != null ) )
				this.srcButton.BackColor = this.addButton.BackColor ;

			TRACE( MASK.API, "--DeleteButton_Click()" ) ;
		}

        /// <summary>
        /// Move Button's Event handler: this a special mode to designate
        /// the Mouse's Click event for Reposition mode
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MoveButton_Click( object sender, EventArgs e )
        {
            TRACE( MASK.API, "++MoveButton_Click()" ) ;

            if ( this.numLanes < 2 )
            {
                TRACE( MASK.INFO, "number of lanes=" + this.numLanes ) ;
                TRACE( MASK.API, "--MoveButton_Click()" ) ;
                return ;
            }

            // Flip-flop the mode
            this.bRepositionMode = !this.bRepositionMode ;

            // Set button state
            this.moveButton.BackColor = ( this.bRepositionMode ? System.Drawing.Color.Gray : this.addButton.BackColor ) ;
            this.addButton.Enabled = ( this.bRepositionMode ? false : ( this.numLanes < MAXLANE ) ) ;
            this.saveButton.Enabled = ( this.bRepositionMode ? false : this.bModify ) ;
#if ( !WINCE )
			this.deleteButton.Enabled = !this.bRepositionMode ;
#endif
            this.exitButton.Enabled = !this.bRepositionMode ;

            // Init/Clear selected button's state
            this.bPushDown = false ;

            // Update instruction label
            this.repositionLabel.Text = ( this.bRepositionMode ? configData.STRSelectSourceText : "" ) ;

            // If srcButton is still in PushDown state, but user cancels Reposition mode
            // we need to clear and reset the srcButton's state to normal
            if ( ( !this.bRepositionMode ) && ( this.srcButton != null ) )
                this.srcButton.BackColor = this.addButton.BackColor ;

            TRACE( MASK.INFO, "Reposition mode is " + ( this.bRepositionMode ? "activated" : "deactivated" ) ) ;
            TRACE( MASK.API, "--MoveButton_Click()" ) ;
        }

        /// <summary>
        /// Reposition: Select a source button, and click on a 
        /// destination button to insert the source button
        /// </summary>
        /// <param name="sender"></param>
        protected void Reposition( object sender )
        {
            TRACE( MASK.API, "++Reposition()" ) ;
            TRACE( MASK.INFO, this.repositionLabel.Text + ": " + ((Button)sender).Text) ;

            // Check if Reposition Mode enable
            if ( !this.bRepositionMode )
            {
                TRACE( MASK.API, "--Repostion(): reposition mode is not enabled" ) ; 
                return ;
            }

            if ( !this.bPushDown )
            {
                // Seclect source button
                this.srcButton = ( Button )sender ;
                this.srcButton.BackColor = System.Drawing.Color.Gray ;
            }
            else
            {
                // Insert source button at the destination lane
                this.InsertLane( this.srcButton, ( Button ) sender ) ;
            }

            // Flip-flop its state
            this.bPushDown = !this.bPushDown ;

            this.repositionLabel.Text = ( this.bPushDown ? configData.STRSelectDestinationText : configData.STRSelectSourceText ) ;
            TRACE( MASK.API, "--Reposition()" ) ;
        }

        /// <summary>
        /// Save change(s) to RapNet.xml
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SaveButton_Click( object sender, EventArgs e )
        {
            TRACE( MASK.API, "++SaveButton_Click()" ) ;

            string srcFileName = this.loadingPath + "\\RapNet.xml" ;
            
            // Check if RapNet.xml is still there
            if ( ( this.loadingPath == null ) || !File.Exists( srcFileName ) )
            {
                TRACE( MASK.INFO, "File: " + srcFileName + " does not exist" ) ;
                TRACE( MASK.API, "--SaveButton_Click()" ) ;
                this.Exit( false ) ;
                return ;
            }

            // Confirm
            DialogResult confirm = MessageBox.Show( configData.STRConfirmSaveText,configData.STRConfirmSaveTitle, 
                MessageBoxButtons.OKCancel, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1 ) ;

			if ( confirm != DialogResult.OK )
            {
                TRACE( MASK.INFO, "User cancels the operation" ) ;
                TRACE( MASK.API, "--SaveButton_Click()" ) ;
                return ;
            }
            else
            {
                // Deactivated other operations
                this.Enabled = false ;
                this.Refresh() ;
            }
			
            this.SaveFile( srcFileName ) ;            
            
            TRACE( MASK.API, "--SaveButton_Click()" ) ;
            this.Exit( true ) ; // Exit application
        }

        /// <summary>
        /// Save changes to RapNet.xml file
        /// </summary>
        /// <param name="srcFileName"></param>
        private void SaveFile( string srcFileName )
        {
            if ( ( srcFileName == null ) || !File.Exists( srcFileName ) )
            {
                return ;
            }

            TRACE( MASK.API, "++SaveFile()" ) ;

            try
            {
                string bakFileName = srcFileName + ".BAK" ;

                // Backup original file
                TRACE( MASK.INFO, "Backup file \'" +srcFileName + "\' to \'" + bakFileName + "\'" ) ;
                File.Copy( srcFileName, bakFileName, true ) ;

                // Load the original RapNet.xml
                XmlTextReader reader = new XmlTextReader( srcFileName );
                XmlDocument xmlDoc = new XmlDocument(); 
                xmlDoc.Load( reader );
                reader.Close(); //Close xml reader

                //root element
                XmlElement root = xmlDoc.DocumentElement;
            
                // Create new <Lanes> section to replace old <Lanes> section in RapNet.xml
                XmlNode newLanesSection = xmlDoc.CreateNode( XmlNodeType.Element, "Lanes", null ) ;
                TRACE( MASK.INFO, "Create new <Lanes> section:" ) ;
                TRACE( MASK.INFO, "<" + newLanesSection.Name + ">" ) ;
                DataCapture.Indent++ ;
                for ( int i = 0; i < this.numLanes; i++ )
                {
                    XmlNode laneNode = xmlDoc.CreateNode( XmlNodeType.Element, "Lane", null ) ;
                    XmlNode hostNode = xmlDoc.CreateNode( XmlNodeType.Element, "Host", null ) ;
                    hostNode.InnerText = this.laneButtonArray[i].Text ;
                
                    laneNode.AppendChild( hostNode ) ;
                    newLanesSection.AppendChild( laneNode ) ;

                    // Tracing
                    TRACE( MASK.INFO, "<" + laneNode.Name + ">" ) ;
                    DataCapture.Indent++; 
                    TRACE( MASK.INFO, "<" + hostNode.Name + ">" + hostNode.InnerText + "</" + hostNode.Name + ">" ) ;
                    DataCapture.Indent--;
                    TRACE( MASK.INFO, "</" + laneNode.Name + ">" ) ;

                }
                DataCapture.Indent--;
                TRACE( MASK.INFO, "</" + newLanesSection.Name + ">" ) ;

                // Replace old Lanes section with new Lanes section
                bool bReplace = false ;
                XmlNodeList oldLaneSectionList = xmlDoc.GetElementsByTagName( "Lanes" ) ;
                foreach ( XmlNode oldLanesSection in oldLaneSectionList )
                {
                    TRACE( MASK.INFO, "Replace old <Lanes> section in RapNet.xml with new <Lanes> section" ) ;
                    root.ReplaceChild( newLanesSection, oldLanesSection ) ;
                    bReplace = true ;
                    break ;
                }

                // If RapNet.xml does not have <Lanes> section, 
                // insert new <Lanes> section into RapNet.xml
                if ( !bReplace )
                {
                    TRACE( MASK.INFO, "Insert new <Lanes> section into RapNet.xml" ) ;
                    root.AppendChild( newLanesSection ) ;
                }

                // Save file
                TRACE( MASK.INFO, "Save change(s) to " + srcFileName ) ;
                xmlDoc.Save( srcFileName ) ;
                xmlDoc.RemoveAll() ;

                // For MC50 only
                if ( this.deviceModel != HHRAPMODEL.MC50 )
                {
                    return ;
                }

                // This section for HHRAP MC50 only
                TRACE( MASK.INFO, "For HHRAP MC50 only:" ) ;
                string toDirectoryPath = this.nonVolatilePath ;
                if ( this.loadingPath == this.nonVolatilePath )
                {
                    toDirectoryPath = this.configPath ;
                }

                // Check for directory
                if ( !Directory.Exists( toDirectoryPath ) )
                {
                    TRACE( MASK.INFO, toDirectoryPath + " does not exist" ) ;
                    return ;
                }

                // Backup file
                string toFileName = toDirectoryPath + "\\RapNet.xml" ;
                bakFileName = toFileName + ".BAK" ;
                if ( File.Exists( toFileName ) )
                {
                    TRACE( MASK.INFO, "Backup file \'" + toFileName + "\' to \'" + bakFileName + "\'" ) ; 
                    File.Copy( toFileName, bakFileName, true ) ;
                }

                // Copy RapNet.xml from current directory to destination directory
                TRACE( MASK.INFO, "Copy \'" + srcFileName + "\' to \'" + toFileName + "\'" ) ;
                File.Copy( srcFileName, toFileName, true ) ;
            }
            catch (Exception e)
            {
                TRACE( MASK.ERROR, e.ToString() ) ;
            }
            finally
            {
                TRACE( MASK.API, "--SaveFile()" ) ;
            }
        }

        /// <summary>
        /// SetModify
        /// </summary>
        /// <param name="bModify"></param>
        public void SetModify( bool bModify )
        {
            TRACE( MASK.API, "++SetModify()" ) ;
            TRACE( MASK.INFO, "bModify = " + bModify.ToString() ) ;

            this.bModify = bModify ;
            this.saveButton.Enabled = bModify ;

            TRACE( MASK.API, "--SetModify()" ) ;
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
#if ( !WINCE ) 
			this.BackColor = System.Drawing.Color.FromArgb(215,215,215);
            try
            {
				if(800 == this.DISPLAY_WIDTH)
					this.BackgroundImage = System.Drawing.Image.FromFile("c:\\scot\\image\\8x6_Background.png", true);
				else
					this.BackgroundImage = System.Drawing.Image.FromFile("c:\\scot\\image\\10x7_Background.png", true);
            }
            catch (IOException e)
            {
                TRACE(MASK.ERROR, e.ToString());
            }
#endif
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None ;
            this.Location = new System.Drawing.Point( 0, 0 ) ;
            this.Size = new System.Drawing.Size( configData.Width, configData.Height ) ;
            this.ClientSize = new System.Drawing.Size( configData.Width, configData.Height ) ;

            try
            {
#if ( WINCE )
                // Set this window to full screen
                IntPtr hMainWnd = FindWindow( null, this.Text ) ;
                if (hMainWnd != IntPtr.Zero)
                {
                    SHFullScreen( hMainWnd, bFullScreen ? SHFS_HIDEALL : SHFS_SHOWALL ) ;
                }

                // Hide Window Task Bar
                IntPtr hWnd = FindWindow( "HHTaskBar", null );
                if (hWnd != IntPtr.Zero)
                {
                    ShowWindow( hWnd, bFullScreen ? (int)ShowWindowFlags.SW_HIDE : (int) ShowWindowFlags.SW_SHOW ) ;
                }
#endif
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
        /// UpdateLaneInfo: sync lane info array with current lanes
        /// </summary>
        public void UpdateLaneInfoArray()
        {
            TRACE( MASK.EXTENSIVE, "++UpdateLaneInfoArray()" ) ;
            try
            {
                if ( ( this.laneButtonArray == null ) || ( this.laneButtonArray.Length == 0 ) )
                {
                    TRACE( MASK.EXTENSIVE, "--UpdateLaneInfoArray()" ) ;
                    return  ;
                }

                // Reset
                if ( this.laneInfoArray != null )
                {
                    this.laneInfoArray.Initialize() ;
                }

                // Set laneInfoArray from actual lane buttons
                this.laneInfoArray = new string[this.laneButtonArray.Length] ;
                for ( int i = 0; i < this.laneButtonArray.Length; i++ )
                {
                    this.laneInfoArray[i] = this.laneButtonArray[i].Text ;
                }
            }
            catch ( Exception e )
            {
                TRACE( MASK.ERROR, e.ToString() ) ;
            }
            finally
            {
                TRACE( MASK.EXTENSIVE, "--UpdateLaneInfoArray()" ) ;
            }
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
        /// RapNet
        /// </summary>
        private const string regRapNetPath = "SOFTWARE\\NCR\\RapNet" ;
        private const string regRapConfigurePath = "SOFTWARE\\NCR\\RapConfigure" ;

#if( WINCE )
        private const string regApiDll = "coredll.dll" ;
#else
        private const string regApiDll = "advapi32.dll" ;
#endif
        
        [DllImport(regApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern IntPtr FindWindow( string lpClassName, string lpWindowName );
        public const uint SHFS_HIDETASKBAR	 = 0x0002;
        public const uint SHFS_HIDESIPBUTTON = 0x0008;
        public const uint SHFS_HIDESTARTICON = 0x00020;
        public const uint SHFS_SHOWTASKBAR   = 0x0001;
        public const uint SHFS_SHOWSIPBUTTON = 0x0004;
        public const uint SHFS_SHOWSTARTICON = 0x0010;
        public const uint SHFS_HIDEALL       =  SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON;
        public const uint SHFS_SHOWALL       =  SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON | SHFS_SHOWSTARTICON;				

        [DllImport( "aygshell.dll" ) ]
        public static extern void SHFullScreen( IntPtr hWnd, uint settings );

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
        [DllImport(regApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern bool ShowWindow(IntPtr hWnd, int nCmdShow ) ;

        [DllImport(regApiDll, CharSet=CharSet.Unicode, SetLastError=false)]
        public static extern bool EnableWindow( IntPtr hWnd, bool bEnabled ) ;
    }
}
