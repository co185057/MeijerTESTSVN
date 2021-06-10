//-----------------------------------------------------------------------
// <copyright file="Finish.xaml.cs" company="NCR Corporation">
//     Copyright (c) NCR Corporation. All rights reserved.
// </copyright>
// <summary>This is the Finish class.</summary>
//-----------------------------------------------------------------------

namespace SSCOUIViews.Views
{
    using System;
    using System.ComponentModel;
    using System.Globalization;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Input;
    using System.Windows.Media.Imaging;
    using RPSWNET;
    using SSCOControls;
    using SSCOControls.Helpers.KeyboardSimulatorComponents;
    using SSCOControls.Interfaces;
    using SSCOUIModels;
    using SSCOUIModels.Controls;
    using SSCOUIModels.Helpers;
    
    /// <summary>
    /// Interaction logic for Finish.xaml.
    /// </summary>
    public partial class Finish : BackgroundView
    {
        /// <summary>
        /// Initializes a new instance of the Finish class.        
        /// </summary>
        /// <param name="viewModel">ViewModel type of parameter.</param>
        public Finish(IMainViewModel viewModel)
            : base(viewModel)
        {
            this.InitializeComponent();
            LoadFinishImage();

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
        /// OnStateParamChanged that accepts param that is set in app.config.
        /// </summary>
        /// <param name="param">String type of param.</param>
        public override void OnStateParamChanged(string param)
        {
            this.OKButton.Visibility = Visibility.Collapsed; //SSCOI-44990 
           
            switch (param)
            {
                case "TakeChange": //(+)SSCOI-44990 
                    this.AmountDue.Visibility = Visibility.Visible;
                    this.AmountPaid.Visibility = Visibility.Visible;
                    this.AmountDueLabel.Visibility = Visibility.Visible;
                    this.AmountChangeLabel.Visibility = Visibility.Visible;
                    this.HeaderText.Visibility = Visibility.Collapsed;
                    this.InstructionTextArea.Text = GetPropertyStringValue("Instructions");
                    this.PrintReceiptButton.Visibility = Visibility.Collapsed;
                    this.OKButton.Visibility = Visibility.Collapsed;
                    break; //(-)SSCOI-44990 
                case "TakeChangeTimeOut":
                    this.HeaderText.Visibility = Visibility.Collapsed; //SSCOI-44990 
                    // this.HeaderText.Property(TextBlock.TextProperty).SetResourceValue("TakeChange"); //SSCOI-44990 
                    this.InstructionTextArea.Text = GetPropertyStringValue("Instructions"); //SSCOI-44990 
                    this.PrintReceiptButton.Visibility = Visibility.Collapsed;
                    this.OKButton.Visibility = Visibility.Visible; //SSCOI-44990 

                    this.AmountDue.Visibility = Visibility.Collapsed;
                    this.AmountPaid.Visibility = Visibility.Collapsed;
                    this.AmountDueLabel.Visibility = Visibility.Collapsed;
                    this.AmountChangeLabel.Visibility = Visibility.Collapsed;
                    break;
                case "TakeReceipt":
					// Start CRD 125377
                    //if (!GetPropertyStringValue("ShopAndDropEnabled").Equals("0"))
					if (GetPropertyStringValue("TransactionMode").Equals("1"))
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, String.Format("*_* SSCOUIViews.Views Finish.xaml.cs - TransactionMode 1)"));
                        this.SNDThankYou.Visibility = Visibility.Visible;
                        this.MeijerGoOnlyThankYou.Visibility = Visibility.Collapsed;
                        this.SCOOnlyThankYou.Visibility = Visibility.Collapsed;

                        this.SNDInstructionTextArea.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_139");
                        this.SNDHeaderText.Visibility = Visibility.Visible;
                        this.SNDHeaderText.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9508");

                        this.SNDLeadthruText.Text = GetPropertyStringValue("LeadthruText");
                        this.SNDLeadthruText.Visibility = Visibility.Visible;
                        this.SNDLeadthruText.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9396");

                        this.SNDAmountDue.Visibility = Visibility.Collapsed;
                        this.SNDAmountPaid.Visibility = Visibility.Collapsed;
                        this.SNDAmountDueLabel.Visibility = Visibility.Collapsed;
                        this.SNDAmountChangeLabel.Visibility = Visibility.Collapsed;
                    }
                    // End CRD 125377
                    //Start CRD 127058 meijerGo only
                    else if (GetPropertyStringValue("TransactionMode").Equals("2"))
                    {
                        this.OKButton.Visibility = Visibility.Collapsed;
                        this.MeijerGoOnlyThankYou.Visibility = Visibility.Visible;
                        this.SCOOnlyThankYou.Visibility = Visibility.Collapsed;
                        this.SNDThankYou.Visibility = Visibility.Collapsed;
                    }
                    //End CRD 127058 meijerGo only
                    //Start CRD 127513 transaction mode 0
                    else
                    {
                        this.HeaderText.Visibility = Visibility.Collapsed;
                        InstructionTextArea.Property(TextBlock.TextProperty).SetResourceValue("PrintingReceipt");
                        this.PrintReceiptButton.Visibility = Visibility.Collapsed;

                        this.SCOOnlyThankYou.Visibility = Visibility.Visible;
                        this.MeijerGoOnlyThankYou.Visibility = Visibility.Collapsed;
                        this.SNDThankYou.Visibility = Visibility.Collapsed;
                        this.SNDHeaderText.Visibility = Visibility.Collapsed;
                        this.SNDLeadthruText.Visibility = Visibility.Collapsed;
                    }
                    //End CRD 127513 transaction mode 0
                    this.HeaderText.Visibility = Visibility.Collapsed;
                    InstructionTextArea.Property(TextBlock.TextProperty).SetResourceValue("PrintingReceipt");
                    this.PrintReceiptButton.Visibility = Visibility.Collapsed;
                    break;
                case "Finish":
                    //Start CRD 127058 meijerGo only
                    if (GetPropertyStringValue("TransactionMode").Equals("2"))
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, String.Format("*_* SSCOUIViews.Views Finish.xaml.cs - TransactionMode - 2)"));
                        this.OKButton.Visibility = Visibility.Collapsed;
                        this.MeijerGoOnlyThankYou.Visibility = Visibility.Visible;
                        this.SCOOnlyThankYou.Visibility = Visibility.Collapsed;
                        this.SNDThankYou.Visibility = Visibility.Collapsed;
                    }
                    //End CRD 127058 meijerGo only
					// Start CRD 125377
                    //if (!GetPropertyStringValue("ShopAndDropEnabled").Equals("0"))
                    else if (GetPropertyStringValue("TransactionMode").Equals("1"))
                    {
                        CmDataCapture.Capture(CmDataCapture.MaskInfo, String.Format("*_* SSCOUIViews.Views Finish.xaml.cs - TransactionMode - 1)"));
                        this.SNDThankYou.Visibility = Visibility.Visible;
                        this.MeijerGoOnlyThankYou.Visibility = Visibility.Collapsed;
                        this.SCOOnlyThankYou.Visibility = Visibility.Collapsed;
                        this.SNDLeadthruText.Text = GetPropertyStringValue("LeadthruText");
                        this.SNDLeadthruText.Visibility = Visibility.Visible;
                        this.SNDLeadthruText.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9395");
                        this.SNDHeaderText.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9507");

                        this.SNDInstructionTextArea.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9509");
                        
                        this.SNDAmountDue.Visibility = Visibility.Collapsed;
                        this.SNDAmountPaid.Visibility = Visibility.Collapsed;
                        this.SNDAmountDueLabel.Visibility = Visibility.Collapsed;
                        this.SNDAmountChangeLabel.Visibility = Visibility.Collapsed;
                    }
                    // End CRD 125377
                    else //CRD 127513 transaction mode 0
                    {
                        this.SNDThankYou.Visibility = Visibility.Collapsed;
                        this.MeijerGoOnlyThankYou.Visibility = Visibility.Collapsed;
                        this.SCOOnlyThankYou.Visibility = Visibility.Visible;

                    }

