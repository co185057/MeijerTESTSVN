using FPsxWPF.Controls;
using SSCOUIModels;
using SSCOUIModels.Controls;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using SSCOControls;

namespace SSCOUIViews.Views
{
    /// <summary>
    /// Interaction logic for CashManagementConfirmAction.xaml
    /// </summary>
    public partial class CashManagementConfirmAction : BackgroundView
    {
        private ObservableCollection<ReceiptItem> loaderObject = new ObservableCollection<ReceiptItem>();

        public CashManagementConfirmAction(IMainViewModel viewModel)
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
        private void ConfirmAction_Unloaded(object sender, RoutedEventArgs e)
        {
            loaderObject.CollectionChanged -= loaderObject_CollectionChanged;
        }

        public override void OnPropertyChanged(string name, object value)
        {
            switch (name)
            {
                case "XMLeadthruText":
                    UpdateLeadthruText();
                    break;
                case "XMLeadthruTextV6HWText":
                    UpdateLeadthruTextV6HW();
                    break;
                case "XMPendingActions":
                    UpdateXMPendingActions();
                    break;
                case "XMPendingActionsV6HWShown":
                case "XMPendingActionsV6HW":
                    UpdateXMPendingActionsV6HW();
                    break;
                case "XMSetupLoaderConfirmTableV6HWShown":
                    UpdateSetupLoaderTable();
                    break;
                case "XMSetupLoaderConfirmTextV6HWShown":
                case "XMSetupLoaderConfirmTextV6HW":
                    UpdateXMSetupLoaderConfirmTextV6HW();
                    break;
            }
        }

        public override void OnStateParamChanged(string param)
        {
            UpdateStateParam(param);
        }

        private void UpdateStateParam(string param)
        {
            UpdateElements();
            UpdateStyles();
            UpdateSubTitleText();
            UpdatePendingActionsText();
            UpdateYesNoButtonCommandParameter();
            UpdateSetupLoaderTable();
            UpdateXMSetupLoaderConfirmTextV6HW();
        }

        private void UpdateElements()
        {
            switch (viewModel.StateParam)
            {
                case "XMConfirmAction":
                    this.CMConfirmActionGrid.ColumnDefinitions[0].Width = new GridLength(684);
                    this.CMConfirmActionGrid.ColumnDefinitions[1].Width = new GridLength(340);
                    this.CMTextArea.RowDefinitions[1].Height = new GridLength(548);
                    break;
                case "XMConfirmActionV6HW":
                    this.CMConfirmActionGrid.ColumnDefinitions[0].Width = new GridLength(748);
                    this.CMConfirmActionGrid.ColumnDefinitions[1].Width = new GridLength(276);
                    this.CMTextArea.RowDefinitions[1].Height = new GridLength(552);
                    break;
            }
        }

        private void UpdateStyles()
        {
            switch (this.viewModel.StateParam)
            {
                case "XMConfirmAction":
                    this.XMConfirmActionSubTitleText.Style = this.FindResource("cashManagementSubTitleTextStyle") as Style;
                    this.XMPendingActionsText.Style = this.FindResource("cashManagementInfoTextStyle") as Style;
                    break;
                case "XMConfirmActionV6HW":
                    this.XMConfirmActionSubTitleText.Style = this.FindResource("cashManagementSubTitleV6HWTextStyle") as Style;
                    this.XMPendingActionsText.Style = this.FindResource("cashManagementInfoV6HWTextStyle") as Style;
                    break;
            }
        }

        private void UpdateSubTitleText()
        {
            switch (this.viewModel.StateParam)
            {
                case "XMConfirmAction":
                    UpdateLeadthruText();
                    break;
                case "XMConfirmActionV6HW":
                    UpdateLeadthruTextV6HW();
                    break;
            }           
        }

        private void UpdatePendingActionsText()
        {
            switch (this.viewModel.StateParam)
            {
                case "XMConfirmAction":
                    UpdateXMPendingActions();
                    break;
                case "XMConfirmActionV6HW":
                    UpdateXMPendingActionsV6HW();
                    break;
            }
        }

        private void UpdateLeadthruText()
        {
            XMConfirmActionSubTitleText.Text = GetPropertyStringValue("XMLeadthruText");
        }

        private void UpdateLeadthruTextV6HW()
        {
            XMConfirmActionSubTitleText.Text = GetPropertyStringValue("XMLeadthruTextV6HWText");
        }

        private void UpdateXMPendingActions()
        {
            XMPendingActionsText.Text = GetPropertyStringValue("XMPendingActions");
        }

        private void UpdateXMPendingActionsV6HW()
        {
            if (GetPropertyBoolValue("XMPendingActionsV6HWShown") && !GetPropertyBoolValue("XMSetupLoaderConfirmTextV6HWShown"))
            {
                XMPendingActionsText.Text = GetPropertyStringValue("XMPendingActionsV6HW");
            }
        }

        private void UpdateXMSetupLoaderConfirmTextV6HW()
        {
            if (GetPropertyBoolValue("XMSetupLoaderConfirmTextV6HWShown"))
            {
                XMPendingActionsText.Text = GetPropertyStringValue("XMSetupLoaderConfirmTextV6HW");
            }
        }

        private void UpdateYesNoButtonCommandParameter()
        {
            switch (viewModel.StateParam)
            {
                case "XMConfirmAction":
                    this.Yes_Button.CommandParameter = "XMButton1";
                    this.No_Button.CommandParameter = "XMButton2";
                    break;
                case "XMConfirmActionV6HW":
                    this.Yes_Button.CommandParameter = "XMButton1V6HW";
                    this.No_Button.CommandParameter = "XMButton2V6HW";
                    break;               
            }
        }

        /// <summary>
        /// Updates the setup loader table with the corresponding values
        /// </summary>
        private void UpdateSetupLoaderTable()
        {
            if (GetPropertyBoolValue("XMSetupLoaderConfirmTableV6HWShown"))
            {
                CashManagementTable.Visibility = Visibility.Visible;
                XMPendingActionsTextArea.SetValue(Grid.RowProperty, 1);
                loaderObject = (ObservableCollection<ReceiptItem>)viewModel.GetPropertyValue("XMSetupLoaderConfirmTableV6HW");
                loaderObject.CollectionChanged += loaderObject_CollectionChanged;
                SetupLoaderTable.ItemsSource = SetupLoaderTable.PopulateLoaderTable(loaderObject);
                XMPendingActionsText.Style = TryFindResource("cashManagementSetUpLoaderV6HWTextStyle") as Style;
            }
            else
            {
                CashManagementTable.Visibility = Visibility.Collapsed;
                XMPendingActionsTextArea.SetValue(Grid.RowProperty, 0);
                XMPendingActionsText.Style = TryFindResource("cashManagementInfoV6HWTextStyle") as Style;
            }
        }
    }
}
