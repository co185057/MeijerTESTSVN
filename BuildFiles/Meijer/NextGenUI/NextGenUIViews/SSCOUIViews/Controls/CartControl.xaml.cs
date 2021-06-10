// <copyright file="CartControl.xaml.cs" company="NCR Corporation">
//     Copyright (c) NCR Corporation. All rights reserved.
// </copyright>
namespace SSCOUIViews.Controls
{
    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.Collections.Specialized;
    using System.ComponentModel;
    using System.Globalization;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Data;
    using System.Windows.Input;
    using System.Windows.Media;
    using System.Windows.Media.Animation;
    using System.Windows.Threading;
    using FPsxWPF.Controls;
    using RPSWNET;
    using SSCOControls;
    using SSCOControls.Helpers.KeyboardSimulatorComponents;
    using SSCOControls.Interfaces;
    using SSCOUIModels;
    using SSCOUIModels.Helpers;
    using SSCOUIModels.Models;

    /// <summary>
    /// The Cart Control class handles the electronic receipt on the main display.
    /// </summary>
    public partial class CartControl : Grid, INavigable
    {
        /// <summary>
        /// Dependency Property backing store for NMI.
        /// </summary>
        public static readonly DependencyProperty NMIProperty =
            DependencyProperty.Register("NMI", typeof(bool), typeof(CartControl), new PropertyMetadata(false));

        /// <summary>
        /// This is the amount of device independent units the pay amount grows by when it is clipped.
        /// </summary>
        private const double AMOUNT_WIDTH_ADJUSTMENT = 62;

        /// <summary>
        /// List variable
        /// </summary>
        private static List<string> paramListForRewardInfoToShow = new List<string>() { "ScanWithReward", "QuickPickWithReward", "Scan", "QuickPick", "VoidItemWithReward", "SmAssistMode" };
		
        /// <summary>
        /// List variable
        /// </summary>
        private static List<string> paramListForPayBtnShowAndCartCollapsed = new List<string>() { "Scan", "QuickPick", "ScanWithReward", "QuickPickWithReward", "ProduceFavorites", "MultiSelectProduceFavorites", "Finish", "EnterCoupons", "CashPayment", "SmAssistMenu", "SmAborted", "SmDataEntry", "SmVisualItems", "SmCouponItems", "SmVoidAgeRestrictedItem" };
		
        /// <summary>
        /// List variable
        /// </summary>
        private static List<string> paramListForPayBtnHideAndCartExpand = new List<string>() { "VoidItem", "VoidItemWithReward", "ConfirmVoid", "ConfirmAbort", "VoidApproval", "AM_VoidItem", "AM_ConfirmAbort", "AM_ConfirmVoid" };

        /// <summary>
        /// List variable
        /// </summary>
        private static List<string> paramListForPayAndEditCartBtnEnable = new List<string>() { "Bag", "CheckBasket", "ConfirmVoid", "UnDeActivatedItemApproval", "VoidItem", "VoidItemWithReward", "VoidApproval" };

        /// <summary>
        /// List variable
        /// </summary>
        private static List<string> paramListForSale = new List<string>() { "Bag", "EnterCoupons", "QuickPick", "QuickPickWithReward", "Scan", "ScanWithReward" };

        /// <summary>
        /// List variable
        /// </summary>
        private static List<string> paramListForPayAndEditCartHideAndCartCollapsed = new List<string>() { "ScanVoucher", "CardMisRead", "DepositCoupon", "SmRestrictedItems", "DepositGiftCard", "InsertGiftCard", "SmAssistMode", "AM_VoidNoMatch", "AM_CustomMessage" };

        /// <summary>
        /// The original width of the pay amount text block.
        /// </summary>
        private readonly double _originalPayAmountWidth;

        /// <summary>
        /// The original width of the pay text next to the amount.
        /// </summary>
        private readonly double _originalPayTitleWidth;

        /// <summary>
        /// The original margin of the pay text next to the amount.
        /// </summary>
        private readonly Thickness _originalPayTitleMargin;

        /// <summary>
        /// The original width of the due amount.
        /// </summary>
        private readonly double _originalDueAmountWidth;

        /// <summary>
        /// The original width of the due text next to the amount.
        /// </summary>
        private readonly double _originalDueTitleWidth;

        /// <summary>
        /// Holds the original fade in animation value found in the resource.
        /// </summary>
        private Storyboard _originalFadeIn;

        /// <summary>
        /// _viewModel variable
        /// </summary>
        private IMainViewModel _viewModel;

        /// <summary>
        /// confirm void variable
        /// </summary>
        private bool _hasConfirmVoid;

        /// <summary>
        /// void approval variable
        /// </summary>
        private bool _hasVoidApproval;

        /// <summary>
        /// enter id variable
        /// </summary>
        private bool _hasEnterId;

