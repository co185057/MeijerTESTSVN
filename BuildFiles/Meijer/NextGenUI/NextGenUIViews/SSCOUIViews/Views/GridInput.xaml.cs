using System;
using System.Windows;
using System.Windows.Controls;
using SSCOUIModels.Controls;
using FPsxWPF.Controls;
using SSCOUIModels;
using System.Globalization;
using System.Collections;
using System.Windows.Data;
using SSCOControls;
using System.Collections.Specialized;
using System.Collections.ObjectModel;
using SSCOControls.SlidingGridPageComponents;
using SSCOUIModels.Helpers;

namespace SSCOUIViews.Views
{
    /// <summary>
    /// Interaction logic for GridInput.xaml
    /// </summary>
    public partial class GridInput : BackgroundView
    {
        public GridInput(IMainViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
            ((INotifyCollectionChanged)ContainerButtonList.Items).CollectionChanged += ContainerButtonList_CollectionChanged;
        }

        private void GridInput_Unloaded(object sender, RoutedEventArgs e)
        {
            ((INotifyCollectionChanged)ContainerButtonList.Items).CollectionChanged -= ContainerButtonList_CollectionChanged;
        }

        public override void OnPropertyChanged(string name, object value)
        {
            if (name.Equals("LeadthruText"))
            {
                UpdateLeadthruText();
            }
            else if (name.Equals("MessageBoxNoEcho"))
            {
                UpdateMessageBoxNoEcho();
            }
            else if (name.Equals("ButtonGoBackShown"))
            {
                UpdateButtonGoBack();
            }
            else if (name.Equals("CMButton2MidiListShown"))
            {
                UpdateMidiList2();
            }
            else if (name.Equals("SMLeadthruText"))
            {
                UpdateSMLine1Text();
            }
        }

        public override void OnStateParamChanged(string param)
        {
            UpdateStateParam(param);
        }

        /// <summary>
        /// Executes the action for the tare container item.
        /// </summary>
        private void PickContainerListItem()
        {
            GridItem item = ContainerList.SelectedItem as GridItem;
            if (null != item)
            {
                if (item.IsEnabled)
                {
                    viewModel.ActionCommand.Execute(String.Format(CultureInfo.InvariantCulture, "SelectTare({0})", item.Data));
                }
            }
        }

        /// <summary>
        /// Executes the action for a container button list item (fuel and lottery)
        /// </summary>
        private void PickContainerButtonListItem()
        {
            switch (viewModel.StateParam)
            {
                case "CmLottery":
                    ExecuteCmdButtonList("CmdBtn_NormalLotteryList({0})");
                    break;
                default:
                    ExecuteCmdButtonList("DataNeededButtonList({0})");
                    break;
            }
        }

        private void ExecuteCmdButtonList(string param)
        {
            GridItem item = ContainerButtonList.SelectedItem as GridItem;
            if (null != item)
            {
                if (item.IsEnabled)
                {
                    viewModel.ActionCommand.Execute(String.Format(CultureInfo.InvariantCulture, param, item.Data));
                }
            }
        }

        private void UpdateLeadthruText()
        {
            switch (this.viewModel.StateParam)
            {
                case "CmPrePayFuel":
                case "CmPrePayFuelAmount":
                case "CmLottery":
                case "SelectTare":
                case "CmDataEntryWithButtonList":
                case "CmDataEntryWithCmdList":
                    GridInputHeader.Text = this.GetPropertyStringValue("LeadthruText");
                    break;
            }
        }

        private void UpdateMessageBoxNoEcho()
        {
            switch (this.viewModel.StateParam)
            {
                case "CmPrePayFuel":
                case "CmPrePayFuelAmount":
                case "CmLottery":
                case "CmDataEntryWithButtonList":
                case "CmDataEntryWithCmdList":
                    GridInputSubMessage.Text = this.GetPropertyStringValue("MessageBoxNoEcho");
                    break;
            }
        }

        private void UpdateButtonGoBack()
        {
            switch (this.viewModel.StateParam)
            {
                case "CmPrePayFuel":
                case "CmPrePayFuelAmount":
                case "CmLottery":
                case "SelectTare":
                case "CmDataEntryWithButtonList":
                case "CmDataEntryWithCmdList":
                    goBackButton.Visibility = (this.GetPropertyBoolValue("ButtonGoBackShown")) ? Visibility.Visible : Visibility.Collapsed;
                    break;
                case "SmDataEntryWithCmdList":
                case "AM_SelectTare":
                    goBackButton.Visibility = Visibility.Collapsed;
                    break;
            }
        }

