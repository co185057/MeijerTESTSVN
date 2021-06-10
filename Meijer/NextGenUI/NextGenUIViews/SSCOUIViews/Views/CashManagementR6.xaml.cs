// <copyright file="CashManagementR6.xaml.cs" company="NCR">
//     Copyright 2016 NCR. All rights reserved.
// </copyright>

namespace SSCOUIViews.Views
{
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.Collections.Specialized;
    using System.Windows;
    using FPsxWPF.Controls;
    using SSCOControls;
    using SSCOUIModels;
    using SSCOUIModels.Controls;

    /// <summary>
    /// Interaction logic for CashManagementR6.XAML
    /// </summary>
    public partial class CashManagementR6 : BackgroundView
    {
        private ObservableCollection<ReceiptItem> loaderObject = new ObservableCollection<ReceiptItem>();

        /// <summary>
        /// Initializes a new instance of the CashManagementR6 class.
        /// </summary>
        /// <param name="viewModel">ViewModel object.</param>
        public CashManagementR6(IMainViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
        }

        /// <summary>
        /// Loader Object CollectionChanged
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of NotifyCollectionChangedEventArgs</param>
        private void loaderObject_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if ((e.Action == NotifyCollectionChangedAction.Add || e.Action == NotifyCollectionChangedAction.Replace) && ReplaceBindValues(sender))
            {
                SetupLoaderTable.ItemsSource = SetupLoaderTable.PopulateLoaderTable(loaderObject);
            }
        }

        /// <summary>
        /// Replaces the bind values
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        private bool ReplaceBindValues(object sender)
        {
            var receiptItem = sender as ObservableCollection<ReceiptItem>;
            if (receiptItem != null && receiptItem.Count > 0)
            {
                return true;
            }

            return false;
        }

        /// <summary>
        /// Method for On Unloaded event
        /// </summary>
        /// <param name="sender">object sender</param>
        /// <param name="e">Routed Event Args</param>
        private void CashManagementR6_Unloaded(object sender, RoutedEventArgs e)
        {
            loaderObject.CollectionChanged -= loaderObject_CollectionChanged;
        }

