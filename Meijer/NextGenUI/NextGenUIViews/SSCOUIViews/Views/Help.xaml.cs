using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using SSCOUIModels.Controls;
using SSCOUIModels.Models;
using SSCOUIModels;
using SSCOUIModels.Helpers;

namespace SSCOUIViews.Views
{
    /// <summary>
    /// Interaction logic for YesNo.xaml
    /// </summary>
    public partial class Help : PopupView
    {
        public Help(IMainViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
        }

        /// <summary>
        /// OnStateParamChanged that accepts param that is set in app.config
        /// </summary>
        /// <param name="param">String type of param.</param>
        public override void OnStateParamChanged(string param)
        {
            UpdateActiveScannerButton();
            ActivateHHScannerButton.CommandParameter = "ActivateHHScanner";
            this.HelpOnWay.Visibility = param.Equals("HelpOnWay") ? Visibility.Visible : Visibility.Collapsed;
            this.CallForHelp.Visibility = param.Equals("ContextHelp") ? Visibility.Visible : Visibility.Collapsed;
            this.ActivateHHScannerButton.Visibility = param.Equals("ContextHelp") ? Visibility.Visible : Visibility.Collapsed;
            this.OkButton.Visibility = param.Equals("ContextHelp") || param.Equals("HelpOnWay") ? Visibility.Visible : Visibility.Collapsed;
            this.StoreLoginButton.Visibility = this.CancelButton.Visibility = param.Equals("ContextHelp") ? Visibility.Visible : Visibility.Collapsed;
            FocusHelper.SetInitialFocus(this, viewModel);
        }
        // +SSCOI-45264: ActivateHandScanner Feature
        private void UpdateActiveScannerButton()
        {
            if (this.viewModel.StateParam.Equals("ContextHelp"))
            {
                ActivateHHScannerButton.IsEnabled = GetPropertyBoolValue("CMButton4MidiListEnabled");
            }
        }// -SSCOI-45264: ActivateHandScanner Feature
        public override void OnPropertyChanged(string name, object value)
        {
            if (name.Equals("CMButton4MidiListEnabled"))
            {
                UpdateActiveScannerButton(); // SSCOI-45264: ActivateHandScanner Feature
            }
        }
    }
}
