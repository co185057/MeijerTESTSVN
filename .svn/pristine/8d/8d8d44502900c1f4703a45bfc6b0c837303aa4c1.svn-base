// <copyright file="ExtendedSignatureCapture.xaml.cs" company="NCR">
//     Copyright (c) NCR. All rights reserved.
// </copyright>
namespace SSCOUIViews.Controls
{
    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.ComponentModel;
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
    using SSCOControls;
    using SSCOUIModels;
    using SSCOUIModels.Controls;
    using SSCOUIModels.Helpers;
    using SSCOUIViews.Controls.BaseClasses;

    /// <summary>
    /// Interaction logic for ExtendedSignatureCapture.xaml
    /// </summary>
    public partial class ExtendedSignatureCapture : SignatureCaptureBase
    {
        /// <summary>
        /// Refund Due field
        /// </summary>
        private double _refundDue = 0;

        /// <summary>
        /// Change Due field
        /// </summary>
        private double _changeDue = 0;

        /// <summary>
        /// Initializes a new instance of the <see cref="ExtendedSignatureCapture"/> class
        /// </summary>
        public ExtendedSignatureCapture()
        {
            InitializeComponent();

            this.BaseClearButton = this.ClearButton;
            this.BaseOKButton = this.OKButton;
            this.BaseSignatureCanvas = this.SignatureCanvas;
            this.BaseSignaturePath = this.SignaturePath;

            InitializeSignaturePath();

            TouchMoveDuplicatorEvent += ExtendedSignatureCapture_TouchMoveDuplicatorEvent;
            TouchDownDuplicatorEvent += ExtendedSignatureCapture_TouchDownDuplicatorEvent;
            TouchUpDuplicatorEvent += ExtendedSignatureCapture_TouchUpDuplicatorEvent;
            TouchLeaveDuplicatorEvent += ExtendedSignatureCapture_TouchLeaveDuplicatorEvent;
        }

        /// <summary>
        /// Extended Signature Capture method for Touch Leave Duplicator Event
        /// </summary>
        /// <param name="sender">Object sender</param>
        /// <param name="e">Event Args</param>
        private void ExtendedSignatureCapture_TouchLeaveDuplicatorEvent(object sender, EventArgs e)
        {
            DuplicateTouchLeave(sender, e);
        }

        /// <summary>
        /// Extended Signature Capture method for Touch Up Duplicator Event
        /// </summary>
        /// <param name="sender">Object sender</param>
        /// <param name="e">Event Args</param>
        private void ExtendedSignatureCapture_TouchUpDuplicatorEvent(object sender, EventArgs e)
        {
            DuplicateTouchUp(sender, e);
        }

        /// <summary>
        /// Extended Signature Capture method for Touch Down Duplicator Event
        /// </summary>
        /// <param name="sender">Object sender</param>
        /// <param name="e">Event Args</param>
        private void ExtendedSignatureCapture_TouchDownDuplicatorEvent(object sender, EventArgs e)
        {
            DuplicateTouchDown(sender, e);
        }

        /// <summary>
        /// Extended Signature Capture method for Touch Move Duplicator Event
        /// </summary>
        /// <param name="sender">Object sender</param>
        /// <param name="e">Event Args</param>
        private void ExtendedSignatureCapture_TouchMoveDuplicatorEvent(object sender, EventArgs e)
        {
            DuplicateTouchMove(sender, e);
        }

        /// <summary>
        /// Method fot Grid Data Context Changed 
        /// </summary>
        /// <param name="sender">Object sender</param>
        /// <param name="e">Dependency Property Changed Event Args</param>
        private void Grid_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            ViewModel = DataContext as IMainViewModel;
            if (this.ViewModel != null)
            {
                this.ViewModel.PropertyChanged -= new PropertyChangedEventHandler(this.ViewModel_PropertyChanged);
            }

            this.ViewModel = DataContext as IMainViewModel;
            this.ViewModel.PropertyChanged += new PropertyChangedEventHandler(this.ViewModel_PropertyChanged);

            DueLabel.Property(TextBlock.TextProperty).SetResourceFormattedValue("Due", string.Empty);

            DueAmount.Text = this.ViewModel.GetPropertyValue("AmountDue").ToString();
            this.Instruction.Property(TextBlock.TextProperty).SetResourceValue("SignBelow");
            EnableSignatureButtons();
            PaidLabel.Property(TextBlock.TextProperty).SetResourceFormattedValue("Paid", string.Empty);
        }

