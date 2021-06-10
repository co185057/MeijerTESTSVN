// <copyright file="CustomPopup.xaml.cs" company="NCR">
//     Copyright 2016 NCR. All rights reserved.
// </copyright>

namespace SSCOUIViews.Views
{
    using System;
    using System.Windows;
    using SSCOUIModels;
    using SSCOUIModels.Controls;    
    using SSCOUIModels.Helpers;

    /// <summary>
    /// Interaction logic for CustomPopup.xaml
    /// </summary>
    public partial class CustomPopup : PopupView
    {
        /// <summary>
        /// Initializes a new instance of the CustomPopup class.
        /// </summary>
        /// <param name="viewModel">ViewModel object.</param>
        public CustomPopup(IMainViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
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
                case "ButtonGoBackShown":
                    this.GoBackButton.Visibility = GetPropertyBoolValue("ButtonGoBackShown") ? Visibility.Visible : Visibility.Collapsed;
                    UpdateButtonGoBackForDataEntryContext();
                    break;
                case "LeadthruText":
                    this.LeadthruText.Text = value.ToString();
                    break;
                case "Instructions":
                    this.Instructions.Text = value.ToString();
                    break;
                case "ButtonGoBack":
                    this.GoBackButton.Text = value.ToString();
                    UpdateButtonGoBackForDataEntryContext();
                    break;
                case "UNav":
                    FocusHelper.SetInitialFocus(this, viewModel);
                    break;
                case "CMButton1MidiListShown":
                case "CMButton1MidiListEnabled":
                    UpdateCMButton1MidiList();
                    break;
                case "CMButton2MidiListShown":
                case "CMButton2MidiListEnabled":
                    UpdateCMButton2MidiList();
                    break;
                case "CMButton3MidiListShown":
                case "CMButton3MidiListEnabled":
                    UpdateCMButton3MidiList();
                    break;
                case "CMButton4MidiListShown":
                case "CMButton4MidiListEnabled":
                    UpdateCMButton4MidiList();
                    break;
                case "CMButton2MedShown":
                case "CMButton2MedEnabled":
                    UpdateCMButton2Med();
                    break;
                case "CMButton3MedShown":
                case "CMButton3MedEnabled":
                    UpdateCMButton3Med();
                    break;
            }
        }

        /// <summary>
        /// OnStateParamChanged that accepts param that is set in app.config
        /// </summary>
        /// <param name="param">String type of param.</param>
        public override void OnStateParamChanged(string param)
        {
            this.LeadthruText.Text = GetPropertyStringValue("LeadthruText");
            this.Instructions.Text = GetPropertyStringValue("Instructions");
            this.GoBackButton.Text = GetPropertyStringValue("ButtonGoBack");
			this.List7Button.Visibility = param.Equals("CmDataEntry5Vertical") ? Visibility.Visible : Visibility.Collapsed; //SSCOI-48730
            this.GoBackButton.Visibility = GetPropertyBoolValue("ButtonGoBackShown") ? Visibility.Visible : Visibility.Collapsed;
            UpdateCMButton1MidiList();
            UpdateCMButton2MidiList();
            UpdateCMButton3MidiList();
            UpdateCMButton4MidiList();
            UpdateCMButton2Med();
            UpdateCMButton3Med();
            UpdateButtonGoBackForDataEntryContext();

            FocusHelper.SetInitialFocus(this, viewModel);
        }

        /// <summary>
        /// Updates the List1Button visibility and state based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateCMButton1MidiList()
        {
            switch (this.viewModel.StateParam)
            {
                case "CmDataEntry":
                case "CmDataEntry1":
                case "CmDataEntry2":
                case "CmDataEntry3":
                case "CmDataEntry4":
                case "SelectContainer":
                case "SelectContainerWith3TareA":
                    this.List1Button.Visibility = GetPropertyBoolValue("CMButton1MidiListShown") ? Visibility.Visible : Visibility.Collapsed;
                    break;
                default:
                    List1Button.Visibility = Visibility.Collapsed;
                    break;
            }                
        }

        /// <summary>
        /// Updates the List2Button visibility and state based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateCMButton2MidiList()
        {
            switch (this.viewModel.StateParam)
            {
                case "CmDataEntry":
                case "CmDataEntry2":
                case "CmDataEntry3":
                case "CmDataEntry4":
                case "SelectContainer":
                case "SelectContainerWith3TareA":
                case "SelectContainerWith3TareB":
                    this.List2Button.Visibility = GetPropertyBoolValue("CMButton2MidiListShown") ? Visibility.Visible : Visibility.Collapsed;
                    break;
                default:
                    List2Button.Visibility = Visibility.Collapsed;
                    break;
            }                
        }

        /// <summary>
        /// Updates the List3Button visibility and state based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateCMButton3MidiList()
        {
            switch (this.viewModel.StateParam)
            {
                case "CmDataEntry3":
                case "CmDataEntry4":
                case "SelectContainer":
                case "SelectContainerWith3TareA":
                case "SelectContainerWith3TareB":
                    this.List3Button.Visibility = GetPropertyBoolValue("CMButton3MidiListShown") ? Visibility.Visible : Visibility.Collapsed;
                    break;
                default:
                    List3Button.Visibility = Visibility.Collapsed;
                    break;
            }       
        }

        /// <summary>
        /// Updates the List4Button visibility and state based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateCMButton4MidiList()
        {
            switch (this.viewModel.StateParam)
            {
                case "CmDataEntry4":
                case "SelectContainer":
                case "SelectContainerWith3TareB":
                    this.List4Button.Visibility = GetPropertyBoolValue("CMButton4MidiListShown") ? Visibility.Visible : Visibility.Collapsed;
                    break;
                default:
                    List4Button.Visibility = Visibility.Collapsed;
                    break;
            }  
        }

        /// <summary>
        /// Updates the List5Button visibility and state based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateCMButton2Med()
        {
            switch (this.viewModel.StateParam)
            {
                case "SelectContainerWith2Tare":
                    this.List5Button.Visibility = GetPropertyBoolValue("CMButton2MedShown") ? Visibility.Visible : Visibility.Collapsed;
                    break;
                default:
                    List5Button.Visibility = Visibility.Collapsed;
                    break;
            }  
        }

        /// <summary>
        /// Updates the List6Button visibility and state based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateCMButton3Med()
        {
            switch (this.viewModel.StateParam)
            {
                case "SelectContainerWith2Tare":
                    this.List6Button.Visibility = GetPropertyBoolValue("CMButton3MedShown") ? Visibility.Visible : Visibility.Collapsed;
                    break;
                default:
                    List6Button.Visibility = Visibility.Collapsed;
                    break;
            }            
        }

        /// <summary>
        /// This method will update the visibility and the text of the go back button for Data Entry contexts
        /// </summary>
        private void UpdateButtonGoBackForDataEntryContext()
        {
            switch (viewModel.StateParam)
            {
                case "CmDataEntry":
                case "CmDataEntry1":
                case "CmDataEntry2":
                case "CmDataEntry3":
                case "CmDataEntry4":
                    GoBackButton.Text = GetPropertyStringValue("ButtonGoBack");
                    GoBackButton.Visibility = GetPropertyBoolValue("ButtonGoBackShown") ? Visibility.Visible : Visibility.Collapsed;
                    break;
            }
        }
    }
}
