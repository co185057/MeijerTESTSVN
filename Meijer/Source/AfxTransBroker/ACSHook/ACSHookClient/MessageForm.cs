using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace ACSHookClient
{
	/// <summary>
	/// Summary description for MessageForm.
	/// </summary>
	public class MessageForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button btnClear;
		private System.Windows.Forms.ListBox lstMessages;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public MessageForm()
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
				if(components != null)
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
			this.lstMessages = new System.Windows.Forms.ListBox();
			this.btnClear = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// lstMessages
			// 
			this.lstMessages.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.lstMessages.Location = new System.Drawing.Point(8, 8);
			this.lstMessages.Name = "lstMessages";
			this.lstMessages.Size = new System.Drawing.Size(656, 212);
			this.lstMessages.TabIndex = 0;
			// 
			// btnClear
			// 
			this.btnClear.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnClear.Location = new System.Drawing.Point(592, 232);
			this.btnClear.Name = "btnClear";
			this.btnClear.TabIndex = 1;
			this.btnClear.Text = "&Clear";
			this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
			// 
			// MessageForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(672, 261);
			this.Controls.Add(this.btnClear);
			this.Controls.Add(this.lstMessages);
			this.Name = "MessageForm";
			this.Text = "Message Form";
			this.Closing += new System.ComponentModel.CancelEventHandler(this.MessageForm_Closing);
			this.Load += new System.EventHandler(this.MessageForm_Load);
			this.ResumeLayout(false);

		}
		#endregion

		private void MessageForm_Load(object sender, System.EventArgs e)
		{
		
		}

		public void Add(string strMsg)
		{
			DateTime dt = DateTime.Now;
			strMsg = dt.ToLocalTime() + "." + dt.Millisecond.ToString().PadLeft(3,'0') + ": " + strMsg;
			
			
			lstMessages.Items.Add(strMsg);
			lstMessages.SelectedIndex = lstMessages.Items.Count - 1;
		}

		private void btnClear_Click(object sender, System.EventArgs e)
		{
			lstMessages.Items.Clear();
		}

		private void MessageForm_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			Hide();
			e.Cancel = true;
		}

	}
}
