// <copyright file="TenderStep.xaml.cs" company="NCR">
//     Copyright (c) NCR. All rights reserved.
// </copyright>
namespace SSCOUIViews.Views
{
    using System.Collections.Generic;
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
    using FPsxWPF;
    using SSCOControls;
    using SSCOUIModels;
    using SSCOUIModels.Controls;
    using SSCOUIModels.Helpers;
    using SSCOUIModels.Models;

    /// <summary>
    /// Interaction logic for TenderStep.xaml
    /// </summary>
    public partial class TenderStep : BackgroundView
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
        /// Initializes a new instance of the <see cref="TenderStep"/> class.
        /// </summary>
        /// <param name="viewModel">IMainViewModel viewModel</param>
        public TenderStep(IMainViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
        }

        /// <summary>
        /// Override method for On Property Changed
        /// </summary>
        /// <param name="name">string name</param>
        /// <param name="value">object value</param>
        public override void OnPropertyChanged(string name, object value)
        {
            switch (name)
            {
                case "ChangeDue":
                    _changeDue = GetAmountDue(name);
                    break;
                case "RefundDue":
                    _refundDue = GetAmountDue(name);
                    break;
                case "CMButton1MedShown":
                    UpdatePayOtherWay();
                    break;
                case "CMButton1MedText":
                    UpdateCMButton1Med();
                    break;
                case "AmountDue":
                    DueAmount.Text = value.ToString();
                    break;
                case "ShowSigCapture":
                    UpdateSignatureControls();
                    break;
                case "DegradedMode":
                    UpdatePayOtherWay();
                    break;
                case "TenderAnimation":
                    HandleCardAnimation();
                    break;
                case "BackgroundStateParam":
                case "AttendantMode":
                case "State":
                    ShowSignatureProcessingPopup();
                    break;
                case "InstructionsShown":
                    UpdateInstructions();
                    break;
                case "LeadthruTextShown":
                    UpdateLeadthruText();				
                    break;
                // SSCOI-45996
                case "Instructions":
                    this.Instructions.Text = value.ToString();
                    break;
				//SSCOI-47481
                case "MPerksScanAndBagEnabled":
                    UpdatemPerksButton();
                    break;
                case "ButtonGoBackShown":
                case "ButtonGoBackEnabled":
                    UpdateGoBackButton();
                    break;
            }
        }

        /// <summary>
        /// OnStateParamChanged method
        /// </summary>
        /// <param name="param">This is a parameter with a type of String</param>
        public override void OnStateParamChanged(string param)
        {
            PayOtherWayButton.Visibility = Visibility.Collapsed;
            UpdateLeadthruText();
            UpdateInstructions();
            UpdateSignatureControls();
            ShowSignatureProcessingPopup();
            UpdateGoBackButton();
            UpdateCMButton1Med();

            switch (param)
            {
                case "CashPayment":
                    PayOtherWayButton.Visibility = GetPropertyBoolValue("CMButton1MedShown") ? Visibility.Visible : Visibility.Collapsed;
                    ChangeDueLabelText();
                    break;
                case "DepositCoupon":
                    this.LeadthruText.Property(TextBlock.TextProperty).SetResourceValue("DepositCoupon");
                    this.CMButton1Med.Property(ImageButton.TextProperty).SetResourceValue("Done");
                    this.Instructions.Text = GetPropertyStringValue("Instructions");
                    this.Instructions.Visibility = System.Windows.Visibility.Visible;
                    ChangeDueLabelText();
                    break;
                case "InsertCoupon":
                    this.LeadthruText.Property(TextBlock.TextProperty).SetResourceValue("InsertCoupon");
                    this.Instructions.Text = GetPropertyStringValue("Instructions");
                    this.Instructions.Visibility = System.Windows.Visibility.Visible;
                    ChangeDueLabelText();
                    UpdatemPerksButton(); // SSCOI-46714
                    break;
                case "EnterPin":
                case "EnterAmount":
                case "ScanCard":                
                case "InsertGiftCard":
                case "ScanVoucher":                
                case "DepositGiftCard":
                case "TakeCash":
                    ChangeDueLabelText();
                    break;
                case "SwipeCard":
                    ChangeDueLabelText();
                    HandleCardAnimation();
                    break;
                case "AcknowledgeSig":
                    DueLabel.Property(TextBlock.TextProperty).SetResourceFormattedValue("Due", string.Empty);
                    SignatureControl.OKButton.TabIndex = 0;
                    break;
                case "RequestSig":
                    DueLabel.Property(TextBlock.TextProperty).SetResourceFormattedValue("Change", string.Empty);
                    break;
            }
        }

        /// <summary>
        /// Override method for Show
        /// </summary>
        /// <param name="isShowing">bool isShowing</param>
        public override void Show(bool isShowing)
        {
            UpdateSignatureControls();
            base.Show(isShowing);
        }

