using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Globalization;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Data;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media.Media3D;
using SSCOUIModels.Controls;
using SSCOUIModels.Helpers;
using SSCOUIModels;
using SSCOUIModels.Models;
using FPsxWPF.Controls;
using RPSWNET;
using PsxNet;
using SSCOControls;
using SSCOControls.SlidingGridPageComponents;
using System.Reflection;
using SSCOUIViews.Controls;

namespace SSCOUIViews.Views
{
    /// <summary>
    /// Interaction logic for Sale.xaml
    /// </summary>
    public partial class Sale : BackgroundView
    {
        public Sale(IMainViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
            if (CmDataCapture.IsCaptureActive(CmDataCapture.MaskExtensive))
            {
                viewModel.PropertyChanged += viewModel_PropertyChanged;
            }
        }

        private void CustomerReceipt_CurrentChanged(object sender, EventArgs e)
        {
            this.currentReceiptItem = CollectionViewSource.GetDefaultView(this.customerReceiptCollection).CurrentItem as CustomerReceiptItem;
            ShowStatus();
        }
        /// <summary>
        /// Refactory & added case ("ShowTransitionEffects")
        /// Added ShowElements() to case ("ShowTransitionEffects")
        /// </summary>
        /// <param name="name">Name of changed property</param>
        /// <param name="value">Value of changed property</param>
        public override void OnPropertyChanged(string name, object value)
        {
            switch (name)
            {
                case ("AttendantMode"):
                    UpdateCardPanel();
                    break;
                case ("NextGenData"):
                    OnNextGenDataChanged();
                    break;
                case ("CustomerReceipt"):
                    OnCustomerReceiptChanged();
                    break;                
                case ("CMButton1MedShown"):
                    UpdateSkipBaggingPanel();
                    break;
                case ("ScanBagTextShown"):
                    ShowCardMessage();
                    break;
                case ("ScanBagTextArea"):
                    ShowCardMessage();
                    break;
                case ("ShowTransitionEffects"):
                    UpdateCardPanel();
                    ShowElements();
                    break;
                case "NextGenUIAllowKeyInCode":
                case "NextGenUIAllowLookUpItem":
                case "NextGenUIAllowPickListSearchButton":
                    UpdateSearchButtonTextAndCommand();
                    break;
                case ("MPerksScanAndBagEnabled"):
                    UpdatemPerksButton(); // SSCOI-44701: Call the helper method to Enable/Disable the mPerks button
                    break;
                case ("WicEbtScanAndBagEnabled"):
                    UpdateWicEBTButton();
                    break;
                case "MPerksScanAndBagShown":
                    if (this.viewModel.State.Equals("scan", StringComparison.OrdinalIgnoreCase))
                    {
                        MPerksScanAndBagButton.Visibility = GetPropertyBoolValue("MPerksScanAndBagShown") ? Visibility.Visible : Visibility.Collapsed;
                        if(MPerksScanAndBagButton.Visibility == System.Windows.Visibility.Visible)
                            MPerksScanAndBagButton.IsEnabled = GetPropertyBoolValue("MPerksScanAndBagEnabled");
                    }
                    break;
                case "PrintGiftReceiptShown":
                    if (this.viewModel.State.Equals("scan", StringComparison.OrdinalIgnoreCase))
                        PrintGiftReceiptButton.Visibility = GetPropertyBoolValue("PrintGiftReceiptShown") ? Visibility.Visible : Visibility.Collapsed;
                    break;
                case "WicEbtScanAndBagShown":
                    if (this.viewModel.State.Equals("scan", StringComparison.OrdinalIgnoreCase))
                    {
                        if (GetPropertyStringValue("WICFeatureEnable").Equals("0"))
                        {
                            WICEBTScanAndBagButton.Visibility = Visibility.Collapsed;
                            //Grid.SetColumn(WICEBTScanAndBagButton, 3);
                            Grid.SetColumn(PrintGiftReceiptButton, 1);
                            Grid.SetColumn(OwnBagButton, 2);
                        }
                        else
                        {
                            WICEBTScanAndBagButton.Visibility = GetPropertyStringValue("TerminalType").Equals("SS90", StringComparison.OrdinalIgnoreCase) ? Visibility.Collapsed : Visibility.Visible;
                            WICEBTScanAndBagButton.IsEnabled = GetPropertyBoolValue("WicEbtScanAndBagEnabled");
                        }
                    }
                    break;
            }
			// Start CRD 125377
            CmDataCapture.Capture(CmDataCapture.MaskInfo, String.Format("*_* SSCOUIViews.Views.Sale.xaml.cs.OnPropertyChanged - transactionMode({0})", GetPropertyStringValue("TransactionMode").ToString()));
            //if (!GetPropertyStringValue("ShopAndDropEnabled").Equals("0"))
            if (GetPropertyStringValue("TransactionMode").Equals("1"))
            {
                scanItemSearchKeyInItem.Visibility = Visibility.Collapsed;
                WICEBTScanAndBagButton.Visibility = Visibility.Collapsed;
                PrintGiftReceiptButton.Visibility = Visibility.Collapsed;
                MPerksScanAndBagButton.Visibility = Visibility.Collapsed;
                this.ScanItemBorder.Visibility = Visibility.Collapsed;

                this.CardPanel.Visibility = Visibility.Collapsed;
                this.NoItemsPanel.Visibility = Visibility.Visible;
                this.NoItemsPrompt.Visibility = Visibility.Collapsed;
                SNDNoItemsPrompt.Visibility = Visibility.Visible;
                SDNLeadthruText.Visibility = Visibility.Visible;

                if (!GetPropertyStringValue("ShopAndDropEnabledYesNoButtons").Equals("0"))
                {
                    SNDYesButton.Visibility = Visibility.Visible;
                    SNDNoButton.Visibility = Visibility.Visible;
                }
                else
                {
                    SNDYesButton.Visibility = Visibility.Collapsed;
                    SNDNoButton.Visibility = Visibility.Collapsed;
                }
            }
            // End CRD 125377 
            else
            {
                if (GetPropertyStringValue("TransactionMode").Equals("0"))
                {
                    CmDataCapture.Capture(CmDataCapture.MaskInfo, String.Format("*_*SSCOUIViews.Views.Sale.xaml.cs set border to Visible "));
                    this.ScanItemBorder.Visibility = Visibility.Visible;
                }       
            }
        }

