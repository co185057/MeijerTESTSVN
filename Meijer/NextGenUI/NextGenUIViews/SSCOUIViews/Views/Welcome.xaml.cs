// <copyright file="Welcome.xaml.cs" company="NCR Corporation">
//     Copyright (c) NCR Corporation. All rights reserved.
// </copyright>
namespace SSCOUIViews.Views
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.ComponentModel;
    using System.Globalization;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Data;
    using System.Windows.Documents;
    using System.Windows.Input;
    using System.Windows.Media;
    using System.Windows.Media.Imaging;
    using System.Windows.Navigation;
    using System.Windows.Shapes;
    using FPsxWPF.Controls;
    using RPSWNET;
    using SSCOControls.Helpers.KeyboardSimulatorComponents;
    using SSCOControls.Interfaces;
    using SSCOUIModels;
    using SSCOUIModels.Controls;
    using SSCOUIModels.Helpers;
    using SSCOControls;
    using SSCOUIViews.Controls;

    /// <summary>
    /// Interaction logic for Welcome.xaml
    /// </summary>
    public partial class Welcome : BackgroundView, INavigable
    {
        /// <summary>
        /// languageCollection variable
        /// </summary>
        private ObservableCollection<GridItem> _languageCollection;

        /// <summary>
        /// Initializes a new instance of the Welcome class        
        /// </summary>
        /// <param name="viewModel">viewModel type of parameter</param>
        public Welcome(IMainViewModel viewModel)
            : base(viewModel)
        {
            this.InitializeComponent();
            LoadWelcomeImage();
            viewModel.PropertyChanged += viewModel_PropertyChanged;
            PreviewKeyDown += Control_PreviewKeyDown;
            LostFocus += Control_LostFocus;
            GotFocus += Control_GotFocus;
        }

        /// <summary>
        /// Gets or sets a value indicating whether the up arrow is converted to Shift-Tab.
        /// </summary>
        public bool ShiftTabForUp
        {
            get { return false; }
            set { }
        }

        /// <summary>
        /// Gets or sets a value indicating whether the down arrow is converted to Tab.
        /// </summary>
        public bool TabForDown
        {
            get { return false; }
            set { }
        }

        /// <summary>
        /// Gets or sets a value indicating whether the left arrow is converted to Shift-Tab.
        /// </summary>
        public bool ShiftTabForLeft
        {
            get { return true; }
            set { }
        }

        /// <summary>
        /// Gets or sets a value indicating whether the right arrow is converted to Tab.
        /// </summary>
        public bool TabForRight
        {
            get { return true; }
            set { }
        }

        /// <summary>
        /// Gets or sets a value indicating whether the control has focus.
        /// Use this instead of IsFocused, which can be false if you have sub controls.
        /// </summary>
        public bool IsControlFocused { get; set; }

        /// <summary>
        /// OnStateParamChanged that accepts parameter that is set in app.config.
        /// </summary>
        /// <param name="parameter">String type of parameter.</param>
        public override void OnStateParamChanged(string parameter)
        {
            switch (parameter)
            {
                case "Welcome":
                    UpdateSearchButtonProperties();
                    AttractLanguageStateChange();
                    RefreshAttractScreenState(); // CRD 125378
					/*
					RFQ 7477
					Added by Toua Moua 06.02.16
					Added CommandParameter for WICEBT/PriceInquiry/MPerks buttons.
					*/
					
					MPerksButton.Text = String.Empty; //SSCOI-45480
					InstructionBox.CommandParameter = "Scan";
                    SNDInstructionBox.CommandParameter = "Scan";
					SearchKeyInItemButton.CommandParameter = "SearchKeyInItem";
					WICEBTButton.CommandParameter = "WicEbt";
					PriceInquiryButton.CommandParameter = "PriceInquiry";
					MPerksButton.CommandParameter = "MPerks";
				
					// +SSCOI-46989
					// The visibility of the WIC Card button will now be based on a TransactionVariable set from SSF
					// instead of the visibility of the button.
                    if (GetPropertyStringValue("WICFeatureEnable").Equals("0"))
                    {
                        WICEBTButton.Visibility = Visibility.Collapsed;
                        //Grid.SetColumn(WICEBTButton, 3);
                        Grid.SetColumn(PriceInquiryButton, 1);
                        Grid.SetColumn(OwnBagButton, 2);
                    }
                    else
                    {
                        WICEBTButton.Visibility = Visibility.Visible;
                    }
					// Make the WIC EBT Card button in Attract screen visibility based on Classic
					// WICEBTButton.Visibility = GetPropertyBoolValue("WicEbtShown") ? Visibility.Visible : Visibility.Collapsed;
					// -SSCOI-46989

                    SetAttractApplyMeijerCardButton(); // CRD 525299

                    break;
                case "AttractMultiLanguage":
                    UpdateSearchButtonProperties();
                    this.AttractMultiLanguageStateChange();
                    break;
            }
        }

        /// <summary>
        /// Event handler for PropertyChanged
        /// </summary>
        /// <param name="name">Property name</param>
        /// <param name="value">New value</param>
        public override void OnPropertyChanged(string name, object value)
        {
            switch (name)
            {
                case "Language":
                    AttractLanguageStateChange();
                    break;
                case "CustomerLanguage":
                    AttractLanguageStateChange();
                    AttractMultiLanguageStateChange();
                    break;
                case "NextGenUIAllowKeyInCode":
                case "NextGenUIAllowLookUpItem":
                case "NextGenUIAllowPickListSearchButton":
                    UpdateSearchButtonProperties();
                    break;					
                case "WicEbtShown":
				case "WICFeatureEnable": // +SSCOI-46989
					// The visibility of the WIC Card button will now be based on a TransactionVariable set from SSF
					// instead of the visibility of the button.
                    if (GetPropertyStringValue("WICFeatureEnable").Equals("0"))
                    {
                        WICEBTButton.Visibility = Visibility.Collapsed;
                        //Grid.SetColumn(WICEBTButton, 3);
                        Grid.SetColumn(PriceInquiryButton, 1);
                        Grid.SetColumn(OwnBagButton, 2);
                    }
                    else
                    {
                        WICEBTButton.Visibility = Visibility.Visible;
                    }
                    //WICEBTButton.Visibility = GetPropertyBoolValue("WicEbtShown") ? Visibility.Visible : Visibility.Collapsed;
					// -SSCOI-46989
					 // start CRD 125377
                    //if (!GetPropertyStringValue("ShopAndDropEnabled").Equals("0"))
                    if (GetPropertyStringValue("TransactionMode").Equals("1") )
                    {
                        //this.PopupTitle.Text = GetPropertyStringValue("LeadthruText");
                        //this.PopupTitle.Visibility = Visibility.Visible;

                        SearchKeyInItemButton.Visibility = Visibility.Collapsed;
                        WICEBTButton.Visibility = Visibility.Collapsed;
                        PriceInquiryButton.Visibility = Visibility.Collapsed;
                        MPerksButton.Visibility = Visibility.Collapsed;
                        BorderSND.Visibility = Visibility.Collapsed;
                    }
                    // end CRD 125377
                    break;
				 case "TransactionMode": // CRD 125378
                    RefreshAttractScreenState();
                    break;
            }
        }

        /// <summary>
        /// Preview key down handler. Typically used for arrow and space keys.
        /// Be sure to check IsControlFocused before running code.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        public void Control_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            switch (e.Key)
            {
                case Key.Left:
                    if (ShiftTabForLeft)
                    {
                        KeyboardSimulator.Instance.SimulateModifiedKeyStroke(VirtualKeyCode.LSHIFT, VirtualKeyCode.TAB);
                        e.Handled = true;
                    }

                    break;
                case Key.Right:
                    if (TabForRight)
                    {
                        KeyboardSimulator.Instance.SimulateKeyPress(VirtualKeyCode.TAB);
                        e.Handled = true;
                    }

                    break;
            }
        }

        /// <summary>
        /// Lost Focus event handler. Be sure to set IsControlFocused in this method.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        public void Control_LostFocus(object sender, RoutedEventArgs e)
        {
            IsControlFocused = false;
        }

        /// <summary>
        /// Got Focus event handler. Be sure to set IsControlFocused in this method.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        public void Control_GotFocus(object sender, RoutedEventArgs e)
        {
            IsControlFocused = true;
        }

        /// <summary>
        /// Attract MultiLanguage State Change Method.
        /// </summary>
        private void AttractMultiLanguageStateChange()
        {
            if (viewModel.StateParam.Equals("AttractMultiLanguage"))
            {
                _languageCollection = viewModel.GetPropertyValue("Languages") as ObservableCollection<GridItem>;
                int index = 0;
                if (_languageCollection.Count > 0)
                {
                    foreach (GridItem item in _languageCollection)
                    {
                        int LanguageData = int.Parse(item.Data.ToString().Split(';')[1], System.Globalization.NumberStyles.HexNumber, CultureInfo.InvariantCulture);

                        if (LanguageData == viewModel.CustomerLanguage)
                        {
                            index = _languageCollection.IndexOf(item);
                            break;
                        }
                    }
                }

                InstructionBox.CommandParameter = string.Format("CMButton{0}MidiList", index + 1);
                SNDInstructionBox.CommandParameter = string.Format("CMButton{0}MidiList", index + 1);   //CRD 125377

                if (GetStringPropertyBoolValue("NextGenUIAllowLookUpItem"))
                {
                    SearchKeyInItemButton.CommandParameter = string.Format("SearchKeyInItem({0})", index + 1);
                }
                else
                {
                    SearchKeyInItemButton.CommandParameter = string.Format("KeyInItemCode({0})", index + 1);
                }
            }
        }

        /// <summary>
        /// Attract Single and Dual Language State Change Method.
        /// </summary>
        private void AttractLanguageStateChange()
        {
            if (viewModel.StateParam.Equals("Welcome"))
            {
                InstructionBox.CommandParameter = "Scan";
                SNDInstructionBox.CommandParameter = "Scan";
                if (GetStringPropertyBoolValue("NextGenUIAllowLookUpItem"))
                {
                    SearchKeyInItemButton.CommandParameter = "SearchKeyInItem";
                }
                else
                {
                    SearchKeyInItemButton.CommandParameter = "KeyInItemCode";
                }

                if (!GetPropertyBoolValue("LanguageWelcomeShown"))
                {
                    _languageCollection = viewModel.GetPropertyValue("Languages") as ObservableCollection<GridItem>;
                    int language = viewModel.Language;

                    if (_languageCollection != null
                        && _languageCollection.Count == 2 && language != 0)
                    {
                        language = viewModel.CustomerLanguage;
                        string langSelected = language.ToString("X", CultureInfo.CurrentCulture);
                        string primeLang = (_languageCollection[0].Data != null) ? _languageCollection[0].Data.Split(';')[1] : language.ToString("X", CultureInfo.CurrentCulture);

                        if (!primeLang.ToLower().Contains(langSelected.ToLower()))
                        {
                            InstructionBox.CommandParameter = "ScanForLanguage2";
                            SNDInstructionBox.CommandParameter = "ScanForLanguage2";
                            if (GetStringPropertyBoolValue("NextGenUIAllowLookUpItem"))
                            {
                                SearchKeyInItemButton.CommandParameter = "SearchKeyInItemForLanguage2";
                            }
                            else
                            {
                                SearchKeyInItemButton.CommandParameter = "KeyInItemCodeForLanguage2";
                            }
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Loads the Welcome image if any.
        /// </summary>
        private void LoadWelcomeImage()
        {
            try
            {
                string imagePath = ItemImageConverter.GetScotDirectory(ItemImageConverter.GetAppDrive(), ItemImageConverter.GetAppDir()) + "\\image\\Welcome.png";
                if (!System.IO.File.Exists(imagePath))
                {
                    CmDataCapture.CaptureFormat(
                        CmDataCapture.MaskInfo,
                        "SSCOUIViews.Views.Welcome.LoadWelcomeImage() - No welcome image found: {0}",
                        imagePath);
                    return;
                }
                BitmapImage bi = new BitmapImage();
                bi.BeginInit();
                bi.CacheOption = BitmapCacheOption.OnLoad;
                bi.CreateOptions = BitmapCreateOptions.IgnoreImageCache;
                bi.UriSource = new Uri(string.Format(CultureInfo.CurrentCulture, imagePath), UriKind.Relative);
                bi.EndInit();
                this.WelcomeImage.Source = bi;
            }
            catch (Exception ex)
            {
                CmDataCapture.CaptureFormat(
                    CmDataCapture.MaskError,
                    "SSCOUIViews.Views.Welcome.LoadWelcomeImage() - Caught Exception {0} ",
                    ex.Message);
            }
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

                IInputElement focusedControl = FocusManager.GetFocusedElement(this);
                if (focusedControl != null && !viewModel.IsUNavEnabled)
                {
                    FocusManager.SetFocusedElement(this, null);
                } 
            }

            UpdateSearchButtonProperties();
        }

        /// <summary>
        /// Updates ScanItemSearchKeyInItemButton text based on NextGenUI transaction variables.
        /// </summary>
        private void UpdateSearchButtonProperties()
        {
            bool allowLookUpItem = GetStringPropertyBoolValue("NextGenUIAllowLookUpItem");
            bool allowKeyInCode = GetStringPropertyBoolValue("NextGenUIAllowKeyInCode");
            bool allowPickListSearchButton = GetStringPropertyBoolValue("NextGenUIAllowPickListSearchButton");

            if (allowLookUpItem)
            {
                SearchKeyInItemButton.Style = this.FindResource("welcomeButtonStyle") as Style;
                if (allowPickListSearchButton && allowKeyInCode)
                {
                    // Search screen has alpha and numeric keypad
                    SearchKeyInItemButton.Property(ImageButton.TextProperty).SetResourceValue("SearchKeyInItem");
                }
                else if (allowPickListSearchButton && !allowKeyInCode)
                {
                    // Search screen has alpha keys only
                    SearchKeyInItemButton.Property(ImageButton.TextProperty).SetResourceValue("ScotApp_1844");
                }
                else if (!allowPickListSearchButton && allowKeyInCode)
                {
                    // Search screen has numeric keypad only
                    SearchKeyInItemButton.Property(ImageButton.TextProperty).SetResourceValue("LookUpKeyInItem");
                }
                else
                {
                    // Search screen has no alpha keys nor numeric keypad
                    SearchKeyInItemButton.Property(ImageButton.TextProperty).SetResourceValue("ScotApp_336");
                }

                ScanItemStatus.Visibility = Visibility.Visible;
            }
            else
            {
                if (allowKeyInCode)
                {
                    // numeric keypad popup available
                    // will not transition to Search screen
                    SearchKeyInItemButton.Style = this.FindResource("welcomeKeyboardButtonStyle") as Style;
                    SearchKeyInItemButton.Property(ImageButton.TextProperty).SetResourceValue("KeyInCodeTitle");
                    ScanItemStatus.Visibility = Visibility.Visible;
                }
                else
                {
                    ScanItemStatus.Visibility = Visibility.Collapsed;
                }
            }
        }

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
			MeijerScanQR.Visibility = Visibility.Collapsed; //CRD 171129
            MobileShopper.Visibility = Visibility.Collapsed; //CRD 171129

		   
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
         
            // + CRD 171129
           if ((GetPropertyStringValue("TransactionMode").Equals("0")) && (viewModel.StateParam.Equals("Welcome")) && (viewModel.BackgroundStateParam.Equals("Welcome")) && (GetPropertyStringValue("EnableMobileShopper").Equals("1")))
            {
                CmDataCapture.CaptureFormat(
                      CmDataCapture.MaskInfo,
                      "SSCOUIViews.Views.Welcome.LoadWelcomeImage() - Entered transaction mode 0 22 "
                      );
                MobileShopper.Visibility = Visibility.Visible;
            }

          
            if ((GetPropertyStringValue("TransactionMode").Equals("2")) && (viewModel.StateParam.Equals("Welcome")) && (viewModel.BackgroundStateParam.Equals("Welcome")) && (GetPropertyStringValue("EnableMobileShopper").Equals("1")))
            {
                CmDataCapture.CaptureFormat(
                      CmDataCapture.MaskInfo,
                      "SSCOUIViews.Views.Welcome.LoadWelcomeImage() - Entered transaction mode 2 44 "
                      );
                MeijerScanQR.Visibility = Visibility.Visible;
            }
           // - CRD 171129

            SetAttractApplyMeijerCardButton(); //CRD 525299
        }
        /////New UI End
        //Start CRD 125378 MeijerGo and Shop n Drop
        // CRD 126015
        /// <summary>
        /// Refresh Attract Screen state
        /// </summary>
        private void RefreshAttractScreenState()
        {
            switch (GetPropertyStringValue("TransactionMode"))
            {
                case "0":	//SCO only
                    WelcomeMeijerGoSnD.Visibility = Visibility.Collapsed;
                    WelcomeSCO.Visibility = Visibility.Visible;
                    MeijerGoOnly.Visibility = Visibility.Collapsed;
                    ShopAndDropOnly.Visibility = Visibility.Collapsed;
                    MobileShopper.Visibility = Visibility.Visible;
                    break;
                case "1":	//SnD only
						//this.Instructions.Text = GetPropertyStringValue("Instructions");
                       //this.Instructions.Visibility = Visibility.Visible;
                       //this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9373");
                    
                       // this.PopupTitle.Text = GetPropertyStringValue("LeadthruText");
                       //this.PopupTitle.Visibility = Visibility.Visible;

                       //this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_92"); 

                    if (GetPropertyStringValue("WasOnTransactionMode3").Equals("1"))   //Meijer Go screen from Selection screen, should show go back button
                        SNDGoBack_WithImage.Visibility = Visibility.Visible;
                    else
                        SNDGoBack_WithImage.Visibility = Visibility.Collapsed;

                        ShopAndDropOnly.Visibility = Visibility.Visible;

                        SearchKeyInItemButton.Visibility = Visibility.Collapsed;
                        WICEBTButton.Visibility = Visibility.Collapsed;
                        PriceInquiryButton.Visibility = Visibility.Collapsed;
                        MPerksButton.Visibility = Visibility.Collapsed;
                        BorderSND.Visibility = Visibility.Collapsed;
						
						MeijerGoOnly.Visibility = Visibility.Collapsed;
						WelcomeMeijerGoSnD.Visibility = Visibility.Collapsed;
						WelcomeSCO.Visibility = Visibility.Collapsed;
                        MobileShopper.Visibility = Visibility.Collapsed;
                    break;
                case "2": 	//meijer Go only
                    if (GetPropertyStringValue("WasOnTransactionMode3").Equals("1"))   //Meijer Go screen from Selection screen, should show go back button
                        MeijerGoOnlyGoBack_WithImage.Visibility = Visibility.Visible;
                    else
                        MeijerGoOnlyGoBack_WithImage.Visibility = Visibility.Collapsed;
                    MeijerGoOnly.Visibility = Visibility.Visible;
                    WelcomeMeijerGoSnD.Visibility = Visibility.Collapsed;
                    WelcomeSCO.Visibility = Visibility.Collapsed;
                    MobileShopper.Visibility = Visibility.Collapsed;
                    ShopAndDropOnly.Visibility = Visibility.Collapsed;
                    break;
                case "3":	//meijer go and SnD
                    WelcomeMeijerGoSnD.Visibility = Visibility.Visible;
                    WelcomeSCO.Visibility = Visibility.Collapsed;
                    MeijerGoOnly.Visibility = Visibility.Collapsed;
                    MobileShopper.Visibility = Visibility.Collapsed;
                    ShopAndDropOnly.Visibility = Visibility.Collapsed;
                    break;
            }
        }
        // End CRD 125378 MeijerGo and Shop n Drop

        /// <summary>
        /// CRD 525299 533042
        /// Sets Apply Meijer Card button at Attract screen and adjust button grid as needed
        /// </summary>
        private void SetAttractApplyMeijerCardButton()
        {
            if (GetPropertyStringValue("AllowApplyMeijerCard").Equals("1") && 
                GetPropertyStringValue("AllowApplyMeijerCardScreens").Contains("Attract") &&
                GetPropertyStringValue("TransactionMode").Equals("0"))
            {
               if (viewModel.BackgroundStateParam.Equals("Welcome"))
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
                            MPerksButton.Height = 107;
                            PriceInquiryButton.Height = 107;
                        }

                        this.ButtonListGrid.ColumnDefinitions[2].Width = GridLength.Auto;
                        this.ApplyMeijerCardButton.Style = this.FindResource("ApplyMeijerCardButtonStyle426x114") as Style;
                        Grid.SetColumn(this.ApplyMeijerCardButton, 2);
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
