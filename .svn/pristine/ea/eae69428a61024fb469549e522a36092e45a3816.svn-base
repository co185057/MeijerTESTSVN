// <copyright file="CashManagement.xaml.cs" company="NCR Corporation">
//     Copyright (c) NCR Corporation. All rights reserved.
// </copyright>

namespace SSCOUIViews.Views
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.Collections.Specialized;
    using System.ComponentModel;
    using System.Diagnostics;
    using System.Text.RegularExpressions;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Input;
    using System.Windows.Threading;
    using FPsxWPF.Controls;
    using RPSWNET;
    using SSCOControls;
    using SSCOUIModels;
    using SSCOUIModels.Controls;
    using SSCOUIModels.Helpers;
    using SSCOUIModels.Models;    

    /// <summary>
    /// Interaction logic for CashManagement.xaml
    /// </summary>
    public partial class CashManagement : BackgroundView
    {
        /// <summary>
        /// Initializes a new instance of the CashManagement class.
        /// </summary>
        /// <param name="viewModel">Parameter that inherits the interface of MainViewModel</param>
        public CashManagement(IMainViewModel viewModel)
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
            UpdateTitleText();
            UpdateLeadthruText();
            UpdateLeadthruTitleText();
            UpdateTotalChangedText();
            UpdatePurgeArea();
            UpdateTotalDescriptionArea();
            UpdateControlVisibility();
            UpdateAlertBox();
            UpdateActionButtons();
            UpdateChangeConfirmationButtons();
            UpdateMenuButtons();
            UpdateExitButtonState();
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
                case "XMLeadthruText":
                    UpdateLeadthruText();
                    break;
                case "XMLeadthruTextTitle":
                    UpdateLeadthruTitleText();
                    break;
                case "XMTotalAddedText":
                case "XMTotalRemovedText":
                    UpdateTotalChangedText();
                    break;
                case "XMTotalCoinCountTextShown":
                case "XMTotalNoteCountTextShown":
                case "XMPendingCoinCountTextShown":
                case "XMPendingNoteCountTextShown":
                case "XMPendingCoinArea1Shown":
                case "XMPendingNoteArea1Shown":
                    UpdateTotalDescriptionArea();
                    break;
                case "XMErrorBoxShown":
                case "XMErrorBoxText":
                case "XMErrorAlertBoxShown":
                case "XMErrorAlertBoxText":
                case "XMAlertBoxShown":
                case "XMAlertBoxText":
                case "XMAlertNegativeLoaderShown":
                case "XMAlertNegativeLoaderText":
                case "XMManualCashRemovalAlertShown":
                case "XMManualCashRemovalAlertText":
                    UpdateAlertBox();
                    break;
                case "XMButton2Enabled":
                case "XMButton2Shown":
                case "XMButton2Text":
                case "XMButton5Enabled":
                case "XMButton5Shown":
                case "XMButton5Text":
                case "XMButton9Enabled":
                case "XMButton9Shown":
                case "XMButton9Text":
                    UpdateActionButton1();
                    break;
                case "XMButton4Enabled":
                case "XMButton4Shown":
                case "XMButton4Text":
                case "XMButton6Enabled":
                case "XMButton6Shown":
                case "XMButton6Text":
                case "XMButton10Enabled":
                case "XMButton10Shown":
                case "XMButton10Text":
                case "XMNonDispenseCylinder501EmptyButtonEnabled":
                case "XMNonDispenseCylinder501EmptyButtonShown":
                case "XMNonDispenseCylinder501EmptyButtonText":
                    UpdateActionButton2();
                    UpdateActionButton5(); //SSCOI-45868
                    break;
                case "XMButton7Enabled":
                case "XMButton7Shown":
                case "XMButton7Text":
                case "XMNonDispenseCylinder401EmptyButtonEnabled":
                case "XMNonDispenseCylinder401EmptyButtonShown":
                case "XMNonDispenseCylinder401EmptyButtonText":
                    UpdateActionButton3();
                    break;
                case "XMButton3Enabled":
                case "XMButton3Shown":
                case "XMButton3Text":
                    UpdateActionButton2();
                    UpdateActionButton4();
                    break;
                case "XMButton1Shown":
                case "XMButton1Enabled":
                    UpdateApplyChangesButton();
                    break;
                case "XMCancelChangesShown":
                case "XMCancelChangesEnabled":
                    UpdateCancelChangesButton();
                    break;
                case "XMMenuButton5001Toggled":
                    UpdateMenuButton1();
                    break;
                case "XMMenuButton5002Toggled":
                    UpdateMenuButton2();
                    break;
                case "XMMenuButton5003Toggled":
                    UpdateMenuButton3();
                    break;
                case "XMMenuButton5004Shown":
				case "XMMenuButton5004Toggled":				
                    UpdateMenuButton4();
                    break;
                case "XMPurgeOperationAlertShown":
                case "XMPurgeOperationCountShown":
                    UpdatePurgeArea();
                    break;
                case "XMButton8Enabled":
                case "SMButton8Enabled":
                    UpdateExitButtonState();
                    break;
            }
        }

        /// <summary>
        /// Updates the title control text using the string resource table.
        /// </summary>
        private void UpdateTitleText()
        {
            switch (this.viewModel.StateParam)
            {
                case "XMCashStatus":
                case "XMCashStatusGlory":
                    this.CashManagementTitle.Property(TextBlock.TextProperty).SetResourceValue("XM_CashStatus");
                    break;
                case "XMCashRemove":
                case "XMCashRemoveBNR":
                case "XMCashRemoveGlory":
                    this.CashManagementTitle.Property(TextBlock.TextProperty).SetResourceValue("XM_CashRemove");
                    break;
                case "XMCashReplenish":
                    this.CashManagementTitle.Property(TextBlock.TextProperty).SetResourceValue("XM_CashReplenishment");
                    break;
            }
        }

        /// <summary>
        /// Updates the lead control text based on corresponding property watch value.
        /// </summary>
        private void UpdateLeadthruText()
        {
            this.CashManagementLeadthru.Text = GetPropertyStringValue("XMLeadthruText");
        }

        /// <summary>
        /// Updates the lead title control text based on corresponding property watch value.
        /// </summary>
        private void UpdateLeadthruTitleText()
        {
            this.CashManagementLeadthruTitle.Text = GetPropertyStringValue("XMLeadthruTextTitle");
        }

        /// <summary>
        /// Updates the total changed control text based on corresponding property watch value.
        /// </summary>
        private void UpdateTotalChangedText()
        {
            switch (this.viewModel.StateParam)
            {
                case "XMCashRemoveGlory":
                    this.CashManagementTotalChanged.Text = GetPropertyStringValue("XMTotalRemovedText");
                    break;
                case "XMCashStatusGlory":
                    this.CashManagementTotalChanged.Text = GetPropertyStringValue("XMTotalAddedText");
                    break;
            }
        }

        /// <summary>
        /// Updates the controls in the total description area based on property watch values for visibility and text.
        /// </summary>
        private void UpdateTotalDescriptionArea()
        {
            switch (this.viewModel.StateParam)
            {
                case "XMCashRemove":
                case "XMCashRemoveBNR":
                case "XMCashRemoveGlory":
                    this.PendingCoinArea1.Text = GetPropertyStringValue("XMPendingCoinArea1Text");
                    this.PendingCoinArea2.Text = GetPropertyStringValue("XMPendingCoinArea2Text");
                    this.PendingNoteArea1.Text = GetPropertyStringValue("XMPendingNoteArea1Text");
                    this.PendingNoteArea2.Text = GetPropertyStringValue("XMPendingNoteArea2Text");
                    this.PendingCoins.Visibility = this.PendingCoinArea1.Visibility = this.PendingCoinArea2.Visibility = GetPropertyBoolValue("XMPendingCoinArea1Shown") ? Visibility.Visible : Visibility.Collapsed;
                    this.PendingNotes.Visibility = this.PendingNoteArea1.Visibility = this.PendingNoteArea2.Visibility = GetPropertyBoolValue("XMPendingNoteArea1Shown") ? Visibility.Visible : Visibility.Collapsed;
                    this.TotalCoinArea.Visibility = this.TotalNoteArea.Visibility = Visibility.Collapsed;
                    this.TotalCoins.Visibility = this.TotalNotes.Visibility = Visibility.Collapsed;
                    this.PendingCoinValue.Visibility = this.PendingCoinCount.Visibility = GetPropertyBoolValue("XMPendingCoinCountTextShown") ? Visibility.Visible : Visibility.Collapsed;
                    this.PendingNoteValue.Visibility = this.PendingNoteCount.Visibility = GetPropertyBoolValue("XMPendingNoteCountTextShown") ? Visibility.Visible : Visibility.Collapsed;
                    break;
                case "XMCashReplenish":
                case "XMCashStatus":
                case "XMCashStatusGlory":
                    this.TotalCoinArea.Property(TextBlock.TextProperty).SetResourceValue("XM_TotalCoinsInserted");
                    this.TotalNoteArea.Property(TextBlock.TextProperty).SetResourceValue("XM_TotalNotesInserted");
                    this.PendingCoinArea1.Visibility = this.PendingCoinArea2.Visibility = Visibility.Collapsed;
                    this.PendingNoteArea1.Visibility = this.PendingNoteArea2.Visibility = Visibility.Collapsed;
                    this.TotalCoinArea.Visibility = this.TotalNoteArea.Visibility = Visibility.Visible;
                    this.TotalCoins.Visibility = this.TotalNotes.Visibility = Visibility.Visible;
                    this.PendingCoins.Visibility = this.PendingNotes.Visibility = Visibility.Collapsed;

                    // Mask obscures controls in legacy Cash Management UI.
                    this.TotalCoinPanel.Visibility = GetPropertyBoolValue("XMTotalCoinAreaShown") ? Visibility.Collapsed : Visibility.Visible;
                    this.TotalNotePanel.Visibility = GetPropertyBoolValue("XMTotalNoteAreaShown") ? Visibility.Collapsed : Visibility.Visible;
                    this.TotalCoinValue.Visibility = this.TotalCoinCount.Visibility = GetPropertyBoolValue("XMTotalCoinCountTextShown") ? Visibility.Visible : Visibility.Collapsed;
                    this.TotalNoteValue.Visibility = this.TotalNoteCount.Visibility = GetPropertyBoolValue("XMTotalNoteCountTextShown") ? Visibility.Visible : Visibility.Collapsed;
                    break;
            }
        }

        /// <summary>
        /// Updates the purge and yield icon controls based on property watch values.
        /// </summary>
        private void UpdatePurgeArea()
        {
            this.PurgeCount.Visibility = GetPropertyBoolValue("XMPurgeOperationCountShown") ? Visibility.Visible : Visibility.Collapsed;
            this.YieldIcon.Visibility = GetPropertyBoolValue("XMPurgeOperationAlertShown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates various control visibility properties based on current state parameter.
        /// </summary>
        private void UpdateControlVisibility()
        {
            switch (this.viewModel.StateParam)
            {
                case "XMCashStatus":
                    this.CashManagementGrandTotal.Visibility = this.CashManagementTotalChanged.Visibility = Visibility.Collapsed;
                    this.LockScreenButton.Visibility = Visibility.Collapsed;
                    this.ActionButtonPanel.Visibility = this.MenuButtonPanel.Visibility = Visibility.Collapsed;
                    this.CashStatusButtonPanel.Visibility = Visibility.Visible;
                    this.ConfirmChangesPanel.Visibility = Visibility.Collapsed;
                    this.CashManagementLeadthruTitle.Visibility = Visibility.Collapsed;
                    break;
                case "XMCashStatusGlory":
                    this.CashManagementGrandTotal.Visibility = this.CashManagementTotalChanged.Visibility = Visibility.Visible;
                    this.AddCashButton.Visibility = Visibility.Hidden;
                    this.NonDispenseCylinder401.Visibility = Visibility.Collapsed;
                    this.PurgeInstructionHolder.Visibility = this.PurgeCountHolder.Visibility = Visibility.Collapsed;
                    this.LockScreenButton.Visibility = Visibility.Collapsed;
                    this.ActionButtonPanel.Visibility = this.MenuButtonPanel.Visibility = Visibility.Collapsed;
                    this.CashStatusButtonPanel.Visibility = Visibility.Visible;
                    this.ConfirmChangesPanel.Visibility = Visibility.Collapsed;
                    this.CashManagementLeadthruTitle.Visibility = Visibility.Collapsed;
                    break;
                case "XMCashReplenish":
                case "XMCashRemove":
                case "XMCashRemoveBNR":
                    this.CashManagementGrandTotal.Visibility = this.CashManagementTotalChanged.Visibility = Visibility.Collapsed;
                    this.LockScreenButton.Visibility = Visibility.Visible;
                    this.CashStatusButtonPanel.Visibility = Visibility.Collapsed;
                    this.MenuButtonPanel.Visibility = this.ActionButtonPanel.Visibility = Visibility.Visible;
                    this.ConfirmChangesPanel.Visibility = Visibility.Visible;
                    this.CashManagementLeadthruTitle.Visibility = Visibility.Visible;   
                    break;
                case "XMCashRemoveGlory":
                    this.CashManagementGrandTotal.Visibility = this.CashManagementTotalChanged.Visibility = Visibility.Visible;
                    this.NonDispenseCylinder401.Visibility = Visibility.Collapsed;
                    this.PurgeInstructionHolder.Visibility = this.PurgeCountHolder.Visibility = Visibility.Collapsed;
                    this.LockScreenButton.Visibility = Visibility.Visible;
                    this.CashStatusButtonPanel.Visibility = Visibility.Collapsed;
                    this.MenuButtonPanel.Visibility = this.ActionButtonPanel.Visibility = Visibility.Visible;
                    this.ConfirmChangesPanel.Visibility = Visibility.Visible;
                    this.CashManagementLeadthruTitle.Visibility = Visibility.Visible;
                    break;
            }
        }

        /// <summary>
        /// Updates alert box control text and visibility based on pertinent property watch values.
        /// Cash management uses multiple controls to track different alert messages.
        /// </summary>
        private void UpdateAlertBox()
        {
            if (GetPropertyBoolValue("XMErrorBoxShown"))
            {
                AlertBoxText.Style = FindResource("cashManagementAlertBoxStyle") as Style;
                AlertBoxText.Text = GetPropertyStringValue("XMErrorBoxText");
                AlertBoxControl.Visibility = Visibility.Visible;
            }
            else if (GetPropertyBoolValue("XMErrorAlertBoxShown"))
            {
                AlertBoxText.Style = FindResource("cashManagementAlertBoxStyle") as Style;
                AlertBoxText.Text = GetPropertyStringValue("XMErrorAlertBoxText");
                AlertBoxControl.Visibility = Visibility.Visible;
            }
            else if (GetPropertyBoolValue("XMAlertNegativeLoaderShown"))
            {
                AlertBoxText.Style = FindResource("cashManagementAlertBoxLongStyle") as Style;
                AlertBoxText.Text = GetPropertyStringValue("XMAlertNegativeLoaderText");
                AlertBoxControl.Visibility = Visibility.Visible;
            }
            else if (GetPropertyBoolValue("XMAlertBoxShown"))
            {
                AlertBoxText.Style = FindResource("cashManagementAlertBoxStyle") as Style;
                AlertBoxText.Text = GetPropertyStringValue("XMAlertBoxText");
                AlertBoxControl.Visibility = Visibility.Visible;
            }
            else if (GetPropertyBoolValue("XMManualCashRemovalAlertShown"))
            {
                AlertBoxText.Style = FindResource("cashManagementAlertBoxStyle") as Style;
                AlertBoxText.Text = GetPropertyStringValue("XMManualCashRemovalAlertText");
                AlertBoxControl.Visibility = Visibility.Visible;
            }
            else
            {
                AlertBoxControl.Visibility = Visibility.Collapsed;
            }
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
            UpdateActionButton5(); //SSCOI-45868
        }

        /// <summary>
        /// Updates action button 1 control based on pertinent property watch values.
        /// Cash management uses multiple controls for the same button position.
        /// </summary>
        private void UpdateActionButton1()
        {
            if (GetPropertyBoolValue("XMButton2Shown"))
            {
                this.ActionButton1.Text = GetPropertyStringValue("XMButton2Text");
                this.ActionButton1.CommandParameter = "XMButton2";
                this.ActionButton1.IsEnabled = GetPropertyBoolValue("XMButton2Enabled");
                this.ActionButton1.Visibility = Visibility.Visible;
            }
            else if (GetPropertyBoolValue("XMButton5Shown"))
            {
                this.ActionButton1.Text = GetPropertyStringValue("XMButton5Text");
                this.ActionButton1.CommandParameter = "XMButton5";
                this.ActionButton1.IsEnabled = GetPropertyBoolValue("XMButton5Enabled");
                this.ActionButton1.Visibility = Visibility.Visible;
            }
            else if (GetPropertyBoolValue("XMButton9Shown"))
            {
                this.ActionButton1.Text = GetPropertyStringValue("XMButton9Text");
                this.ActionButton1.CommandParameter = "XMButton9";
                this.ActionButton1.IsEnabled = GetPropertyBoolValue("XMButton9Enabled");
                this.ActionButton1.Visibility = Visibility.Visible;
            }
            else
            {
                this.ActionButton1.Visibility = Visibility.Collapsed;
            }
        }

        /// <summary>
        /// Updates action button 2 control based on pertinent property watch values.
        /// Cash management uses multiple controls for the same button position.
        /// </summary>
        private void UpdateActionButton2()
        {
            switch (this.viewModel.StateParam)
            {
                case "XMCashRemove":
                case "XMCashRemoveBNR":
                case "XMCashRemoveGlory":
                    if (GetPropertyBoolValue("XMButton4Shown"))
                    {
                        this.ActionButton2.Text = GetPropertyStringValue("XMButton4Text");
                        this.ActionButton2.CommandParameter = "XMButton4";
                        this.ActionButton2.IsEnabled = GetPropertyBoolValue("XMButton4Enabled");
                        this.ActionButton2.Visibility = Visibility.Visible;
                    }
                    else if (GetPropertyBoolValue("XMButton6Shown"))
                    {
                        this.ActionButton2.Text = GetPropertyStringValue("XMButton6Text");
                        this.ActionButton2.CommandParameter = "XMButton6";
                        this.ActionButton2.IsEnabled = GetPropertyBoolValue("XMButton6Enabled");
                        this.ActionButton2.Visibility = Visibility.Visible;
                    }
                    else if (GetPropertyBoolValue("XMButton10Shown"))
                    {
                        this.ActionButton2.Text = GetPropertyStringValue("XMButton10Text");
                        this.ActionButton2.CommandParameter = "XMButton10";
                        this.ActionButton2.IsEnabled = GetPropertyBoolValue("XMButton10Enabled");
                        this.ActionButton2.Visibility = Visibility.Visible;
                    }
                    else if (GetPropertyBoolValue("XMNonDispenseCylinder501EmptyButtonShown"))
                    {
                        this.ActionButton2.Text = GetPropertyStringValue("XMNonDispenseCylinder501EmptyButtonText");
                        this.ActionButton2.CommandParameter = "XMNonDispenseCylinder501EmptyButton";
                        this.ActionButton2.IsEnabled = GetPropertyBoolValue("XMNonDispenseCylinder501EmptyButtonEnabled");
                        this.ActionButton2.Visibility = Visibility.Visible;
                    }
                    else
                    {
                        this.ActionButton2.Visibility = Visibility.Collapsed;
                    }

                    break;
                case "XMCashReplenish":
                    if (GetPropertyBoolValue("XMButton3Shown"))
                    {
                        this.ActionButton2.Text = GetPropertyStringValue("XMButton3Text");
                        this.ActionButton2.CommandParameter = "XMButton3";
                        this.ActionButton2.IsEnabled = GetPropertyBoolValue("XMButton3Enabled");
                        this.ActionButton2.Visibility = Visibility.Visible;
                    }
                    else
                    {
                        this.ActionButton2.Visibility = Visibility.Collapsed;
                    }

                    break;
            }
        }

        /// <summary>
        /// Updates action button 3 control based on pertinent property watch values.
        /// Cash management uses multiple controls for the same button position.
        /// </summary>
        private void UpdateActionButton3()
        {
            if (GetPropertyBoolValue("XMButton7Shown"))
            {
                this.ActionButton3.Text = GetPropertyStringValue("XMButton7Text");
                this.ActionButton3.CommandParameter = "XMButton7";
                this.ActionButton3.IsEnabled = GetPropertyBoolValue("XMButton7Enabled");
                this.ActionButton3.Visibility = Visibility.Visible;
            }
            else if (GetPropertyBoolValue("XMNonDispenseCylinder401EmptyButtonShown"))
            {
                this.ActionButton3.Text = GetPropertyStringValue("XMNonDispenseCylinder401EmptyButtonText");
                this.ActionButton3.CommandParameter = "XMNonDispenseCylinder401EmptyButton";
                this.ActionButton3.IsEnabled = GetPropertyBoolValue("XMNonDispenseCylinder401EmptyButtonEnabled");
                this.ActionButton3.Visibility = Visibility.Visible;
            }
            else
            {
                this.ActionButton3.Visibility = Visibility.Collapsed;
            }
        }
        //+SSCOI-45868
        private void UpdateActionButton5()
        {
             if (GetPropertyBoolValue("XMNonDispenseCylinder501EmptyButtonShown"))
            {
                this.ActionButton5.Text = GetPropertyStringValue("XMNonDispenseCylinder501EmptyButtonText");
                this.ActionButton5.CommandParameter = "XMNonDispenseCylinder501EmptyButton";
                this.ActionButton5.IsEnabled = GetPropertyBoolValue("XMNonDispenseCylinder501EmptyButtonEnabled");
                this.ActionButton5.Visibility = Visibility.Visible;
            }
            else
            {
                this.ActionButton5.Visibility = Visibility.Collapsed;
            }
        }
        //-SSCOI-45868

        /// <summary>
        /// Updates action button 4 control based on pertinent property watch values.
        /// Cash management uses multiple controls for the same button position.
        /// </summary>
        private void UpdateActionButton4()
        {
            switch (this.viewModel.StateParam)
            {
                case "XMCashRemove":
                case "XMCashRemoveBNR":
                case "XMCashRemoveGlory":
                    if (GetPropertyBoolValue("XMButton3Shown"))
                    {
                        this.ActionButton4.Text = GetPropertyStringValue("XMButton3Text");
                        this.ActionButton4.CommandParameter = "XMButton3";
                        this.ActionButton4.IsEnabled = GetPropertyBoolValue("XMButton3Enabled");
                        this.ActionButton4.Visibility = Visibility.Visible;
                    }
                    else
                    {
                        this.ActionButton4.Visibility = Visibility.Collapsed;
                    }

                    break;
                case "XMCashReplenish":
                    this.ActionButton4.Visibility = Visibility.Collapsed;
                    break;
            }
        }

        /// <summary>
        /// Updates all confirmation button controls.
        /// </summary>
        private void UpdateChangeConfirmationButtons()
        {
            UpdateApplyChangesButton();
            UpdateCancelChangesButton();
        }

        /// <summary>
        /// Updates apply changes button control based on pertinent property watch values.
        /// </summary>
        private void UpdateApplyChangesButton()
        {
            this.ApplyChangesButton.IsEnabled = GetPropertyBoolValue("XMButton1Enabled");
            this.ApplyChangesButton.Visibility = GetPropertyBoolValue("XMButton1Shown") ? Visibility.Visible : Visibility.Hidden;
        }

        /// <summary>
        /// Updates cancel changes button control based on pertinent property watch values.
        /// </summary>
        private void UpdateCancelChangesButton()
        {
            this.CancelChangesButton.IsEnabled = GetPropertyBoolValue("XMCancelChangesEnabled");
            this.CancelChangesButton.Visibility = GetPropertyBoolValue("XMCancelChangesShown") ? Visibility.Visible : Visibility.Hidden;
        }

        /// <summary>
        /// Updates all menu button controls.
        /// </summary>
        private void UpdateMenuButtons()
        {
            UpdateMenuButton1();
            UpdateMenuButton2();
            UpdateMenuButton3();
            UpdateMenuButton4();
        }

        /// <summary>
        /// Update exit button state.
        /// </summary>
        private void UpdateExitButtonState()
        {
            switch (viewModel.StateParam)
            {
                case "XMCashRemove":
                case "XMCashRemoveBNR":
                case "XMCashRemoveGlory":
                case "XMCashReplenish":
                    ExitButton.IsEnabled = GetPropertyBoolValue("XMButton8Enabled");
                    break;
                case "XMCashStatus":
                case "XMCashStatusGlory":
                case "SmCardManagement":
                    ExitButton.IsEnabled = GetPropertyBoolValue("SMButton8Enabled");
                    break;
            }
        }

        /// <summary>
        /// Updates menu button 1 control based on pertinent property watch values.
        /// Toggled status 1 denotes a toggled button, 2 denotes a disabled button.
        /// </summary>
        private void UpdateMenuButton1()
        {
            this.MenuButton1.Visibility = GetPropertyBoolValue("XMMenuButton5001Shown") ? Visibility.Visible : Visibility.Hidden;
            this.MenuButton1.IsChecked = GetPropertyStringValue("XMMenuButton5001Toggled").Equals("1");
            this.MenuButton1.IsEnabled = !GetPropertyStringValue("XMMenuButton5001Toggled").Equals("2");
        }

        /// <summary>
        /// Updates menu button 2 control based on pertinent property watch values.
        /// Toggled status 1 denotes a toggled button, 2 denotes a disabled button.
        /// </summary>
        private void UpdateMenuButton2()
        {
            this.MenuButton2.Visibility = GetPropertyBoolValue("XMMenuButton5002Shown") ? Visibility.Visible : Visibility.Hidden;
            this.MenuButton2.IsChecked = GetPropertyStringValue("XMMenuButton5002Toggled").Equals("1");
            this.MenuButton2.IsEnabled = !GetPropertyStringValue("XMMenuButton5002Toggled").Equals("2");
        }

        /// <summary>
        /// Updates menu button 3 control based on pertinent property watch values.
        /// Toggled status 1 denotes a toggled button, 2 denotes a disabled button.
        /// </summary>
        private void UpdateMenuButton3()
        {
            this.MenuButton3.Visibility = GetPropertyBoolValue("XMMenuButton5003Shown") ? Visibility.Visible : Visibility.Hidden;
            this.MenuButton3.IsChecked = GetPropertyStringValue("XMMenuButton5003Toggled").Equals("1");
            this.MenuButton3.IsEnabled = !GetPropertyStringValue("XMMenuButton5003Toggled").Equals("2");
        }

        /// <summary>
        /// Updates menu button 4 control based on pertinent property watch values.
        /// Toggled status 1 denotes a toggled button, 2 denotes a disabled button.
        /// </summary>
        private void UpdateMenuButton4()
        {
            this.MenuButton4.Visibility = GetPropertyBoolValue("XMMenuButton5004Shown") ? Visibility.Visible : Visibility.Hidden;
            this.MenuButton4.IsChecked = GetPropertyStringValue("XMMenuButton5004Toggled").Equals("1");
            this.MenuButton4.IsEnabled = !GetPropertyStringValue("XMMenuButton5004Toggled").Equals("2");
        }

        /// <summary>
        /// Click handler for the exit button.
        /// </summary>
        /// <param name="sender">Object event sender</param>
        /// <param name="e">RoutedEventArgs object</param>
        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {
            switch (this.viewModel.StateParam)
            {
                case "XMCashRemove":
                case "XMCashRemoveBNR":
                case "XMCashRemoveGlory":
                case "XMCashReplenish":
                    this.viewModel.ActionCommand.Execute("XMButton8");
                    break;
                case "XMCashStatus":
                case "XMCashStatusGlory":
                case "SmCardManagement":
                    this.viewModel.ActionCommand.Execute("SMButton8");
                    break;
                default:
                    break;
            }
        }

        /// <summary>
        /// Enabled property event handler for the numeric keypad.
        /// The XMNumericKey control is not functional and only tracks the state of the Cash Management numeric keypad.
        /// </summary>
        /// <param name="sender">Object event sender</param>
        /// <param name="e">DependencyPropertyChangedEventArgs object</param>
        private void XMNumericKey_IsEnabledChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            this.NumericPad.IsEnabled = (bool)e.NewValue;
            if (!NumericPad.IsEnabled)
            {
                NumericPad.InputTextBox.Clear();
            }
        }

        /// <summary>
        /// Loaded event handler for the numeric keypad.
        /// The XMNumericKey control is not functional and only tracks the state of the Cash Management numeric keypad.
        /// </summary>
        /// <param name="sender">Object event sender</param>
        /// <param name="e">RoutedEventArgs object</param>
        private void XMNumericKey_Loaded(object sender, RoutedEventArgs e)
        {
            var button = sender as ImageButton;
            if (null != button)
            {
                GridItem item = button.DataContext as GridItem;
                if (null != item)
                {
                    this.NumericPad.IsEnabled = item.IsEnabled;
                }
            }
        }
    }
}