        /// <summary>
        /// Initializes a new instance of the CartControl class.
        /// </summary>
        public CartControl()
        {
            InitializeComponent();

            NMI = Properties.Settings.Default.NMI;

            //if (Properties.Settings.Default.ShopAndDrop)
            //+CRD 125377
           // if (_viewModel.GetPropertyValue("TransactionMode").ToString().Equals("1"))
          //  {

          //  }
            //-CRD 125377
            _originalPayAmountWidth = PayAmountValue.Width;
            _originalPayTitleWidth = PayTitleText.Width;
            _originalPayTitleMargin = PayTitleText.Margin;

            _originalDueAmountWidth = DueAmountValue.Width;
            _originalDueTitleWidth = DueTitleText.Width;

            PayAmountValue.ClippingChanged += PayAmountValue_ClippingChanged;
            DueAmountValue.ClippingChanged += DueAmountValue_ClippingChanged;

            _originalFadeIn = FindResource("fadeInStoryboard") as Storyboard;

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
        /// Gets or sets a value indicating whether or not minimum size requirements for the National Measurement institute are enabled.
        /// </summary>
        public bool NMI
        {
            get { return (bool)GetValue(NMIProperty); }
            set { SetValue(NMIProperty, value); }
        }

        /// <summary>
        /// Preview key down handler. Typically used for arrow and space keys.
        /// Be sure to check IsControlFocused before running code.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        public void Control_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            var originalSource = e.OriginalSource as System.Windows.FrameworkElement;
            CartListItem item = null;
            CustomerReceiptItem item2 = null;
            int itemIndex = -1;
            if (originalSource != null && originalSource.Parent != null && originalSource.Name.Equals("RemoveButton"))
            {
                item = ((System.Windows.FrameworkElement)originalSource.Parent).Parent as CartListItem;
                if (item != null)
                {
                    item2 = item.DataContext as CustomerReceiptItem;
                    var itemsList = CartReceipt.ItemsSource as ObservableCollection<ReceiptItem>;
                    if (item2 != null)
                    {
                        itemIndex = itemsList.IndexOf(item2);
                    }
                }
            }

            switch (e.Key)
            {
                case Key.Space:
                    if (_viewModel.StateParam.Equals("Bag"))
                    {
                        e.Handled = true;
                    }

                    return;
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
                case Key.Up:
                    if (itemIndex < 0)
                    {
                        break;
                    }

                    if (itemIndex == 0)
                    {
                        KeyboardSimulator.Instance.SimulateKeyPress(VirtualKeyCode.TAB);
                    }
                    else
                    {
                        KeyboardSimulator.Instance.SimulateKeyPress(VirtualKeyCode.LEFT);
                    }

                    e.Handled = true;

                    break;
                case Key.Down:
                    if (itemIndex < 0)
                    {
                        break;
                    }

                    if (itemIndex == CartReceipt.Items.Count - 1)
                    {
                        KeyboardSimulator.Instance.SimulateKeyPress(VirtualKeyCode.TAB);
                    }
                    else
                    {
                        KeyboardSimulator.Instance.SimulateKeyPress(VirtualKeyCode.LEFT);
                    }

                    e.Handled = true;

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
        /// This responds to the Due amount text being clipped. It shrinks the label to the left and grows the due amount area.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void DueAmountValue_ClippingChanged(object sender, EventArgs e)
        {
            if (DueAmountValue.IsClipped)
            {
                DueAmountValue.IgnoreNextClippingChange = true;
                DueAmountValue.Width = _originalDueAmountWidth + AMOUNT_WIDTH_ADJUSTMENT;
                DueTitleText.Width = _originalDueTitleWidth - AMOUNT_WIDTH_ADJUSTMENT;
            }
            else
            {
                DueAmountValue.Width = _originalDueAmountWidth;
                DueTitleText.Width = _originalDueTitleWidth;
                DueTitleText.FontSize = DueTitleText.PreferredFontSize;
            }
        }

        /// <summary>
        /// Handles the text clipping event from the pay amount.
        /// </summary>
        /// <param name="sender">event sender</param>
        /// <param name="e">event arguments</param>
        private void PayAmountValue_ClippingChanged(object sender, EventArgs e)
        {
            if (PayAmountValue.IsClipped)
            {
                PayArrowImage.Visibility = System.Windows.Visibility.Collapsed;
                PayAmountValue.IgnoreNextClippingChange = true;
                PayAmountValue.Width = _originalPayAmountWidth + AMOUNT_WIDTH_ADJUSTMENT;
                PayTitleText.Width = _originalPayTitleWidth - 16;
                PayTitleText.Margin = new Thickness(0, 0, 0, 0);
            }
            else
            {
                PayArrowImage.Visibility = System.Windows.Visibility.Visible;
                PayAmountValue.Width = _originalPayAmountWidth;
                PayTitleText.Width = _originalPayTitleWidth;
                PayTitleText.Margin = _originalPayTitleMargin;
            }
        }

        /// <summary>
        /// CartReceipt Loaded
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of RoutedEventArgs</param> 
        private void CartReceipt_Loaded(object sender, RoutedEventArgs e)
        {
            if (CmDataCapture.IsCaptureActive(CmDataCapture.MaskExtensive))
            {
                try
                {
                    ((INotifyCollectionChanged)CartReceipt.Items).CollectionChanged += CartReceipt_CollectionChanged;
                }
                catch
                {
                }
            }

            OnNextGenDataChanged();
        }
       
        /// <summary>
        /// CartReceipt CollectionChanged
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of NotifyCollectionChangedEventArgs</param> 
        private void CartReceipt_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            _viewModel.Perfcheck.StartEventLog(PerfMeasureEvents.CartReceiptRender);
            if (_viewModel.Perfcheck.CartReceiptRenderStarted)
            {
                Dispatcher.BeginInvoke(
                    DispatcherPriority.Loaded,
                    new Action(() =>
                    {
                        _viewModel.Perfcheck.EndEventLog(PerfMeasureEvents.CartReceiptRender);
                    }));
            }

			// +SSCOI-46440
			ObservableCollection<ReceiptItem> customerReceiptCollection = null;
            if (GetPropertyBoolValue("TaxListShown"))
                customerReceiptCollection = this._viewModel.GetPropertyValue("TaxList") as ObservableCollection<ReceiptItem>;
            else if (GetPropertyBoolValue("EASListShown"))	//CRD 212453
                customerReceiptCollection = this._viewModel.GetPropertyValue("EASList") as ObservableCollection<ReceiptItem>;
            else
                customerReceiptCollection = this._viewModel.GetPropertyValue("CustomerReceipt") as ObservableCollection<ReceiptItem>;
			// -SSCOI-46440
            if (NotifyCollectionChangedAction.Add == e.Action)
            {
                UpdateCartReceiptIndexSelected(_viewModel.StateParam);
            }
        }

        /// <summary>
        /// Grid DataContextChanged
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of DependencyPropertyChangedEventArgs</param> 
        private void Grid_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            _viewModel = DataContext as IMainViewModel;
            _viewModel.PropertyChanged += new PropertyChangedEventHandler(ViewModel_PropertyChanged);
            UpdateCartReceiptItemsSource();
            OnBackgroundStateParamChanged(_viewModel.BackgroundStateParam);
            OnStateParamChanged(_viewModel.StateParam);
            OnTotalChanged();
            OnAmountDueChanged();
            OnTaxChanged();
            ShowHideTax();
            OnCMTotalCMDiscountChanged();
            OnCMTotalGMChanged();
            OnCMTotalCMDiscountChanged();
            OnReceiptItemCountChange();
            SetPanelZIndex();
            UpdateCartReceiptIndexSelected(_viewModel.StateParam);
            UpdateEditCartCommandParameter();
            UpdateCancelAllButton();
        }

