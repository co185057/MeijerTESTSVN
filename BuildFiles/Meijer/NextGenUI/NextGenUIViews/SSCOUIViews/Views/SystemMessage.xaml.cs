// <copyright file="SystemMessage.xaml.cs" company="NCR">
//     Copyright 2016 NCR. All rights reserved.
// </copyright>

namespace SSCOUIViews.Views
{
    using System;
    using System.Collections;
    using System.Globalization;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Data;
    using System.Windows.Input;
    using System.Windows.Media.Imaging;
    using System.Windows.Navigation;
    using PsxNet;
    using RPSWNET;
    using SSCOControls;
    using SSCOUIModels;
    using SSCOUIModels.Controls;
    using SSCOUIModels.Models;

    /// <summary>
    /// Interaction logic for SystemMessage.XAML
    /// </summary>
    public partial class SystemMessage : BackgroundView
    {
        /// <summary>
        /// Initializes a new instance of the SystemMessage class.
        /// </summary>
        /// <param name="viewModel">ViewModel object.</param>
        public SystemMessage(IMainViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
        }

        /// <summary>
        /// Handle the transition of showing this view behind a pop-up view.
        /// Hide the web control to prevent it from obscuring the pop-up.
        /// </summary>
        public override void OnShowBehindPopup()
        {
            SystemMessageWebGrid.Visibility = Visibility.Collapsed;
            SystemMessageButton1.Visibility = Visibility.Collapsed;
            SystemMessageButton2.Visibility = Visibility.Collapsed;
        }

        /// <summary>
        /// Accepts a parameter change as configured in app.config.
        /// </summary>
        /// <param name="param">Changed parameter.</param>
        public override void OnStateParamChanged(string param)
        {
            switch (param)
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
                    SystemMessageGrid.Visibility = Visibility.Visible;
                    SystemMessageWebGrid.Visibility = Visibility.Collapsed;
                    SystemMessageImage.Visibility = Visibility.Collapsed;
                    SystemMessageVideo.Visibility = Visibility.Collapsed;
                    TitleLine1.Visibility = Visibility.Collapsed;
                    TitleLine2.Visibility = Visibility.Collapsed;
                    TitleLine3.Visibility = Visibility.Collapsed;
                    SystemMessageVideo.Stop();
                    UpdateEntryListItemsSource();
                    break;

                case "SystemMessageWithWebControl":
                case "SystemMessageDegradedModeWithWebControl":
                case "SystemMessageHopperFailureWithWebControl":
                case "SystemMessageHopperSubstitutionWithWebControl":
                case "AM_SystemMessageWithWebControl":
                case "AM_SystemMessageDegradedModeWithWebControl":
                case "AM_SystemMessageHopperFailureWithWebControl":
                case "AM_SystemMessageHopperSubstitutionWithWebControl":
                    SystemMessageWebGrid.Visibility = Visibility.Visible;
                    SystemMessageGrid.Visibility = Visibility.Collapsed;
                    break;

                default:
                    SystemMessageWebGrid.Visibility = Visibility.Collapsed;
                    SystemMessageGrid.Visibility = Visibility.Collapsed;
                    break;
            }

            UpdateWebControl(); 
            UpdateImage(); 
            UpdateVideo();
            UpdateCMButton1Style();
            UpdateRetryBtnVisibility();
            UpdateSystemMessageButton1Shown();
            UpdateSystemMessageButton2Shown();
        }