                    this.HeaderText.Visibility = Visibility.Visible;
                    this.HeaderText.Property(TextBlock.TextProperty).SetResourceValue("Finish");
                    InstructionTextArea.Property(TextBlock.TextProperty).SetResourceValue("TakeItemsAndReceipt");
                    this.OKButton.Visibility = Visibility.Collapsed; //SSCOI-44990 
                    this.PrintReceiptButton.Visibility = (bool)viewModel.GetPropertyValue("CMButton1MedShown") ? Visibility.Visible : Visibility.Collapsed;
                    break;
            }           
        }

        /// <summary>
        /// Event handler for PropertyChanged event
        /// </summary>
        /// <param name="name">Property name</param>
        /// <param name="value">New value</param>
        public override void OnPropertyChanged(string name, object value)
        {
            if (name.Equals("OKButton"))
            {
                OnStateParamChanged(viewModel.StateParam);
            }
            if (name.Equals("Instructions"))
            {
                OnStateParamChanged(viewModel.StateParam);
            }
            if (name.Equals("CMButton1MedShown"))
            {
                OnStateParamChanged(viewModel.StateParam);
            }
            else if (name.Equals("AmountDue"))
            {
                AmountDue.Text = value.ToString();
            }
            else if (name.Equals("BackgroundStateParam") && viewModel.StateParam.Equals("Finish"))
            {
                OnStateParamChanged(viewModel.StateParam);
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
        /// Loads the Finish image if any.
        /// </summary>
        private void LoadFinishImage()
        {
            try
            {
                string imagePath = ItemImageConverter.GetScotDirectory(ItemImageConverter.GetAppDrive(), ItemImageConverter.GetAppDir()) + "\\image\\Finish.png";
                if (!System.IO.File.Exists(imagePath))
                {
                    CmDataCapture.CaptureFormat(
                        CmDataCapture.MaskInfo,
                        "SSCOUIViews.Views.Finish.LoadFinishImage() - No finish image found: {0}",
                        imagePath);
                    return;
                }

                BitmapImage bi = new BitmapImage();
                bi.BeginInit();
                bi.CacheOption = BitmapCacheOption.OnLoad;
                bi.CreateOptions = BitmapCreateOptions.IgnoreImageCache;
                bi.UriSource = new Uri(string.Format(CultureInfo.CurrentCulture, imagePath), UriKind.Relative);
                bi.EndInit();
                this.FinishImage.Source = bi;
            }
            catch (Exception ex)
            {
                CmDataCapture.CaptureFormat(
                    CmDataCapture.MaskError,
                    "SSCOUIViews.Views.Finish.LoadFinishImage() - Caught Exception {0} ",
                    ex.Message);
            }
        }

        /// <summary>
        /// Event handler for Loaded event
        /// </summary>
        /// <param name="sender">sending object</param>
        /// <param name="e">Event arguments</param>
        private void OnLoaded(object sender, RoutedEventArgs e)
        {
            AmountDue.Text = GetPropertyStringValue("AmountDue");
            MeijerGoAmountDue.Text = GetPropertyStringValue("AmountDue");	//VSTS 127058
            if (viewModel.IsUNavEnabled && PrintReceiptButton.IsVisible)
            {
                PrintReceiptButton.Focus();
            }
        }
    }
}