        /// <summary>
        /// Method for handling background state parameter changes.
        /// </summary>
        /// /// <param name="param">This is a parameter with a type of String</param>
        private void OnBackgroundStateParamChanged(string param)
        {
            if (param != null)
            {
                ShowPayButton(paramListForSale.Contains(param));
            }
        }

        /// <summary>
        /// Method for handling state parameter changed.
        /// </summary>   
        /// <param name="param">This is a parameter with a type of String</param>
        private void OnStateParamChanged(string param)
        {
            if (param != null)
            {
                UpdateCartReceiptItemsSource();
                UpdateCartReceiptIndexSelected(param);
                UpdateFinishButton();
                cartControlBox.IsEnabled = true;
                CartReceipt.DisableSelection = param.Equals("Bag") || param.Equals("AM_KeyInPrice") || param.Equals("AM_KeyInQuantity");
                SetPanelZIndex();
                RewardInfo.Visibility = Visibility.Collapsed;
				// SSCOI-47003: Remove code below as it made the Tax line disappear in StoreMode/Assistmode
                // TaxText.Visibility = TaxValue.Visibility = Visibility.Collapsed;

                if (!_viewModel.StoreMode)
                {
                    ShowHideRewardInfo();
                    ShowHideTotalRewards();
                    ShowHideAmountSaved();
                    ShowHideTax();
                }

                // start CRD 125377
                //if (Properties.Settings.Default.ShopAndDrop)    
                if (_viewModel.GetPropertyValue("TransactionMode").ToString().Equals("1"))
                {
                    SNDSmallLogo.Visibility = Visibility.Visible;
                    SDNCartInfoText.Visibility = Visibility.Visible;

                    ReceiptItemCount.Visibility = Visibility.Collapsed;
                    EditCartBtn.Visibility = Visibility.Collapsed;
                    SNDCartImage.Visibility = Visibility.Collapsed;
                                        
					HideTotals();
                    
                    CartControlExpand(false);

                    SND30Percent.Visibility = Visibility.Collapsed;
                    SND60Percent.Visibility = Visibility.Collapsed;
                    if (param.Equals("Scan"))
                    {
                        SND30Percent.Visibility = Visibility.Visible;
                    }
                    else if (param.Equals("Payment"))
                    {
                        SND60Percent.Visibility = Visibility.Visible;
                    }
                 }
				//Start CRD 150652
				else if(_viewModel.GetPropertyValue("IsAuditOnSSCO").ToString().Equals("1"))
				{
					HideTotals();
					CartControlExpand(false);
				}
				//End CRD 150652
                else if (paramListForPayBtnShowAndCartCollapsed.Contains(param))
                {
                    CartControlExpand(false);
                    GoBackBtn.Visibility = Visibility.Collapsed;
                    ShowPayButton(true);
                    CartReceipt.AutoScrollIntoView = AutoScrollSelection.Bottom;
                }
                else if (paramListForPayBtnHideAndCartExpand.Contains(param))
                {
                    GoBackBtn.Visibility = param.Equals("AM_VoidItem") || param.Equals("AM_ConfirmAbort") || param.Equals("AM_ConfirmVoid") ? Visibility.Collapsed : Visibility.Visible;
                    CartControlExpand(true);
                    ShowPayButton(false);
                }
                else if (param.Equals("Finish"))
                {
                    OnReceiptItemCountChange();
                }
                else if (param.Equals("Payment"))
                {
                    OnTotalChanged();
                    OnAmountDueChanged();
                    OnTaxChanged();
                    OnCMTotalGMChanged();
                    OnCMTotalCMDiscountChanged();
                    OnCMTotalTMDiscountChanged();
                    CartControlExpand(false);
                    GoBackBtn.Visibility = Visibility.Collapsed;
                }
                else if (param.Equals("Bag"))
                {
                    CartReceipt.AutoScrollIntoView = AutoScrollSelection.Bottom;
                }
                else if (paramListForPayAndEditCartHideAndCartCollapsed.Contains(param))
                {
                    OnTotalChanged();
                    OnAmountDueChanged();
                    OnTaxChanged();
                    OnCMTotalGMChanged();
                    OnCMTotalCMDiscountChanged();
                    OnCMTotalTMDiscountChanged();
                    ShowPayButton(false);
                    CartControlExpand(false);
                    GoBackBtn.Visibility = Visibility.Collapsed;
                    CancelAllBtn.Visibility = Visibility.Collapsed;
                    ShowAssistOperatorMessage(param);
                }
                //start CRD 125377
                else
                {
                    SNDSmallLogo.Visibility = Visibility.Collapsed;
                    SDNCartInfoText.Visibility = Visibility.Collapsed;
                    SND30Percent.Visibility = Visibility.Collapsed;
                    SND60Percent.Visibility = Visibility.Collapsed;

					ShowTotals();
                    ReceiptItemCount.Visibility = Visibility.Visible;
                    EditCartBtn.Visibility = Visibility.Visible;
                    SNDCartImage.Visibility = Visibility.Visible;
                }
                //end CRD 125377

                UpdateFinishButton();
                UpdateEditCartCommandParameter();
                UpdateCancelAllButton();
            }
        }