        public override void OnStateParamChanged(String param)
        {
            this.cancelCoupons = false;
            UpdateCardPanel();
            OnCustomerReceiptChanged();
            ShowCardMessage();
            ShowElements();
            UpdateSkipBaggingPanel();
            ShowFuelAndLottery();
            UpdatemPerksButton(); // SSCOI-44701: Call the helper method to Enable/Disable the mPerks button
            UpdateWicEBTButton();
            //Start CRD 127035 meijerGo only mode
            CmDataCapture.Capture(CmDataCapture.MaskInfo, String.Format("*_* SSCOUIViews.Views.Sale.xaml.cs.OnPropertyChanged - transactionMode({0})", GetPropertyStringValue("TransactionMode").ToString()));
            if (GetPropertyStringValue("TransactionMode").Equals("2"))
            {
                onMeijerGoSNB();
            }
            //End CRD 127035
            // Start CRD 125377
            //if (!GetPropertyStringValue("ShopAndDropEnabled").Equals("0"))
            else if (GetPropertyStringValue("TransactionMode").Equals("1"))
            {
                scanItemSearchKeyInItem.Visibility = Visibility.Collapsed;
                WICEBTScanAndBagButton.Visibility = Visibility.Collapsed;
                PrintGiftReceiptButton.Visibility = Visibility.Collapsed;
                MPerksScanAndBagButton.Visibility = Visibility.Collapsed;
                ScanItemBorder.Visibility = Visibility.Collapsed;

                this.CardPanel.Visibility = Visibility.Collapsed;
                this.CardMessagePanel.Visibility = Visibility.Collapsed;
                this.NoItemsPanel.Visibility = Visibility.Visible;
                this.NoItemsPrompt.Visibility = Visibility.Collapsed;
                SNDNoItemsPrompt.Visibility = Visibility.Visible;
                SDNLeadthruText.Visibility = Visibility.Visible;

                if (!GetPropertyStringValue("ShopAndDropEnabledYesNoButtons").Equals("0"))
                {
                    SNDYesButton.Visibility = Visibility.Visible;
                    SNDNoButton.Visibility = Visibility.Visible;
                }
                else
                {
                    SNDYesButton.Visibility = Visibility.Collapsed;
                    SNDNoButton.Visibility = Visibility.Collapsed;
                }
            }
            // End CRD 125377 
            else //transaction mode 0
            {
                //CRD 127513
                ScanItemBorder.Visibility = Visibility.Visible;
                scanItemSearchKeyInItem.Visibility = WICEBTScanAndBagButton.Visibility = GetPropertyStringValue("TerminalType").Equals("SS90", StringComparison.OrdinalIgnoreCase) ? Visibility.Collapsed : Visibility.Visible;
                PrintGiftReceiptButton.Visibility = Visibility.Visible;
                MPerksScanAndBagButton.Visibility = Visibility.Visible;
                MPerksScanAndBagButton.IsEnabled = GetPropertyBoolValue("MPerksScanAndBagEnabled");
                ScanItemBorder.Visibility = Visibility.Visible;
                SNDNoItemsPrompt.Visibility = Visibility.Collapsed;
                SDNLeadthruText.Visibility = Visibility.Collapsed;
            }

            OnNextGenDataChanged();
            SetSNBApplyMeijerCardButton();
        }