        private void UpdateMidiList2()
        {
            switch (this.viewModel.StateParam)
            {
                case "CmPrePayFuel":
                case "CmPrePayFuelAmount":
                case "CmLottery":
                case "SelectTare":
                case "CmDataEntryWithButtonList":
                case "CmDataEntryWithCmdList":
                    cmBackButton.Visibility = (this.GetPropertyBoolValue("CMButton2MidiListShown")) ? Visibility.Visible : Visibility.Collapsed;
                    break;
                case "SmDataEntryWithCmdList":
                case "AM_SelectTare":
                    cmBackButton.Visibility = Visibility.Collapsed;
                    break;
            }
        }

        private void UpdateHeader()
        {
            switch (this.viewModel.StateParam)
            {
                case "SmDataEntryWithCmdList":
                case "AM_SelectTare":
                    this.ShowHeader = true;
                    break;
                default:
                    this.ShowHeader = false;
                    break;
            }
        }

        private void UpdateStateParam(string param)
        {
            UpdateLeadthruText();
            UpdateMessageBoxNoEcho();
            UpdateButtonGoBack();
            UpdateMidiList2();
            UpdateHeader();
            UpdateSMLine1Text();
            UpdateSMLine2Text();

            switch (param)
            {
                case "SelectTare":
                    ContainerListStackPanel.Visibility = Visibility.Visible;
                    ButtonListStackPanel.Visibility = Visibility.Collapsed;
                    CmdListStackPanel.Visibility = Visibility.Collapsed;
                    GridInputHeader.Visibility = Visibility.Visible;
                    GridInputSubMessage.Visibility = Visibility.Collapsed;
                    break;
                case "CmPrePayFuel":
                case "CmPrePayFuelAmount":
                case "CmLottery":
                case "CmDataEntryWithButtonList":
                    ContainerListStackPanel.Visibility = Visibility.Collapsed;
                    ButtonListStackPanel.Visibility = Visibility.Visible;
                    CmdListStackPanel.Visibility = Visibility.Collapsed;
                    GridInputHeader.Visibility = Visibility.Visible;
                    GridInputSubMessage.Visibility = Visibility.Visible;
                    UpdateBtnListItemsSource();
                    break;
                case "CmDataEntryWithCmdList":
                    ContainerListStackPanel.Visibility = Visibility.Collapsed;
                    ButtonListStackPanel.Visibility = Visibility.Collapsed;
                    CmdListStackPanel.Visibility = Visibility.Visible;
                    GridInputHeader.Visibility = Visibility.Visible;
                    GridInputSubMessage.Visibility = Visibility.Visible;
                    UpdateCmdListItemsSource();
                    UpdateCmdListPanel();
                    break;
                case "SmDataEntryWithCmdList":
                case "AM_SelectTare":
                    ContainerListStackPanel.Visibility = Visibility.Collapsed;
                    ButtonListStackPanel.Visibility = Visibility.Collapsed;
                    CmdListStackPanel.Visibility = Visibility.Visible;
                    GridInputHeader.Visibility = Visibility.Collapsed;
                    GridInputSubMessage.Visibility = Visibility.Collapsed;
                    UpdateCmdListItemsSource();
                    UpdateCmdListPanel();
                    break;
                default:
                    break;
            }
        }

        private void UpdateSMLine1Text()
        {
            switch (viewModel.StateParam)
            {
                case "SmDataEntryWithCmdList":
                    this.SMLine1Text.Text = GetPropertyStringValue("SMLeadthruText");
                    break;
                case "AM_SelectTare":
                    this.SMLine1Text.Property(TextBlock.TextProperty).SetResourceValue("SelectContainerTitle");
                    break;
                default:
                    this.SMLine1Text.Property(TextBlock.TextProperty).Clear();
                    this.SMLine1Text.Text = String.Empty;
                    break;
            }
        }

        private void UpdateSMLine2Text()
        {
            switch (viewModel.StateParam)
            {
                case "AM_SelectTare":
                    this.SMLine2Text.Property(TextBlock.TextProperty).SetResourceValue("SelectContainerLeadthru");
                    break;
                default:
                    this.SMLine2Text.Property(TextBlock.TextProperty).Clear();
                    this.SMLine2Text.Text = String.Empty;
                    break;
            }
        }

