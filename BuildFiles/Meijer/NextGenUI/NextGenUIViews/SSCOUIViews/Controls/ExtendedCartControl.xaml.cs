// <copyright file="ExtendedCartControl.xaml.cs" company="NCR Corporation">
//     Copyright (c) NCR Corporation. All rights reserved.
// </copyright>
namespace SSCOUIViews.Controls
{
    using System;
    using System.Collections.Specialized;
    using System.ComponentModel;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Input;
    using SSCOUIModels;
    using SSCOUIModels.Models;
    using SSCOUIViews.Properties;
    using SSCOControls;
    using SSCOUIModels.Helpers;

    /// <summary>
    /// Interaction logic for ExtendedCartControl.xaml
    /// </summary>
    public partial class ExtendedCartControl : Grid
    {
        /// <summary>
        /// Dependency property for the National Measurement Infrastructure standard compliance.
        /// </summary>
        public static readonly DependencyProperty NMIProperty =
            DependencyProperty.Register(
            "NMI",
            typeof(bool),
            typeof(ExtendedCartControl),
            new FrameworkPropertyMetadata(OnNMIChanged));

        /// <summary>
        /// The main view model.
        /// </summary>
        private IMainViewModel _viewModel;

        /// <summary>
        /// The last item on the receipt. This is the one that appears selected.
        /// </summary>
        private CustomerReceiptItem _cartReceiptItem;

        /// <summary>
        /// Initializes a new instance of the ExtendedCartControl class.
        /// </summary>
        public ExtendedCartControl()
        {
            InitializeComponent();

            this.NMI = Settings.Default.NMI;

            this.Loaded += ExtendedCartControl_Loaded;
            this.IsVisibleChanged += ExtendedCartControl_IsVisibleChanged;
            this.PreviewTouchDown += ExtendedCartControl_PreviewTouchDown;
            this.PreviewMouseDown += ExtendedCartControl_PreviewMouseDown;
        }

        /// <summary>
        /// Gets or sets a value indicating whether or not National Measurement Infrastructure standards must be met.
        /// This dictates minimum text sizes on the screen.
        /// </summary>
        public bool NMI
        {
            get { return (bool)GetValue(NMIProperty); }
            set { SetValue(NMIProperty, value); }
        }

        /// <summary>
        /// Handler for when the NMI property changes
        /// </summary>
        /// <param name="d">dependency object</param>
        /// <param name="e">event arguments</param>
        private static void OnNMIChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            UpdateCartControlStyle(d);
        }

        /// <summary>
        /// Static method to updates the styles on the cart control.
        /// This is what is called in the dependency property changed event handlers.
        /// </summary>
        /// <param name="d">dependency object</param>
        private static void UpdateCartControlStyle(DependencyObject d)
        {
            var extendedCartContol = d as ExtendedCartControl;

            if (extendedCartContol != null)
            {
                extendedCartContol.UpdateStyle();
            }
        }

        /// <summary>
        /// Updates the styles on the various controls. The styles are different when NMI is true.
        /// </summary>
        private void UpdateStyle()
        {
            if (NMI)
            {
                this.ExtendedTaxText.Style = this.FindResource("NMITaxTextStyle") as Style;
                this.ExtendedTaxValue.Style = this.FindResource("NMITaxValueTextStyle") as Style;
                this.ExtendedTotalText.Style = this.FindResource("NMITotalTextStyle") as Style;
                this.ExtendedTotalAmountValue.Style = this.FindResource("NMITotalValueTextStyle") as Style;
                this.ExtendedDueAmountText.Style = this.FindResource("NMITotalTextCustomerModeStyle") as Style;
                this.ExtendedDueAmountValue.Style = this.FindResource("NMITotalAmountCustomerModeStyle") as Style;
            }
            else
            {
                this.ExtendedTaxText.Style = this.FindResource("taxTextStyle") as Style;
                this.ExtendedTaxValue.Style = this.FindResource("taxValueTextStyle") as Style;
                this.ExtendedTotalText.Style = this.FindResource("totalTextStyle") as Style;
                this.ExtendedTotalAmountValue.Style = this.FindResource("totalValueTextStyle") as Style;
                this.ExtendedDueAmountText.Style = this.FindResource("totalTextCustomerModeStyle") as Style;
                this.ExtendedDueAmountValue.Style = this.FindResource("totalAmountCustomerModeStyle") as Style;
				//SSCOI-47250
                this.ExtendedCMText.Style = this.FindResource("CMTotalCMDiscountText") as Style;
                this.ExtendedCMAmount.Style = this.FindResource("CMTotalCMDiscountAmount") as Style;
                this.ExtendedGMText.Style = this.FindResource("CMTotalGMText") as Style;
                this.ExtendedGMAmount.Style = this.FindResource("CMTotalGMAmount") as Style;
                this.ExtendedTMDText.Style = this.FindResource("CMTotalTMDiscountText") as Style;
                this.ExtendedTMDAmount.Style = this.FindResource("CMTotalTMDiscountAmount") as Style;
				//SSCOI-47250
            }
        }

