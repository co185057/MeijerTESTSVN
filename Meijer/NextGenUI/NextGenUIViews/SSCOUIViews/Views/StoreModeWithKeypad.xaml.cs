// <copyright file="StoreModeWithKeypad.xaml.cs" company="NCR">
//     Copyright 2016 NCR. All rights reserved.
// </copyright>

namespace SSCOUIViews.Views
{
    using System;
    using System.Collections.Generic;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Input;
    using SSCOUIModels.Controls;
    using SSCOUIModels;
    using SSCOControls;
    using System.ComponentModel;
    using SSCOUIModels.Helpers;
    using System.Windows.Data;

    /// <summary>
    /// Interaction logic for StoreModeWithKeypad.xaml
    /// </summary>
    public partial class StoreModeWithKeypad : BackgroundView
    {
        /// <summary>
        /// Initializes a new instance of the StoreModeWithKeypad class.
        /// </summary>
        /// <param name="viewModel">ViewModel object.</param>
        public StoreModeWithKeypad(IMainViewModel viewModel)
            : base(viewModel)
        {           
            InitializeComponent();
        }

        /// <summary>
        /// Accepts a parameter change as configured in app.config.
        /// </summary>
        /// <param name="param">Changed parameter.</param>
        public override void OnStateParamChanged(string param)
        {
            this.UpdateSMButton1Shown();
            this.UpdateSMButton2Shown();
            this.UpdateSMButton3Shown();
            this.UpdateSMButton4Shown();
            this.UpdateSMButton5Shown();
            this.UpdateSMButton6Shown();
            this.UpdateSMButton7Shown();
            this.UpdateSMButton1Enabled();
            this.UpdateSMButton2Enabled();
            this.UpdateSMButton3Enabled();
            this.UpdateSMButton4Enabled();
            this.UpdateSMButton5Enabled();
            this.UpdateSMButton6Enabled();
            this.UpdateSMButton7Enabled();
            this.UpdateSMButton1Text();
            this.UpdateSMButton2Text();
            this.UpdateSMButton3Text();
            this.UpdateSMButton4Text();
            this.UpdateSMButton5Text();
            this.UpdateSMButton6Text();
            this.UpdateSMButton7Text();
            this.UpdateSMLineText();
            this.UpdateSMLineShown();
        }

        /// <summary>
        /// Accepts a property change as configured in app.config.
        /// </summary>
        /// <param name="name">Name of changed property.</param>
        /// <param name="value">Value of changed property.</param>
        public override void OnPropertyChanged(string name, object value)
        {
            switch (name)
            {
                case "SMButton1Shown":
                    this.UpdateSMButton1Shown();
                    break;
                case "SMButton2Shown":
                    this.UpdateSMButton2Shown(); 
                    break;
                case "SMButton3Shown":
                    this.UpdateSMButton3Shown();
                    break;
                case "SMButton4Shown":
                    this.UpdateSMButton4Shown();
                    break;
                case "SMButton5Shown":
                    this.UpdateSMButton5Shown();
                    break;
                case "SMButton6Shown":
                    this.UpdateSMButton6Shown();
                    break;
                case "SMButton7Shown":
                    this.UpdateSMButton7Shown();
                    break;
                case "SMButton1Text":
                    this.UpdateSMButton1Text();
                    break;
                case "SMButton2Text":
                    this.UpdateSMButton2Text();
                    break;
                case "SMButton3Text":
                    this.UpdateSMButton3Text();
                    break;
                case "SMButton4Text":
                    this.UpdateSMButton4Text();
                    break;
                case "SMButton5Text":
                    this.UpdateSMButton5Text();
                    break;
                case "SMButton6Text":
                    this.UpdateSMButton6Text();
                    break;
                case "SMButton7Text":
                    this.UpdateSMButton7Text();
                    break;
                case "SMButton1Enabled":
                    this.UpdateSMButton1Enabled();
                    break;
                case "SMButton2Enabled":
                    this.UpdateSMButton2Enabled();
                    break;
                case "SMButton3Enabled":
                    this.UpdateSMButton3Enabled();
                    break;
                case "SMButton4Enabled":
                    this.UpdateSMButton4Enabled();
                    break;
                case "SMButton5Enabled":
                    this.UpdateSMButton5Enabled();
                    break;
                case "SMButton6Enabled":
                    this.UpdateSMButton6Enabled();
                    break;
                case "SMButton7Enabled":
                    this.UpdateSMButton7Enabled();
                    break;
                case "SMLeadthruText":
                case "SMLeadthruTextShown":                 
                case "LeadthruText2Text":
                case "LeadthruText2TextShown":
                case "LeadthruText":
                case "LeadthruTextShown":                    
                case "MessageBoxNoEcho":                
                case "MessageBoxNoEchoShown":
				case "SMPromptText": // SSCOI-47005
                    this.UpdateSMLineText();
                    this.UpdateSMLineShown();
                    break;                    
            }           
        }