        private void CancelCoupon_Click(object sender, RoutedEventArgs e)
        {
            this.cancelCoupons = true;
            ShowElements();
            viewModel.ActionCommand.Execute("CancelCoupon");
        }

        private void CustomerReceipt_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if ((NotifyCollectionChangedAction.Add == e.Action || NotifyCollectionChangedAction.Remove == e.Action || NotifyCollectionChangedAction.Reset == e.Action) &&
                (0 == this.customerReceiptCollection.Count || 1 == this.customerReceiptCollection.Count))
            {
                ShowElements();
            }
        }

        private void IsQuickPick()
        {
            ShowCardMessage();
            ShowElements();
        }

        private void UpdateCardPanel()
        {
            this.allowFilmstrip = (this.viewModel.ShowTransitionEffects && !this.viewModel.AttendantMode &&
                Properties.Settings.Default.ShowFilmstrip);
            if ((this.allowFilmstrip && Visibility.Collapsed == this.FilmstripPanel.Visibility) ||
                !this.allowFilmstrip && Visibility.Collapsed == this.SingleCardPanel.Visibility)
            {
                this.FilmstripPanel.Visibility = this.allowFilmstrip ? Visibility.Visible : Visibility.Collapsed;
                this.SingleCardPanel.Visibility = this.allowFilmstrip ? Visibility.Collapsed : Visibility.Visible;
                if (this.allowFilmstrip)
                {
                    BindingOperations.SetBinding(this.FilmstripPanel, ItemsControl.ItemsSourceProperty,
                        new Binding() { Source = this.viewModel.GetPropertyValue("CustomerReceipt") });
                }
                else
                {
                    BindingOperations.ClearBinding(this.FilmstripPanel, ItemsControl.ItemsSourceProperty);
                }
            }
        }

        private void OnCustomerReceiptChanged()
        {
            ObservableCollection<ReceiptItem> customerReceiptCollection = this.viewModel.GetPropertyValue("CustomerReceipt") as ObservableCollection<ReceiptItem>;
            if (customerReceiptCollection != this.customerReceiptCollection)
            {
                if (null != this.customerReceiptCollection)
                {
                    CollectionViewSource.GetDefaultView(this.customerReceiptCollection).CurrentChanged -= CustomerReceipt_CurrentChanged;
                    this.customerReceiptCollection.CollectionChanged -= CustomerReceipt_CollectionChanged;
                    this.currentReceiptItem = null;
                }
                this.customerReceiptCollection = customerReceiptCollection;
                if (null != this.customerReceiptCollection)
                {
                    this.customerReceiptCollection.CollectionChanged += CustomerReceipt_CollectionChanged;
                    CollectionViewSource.GetDefaultView(this.customerReceiptCollection).CurrentChanged += CustomerReceipt_CurrentChanged;
                    if (this.allowFilmstrip)
                    {
                        BindingOperations.SetBinding(this.FilmstripPanel, ItemsControl.ItemsSourceProperty,
                            new Binding() { Source = this.viewModel.GetPropertyValue("CustomerReceipt") });
                    }
                }
            }
        }



        private void OnNextGenDataChanged()
        {
            bool enabled = !GetPropertyStringValue("NextGenData").Equals("BagAndEAS");

            this.scanItemSearchKeyInItem.IsEnabled = enabled;
            this.FilmstripPanel.IsEnabled = enabled;
            if (enabled)
                UpdatemPerksButton();
            else
                this.MPerksScanAndBagButton.IsEnabled = enabled;
            this.PrintGiftReceiptButton.IsEnabled = enabled;
            this.WICEBTScanAndBagButton.IsEnabled = enabled;

            // this fixes focus being lost when bagging is off and a 'normal' quickpick item is ordered.
            if (enabled)
            {
                FocusHelper.SetInitialFocus(this, viewModel);
            }

        }

        private void ShowCardMessage()
        {
            //CRD 127035 meijerGo only mode
            this.SS90NoItemsContent.Visibility = Visibility.Collapsed;
            if (GetPropertyStringValue("TransactionMode").Equals("2"))
            {
                this.SS90NoItemsContent.Visibility = Visibility.Visible;
            }
			// start CRD 125377
            //if (!GetPropertyStringValue("ShopAndDropEnabled").Equals("0"))
            else if (GetPropertyStringValue("TransactionMode").Equals("1"))
            {
                this.QuickPickNoItemsContent.Visibility = Visibility.Collapsed;
                this.NoItemsContent.Visibility = Visibility.Collapsed;
                this.CardMessagePanel.Visibility = Visibility.Collapsed;
                this.NoItemsPanel.Visibility = Visibility.Visible;
                this.NoItemsPrompt.Visibility = Visibility.Collapsed;
                SNDNoItemsPrompt.Visibility = Visibility.Visible;
                SDNLeadthruText.Visibility = Visibility.Visible;
            }
            // end CRD 125377
            else
            {
                this.CardMessagePanel.Visibility = (Boolean)this.viewModel.GetPropertyValue("ScanBagTextShown") && this.viewModel.GetPropertyValue("ScanBagTextArea").ToString().Length != 0 ? Visibility.Visible : Visibility.Collapsed;
                this.NoItemsContent.Visibility = Visibility.Collapsed == this.CardMessagePanel.Visibility && !this.isQuickPick ?
                    Visibility.Visible : Visibility.Collapsed;
                this.QuickPickNoItemsContent.Visibility = Visibility.Collapsed == this.CardMessagePanel.Visibility && this.isQuickPick ?
                    Visibility.Visible : Visibility.Collapsed;
            }
        }