        /// <summary>
        /// Event handler for when the extended cart control is loaded.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void ExtendedCartControl_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
                // Since cart control's IsSynchronizedWithCurrentItem is set to False 
                // we want trap collection change event for selection update.
                ((INotifyCollectionChanged)CartReceipt.Items).CollectionChanged += ExtendedCartControl_CollectionChanged;
            }
            catch
            {
            }
        }

        /// <summary>
        /// Preview mouse down event handler.
        /// This is used to disable mouse events on this control
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void ExtendedCartControl_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            e.Handled = true;
        }

        /// <summary>
        /// Preview touch down event handler.
        /// This is used to disable touch events on this control.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void ExtendedCartControl_PreviewTouchDown(object sender, TouchEventArgs e)
        {
            e.Handled = true;
        }

        /// <summary>
        /// Event handler for when the IsVisible property changes.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void ExtendedCartControl_IsVisibleChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            ProcessSelection(); // Process selection if visibility has changed (when this control become visible)

            // This fixes an odd case of an item indented when exiting from store mode.
            if ((bool)e.NewValue)
            {
                CartReceipt.Items.Refresh();
            }
        }

        /// <summary>
        /// Event handler for when the list of receipt items changes.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void ExtendedCartControl_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            ProcessSelection(); // When item collection changes, update the selection.

            // On POSReady2009 the selected item was not unselecting on deletes. This fixes it.
            if (e.Action == NotifyCollectionChangedAction.Replace)
            {
                CartReceipt.Items.Refresh();
            }
        }

        /// <summary>
        /// Selects the last selectable item in the cart.
        /// </summary>
        private void ProcessSelection()
        {
            // Process selection only when control is visible & there are items in the cart.
            if (System.Windows.Visibility.Visible == this.Visibility && CartReceipt.Items.Count > 0)
            {
                int cnt = CartReceipt.Items.Count;
                while (--cnt >= 0)
                {
                    _cartReceiptItem = CartReceipt.Items[cnt] as CustomerReceiptItem;
                    if (_cartReceiptItem.Voidable && !_cartReceiptItem.Strikeout)
                    {
                        break;
                    }
                }

                CartReceipt.SelectedIndex = cnt;
            }
        }

        /// <summary>
        /// Event handler for when the binding context changes.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void Grid_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            _viewModel = DataContext as IMainViewModel;
            _viewModel.PropertyChanged += new PropertyChangedEventHandler(ViewModel_PropertyChanged);
            OnAmountDueChanged();
            OnTotalChanged();
            OnTaxChanged();
			//SSCOI-47250
            OnCMTotalCMDiscountChanged();
            OnCMTotalGMChanged();
            OnCMTotalCMDiscountChanged();
			//SSCOI-47250
            ShowHideTax();
            UpdateWeightDetailStyle();
        }

        /// <summary>
        /// Event handler for when a property changes in the view model.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void ViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch (e.PropertyName)
            {
                case "Total":
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
                case "CMTotalCMDiscountAmount": //SSCOI-47250
					OnCMTotalCMDiscountChanged();
                    break;
                case "CMTotalGMAmount": //SSCOI-47250
					OnCMTotalGMChanged();
                    break;
                case "CMTotalTMDiscountAmount": //SSCOI-47250
					OnCMTotalTMDiscountChanged();
                    break;
            }
        }

        public void UpdateWeightDetailStyle()
        {
            WeightDetailsPanel.WeightTextBlock.Style = FindResource("extendedWeightTextStyle") as Style;
            WeightDetailsPanel.WeightInfoTextBlock.Style = FindResource("measuresImageLabelCustomerModeStyle") as Style;
            WeightDetailsPanel.ScaleLogo.Style = FindResource("scaleLogoCustomerModeStyle") as Style;
        }

        /// <summary>
        /// ShowHideTax method
        /// </summary>
        private void ShowHideTax()
        {
            ExtendedTaxText.Visibility = ExtendedTaxValue.Visibility = GetPropertyBoolValue("NextGenUIIsTaxLine") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the total amount text block.
        /// </summary>
        private void OnTotalChanged()
        {
            object val = _viewModel.GetPropertyValue("Total");
            if (val != null)
            {
                ExtendedTotalAmountValue.Value = val.ToString();
            }
        }

        /// <summary>
        /// Updates the due amount text block.
        /// </summary>
        private void OnAmountDueChanged()
        {
            object due = _viewModel.GetPropertyValue("NextGenUIAmountDue");
            if (due != null)
            {
                ExtendedDueAmountValue.Value = due.ToString();
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
                this.ExtendedTaxValue.Value = val.ToString();
            }
        }
		
		//SSCOI-47250
        private void OnCMTotalCMDiscountChanged()
        {
            object val = this._viewModel.GetPropertyValue("CMTotalCMDiscountAmount");
            if (null != val)
            {
                this.ExtendedCMAmount.Value = val.ToString();
            }

        }

        private void OnCMTotalGMChanged()
        {
            object val = this._viewModel.GetPropertyValue("CMTotalGMAmount");
            if (null != val)
            {
                this.ExtendedGMAmount.Value = val.ToString();
            }

        }

        private void OnCMTotalTMDiscountChanged()
        {
            object val = this._viewModel.GetPropertyValue("CMTotalTMDiscountAmount");
            if (null != val)
            {
                this.ExtendedTMDAmount.Value = val.ToString();
            }

        }
		//SSCOI-47250
		
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
    }
}