        /// <summary>
        /// Shows/hides SMButton1
        /// </summary>
        private void UpdateSMButton1Shown()
        {
            StoreButton1.Visibility = GetPropertyBoolValue("SMButton1Shown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Shows/hides SMButton2
        /// </summary>
        private void UpdateSMButton2Shown()
        {
            StoreButton2.Visibility = GetPropertyBoolValue("SMButton2Shown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Shows/hides SMButton3
        /// </summary>
        private void UpdateSMButton3Shown()
        {
            StoreButton3.Visibility = GetPropertyBoolValue("SMButton3Shown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Shows/hides SMButton4
        /// </summary>
        private void UpdateSMButton4Shown()
        {
            StoreButton4.Visibility = GetPropertyBoolValue("SMButton4Shown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Shows/hides SMButton5
        /// </summary>
        private void UpdateSMButton5Shown()
        {
            StoreButton5.Visibility = GetPropertyBoolValue("SMButton5Shown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Shows/hides SMButton6
        /// </summary>
        private void UpdateSMButton6Shown()
        {
            StoreButton6.Visibility = GetPropertyBoolValue("SMButton6Shown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Shows/hides SMButton7
        /// </summary>
        private void UpdateSMButton7Shown()
        {
            StoreButton7.Visibility = GetPropertyBoolValue("SMButton7Shown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates SMButton1 text
        /// </summary>
        private void UpdateSMButton1Text()
        {
            this.StoreButton1.Text = this.GetPropertyStringValue("SMButton1Text");
        }

        /// <summary>
        /// Updates SMButton2 text
        /// </summary>
        private void UpdateSMButton2Text()
        {
            this.StoreButton2.Text = this.GetPropertyStringValue("SMButton2Text");
        }

        /// <summary>
        /// Updates SMButton3 text
        /// </summary>
        private void UpdateSMButton3Text()
        {
            this.StoreButton3.Text = this.GetPropertyStringValue("SMButton3Text");
        }

        /// <summary>
        /// Updates SMButton4 text
        /// </summary>
        private void UpdateSMButton4Text()
        {
            this.StoreButton4.Text = this.GetPropertyStringValue("SMButton4Text");
        }

        /// <summary>
        /// Updates SMButton5 text
        /// </summary>
        private void UpdateSMButton5Text()
        {
            this.StoreButton5.Text = this.GetPropertyStringValue("SMButton5Text");
        }

        /// <summary>
        /// Updates SMButton6 text
        /// </summary>
        private void UpdateSMButton6Text()
        {
            this.StoreButton6.Text = this.GetPropertyStringValue("SMButton6Text");
        }

        /// <summary>
        /// Updates SMButton7 text
        /// </summary>
        private void UpdateSMButton7Text()
        {
            this.StoreButton7.Text = this.GetPropertyStringValue("SMButton7Text");
        }

        /// <summary>
        /// Enables/disables SMButton1
        /// </summary>
        private void UpdateSMButton1Enabled()
        {
            this.StoreButton1.IsEnabled = this.GetPropertyBoolValue("SMButton1Enabled");
        }

        /// <summary>
        /// Enables/disables SMButton2
        /// </summary>
        private void UpdateSMButton2Enabled()
        {
            this.StoreButton2.IsEnabled = this.GetPropertyBoolValue("SMButton2Enabled");
        }

        /// <summary>
        /// Enables/disables SMButton3
        /// </summary>
        private void UpdateSMButton3Enabled()
        {
            this.StoreButton3.IsEnabled = this.GetPropertyBoolValue("SMButton3Enabled");
        }

        /// <summary>
        /// Enables/disables SMButton4
        /// </summary>
        private void UpdateSMButton4Enabled()
        {
            this.StoreButton4.IsEnabled = this.GetPropertyBoolValue("SMButton4Enabled");
        }

        /// <summary>
        /// Enables/disables SMButton5
        /// </summary>
        private void UpdateSMButton5Enabled()
        {
            this.StoreButton5.IsEnabled = this.GetPropertyBoolValue("SMButton5Enabled");
        }

        /// <summary>
        /// Enables/disables SMButton6
        /// </summary>
        private void UpdateSMButton6Enabled()
        {
            this.StoreButton6.IsEnabled = this.GetPropertyBoolValue("SMButton6Enabled");
        }

        /// <summary>
        /// Enables/disables SMButton7
        /// </summary>
        private void UpdateSMButton7Enabled()
        {
            this.StoreButton7.IsEnabled = this.GetPropertyBoolValue("SMButton7Enabled");
        }

        /// <summary>
        /// Shows/hides SMLineText control
        /// </summary>
        private void UpdateSMLineShown()
        {
            this.SMLineText.Visibility = !(this.SMLineText.Text.Equals(String.Empty)) ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates SMLineText text
        /// </summary>
        private void UpdateSMLineText()
        {
            string line1 = string.Empty;
            string line2 = string.Empty;
            string line3 = string.Empty;
			string line4 = string.Empty;  // SSCOI-47005
            if ((this.GetPropertyBoolValue("SMLeadthruTextShown") && this.GetPropertyStringValue("SMLeadthruText").Length > 0))
            {
                line1 = this.GetPropertyStringValue("SMLeadthruText");
            }
            if ((this.GetPropertyBoolValue("LeadthruText2TextShown") && this.GetPropertyStringValue("LeadthruText2Text").Length > 0))
            {
                line2 = this.GetPropertyStringValue("LeadthruText2Text");
            }
            else if ((this.GetPropertyBoolValue("LeadthruTextShown") && this.GetPropertyStringValue("LeadthruText").Length > 0))
            {
                line2 = this.GetPropertyStringValue("LeadthruText");
            }
            if ((this.GetPropertyBoolValue("MessageBoxNoEchoShown") && this.GetPropertyStringValue("MessageBoxNoEcho").Length > 0))
            {
                line3 = this.GetPropertyStringValue("MessageBoxNoEcho");
            }
			// +SSCOI-47005
			if ((this.GetPropertyBoolValue("SMPromptTextShown") && this.GetPropertyStringValue("SMPromptText").Length > 0))
            {
                line4 = this.GetPropertyStringValue("SMPromptText");
            }
			// -SSCOI-47005
            this.SMLineText.Text = UIFormat.MultiLineFormat(new string[] { line1, line2, line3, line4 });
        }
        
    }
}