        private void ShowElements()
        {
            //Start CRD 127035 meijerGo only mode
			if(GetPropertyStringValue("TransactionMode").Equals("2"))
			{
                this.SNDNoItemsPrompt.Visibility = Visibility.Collapsed;
                this.NoItemsContent.Style = FindResource("can-scanItemAnimationControlStyle") as Style;
                this.NoItemsPrompt.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9387");
                this.isQuickPick = false;
            }
            //EndCRD 127035 meijerGo only mode
            // start CRD 125377
            else if (GetPropertyStringValue("TransactionMode").Equals("1"))
            {
                this.EnterCouponsPanel.Visibility = Visibility.Collapsed;
                this.QuickPickListPanel.Visibility = Visibility.Collapsed;
                this.NoItemsPrompt.Visibility = Visibility.Collapsed;
                this.SNDNoItemsPrompt.Visibility = Visibility.Visible;
                this.CardPanel.Visibility = Visibility.Collapsed;
                this.isQuickPick = false;
            }
            else
            {
                //CRD 127513 transaction mode 0
                this.SNDNoItemsPrompt.Visibility = Visibility.Collapsed;
                this.NoItemsPrompt.Visibility = Visibility.Visible;
                this.isQuickPick = true;
            }
            // end CRD 125377

            this.NoItemsPanel.Height = this.isQuickPick ? 320 : 458;
            if (this.allowFilmstrip)
            {
                this.FilmstripPanel.Camera.Position = this.isQuickPick ? new Point3D(0, 0, 1.8) : new Point3D(0, 0, 1.2);
            }
            this.CardPanel.Height = this.FilmstripPanel.ElementHeight = this.isQuickPick ? 320 : 458;
            this.EnterCouponsPanel.Visibility = this.viewModel.StateParam.Equals("EnterCoupons") && !this.cancelCoupons ?
                Visibility.Visible : Visibility.Collapsed;
            this.QuickPickListPanel.Visibility = this.isQuickPick && Visibility.Collapsed == this.EnterCouponsPanel.Visibility ?
                Visibility.Visible : Visibility.Collapsed;
            this.NoItemsPanel.Visibility = (null == this.customerReceiptCollection || this.customerReceiptCollection.Count == 0) &&
                Visibility.Collapsed == this.EnterCouponsPanel.Visibility ? Visibility.Visible : Visibility.Collapsed;
            this.CardPanel.Visibility = Visibility.Collapsed == this.NoItemsPanel.Visibility && Visibility.Collapsed == this.EnterCouponsPanel.Visibility ?
                Visibility.Visible : Visibility.Collapsed;
            this.CardMessage.IsQuickPick = this.isQuickPick;
            

            ShowStatus();
        }

        private void ShowStatus()
        {
            this.skipBagging.Visibility = this.viewModel.StateParam.Equals("Bag") ? Visibility.Visible : Visibility.Collapsed;
            this.ScanItemTextBlock.Property(TextBlock.TextProperty).SetResourceValue(null != this.currentReceiptItem && this.currentReceiptItem.IsIntervention ?
                "AssistanceComing" : "ScanNextItem");
            this.scanItemStatus.Visibility = (Visibility.Visible != this.skipBagging.Visibility && Visibility.Visible != this.EnterCouponsPanel.Visibility) ?
                Visibility.Visible : Visibility.Collapsed;
				// start CRD 125377
            //if (!GetPropertyStringValue("ShopAndDropEnabled").Equals("0"))
            if (GetPropertyStringValue("TransactionMode").Equals("1"))
            {
                this.skipBagging.Visibility = Visibility.Collapsed;
                this.ScanItemTextBlock.Property(TextBlock.TextProperty).SetResourceValue(null != this.currentReceiptItem && this.currentReceiptItem.IsIntervention ?
                    "AssistanceComing" : "ScanNextItem");
                this.scanItemStatus.Visibility = Visibility.Collapsed;
            }
            // end CRD 125377
        }

