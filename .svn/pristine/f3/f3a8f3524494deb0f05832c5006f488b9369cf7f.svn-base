// <copyright file="CartListItem.xaml.cs" company="NCR Corporation">
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
    using System.Windows.Data;
    using System.Windows.Input;
    using FPsxWPF.Controls;
    using SSCOControls;
    using SSCOControls.Helpers.KeyboardSimulatorComponents;
    using SSCOControls.Interfaces;
    using SSCOUIModels;
    using SSCOUIModels.Models;
    using System.Windows.Media;
    
    /// <summary>
    /// Interaction logic for CartListItem.xaml
    /// </summary>
    public partial class CartListItem : Grid, INavigable
    {
        /// <summary>
        /// Dependency property for ShowInterventionSection.
        /// </summary>
        public static readonly DependencyProperty ShowInterventionSectionProperty = DependencyProperty.Register(
            "ShowInterventionSection",
            typeof(bool),
            typeof(CartListItem),
            new FrameworkPropertyMetadata(OnShowInterventionSectionChanged));

        /// <summary>
        /// Dependency property for ShowRemoveSection.
        /// </summary>
        public static readonly DependencyProperty ShowRemoveSectionProperty = DependencyProperty.Register("ShowRemoveSection", typeof(bool), typeof(CartListItem));

        /// <summary>
        /// Dependency property for NMI.
        /// </summary>
        public static readonly DependencyProperty NMIProperty = DependencyProperty.Register(
            "NMI",
            typeof(bool),
            typeof(CartListItem),
            new FrameworkPropertyMetadata(OnNMIChanged));

        /// <summary>
        /// Dependency property for NMIPopup.
        /// </summary>
        public static readonly DependencyProperty NMIPopupProperty = DependencyProperty.Register(
            "NMIPopup",
            typeof(bool),
            typeof(CartListItem),
            new FrameworkPropertyMetadata(OnNMIPopupChanged));

        /// <summary>
        /// Dependency property for show line number property
        /// </summary>
        public static readonly DependencyProperty ShowLineNumberProperty =
            DependencyProperty.Register("ShowLineNumber", typeof(bool), typeof(CartListItem), new PropertyMetadata(true, OnShowLineNumberChanged));

        /// <summary>
        /// Last deleted item.
        /// </summary>
        private static CustomerReceiptItem _lastDeletedItem;

        /// <summary>
        /// View model.
        /// </summary>
        private IMainViewModel _viewModel;

        /// <summary>
        /// Current item.
        /// </summary>
        private CustomerReceiptItem _currentItem;

        /// <summary>
        /// Initializes a new instance of the <see cref="CartListItem"/> class.
        /// </summary>
        public CartListItem()
        {
            InitializeComponent();

            if (Application.Current.MainWindow != null)
            {
                _viewModel = (IMainViewModel)Application.Current.MainWindow.DataContext;
            }

            PreviewKeyDown += Control_PreviewKeyDown;
            LostFocus += Control_LostFocus;
            GotFocus += Control_GotFocus;
        }

        /// <summary>
        /// Gets or sets the last deleted item.
        /// </summary>
        public static CustomerReceiptItem LastDeletedItem
        {
            get
            {
                return _lastDeletedItem;
            }

            set
            {
                _lastDeletedItem = value;
            }
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
        /// Gets or sets a value indicating whether or not to show intervention section.
        /// </summary>
        public bool ShowInterventionSection
        {
            get
            {
                return Convert.ToBoolean(GetValue(ShowInterventionSectionProperty));
            }

            set
            {
                SetValue(ShowInterventionSectionProperty, value);
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether to show remove section.
        /// </summary>
        public bool ShowRemoveSection
        {
            get
            {
                return Convert.ToBoolean(GetValue(ShowRemoveSectionProperty));
            }

            set
            {
                SetValue(ShowRemoveSectionProperty, value);
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether National Measurements Institute compliance is enforced.
        /// </summary>
        public bool NMI
        {
            get
            {
                return Convert.ToBoolean(GetValue(NMIProperty));
            }

            set
            {
                SetValue(NMIProperty, value);
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether to display NMI popup.
        /// </summary>
        public bool NMIPopup
        {
            get
            {
                return Convert.ToBoolean(GetValue(NMIPopupProperty));
            }

            set
            {
                SetValue(NMIPopupProperty, value);
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether or not to show the line number
        /// </summary>
        public bool ShowLineNumber
        {
            get { return (bool)GetValue(ShowLineNumberProperty); }
            set { SetValue(ShowLineNumberProperty, value); }
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
        /// Event handler for ShowInterventionChanged event.
        /// </summary>
        /// <param name="d">Dependency object</param>
        /// <param name="e">Event arguments</param>
        private static void OnShowInterventionSectionChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            UpdateCartListItemStyle(d);
        }

        /// <summary>
        /// Update CartListItem's style.
        /// </summary>
        /// <param name="d">Dependency object</param>
        private static void UpdateCartListItemStyle(DependencyObject d)
        {
            var cartListItem = d as CartListItem;

            if (cartListItem != null)
            {
                cartListItem.UpdateCartListStyle();
            }
        }

        /// <summary>
        /// Handle NMIChanged event.
        /// </summary>
        /// <param name="d">Dependency object</param>
        /// <param name="e">Event arguments</param>
        private static void OnNMIChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            UpdateCartListItemStyle(d);
        }

        /// <summary>
        /// Handle NMIPopup's Changed event.
        /// </summary>
        /// <param name="d">Dependency object</param>
        /// <param name="e">Event arguments</param>
        private static void OnNMIPopupChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            UpdateCartListItemStyle(d);
        }

        /// <summary>
        /// Event handler for show line number property
        /// </summary>
        /// <param name="d">dependency object</param>
        /// <param name="e">event arguments</param>
        private static void OnShowLineNumberChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var cartListItem = d as CartListItem;

            if (cartListItem == null)
            {
                return;
            }

            bool isVisible = (bool)e.NewValue;

            cartListItem.ItemLineNumber.Visibility = isVisible ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Event handler for "DataContextChanged" event.
        /// </summary>
        /// <param name="sender">Sending object</param>
        /// <param name="e">Event arguments</param>
        private void Grid_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            _currentItem = DataContext as CustomerReceiptItem;

            // start CRD 125377
            if (_viewModel.GetPropertyValue("TransactionMode").ToString().Equals("1"))
            {
                SNDCheckIcon.Visibility = Visibility.Visible;
            }
            else if (_currentItem != null && IsmPerksCardNumber(_currentItem.Description) && !IsmPerksOffline(_currentItem))
            {
                SNDCheckIcon.Visibility = Visibility.Collapsed;
                //Set the foreground to Red
                ItemDescription.Foreground = new SolidColorBrush(_currentItem.TextColor);
            }
            // end CRD 125377
        }

        /// <summary>
        /// Update cart list's style.
        /// </summary>
        private void UpdateCartListStyle()
        {
            if (NMI)
            {
                UpdateStyle(typeof(StackPanel), StackPanel.MarginProperty, new Thickness(8), CartListItemStackPanel);
                if (ShowInterventionSection)
                {
                    Resources["cartListBoxItemTextStyle"] = FindResource("NMIListItemDescTextStyle") as Style;
                    Resources["cartListItemPriceTextStyle"] = FindResource("NMIListItemPriceTextStyle") as Style;
                    Resources["cartListBoxSubItemTextStyle"] = FindResource("NMICartListBoxSubItemTextStyle") as Style;
                    Resources["cartListBoxSubItemDescTextStyle"] = FindResource("NMICartListBoxSubItemDescTextStyle") as Style;
                    Resources["cartListBoxSubItemPriceTextStyle"] = FindResource("NMICartListBoxSubItemPriceTextStyle") as Style;
                    Resources["CartDelayedInterventionStyle"] = FindResource("NMICartDelayedInterventionStyle") as Style;
                    Resources["cartListBoxLineNumberTextStyle"] = FindResource("NMICartListBoxLineNumberTextStyle") as Style;
                }
            }
            else if (ShowInterventionSection)
            {
                Resources["cartListBoxItemTextStyle"] = !_viewModel.StoreMode ? FindResource("cartControlListItemDescTextStyle") as Style :
                    FindResource("SMCartListBoxItemTextStyle") as Style;
                Resources["cartListItemPriceTextStyle"] = !_viewModel.StoreMode ? FindResource("cartListItemPriceTextStyle") as Style :
                    FindResource("SMCartListItemPriceTextStyle") as Style;
                Resources["cartListBoxSubItemTextStyle"] = !_viewModel.StoreMode ? FindResource("cartListBoxSubItemTextStyle") as Style :
                    FindResource("SMCartListBoxSubItemTextStyle") as Style;
                Resources["cartListBoxSubItemDescTextStyle"] = !_viewModel.StoreMode ? FindResource("cartListBoxSubItemDescTextStyle") as Style :
                    FindResource("SMCartListBoxSubItemDescTextStyle") as Style;
                Resources["cartListBoxSubItemPriceTextStyle"] = !_viewModel.StoreMode ? FindResource("cartListBoxSubItemPriceTextStyle") as Style :
                    FindResource("SMCartListBoxSubItemPriceTextStyle") as Style;
            }

            if (NMIPopup)
            {
                UpdateStyle(typeof(DockPanel), DockPanel.MarginProperty, new Thickness(12, 10, 6, 10), ItemDockPanel);
                UpdateStyle(typeof(ItemsControl), ItemsControl.MarginProperty, new Thickness(12, 0, 6, 0), subItemsControl);
                Resources["cartListBoxItemTextStyle"] = FindResource("NMIPopupListItemDescTextStyle") as Style;
                Resources["cartListItemPriceTextStyle"] = FindResource("NMIPopupListItemPriceTextStyle") as Style;
                Resources["cartListBoxSubItemTextStyle"] = FindResource("NMIPopupListBoxSubItemTextStyle") as Style;
                Resources["cartListBoxSubItemDescTextStyle"] = FindResource("NMIPopupCartListBoxSubItemDescTextStyle") as Style;
                Resources["cartListBoxSubItemPriceTextStyle"] = FindResource("NMIPopupCartListBoxSubItemPriceTextStyle") as Style;
            }
        }

        /// <summary>
        /// Update style.
        /// </summary>
        /// <param name="targetType">Target type</param>
        /// <param name="property">Property to set</param>
        /// <param name="value">Value to set property to</param>
        /// <param name="control">Control whose style is to be updated</param>
        private void UpdateStyle(Type targetType, DependencyProperty property, object value, UIElement control)
        {
            Style style = new Style { TargetType = targetType };
            style.Setters.Add(new Setter(property, value));
            ((FrameworkElement)control).Style = style;
        }

        /// <summary>
        /// Handle RemoveButton's TouchDown event.
        /// </summary>
        /// <param name="sender">Sending object</param>
        /// <param name="e">Event arguments</param>
        private void RemoveButton_TouchDown(object sender, System.Windows.Input.TouchEventArgs e)
        {
            e.Handled = true;
        }

        /// <summary>
        /// Handle RemoveButton's Click event
        /// </summary>
        /// <param name="sender">Sending object</param>
        /// <param name="e">Event arguments</param>
        private void RemoveButton_Click(object sender, RoutedEventArgs e)
        {
            if (_currentItem != null)
            {
                _lastDeletedItem = _currentItem;
                string cancelItem = _viewModel.StoreMode ? "SMCancelItem({0})" : "CancelItem({0})";
                _viewModel.ActionCommand.Execute(string.Format(CultureInfo.InvariantCulture, cancelItem, _currentItem.Data));
            }
        }

        /// <summary>
        /// Event handler for when the price field text is cut off (clipped).
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void ItemPrice_ClippingChanged(object sender, EventArgs e)
        {
            if (sender is MeasureTextBlock)
            {
                MeasureTextBlock mtb = (MeasureTextBlock)sender;

                if (mtb.NMI && mtb.IsClipped)
                {
                    ItemDescription.Width = 200;
                    ItemPrice.Width = 235;
                }
            }
        }
  
        private bool IsmPerksCardNumber(string data)
        {
            return string.IsNullOrEmpty(data) ? false : data.StartsWith(@"mPerks #", StringComparison.OrdinalIgnoreCase);
        }

        private bool IsmPerksOffline(CustomerReceiptItem receiptItem)
        {
            //text color will be Black when mPerks is offline, otherwise Red
            return (receiptItem.TextColor.R == 0 && receiptItem.TextColor.G == 0 && receiptItem.TextColor.B == 0);
        }
		
    }
}