        private void ShowAssistOperatorMessage(string param)
        {
            if (param.Equals("SmAssistMode"))
            {
                assistOperatorMsgText.Visibility = Convert.ToBoolean(_viewModel.GetPropertyValue("AssistOperatorMessageShown")) ? Visibility.Visible : Visibility.Collapsed;
            }
            else
                assistOperatorMsgText.Visibility = Visibility.Collapsed;
        }

        /// <summary>
        /// UpdateCartReceiptIndexSelected method
        /// </summary>
        /// <param name="param">This is a parameter with a type of string</param>
        private void UpdateCartReceiptIndexSelected(string param)
        {
            switch (param)
            {
                case "Scan":
                case "QuickPick":
                case "ScanWithReward":
                case "QuickPickWithReward":
				case "SmAssistMode": // SSCOI-46440
                    if (CartReceipt.Items.Count > 0 && CartReceipt.SelectedIndex == -1)
                    {
                        CartReceipt.SelectedIndex = 0;
                    }
                    else
                        CartReceipt.AutoScrollIntoView = AutoScrollSelection.Bottom;

                    break;
				case "SmAssistMenu": //SSCOI-50040
                case "AM_VoidItem": //SSCOI-50040
                case "AM_ConfirmAbort":
                    SetStoreModeCartReceiptSelectedIndex(); //SSCOI-50040
                    break;	
            }
        }

        /// <summary>
        /// UpdateCancelAllButton method
        /// </summary>
        /// <param name="param">This is a parameter with a type of string</param>
        private void UpdateCancelAllButton()
        {
            if (GetPropertyBoolValue("NextGenUIAllowCancelAll"))
            {
                CancelAllBtn.Visibility = (GoBackBtn.Visibility.Equals(Visibility.Visible) ||
                    _viewModel.StateParam.Equals("AM_VoidItem") || _viewModel.StateParam.Equals("AM_ConfirmAbort") ||
                    _viewModel.StateParam.Equals("AM_ConfirmVoid")) ? Visibility.Visible : Visibility.Collapsed;
            }
        }

        /// <summary>
        /// Method to show the pay button.
        /// </summary>   
        /// <param name="visibility">true for show, false for hide</param>
        private void ShowPayButton(bool visibility)
        {
			 // start CRD 150652
            if (_viewModel.GetPropertyValue("IsAuditOnSSCO").ToString().Equals("1"))
            {
                EditCartBtn.Visibility = Visibility.Collapsed;
                Total.Visibility = Visibility.Collapsed;
            }
            // end CRD 150652
            else if (_viewModel.StateParam.Equals("EnterCoupons"))
            {
                EditCartBtn.Visibility = Visibility.Collapsed;
                PayButton.Visibility = visibility ? Visibility.Visible : Visibility.Collapsed;
                Total.Visibility = visibility ? Visibility.Collapsed : Visibility.Visible;
            }
            else if (_viewModel.StateParam.Equals("CashPayment"))
            {
                EditCartBtn.Visibility = PayButton.Visibility = Visibility.Collapsed;
                Total.Visibility = Visibility.Visible;
            }
            else if (_viewModel.StateParam.Equals("SmAssistMenu"))
            {
                Total.Visibility = Visibility.Visible;
                EditCartBtn.Visibility = Convert.ToBoolean(_viewModel.GetPropertyValue("SMButton2Shown")) ? Visibility.Visible : Visibility.Collapsed;
                GoBackBtn.Visibility = PayButton.Visibility = Visibility.Collapsed;
                // start CRD 125377
                if (_viewModel.GetPropertyValue("TransactionMode").ToString().Equals("1"))
                {
                    EditCartBtn.Visibility = Visibility.Collapsed;
                    Total.Visibility = Visibility.Collapsed;
                }
                // end CRD 125377
            }
            // start CRD 125377
            else if (_viewModel.GetPropertyValue("TransactionMode").ToString().Equals("1"))
            {
                EditCartBtn.Visibility = Visibility.Collapsed;
                PayButton.Visibility =  Visibility.Collapsed;
                Total.Visibility = Visibility.Collapsed;
            }
            // end CRD 125377
            else
            {
                EditCartBtn.Visibility = PayButton.Visibility = visibility ? Visibility.Visible : Visibility.Collapsed;
                Total.Visibility = visibility ? Visibility.Collapsed : Visibility.Visible;
            }
        }

        /// <summary>
        /// Method to expand the cart control.
        /// </summary>   
        /// <param name="expand">true for expand, false for collapse.</param>
        private void CartControlExpand(bool expand)
        {
            const double CART_WIDTH = 340;
            const double CART_EXPANDED_WIDTH = 452;
            if ((expand && (CART_EXPANDED_WIDTH != cartControlBox.Width)) || (!expand && (CART_WIDTH != cartControlBox.Width)))
            {
                double toWidth = expand ? CART_EXPANDED_WIDTH : CART_WIDTH;
                cartControlBox.Width = toWidth;
                if (_viewModel.ShowTransitionEffects)
                {
                    double fromWidth = expand ? CART_WIDTH : CART_EXPANDED_WIDTH;
                    Storyboard storyboard = new Storyboard();

                    storyboard.Completed += Storyboard_Completed;

                    DoubleAnimation expandAnimation = new DoubleAnimation(
                        fromWidth,
                        toWidth,
                        TimeSpan.FromMilliseconds(_viewModel.TransitionDuration),
                        FillBehavior.Stop);
                    Storyboard.SetTargetName(expandAnimation, "cartControlBox");
                    Storyboard.SetTargetProperty(expandAnimation, new PropertyPath(Grid.WidthProperty));
                    storyboard.Children.Add(expandAnimation);
                    storyboard.Begin(this);
                }
                else
                {
                    FocusInitialButton();
                }
            }
        }

