using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace ACSHookClient
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
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

				if( acsHooksConnected )
				{
					try
					{
						acsHooks.Disconnect();

						acsHooks = null;
						System.GC.Collect();
					}
					catch(Exception)
					{
						// nothing to do, we're dying anyway
					}
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
			this.btnMessages = new System.Windows.Forms.Button();
			this.btnConnect = new System.Windows.Forms.Button();
			this.cbInput = new System.Windows.Forms.ComboBox();
			this.btnInput = new System.Windows.Forms.Button();
			this.btnProp = new System.Windows.Forms.Button();
			this.cbProp = new System.Windows.Forms.ComboBox();
			this.txtProp = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// btnMessages
			// 
			this.btnMessages.Location = new System.Drawing.Point(264, 8);
			this.btnMessages.Name = "btnMessages";
			this.btnMessages.TabIndex = 0;
			this.btnMessages.Text = "&Messages";
			this.btnMessages.Click += new System.EventHandler(this.btnMessages_Click);
			// 
			// btnConnect
			// 
			this.btnConnect.Location = new System.Drawing.Point(8, 8);
			this.btnConnect.Name = "btnConnect";
			this.btnConnect.TabIndex = 1;
			this.btnConnect.Text = "&Connect";
			this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
			// 
			// cbInput
			// 
			this.cbInput.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbInput.Location = new System.Drawing.Point(8, 64);
			this.cbInput.Name = "cbInput";
			this.cbInput.Size = new System.Drawing.Size(121, 21);
			this.cbInput.TabIndex = 2;
			// 
			// btnInput
			// 
			this.btnInput.Location = new System.Drawing.Point(136, 64);
			this.btnInput.Name = "btnInput";
			this.btnInput.TabIndex = 3;
			this.btnInput.Text = "Send &Input";
			this.btnInput.Click += new System.EventHandler(this.btnInput_Click);
			// 
			// btnProp
			// 
			this.btnProp.Location = new System.Drawing.Point(248, 104);
			this.btnProp.Name = "btnProp";
			this.btnProp.TabIndex = 5;
			this.btnProp.Text = "Set &Property";
			this.btnProp.Click += new System.EventHandler(this.btnProp_Click);
			// 
			// cbProp
			// 
			this.cbProp.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbProp.Location = new System.Drawing.Point(8, 104);
			this.cbProp.Name = "cbProp";
			this.cbProp.Size = new System.Drawing.Size(121, 21);
			this.cbProp.TabIndex = 4;
			this.cbProp.SelectedIndexChanged += new System.EventHandler(this.cbProp_SelectedIndexChanged);
			// 
			// txtProp
			// 
			this.txtProp.Location = new System.Drawing.Point(136, 104);
			this.txtProp.Name = "txtProp";
			this.txtProp.TabIndex = 6;
			this.txtProp.Text = "";
			// 
			// MainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(344, 181);
			this.Controls.Add(this.txtProp);
			this.Controls.Add(this.btnProp);
			this.Controls.Add(this.cbProp);
			this.Controls.Add(this.btnInput);
			this.Controls.Add(this.cbInput);
			this.Controls.Add(this.btnConnect);
			this.Controls.Add(this.btnMessages);
			this.Name = "MainForm";
			this.Text = "ACSHook Test Client";
			this.Load += new System.EventHandler(this.MainForm_Load);
			this.ResumeLayout(false);

		}
		#endregion

		private System.Windows.Forms.Button btnMessages;
		private System.Windows.Forms.Button btnConnect;
		private System.Windows.Forms.ComboBox cbInput;
		private System.Windows.Forms.Button btnInput;
		private System.Windows.Forms.Button btnProp;
		private System.Windows.Forms.ComboBox cbProp;
		private System.Windows.Forms.TextBox txtProp;


		public enum ACSHookEventTypes
		{
			OnConnectEvent = 1,
			OnDisconnectEvent = 2,
			OnGenMsgEvent = 3,
			OnTraceEvent = 4
		};

		ACSHookManager.CACSHook acsHooks = null;
		
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new MainForm());
		}


		protected MessageForm msgForm = new MessageForm();

		private void MainForm_Load(object sender, System.EventArgs e)
		{
			try
			{
				acsHooks = new ACSHookManager.CACSHookClass();
				acsHooks.OnEvent += new ACSHookManager.IACSHookEvents_OnEventEventHandler(this.acsHooks_OnEvent);
			}
			catch(Exception ex)
			{
				msgForm.Add(ex.Message);
			}
		}
		
		protected void acsHooks_OnEvent(int iType, int iMsg, string strParam1, string strParam2)
		{
			ACSHookEventTypes et = (ACSHookEventTypes)iType;
			switch(et)
			{
				case ACSHookEventTypes.OnConnectEvent:
					msgForm.Add("[OnConnectEvent] " + iMsg.ToString() + " " + strParam1 + " " + strParam2);
					SetConnectState(true);

					break;

				case ACSHookEventTypes.OnDisconnectEvent:
					msgForm.Add("[OnDisconnectEvent] " + iMsg.ToString() + " " + strParam1 + " " + strParam2);
					SetConnectState(false);

					break;

				case ACSHookEventTypes.OnGenMsgEvent:
					msgForm.Add("[OnGenMsgEvent] " + iMsg.ToString() + " " + strParam1 + " " + strParam2);

					break;

				case ACSHookEventTypes.OnTraceEvent:
					msgForm.Add("[OnReceiveEvent] " + iMsg.ToString() + " " + strParam1 + " " + strParam2);

					break;

				default:
					msgForm.Add("[Undefined event:" + iType.ToString() + "] " + iMsg.ToString() + " " + strParam1 + " " + strParam2);
					break;
			};
		}

		private void btnMessages_Click(object sender, System.EventArgs e)
		{
			msgForm.Show();
		}

		protected bool acsHooksConnected = false;
		void SetConnectState(bool bConnected)
		{
			acsHooksConnected = bConnected;
			if( bConnected )
			{
				btnConnect.Text = "Dis&connect";
			}
			else 
			{
				btnConnect.Text = "&Connect";
			}
				
		}

		private void btnConnect_Click(object sender, System.EventArgs e)
		{
			try
			{
				if( !acsHooksConnected )
				{
					msgForm.Add("Connecting to ACS server.");
					acsHooks.Connect();
					msgForm.Add("Connection completed successfully.");
					SetConnectState(true);

					cbProp.Items.AddRange((object[])acsHooks.Properties);
					cbInput.Items.AddRange((object[])acsHooks.Inputs);
				}
				else
				{
					msgForm.Add("Disconnecting from ACS server.");
					acsHooks.Disconnect();
					msgForm.Add("Disconnection completed successfully.");
					SetConnectState(false);
					cbProp.Items.Clear();
					cbInput.Items.Clear();
				}
			}
			catch( Exception ex)
			{
				msgForm.Add("Error while connecting/disconnecting to ACS server: " + ex.Message);
			}
		}

		private void btnInput_Click(object sender, System.EventArgs e)
		{
			acsHooks.SendInput(cbInput.Text);
		}

		private void cbProp_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			txtProp.Text = acsHooks.GetProperty(cbProp.Text);
		}

		private void btnProp_Click(object sender, System.EventArgs e)
		{
			acsHooks.SetProperty(cbProp.Text, txtProp.Text);
		}
	}
}
