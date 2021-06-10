// <copyright file="Card.xaml.cs" company="NCR">
//     Copyright (c) NCR. All rights reserved.
// </copyright>
namespace SSCOUIViews.Controls
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.ComponentModel;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Media;
    using FPsxWPF.Controls;
    using RPSWNET;
    using SSCOControls;
    using SSCOUIModels;
    using SSCOUIModels.Models;

    /// <summary>
    /// Interaction logic for Card.xaml
    /// </summary>
    public partial class Card : Grid
    {
        /// <summary>
        /// Dependency property for NMI
        /// </summary>
        public static readonly DependencyProperty NMIProperty =
            DependencyProperty.Register("NMI", typeof(bool), typeof(Card), new PropertyMetadata(false));

        /// <summary>
        /// Dependency property for NMINumberScale
        /// </summary>
        public static readonly DependencyProperty NMINumberScaleProperty =
            DependencyProperty.Register("NMINumberScale", typeof(double), typeof(Card), new PropertyMetadata(1.0));

        /// <summary>
        /// Dependency property for IsSelectedOnFilmstrip
        /// </summary>
        public static readonly DependencyProperty IsSelectedOnFilmstripProperty =
            DependencyProperty.Register("IsSelectedOnFilmstrip", typeof(bool), typeof(Card), new PropertyMetadata(false));

        /// <summary>
        /// The number of device independent units to expand the text area when the text is clipped.
        /// </summary>
        private const double TEXT_AREA_EXPANSION = 200;

        /// <summary>
        /// The number of device independent units to expand the text area when the text is clipped and quick pick is enabled.
        /// </summary>
        private const double TEXT_AREA_EXPANSION_QUICK_PICK = 100;

        /// <summary>
        /// Stores whether or not extended data capture (logging) is enabled. This as a field makes unit testing easier.
        /// </summary>
        private static bool? _isExtendedCaptureActive = null;

        /// <summary>
        /// The selected item on the eReceipt.
        /// </summary>
        private CustomerReceiptItem _currentItem;
        
        /// <summary>
        /// The View Model
        /// </summary>
        private IMainViewModel _viewModel;

        /// <summary>
        /// Initializes a new instance of the Card class.
        /// </summary>
        public Card()
        {
            InitializeComponent();

            if (_isExtendedCaptureActive == null)
            {
                _isExtendedCaptureActive = CmDataCapture.IsCaptureActive(CmDataCapture.MaskExtensive);
            }

            this.NMI = Properties.Settings.Default.NMI;

            if (Application.Current.MainWindow != null)
            {
                _viewModel = (IMainViewModel)Application.Current.MainWindow.DataContext;
            }          

            ShowPanelConsideringQuickPick();
        }

        /// <summary>
        /// Gets or sets a value indicating whether the National Measurement Infrastructure compliance is turned on.
        /// </summary>
        public bool NMI
        {
            get { return (bool)GetValue(NMIProperty); }
            set { SetValue(NMIProperty, value); }
        }

        /// <summary>
        /// Gets or sets a value indicating whether a scaling factor should be applied to numbers to be NMI compliant.
        /// This is needed because the filmstrip card is 3D, and 'shrinks' the numbers with its camera perspective.
        /// </summary>
        public double NMINumberScale
        {
            get { return (double)GetValue(NMINumberScaleProperty); }
            set { SetValue(NMINumberScaleProperty, value); }
        }

        /// <summary>
        /// Gets or sets a value indicating whether or not the card is the selected item on the filmstrip.
        /// </summary>
        public bool IsSelectedOnFilmstrip
        {
            get { return (bool)GetValue(IsSelectedOnFilmstripProperty); }
            set { SetValue(IsSelectedOnFilmstripProperty, value); }
        }

        /// <summary>
        /// Gets a value indicating whether or not quick pick items are being shown.
        /// </summary>
        private bool IsQuickPickShowing
        {
            get
            {
				//+127021 meijerGo only mode
				//if (GetPropertyStringValue("TransactionMode").Equals("2") || GetPropertyStringValue("TransactionMode").Equals("3"))	 
				if(_viewModel.GetPropertyValue("TransactionMode").ToString().Equals("2") || _viewModel.GetPropertyValue("TransactionMode").ToString().Equals("3"))
				{
					return false;
				}
				//-127021
                return true;
            }
        }

        /// <summary>
        /// Event handler for when the binding context changes.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void Card_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            _currentItem = this.DataContext as CustomerReceiptItem;
            if (_currentItem != null)
            {
                if (IsmPerksCardNumber(_currentItem.Description) && !IsmPerksOffline(_currentItem))
                {
                    ItemDescription.Foreground = new SolidColorBrush(_currentItem.TextColor);
                }
                else
                {
                    ItemDescription.Foreground = _currentItem.Voided ? (SolidColorBrush)this.FindResource("voidedItemForegroundColorBrush") : (SolidColorBrush)this.FindResource("mainTextColorBrush");
                }
            }
            ShowPanelConsideringQuickPick();
            ShowElements();
            ResetCardImage();
        }

        /// <summary>
        /// Event handler for when the card is loaded.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void Card_Loaded(object sender, RoutedEventArgs e)
        {
            _viewModel.PropertyChanged += ViewModel_PropertyChanged;
            ShowCardMessage();
            ShowElements();
        }

        /// <summary>
        /// Event handler for when the card is unloaded.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void Card_Unloaded(object sender, RoutedEventArgs e)
        {
            _viewModel.PropertyChanged -= ViewModel_PropertyChanged;
        }

        /// <summary>
        /// Sets the visibility of the card message.
        /// Often, the card message shows information about coupons.
        /// </summary>
        private void ShowCardMessage()
        {
            this.CardMessage.Visibility = this.QuickPickCardMessage.Visibility = !_viewModel.StateParam.Equals("EnterCoupons") && !_viewModel.StateParam.StartsWith("VoidItem") &&
                (bool)_viewModel.GetPropertyValue("ScanBagTextShown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Sets the visibility of the various visual elements on the card panel.
        /// </summary>
        private void ShowElements()
        {
            if (_currentItem != null)
            {
                this.ApprovedPanel.Visibility = this.QuickPickApprovedPanel.Visibility = !_viewModel.BackgroundStateParam.Equals("Bag") &&
                    _currentItem.IsApproved ? Visibility.Visible : Visibility.Collapsed;
                this.AssistanceAnimation.Visibility = this.QuickPickAssistanceAnimation.Visibility = !_viewModel.BackgroundStateParam.Equals("Bag") &&
                    _currentItem.IsIntervention ? Visibility.Visible : Visibility.Collapsed;
            }

            this.CardMessage.IsSmaller = this.QuickPickCardMessage.IsSmaller = Visibility.Visible == this.ApprovedPanel.Visibility
                || Visibility.Visible == this.AssistanceAnimation.Visibility;
        }

        /// <summary>
        /// Changes the card panel visibility based on whether or not quick pick is enabled.
        /// </summary>
        private void ShowPanelConsideringQuickPick()
        {
            bool isQuickPick = IsQuickPickShowing;
            if ((isQuickPick && (FullPanel.Visibility == Visibility.Visible)) ||
                (!isQuickPick && (QuickPickPanel.Visibility == Visibility.Visible)))
            {
                this.FullPanel.Visibility = isQuickPick ? Visibility.Collapsed : Visibility.Visible;
                this.QuickPickPanel.Visibility = isQuickPick ? Visibility.Visible : Visibility.Collapsed;
            }
        }

        /// <summary>
        /// Handler for when a property changes on the view model.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void ViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch (e.PropertyName)
            {
                case "BackgroundStateParam":
                    ShowElements();
                    break;
                case "StateParam":
                case "ScanBagTextShown":
                    ShowCardMessage();
                    break;
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

        /// <summary>
        /// Event handler for when the text is clipped or not on the extra lines text.
        /// The extra lines typically shows the price per weight or quantity.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void ExtraLine_ClippingChanged(object sender, EventArgs e)
        {
            AdjustForClipping(sender);
        }

        /// <summary>
        /// Hides the image if NMI is on and a measure text block is clipped.
        /// </summary>
        /// <param name="sender">object firing the Clipping Changed event</param>
        private void AdjustForClipping(object sender)
        {
            if (sender is MeasureTextBlock)
            {
                MeasureTextBlock measureTextBlock = (MeasureTextBlock)sender;

                if (!measureTextBlock.NMI)
                {
                    return;
                }

                if (measureTextBlock.IsClipped)
                {
                    measureTextBlock.TextWrapping = TextWrapping.Wrap;
                    HideCardImage();
                }
            }
        }

        /// <summary>
        /// Hides the card image and moves the animation upwards.
        /// This gives more room for the extra lines text which is usually the price per weight.
        /// </summary>
        private void HideCardImage()
        {
            if ((bool)_isExtendedCaptureActive)
            {
                if (_currentItem == null)
                {
                    CmDataCapture.CaptureFormat(
                       CmDataCapture.MaskExtensive, "Hiding card image for null item.");
                }
                else
                {
                    CmDataCapture.CaptureFormat(
                       CmDataCapture.MaskExtensive, "Hiding card image for receipt item {0}", _currentItem.Description);
                }
            }

            GridLength gridRow0Height = (GridLength)Resources["mainGridRow0Height"];

            if (IsQuickPickShowing)
            {
                MainGrid.RowDefinitions[0].Height = new GridLength(gridRow0Height.Value + TEXT_AREA_EXPANSION_QUICK_PICK);
                QuickPickAnimationGrid.RenderTransform = new TranslateTransform(0, 0 - TEXT_AREA_EXPANSION_QUICK_PICK);
            }
            else
            {
                MainGrid.RowDefinitions[0].Height = new GridLength(gridRow0Height.Value + TEXT_AREA_EXPANSION);
                AnimationGrid.RenderTransform = new TranslateTransform(0, 0 - TEXT_AREA_EXPANSION);
            }

            ImageHolder.Visibility = Visibility.Collapsed;
            QuickPickImageHolder.Visibility = Visibility.Collapsed;
        }

        /// <summary>
        /// Resets the visibility of the card image and animation positions.
        /// </summary>
        private void ResetCardImage()
        {
            if ((bool)_isExtendedCaptureActive)
            {
                if (_currentItem == null)
                {
                    CmDataCapture.CaptureFormat(
                       CmDataCapture.MaskExtensive, "Resetting card image for null item");
                }
                else
                {
                    CmDataCapture.CaptureFormat(
                       CmDataCapture.MaskExtensive, "Resetting card image for receipt item {0}", _currentItem.Description);
                }
            }

            WrapMeasureTextBlock(ExtraLinesControl, "ExtraLine", false);
            WrapMeasureTextBlock(SubItemsControl, "SubItemsDescription", false);

            MainGrid.RowDefinitions[0].Height = (GridLength)Resources["mainGridRow0Height"];
            ImageHolder.Visibility = Visibility.Visible;
            QuickPickImageHolder.Visibility = Visibility.Visible;

            AnimationGrid.RenderTransform = null;
            QuickPickAnimationGrid.RenderTransform = null;
        }

        /// <summary>
        /// Sets the measure text block text wrapping when it is in a data template.
        /// </summary>
        /// <param name="itemsControl">The items control with the data template</param>
        /// <param name="controlName">The name of the measure text block</param>
        /// <param name="wrapText">true for wrap, false for no wrap</param>
        private void WrapMeasureTextBlock(ItemsControl itemsControl, string controlName, bool wrapText)
        {
            foreach (var item in itemsControl.Items)
            {
                DependencyObject dependencyObject = itemsControl.ItemContainerGenerator.ContainerFromItem(item);

                if (dependencyObject is ContentPresenter)
                {
                    ContentPresenter contentPresenter = (ContentPresenter)dependencyObject;

                    // this is needed to fix a potential crash if the data template has not been applied yet
                    contentPresenter.ApplyTemplate();

                    object textBlockControl = contentPresenter.ContentTemplate.FindName(controlName, contentPresenter);

                    if (textBlockControl is MeasureTextBlock)
                    {
                        ((MeasureTextBlock)textBlockControl).TextWrapping = wrapText ? TextWrapping.Wrap : TextWrapping.NoWrap;
                    }
                }
            }
        }

        /// <summary>
        /// Event handler for when the sub-items description is clipped.
        /// Linked items usually appear in sub-items.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void SubItemsDescription_ClippingChanged(object sender, EventArgs e)
        {
            AdjustForClipping(sender);
        }
    }
}