        /// <summary>
        /// Runs when storyboard plays to completion.
        /// </summary>
        /// <param name="sender">Sending object</param>
        /// <param name="e">Event arguments</param>
        private void Storyboard_Completed(object sender, EventArgs e)
        {
            FocusInitialButton();
        }

        /// <summary>
        /// Set focus initially
        /// </summary>
        private void FocusInitialButton()
        {
            if (_viewModel.IsUNavEnabled)
            {
                if (GoBackBtn.IsVisible)
                {
                    FocusHelper.SetInitialFocus(this, _viewModel);
                }
            }
        }

        /// <summary>
        /// UpdateFinishButton method
        /// </summary>  
        private void UpdateFinishButton()
        {
            if (!_viewModel.StoreMode)
            {
                EditCartBtn.IsEnabled = PayButton.IsEnabled = !paramListForPayAndEditCartBtnEnable.Contains(_viewModel.StateParam);
            }
            else
            {
                EditCartBtn.IsEnabled = Convert.ToBoolean(_viewModel.GetPropertyValue("SMButton2Enabled"));
            }
        }

        /// <summary>
        /// ViewModel PropertyChanged
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of PropertyChangedEventArgs</param> 
        private void ViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch (e.PropertyName)
            {
                case "BackgroundStateParam":
                    OnBackgroundStateParamChanged(_viewModel.BackgroundStateParam);
                    break;
                case "StoreMode":
                    OnStoreModePropertyChanged();
                    break;
                case "StateParam":
                    OnStateParamChanged(_viewModel.StateParam);
                    break;
                case "Total":
                case "SMTotal":
                    OnTotalChanged();
                    break;
                case "Tax":
                    OnTaxChanged();
                    break;
                case "NextGenUIAmountDue":
                    OnAmountDueChanged();
                    break;
                case "NextGenUIIsTaxLine":
                    ShowHideTax();
                    break;
                case "CMTotalCMDiscountAmount":
                    OnCMTotalCMDiscountChanged();
                    break;
                case "CMTotalGMAmount":
                    OnCMTotalGMChanged();
                    break;
                case "CMTotalTMDiscountAmount":
                    OnCMTotalTMDiscountChanged();
                    break;
                case "ReceiptItemCount":
                    OnReceiptItemCountChange();
                    break;
                case "NextGenData":
                    OnNextGenDataChanged();
                    break;
                case "CurrentItem":
                    UpdateCartReceiptIndexSelected(_viewModel.StateParam);
                    break;
                case "SMButton2Enabled":
                    EditCartBtn.IsEnabled = Convert.ToBoolean(_viewModel.GetPropertyValue("SMButton2Enabled"));
                    break;
                case "AssistOperatorMessageShown":
                    ShowAssistOperatorMessage(_viewModel.StateParam);
                    break;
                case "NextGenUICMTotalRewardPoints":
                    ShowHideTotalRewards();
                    break;
                case "NextGenUICMTotalSavings":
                    ShowHideAmountSaved();
                    break;
                case "ShowTransitionEffects":
                    Resources["fadeInStoryboard"] = (bool)_viewModel.GetPropertyValue("ShowTransitionEffects") ? _originalFadeIn : null;
                    break;
                case "NextGenUICMRewardInfo":
                    ShowHideRewardInfo();
                    break;
                case "NextGenUIAllowCancelAll":
                    UpdateCancelAllButton();
                    break;
                case "UNav":
                    FocusInitialButton();
                    break;
                case "IsTransitioning":
                    if (!_viewModel.IsTransitioning)
                    {
                        FocusInitialButton();
                    }
                    break;
				case "TaxListShown": // SSCOI-46440
                case "EASListShown": //CRD 212453
                    UpdateCartReceiptItemsSource();
                    break;
            }
        }

