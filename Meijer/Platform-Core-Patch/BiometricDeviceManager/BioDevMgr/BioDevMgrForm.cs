using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Diagnostics;
using RPSWNET;

namespace BioDevMgr
{
	/// <summary>
    /// Summary description for BioDevMgrForm.
	/// </summary>
	public class BioDevMgrForm : System.Windows.Forms.Form
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private TraceHelper trace = new TraceHelper("BioDevMgr");
        private DataGridView pluginGrid;
        private DataGridViewTextBoxColumn columnPlugin;
        private DataGridViewTextBoxColumn columnPluginType;
        private DataGridViewTextBoxColumn columnStatus;
        private DataGridViewTextBoxColumn columnActivity;

		private BioDevMgr bioDevMgr = null;

		public BioDevMgrForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(BioDevMgrForm));
            this.pluginGrid = new System.Windows.Forms.DataGridView();
            this.columnPlugin = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.columnPluginType = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.columnStatus = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.columnActivity = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.pluginGrid)).BeginInit();
            this.SuspendLayout();
            // 
            // pluginGrid
            // 
            this.pluginGrid.AllowUserToAddRows = false;
            this.pluginGrid.AllowUserToDeleteRows = false;
            this.pluginGrid.AllowUserToOrderColumns = true;
            this.pluginGrid.AllowUserToResizeRows = false;
            this.pluginGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pluginGrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.pluginGrid.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.columnPlugin,
            this.columnPluginType,
            this.columnStatus,
            this.columnActivity});
            this.pluginGrid.Location = new System.Drawing.Point(12, 12);
            this.pluginGrid.Name = "pluginGrid";
            this.pluginGrid.RowHeadersVisible = false;
            this.pluginGrid.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.pluginGrid.Size = new System.Drawing.Size(370, 151);
            this.pluginGrid.TabIndex = 0;
            // 
            // columnPlugin
            // 
            this.columnPlugin.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.columnPlugin.HeaderText = "Plugin";
            this.columnPlugin.Name = "columnPlugin";
            // 
            // columnPluginType
            // 
            this.columnPluginType.HeaderText = "Type";
            this.columnPluginType.Name = "columnPluginType";
            this.columnPluginType.Width = 70;
            // 
            // columnStatus
            // 
            this.columnStatus.HeaderText = "Status";
            this.columnStatus.Name = "columnStatus";
            this.columnStatus.Width = 70;
            // 
            // columnActivity
            // 
            this.columnActivity.HeaderText = "";
            this.columnActivity.Name = "columnActivity";
            this.columnActivity.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.columnActivity.Width = 30;
            // 
            // BioDevMgrForm
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(394, 175);
            this.Controls.Add(this.pluginGrid);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimumSize = new System.Drawing.Size(400, 200);
            this.Name = "BioDevMgrForm";
            this.Text = "Biometric Device Manager";
            this.Load += new System.EventHandler(this.BioDevMgrForm_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.BioDevMgrForm_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.pluginGrid)).EndInit();
            this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
            string procName = System.IO.Path.GetFileNameWithoutExtension(Application.ExecutablePath);
            
            Process[] processes = Process.GetProcessesByName(procName);
            
            if (processes.Length > 1)
            {
                // There is already another instance of the application running;
                Application.Exit();
            }
            else
            {
                ConfigData configData = ConfigData.GetConfigData();

                CmDataCapture.ReadRegistry(configData.RegPath + "\\", "DataCapture");
                CmDataCapture.Initialize("-- BioDevMgr Start Tracing --");

                Application.Run(new BioDevMgrForm());

                CmDataCapture.Flush();
                CmDataCapture.Cleanup();
            }
		}

		private void BioDevMgrForm_Load(object sender, System.EventArgs e)
		{
			bioDevMgr = new BioDevMgr(this);
			bioDevMgr.Initialize();		
		}

        private void BioDevMgrForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            bioDevMgr.Uninitialize();
            bioDevMgr = null;
        }

        public void UpdatePluginRow(string pluginClass, string pluginType, string pluginStatus, string pluginActivity)
        {
            DataGridViewRow pluginRow = null;
            foreach (DataGridViewRow row in pluginGrid.Rows)
            {
                // find a match by class
                if( (row.Cells[columnPlugin.Name] as DataGridViewTextBoxCell).Value.ToString() == pluginClass)
                {
                    pluginRow = row;
                    break;
                }
            }
            if (pluginRow == null)
            {
                // row doesn't exist, add it.
                int i = pluginGrid.Rows.Add();
                pluginRow = pluginGrid.Rows[i];
                (pluginRow.Cells[columnPlugin.Name] as DataGridViewTextBoxCell).Value = pluginClass;
            }
            
            (pluginRow.Cells[columnPluginType.Name] as DataGridViewTextBoxCell).Value = pluginType;
            (pluginRow.Cells[columnStatus.Name] as DataGridViewTextBoxCell).Value = pluginStatus;
            pluginRow.Cells[columnActivity.Name].Style.BackColor = 
                pluginActivity == "Idle" ? Color.White : Color.Green;
        }
	}


}