        /// <summary>
        /// Accepts a parameter change as configured in app.config.
        /// </summary>
        /// <param name="param">Changed parameter.</param>
        public override void OnStateParamChanged(string param)
        {
            UpdateStateParam(param);
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
                case "XMAlertBoxV6HWShown":
                case "XMAlertOKButtonV6HWShown":
                    UpdateAlertBoxVisibility();
                    UpdateAlertButtonVisibility();
                    break;
                case "XMAlertBoxV6HWText":
                    UpdateAlertBoxText();
                    break;
                case "XMFailedCoinAlertV6HWShown":
                case "XMFailedCoinAlertOKButtonV6HWShown":
                    UpdateFailedCoinAlertControlsVisibility();
                    break;
                case "XMFailedCoinAlertV6HWText":
                    UpdateFailedCoinAlertBoxText();
                    break;
                case "ActiveCoinBoxLowRedFillShown":
                case "ActiveCoinBoxFullRedFillShown":
                    UpdateActiveBoxAreaFill();
                    break;
                case "XMLeadthruTextV6HWText":
                    UpdateLeadthruText();
                    break;
                case "XMLeadthruTextV6HWShown":
                    UpdateLeadthruTextVisibility();
                    break;
                case "XMTitle1V6HWShown":
                    UpdateTitleVisibility();
                    break;
                case "XMTitle1V6HWText":
                    UpdateTitle();
                    break;
                case "XMButton2V6HWShown":
                case "XMButton2V6HWToggled":
                case "XMButton2V6HWText":
                case "XMButton4V6HWShown":
                case "XMButton4V6HWToggled":
                case "XMButton4V6HWText":
                case "XMButton6V6HWShown":
                case "XMButton6V6HWToggled":
                case "XMButton6V6HWText":
                case "XMButton10V6HWShown":
                case "XMButton10V6HWToggled":
                case "XMButton10V6HWText":
                    UpdateActionButton1();
                    break;
                case "XMButton3V6HWShown":
                case "XMButton3V6HWToggled":
                case "XMButton3V6HWText":
                case "XMButton5V6HWShown":
                case "XMButton5V6HWToggled":
                case "XMButton5V6HWText":
                case "XMButton9V6HWShown":
                case "XMButton9V6HWToggled":
                case "XMButton9V6HWText":
                    UpdateActionButton2();
                    break;
                case "XMButton7V6HWShown":
                case "XMButton7V6HWToggled":
                case "XMButton7V6HWText":
                case "XMPurgeCoinBoxButtonV6HWShown":
                case "XMPurgeCoinBoxButtonV6HWToggled":
                case "XMPurgeCoinBoxButtonV6HWText":
                case "XMResetCoinBoxButtonV6HWShown":
                case "XMResetCoinBoxButtonV6HWToggled":
                case "XMResetCoinBoxButtonV6HWText":
                    UpdateActionButton3();
                    break;
                case "XMMenuButtonV6HW5001Shown":
                case "XMMenuButtonV6HW5001Toggled":
                    UpdateMenuButton1();
                    break;
                case "XMMenuButtonV6HW5002Shown":
                case "XMMenuButtonV6HW5002Toggled":
                    UpdateMenuButton2();
                    break;
                case "XMMenuButtonV6HW5003Shown":
                case "XMMenuButtonV6HW5003Toggled":
                    UpdateMenuButton3();
                    break;
                case "XMClearCoinBoxErrorButtonV6HWShown":
                case "XMClearCoinBoxErrorButtonV6HWToggled":
                case "XMClearCoinBoxErrorButtonV6HWText":
                    UpdateActionButton4();
                    break;
                case "XMNumericKeyPadZeroKeyV6HWShown":
                    UpdateNumericPadVisibility();
                    break;
                case "XMApplyChangesV6HWShown":
                    UpdateApplyChangesButtonVisibility();
                    break;
                case "XMCancelChangesV6HWShown":
                    UpdateCancelChangesButtonVisibility();
                    break;
                case "XMActiveCoinBoxV6HWFailedCoinCountsShown":
                    UpdateFailedCoinStatusPanelVisibility();
                    break;
                case "XMActiveCoinBoxV6HWFailedCoinCountsText":
                    UpdateFailedCoinStatusText();
                    break;
                case "XMNoteBoxV6HWFailedNoteCountShown":
                    UpdateFailedNoteStatusPanelVisibility();
                    break;
                case "XMNoteBoxV6HWFailedNoteCountText":
                    UpdateFailedNoteStatusText();
                    break;
                case "XMSetupLoaderLeadthruV6HWShown":
                case "XMSetupLoaderLeadthruV6HW":
                    UpdateXMSetupLoaderLeadthruV6HW();
                    break;
                case "XMLoaderStatusTableV6HWShown":
                    UpdateSetupLoaderTable();
                    break;
            }
        }

        /// <summary>
        /// Triggers view updates based on a parameter change.
        /// </summary>
        /// <param name="param">Changed parameter.</param>
        private void UpdateStateParam(string param)
        {
            UpdateAlertBoxVisibility();
            UpdateAlertBoxText();
            UpdateFailedCoinAlertControlsVisibility();
            UpdateFailedCoinAlertBoxText();
            UpdateActiveBoxAreaFill();
            UpdateTitle();
            UpdateTitleVisibility();
            UpdateLeadthruText();
            UpdateLeadthruTextVisibility();
            UpdateButtonPanelVisibility();
            UpdateMenuButtons();
            UpdateActionButtons();
            UpdateNumericPadVisibility();
            UpdateApplyChangesButtonVisibility();
            UpdateCancelChangesButtonVisibility();
            UpdateFailedCoinStatusPanelVisibility();
            UpdateFailedCoinStatusText();
            UpdateFailedNoteStatusPanelVisibility();
            UpdateFailedNoteStatusText();
            UpdateXMSetupLoaderLeadthruV6HW();
            UpdateSetupLoaderTable();
        }

        /// <summary>
        /// Updates the AlertBoxControl visibility based on the corresponding property watch.
        /// </summary>
        private void UpdateAlertBoxVisibility()
        {
            AlertBoxControl.Visibility = GetPropertyBoolValue("XMAlertBoxV6HWShown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the AlertBoxControl text based on the corresponding property watch.
        /// </summary>
        private void UpdateAlertBoxText()
        {
            AlertBoxInstructions.Text = GetPropertyStringValue("XMAlertBoxV6HWText");
        }

        /// <summary>
        /// Updates the AlertBoxButton visibility based on the corresponding property watch.
        /// </summary>
        private void UpdateAlertButtonVisibility()
        {
            AlertBoxButton.Visibility = GetPropertyBoolValue("XMAlertOKButtonV6HWShown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the FailedCoinAlertControls visibility based on the corresponding property watches.
        /// </summary>
        private void UpdateFailedCoinAlertControlsVisibility()
        {
            FailedCoinAlertBoxControl.Visibility = GetPropertyBoolValue("XMFailedCoinAlertV6HWShown") ? Visibility.Visible : Visibility.Collapsed;
            FailedCoinAlertBoxButton.Visibility = GetPropertyBoolValue("XMFailedCoinAlertOKButtonV6HWShown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the FailedCoinAlertBoxControl text based on the corresponding property watch.
        /// </summary>
        private void UpdateFailedCoinAlertBoxText()
        {
            FailedCoinAlertBoxInstructions.Text = GetPropertyStringValue("XMFailedCoinAlertV6HWText");
        }

        /// <summary>
        /// Updates the ActiveCoinBoxArea style based on the corresponding high/low fill property watches.
        /// </summary>
        private void UpdateActiveBoxAreaFill()
        {
            if (GetPropertyBoolValue("ActiveCoinBoxLowRedFillShown") || GetPropertyBoolValue("ActiveCoinBoxFullRedFillShown"))
            {
                ActiveCoinBoxArea.Style = FindResource("cashManagementR6ActiveBoxBodyAlertStyle") as Style;
            }
            else
            {
                ActiveCoinBoxArea.Style = FindResource("cashManagementR6ActiveBoxBodyStyle") as Style;
            }
        }

        /// <summary>
        /// Updates the CashManagementTitle style based on the corresponding high/low fill property watches.
        /// </summary>
        private void UpdateTitle()
        {
            CashManagementTitle.Text = GetPropertyStringValue("XMTitle1V6HWText");
        }

        /// <summary>
        /// Updates the CashManagementTitle visibility based on property watches.
        /// </summary>
        private void UpdateTitleVisibility()
        {
            switch (viewModel.StateParam)
            {
                case "CMCashAddV6HW":
                case "CMCashRemoveV6HW":
                case "XMCashStatusV6HW":
                    CashManagementTitle.Visibility = GetPropertyBoolValue("XMTitle1V6HWShown") ? Visibility.Visible : Visibility.Collapsed;
                    break;
                default:
                    CashManagementTitle.Visibility = Visibility.Visible;
                    break;
            }
        }

        /// <summary>
        /// Updates the cash management lead thru text property watch.
        /// </summary>
        private void UpdateLeadthruText()
        {
            CashManagementLeadthru.Text = GetPropertyStringValue("XMLeadthruTextV6HWText");
        }

        /// <summary>
        /// Updates the cash management lead thru text visibility property watch.
        /// </summary>
        private void UpdateLeadthruTextVisibility()
        {
            switch (viewModel.StateParam)
            {
                case "CMCashAddV6HW":
                case "CMCashRemoveV6HW":
                case "XMCashStatusV6HW":
                    CashManagementLeadthru.Visibility = GetPropertyBoolValue("XMLeadthruTextV6HWShown") ? Visibility.Visible : Visibility.Collapsed;
                    break;
                default:
                    CashManagementLeadthru.Visibility = Visibility.Visible;
                    break;
            }
        }

        /// <summary>
        /// Updates the visibility property of button panel controls based on the current state parameter.
        /// </summary>
        private void UpdateButtonPanelVisibility()
        {
            switch (viewModel.StateParam)
            {
                case "CMCashAddV6HW":
                    CashStatusMenuButtonPanel.Visibility = RemoveCashMenuButtonPanel.Visibility = RemoveCashActionButtonPanel.Visibility = Visibility.Collapsed;
                    AddCashMenuButtonPanel.Visibility = AddCashActionButtonPanel.Visibility = NumericPadPanel.Visibility = Visibility.Visible;
                    break;
                case "CMCashRemoveV6HW":
                    CashStatusMenuButtonPanel.Visibility = AddCashMenuButtonPanel.Visibility = AddCashActionButtonPanel.Visibility = Visibility.Collapsed;
                    RemoveCashMenuButtonPanel.Visibility = RemoveCashActionButtonPanel.Visibility = NumericPadPanel.Visibility = Visibility.Visible;
                    break;
                case "XMCashStatusV6HW":
                    RemoveCashMenuButtonPanel.Visibility = RemoveCashActionButtonPanel.Visibility = AddCashMenuButtonPanel.Visibility = AddCashActionButtonPanel.Visibility = Visibility.Collapsed;
                    CashStatusMenuButtonPanel.Visibility = Visibility.Visible;
                    NumericPadPanel.Visibility = Visibility.Hidden;
                    break;
            }
        }

        /// <summary>
        /// Updates all menu button controls.
        /// </summary>
        private void UpdateMenuButtons()
        {
            UpdateMenuButton1();
            UpdateMenuButton2();
            UpdateMenuButton3();
        }

        /// <summary>
        /// Updates all action button controls.
        /// </summary>
        private void UpdateActionButtons()
        {
            UpdateActionButton1();
            UpdateActionButton2();
            UpdateActionButton3();
            UpdateActionButton4();
        }

        /// <summary>
        /// Updates MenuButton1 state.
        /// </summary>
        private void UpdateMenuButton1()
        {
            UpdateToggleButtonState(MenuButton1, "XMMenuButtonV6HW5001");
        }

        /// <summary>
        /// Updates MenuButton2 state.
        /// </summary>
        private void UpdateMenuButton2()
        {
            UpdateToggleButtonState(MenuButton2, "XMMenuButtonV6HW5002");
        }

        /// <summary>
        /// Updates MenuButton3 state.
        /// </summary>
        private void UpdateMenuButton3()
        {
            UpdateToggleButtonState(MenuButton3, "XMMenuButtonV6HW5003");
        }

        /// <summary>
        /// Updates ActionButton1 state, text and command binding.
        /// </summary>
        private void UpdateActionButton1()
        {
            switch (viewModel.StateParam)
            {
                case "CMCashAddV6HW":
                    if (GetPropertyBoolValue("XMButton4V6HWShown"))
                    {
                        UpdateActionButtonState(AddCashButton1, "XMButton4V6HW");
                        AddCashButton1.Visibility = Visibility.Visible;
                    }
                    else
                    {
                        AddCashButton1.Visibility = Visibility.Collapsed;
                    }

                    break;
                case "CMCashRemoveV6HW":
                    if (GetPropertyBoolValue("XMButton2V6HWShown"))
                    {
                        UpdateActionButtonState(RemoveCashButton1, "XMButton2V6HW");
                        RemoveCashButton1.Visibility = Visibility.Visible;
                        RemoveCashButton1.IsEnabled = false; //SSCOI-47285
                    }
                    else if (GetPropertyBoolValue("XMButton6V6HWShown"))
                    {
                        UpdateActionButtonState(RemoveCashButton1, "XMButton6V6HW");
                        RemoveCashButton1.Visibility = Visibility.Visible;
                    }
                    else if (GetPropertyBoolValue("XMButton10V6HWShown"))
                    {
                        UpdateActionButtonState(RemoveCashButton1, "XMButton10V6HW");
                        RemoveCashButton1.Visibility = Visibility.Visible;
                    }
                    else
                    {
                        RemoveCashButton1.Visibility = Visibility.Collapsed;
                    }

                    break;
            }
        }

        /// <summary>
        /// Updates ActionButton2 state, text and command binding.
        /// </summary>
        private void UpdateActionButton2()
        {
            switch (viewModel.StateParam)
            {
                case "CMCashAddV6HW":
                    if (GetPropertyBoolValue("XMButton5V6HWShown"))
                    {
                        UpdateActionButtonState(AddCashButton2, "XMButton5V6HW");
                        AddCashButton2.Visibility = Visibility.Visible;
                    }
                    else
                    {
                        AddCashButton2.Visibility = Visibility.Collapsed;
                    }

                    break;
                case "CMCashRemoveV6HW":
                    if (GetPropertyBoolValue("XMButton3V6HWShown"))
                    {
                        UpdateActionButtonState(RemoveCashButton2, "XMButton3V6HW");
                        RemoveCashButton2.Visibility = Visibility.Visible;
                    }
                    else if (GetPropertyBoolValue("XMButton5V6HWShown"))
                    {
                        UpdateActionButtonState(RemoveCashButton2, "XMButton5V6HW");
                        RemoveCashButton2.Visibility = Visibility.Visible;
                        RemoveCashButton2.IsEnabled = false; //SSCOI-47285
                    }
                    else if (GetPropertyBoolValue("XMButton9V6HWShown"))
                    {
                        UpdateActionButtonState(RemoveCashButton2, "XMButton9V6HW");
                        RemoveCashButton2.Visibility = Visibility.Visible;
                    }
                    else
                    {
                        RemoveCashButton2.Visibility = Visibility.Collapsed;
                    }

                    break;
            }
        }

        /// <summary>
        /// Updates ActionButton3 state, text and command binding.
        /// </summary>
        private void UpdateActionButton3()
        {
            switch (viewModel.StateParam)
            {
                case "XMCashStatusV6HW":
                    if (GetPropertyBoolValue("XMButton7V6HWShown"))
                    {
                        UpdateActionButtonState(EnableCoinAcceptorButton, "XMButton7V6HW");
                        EnableCoinAcceptorButton.Visibility = Visibility.Visible;
                    }
                    else
                    {
                        EnableCoinAcceptorButton.Visibility = Visibility.Collapsed;
                    }

                    break;
                case "CMCashAddV6HW":
                    if (GetPropertyBoolValue("XMButton7V6HWShown"))
                    {
                        UpdateActionButtonState(AddCashButton3, "XMButton7V6HW");
                        AddCashButton3.Visibility = Visibility.Visible;
                    }
                    else
                    {
                        AddCashButton3.Visibility = Visibility.Collapsed;
                    }

                    break;
				//SSCOI-50040
                case "CMCashRemoveV6HW":
                    if (GetPropertyBoolValue("XMPurgeCoinBoxButtonV6HWShown"))
                    {
                        UpdateActionButtonState(RemoveCashButton3, "XMPurgeCoinBoxButtonV6HW");
                        RemoveCashButton3.Visibility = Visibility.Visible;
                    }
                    else if (GetPropertyBoolValue("XMResetCoinBoxButtonV6HWShown"))
                    {
                        UpdateActionButtonState(RemoveCashButton3, "XMResetCoinBoxButtonV6HW");
                        RemoveCashButton3.Visibility = Visibility.Visible;
                    }
                    else
                    {
                        RemoveCashButton3.Visibility = Visibility.Collapsed;
                    }

                    break;
            }
        }

        // <summary>
        /// Updates the state, text and command binding for ActionButton4 when StateParam is CMCashRemoveV6HW.
        /// </summary>
        private void UpdateActionButton4()
        {
            switch (viewModel.StateParam)
            {
                case "CMCashRemoveV6HW":
                    if (GetPropertyBoolValue("XMClearCoinBoxErrorButtonV6HWShown"))
                    {
                        UpdateActionButtonState(RemoveCashButton4, "XMClearCoinBoxErrorButtonV6HW");
                        RemoveCashButton4.Visibility = Visibility.Visible;
                    }
                    else
                    {
                        RemoveCashButton4.Visibility = Visibility.Collapsed;
                    }
                    break;
            }
        }

        /// <summary>
        /// Updates the NumericPad control visibility based on the corresponding property watch.
        /// </summary>
        private void UpdateNumericPadVisibility()
        {
            NumericPad.Visibility = GetPropertyBoolValue("XMNumericKeyPadZeroKeyV6HWShown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the ApplyChanges button visibility based on the corresponding property watch.
        /// </summary>
        private void UpdateApplyChangesButtonVisibility()
        {
            ApplyChanges.Visibility = GetPropertyBoolValue("XMApplyChangesV6HWShown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the CancelChanges button visibility based on the corresponding property watch.
        /// </summary>
        private void UpdateCancelChangesButtonVisibility()
        {
            CancelChanges.Visibility = GetPropertyBoolValue("XMCancelChangesV6HWShown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the FailedCoinStatusPanel visibility based on the corresponding property watch.
        /// </summary>
        private void UpdateFailedCoinStatusPanelVisibility()
        {
            FailedCoinStatusPanel.Visibility = GetPropertyBoolValue("XMActiveCoinBoxV6HWFailedCoinCountsShown") ? Visibility.Visible : Visibility.Hidden;
        }

        /// <summary>
        /// Updates the FailedCoinCount text based on the corresponding property watch.
        /// </summary>
        private void UpdateFailedCoinStatusText()
        {
            FailedCoinCount.Text = GetPropertyStringValue("XMActiveCoinBoxV6HWFailedCoinCountsText");
        }

        /// <summary>
        /// Updates the FailedNoteStatusPanel visibility based on the corresponding property watch.
        /// </summary>
        private void UpdateFailedNoteStatusPanelVisibility()
        {
            FailedNoteStatusPanel.Visibility = GetPropertyBoolValue("XMNoteBoxV6HWFailedNoteCountShown") ? Visibility.Visible : Visibility.Hidden;
        }

        /// <summary>
        /// Updates the FailedNoteCount text based on the corresponding property watch.
        /// </summary>
        private void UpdateFailedNoteStatusText()
        {
            FailedNoteCount.Text = GetPropertyStringValue("XMNoteBoxV6HWFailedNoteCountText");
        }

        /// <summary>
        /// Updates the enabled and toggled states for a generic toggle button control.
        /// </summary>
        /// <param name="button">ImageToggleButton object to change.</param>
        /// <param name="controlName">String name of the control as defined in app.config.</param>
        private void UpdateToggleButtonState(ImageToggleButton button, string controlName)
        {
            if (GetPropertyStringValue(controlName + "Toggled").Equals("3"))
            {
                button.IsEnabled = true;
                button.IsChecked = true;
            }
            else if (GetPropertyStringValue(controlName + "Toggled").Equals("2"))
            {
                button.IsEnabled = false;
                if (GetPropertyBoolValue(controlName + "Shown"))
                {
                    button.IsChecked = false;
                }
                else
                {
                    button.IsChecked = true;
                }
            }
            else
            {
                button.IsEnabled = true;
                button.IsChecked = false;
            }
        }

        /// <summary>
        /// Updates the command parameter, enabled and text binding for a generic action button control.
        /// </summary>
        /// <param name="button">ImageToggleButton object to change.</param>
        /// <param name="controlName">String name of the control as defined in app.config.</param>
        private void UpdateActionButtonState(ImageToggleButton button, string controlName)
        {
            button.CommandParameter = controlName;
            button.Text = GetPropertyStringValue(controlName + "Text");
            UpdateToggleButtonState(button, controlName);
        }

        /// <summary>
        /// Updates the setup loader table with the corresponding values
        /// </summary>
        private void UpdateSetupLoaderTable()
        {
            if (GetPropertyBoolValue("XMLoaderStatusTableV6HWShown"))
            {
                CashManagementTable.Visibility = Visibility.Visible;
                CashManagementCylinderArea.Visibility = Visibility.Collapsed;
                loaderObject = (ObservableCollection<ReceiptItem>)viewModel.GetPropertyValue("XMLoaderStatusTableV6HW");
                loaderObject.CollectionChanged += loaderObject_CollectionChanged;
                SetupLoaderTable.ItemsSource = SetupLoaderTable.PopulateLoaderTable(loaderObject);
            }
            else
            {
                CashManagementTable.Visibility = Visibility.Collapsed;
                CashManagementCylinderArea.Visibility = Visibility.Visible;
            }
        }

        /// <summary>
        /// Updates the XMSetupLoaderLeadthruV6HW text
        /// </summary>
        private void UpdateXMSetupLoaderLeadthruV6HW()
        {
            XMSetupLoaderLeadthruV6HW.Text = GetPropertyStringValue("XMSetupLoaderLeadthruV6HW");
        }

        /// <summary>
        /// NumericPad handler for IsEnabledChanged event.
        /// Clears the input text after the numeric pad is disabled.
        /// </summary>
        /// <param name="sender">Event Sender.</param>
        /// <param name="e">DependencyPropertyChangedEventArgs struct.</param>
        private void NumericPad_IsEnabledChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (!NumericPad.IsEnabled)
            {
                NumericPad.InputTextBox.Clear();
            }
        }

        /// <summary>
        /// MenuButton1 handler for Click event.
        /// Updates menu button state.
        /// </summary>
        /// <param name="sender">Event Sender.</param>
        /// <param name="e">RoutedEventArgs struct.</param>
        private void MenuButton1_Click(object sender, RoutedEventArgs e)
        {
            UpdateMenuButton1();
        }

        /// <summary>
        /// MenuButton2 handler for Click event.
        /// Updates menu button state.
        /// </summary>
        /// <param name="sender">Event Sender.</param>
        /// <param name="e">RoutedEventArgs struct.</param>
        private void MenuButton2_Click(object sender, RoutedEventArgs e)
        {
            UpdateMenuButton2();
        }

        /// <summary>
        /// MenuButton3 handler for Click event.
        /// Updates menu button state.
        /// </summary>
        /// <param name="sender">Event Sender.</param>
        /// <param name="e">RoutedEventArgs struct.</param>
        private void MenuButton3_Click(object sender, RoutedEventArgs e)
        {
            UpdateMenuButton3();
        }

        /// <summary>
        /// RemoveCashButton1 handler for Click event.
        /// Updates action button state.
        /// </summary>
        /// <param name="sender">Event Sender.</param>
        /// <param name="e">RoutedEventArgs struct.</param>
        private void RemoveCashButton1_Click(object sender, RoutedEventArgs e)
        {
            if (GetPropertyBoolValue("XMButton2V6HWShown"))
            {
                 if (viewModel.StateParam.Equals("CMCashRemoveV6HW"))
                 {
                    RemoveCashButton1.IsEnabled = false; //SSCOI-47285
                    UpdateToggleButtonState(RemoveCashButton1, "XMButton2V6HW");
                 }
                
            }
            else if (GetPropertyBoolValue("XMButton6V6HWShown"))
            {
                UpdateToggleButtonState(RemoveCashButton1, "XMButton6V6HW");
            }
            else if (GetPropertyBoolValue("XMButton10V6HWShown"))
            {
                UpdateToggleButtonState(RemoveCashButton1, "XMButton10V6HW");
            }
        }

        /// <summary>
        /// RemoveCashButton2 handler for Click event.
        /// Updates action button state.
        /// </summary>
        /// <param name="sender">Event Sender.</param>
        /// <param name="e">RoutedEventArgs struct.</param>
        private void RemoveCashButton2_Click(object sender, RoutedEventArgs e)
        {
            if (GetPropertyBoolValue("XMButton3V6HWShown"))
            {
                UpdateToggleButtonState(RemoveCashButton2, "XMButton3V6HW");
            }
            else if (GetPropertyBoolValue("XMButton5V6HWShown"))
            {
                if (viewModel.StateParam.Equals("CMCashRemoveV6HW"))
                {
                    RemoveCashButton2.IsEnabled = false; //SSCOI-47285
                    UpdateToggleButtonState(RemoveCashButton2, "XMButton5V6HW");
                }
            }
            else if (GetPropertyBoolValue("XMButton9V6HWShown"))
            {
                UpdateToggleButtonState(RemoveCashButton2, "XMButton9V6HW");
            }
        }

        /// <summary>
        /// RemoveCashButton3 handler for Click event.
        /// Updates action button state.
        /// </summary>
        /// <param name="sender">Event Sender.</param>
        /// <param name="e">RoutedEventArgs struct.</param>
        private void RemoveCashButton3_Click(object sender, RoutedEventArgs e)
        {
            if (GetPropertyBoolValue("XMPurgeCoinBoxButtonV6HWShown"))
            {
                UpdateToggleButtonState(RemoveCashButton3, "XMPurgeCoinBoxButtonV6HW");
            }
            else if (GetPropertyBoolValue("XMResetCoinBoxButtonV6HWShown"))
            {
                UpdateToggleButtonState(RemoveCashButton3, "XMResetCoinBoxButtonV6HW");
            }
        }

        /// <summary>
        /// Click event handler of RemoveCashButton4 for updating the state of action button.
        /// </summary>
        /// <param name="sender">Event Sender.</param>
        /// <param name="e">RoutedEventArgs struct.</param>
        private void RemoveCashButton4_Click(object sender, RoutedEventArgs e)
        {
            if (GetPropertyBoolValue("XMClearCoinBoxErrorButtonV6HWShown"))
            {
                UpdateToggleButtonState(RemoveCashButton4, "XMClearCoinBoxErrorButtonV6HW");
            }
        }

        /// <summary>
        /// AddCashButton1 handler for Click event.
        /// Updates action button state.
        /// </summary>
        /// <param name="sender">Event Sender.</param>
        /// <param name="e">RoutedEventArgs struct.</param>
        private void AddCashButton1_Click(object sender, RoutedEventArgs e)
        {
            UpdateToggleButtonState(AddCashButton1, "XMButton4V6HW");
        }

        /// <summary>
        /// AddCashButton2 handler for Click event.
        /// Updates action button state.
        /// </summary>
        /// <param name="sender">Event Sender.</param>
        /// <param name="e">RoutedEventArgs struct.</param>
        private void AddCashButton2_Click(object sender, RoutedEventArgs e)
        {
            UpdateToggleButtonState(AddCashButton2, "XMButton5V6HW");
        }

        /// <summary>
        /// AddCashButton3 handler for Click event.
        /// Updates action button state.
        /// </summary>
        /// <param name="sender">Event Sender.</param>
        /// <param name="e">RoutedEventArgs struct.</param>
        private void AddCashButton3_Click(object sender, RoutedEventArgs e)
        {
            UpdateToggleButtonState(AddCashButton3, "XMButton7V6HW");
        }
    }
}