        /// <summary>
        /// Method for updating Signature Controls
        /// </summary>
        private void UpdateSignatureControls()
        {
            if (viewModel.StateParam.Equals("RequestSig") || viewModel.StateParam.Equals("AcknowledgeSig") ||
                viewModel.BackgroundStateParam.Equals("RequestSig") || viewModel.BackgroundStateParam.Equals("AcknowledgeSig"))
            {
                if (FPsx.ConvertFromOleBool((string)viewModel.GetPropertyValue("ShowSigCapture")))
                {
                    signNameAnimation.Visibility = Visibility.Collapsed;
                }
                else
                {
                    signNameAnimation.Visibility = Visibility.Visible;
                }

                SignatureControl.Visibility = Visibility.Visible;
                SignatureControl.RefreshUI();
            }
            else
            {
                signNameAnimation.Visibility = Visibility.Collapsed;
                SignatureControl.Visibility = Visibility.Collapsed;
            }
        }

        /// <summary>
        /// Shows a popup on top of the Signature Capture screen when there is an extended display.
        /// </summary>
        private void ShowSignatureProcessingPopup()
        {
            if (viewModel.AttendantMode && viewModel.ExtendedDisplayTouchSupported)
            {
                if ((viewModel.StateParam.Equals("RequestSig") && viewModel.BackgroundStateParam.Equals("RequestSig")) ||
                   (viewModel.StateParam.Equals("AcknowledgeSig") && viewModel.BackgroundStateParam.Equals("AcknowledgeSig") && !viewModel.State.Equals("Loading")))
                {
                    if (FPsx.ConvertFromOleBool(GetPropertyStringValue("ShowSigCapture")))
                    {
                        viewModel.ActionCommand.Execute("ViewModelSet(Context;SignatureProcessing)");
                    }
                }
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
        /// <returns>Amount Due</returns>
        private double GetAmountDue(string propertyName)
        {
            string amount = GetPropertyStringValue(propertyName);
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

        /// <summary>
        /// Updates Pay in Another Way button.
        /// </summary>
        private void UpdatePayOtherWay()
        {
            if (viewModel.BackgroundStateParam.Equals("CashPayment"))
            {
                PayOtherWayButton.Visibility = GetPropertyBoolValue("CMButton1MedShown") ? Visibility.Visible : Visibility.Collapsed;
            }
            else
            {
                PayOtherWayButton.Visibility = Visibility.Collapsed;
            }
        }

        /// <summary>
        /// Handles the correct animation for each type of payment.
        /// </summary>
        private void HandleCardAnimation()
        {
            string animationStyle = string.Empty;

            switch (this.viewModel.StateParam)
            {
                case "SwipeCard":
                    animationStyle = GetTenderAnimation();
                    if (!string.IsNullOrEmpty(animationStyle))
                    {
                        CardAnimation.Style = this.FindResource(animationStyle) as Style;
                    }

                    break;
                default:
                    break;
            }
        }

        /// /// <summary>
        /// Gets the current type of animation from TenderAnimation property.
        /// </summary>
        /// <returns>the animation of the selected tender type</returns>
        private string GetTenderAnimation()
        {
            string animation = this.viewModel.TenderAnimation.Animation;
            string animationStyle = string.Empty;

            if (animation != null)
            {
                switch (animation)
                {
                    case "scan":
                        animationStyle = "scanCardPaymentAnimationControlStyle";
                        break;
                    case "insert":
                        animationStyle = "insertCardAnimationControlStyle";
                        break;
                    default:
                        animationStyle = "cardPaymentAnimationControlStyle";
                        break;
                }
            }

            return animationStyle;
        }

        /// <summary>
        /// Method for On Load event
        /// </summary>
        /// <param name="sender">object sender</param>
        /// <param name="e">Routed Event Args</param>
        private void OnLoaded(object sender, RoutedEventArgs e)
        {
            DueAmount.Text = GetPropertyStringValue("AmountDue");
            PaidLabel.Property(TextBlock.TextProperty).SetResourceFormattedValue("Paid", string.Empty);

            if (viewModel.IsActiveBackground)
            {
                FocusHelper.SetInitialFocus(this, viewModel);
            }
        }

        /// <summary>
        /// Update Instructions property.
        /// </summary>
        private void UpdateInstructions()
        {
            switch (viewModel.StateParam)
            {
                case "SwipeCard":
                    Instructions.Property(TextBlock.TextProperty).SetResourceValue("TouchCancel");
                    Instructions.Visibility = GetPropertyBoolValue("InstructionsShown") ? Visibility.Visible : Visibility.Collapsed;
                    break;
                case "InsertGiftCard":
                    Instructions.Property(TextBlock.TextProperty).SetResourceValue("DepositCard");
                    Instructions.Visibility = GetPropertyBoolValue("InstructionsShown") ? Visibility.Visible : Visibility.Collapsed;
                    break;
                // SSCOI-45996
                case "InsertCoupon":
                    this.Instructions.Text = GetPropertyStringValue("Instructions");
                    this.Instructions.Visibility = System.Windows.Visibility.Visible;
                    break;
                default:
                    Instructions.Visibility = Visibility.Collapsed;
                    break;
            }
        }

        /// <summary>
        /// Update LeadthruText property.
        /// </summary>
        private void UpdateLeadthruText()
        {
            LeadthruText.Visibility = GetPropertyBoolValue("LeadthruTextShown") ? Visibility.Visible : Visibility.Collapsed;
            switch (viewModel.StateParam)
            {
                case "CashPayment":
                    LeadthruText.Property(TextBlock.TextProperty).SetResourceValue("InsertCash");
                    break;
                case "EnterPin":
                    LeadthruText.Property(TextBlock.TextProperty).SetResourceValue("EnterPin");
                    break;
                case "EnterAmount":
                    LeadthruText.Property(TextBlock.TextProperty).SetResourceValue("EnterAmount");
                    break;
                case "SwipeCard":
                    LeadthruText.Property(TextBlock.TextProperty).SetResourceValue("UsePinpad");
                    break;
                case "ScanCard":
                    LeadthruText.Property(TextBlock.TextProperty).SetResourceValue("ScanGiftCard");
                    break;
                case "ScanVoucher":
                    LeadthruText.Property(TextBlock.TextProperty).SetResourceValue("ScanVoucher");
                    break;
                case "DepositCoupon":
                    LeadthruText.Property(TextBlock.TextProperty).SetResourceValue("DepositCoupon");
                    break;
                case "InsertCoupon":
                    LeadthruText.Property(TextBlock.TextProperty).SetResourceValue("InsertCoupon");
                    break;
                case "DepositGiftCard":
                case "InsertGiftCard":
                    LeadthruText.Property(TextBlock.TextProperty).SetResourceValue("DepositGiftCard");
                    break;
                case "AcknowledgeSig":
                    LeadthruText.Property(TextBlock.TextProperty).SetResourceValue("SignNeeded");
                    break;
                case "RequestSig":
                    LeadthruText.Property(TextBlock.TextProperty).SetResourceValue("SignNeeded");
                    break;
                case "TakeCash":
                    LeadthruText.Property(TextBlock.TextProperty).SetResourceValue("TakeCash");
                    break;
                default:
                    LeadthruText.Visibility = Visibility.Collapsed;
                    break;
            }
        }

        /// <summary>
        /// Update GoBack Button Property
        /// </summary>
        private void UpdateGoBackButton()
        {
            switch (viewModel.StateParam)
            {
                case "CashPayment":
                case "EnterPin":
                case "EnterAmount":
                    GoBack.Style = FindResource("cancelPaymentStyle") as Style;
                    GoBack.Property(ImageButton.TextProperty).SetResourceValue("CancelPayment");
                    GoBack.Visibility = GetPropertyBoolValue("ButtonGoBackShown") ? Visibility.Visible : Visibility.Collapsed;
                    GoBack.IsEnabled = GetPropertyBoolValue("ButtonGoBackEnabled");
                    break;
                case "SwipeCard":
                case "ScanCard":
                case "ScanVoucher":
                    GoBack.Style = FindResource("buttonGoBackStyle") as Style;
                    GoBack.Property(ImageButton.TextProperty).SetResourceValue("GoBack");
                    GoBack.Visibility = GetPropertyBoolValue("ButtonGoBackShown") ? Visibility.Visible : Visibility.Collapsed;
                    GoBack.IsEnabled = GetPropertyBoolValue("ButtonGoBackEnabled");
                    break;
                default:
                    GoBack.Visibility = Visibility.Collapsed;
                    break;
            }
        }

        /// <summary>
        /// Method for On Unloaded event
        /// </summary>
        /// <param name="sender">object sender</param>
        /// <param name="e">Routed Event Args</param>
        private void TenderStep_Unloaded(object sender, RoutedEventArgs e)
        {
            if (viewModel.StateParam.Equals("SignatureProcessing"))
            {
                viewModel.ActionCommand.Execute("ViewModelSet(Context;)");
            }
        }

        /// <summary>
        /// Update CMButton1Med button properties.
        /// </summary>
        private void UpdateCMButton1Med()
        {
            switch (viewModel.StateParam)
            {
                case "ScanVoucher":
                    CMButton1Med.Property(ImageButton.TextProperty).SetResourceValue("OtherPayment");
                    break;
                case "DepositCoupon":
                case "DepositGiftCard":
                    CMButton1Med.Property(ImageButton.TextProperty).SetResourceValue("Done");
                    break;
                case "TakeCash":
                    CMButton1Med.Property(ImageButton.TextProperty).SetResourceValue("Proceed");
                    break;
                default:
                    CMButton1Med.Visibility = Visibility.Collapsed;
                    break;
            }
        }

        /// <summary>
        /// SSCOI-46714: Method to update the mPerks buttons in Insert Coupon screen
        /// </summary>
        private void UpdatemPerksButton()
        {
            if (this.viewModel.StateParam.Equals("InsertCoupon"))
            {
                MPerksScanAndBagButton.IsEnabled = GetPropertyBoolValue("MPerksScanAndBagEnabled");
            }
        }// -SSCOI-46714
    }
}
