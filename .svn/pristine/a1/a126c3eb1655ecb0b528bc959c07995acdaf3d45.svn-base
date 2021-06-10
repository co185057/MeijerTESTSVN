// <copyright file="SystemFunctionsControl.xaml.cs" company="NCR Corporation">
//     Copyright (c) NCR Corporation. All rights reserved.
// </copyright>
namespace SSCOUIViews.Controls
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.Collections.Specialized;
    using System.ComponentModel;
    using System.Globalization;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Input;
    using FPsxWPF.Controls;
    using SSCOControls;
    using SSCOControls.Helpers.KeyboardSimulatorComponents;
    using SSCOControls.Interfaces;
    using SSCOUIModels;
    using SSCOUIModels.Models;
    using SSCOUIModels.Helpers;
    using System.Text;

    /// <summary>
    /// Interaction logic for SystemFunctionsControl.xaml
    /// </summary>
    public partial class SystemFunctionsControl : Grid, INavigable
    {
        /// <summary>
        /// Using a DependencyProperty as the backing store for NMI. This enables animation, styling, binding, etc.
        /// </summary>
        public static readonly DependencyProperty NMIProperty =
            DependencyProperty.Register("NMI", typeof(bool), typeof(SystemFunctionsControl), new PropertyMetadata(false));

        /// <summary>
        /// List variable
        /// </summary>
        private static readonly List<string> ParamListForButtonStoreLogin = new List<string>() { "InsertCoupon", "Bag", "InsertGiftCard", "TakeChange" };

        /// <summary>
        /// List of contexts where the assistance enabled follows the AssistanceEnabled property.
        /// </summary>
        private static readonly List<string> AssistanceFollowsEnabledContexts = new List<string> { "CashPayment", "Finish", "SelectTare", "UnknownItem" };

        /// <summary>
        /// List variable
        /// </summary>
        private static List<string> paramListAssistanceButton = new List<string>() { "Scan", "QuickPick", "ScanWithReward", "QuickPickWithReward", "SecMisMatchWeight", "SecSkipBaggingThreshold", "SecBagBoxNotValid" };

        /// <summary>
        /// List variable
        /// </summary>
        private static List<string> ParamListForCashManagementErrorMessagesV6HW = new List<string>() { "CMLoanPickupFailureV6HW", "CMTimedOutWaitingForTBV6HW", "XMSystemBusyV6HW", "XMCashierIDV6HW", "XMCashierPasswordV6HW" };

        /// <summary>
        /// List variable
        /// </summary>
        private static List<string> ParamListForSystemFunction = new List<string>() { "InTransaction", "InTransactionVoid", "Store" };

        /// <summary>
        /// viewModel variable
        /// </summary>
        private IMainViewModel _viewModel;

        /// <summary>
        /// languageCollection variable
        /// </summary>
        private ObservableCollection<GridItem> _languageCollection;

        /// <summary>
        /// languages variable
        /// </summary>
        public static int _languages;

        private CultureInfo cultureInfo;

        /// <summary>
        /// Initializes a new instance of the SystemFunctionsControl class.
        /// </summary>
        public SystemFunctionsControl()
        {
            InitializeComponent();

            this.NMI = Properties.Settings.Default.NMI;

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
        /// Gets or sets a value indicating whether the NMI standards are on for this control
        /// </summary>
        public bool NMI
        {
            get { return (bool)GetValue(NMIProperty); }
            set { SetValue(NMIProperty, value); }
        }

        /// <summary>
        /// Gets a value indicating whether to show VolumeControl
        /// </summary>
        private bool GetUIShowVolumeControl
        {
            get
            {
                if (_viewModel != null && _viewModel.GetPropertyValue("UIShowVolumeControl") != null && _viewModel.GetPropertyValue("UIShowVolumeControl").Equals("-1"))
                {
                    return true;
                }

                return false;
            }
        }        

        
        /// <summary>
        /// Gets a value indicating whether dual language is enabled
        /// </summary>
        private bool GetUIDualLanguage
        {
            get
            {
                if (_viewModel.GetPropertyValue("UIDualLanguage").Equals("-1"))
                {
                    return true;
                }

                return false;
            }
        }

        /// <summary>
        /// Gets a value indicating whether to allow Language selection during transaction 
        /// </summary>
        private bool GetUIAllowLanguageSelectionDuringTransaction
        {
            get
            {
                if (!_viewModel.GetPropertyValue("UIAllowLanguageSelectionDuringTransaction").Equals("0"))
                {
                    return true;
                }

                return false;
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
        /// Event handler for DataContextChanged
        /// </summary>
        /// <param name="sender">Sender object</param> 
        /// <param name="e">Event parameters</param> 
        private void Grid_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (_viewModel != null)
            {
                _viewModel.PropertyChanged -= new PropertyChangedEventHandler(ViewModel_PropertyChanged);
                (_viewModel.GetPropertyValue("Languages") as ObservableCollection<GridItem>).CollectionChanged -= languageCollection_CollectionChanged;
            }

            _viewModel = DataContext as IMainViewModel;
            _viewModel.PropertyChanged += new PropertyChangedEventHandler(ViewModel_PropertyChanged);

            

            this.RefreshLanguageEventHandlers();
            this.RefreshLanguages();
            this.RefreshLanguageState();
            this.RefreshLanguageText();
            this.RefreshVolumeState();
            this.OnRefreshACSVersion();
            this.OnRefreshTerminalNumber();
            this.OnRefreshVersion();
            this.RefreshAssistanceState();
        }

        /// <summary>
        /// Event handler for CollectionChanged
        /// </summary>
        /// <param name="sender">Sender object</param> 
        /// <param name="e">Event parameters</param> 
        private void languageCollection_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Reset)
            {
                RefreshLanguages();
                RefreshLanguageState();
                RefreshLanguageText();
                WeightDetailsPanel.UpdateWeightTextFlowDirection();
            }
        }

        /// <summary>
        /// Refresh language collection.
        /// </summary>
        private void RefreshLanguages()
        {
            if (null != _languageCollection)
            {
                _languages = _languageCollection.Count;
            }
            else
            {
                _languages = 0;
            }
        }

        /// <summary>
        /// Refresh language state.
        /// </summary>
        private void RefreshLanguageState()
        {
            this.UpdateButtonVisibility(true);
            if (_viewModel.GetPropertyValue("NextGenData").ToString().Equals("BagAndEAS"))
            {
                LanguageButton.IsEnabled = false;
            }
            else
            {
                if (_viewModel.StateParam.Equals("Welcome")
                    || _viewModel.StateParam.Equals("AttractMultiLanguage"))
                {
                    LanguageButton.IsEnabled = _languages > 1;
                }
                else
                {
                    LanguageButton.IsEnabled = (bool)_viewModel.GetPropertyValue("CMButton5MidiListShown");
                }

                if (_languages < 2 && _languages != 2)
                {
                    this.LanguageButton.IsEnabled = false;
                }
            }
        }

        /// <summary>
        /// Refresh language text.
        /// </summary>
        private void RefreshLanguageText()
        {
            if (2 < _languages)
            {
                this.LanguageButton.Property(ImageButton.TextProperty).SetResourceValue("Language");
            }
            else if (2 == _languages)
            {
                if (_viewModel.Language != 0)
                {
                    this.LanguageButton.Property(ImageButton.TextProperty).Clear();
                    string langSelected = _viewModel.CustomerLanguage.ToString("X", CultureInfo.CurrentCulture);
                    string primeLang = langSelected;
                    if (null != _languageCollection[0].Data)
                    {
                        string[] splitData = _languageCollection[0].Data.Split(';');
                        if (splitData.Length > 1)
                        {
                            primeLang = splitData[1];
                        }
                    }
                    if (primeLang.ToLower().Contains(langSelected.ToLower()) && null != _languageCollection[1].Text)
                    {
                        this.LanguageButton.Text = _languageCollection[1].Text;
                    }
                    else if (null != _languageCollection[0].Text)
                    {
                        this.LanguageButton.Text = _languageCollection[0].Text;
                    }
                    else
                    {
                        this.LanguageButton.Property(ImageButton.TextProperty).SetResourceValue("Language");
                    }
                }
                else
                {
                    if (null != _viewModel.GetPropertyValue("Language"))
                    {
                        this.LanguageButton.Text = _viewModel.GetPropertyValue("Language").ToString();
                    }
                    else
                    {
                        this.LanguageButton.Property(ImageButton.TextProperty).SetResourceValue("Language");
                    }
                }
            }
            else
            {
                this.LanguageButton.Property(ImageButton.TextProperty).SetResourceValue("Language");
            }
        }

        /// <summary>
        /// Update Buttons Visibility
        /// </summary>
        /// <param name="IsVisible">Whether buttons are Visible</param>
        private void UpdateButtonVisibility(bool IsVisible)
        {
            const string DisconnectedStatus = "Disconnected";
            const string ContextHelp = "ContextHelp";
            if (_viewModel.StateParam.Equals(DisconnectedStatus))
            {
                this.LanguageButton.Visibility = Visibility.Visible;
                this.VolumeButton.Visibility = Visibility.Visible;
            }
            else
            {
                if (!_viewModel.StateParam.Equals(ContextHelp))
                {
                    if (IsVisible)
                    {
                        this.UpdateVolumeButtonVisibility();
                        this.UpdateLanguageSelectionDuringTransactionVisibility();
                    }
                    else
                    {
                        this.LanguageButton.Visibility = Visibility.Collapsed;
                        this.VolumeButton.Visibility = Visibility.Collapsed;
                    }

                    // +CRD 125377
                    CancelOrderButton.Visibility = Visibility.Collapsed;
                    if (_viewModel.GetPropertyValue("TransactionMode").ToString().Equals("1"))
                    {
                        switch (_viewModel.StateParam)
                        {
                            case "Scan":
                            case "Finish":
                            case "Payment":
                                if (!_viewModel.State.Equals("Store"))
                                {
                                    // CRD 125377 

                                    // if (!GetPropertyStringValue("ShopAndDropEnabledYesNoButtons").Equals("0"))
                                    // if (Convert.ToBoolean(this.viewModel.GetPropertyValue("ShopAndDropEnabledYesNoButtons")))
                                    if (!_viewModel.GetPropertyValue("ShopAndDropEnabledYesNoButtons").ToString().Equals("0"))
                                    {
                                        CancelOrderButton.Visibility = Visibility.Visible;
                                        //this.LanguageButton.Visibility = Visibility.Visible;
                                        this.VolumeButton.Visibility = Visibility.Visible;
                                    }
                                }
                                break;
                        }
                    }
                    // -CRD 125377


                }
            }
        }        

        /// <summary>
        /// hide,collapse or visible Volume Button 
        /// </summary>
        private void UpdateVolumeButtonVisibility()
        {
            switch (_viewModel.StateParam)
            {
                case "Welcome":
                case "Startup":
                case "OutOfService":
                case "AttractMultiLanguage":
                case "ProduceFavorites":
                case "Scan":
                case "Payment":
                case "QuickPick":
                case "OwnBag":
                case "MultiSelectProduceFavorites":
                case "SelectTare":
                case "ScanVoucher":
                case "EnterCoupons":
                case "QuickPickItemsWithReward":
                case "ScanAndBagWithReward":
                case "QuickPickWithReward":
                case "ScanWithReward":
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
                case "Closed":
                case "SellBags":
                case "CheckBasket":
                case "InsertCoupon":
                case "InsertGiftCard":
                case "DepositGiftCard":
                case "EnterId":
                case "OperatorPasswordStateInvalidPassword":
                    if (!_viewModel.State.Equals("Store"))
                    {
                        VolumeButton.Visibility = GetUIShowVolumeControl ? Visibility.Visible : Visibility.Collapsed;
                    }
                    break;
            }
        }

        /// <summary>
        /// hide,collapse or visible Language Button 
        /// </summary>
        private void UpdateLanguageSelectionDuringTransactionVisibility()
        {
            //+127021 meijerGo only mode
            //if (GetPropertyStringValue("TransactionMode").Equals("2") || GetPropertyStringValue("TransactionMode").Equals("3"))	 
            if (!(_viewModel.GetPropertyValue("TransactionMode").ToString().Equals("0")))
            {
                this.LanguageButton.Visibility = Visibility.Collapsed;
            }
            //-127021
            else
            {
                switch (_viewModel.StateParam)
                {
                    case "Welcome":
                    case "Startup":
                    case "OutOfService":
                    case "AttractMultiLanguage":
                    case "Closed":
                    case "EnterId":
                    case "OperatorPasswordStateInvalidPassword":
                        if (!ParamListForSystemFunction.Contains(_viewModel.State))
                        {
                            this.LanguageButton.Visibility = GetUIDualLanguage ? Visibility.Visible : Visibility.Collapsed;
                        }
                        break;
                    case "ProduceFavorites":
                    case "Scan":
                    case "Payment":
                    case "QuickPick":
                    case "OwnBag":
                    case "MultiSelectProduceFavorites":
                    case "SelectTare":
                    case "ScanVoucher":
                    case "EnterCoupons":
                    case "QuickPickItemsWithReward":
                    case "ScanAndBagWithReward":
                    case "QuickPickWithReward":
                    case "ScanWithReward":
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
                        if (!_viewModel.BackgroundStateParam.Equals("Welcome") || _viewModel.ActiveStateParam.Equals("QuickPick"))
                        {
                            this.LanguageButton.Visibility = GetUIAllowLanguageSelectionDuringTransaction ? Visibility.Visible : Visibility.Collapsed;
                        }
                        break;
                }
            }
        }

        

        /// <summary>
        /// Refresh volume state
        /// </summary>
        private void RefreshVolumeState()
        {
            UpdateButtonVisibility(true);
            if (_viewModel.GetPropertyValue("NextGenData").ToString().Equals("BagAndEAS"))
            {
                VolumeButton.IsEnabled = false;
            }
            else
            {
                VolumeButton.IsEnabled = (bool)_viewModel.GetPropertyValue("VolumeShown");
            }
        }

        /// <summary>
        /// Refresh assistance state
        /// </summary>
        private void RefreshAssistanceState()
        {
            bool isEnable = false;

            if (_viewModel.GetPropertyValue("NextGenData").ToString().Equals("BagAndEAS"))
            {
                if (paramListAssistanceButton.Contains(_viewModel.StateParam))
                {
                    isEnable = false;
                }
                else
                {
                    if (_viewModel.BackgroundStateParam.Equals("Bag"))
                    {
                        isEnable = (bool)_viewModel.GetPropertyValue("ButtonStoreLogInShown");
                    }
                    else if (paramListAssistanceButton.Contains(_viewModel.StateParam))
                    {
                        isEnable = (bool)_viewModel.GetPropertyValue("AssistanceShown");
                    }
                }
            }
            else
            {
                if (_viewModel.StateParam.Equals("Payment") && _viewModel.BackgroundStateParam.Equals("TakeChange"))
                {
                    isEnable = (bool)_viewModel.GetPropertyValue("AssistanceShown");
                }
                else if (AssistanceFollowsEnabledContexts.Contains(_viewModel.StateParam))
                {
                    isEnable = (bool)_viewModel.GetPropertyValue("AssistanceEnabled");
                }
                else if (_viewModel.StateParam.Equals("NMIPopup") || _viewModel.StateParam.Equals("TransportItem") || _viewModel.StateParam.Equals("TransportItemWithReward"))
                {
                    isEnable = false;
                }
                else if (ParamListForButtonStoreLogin.Contains(_viewModel.StateParam))
                {
                    isEnable = (bool)_viewModel.GetPropertyValue("ButtonStoreLogInShown");
                }
                else if ((bool)_viewModel.GetPropertyValue("AssistanceShown"))
                {
                    if (!_viewModel.StateParam.Equals("Disconnected"))
                    {
                        isEnable = true;
                    }
                }
            }
            AssistanceButton.IsEnabled = isEnable;
        }

        /// <summary>
        /// Updates UNav Focus when in Finish state
        /// </summary>
        private void UpdateUNavFocus()
        {
            if (_viewModel.StateParam.Equals("Finish"))
            {
                FocusHelper.SetInitialFocus(this, _viewModel);
            }
        }

        /// <summary>
        /// RefreshLanguageEventHandlers
        /// adds an event handler to languageCollection but remove first existing event handler
        /// </summary>
        private void RefreshLanguageEventHandlers()
        {
            if (null != _languageCollection)
            {
                _languageCollection.CollectionChanged -= languageCollection_CollectionChanged;
            }
            _languageCollection = _viewModel.GetPropertyValue("Languages") as ObservableCollection<GridItem>;
            if (null != _languageCollection)
            {
                _languageCollection.CollectionChanged += languageCollection_CollectionChanged;
            }
        }

        /// <summary> 
        /// ViewModel PropertyChanged 
        /// </summary> 
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of PropertyChangedEventArgs</param> 
        private void ViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (!_viewModel.StateParam.Equals("ContextHelp"))
            {
                this.UpdateButtonVisibility(true);
            }
            switch (e.PropertyName)
            {
                case "CustomerLanguage":
                    this.RefreshLanguageText();
                    break;
                case "Language":
                    this.RefreshLanguageState();
                    this.RefreshLanguageText();
                    break;
                case "Languages":
                    this.RefreshLanguageEventHandlers();
                    this.RefreshLanguages();
                    this.RefreshLanguageState();
                    this.RefreshLanguageText();
                    break;
                case "CMButton5MidiListShown":
                    this.RefreshLanguages();
                    this.RefreshLanguageState();
                    this.RefreshLanguageText();
                    break;
                case "OwnBagSaleShown":
                case "OwnBagWelcomeShown":
                case "LanguageWelcomeShown":
                    this.RefreshLanguageState();
                    this.RefreshLanguageText();
                    break;
                case "VolumeShown":
                    this.RefreshVolumeState();
                    break;                
                case "StoreMode":
                    this.UpdateWeightDetailsPanel();
                    this.UpdateSMButton8StoreMode();
                    this.UpdateStoreButtonGoBack();
                    break;               
                case "CMVersionText":
                    this.OnRefreshVersion();
                    break;
                case "CMACSVersionText":
                    this.OnRefreshACSVersion();
                    break;
                case "CMTerminalNumberText":
                    this.OnRefreshTerminalNumber();
                    break;
                case "ButtonStoreLogInShown":
                case "AssistanceEnabled":
                case "AssistanceShown":
                    this.RefreshAssistanceState();
                    UpdateUNavFocus();
                    break;
                case "StateParam":
                    this.UpdateWeightDetailsPanel();
                    this.RefreshLanguageState();
                    this.RefreshLanguageText();
                    this.RefreshAssistanceState();
                    this.OnRefreshACSVersion();
                    this.OnRefreshTerminalNumber();
                    this.OnRefreshVersion();
                    this.UpdateSMButton8StoreMode();
                    this.UpdateStoreButtonGoBack();
					//+127021 meijerGo only mode
					//if (GetPropertyStringValue("TransactionMode").Equals("2") || GetPropertyStringValue("TransactionMode").Equals("3"))	 
                    if (!(_viewModel.GetPropertyValue("TransactionMode").ToString().Equals("0")))
					{
						UpdateButtonVisibility(false);
						this.OnMeijerGo();
                    }
                    else
                    {
                        this.OnSCOMode();
                    }
                    //-127021
                    break;
                case "NextGenData":
                    this.RefreshAssistanceState();
                    this.RefreshVolumeState();
                    this.RefreshLanguageState();
                    this.RefreshLanguageText();
                    break;                
                case "SMButton1Text":
                case "SMButton1Shown":
                case "SMButton1Enabled":
                case "SMButton8Text":
                case "SMButton8Shown":
                case "SMButton8Enabled":
                case "ButtonGoBack":
                case "ButtonGoBackEnabled":
                case "ButtonGoBackShown":
                case "XMButton8Text":
                case "XMButton8Enabled":
                case "XMButton8Shown":
                case "CommandButton_GoBackText":
                case "CommandButton_GoBackEnabled":
                case "CommandButton_GoBackShown":
                    this.UpdateStoreButtonGoBack();
                    break;
                case "BackgroundStateParam":
                    this.RefreshAssistanceState();
                    break;
            }
        }

        /// <summary>
        /// Update Assistance and StoreButton8
        /// </summary>
        private void UpdateSMButton8StoreMode()
        {
            if (_viewModel.StoreMode)
            {
                this.AssistanceButton.Visibility = Visibility.Collapsed;
                this.UpdateButtonVisibility(false);
            }
            else
            {
                this.UpdateButtonVisibility(true);
                this.AssistanceButton.Visibility = Visibility.Visible;
                this.StoreButton8.Visibility = Visibility.Collapsed;
            }
        }

       /// <summary>
        /// Checks the state parameter to see if we are on the system message screen.
        /// </summary>
        /// <returns>True if on a system message screen, otherwise false.</returns>
        private bool IsInSystemMessageState()
        {
            switch (_viewModel.StateParam)
            {
                case "SystemMessageHopperFailure":
                case "SystemMessageHopperSubstitution":
                case "SystemMessageDegradedMode":
                case "SystemMessage":
                case "SystemMessageDegradedModeWithBitmap":
                case "SystemMessageWithBitmap":
                case "SystemMessageWithAVI":
                case "AM_SystemMessageWithAVI":
                case "AM_SystemMessageDegradedModeWithBitmap":
                case "AM_SystemMessageWithBitmap":
                case "AM_SystemMessage":
                case "AM_SystemMessageDegradedMode":
                case "AM_SystemMessageHopperFailure":
                case "AM_SystemMessageHopperSubstitution":
                case "CMLoanPickupFailureV6HW":
                case "CMTimedOutWaitingForTBV6HW":
                case "SystemMessageWithWebControl":
                case "SystemMessageDegradedModeWithWebControl":
                case "SystemMessageHopperFailureWithWebControl":
                case "SystemMessageHopperSubstitutionWithWebControl":
                case "AM_SystemMessageWithWebControl":
                case "AM_SystemMessageDegradedModeWithWebControl":
                case "AM_SystemMessageHopperFailureWithWebControl":
                case "AM_SystemMessageHopperSubstitutionWithWebControl":
                    return true;
                default:
                    return false;
            }
        }

        /// <summary>
        /// Update weight details panel
        /// </summary>
        private void UpdateWeightDetailsPanel()
        {
            if (IsInSystemMessageState())
            {
                MainGrid.ColumnDefinitions[0].Width = new GridLength(20);
                Thickness weightDetailsMargin = (Thickness)Resources["weightDetailsPanelMargin"];
                MainGrid.Margin = new Thickness(-30, weightDetailsMargin.Top, weightDetailsMargin.Right, weightDetailsMargin.Bottom);
            }
            else
            {
                MainGrid.ColumnDefinitions[0].Width = (GridLength)Resources["systemFunctionsGridColumn0Width"];
                MainGrid.Margin = (Thickness)Resources["weightDetailsPanelMargin"];
            }

            if (ParamListForCashManagementErrorMessagesV6HW.Contains(_viewModel.StateParam))
            {
                this.WeightDetailsPanel.Visibility = Visibility.Collapsed;
            }
            else
            {
                this.WeightDetailsPanel.Visibility = Visibility.Visible;
            }
        }

        /// <summary>
        /// Update Store Mode Button1's text
        /// </summary>
        private void UpdateSMButton1Text()
        {
            this.StoreButton8.Text = _viewModel.GetPropertyValue("SMButton1Text").ToString();
        }

        /// <summary>
        /// Update whether Store Mode Button1 is shown
        /// </summary>
        private void UpdateSMButton1Shown()
        {
            this.StoreButton8.Visibility = ((bool)_viewModel.GetPropertyValue("SMButton1Shown")) ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Update whether Store Mode Button1 is enabled
        /// </summary>
        private void UpdateSMButton1Enabled()
        {
            this.StoreButton8.IsEnabled = (bool)_viewModel.GetPropertyValue("SMButton1Enabled");
        }

        /// <summary>
        /// Update Store Mode Button8's text
        /// </summary>
        private void UpdateSMButton8Text()
        {
            this.StoreButton8.Text = _viewModel.GetPropertyValue("SMButton8Text").ToString();
        }

        /// <summary>
        /// Update whether Store Mode Button8 is shown
        /// </summary>
        private void UpdateSMButton8Shown()
        {
            this.StoreButton8.Visibility = ((bool)_viewModel.GetPropertyValue("SMButton8Shown")) ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Update whether Store Mode Button8 is enabled
        /// </summary>
        private void UpdateSMButton8Enabled()
        {
            if (_viewModel.StateParam.Equals("EchoPopup") && _viewModel.BackgroundStateParam.Equals("SmSystemFunctions"))
            {
                this.StoreButton8.IsEnabled = false;
            }
            else
            {
                this.StoreButton8.IsEnabled = (bool)_viewModel.GetPropertyValue("SMButton8Enabled");
            }
        }

        /// <summary>
        /// Update ButtonGoBack's text
        /// </summary>
        private void UpdateButtonGoBackText()
        {
            this.StoreButton8.Text = _viewModel.GetPropertyValue("ButtonGoBack").ToString();
        }

        /// <summary>
        /// Update whether ButtonGoBack is shown
        /// </summary>
        private void UpdateButtonGoBackShown()
        {
            this.StoreButton8.Visibility = ((bool)_viewModel.GetPropertyValue("ButtonGoBackShown")) ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Update whether ButtonGoBack is enabled
        /// </summary>
        private void UpdateButtonGoBackEnabled()
        {
            this.StoreButton8.IsEnabled = ((bool)_viewModel.GetPropertyValue("ButtonGoBackEnabled")) ? true : false;
        }

        /// <summary>
        /// Update XMButton8's text
        /// </summary>
        private void UpdateXMButton8Text()
        {
            this.StoreButton8.Text = _viewModel.GetPropertyValue("XMButton8Text").ToString();
        }

        /// <summary>
        /// Update whether XMButton8 is shown
        /// </summary>
        private void UpdateXMButton8Shown()
        {
            this.StoreButton8.Visibility = ((bool)_viewModel.GetPropertyValue("XMButton8Shown")) ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Update whether XMButton8 is enabled
        /// </summary>
        private void UpdateXMButton8Enabled()
        {
            this.StoreButton8.IsEnabled = ((bool)_viewModel.GetPropertyValue("XMButton8Enabled")) ? true : false;
        }

        /// <summary>
        /// Update CommandButton_GoBack's text
        /// </summary>
        private void UpdateCommandButton_GoBackText()
        {
            this.StoreButton8.Text = _viewModel.GetPropertyValue("CommandButton_GoBackText").ToString();
        }

        /// <summary>
        /// Update whether CommandButton_GoBack is shown
        /// </summary>
        private void UpdateCommandButton_GoBackShown()
        {
            this.StoreButton8.Visibility = ((bool)_viewModel.GetPropertyValue("CommandButton_GoBackShown")) ? Visibility.Visible : Visibility.Collapsed;
	       }

        /// <summary>
        /// Update whether CommandButton_GoBack is enabled
        /// </summary>
        private void UpdateCommandButton_GoBackEnabled()
        {
            this.StoreButton8.IsEnabled = ((bool)_viewModel.GetPropertyValue("CommandButton_GoBackEnabled")) ? true : false;
        }       

        /// <summary>
        /// Event handler for VolumeButton's Click event
        /// </summary>
        /// <param name="sender">Sending object</param>
        /// <param name="e">Event arguments</param>
        private void VolumeButton_Click(object sender, RoutedEventArgs e)
        {
            _viewModel.ActionCommand.Execute("ViewModelSet(Context;ShowVolume)");
        }

        /// <summary>
        /// Event handler for the 'Language' button's Click event
        /// </summary>
        /// <param name="sender">event sender</param>
        /// <param name="e">event arguments</param>
        private void LanguageButton_Click(object sender, RoutedEventArgs e)
        {
            if (_languages > 2)
            {
                VolumeButton.IsEnabled = false;
            }
        }

        /// <summary>
        /// Event handler for StoreButton8's Click event
        /// </summary>
        /// <param name="sender">Sending object</param>
        /// <param name="e">Event arguments</param>
        private void StoreButton8_Click(object sender, RoutedEventArgs e)
        {
            switch (_viewModel.StateParam)
            {
                case "AM_EnterWeight":
                case "AM_EnterWtForPriceEmbedded":
                case "AM_ScaleBroken":
                case "AM_SelectTare":
                case "AM_VoidItem":
                case "SystemMessageDegradedModeWithBitmap":
                case "SystemMessageWithBitmap":
                case "AM_SystemMessageDegradedModeWithBitmap":
                case "AM_SystemMessageWithBitmap":
                case "SystemMessageWithWebControl":
                case "SystemMessageDegradedModeWithWebControl":
                case "SystemMessageHopperFailureWithWebControl":
                case "SystemMessageHopperSubstitutionWithWebControl":
                case "AM_SystemMessageWithWebControl":
                case "AM_SystemMessageDegradedModeWithWebControl":
                case "AM_SystemMessageHopperFailureWithWebControl":
                case "AM_SystemMessageHopperSubstitutionWithWebControl":
                case "SystemMessageWithAVI":
                case "AM_SystemMessageWithAVI":
                case "SystemMessageHopperFailure":
                case "SystemMessageHopperSubstitution":
                case "SystemMessageDegradedMode":
                case "SystemMessage":
                case "AM_SystemMessage":
                case "AM_SystemMessageDegradedMode":
                case "AM_SystemMessageHopperFailure":
                case "AM_SystemMessageHopperSubstitution":
                case "AM_EnterCouponValue":
                case "AM_EnterQuantity":
                case "AM_KeyInCode":
                    _viewModel.ActionCommand.Execute("ButtonGoBack");
                    break;
                case "XMMediaStatus":
                    _viewModel.ActionCommand.Execute("XMButton8");
                    break;
                case "SmAuthorizationHC":
                case "SmAssistMenuHC":
                case "SmAssistMenuFinalizeHC":
                case "SmAssistMenuFinalize":
                    _viewModel.ActionCommand.Execute(string.Format("CommandButton_GoBack({0})", "OnReturnToShopping"));
                    break;
                case "SmAssistMode":
                    _viewModel.ActionCommand.Execute("SMButton1");
                    break;
                default:
                    _viewModel.ActionCommand.Execute("SMButton8");
                    break;
            }
        }

        /// <summary>
        /// Update StoreButtonGoBack's state
        /// </summary>
        private void UpdateStoreButtonGoBack()
        {
            switch (_viewModel.StateParam)
            {
                case "AM_EnterWeight":
                case "AM_EnterWtForPriceEmbedded":
                case "AM_ScaleBroken":
                case "AM_SelectTare":
                case "AM_VoidItem":
                case "AM_EnterCouponValue":
                case "AM_EnterQuantity":
                case "AM_KeyInCode":
                case "SystemMessageDegradedModeWithBitmap":
                case "SystemMessageWithBitmap":
                case "AM_SystemMessageDegradedModeWithBitmap":
                case "AM_SystemMessageWithBitmap":
                case "SystemMessageWithWebControl":
                case "SystemMessageDegradedModeWithWebControl":
                case "SystemMessageHopperFailureWithWebControl":
                case "SystemMessageHopperSubstitutionWithWebControl":
                case "AM_SystemMessageWithWebControl":
                case "AM_SystemMessageDegradedModeWithWebControl":
                case "AM_SystemMessageHopperFailureWithWebControl":
                case "AM_SystemMessageHopperSubstitutionWithWebControl":
                case "SystemMessageWithAVI":
                case "AM_SystemMessageWithAVI":
                case "SystemMessageHopperFailure":
                case "SystemMessageHopperSubstitution":
                case "SystemMessageDegradedMode":
                case "SystemMessage":
                case "AM_SystemMessage":
                case "AM_SystemMessageDegradedMode":
                case "AM_SystemMessageHopperFailure":
                case "AM_SystemMessageHopperSubstitution":
                    this.UpdateButtonGoBackShown();
                    this.UpdateButtonGoBackEnabled();
                    this.UpdateButtonGoBackText();
                    break;
                case "XMMediaStatus":
                    this.UpdateXMButton8Shown();
                    this.UpdateXMButton8Enabled();
                    this.UpdateXMButton8Text();
                    break;
                case "SmAuthorizationHC":
                case "SmAssistMenuHC":
                case "SmAssistMenuFinalizeHC":
                case "SmAssistMenuFinalize":
                    this.UpdateCommandButton_GoBackShown();
                    this.UpdateCommandButton_GoBackEnabled();
                    this.UpdateCommandButton_GoBackText();
                    break;
                case "SmAssistMode":
                    this.UpdateSMButton1Shown();
                    this.UpdateSMButton1Enabled();
                    this.UpdateSMButton1Text();
                    break;
                case "Disconnected":
                    this.StoreButton8.Visibility = Visibility.Collapsed;
                    break;
                default:
                    this.UpdateSMButton8Shown();
                    this.UpdateSMButton8Enabled();
                    this.UpdateSMButton8Text();
                    break;
            }
        }

        /// <summary> 
        /// Toggles uNav on/off.
        /// </summary> 
        private void ToggleUNav()
        {
            if (!_viewModel.IsUNavEnabled)
            {
                _viewModel.UNav = "basic";
            }
            else
            {
                _viewModel.UNav = string.Empty;
            }
        }
        /// <summary>
        /// OnRefreshVersion method
        /// </summary>
        private void OnRefreshVersion()
        {
            if (_viewModel.StateParam.Equals("Welcome")
                || _viewModel.StateParam.Equals("AttractMultiLanguage"))
            {
                string version = _viewModel.GetPropertyValue("CMVersionText").ToString();

                VersionTextBlock.Text = FormatVersion(version, "SCO");
                this.OnSCOMode();
            }
            else
            {
                VersionTextBlock.Text = String.Empty;
            }
        }

        /// <summary>
        /// OnRefreshACSVersion method
        /// </summary>
        private void OnRefreshACSVersion()
        {
            if (_viewModel.StateParam.Equals("Welcome")
                || _viewModel.StateParam.Equals("AttractMultiLanguage"))
            {
                string acsVersion = _viewModel.GetPropertyValue("CMACSVersionText").ToString();

                ACSVersionTextBlock.Text = FormatVersion(acsVersion, "ACS");
                this.OnSCOMode();
            }
            else
            {
                ACSVersionTextBlock.Text = String.Empty;
            }
        }

        private string FormatVersion(string version, string application)
        {
            if(string.IsNullOrEmpty(version))
                return string.Empty;
            var tokens = version.Trim().Split(new char[] { '.' }, StringSplitOptions.RemoveEmptyEntries);
            if(tokens != null && tokens.Length > 1)
            {
                var formattedVersion = new StringBuilder();
                switch (application)
                {
                    case "ACS":
                        formattedVersion.Append("vACS.");
                        break;
                    case "SCO":
                        formattedVersion.Append("v");
                        formattedVersion.Append(tokens[tokens.Length - 3]);//PI number
                        formattedVersion.Append(".SCO.");
                        break;
                }
                formattedVersion.Append(tokens[tokens.Length - 2]);//Fruit name
                formattedVersion.Append(".");
                formattedVersion.Append(tokens[tokens.Length - 1]);//Build number
                return formattedVersion.ToString();
            }
            return string.Empty;
        }

        /// <summary>
        /// OnRefreshTerminalNumber method
        /// </summary>
        private void OnRefreshTerminalNumber()
        {
            if (_viewModel.StateParam.Equals("Welcome")
                || _viewModel.StateParam.Equals("AttractMultiLanguage"))
            {
                string termNum = _viewModel.GetPropertyValue("CMTerminalNumberText").ToString();

                TerminalNumberTextBlock.Text = termNum;
            }
            else
            {
                TerminalNumberTextBlock.Text = String.Empty;
            }
        }        
		// +127021
        private void OnMeijerGo()
        {
            WeightDetailsPanel.Visibility = Visibility.Collapsed;

            this.ColumnVersion.Width = new GridLength(500, GridUnitType.Auto);
            this.VersionBlock.Width = 500;
            this.TerminalNumberTextBlock.Width = 500;
            this.VersionTextBlock.Width = 500;
            this.ACSVersionTextBlock.Width = 500;
            this.VersionTextBlock.Width = 500;
        }
        // -127021

        //method to use to adjust build info for SCO mode
        private void OnSCOMode()
        {
            this.ColumnVersion.Width = new GridLength(120, GridUnitType.Auto);
            this.VersionBlock.Width = 120;
            this.TerminalNumberTextBlock.Width = 120;
            this.ACSVersionTextBlock.Width = 120;
            this.VersionTextBlock.Width = 120;
            if(ACSVersionTextBlock.Text.Length < 24 && VersionTextBlock.Text.Length < 24) //Default height can accommodate only 24 chars. Increase if needed
            {
                VersionBlock.RowDefinitions[1].Height = new GridLength(15); 
                VersionBlock.RowDefinitions[2].Height = new GridLength(15);
                VersionBlock.Margin = new Thickness(0, 50, 0, 0);
            }
            else
            {
                VersionBlock.RowDefinitions[1].Height = new GridLength(25);
                VersionBlock.RowDefinitions[2].Height = new GridLength(25);
                VersionBlock.Margin = new Thickness(0, 30, 0, 0);
            }
        }
    }
}