        private void ShowFuelAndLottery()
        {
            int fuelEnabled = 0;
            int lottoEnabled = 0;
            int.TryParse(this.viewModel.GetPropertyValue("NextGenUIFuelDemoEnabled") as string, out fuelEnabled);
            int.TryParse(this.viewModel.GetPropertyValue("NextGenUILottoDemoEnabled") as string, out lottoEnabled);
            bool show = fuelEnabled == -1 || lottoEnabled == -1;

            if (show)
            {
                QuickPickListPanel.ColumnDefinitions[0].Width = new GridLength(445);
                QuickPickListPanel.ColumnDefinitions[1].Width = new GridLength(200);
            }
            else
            {
                QuickPickListPanel.ColumnDefinitions[0].Width = new GridLength(647);
                QuickPickListPanel.ColumnDefinitions[1].Width = new GridLength(0);
            }
        }

        private void quickPickCollection_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (NotifyCollectionChangedAction.Add == e.Action || NotifyCollectionChangedAction.Remove == e.Action || NotifyCollectionChangedAction.Reset == e.Action)
            {
                IsQuickPick();
            }
        }



        private void QuickPickItemList_LogEvent(object sender, LogEventArgs e)
        {
            if (e.LogMessage.Equals("StartItemListRender"))
            {
                viewModel.Perfcheck.StartEventLog(PerfMeasureEvents.PickListItemsRender, string.Empty);
            }
            else if (e.LogMessage.Equals("EndItemListRender"))
            {
                viewModel.Perfcheck.EndEventLog(PerfMeasureEvents.PickListItemsRender, string.Format("Items rendered: {0}", e.RenderedItemCount));
            }
        }

        /// <summary>
        /// Executes the action for a quick pick item
        /// </summary>
        /// <param name="item">quick pick item</param>
        private void PickQuickPickItem(GridItem item)
        {
            if (item != null)
            {
                viewModel.ActionCommand.Execute(String.Format(CultureInfo.CurrentCulture, "QuickPickItem({0})", item.Data));
            }
        }

        /// <summary>
        /// Update SkipBagging panel
        /// </summary>
        private void UpdateSkipBaggingPanel()
        {
            if (viewModel.StateParam.Equals("Bag"))
            {
                bool isTABConfigured = GetStringPropertyBoolValue("NextGenUITABConfigured");

                if (GetPropertyBoolValue("CMButton1MedShown"))
                {
                    string stringResource = isTABConfigured ? "BagItemOrSkipTAB" : "BagItemOrSkip";
                    skipBaggingText.Property(TextBlock.TextProperty).SetResourceValue(stringResource);
                    skipBaggingButton.Visibility = Visibility.Visible;
                }
                else
                {
                    string stringResource = isTABConfigured ? "ScotApp_985" : "BagItem";
                    skipBaggingText.Property(TextBlock.TextProperty).SetResourceValue(stringResource);
                    skipBaggingButton.Visibility = Visibility.Collapsed;
                }
            }
        }

        // +SSCOI-44701: Enable/Disable the mPerks button in Scan&bag
        private void UpdatemPerksButton()
        {
            if (this.viewModel.StateParam.Equals("Scan"))
            {
				//Start CRD 127021 meijerGo only mode
				if (GetPropertyStringValue("TransactionMode").Equals("2") || GetPropertyStringValue("TransactionMode").Equals("3"))	 
				{
					MPerksScanAndBagButton.Visibility = Visibility.Collapsed;
					PrintGiftReceiptButton.Visibility = Visibility.Collapsed;
				}
				//End CRD 127021 meijerGo only mode
				else
                MPerksScanAndBagButton.IsEnabled = GetPropertyBoolValue("MPerksScanAndBagEnabled");
            }
        }// -SSCOI-44701

        private void UpdateWicEBTButton()
        {
            if (this.viewModel.StateParam.Equals("Scan"))
            {
                if (GetPropertyStringValue("WICFeatureEnable").Equals("0"))
                {
                    WICEBTScanAndBagButton.Visibility = Visibility.Collapsed;
                    //Grid.SetColumn(WICEBTScanAndBagButton, 3);
                    Grid.SetColumn(PrintGiftReceiptButton, 1);
                    Grid.SetColumn(OwnBagButton, 2);
                }
                else                
                {
					//Start CRD 127021 meijerGo only mode
					if (GetPropertyStringValue("TransactionMode").Equals("2") || GetPropertyStringValue("TransactionMode").Equals("3"))	 
					{
						WICEBTScanAndBagButton.Visibility = Visibility.Collapsed;
					}	//End CRD 127021
					else
					{
                    WICEBTScanAndBagButton.Visibility = Visibility.Visible;
                    WICEBTScanAndBagButton.IsEnabled = GetPropertyBoolValue("WicEbtScanAndBagEnabled");
					}
                }
            }
        }

        private void FuelImageButton_TouchDown(object sender, TouchEventArgs e)
        {
            viewModel.ActionCommand.Execute(string.Format("CmdBtn_PrePayFuel({0})", viewModel.GetPropertyValue("CmdBtn_PrePayFuelData").ToString()));
        }

        private void LotteryImageButton_TouchDown(object sender, TouchEventArgs e)
        {
            viewModel.ActionCommand.Execute(string.Format("CmdBtn_Lottery({0})", viewModel.GetPropertyValue("CmdBtn_LotteryData").ToString()));
        }

        private bool allowFilmstrip;
        private bool cancelCoupons;
        private CustomerReceiptItem currentReceiptItem;
        private ObservableCollection<ReceiptItem> customerReceiptCollection;
        private bool isQuickPick = true;

        //SSCOI-50040
        /// <summary>
        /// Loaded event handler
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event arguments</param>
        private void BackgroundView_Loaded(object sender, RoutedEventArgs e)
        {
            // If we have made it back to Sale (usually via assist mode), no NMI popups are pending.
            Search.IsNMIPopupPending = false;

            switch (viewModel.StateParam)
            {
                case "VoidItem":
                case "VoidItemWithReward":
                case "AM_VoidItem":
                case "CmDataEntry2":
                    break;
                default:
                    if (viewModel.IsActiveBackground)
                    {
                        FocusHelper.SetInitialFocus(this, viewModel);
                    }
                    break;
            }

            UpdateSearchButtonTextAndCommand();
        }



        /// <summary>
        /// Handles the transition of showing this view behind a pop-up view.
        /// Updates the Filmstrip Card panel when transferring items from mobile cart
        /// </summary>
        public override void OnShowBehindPopup()
        {
            switch (viewModel.StateParam)
            {
                case "Processing":
                    FilmstripPanel.IsSynchronizedWithCurrentItem = false;
                    break;
                default:
                    FilmstripPanel.IsSynchronizedWithCurrentItem = true;
                    break;
            }
        }

        /// <summary>
        /// Updates ScanItemSearchKeyInItemButton text based on NextGenUI transaction variables.
        /// </summary>
        private void UpdateSearchButtonTextAndCommand()
        {
            bool allowLookUpItem = GetStringPropertyBoolValue("NextGenUIAllowLookUpItem");
            bool allowKeyInCode = GetStringPropertyBoolValue("NextGenUIAllowKeyInCode");
            bool allowPickListSearchButton = GetStringPropertyBoolValue("NextGenUIAllowPickListSearchButton");

			//Start CRD 127021 meijerGo only mode
			if(GetPropertyStringValue("TransactionMode").Equals("2") || GetPropertyStringValue("TransactionMode").Equals("3"))
			{
				ScanItemTextBlock.Visibility = Visibility.Collapsed;
				scanItemSearchKeyInItem.Visibility = Visibility.Collapsed;
				ScanItemBorder.Visibility = Visibility.Collapsed;
			}
            //End CRD 127021
            else if (allowLookUpItem)
            {
                // will transition to Search screen
                scanItemSearchKeyInItem.Style = this.FindResource("searchScanButtonStyle") as Style;
                this.scanItemSearchKeyInItem.CommandParameter = String.Format("SearchKeyInItem");
                if (allowPickListSearchButton && allowKeyInCode)
                {
                    // Search screen has alpha and numeric keypad
                    NoItemsPrompt.Property(MeasureTextBlock.TextProperty).SetResourceValue("ScanAndBag");
                    scanItemSearchKeyInItem.Property(ImageButton.TextProperty).SetResourceValue("SearchKeyInItem");
                }
                else if (allowPickListSearchButton && !allowKeyInCode)
                {
                    // Search screen has alpha keys only
                    NoItemsPrompt.Property(MeasureTextBlock.TextProperty).SetResourceValue("ScanAndBagNoKeyIn");
                    scanItemSearchKeyInItem.Property(ImageButton.TextProperty).SetResourceValue("ScotApp_1844");
                }
                else if (!allowPickListSearchButton && allowKeyInCode)
                {
                    // Search screen has numeric keypad only
                    NoItemsPrompt.Property(MeasureTextBlock.TextProperty).SetResourceValue("ScanLookUpKeyInItem");
                    scanItemSearchKeyInItem.Property(ImageButton.TextProperty).SetResourceValue("LookUpKeyInItem");
                }
                else
                {
                    // Search screen has no alpha keys nor numeric keypad
                    NoItemsPrompt.Property(MeasureTextBlock.TextProperty).SetResourceValue("ScanAndBagNoSearchKeyIn");
                    scanItemSearchKeyInItem.Property(ImageButton.TextProperty).SetResourceValue("ScotApp_336");
                }

                if ((GetPropertyStringValue("TransactionMode").Equals("0")) && GetPropertyStringValue("TerminalType").Equals("SS90", StringComparison.OrdinalIgnoreCase))
                    scanItemSearchKeyInItem.Visibility = Visibility.Collapsed;
                else
                    scanItemSearchKeyInItem.Visibility = Visibility.Visible;
            }
            else
            {
                // will not transition to Search screen
                scanItemSearchKeyInItem.Style = this.FindResource("keyboardScanButtonStyle") as Style;
                this.scanItemSearchKeyInItem.CommandParameter = String.Format("KeyInItemCode");
                if (allowKeyInCode)
                {
                    // numeric keypad popup available
                    NoItemsPrompt.Property(MeasureTextBlock.TextProperty).SetResourceValue("ScanOrKeyInItem");
                    scanItemSearchKeyInItem.Property(ImageButton.TextProperty).SetResourceValue("KeyInCodeTitle");
                    if ((GetPropertyStringValue("TransactionMode").Equals("0")) && GetPropertyStringValue("TerminalType").Equals("SS90", StringComparison.OrdinalIgnoreCase))
                        scanItemSearchKeyInItem.Visibility = Visibility.Collapsed;
                    else
                        scanItemSearchKeyInItem.Visibility = Visibility.Visible;
                }
                else
                {
                    NoItemsPrompt.Property(MeasureTextBlock.TextProperty).SetResourceValue("ScanAndBagScanOnly");
                    scanItemSearchKeyInItem.Visibility = Visibility.Collapsed;
                }
            }
        }
        //-SSCOI-50040

        /////New UI
        private void UpdateButtonVisibility(bool IsVisible)
        {
            const string DisconnectedStatus = "Disconnected";
            const string ContextHelp = "ContextHelp";
            if (viewModel.StateParam.Equals(DisconnectedStatus))
            {
                this.OwnBagButton.Visibility = Visibility.Visible;
            }
            else
            {
                if (!viewModel.StateParam.Equals(ContextHelp))
                {
                    if (IsVisible)
                    {
                        this.UpdateBagButtonVisibility();
                    }
                    else
                    {
                        this.OwnBagButton.Visibility = Visibility.Collapsed;
                    }
                }
            }
        }

        private static List<string> ParamListForSystemFunction = new List<string>() { "InTransaction", "InTransactionVoid", "Store" };

        /// <summary>
        /// Gets a value indicating whether to show  OwnBag button at Attract
        /// </summary>
        private bool GetUIOwnBagButtonAtAttract
        {
            get
            {
                if (viewModel.GetPropertyValue("UIOwnBagButtonAtAttract").Equals("-1"))
                {
                    return true;
                }

                return false;
            }
        }

        /// <summary>
        /// Gets a value indicating whether the OwnBagButton is displayed at ScanAndBag 
        /// </summary>
        private bool GetUIOwnBagButtonAtScanAndBag
        {
            get
            {
                if (viewModel.GetPropertyValue("UIOwnBagButtonAtScanAndBag").Equals("-1") ||
                   (bool)viewModel.GetPropertyValue("OwnBagSaleShown") ||   // To display the Own bag button in Scan&Bag screen
                   (viewModel.ActiveStateParam.Equals("OwnBag")))  // SSCOI-46399: This is to display the Own bag button in OwnBag screen. Meijer is not using the scotopts config.
                {
                    return true;
                }

                return false;
            }
        }

        /// <summary>
        /// Refresh bag state
        /// </summary>
        private void RefreshOwnBagState()
        {
            this.UpdateButtonVisibility(true);
            if (viewModel.GetPropertyValue("NextGenData").ToString().Equals("BagAndEAS"))
            {
                OwnBagButton.IsEnabled = false;
            }
            else
            {
                if (viewModel.State.Equals("OutOfTransaction"))
                {
                    OwnBagButton.IsEnabled = (2 != SystemFunctionsControl._languages || (bool)viewModel.GetPropertyValue("LanguageWelcomeShown")) ?
                        (bool)viewModel.GetPropertyValue("OwnBagWelcomeShown") : false;
                }
                else
                {
                    OwnBagButton.IsEnabled = (bool)viewModel.GetPropertyValue("OwnBagSaleShown");
                }
            }
        }

        /// <summary>
        /// hide,collapse or visible Bag Button  
        /// </summary>
        private void UpdateBagButtonVisibility()
        {
            switch (viewModel.StateParam)
            {
                case "Welcome":
                case "Startup":
                case "OutOfService":
                case "AttractMultiLanguage":
                case "Closed":
                case "EnterId":
                case "OperatorPasswordStateInvalidPassword":
                    if (!ParamListForSystemFunction.Contains(viewModel.State))
                    {
                        this.OwnBagButton.Visibility = GetUIOwnBagButtonAtAttract ? Visibility.Visible : Visibility.Collapsed;
                    }
                    break;
                case "ProduceFavorites":
                case "Scan":
                case "Payment":
                case "OwnBag":
                case "MultiSelectProduceFavorites":
                case "SelectTare":
                case "ScanVoucher":
                case "EnterCoupons":
                case "QuickPickItemsWithReward":
                case "ScanAndBagWithReward":
                case "ScanWithReward":
                case "QuickPickWithReward":
                case "RequestSig":
                case "AcknowledgeSig":
                case "Finish":
                case "TakeReceipt":
                case "CashPayment":
                case "DepositCoupon":
                case "SelectPayment":
                case "SelectPaymentEBT":
                case "TakeChange":
                case "SystemMessageOpPass":
                case "ScanCard":
                case "SwipeCard":
                case "TakeCash":
                case "Bag":
                case "SellBags":
                case "CheckBasket":
                case "InsertCoupon":
                case "InsertGiftCard":
                case "DepositGiftCard":
                    if (!viewModel.BackgroundStateParam.Equals("Welcome"))
                    {
                        this.OwnBagButton.Visibility = GetUIOwnBagButtonAtScanAndBag ? Visibility.Visible : Visibility.Collapsed;
                    }
                    break;
            }
        }

        void viewModel_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (!viewModel.StateParam.Equals("ContextHelp"))
            {
                this.UpdateButtonVisibility(true);
            }
            switch (e.PropertyName)
            {
                case "Languages":
                    this.RefreshOwnBagState();
                    break;
                case "OwnBagSaleShown":
                case "OwnBagWelcomeShown":
                case "LanguageWelcomeShown":
                    this.RefreshOwnBagState();
                    break;
                case "State":
                    this.RefreshOwnBagState();
                    break;
                case "NextGenData":
                    this.RefreshOwnBagState();
                    break;
            }

            SetSNBApplyMeijerCardButton(); //CRD 525299

        }
        /////New UI End
		
		//Start CRD 127021 meijerGo only mode
        private void onMeijerGoSNB()
        {
            SNDYesButton.Visibility = Visibility.Collapsed;
            SNDNoButton.Visibility = Visibility.Collapsed;
            SNDNoItemsPrompt.Visibility = Visibility.Collapsed;
            SDNLeadthruText.Visibility = Visibility.Collapsed;
            this.NoItemsPrompt.Visibility = Visibility.Visible;
            FuelLotteryPanel.Visibility = Visibility.Collapsed;
        }
        //End CRD 127021 meijerGo only mode

        /// <summary>
        /// CRD 525299 533042
        /// Sets Apply Meijer Card button at Attract screen and adjust button grid as needed
        /// </summary>
        private void SetSNBApplyMeijerCardButton()
        {
            if (GetPropertyStringValue("AllowApplyMeijerCard").Equals("1") &&
                GetPropertyStringValue("AllowApplyMeijerCardScreens").Contains("ScanAndBag") &&
                GetPropertyStringValue("TransactionMode").Equals("0"))
            {
               if (viewModel.StateParam.Equals("Scan"))
               {
                    // If WIC Button is disabled or Own Bag Button is disabled, set Grid Column to "3"
                    if (GetPropertyStringValue("WICFeatureEnable").Equals("0") || OwnBagButton.IsEnabled == false)
                    {
                        if (OwnBagButton.IsEnabled == false)
                            OwnBagButton.Visibility = Visibility.Collapsed;

                        this.ApplyMeijerCardButton.Style = this.FindResource("ApplyMeijerCardButtonStyle334x114") as Style;
                        Grid.SetColumn(this.ApplyMeijerCardButton, 3);
                    }
                    // Own Bag button is disabled while in the transaction and this leads to ApplyMeijerCardButton to cover OwnBag Button.
                    // To fix this, we need to set again to Column 4 the ApplyMeijerCardButton so it won't cover OwnBagButton
                    else if (OwnBagButton.IsEnabled == true)
                    {
                        this.ApplyMeijerCardButton.Style = this.FindResource("ApplyMeijerCardButtonStyle") as Style;
                        Grid.SetColumn(this.ApplyMeijerCardButton, 4);
                    }

                    // If SS90, WIC and Own Bag is collapsed regardless if it is configured or not. set Grid Column to "2"
                    if (GetPropertyStringValue("TerminalType").Equals("SS90") ||
                          (GetPropertyStringValue("WICFeatureEnable").Equals("0") && GetUIOwnBagButtonAtAttract == false))
                    {
                        if (GetPropertyStringValue("TerminalType").Equals("SS90"))
                        {
                            this.ApplyMeijerCardButton.ImageVerticalAlignment = VerticalAlignment.Center;
                            this.ApplyMeijerCardButton.Height = 107;
                            MPerksScanAndBagButton.Height = 107;
                            PrintGiftReceiptButton.Height = 107;
                        }

                        this.ButtonListGrid.ColumnDefinitions[2].Width = GridLength.Auto;
                        this.ApplyMeijerCardButton.Style = this.FindResource("ApplyMeijerCardButtonStyle426x114") as Style;
                        Grid.SetColumn(ApplyMeijerCardButton, 2);
                        this.ApplyMeijerCardButton.Width = 438;
                        this.ApplyMeijerCardButton.Margin = new Thickness(6, 0, -300, -0.4);
                    }
                    this.ApplyMeijerCardButton.Visibility = Visibility.Visible;
                }
            }
            else
            {
                this.ApplyMeijerCardButton.Visibility = Visibility.Collapsed;
            }
        }
    }
}