        /// <summary>
        /// Method for ViewModel Property Changed
        /// </summary>
        /// <param name="sender">Object sender</param>
        /// <param name="e">Property Changed Event Args</param>
        private void ViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName.Equals("ChangeDue"))
            {
                _changeDue = GetAmountDue(e.PropertyName.ToString());
            }
            else if (e.PropertyName.Equals("RefundDue"))
            {
                _refundDue = GetAmountDue(e.PropertyName.ToString());
            }
            else if (e.PropertyName.Equals("AmountDue"))
            {
                DueAmount.Text = this.ViewModel.GetPropertyValue("AmountDue").ToString();
            }
            else if (e.PropertyName.Equals("StateParam"))
            {
                OnStateParamChanged(ViewModel.StateParam);
            }
            else if (e.PropertyName.Equals("CMButton1MedShown") || e.PropertyName.Equals("CMButton1MedEnabled"))
            {
                EnableOKButton();
            }
            else if (e.PropertyName.Equals("CMButton2MedShown") || e.PropertyName.Equals("CMButton2MedEnabled"))
            {
                EnableClearButton();
            }
        }

        /// <summary>
        /// OnStateParamChanged method
        /// </summary>   
        /// <param name="param">This is a parameter with a type of String</param>
        private void OnStateParamChanged(string param)
        {
            string dueLabel = string.Empty;
            switch (param)
            {
                case "AcknowledgeSig":
                    this.LeadthruText.Property(TextBlock.TextProperty).SetResourceValue("SignNeeded");
                    DueLabel.Property(TextBlock.TextProperty).SetResourceFormattedValue("Due", string.Empty);
                    EnableSignatureButtons();
                    break;
                case "RequestSig":
                    this.LeadthruText.Property(TextBlock.TextProperty).SetResourceValue("SignNeeded");
                    DueLabel.Property(TextBlock.TextProperty).SetResourceFormattedValue("Change", string.Empty);
                    ClearSignature();
                    break;
                case "SignatureProcessing":
                    if (!ViewModel.BackgroundStateParam.Equals("AcknowledgeSig"))
                    {
                        ClearSignature();
                    }

                    if (ViewModel.BackgroundStateParam.Equals("RequestSig"))
                    {
                        DueLabel.Property(TextBlock.TextProperty).SetResourceFormattedValue("Change", string.Empty);
                    }
                    else
                    {
                        DueLabel.Property(TextBlock.TextProperty).SetResourceFormattedValue("Due", string.Empty);
                    }

                    break;
                case "CashPayment":
                case "EnterPin":
                case "EnterAmount":
                case "SwipeCard":
                case "ScanCard":
                case "ScanVoucher":
                case "DepositCoupon":
                case "InsertCoupon":
                case "DepositGiftCard":
                case "InsertGiftCard":
                case "TakeCash":
                    ChangeDueLabelText();
                    break;
            }
        }

        /// <summary>
        /// Method for updating Due Label text
        /// </summary>
        private void ChangeDueLabelText()
        {
            _refundDue = GetAmountDue("RefundDue");
            _changeDue = GetAmountDue("ChangeDue");
            if (_refundDue <= 0 && _changeDue <= 0)
            {
                DueLabel.Property(TextBlock.TextProperty).SetResourceFormattedValue("Due", string.Empty); 
            }
        }

        /// <summary>
        /// Method for getting the Amount Due
        /// </summary>
        /// <param name="propertyName">Property Name</param>
        /// <returns>Value for Amount Due</returns>
        private double GetAmountDue(string propertyName)
        {
            string amount = this.ViewModel.GetPropertyValue(propertyName).ToString();
            string dueLabel = string.Empty;
            double value = amount != null && amount.Length > 0 ? double.Parse(amount) : 0;
            value = value / 100;
            if (value > 0 && propertyName.Equals("ChangeDue"))
            {
                DueLabel.Property(TextBlock.TextProperty).SetResourceFormattedValue("Change", string.Empty); 
            }
            else if (value > 0 && propertyName.Equals("RefundDue"))
            {
                DueLabel.Property(TextBlock.TextProperty).SetResourceFormattedValue("Refunded", string.Empty); 
            }

            return value;
        }
    }
}