        /// <summary>
        /// ShowHideTax method
        /// </summary>
        private void ShowHideTax()
        {
            TaxText.Visibility = TaxValue.Visibility = GetPropertyBoolValue("NextGenUIIsTaxLine") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// OnTotalChanged method
        /// </summary>
        private void OnTotalChanged()
        {
            string totalName = _viewModel.StoreMode ? "SMTotal" : "Total";
            object total = _viewModel.GetPropertyValue(totalName);
            if (total != null)
            {
                double totalVal = 0;
                double.TryParse(total.ToString(), out totalVal);
                TotalAmountValue.Value = totalVal;
            }
        }

        /// <summary>
        /// Method for handling amount due changed. Sets both the pay and due amount text blocks.
        /// </summary>
        private void OnAmountDueChanged()
        {
            object due = _viewModel.GetPropertyValue("NextGenUIAmountDue");
            if (due != null)
            {
                DueAmountValue.Value = PayAmountValue.Value = due.ToString();
            }
        }

        /// <summary>
        /// OnTaxChanged method
        /// </summary>
        private void OnTaxChanged()
        {
            object val = _viewModel.GetPropertyValue("Tax");
            if (val != null)
            {
                TaxValue.Value = val.ToString();
            }
        }

        /// <summary>
        /// OnReceiptItemCountChange method
        /// </summary>
        private void OnReceiptItemCountChange()
        {
            int count = 0;
            object val = _viewModel.GetPropertyValue("ReceiptItemCount");
            if (val != null)
            {
                if (!int.TryParse(val.ToString(), out count))
                {
                    count = 0;
                }
            }

            ReceiptItemCount.Property(TextBlock.TextProperty).SetResourceFormattedValue(count > 1 ? "Items" : "Item", count);
        }

        private void OnCMTotalCMDiscountChanged()
        {
            object val = this._viewModel.GetPropertyValue("CMTotalCMDiscountAmount");
            if (null != val)
            {
                this.CMAmount.Value = val.ToString();
            }

        }

        private void OnCMTotalGMChanged()
        {
            object val = this._viewModel.GetPropertyValue("CMTotalGMAmount");
            if (null != val)
            {
                this.GMAmount.Value = val.ToString();
            }

        }

        private void OnCMTotalTMDiscountChanged()
        {
            object val = this._viewModel.GetPropertyValue("CMTotalTMDiscountAmount");
            if (null != val)
            {
                this.TMDAmount.Value = val.ToString();
            }

        }

        /// <summary>
        /// OnNextGenDataChanged method
        /// </summary>
        private void OnNextGenDataChanged()
        {
            if ((_viewModel.GetPropertyValue("NextGenData") as string).Equals("BagAndEAS"))
            {
                IsEnabled = false;
                CmDataCapture.Capture(CmDataCapture.MaskPerformance, "CartControl NextGenData=BagAndEAS");
            }
            else
            {
                IsEnabled = true;
            }
        }

        /// <summary>
        /// Shows the cart when removing an item.
        /// </summary>
        private void SetPanelZIndex()
        {
            if (_viewModel.StateParam.Equals("VoidItem") ||
                _viewModel.StateParam.Equals("VoidItemWithReward") ||
                _viewModel.StateParam.Equals("AM_VoidItem"))
            {
                Panel.SetZIndex(cartControlBox, 1);
            }
            else
            {
                Panel.SetZIndex(cartControlBox, 0);
            }
        }

        /// <summary>
        /// Refreshes the data binding for the cart receipt items.
        /// </summary>
        private void UpdateCartReceiptItemsSource()
        {
            switch (_viewModel.StateParam)
            {
                case "AM_ConfirmQuantity":
                case "AM_ContinueTrans":
                case "AM_CouponNoMatch":
                case "AM_CouponNotAllowed":
                case "AM_CouponTooHigh":
                case "AM_CrateableItem":
                case "AM_CustomMessage":
                case "AM_EnterCouponValue":
                case "AM_EnterQuantity":
                case "AM_EnterWeight":
                case "AM_EnterWtForPriceEmbedded":
                case "AM_ItemQuantityExceeded":
                case "AM_KeyInCode":
                case "AM_KeyInPrice":
                case "AM_KeyInQuantity":
                case "AM_Processing":
                case "AM_RestrictedNotAllowed":
                case "AM_ScaleBroken":
                case "AM_UpdateNotAllowed":
                case "AM_VoidNoMatch":
                case "RemoteSystemMessage":
                case "SmAbort":
                case "SmAborted":
                case "SmAssistMenu":
                case "SmAssistEnterItemMenu":
                case "SmAssistMenuFinalize":
                case "SmAssistMenuFinalizeHC":
                case "SmAssistMenuHC":
                case "SmAssistUpdateItemMenu":
                case "SmAssistModifyTrxMenu":
                case "SmAssistSelectDept":
                case "SmAuthorization":
                case "SmAuthorizationHC":
                case "SmConfirmOnScreenSignature":
                case "SmConfirmSignature":
                case "SmConfirmSuspendedTransaction":
                case "SmDataEntry":
                case "SmDataEntryNumericSmall":
                case "SmDataEntrySmall":
                case "SmDataEntryWithDetailsList":
                case "SmDataEntryWithImageControl":
                case "SmDataEntryWithListBG":
                case "SmDataEntryWithVideoControl":
                case "SmEnterBirthdate":
                case "SmLoadLift":
                case "SmmKeyInItemCode":
                case "SmmKeyInWtTol":
                case "SmSelectModeOfOperation":
                case "SmSystemFunctions":
                case "SmUpdateItem":
                case "SmUtility":
                case "SmVoidedItems":
                case "SmVoidAgeRestrictedItem":
                    SetCartReceiptItemSource(_viewModel.GetPropertyValue("StoreModeReceipt") as IEnumerable);
                    break;
                case "SmCouponItems":
                    SetCartReceiptItemSource(_viewModel.GetPropertyValue("StoreModeReceiptCoupon") as IEnumerable);
                    break;
                case "SmVisualItems":
                    SetCartReceiptItemSource(_viewModel.GetPropertyValue("StoreModeReceiptVisualItem") as IEnumerable);
                    break;
                case "SmRestrictedItems":
                case "SmVoidAndRemoveItem":
                    SetCartReceiptItemSource(_viewModel.GetPropertyValue("StoreModeReceiptRestricted") as IEnumerable);
                    break;
                case "AM_ConfirmAbort":
                case "AM_ConfirmVoid":
                case "AM_VoidItem":
                    SetCartReceiptItemSource(_viewModel.GetPropertyValue("StoreModeReceiptVoid") as IEnumerable);
                    break;
                case "SmSecurityLogs":
                    SetCartReceiptItemSource(_viewModel.GetPropertyValue("StoreModeReceiptDelaySecurityList") as IEnumerable);
                    break;
                case "ConfirmVoid":
                    _hasConfirmVoid = true;
                    SetCartReceiptSelectedIndex();
                    break;
                case "VoidApproval":
                    _hasVoidApproval = true;
                    SetCartReceiptSelectedIndex();
                    break;
                case "EnterId":
                    _hasEnterId = _hasConfirmVoid && _hasVoidApproval;
                    break;
				case "SmAssistMode": // SSCOI-46440
                    if (((bool)_viewModel.GetPropertyValue("TaxListShown")))
                    {
                        SetCartReceiptItemSource(_viewModel.GetPropertyValue("TaxList") as IEnumerable);
                    }
                    else if (((bool)_viewModel.GetPropertyValue("EASListShown")))   //CRD 212453
                    {
                        SetCartReceiptItemSource(_viewModel.GetPropertyValue("EASList") as IEnumerable);
                    }
                    else
                    {
                        SetCartReceiptItemSource(_viewModel.GetPropertyValue("StoreModeReceipt") as IEnumerable);
                    }
                    break;
                default:
                    if (!_viewModel.StoreMode)
                    {
                        if (_hasConfirmVoid && _hasVoidApproval && _hasEnterId)
                        {
                            SetCartReceiptSelectedIndex();
                        }
                        else
                        {
                            SetCartReceiptItemSource(_viewModel.GetPropertyValue("CustomerReceipt") as IEnumerable);
                            CartReceipt.AutoScrollIntoView = AutoScrollSelection.None;
                        }
                    }

                    break;
            }
        }

        /// <summary>
        /// Set cart receipt's selected index
        /// </summary>
        private void SetCartReceiptSelectedIndex()
        {
            CartReceipt.AutoScrollIntoView = AutoScrollSelection.None;
            var customerReceiptItemList = _viewModel.GetPropertyValue("CustomerReceipt") as IEnumerable;

            if (customerReceiptItemList != null)
            {
                if (_viewModel.StateParam.Equals("ConfirmVoid"))
                {
                    CartReceipt.SelectedIndex = GetCartReceiptIndex(customerReceiptItemList);
                }
                else
                {
                    if (!_viewModel.StateParam.Equals("VoidApproval"))
                    {
                        CartReceipt.SelectedIndex = GetCartReceiptIndex(customerReceiptItemList);
                        _hasConfirmVoid = false;
                        _hasVoidApproval = false;
                        _hasEnterId = false;
                    }
                }
            }
        }
	
		//+SSCOI-50040
        /// <summary>
        /// Method for highlighting the last voidable item
        /// </summary>
        private void SetStoreModeCartReceiptSelectedIndex()
        {
            if (CartReceipt.Items.Count > 0)
            {
                int cnt = CartReceipt.Items.Count;
                CustomerReceiptItem cartReceiptItem;
                while (--cnt >= 0)
                {
                    cartReceiptItem = CartReceipt.Items[cnt] as CustomerReceiptItem;
                    if (cartReceiptItem.Voidable && !cartReceiptItem.Strikeout)
                    {
                        break;
                    }
                }

                CartReceipt.SelectedIndex = cnt;
            }
        }
		//-SSCOI-50040
        /// <summary>
        /// Convert enumeration of receipt items to list.
        /// </summary>
        /// <param name="enumerableReceiptItemList">Receipt item enumeration</param>
        /// <returns>Resulting list</returns>
        private List<CustomerReceiptItem> ConvertReceiptItems(IEnumerable enumerableReceiptItemList)
        {
            List<CustomerReceiptItem> receiptItemList = new List<CustomerReceiptItem>();
            foreach (CustomerReceiptItem item in enumerableReceiptItemList)
            {
                receiptItemList.Add(item);
            }

            return receiptItemList;
        }

        /// <summary>
        /// Get cart receipt's selected item index
        /// </summary>
        /// <param name="enumerableReceiptItemList">Receipt item enumeration</param>
        /// <returns>Selected item's index</returns>
        private int GetCartReceiptIndex(IEnumerable enumerableReceiptItemList)
        {
            int returnVal = 0, counterIndex = 0;
            List<CustomerReceiptItem> receiptItemList = new List<CustomerReceiptItem>(ConvertReceiptItems(enumerableReceiptItemList));

            if (_viewModel.StateParam.Equals("ConfirmVoid") && CartListItem.LastDeletedItem != null)
            {
                CustomerReceiptItem itemDetail = new CustomerReceiptItem();
                foreach (var item in receiptItemList)
                {
                    if (item.Name.Equals(CartListItem.LastDeletedItem.Name) && item.Strikeout == false)
                    {
                        returnVal = counterIndex;
                        itemDetail = item;
                    }

                    counterIndex++;
                }

                CartListItem.LastDeletedItem = itemDetail;
            }
            else if (!_viewModel.StateParam.Equals("ConfirmVoid") && CartListItem.LastDeletedItem != null)
            {
                foreach (var item in receiptItemList)
                {
                    if (item.Data.Equals(CartListItem.LastDeletedItem.Data))
                    {
                        returnVal = counterIndex;
                        break;
                    }

                    counterIndex++;
                }
            }
            else
            {
                foreach (var item in receiptItemList)
                {
                    returnVal = counterIndex;
                    counterIndex++;
                }
            }

            return returnVal;
        }

        /// <summary>
        /// Handles store mode property changes.
        /// </summary>
        private void OnStoreModePropertyChanged()
        {
            if (_viewModel.StoreMode && !_viewModel.StateParam.Equals("AM_VoidItem"))
            {
                GoBackBtn.Visibility = Visibility.Collapsed;
                CancelAllBtn.Visibility = Visibility.Collapsed;
                CartControlExpand(false);
                RewardInfo.Visibility = Visibility.Collapsed;
                ShowPayButton(false);
            }

            UpdateCartReceiptItemsSource();
            OnTotalChanged();
        }

        /// <summary>
        /// Binds the cart receipt items.
        /// </summary>
        /// <param name="receiptItems">The list for binding</param>
        private void SetCartReceiptItemSource(IEnumerable receiptItems)
        {
            Binding receiptBinding = new Binding();
            receiptBinding.Source = receiptItems;
            BindingOperations.SetBinding(CartReceipt, TouchListBox.ItemsSourceProperty, receiptBinding);
        }

        /// <summary>
        /// Updates the command parameter for the edit cart button.
        /// </summary>
        private void UpdateEditCartCommandParameter()
        {
            switch (_viewModel.StateParam)
            {
                case "SmAssistMenu":
                case "SmVoidAgeRestrictedItem":
                    EditCartBtn.CommandParameter = "SMButton2";
                    break;
                default:
                    EditCartBtn.CommandParameter = "VoidItem";
                    break;
            }
        }

        /// <summary>
        /// Handles the selection changing for the electronic receipt.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void CartReceipt_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (CartReceipt.ItemsSource != null)
            {
                var itemsList = CartReceipt.ItemsSource as ObservableCollection<ReceiptItem>;
                if (itemsList != null && CartReceipt.SelectedItem != null)
                {
                    CartReceipt.SelectedIndex = itemsList.IndexOf(CartReceipt.SelectedItem as CustomerReceiptItem);
                }
            }

            if (_viewModel.StoreMode)
            {
                if (e.RemovedItems.Count != 0 && e.RemovedItems[0] != CartReceipt.SelectedValue && CartReceipt.Items.Count > 0)
                {
                    var item = CartReceipt.Items[CartReceipt.SelectedIndex < 0 ? 0 : CartReceipt.SelectedIndex] as CustomerReceiptItem;
                    if (item != null && !item.Voidable)
                    {
                        CartReceipt.SelectedValue = e.RemovedItems[0];
                    }

                    if (CartReceipt.SelectedIndex == CartReceipt.Items.Count - 1)
                    {
                        CartReceipt.AutoScrollIntoView = AutoScrollSelection.Bottom;
                    }
                }
                else if (e.RemovedItems.Count == 0 && !(CartReceipt.SelectedValue as CustomerReceiptItem).Voidable)
                {
                    CartReceipt.SelectedValue = null;
                }
				// SSCOI-46453: Move the code fix below so that everytime the cart receipt selection changed,
				// it will also send command to core to select corresponding item to POS.
				// +SSCOI-46521
				// SSCOI-46440: Check first if the CartReceipt.SelectedValue is an instance of CustomerReceiptItem before casting.
                //if (CartReceipt.SelectedValue is CustomerReceiptItem)
                if (_viewModel.BackgroundStateParam.Equals("SmAssistMode") && CartReceipt.SelectedValue is CustomerReceiptItem) //SSCOI-46717
                {
                    CmDataCapture.CaptureFormat(CmDataCapture.MaskExtensive, "{0}",_viewModel.StateParam);
                    string data = CartReceipt.SelectedValue != null ? ((CustomerReceiptItem)CartReceipt.SelectedValue).Data : "0";
                    _viewModel.ActionCommand.Execute(String.Format(CultureInfo.InvariantCulture, "SMSetSelectedItem({0})", data));
                }
				// -SSCOI-46521
            }
            // SSCOI-46758: Remove the else statement so that when an item was sold in store mode or CM mode, it will always scroll to last receipt and select the last receipt item
            //else
            //{
                if (CartReceipt.Items.Count > 0)
                {
                    if (CartReceipt.SelectedIndex == CartReceipt.Items.Count - 1)
                    {
                        CartReceipt.AutoScrollIntoView = AutoScrollSelection.Bottom;
                    }
                }
            //}
        }

