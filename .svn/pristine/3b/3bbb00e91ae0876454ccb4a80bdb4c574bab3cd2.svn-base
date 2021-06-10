using System;
using System.Windows;
using SSCOControls;
using SSCOUIModels;
using SSCOUIModels.Controls;
using FPsxWPF.Controls;
using SSCOUIModels.Helpers;
using System.Collections.Specialized;
using System.Collections;
using System.Windows.Controls;
using System.Windows.Data;

namespace SSCOUIViews.Views
{
    /// <summary>
    /// Interaction logic for StoreModeFullAssist.xaml
    /// </summary>
    public partial class StoreModeFullAssist : BackgroundView
    {
        public StoreModeFullAssist(IMainViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
        }

        public override void OnStateParamChanged(string param)
        {
            this.UpdateInputBox1();
            this.UpdateSMLineText();
            this.UpdateSMLineShown();
        }

        public override void OnPropertyChanged(string name, object value)
        {
            switch (name)
            {
                case "MessageBoxEcho":
                    this.UpdateInputBox1();
                    break;
                case "LeadthruTextShown":
                case "LeadthruText":
                case "LeadthruText2TextShown":
                case "LeadthruText2Text":
                    this.UpdateSMLineText();
                    this.UpdateSMLineShown();
                    break;
                case "NextGenData":
                    UpdateButtonList6();
                    break;                    
            }
        }

        private void UpdateInputBox1()
        {
            this.FullAssistInput1.Text = this.viewModel.GetPropertyValue("MessageBoxEcho").ToString();
        }

        /*
		 * Remove code below, it is not used in this current solution
		private void UpdateInputBox2()
        {
            this.FullAssistInput2.Text = this.viewModel.GetPropertyValue("MessageBoxEcho1").ToString();
        }*/

        private void AssistKeyboard_Click(object sender, RoutedEventArgs e)
        {
            var button = sender as ImageButton;
            if (null != button)
            {
                GridItem item = button.DataContext as GridItem;
                if (null != item)
                {
                    this.viewModel.ActionCommand.Execute(string.Format("{0}Click({1})", button.Name, item.Data));
                }
            }
        }

        private void UpdateSMLineShown()
        {
			// SSCOI-46487: Don't set the visibility of the SMLineText if the string is empty. Always display the control.
            //this.SMLineText.Visibility = !(this.SMLineText.Text.Equals(String.Empty)) ? Visibility.Visible : Visibility.Collapsed;
        }

        private void UpdateSMLineText()
        {
            string line1 = string.Empty;
            string line2 = string.Empty;
            if ((this.GetPropertyBoolValue("LeadthruTextShown") && this.GetPropertyStringValue("LeadthruText").Length > 0))
            {
                line1 = this.GetPropertyStringValue("LeadthruText");
            }
            if ((this.GetPropertyBoolValue("LeadthruText2TextShown") && this.GetPropertyStringValue("LeadthruText2Text").Length > 0))
            {
                line2 = this.GetPropertyStringValue("LeadthruText2Text");
            }
            this.SMLineText.Text = UIFormat.MultiLineFormat(new string[] { line1, line2 });
        }		

        private void UpdateButtonList6()
        {
            var nextGenData = GetPropertyStringValue("NextGenData");
            if (!string.IsNullOrEmpty(nextGenData) && nextGenData.Equals("NextGenUIDynaKeysUpdateComplete"))
            {                
                this.ButtonList6.Items.Refresh();
            }
        }		
    }
}