        private void UpdateCmdListPanel()
        {
            switch (viewModel.StateParam)
            {
                case "SmDataEntryWithCmdList":
                case "AM_SelectTare":
                    this.CmdListStackPanel.Style = null;
                    this.CmdListBorder.Margin = new Thickness(24, 0, 24, 0);
                    this.CmdListBorder.Style = this.FindResource("glowBorderStyle") as Style;
                    this.ContainerCmdList.Height = 430;
                    this.InstructionTextPanel.Visibility = Visibility.Visible;
                    break;
                default:
                    this.CmdListStackPanel.Style = this.FindResource("cmdListStackPanelStyle") as Style;
                    this.CmdListBorder.Margin = new Thickness(0, 0, 0, 0);
                    this.CmdListBorder.Style = this.FindResource("containerCmdListBorderStyle") as Style;
                    this.ContainerCmdList.Height = 522;
                    this.InstructionTextPanel.Visibility = Visibility.Collapsed;
                    break;
            }
        }

        private void UpdateCmdListItemsSource()
        {
            switch (viewModel.StateParam)
            {
                case "CmDataEntryWithCmdList":
                case "SmDataEntryWithCmdList":
                    SetCmdListItemSource(viewModel.GetPropertyValue("DataEntryCmdList") as IEnumerable);
                    break;
                case "AM_SelectTare":
                    SetCmdListItemSource(viewModel.GetPropertyValue("DataEntryTareList") as IEnumerable);
                    break;
            }
        }

        private void SetCmdListItemSource(IEnumerable receiptItems)
        {
            Binding receiptBinding = new Binding();
            receiptBinding.Source = receiptItems;
            BindingOperations.SetBinding(this.ContainerCmdList, TouchListBox.ItemsSourceProperty, receiptBinding);
        }

        private void UpdateBtnListItemsSource()
        {
            switch (viewModel.StateParam)
            {
                case "CmLottery":
                    SetBtnListItemsSource(viewModel.GetPropertyValue("LotteryButtonList") as IEnumerable);
                    break;
                case "CmPrePayFuel":
                case "CmPrePayFuelAmount":
                case "CmDataEntryWithButtonList":
                    SetBtnListItemsSource(viewModel.GetPropertyValue("DataEntryButtonList") as IEnumerable);
                    break;
            }
        }

        private void SetBtnListItemsSource(IEnumerable btnList)
        {
            Binding btnListBinding = new Binding();
            btnListBinding.Source = btnList;
            BindingOperations.SetBinding(this.ContainerButtonList, SlidingGridPage.ItemsSourceProperty, btnListBinding);
        }

        private void ContainerButtonList_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Reset)
            {
                UpdateBtnListItemsSource();
            }
        }

        /// <summary>
        /// Makes sure the correct action sequence is executed by the CmdListItemConfirm button.
        /// </summary>
        private void ExecuteCmdListItemConfirmAction()
        {
            switch (viewModel.StateParam)
            {
                case "CmDataEntryWithCmdList":
                    viewModel.ActionCommand.Execute(string.Format(CultureInfo.InvariantCulture, "DataNeededCmdList({0})", ContainerCmdList.SelectedIndex));
                    break;
                case "SmDataEntryWithCmdList":
                    viewModel.ActionCommand.Execute(string.Format(CultureInfo.InvariantCulture, "DataNeededStoreCmdList({0})", ContainerCmdList.SelectedIndex));
                    break;
                case "AM_SelectTare":
                    viewModel.ActionCommand.Execute(string.Format(CultureInfo.InvariantCulture, "SMSelectReceiptItem({0})", ContainerCmdList.SelectedIndex));
                    break;
            }
        }

        /// <summary>
        /// Fires when CommandListItemConfirm is clicked.
        /// </summary>
        /// <param name="sender">object sender</param>
        /// <param name="e">event arguments</param>
        private void CmdListItemConfirm_Click(object sender, RoutedEventArgs e)
        {
            ExecuteCmdListItemConfirmAction();
        }

        /// <summary>
        /// Fires when CmdListItemConfirm is touched
        /// </summary>
        /// <param name="sender">object sender</param>
        /// <param name="e">event arguments</param>
        private void CmdListItemConfirm_TouchDown(object sender, System.Windows.Input.TouchEventArgs e)
        {
            e.Handled = true;
        }

        /// <summary>
        /// Click handler for the container button list sliding grid page (fuel and lottery)
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void ContainerButtonList_ItemClicked(object sender, EventArgs e)
        {
            PickContainerButtonListItem();
        }

        /// <summary>
        /// Click handler for the container list sliding grid page
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void ContainerList_ItemClicked(object sender, EventArgs e)
        {
            PickContainerListItem();
        }

        /// <summary>
        /// Handler for when the view is loaded.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void BackgroundView_Loaded(object sender, RoutedEventArgs e)
        {
            if (viewModel.IsActiveBackground)
            {
                FocusHelper.SetInitialFocus(this, viewModel);
            }
        }
    }
}