        /// <summary>
        /// Accepts a property change as configured in app.config.
        /// </summary>
        /// <param name="name">Name of changed property.</param>
        /// <param name="value">Value of changed property.</param>
        public override void OnPropertyChanged(string name, object value)
        {
            switch (name)
            {
                case "DeviceErrorWebControl":
                    UpdateWebControl();
                    break;
                case "DeviceErrorImage":
                    UpdateImage();
                    break;
                case "DeviceErrorVideo":
                    UpdateVideo();
                    break;
                case "CMButton2MedShown":
                    UpdateCMButton1Style();
                    UpdateSystemMessageButton2Shown();
                    break;
                case "Title2Area":
                    if (!string.IsNullOrEmpty(GetPropertyStringValue(name)))
                    {
                        TitleLine1.Text = GetPropertyStringValue(name);
                        TitleLine1.Visibility = Visibility.Visible;
                    }
                    break;

                case "InstructionScreenTitle":
                    if (!string.IsNullOrEmpty(GetPropertyStringValue(name)))
                    {
                        TitleLine2.Text = GetPropertyStringValue(name);
                        TitleLine2.Visibility = Visibility.Visible;
                    }
                    break;

                case "TitleText":
                    if (!string.IsNullOrEmpty(GetPropertyStringValue(name)))
                    {
                        TitleLine3.Text = GetPropertyStringValue(name);
                        TitleLine3.Visibility = Visibility.Visible;
                    }
                    break;

                case "XMRetryButtonV6HWShown":
                    UpdateRetryBtnVisibility();
                    break;
                case "CMButton1MedShown":
                    UpdateSystemMessageButton1Shown();
                    break;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        private void UpdateRetryBtnVisibility()
        {
            XMRetryButtonV6HW.Visibility = Convert.ToBoolean(viewModel.GetPropertyValue("XMRetryButtonV6HWShown")) ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates web control container and displays web control if current context supports it.
        /// </summary>
        private void UpdateWebControl()
        {
            if (viewModel.StateParam.Contains("WithWebControl"))
            {
                string value = GetPropertyStringValue("DeviceErrorWebControl");
                if (!string.IsNullOrEmpty(value))
                {
                    try
                    {
                        SystemMessageWebControl.Navigate(new Uri(string.Format(CultureInfo.CurrentCulture, value.ToString()), UriKind.Absolute));
                    }
                    catch (Exception e)
                    {
                        CmDataCapture.CaptureFormat(CmDataCapture.MaskError, "SystemMessage::OnPropertyChanged DeviceErrorWebControl - Caught exception {0}", e.Message);
                    }
                }
            }
        }

        /// <summary>
        /// Updates image control container and displays image if current context supports it.
        /// </summary>
        private void UpdateImage()
        {
            if (viewModel.StateParam.Contains("WithBitmap"))
            {
                string value = GetPropertyStringValue("DeviceErrorImage");
                if (!string.IsNullOrEmpty(value))
                {
                    try
                    {
                        BitmapImage image = new BitmapImage();
                        image.BeginInit();
                        image.UriSource = new Uri(string.Format(CultureInfo.CurrentCulture, value.ToString()), UriKind.Absolute);
                        image.EndInit();
                        SystemMessageImage.Source = image;
                        SystemMessageImage.Visibility = Visibility.Visible;
                        SystemMessageVideo.Visibility = Visibility.Collapsed;
                    }
                    catch (Exception e)
                    {
                        CmDataCapture.CaptureFormat(CmDataCapture.MaskError, "SystemMessage::OnPropertyChanged DeviceErrorImage - Caught exception {0}", e.Message);
                    }
                }
            }
        }

        /// <summary>
        /// Updates video control container and plays video if current context supports it.
        /// </summary>
        private void UpdateVideo()
        {
            string value = GetPropertyStringValue("DeviceErrorImage");
            if (viewModel.StateParam.Contains("WithAVI"))
            {
                if (!string.IsNullOrEmpty(value))
                {
                    try
                    {
                        SystemMessageVideo.Source = new Uri(string.Format(CultureInfo.CurrentCulture, value.ToString()), UriKind.Absolute);
                        SystemMessageImage.Visibility = Visibility.Collapsed;
                        SystemMessageVideo.Visibility = Visibility.Visible;
                        PlayVideo();
                    }
                    catch (Exception e)
                    {
                        CmDataCapture.CaptureFormat(CmDataCapture.MaskError, "SystemMessage::OnPropertyChanged DeviceErrorVideo - Caught exception {0}", e.Message);
                    }
                }
            }
        }

        /// <summary>
        /// Updates button style.
        /// </summary>
        private void UpdateCMButton1Style()
        {
            SystemMessageButton1.Style = FindResource(GetPropertyBoolValue("CMButton2MedShown") ? "systemMessageButtonStyle" : "systemMessageButtonWithCheckStyle") as Style;
        }

        /// <summary>
        /// SystemMessageVideo handler for Ended event.
        /// Triggers a call to play video.
        /// </summary>
        /// <param name="sender">Event Sender.</param>
        /// <param name="e">EventArgs struct.</param>
        private void SystemMessageVideo_Ended(object sender, EventArgs e)
        {
            PlayVideo();
        }

        /// <summary>
        /// Sets the video playback progress and plays the video.
        /// </summary>
        private void PlayVideo()
        {
            SystemMessageVideo.Position = TimeSpan.FromSeconds(0);
            SystemMessageVideo.Play();
        }

        /// <summary>
        /// SystemMessageVideo handler for VideoFailed event.
        /// Captures the error.
        /// </summary>
        /// <param name="sender">Event Sender.</param>
        /// <param name="e">ExceptionRoutedEventArgs struct.</param>
        private void SystemMessageVideo_VideoFailed(object sender, ExceptionRoutedEventArgs e)
        {
            CmDataCapture.CaptureFormat(CmDataCapture.MaskError, "SystemMessage::OnPropertyChanged DeviceErrorVideo - Caught exception {0} {1}", e.ToString(), e.ErrorException.Message);
        }

        /// <summary>
        /// Updates the entry list, instruction area and titles based on current state.
        /// </summary>
        private void UpdateEntryListItemsSource()
        {
            switch (viewModel.StateParam)
            {
                case "CMLoanPickupFailureV6HW":
                case "CMTimedOutWaitingForTBV6HW":
                    SetEntryListItemSource(viewModel.GetPropertyValue("CMErrorMessagesV6HWList") as IEnumerable);
                    CashManagementErrorMessagesV6HW.Visibility = Visibility.Visible;
                    SystemMessageInstructionArea.Visibility = Visibility.Collapsed;
                    TitleLine1.Text = GetPropertyStringValue("XM_Title2AreaV6HWText");
                    TitleLine1.Visibility = Visibility.Visible;
                    break;
                default:
                    CashManagementErrorMessagesV6HW.Visibility = Visibility.Collapsed;
                    SystemMessageInstructionArea.Visibility = Visibility.Visible;
                    break;
            }
        }

        /// <summary>
        /// Set the entry list binding for error messages.
        /// </summary>
        /// <param name="receiptItems">IEnumerable receipt items.</param>
        private void SetEntryListItemSource(IEnumerable receiptItems)
        {
            Binding receiptBinding = new Binding();
            receiptBinding.Source = receiptItems;
            BindingOperations.SetBinding(this.CashManagementErrorMessagesV6HW, TouchListBox.ItemsSourceProperty, receiptBinding);
        }

        /// <summary>
        /// Updates SystemMessageButton1 visibility.
        /// </summary>
        private void UpdateSystemMessageButton1Shown()
        {
            SystemMessageButton1.Visibility = GetPropertyBoolValue("CMButton1MedShown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates SystemMessageButton2 visibility.
        /// </summary>
        private void UpdateSystemMessageButton2Shown()
        {
            SystemMessageButton2.Visibility = GetPropertyBoolValue("CMButton2MedShown") ? Visibility.Visible : Visibility.Collapsed;
        }
    }
}