        /// <summary>
        /// Updates the visibility of TotalRewards label and value.
        /// </summary>
        private void ShowHideTotalRewards()
        {
            TotalRewardsText.Visibility = TotalRewardsValue.Visibility = GetPropertyBoolValue("NextGenUICMTotalRewardPoints") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the visibility of AmountSaved label and value.
        /// </summary>
        private void ShowHideAmountSaved()
        {
            AmountSavedText.Visibility = AmountSavedValue.Visibility = GetPropertyBoolValue("NextGenUICMTotalSavings") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the visibility of RewardInfo panel.
        /// </summary>
        private void ShowHideRewardInfo()
        {
            RewardInfo.Visibility = GetPropertyBoolValue("NextGenUICMRewardInfo") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Gets the boolean value of a property.
        /// </summary>
        /// <param name="property">Property name being processed</param>
        /// <returns>True if property value not equal to 0</returns> 
        private bool GetPropertyBoolValue(string property)
        {
            string value = _viewModel.GetPropertyValue(property) as string;
            if (!string.IsNullOrEmpty(value))
            {
                return !value.Equals("0");
            }

            return false;
        }
		
		//Start CRD 150652
		private void HideTotals()
		{
			CMText.Visibility = Visibility.Collapsed;
            CMAmount.Visibility = Visibility.Collapsed;
            GMText.Visibility = Visibility.Collapsed;
            GMAmount.Visibility = Visibility.Collapsed;
            TMDText.Visibility = Visibility.Collapsed;
            TMDAmount.Visibility = Visibility.Collapsed;
            TaxText.Visibility = Visibility.Collapsed;
            TaxValue.Visibility = Visibility.Collapsed;
            TotalText.Visibility = Visibility.Collapsed;
            TotalAmountValue.Visibility = Visibility.Collapsed;
		}
		
		private void ShowTotals()
		{
			CMText.Visibility = Visibility.Visible;
            CMAmount.Visibility = Visibility.Visible;
            GMText.Visibility = Visibility.Visible;
            GMAmount.Visibility = Visibility.Visible;
            TMDText.Visibility = Visibility.Visible;
            TMDAmount.Visibility = Visibility.Visible;
            TaxText.Visibility = Visibility.Visible;
            TaxValue.Visibility = Visibility.Visible;
            TotalText.Visibility = Visibility.Visible;
            TotalAmountValue.Visibility = Visibility.Visible;
		}
		//End CRD 150652
    }
}
