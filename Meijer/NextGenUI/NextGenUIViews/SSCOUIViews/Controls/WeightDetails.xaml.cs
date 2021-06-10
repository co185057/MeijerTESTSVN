// <copyright file="WeightDetails.xaml.cs" company="NCR Corporation">
//     Copyright (c) NCR Corporation. All rights reserved.
// </copyright>
namespace SSCOUIViews.Controls
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Globalization;
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
    using SSCOUIModels;

    /// <summary>
    /// Interaction logic for WeightDetails.xaml
    /// </summary>
    public partial class WeightDetails : Grid
    {
        /// <summary>
        /// Dependency property for the National Measurement Infrastructure compliance.
        /// </summary>
        public static readonly DependencyProperty NMIProperty =
            DependencyProperty.Register("NMI", typeof(bool), typeof(WeightDetails), new PropertyMetadata(false));

        /// <summary>
        /// Dependency property for the horizontal alignment of the scale information.
        /// </summary>
        public static readonly DependencyProperty ScaleInfoAlignmentProperty =
            DependencyProperty.Register("ScaleInfoAlignment", typeof(HorizontalAlignment), typeof(WeightDetails), new PropertyMetadata(HorizontalAlignment.Right));

        /// <summary>
        /// Dependency property for the width of the scale information.
        /// </summary>
        public static readonly DependencyProperty ScaleInfoWidthProperty =
            DependencyProperty.Register("ScaleInfoWidth", typeof(double), typeof(WeightDetails), new PropertyMetadata(175.0));

        /// <summary>
        /// The main view model
        /// </summary>
        private IMainViewModel _viewModel;

        /// <summary>
        /// Culture Information for this view
        /// </summary>
        private CultureInfo _cultureInfo;

        /// <summary>
        /// Initializes a new instance of the WeightDetails class.
        /// </summary>
        public WeightDetails()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Gets or sets a value indicating whether minimum text height requirements must be met.
        /// </summary>
        public bool NMI
        {
            get { return (bool)GetValue(NMIProperty); }
            set { SetValue(NMIProperty, value); }
        }

        /// <summary>
        /// Gets or sets a value indicating the horizontal alignment of the scale information.
        /// </summary>
        public HorizontalAlignment ScaleInfoAlignment
        {
            get { return (HorizontalAlignment)GetValue(ScaleInfoAlignmentProperty); }
            set { SetValue(ScaleInfoAlignmentProperty, value); }
        }

        /// <summary>
        /// Gets or sets a value indicating the width of the scale information.
        /// </summary>
        public double ScaleInfoWidth
        {
            get { return (double)GetValue(ScaleInfoWidthProperty); }
            set { SetValue(ScaleInfoWidthProperty, value); }
        }

        /// <summary>
        /// Changes the weight to read Left-To-Right or Right-To-Left based on the current language.
        /// </summary>
        internal void UpdateWeightTextFlowDirection()
        {
            _cultureInfo = new CultureInfo(_viewModel.Language);

            if (_cultureInfo.TextInfo.IsRightToLeft)
            {
                WeightTextBlock.FlowDirection = FlowDirection.RightToLeft;
            }
            else
            {
                WeightTextBlock.FlowDirection = FlowDirection.LeftToRight;
            }
        }

        /// <summary> 
        /// Refresh Scale 
        /// </summary> 
        private void RefreshScale()
        {
            if (_viewModel.StateParam.Equals("Disconnected")
                || (_viewModel.State.Equals("Loading") && _viewModel.StateParam.Equals("Startup")))
            {
                ScaleLogo.Visibility = Visibility.Collapsed;
            }
            else
            {
                ScaleLogo.Visibility = Convert.ToBoolean(_viewModel.GetPropertyValue("ScaleLogo")) || Convert.ToBoolean(_viewModel.GetPropertyValue("SMScaleImage"))
                    ? Visibility.Visible : Visibility.Collapsed;
            }

            if (ScaleLogo.Visibility == Visibility.Visible)
            {
                ScaleInfo.Text = _viewModel.GetPropertyValue("ScaleInfo").ToString();
            }
            else
            {
                ScaleInfo.Text = string.Empty;
            }
        }

        /// <summary>
        /// Event handler for when the grid data context changes.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void Grid_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (_viewModel != null)
            {
                _viewModel.PropertyChanged -= new PropertyChangedEventHandler(ViewModel_PropertyChanged);
            }

            _viewModel = DataContext as IMainViewModel;
            _viewModel.PropertyChanged += new PropertyChangedEventHandler(ViewModel_PropertyChanged);

            OnRefreshWeight();
            RefreshScale();
            UpdateWeightTextFlowDirection();
        }

        /// <summary>
        /// OnRefreshWeight method
        /// </summary>
        private void OnRefreshWeight()
        {
            // CRD 229358 - added conditions to disable the weightDetails panel on SS90s.
            if (_viewModel.StateParam.Equals("Disconnected")
                || (_viewModel.State.Equals("Loading") && _viewModel.StateParam.Equals("Startup"))
                || (_viewModel.StateParam.Equals("Welcome")
                || _viewModel.StateParam.Equals("Scan")
                || _viewModel.StateParam.Equals("Finish")
                || _viewModel.StateParam.Equals("Payment"))
                && _viewModel.GetPropertyValue("TerminalType").Equals("SS90"))
            {
                WeightTextBlock.Text = string.Empty;
            }
            else
            {
                string weight = _viewModel.GetPropertyValue("Weight").ToString();

                WeightTextBlock.Text = weight;
            }
        }

        /// <summary> 
        /// ViewModel PropertyChanged 
        /// </summary> 
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of PropertyChangedEventArgs</param> 
        private void ViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            HandlePropertyChanged(e.PropertyName, IsVisible);
        }

        /// <summary>
        /// Handles the view model property changes.
        /// </summary>
        /// <param name="propertyName">The name of the property</param>
        /// <param name="isControlVisible">Whether or not the control is visible</param>
        private void HandlePropertyChanged(string propertyName, bool isControlVisible)
        {
            if (!isControlVisible)
            {
                return;
            }

            switch (propertyName)
            {
                case "Language":
                case "Languages":
                case "CMButton5MidiListShown":
                case "LanguageWelcomeShown":
                case "NextGenData":
                    UpdateWeightTextFlowDirection();
                    break;
                case "StateParam":
                    RefreshScale();
                    OnRefreshWeight();
                    UpdateWeightTextFlowDirection();
                    break;
                case "Weight":
                    OnRefreshWeight();
                    break;
                case "ScaleLogo":
                case "ScaleInfo":
                case "SMScaleImage":
                    RefreshScale();
                    break;
            }
        }
    }
}
