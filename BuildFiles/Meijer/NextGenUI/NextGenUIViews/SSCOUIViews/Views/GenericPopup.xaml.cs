// <copyright file="GenericPopup.xaml.cs" company="NCR">
//     Copyright 2016 NCR. All rights reserved.
// </copyright>

namespace SSCOUIViews.Views
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
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
    using SSCOUIModels.Models;
    using RPSWNET;
    using System.Globalization;  //VSTS 131568

    /// <summary>
    /// Interaction logic for the generic popup
    /// </summary>
    public partial class GenericPopup : PopupView
    {
        /// <summary>
        /// List variable
        /// </summary>
        private static List<string> paramListForYesNoButtonToShow = new List<string>() { "ConfirmQuantity", "Continue", "CrateableItem", "SellBagsConfirmQuantity", "SavePreferences", "LimitedEBTBenefit", "AM_ConfirmAbort", "AM_ConfirmVoid", "SmAbort", "mPerksApplyPrompt", "XMConfirmLockScreenV6HW" };

        /// <summary>
        /// Initializes a new instance of the GenericPopup class.
        /// </summary>
        /// <param name="viewModel">ViewModel object.</param>
        public GenericPopup(IMainViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
        }

        /// <summary>
        /// Accepts a parameter change as configured in app.config.
        /// </summary>
        /// <param name="param">Changed parameter.</param>
        public override void OnStateParamChanged(string param)
        {
            UpdateStateParam(param);
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
                case "ButtonGoBackShown":
					UpdateGoBack(); //SSCOI-48082
                    UpdateCancelAcceptVisibility();
                    UpdateButtonGoBackForDataNeededContext();
                    break;
                case "Instructions":
                    UpdateInstructions();
                    break;
                case "LeadthruText":
                    UpdateLeadthruText();
                    break;
                case "ButtonStoreLogInShown":
                case "CMButton1StoreLogInShown":
                    UpdateStoreLogin();
                    break;
                case "CMButton1MedShown":
                case "CMButton1MedText":
                    UpdateItemRemoved();
                    break;
                case "MessageBoxNoEcho":
                    UpdateMessageBox();
                    break;
                case "CMButton1MidiListShown":
                case "CMButton1MidiList":
                    UpdateMidiList1();
                    break;
                case "CMButton2MidiListShown":
                case "CMButton2MidiList":
                    UpdateMidiList2();
                    break;
                case "CMButton3MidiListShown":
                case "CMButton3MidiList":
                    UpdateMidiList3();
                    break;
                case "CMButton4MidiListShown":
                case "CMButton4MidiList":
                    UpdateMidiList4();
                    break;
                case "CMButton5MidiListShown":
                case "CMButton5MidiList":
                    UpdateMidiList5();
                    break;
                case "CMButton6MidiListShown":
                case "CMButton6MidiList":
                    UpdateMidiList6();
                    break;
                case "CMButton7MidiListShown":
                case "CMButton7MidiList":
                    UpdateMidiList7();
                    break;
                case "NextGenData":
                    UpdateNextGenData();
                    break;
                case "EchoMessage":
                    UpdateEchoMessage();
                    UpdateSMButton8();
                    break;
                case "RemoteButton1Shown":
                    UpdateGenericOkButton();
                    break;
                case "UNav":
                    FocusHelper.SetInitialFocus(this, viewModel);
                    break;
                case "ButtonGoBack":
                    UpdateButtonGoBackForDataNeededContext();
                    break;
                case "CMButton1Lg":  //SSCOI-44290
					UpdateCMButton1Lg();
                    break;
				case "CMButton2Lg":  //SSCOI-44290
					UpdateCMButton2Lg();
                    break;
				case "HighValuePromoChoice":  //SSCOI-45597
                    if (!this.viewModel.State.Equals("Loading"))
                    {
                        this.YesNoStackPanel.Visibility = Visibility.Visible;
                        this.Ok_Button.Visibility = Visibility.Collapsed;
                        UpdateYesNoButtonCommandParameter();
                    }
                    break;
				case "Amount1":                
				case "AmountDue": // SSCOI-47527
                UpdateAmountPaidAmountDue();
                    break;
			}
        }

		//(+) VSTS 131568
        private void HideAmountDue(string stringID)
        {
            
            string text1 = GetPropertyStringValue(stringID);
            CmDataCapture.CaptureFormat(CmDataCapture.MaskInfo, "text1:{0}", text1);
            string text2 = LocalizationManager.Instance.DefaultResourceManager.GetString("ScotApp_9429", new CultureInfo(viewModel.CustomerLanguage));
            CmDataCapture.CaptureFormat(CmDataCapture.MaskInfo, "text2:{0}", text2);
			 //(+) VSTS 149725
			string text3 = LocalizationManager.Instance.DefaultResourceManager.GetString("ScotApp_9375", new CultureInfo(viewModel.CustomerLanguage));
            string text4 = LocalizationManager.Instance.DefaultResourceManager.GetString("ScotApp_9376", new CultureInfo(viewModel.CustomerLanguage));
            string text5 = LocalizationManager.Instance.DefaultResourceManager.GetString("ScotApp_9377", new CultureInfo(viewModel.CustomerLanguage));
            CmDataCapture.CaptureFormat(CmDataCapture.MaskInfo, "text1:{0} text3:{1} text4:{2} text5:{3} ", text1, text3, text4, text5);
            if ((text1.Equals(text2)) || (text1.Equals(text3)) ||
                (text1.Equals(text4)) || 
				(text1.Equals(text5)))
            {
                Amount1.Visibility = Visibility.Collapsed;
                Amount1LabelText.Visibility = Visibility.Collapsed;
                Amount2.Visibility = Visibility.Collapsed;
                Amount2LabelText.Visibility = Visibility.Collapsed;
            }
			 //(-) VSTS 149725
            //(-) VSTS 131568
        }

        /// <summary>
        /// Updates the visibility of the cancel button on the enter weight screen.
        /// </summary>
        private void UpdateCancelAcceptVisibility()
        {
            bool showCancelAcceptWeight = viewModel.StateParam.Equals("EnterWeight") && GetPropertyBoolValue("ButtonGoBackShown");
            CancelAcceptWeight.Visibility = showCancelAcceptWeight ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the EchoPopupMessage text and visibility, loading animation style based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateEchoMessage()
        {
            switch (this.viewModel.StateParam)
            {
                case "EchoPopup":
                    this.EchoPopupMessage.Text = GetPropertyStringValue("EchoMessage");
                    this.EchoPopupMessage.Visibility = Visibility.Visible;
                    this.LoadingAnimation.Style = this.FindResource("storeModeLoadingAnimationControlStyle") as Style;
                    break;
                default:
                    this.EchoPopupMessage.Visibility = Visibility.Collapsed;
                    this.LoadingAnimation.Style = this.FindResource("loadingAnimationControlStyle") as Style;
                    break;
            }
        }

        /// <summary>
        /// Updates the controls to enable based on the background state parameter and corresponding property watch.
        /// </summary>
        private void UpdateSMButton8()
        {
            if (this.viewModel.StateParam.Equals("EchoPopup") && this.viewModel.BackgroundStateParam.Equals("SmReportsMenu"))
            {
                this.ControlsToAllowEnabled = "UNavControl,AccessibilityButton,StoreButton8";
            }
        }

        /// <summary>
        /// Updates the Instructions visibility and text based on the background state parameter and corresponding property watch.
        /// </summary>
        private void UpdateNextGenData()
        {
            switch (this.viewModel.StateParam)
            {
                case "Startup":
                case "OutOfService":
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue(this.viewModel.GetPropertyValue("NextGenData").ToString().Equals("Unloading") ?
                        "ShuttingDown" : "LoadingText");
                    this.Instructions.Visibility = Visibility.Visible;
                    break;
            }
        }

        /// <summary>
        /// Updates the GenericOkButton visibility, text and command parameter based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateGenericOkButton()
        {
            switch (this.viewModel.StateParam)
            {
                case "CheckBasket":
                    this.GenericOkButton.Visibility = Visibility.Visible;
                    this.GenericOkButton.Property(ImageButton.TextProperty).SetResourceValue("OK");
                    this.GenericOkButton.CommandParameter = "OKCheckBasket";
                    break;
                case "WaitForRemoteAssistance":
                    if (GetPropertyBoolValue("RemoteButton1Shown"))
                    {
                        this.GenericOkButton.Visibility = Visibility.Visible;
                        this.GenericOkButton.Property(ImageButton.TextProperty).SetResourceValue("Done");
                        this.GenericOkButton.CommandParameter = "RemoteButton1";
                    }
                    break;
                default:
                    this.GenericOkButton.Visibility = Visibility.Collapsed;
                    break;
            }
        }

        /// <summary>
        /// Updates the button visibility and text based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateMidiList1()
        {
            switch (this.viewModel.StateParam)
            {
                case "DataNeededConfirm":
                case "DataNeededMsg":
                case "DataNeededScanCard":
                    this.Ok_Button.Text = GetPropertyStringValue("CMButton1MidiList");
                    break;
            }

            this.CMButton1MidiList.Visibility = GetPropertyBoolValue("CMButton1MidiListShown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the button visibility and text based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateMidiList2()
        {
            switch (this.viewModel.StateParam)
            {
                case "DataNeededConfirm":
                case "DataNeededMsg":
                case "DataNeededScanCard":
                    this.GenericButton.Text = GetPropertyStringValue("CMButton2MidiList");
                    break;
            }

            this.CMButton2MidiList.Visibility = GetPropertyBoolValue("CMButton2MidiListShown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the CMButton3MidiList button visibility based on the corresponding property watch.
        /// </summary>
        private void UpdateMidiList3()
        {
            this.CMButton3MidiList.Visibility = GetPropertyBoolValue("CMButton3MidiListShown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the UpdateMidiList4 button visibility based on the corresponding property watch.
        /// </summary>
        private void UpdateMidiList4()
        {
            this.CMButton4MidiList.Visibility = GetPropertyBoolValue("CMButton4MidiListShown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the UpdateMidiList5 button visibility based on the corresponding property watch.
        /// </summary>
        private void UpdateMidiList5()
        {
            this.CMButton5MidiList.Visibility = GetPropertyBoolValue("CMButton5MidiListShown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the UpdateMidiList6 button visibility based on the corresponding property watch.
        /// </summary>
        private void UpdateMidiList6()
        {
            this.CMButton6MidiList.Visibility = GetPropertyBoolValue("CMButton6MidiListShown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the UpdateMidiList7 button visibility based on the corresponding property watch.
        /// </summary>
        private void UpdateMidiList7()
        {
            this.CMButton7MidiList.Visibility = GetPropertyBoolValue("CMButton7MidiListShown") ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the Instructions text based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateMessageBox()
        {
            this.MessageEchoCoupon.Visibility = Visibility.Collapsed;
            this.Instructions.Visibility = Visibility.Collapsed;

            switch (this.viewModel.StateParam)
            {
                case "DataNeededSelection":
                case "DataNeededConfirm":
                case "DataNeededMsg":
                case "DataNeededScanCard":
                    this.Instructions.Text = GetPropertyStringValue("MessageBoxNoEcho");
                    break;
                case "MeijerDigitalVendorCouponChoice"://SSCOI-44290
                    this.MessageEchoCoupon.Text = GetPropertyStringValue("MessageBoxNoEcho"); //SSCOI-47241
                    this.MessageEchoCoupon.Visibility = Visibility.Visible;
                    break;
            }

            UpdateEchoMessage();
        }

        /// <summary>
        /// Updates the ItemRemovedButton button visibility, text and command parameter based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateItemRemoved()
        {
            switch (this.viewModel.StateParam)
            {
                case "PickingUpItems":
                case "SecUnexpectedDecrease":
                    this.ItemRemovedButton.Visibility = GetPropertyBoolValue("CMButton1MedShown") ? Visibility.Visible : Visibility.Collapsed;
                    this.ItemRemovedButton.CommandParameter = "OKUnknownItem";
                    this.ItemRemovedButton.Property(ImageButton.TextProperty).SetResourceValue("RemovedItem");
                    break;
                case "SecUnExpectedIncrease":
                case "SecMisMatchWeight":
                    this.ItemRemovedButton.Visibility = GetPropertyBoolValue("CMButton1MedShown") ? Visibility.Visible : Visibility.Collapsed;
                    this.ItemRemovedButton.CommandParameter = "OKUnknownItem";
                    this.ItemRemovedButton.Property(ImageButton.TextProperty).SetResourceValue("UseOwnBag");
                    break;
                case "EnterWeight":
                    this.ItemRemovedButton.Visibility = GetPropertyBoolValue("CMButton1MedShown") ? Visibility.Visible : Visibility.Collapsed;
                    this.ItemRemovedButton.CommandParameter = "Yes";
                    this.ItemRemovedButton.Text = GetPropertyStringValue("CMButton1MedText");
                    break;
                default:
                    this.ItemRemovedButton.Visibility = Visibility.Collapsed;
                    break;
            }
        }

        /// <summary>
        /// Updates the StoreLogin button visibility based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateStoreLogin()
        {
            if (this.viewModel.StateParam.Equals("Disconnected"))
            {
                StoreLogin.Visibility = Visibility.Collapsed;
            }
            else
            {
                StoreLogin.Visibility = GetPropertyBoolValue("ButtonStoreLogInShown") || GetPropertyBoolValue("CMButton1StoreLogInShown") ? Visibility.Visible : Visibility.Collapsed;
            }
        }

        /// <summary>
        /// Updates the Instructions text and visibility based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateInstructions()
        {
            this.InstructionsCoupon.Visibility = Visibility.Collapsed;
            this.Instructions.Visibility = Visibility.Collapsed;
            switch (this.viewModel.StateParam)
            {
                case "SecNewItemEntry":
                    //(+)SSCOI-47755  //VSTS 131568
                    this.Instructions.Text = GetPropertyStringValue("Instructions");
                    this.Instructions.Visibility = Visibility.Visible;
                    break;
                    //(-)SSCOI-47755
                case "CustomMessage": 
                //(+)SSCOI-47755
                    this.Instructions.Text = GetPropertyStringValue("Instructions");
                    this.Instructions.Visibility = Visibility.Visible;
                    CmDataCapture.CaptureFormat(CmDataCapture.MaskInfo, "UpdateInstructions()");  //VSTS 131568
                    HideAmountDue("Instructions");
                    break;
                //(-)SSCOI-47755
                case "AssistedTender":
                case "CardMisRead":
                case "CouponNotAllowed":
                case "SavePreferences":
                case "CmDataEntry0":               
                case "BiometricProcessing":
                case "ConfirmEBTAmount":
                case "CouponTooHigh":
                case "CrateableItem":
                case "VoidNoMatch":
                case "DropoffCoupons":
                case "LimitedEBTBenefitOK":
                case "LimitedEBTBenefit":
                case "OperatorPasswordStateInvalidPasswordWithFP":
                case "OperatorPasswordStateInvalidPassword":
                case "ConfirmDebitAmount":
                case "SecBagAreaBackup":
                case "SignCharge":
                case "TakeCard":
                case "CheckBasket":
                case "SystemMessageOpPass":
                case "TakeLoyaltyCard":
                case "WaitForRemoteAssistance":
                case "SecOperatorPWStateInvalidPassword":
                case "TenderBalance":
				case "TimeRestrictedItem": //SSCOI-46723
                case "ConfirmQuantity":
                case "SellBagsConfirmQuantity":
                case "RestrictedNotAllowed":
                case "CouponNoMatch":
                case "Continue":
                case "CardProcessing":
                case "SecItemRemovedThreshold":
                case "VoidApproval":
                case "SecBagBoxThreshold":
                case "CollectGiftCard":
                case "FatalError":
                case "Suspend":
                case "UnDeActivatedItemApproval":
                case "SecSkipBaggingThreshold":
                case "RAPDataNeeded":
                case "UnknownItem":
                case "AM_ConfirmAbort":
                case "AM_ConfirmVoid":
				case "ChipCardEntry": //SSCOI-45885
                case "CheckinConfirmation":  
				case "InvalidmPerksPINEntry": //SSCOI-47170				
				case "mPerksPINEntryLimitReached": //CRD 125377					
                case "mPerksPINEntry": //SSCOI-44290
                case "RetrievingReceipt": // SSCOI-47555
                    this.Instructions.Text = GetPropertyStringValue("Instructions");
                    this.Instructions.Visibility = Visibility.Visible;
                    break;
                case "Processing":
                    //+SSCOI-47711
                    this.Instructions.Text = GetPropertyStringValue("Instructions");
                    this.Instructions.Visibility = Visibility.Visible;
                    Amount1.Visibility = Visibility.Collapsed;
                    Amount1LabelText.Visibility = Visibility.Collapsed;
                    Amount2.Visibility = Visibility.Collapsed;
                    Amount2LabelText.Visibility = Visibility.Collapsed;
                    break;
                //-SSCOI-47711
                case "mPerksApplyPrompt":
                    this.Instructions.Text = GetPropertyStringValue("MessageBoxNoEcho");
                    this.Instructions.Visibility = Visibility.Visible;
                    break;

                 //+CRD 125377
                case "RestrictedAgeYN":
                    //this.PopupTitle.Text = GetPropertyStringValue("Instructions");
                    //this.PopupTitle.Visibility = Visibility.Visible;
                    this.Instructions.Text = GetPropertyStringValue("MessageBoxNoEcho");
                    this.Instructions.Visibility = Visibility.Visible;
                    break;
                case "TextMsgYN":
                    //this.PopupTitle.Text = GetPropertyStringValue("Instructions");
                    //this.PopupTitle.Visibility = Visibility.Visible;
                    this.Instructions.Text = GetPropertyStringValue("MessageBoxNoEcho");
                    this.Instructions.Visibility = Visibility.Visible;
                    break;
                //-CRD 125377

                case "WICConfirmation":
                    this.Instructions.Text = GetPropertyStringValue("Instructions");
                    this.Instructions.Visibility = Visibility.Visible;
                    this.PopupTitle.Text = string.Empty; // This will collapse the visibility of PopupTitle
                    break;
                case "SmAbort":
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("ConfirmAbortTitle");
                    this.Instructions.Visibility = Visibility.Visible;
                    break;
                case "SecurityAnalysis":
                    this.Instructions.Text = GetPropertyStringValue("Instructions").Replace(".  ", ".\n");
                    this.Instructions.Visibility = Visibility.Visible;
                    break;
                case "PasswordExpired":	
                case "UsePinPadForWIC": //SSCOI-44290: Instructions text of previous screen  flickers
                    this.Instructions.Visibility = Visibility.Collapsed;
					//SSCOI-44290
                    Amount1.Visibility = Visibility.Collapsed;
                    Amount1LabelText.Visibility = Visibility.Collapsed;
                    Amount2.Visibility = Visibility.Collapsed;
                    Amount2LabelText.Visibility = Visibility.Collapsed;
                    Amount1.Property(TextBlock.TextProperty).SetResourceValue("AlertStatusTextOff");
                    Amount1LabelText.Property(TextBlock.TextProperty).SetResourceValue("AlertStatusTextOff");
                    Amount2.Property(TextBlock.TextProperty).SetResourceValue("AlertStatusTextOff");
                    Amount2LabelText.Property(TextBlock.TextProperty).SetResourceValue("AlertStatusTextOff");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("AlertStatusTextOff");
                    break;
                case "SwipeCardAsItem": //SSCOI-44592  
                    //(+) SSCOI-45159
                    this.Instructions.Visibility = Visibility.Collapsed;
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("AlertStatusTextOff");
                    this.PopupTitle.Text = GetPropertyStringValue("Instructions");
                    this.PopupTitle.Visibility = Visibility.Visible;
					
                    Amount1.Visibility = Visibility.Collapsed;
                    Amount1LabelText.Visibility = Visibility.Collapsed;
                    Amount2.Visibility = Visibility.Collapsed;
                    Amount2LabelText.Visibility = Visibility.Collapsed;
                    Amount1.Property(TextBlock.TextProperty).SetResourceValue("AlertStatusTextOff");
                    Amount1LabelText.Property(TextBlock.TextProperty).SetResourceValue("AlertStatusTextOff");
                    Amount2.Property(TextBlock.TextProperty).SetResourceValue("AlertStatusTextOff");
                    Amount2LabelText.Property(TextBlock.TextProperty).SetResourceValue("AlertStatusTextOff");                                     
                    break;
                //(-) SSCOI-45159
                case "UsePinPad": // SSCOI-44661: Instruction text from previous screen displayed.
                    this.Instructions.Visibility = Visibility.Collapsed;
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("AlertStatusTextOff");
					Amount1.Visibility = Visibility.Visible;
					Amount2.Visibility = Visibility.Visible;
                    break;
                case "MeijerDigitalVendorCouponChoice":    //SSCOI-44290               
                    this.InstructionsCoupon.Text = GetPropertyStringValue("Instructions"); //SSCOI-47241
                    this.InstructionsCoupon.Visibility = Visibility.Visible;
                    this.Instructions.Visibility = Visibility.Collapsed;
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("AlertStatusTextOff");
                    break;
                case "WaitApproval":   
				 // (+) VSTS 131568
                    this.Instructions.Text = GetPropertyStringValue("Instructions");
                    this.Instructions.Visibility = Visibility.Visible;
                    break;
				 // (-) VSTS 131568
				default:
                    this.Instructions.Property(TextBlock.TextProperty).Clear();
                    break;
            }
        }

        /// <summary>
        /// Updates the PopupTitle text and visibility based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateLeadthruText()
        {            
            switch (this.viewModel.StateParam)
            {
                case "CustomMessage":
				// (+) VSTS 131568
                    this.PopupTitle.Text = GetPropertyStringValue("LeadthruText");
                    this.PopupTitle.Visibility = Visibility.Visible;
                    CmDataCapture.CaptureFormat(CmDataCapture.MaskInfo, "UpdateLeadthruText()");
                    HideAmountDue("LeadthruText");
                    break;
				// (-) VSTS 131568
                case "AssistedTender":
                case "CardMisRead":
                case "CouponNotAllowed":
                case "SavePreferences":
                case "CmDataEntry0":
                case "Processing":
                case "BiometricProcessing":
                case "DataNeededConfirm":
                case "DataNeededMsg":
                case "DataNeededScanCard":
                case "InvalidBarcodeScan":
                case "DataNeededSelection":
                case "UnknownItem":
                case "CardProcessing":
                case "OperatorPasswordStateInvalidPassword":
				case "SecurityAnalysis":
                case "RAPDataNeeded":				
                case "MeijerDigitalVendorCouponChoice":
				case "mPerksApplyPrompt":
                case "RestrictedAgeYN":
                case "TextMsgYN":
				case "InvalidmPerksPINEntry": //SSCOI-47170
				case "mPerksPINEntryLimitReached": //CRD 125377
				case "mPerksPINEntry": //SSCOI-46393
                case "CheckinConfirmation": //SSCOI-44290
                case "PasswordExpired":
				//SSCOI-44290
                case "UsePinPadForWIC": 
                // SSCOI-44661
				case "SwipeCardAsItem"://SSCOI-44592 	
                case "UsePinPad":
                case "RetrievingReceipt": // SSCOI-47555
                case "WaitApproval": //VSTS 131568
                    this.PopupTitle.Text = GetPropertyStringValue("LeadthruText");
                    this.PopupTitle.Visibility = Visibility.Visible;
                    break;
                case "AM_ConfirmAbort":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ConfirmAbortTitle");
                    this.PopupTitle.Visibility = Visibility.Visible;
                    break;
                case "SmAbort":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("TransactionCancelTitle");
                    this.PopupTitle.Visibility = Visibility.Visible;                    
                    break;
                case "AM_ConfirmVoid":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ConfirmVoidTitle");                    
                    this.PopupTitle.Visibility = Visibility.Visible;                    
                    break;
                default:
                    this.PopupTitle.Property(TextBlock.TextProperty).Clear();
                    break;
            }
        }

        /// <summary>
        /// Accepts a parameter change as configured in app.config.
        /// </summary>
        /// <param name="param">Changed parameter.</param>
        private void UpdateStateParam(string param)
        {
            //SSCOI-44290
            Amount1.Visibility = Visibility.Collapsed;
            Amount1LabelText.Visibility = Visibility.Collapsed;
            Amount2.Visibility = Visibility.Collapsed;
            Amount2LabelText.Visibility = Visibility.Collapsed;
            this.ShowBackground = true;
            this.Height = double.NaN;
            this.Width = double.NaN;
            this.ControlsToAllowEnabled = "UNavControl,AccessibilityButton,AssistanceButton,StoreButton8";
			UpdateAmountPaidAmountDue(); // SSCOI-47230
            UpdateInstructions();
            UpdateNextGenData();
            UpdateLeadthruText();
            UpdateStoreLogin();
            UpdateItemRemoved();
            UpdateMessageBox();
            UpdateMidiList1();
            UpdateMidiList2();
            UpdateMidiList3();
            UpdateMidiList4();
            UpdateMidiList5();
            UpdateMidiList6();
            UpdateMidiList7();
            //UpdateScanAnimationStyle(); SSCOI-45545: Meijer does not support void thru scanning
            UpdateStoreModeButtonStyle();
            UpdateYesNoButtonCommandParameter();
            UpdateElements();
			UpdateCMButton1Lg();
            UpdateCMButton2Lg();
            UpdateEchoMessage();
            UpdateGenericOkButton();
			UpdateButtonGoBackForDataNeededContext(); //SSCOI-50040
            UpdateGoBack(); //SSCOI-48082
            PopupAlignment alignment = PopupAlignment.Center;
            this.XMLockScreenPanel.Visibility = Visibility.Collapsed;
            Opacity = viewModel.StateParam.Equals("SignatureProcessing") ? 0.5 : 1.0;
            switch (param)
            {
                case "CollectGiftCard":
                case "FatalError":
                case "Suspend":
                case "VoidApproval":
                case "UnDeActivatedItemApproval":
                case "SecSkipBaggingThreshold":
                case "SecItemRemovedThreshold":
                case "SecBagBoxThreshold":
                case "RAPDataNeeded":
                case "SecNewItemEntry":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("AssistanceNeededTitle");
                    break;
                case "ConfirmEBTAmount":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ConfirmEBTAmountTitle");
                    break;
                case "CouponTooHigh":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("CouponTooHighTitle");
                    break;
                case "CrateableItem":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("PurchaseCrateTitle");
                    break;
                case "VoidNoMatch":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("VoidNoMatchTitle");
                    break;
                case "DropoffCoupons":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("DropoffCouponsTitle");
                    break;
                case "LimitedEBTBenefitOK":
                case "LimitedEBTBenefit":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("InsufficientBalanceTitle");
                    break;
                case "OperatorPasswordStateInvalidPasswordWithFP":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("OperatorPasswordStateInvalidPasswordWithFPTitle");
                    break;
                case "ConfirmDebitAmount":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ConfirmDebitAmountTitle");
                    break;
                case "SecBagAreaBackup":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("SecBagAreaBackupTitle");
                    break;
                case "SignCharge":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("SignChargeTitle");
                    break;
                case "TakeCard":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("TakeCardTitle");
                    break;
                case "CheckBasket":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("CheckBasketTitle");
                    break;
                case "SystemMessageOpPass":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ApprovalNeededTitle");
                    break;
                case "TakeLoyaltyCard":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("TakeLoyaltyCardTitle");
                    break;
                case "WaitForRemoteAssistance":                    
                    this.Instructions.Text = string.Empty; //SSCOI-45479
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("AlertStatusTextOff"); //SSCOI-45479
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("WaitForRemoteAssistanceTitle");
                    break;
                case "ItemNotForSale":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("RecalledItemTitle");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("UnknownItemMessage");
                    break;
                case "UnknownPrice":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("UnknownPriceTitle");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("UnknownPriceMessage");
                    break;
                case "SetAllItemsAside":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("SetAllItemsAsideTitle");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("SetAllItemsAsideMessage");
                    break;
                case "SecOperatorPWStateInvalidPassword":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("IncorrectLoginTitle");
                    break;
                case "TimeRestrictedItem":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("TimeRestrictedItem");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("TimeRestrictedItemMessage");
                    break;
                case "TenderBalance":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("GiftCard");
                    break;
                case "ItemQuantityExceeded":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_496");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_495");
                    break;
                case "SecMisMatchWeight":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("WeightMismatchTitle");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("WeightMismatch");
                    break;
                case "SecBagBoxNotValid":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("VerifyBags");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("VerifyBagsMessage");
                    break;
                case "VoidItem":
                case "VoidItemWithReward":
                case "AM_VoidItem":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("CancelItems");
                    this.ControlsToAllowEnabled = "UNavControl,AccessibilityButton,AssistanceButton,cartControlBox,GoBackBtn,CancelAllBtn,CartReceipt,StoreButton8";
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("VoidItem");
                    this.Instructions.MaxWidth = 475;
                    alignment = PopupAlignment.Left;
                    this.LeftPadding = 450;
                    this.Height = double.NaN;
                    this.Width = double.NaN;
                    break;
                case "ConfirmQuantity":
                case "SellBagsConfirmQuantity":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ConfirmQuantityTitle");
                    break;
                case "RestrictedNotAllowed":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("RestrictedItemTitle");
                    break;
                case "CouponNoMatch":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("CouponNoMatchTitle");
                    break;
                case "ScaleBroken":
                    //+SSCOI-47704
                    this.Amount1LabelText.Visibility = Visibility.Collapsed;
                    this.Amount2LabelText.Visibility = Visibility.Collapsed;
                    this.Amount1.Visibility = Visibility.Collapsed;
                    this.Amount2.Visibility = Visibility.Collapsed;
                    //-SSCOI-47704
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ScaleBrokenTitle");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("ScaleBroken");
                    break;
                case "Continue":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ContinueTitle");
                    this.ControlsToAllowEnabled = "UNavControl,AccessibilityButton,AssistanceButton,UNavButton";
                    break;
                case "WaitApproval":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("AssistanceNeededTitle");
                    this.Height = double.NaN;
                    this.Width = 400;
                    break;
                case "InvalidBarcodeScan":
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("InvalidBarcodeScan");
                    break;
                case "SecUnExpectedIncrease":
                    UpdatePopupTitleAndInstructionText();
                    break;
                case "PickingUpItems":
                case "SecUnexpectedDecrease":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("RemovedItemTitle");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("UnexpectedDecreaseMessage");
                    break;
                case "EnterWeight":
                case "EnterWtForPriceEmbedded":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("EnterWeightTitle");
                    if (itemImage.Source != null)
                    {
                        EnterWeightImage.Visibility = Visibility.Visible;
                        CheckItemDescriptionExists();
                    }
                    else
                    {
                        EnterWeightImage.Visibility = Visibility.Collapsed;
                    }

                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("EnterWeight");
                    break;
                case "RemoveItemFromProduceScale":
                    PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("RemoveItemFromProduceScaleTitle");

                    EnterWeightImage.Visibility = Visibility.Collapsed;
                    ItemImageUPC.Visibility = Visibility.Collapsed;
                    ItemImageName.Visibility = Visibility.Collapsed;

                    Instructions.Property(TextBlock.TextProperty).SetResourceValue("RemoveItemFromProduceScale");
                    CancelAcceptWeight.Visibility = Visibility.Collapsed;
                    break;
                case "Closed":
                    this.PopupTitle.Text = string.Empty;
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("LaneClosedMessage");
                    this.Height = 520;
                    this.Width = 400;
                    this.ShowBackground = false;
                    break;
                case "Startup":
                case "OutOfService":
                    this.PopupTitle.Text = string.Empty;
                    this.Height = 520;
                    this.Width = 400;
                    this.ShowBackground = false;
                    break;
                case "Offline":
                    this.PopupTitle.Text = string.Empty;
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("HostOfflineMessage");
                    break;
                case "Disconnected":
                    this.PopupTitle.Text = string.Empty;
                    this.Instructions.Text = string.Empty;
                    this.Height = 520;
                    this.Width = 400;
                    this.ShowBackground = false;
                    break;
                case "EchoPopup":
                    this.PopupTitle.Text = string.Empty;
                    this.Instructions.Text = string.Empty;
                    this.Height = double.NaN;
                    this.Width = 400;
                    UpdateSMButton8();
                    break;
                case "SignatureProcessing":
                    PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("SignNeeded");
                    Instructions.Property(TextBlock.TextProperty).SetResourceValue("ProcessingLeadthru");
                    Height = double.NaN;
                    Width = 400;
                    LoadingAnimation.Style = this.FindResource("loadingAnimationControlStyle") as Style;
                    break;
                case "CustomMessage":
                    //SSCOI-44290
					// SSCOI-46988
                    UpdateAmountPaidAmountDue(); // SSCOI-47182
                    break;
                case "AssistedTender":
                case "CardMisRead":
                case "WICConfirmation"://SSCOI-44631
                    UpdateAmountPaidAmountDue();       
                    break;
                case "SwipeCardAsItem": //SSCOI-44592
                    //(+) SSCOI-45159
                    this.Instructions.Visibility = Visibility.Collapsed;
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("AlertStatusTextOff");
                    this.PopupTitle.Text = GetPropertyStringValue("Instructions");
                    this.PopupTitle.Visibility = Visibility.Visible;
                    UpdateAmountPaidAmountDue();                
                    break;
                //(-) SSCOI-45159
                // SSCOI-44661
                case "UsePinPad":
                    // For non-SMART WIC stores 
                    Amount1.Visibility = Visibility.Visible;
                    Amount1.Text = GetPropertyStringValue("AmountDue");
                    Amount1LabelText.Visibility = Visibility.Visible;
                    //Amount1LabelText.Text = GetPropertyStringValue("Amount1Label");
					this.Amount1LabelText.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_497");
                    Amount2.Visibility = Visibility.Visible;
                    Amount2.Text = GetPropertyStringValue("AmountPaid");
                    Amount2LabelText.Visibility = Visibility.Visible;
                    //Amount2LabelText.Text = GetPropertyStringValue("Amount2Label");
					this.Amount2LabelText.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_498");
                    this.Instructions.Text = String.Empty;
                    break;
				//SSCOI-45885
				case "ChipCardEntry":
                    // this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9321");
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9021"); // SSCOI-47529
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9369"); // SSCOI-47291
                    break;	
                case "mPerksApplyPrompt":
                    Yes_Button.CommandParameter = "CMButton1MidiList";  // SSCOI-45877
                    No_Button.CommandParameter = "CMButton2MidiList";   // SSCOI-45877
                    break;
                //+CRD 125377
                case "RestrictedAgeYN":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9501");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9502");

                    Yes_Button.Visibility = Visibility.Visible;
                    Yes_Button.CommandParameter = "CMButton1Med";   // SSCOI-45877

                    No_Button.Visibility = Visibility.Visible;
                    No_Button.CommandParameter = "CMButton2Med";   // SSCOI-45877

                    break;
                case "TextMsgYN":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9376"); // SSCOI-47529
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9377"); // SSCOI-47291

                    //this.Yes_Button.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9378");
                    //Yes_Button.Visibility = Visibility.Visible;
                    //Yes_Button.CommandParameter = "CMButton1Med";   // SSCOI-45877
                    Ok_Button.CommandParameter = "CMButton1Med";
                    // No_Button.Visibility = Visibility.Visible;
                    // No_Button.CommandParameter = "CMButton2Med";   // SSCOI-45877
                    // this.Yes_Button.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9379");

                    break;
                //-CRD 125377
                case "MeijerDigitalVendorCouponChoice"://SSCOI-44290
                    UpdateAmountPaidAmountDue();
                    CMButton1LgButton.CommandParameter = "Coupon1";
                    CMButton1LgButton.Visibility = Visibility.Visible;
                    this.CMButton1LgButton.Height = 70;  //SSCOI-47435
                    CMButton2LgButton.CommandParameter = "Coupon2";
                    CMButton2LgButton.Visibility = Visibility.Visible;
                    this.CMButton2LgButton.Height = 70;  //SSCOI-47435
                    this.Instructions.Visibility = Visibility.Collapsed;
                    break;
                case "XMLockScreen":
                case "XMSystemBusyV6HW":
                    this.PopupTitle.Text = string.Empty;
                    this.Instructions.Text = string.Empty;
                    this.XMLockScreenText.Property(TextBlock.TextProperty).SetResourceValue("XM_SystemBusy");
                    this.Width = 500;
                    this.Height = 200;
                    this.ShowBackground = false;
                    this.XMLockScreenPanel.Visibility = Visibility.Visible;
                    break;
                case "XMConfirmLockScreenV6HW":
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("XM_ConfirmAction");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("XM_AreYouSure_LockScreen");
                    break;
                case "TransportItem":
                case "TransportItemWithReward":
                    PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_1011");
                    Instructions.Text = string.Empty;
                    Height = double.NaN;
                    Width = 400;
                    LoadingAnimation.Style = FindResource("loadingAnimationControlStyle") as Style;
                    break;
                case "InvalidmPerksPINEntry": //SSCOI-47170
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9101");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9102");
                    break;					
				case "mPerksPINEntryLimitReached": //CRD 125377 
                    this.PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9399");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9500");
                    break;
            }
            this.PopupTitle.Visibility = this.PopupTitle.Text.Length > 0 ? Visibility.Visible : Visibility.Collapsed;
            this.Instructions.Visibility = this.Instructions.Text.Length > 0 ? Visibility.Visible : Visibility.Collapsed;
            this.Alignment = alignment;
			UpdateItemDescription(); //SSCOI-50040
            FocusHelper.SetInitialFocus(this, viewModel);
        }

        /// <summary>
        /// OkButton click events
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param>
        /// <param name="e">This is a parameter with a type of RoutedEventArgs</param>
        private void OkButton_Click(object sender, RoutedEventArgs e)
        {
            switch (this.viewModel.StateParam)
            {
                case "UnknownItem":
                case "VoidNoMatch":
                case "TenderBalance":
                case "SetAllItemsAside":
                case "InvalidBarcodeScan":
                case "UnknownPrice":
                case "TimeRestrictedItem":
                case "ItemQuantityExceeded":
                case "OperatorPasswordStateInvalidPassword":
                case "CouponTooHigh":
                case "LimitedEBTBenefitOK":
                case "CouponNotAllowed":
                case "CouponNoMatch":
                case "DropoffCoupons":
                case "OperatorPasswordStateInvalidPasswordWithFP":
                case "Suspend":
                case "ConfirmDebitAmount":
                case "ConfirmEBTAmount":
                case "CardMisRead":
                case "RestrictedNotAllowed":                
                case "ItemNotForSale":
                    viewModel.ActionCommand.Execute("OKUnknownItem");
                    break;
                case "CustomMessage":
                    viewModel.ActionCommand.Execute("CMButton1Med");
                    break;
                case "DataNeededConfirm":
                case "DataNeededMsg":
                case "DataNeededScanCard":
                    viewModel.ActionCommand.Execute("DataNeededButton1");
                    break;
                case "CheckinConfirmation": //SSCOI-44290
                    viewModel.ActionCommand.Execute("CheckinConfirmationOK");
                    break;
                case "PasswordExpired": //SSCOI-44616
                    viewModel.ActionCommand.Execute("PasswordExpiredOK");
                    break;
                case "InvalidmPerksPINEntry": //SSCOI-47170
                    viewModel.ActionCommand.Execute("CMButton1MidiList");
                    break;
				case "mPerksPINEntryLimitReached": //CRD 125377
                    viewModel.ActionCommand.Execute("CMButton1MidiList");
                    break;	
				case "TextMsgYN":
                     viewModel.ActionCommand.Execute("CMButton1Med");
                    break;					
                default:
                    break;
            }
        }

        /// <summary>
        /// StoreLogin click events
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param>
        /// <param name="e">This is a parameter with a type of RoutedEventArgs</param>
        private void StoreLogin_Click(object sender, RoutedEventArgs e)
        {
            switch (this.viewModel.StateParam)
            {
                case "WaitApproval":
                case "SignCharge":
                case "Closed":
                case "SystemMessageOpPass":
                    viewModel.ActionCommand.Execute("WaitApprovalStoreLogin");
                    break;
                case "VoidApproval":
                case "SecBagBoxNotValid":
                case "SecUnexpectedDecrease":
                case "SecNewItemEntry":
                case "CollectGiftCard":
                case "FatalError":
                case "RAPDataNeeded":
                case "SecBagBoxThreshold":
                case "SecSkipBaggingThreshold":
                case "UnDeActivatedItemApproval":
                case "SecUnExpectedIncrease":
                case "SecItemRemovedThreshold":
                case "SecMisMatchWeight":
                case "PickingUpItems":
                    viewModel.ActionCommand.Execute("ButtonStoreLogIn");
                    break;
                default:
                    break;
            }
        }

        /// <summary>
        /// Sets the visibility of item image name and UPC.
        /// </summary>
        private void CheckItemDescriptionExists()
        {
            ItemImageUPC.Visibility = String.IsNullOrEmpty(ItemImageName.Text) ? Visibility.Visible : Visibility.Collapsed;
            ItemImageName.Visibility = String.IsNullOrEmpty(ItemImageName.Text) ? Visibility.Collapsed : Visibility.Visible;
        }
		
		/*SSCOI-45545: Meijer does not support void thru scanning
        /// Updates the VoidItemAnimation style and visibility based on the state parameter.
        /// </summary>
        private void UpdateScanAnimationStyle()
        {
            VoidItemAnimation.Visibility = Visibility.Collapsed;
            switch (viewModel.StateParam)
            { 
                case "VoidItem":
                case "VoidItemWithReward":
                    VoidItemAnimation.Style = this.FindResource("can-scanAnimationControlStyle") as Style;
                    VoidItemAnimation.Visibility = Visibility.Visible;
                    break;
                case "AM_VoidItem":
                    VoidItemAnimation.Style = this.FindResource("can-StoreModeScanAnimationControlStyle") as Style;
                    VoidItemAnimation.Visibility = Visibility.Visible;
                    break;
            }
        }
		*/
        /// <summary>
        /// Updates the styles for generic popup action buttons.
        /// </summary>
        private void UpdateStoreModeButtonStyle()
        {
            switch (viewModel.StateParam)
            {
                case "SmAbort":
                case "AM_ConfirmAbort":
                case "AM_ConfirmVoid":
                case "XMConfirmLockScreenV6HW":
                    No_Button.Style = Yes_Button.Style = FindResource("storeModeMainButtonStyle") as Style;
                    break;
                default:
                    No_Button.Style = Yes_Button.Style = FindResource("mainButtonStyle") as Style;
                    break;
            }

            Ok_Button.Style = viewModel.StoreMode ? FindResource("storeModeMainButtonStyle") as Style : FindResource("mainButtonStyle") as Style;
        }

        /// <summary>
        /// Updates the command parameter of Yes and No buttons based on the state parameter
        /// </summary>
        private void UpdateYesNoButtonCommandParameter()
        {
            switch (viewModel.StateParam)
            {
                case "SmAbort":
                    this.Yes_Button.CommandParameter = "SmAbortYesButton";
                    this.No_Button.CommandParameter = "SmAbortNoButton";
                    break;
                case "XMConfirmLockScreenV6HW":
                    this.Yes_Button.CommandParameter = "XMLockScreenConfirmV6HW";
                    this.No_Button.CommandParameter = "XMLockScreenCancelV6HW";
                    break;
				case "CustomMessage":
                    if (GetPropertyStringValue("HighValuePromoChoice").Equals("0"))
                    {
						this.Yes_Button.CommandParameter = "Yes";
                        this.No_Button.CommandParameter = "No";
                    }
                    else
                    {
						this.Yes_Button.CommandParameter = "mPerksApplyPromptYes";
                        this.No_Button.CommandParameter = "mPerksApplyPromptNo";   
                    }
                    break;
                default:
                    this.Yes_Button.CommandParameter = "Yes";
                    this.No_Button.CommandParameter = "No";
                    break;
            }
        }

        /// <summary>
        /// Updates the item description for weight needed popup.
        /// </summary>
        private void UpdateItemDescription()
        {
            if ((viewModel.StateParam.Equals("EnterWeight") ||
                viewModel.StateParam.Equals("EnterWtForPriceEmbedded")) && 
                EnterWeightImage.Visibility == Visibility.Collapsed)
            {
                ItemDescription.Visibility = Visibility.Visible;
            }
            else
            {
                ItemDescription.Visibility = Visibility.Collapsed;
            }
        }
		//-SSCOI-50040
        /// <summary>
        /// Updates the elements based on the state parameter and corresponding property watches.
        /// </summary>
        private void UpdateElements()
        {
            this.Ok_Button.Visibility = this.viewModel.StateParam.Equals("InvalidBarcodeScan") || this.viewModel.StateParam.Equals("UnknownPrice") ||
                this.viewModel.StateParam.Equals("SetAllItemsAside") || this.viewModel.StateParam.Equals("OperatorPasswordStateInvalidPassword") || this.viewModel.StateParam.Equals("UnknownItem") ||
                this.viewModel.StateParam.Equals("ItemNotForSale") || this.viewModel.StateParam.Equals("TimeRestrictedItem") || this.viewModel.StateParam.Equals("DataNeededMsg") ||
                this.viewModel.StateParam.Equals("DataNeededConfirm") || this.viewModel.StateParam.Equals("DataNeededScanCard") || this.viewModel.StateParam.Equals("ItemQuantityExceeded") ||
                this.viewModel.StateParam.Equals("TenderBalance") || this.viewModel.StateParam.Equals("VoidNoMatch") || this.viewModel.StateParam.Equals("CouponTooHigh") ||
                this.viewModel.StateParam.Equals("RestrictedNotAllowed") || this.viewModel.StateParam.Equals("LimitedEBTBenefitOK") || this.viewModel.StateParam.Equals("CustomMessage") ||
                this.viewModel.StateParam.Equals("CouponNotAllowed") || this.viewModel.StateParam.Equals("CouponNoMatch") || this.viewModel.StateParam.Equals("DropoffCoupons") ||
                this.viewModel.StateParam.Equals("OperatorPasswordStateInvalidPasswordWithFP") || this.viewModel.StateParam.Equals("Suspend") || this.viewModel.StateParam.Equals("ConfirmDebitAmount") ||
                this.viewModel.StateParam.Equals("CardMisRead") || this.viewModel.StateParam.Equals("LimitedEBTBenefitOK") || this.viewModel.StateParam.Equals("ConfirmEBTAmount") || this.viewModel.StateParam.Equals("CheckinConfirmation") || 
                this.viewModel.StateParam.Equals("PasswordExpired") || this.viewModel.StateParam.Equals("InvalidmPerksPINEntry") ? Visibility.Visible : Visibility.Collapsed;
            this.YesNoStackPanel.Visibility = paramListForYesNoButtonToShow.Contains(viewModel.StateParam) ? Visibility.Visible : Visibility.Collapsed;
            this.CancelButton.Visibility = this.viewModel.StateParam.Equals("EnterWtForPriceEmbedded") ? Visibility.Visible : Visibility.Collapsed;
            UpdateCancelAcceptVisibility();				
            //this.CancelAcceptWeight.Visibility = this.viewModel.StateParam.Equals("EnterWeight") ? Visibility.Visible : Visibility.Collapsed; //SSCOI-47204
            this.GoBackButton.Visibility = this.viewModel.StateParam.Equals("CheckBasket") || this.viewModel.StateParam.Equals("SecOperatorPWStateInvalidPassword") ||
                this.viewModel.StateParam.Equals("UsePinPadForWIC") || //SSCOI-44290
                this.viewModel.StateParam.Equals("UsePinPad") || // SSCOI-44661
                this.viewModel.StateParam.Equals("WICConfirmation") || //SSCOI-44631
                this.viewModel.StateParam.Equals("SwipeCardAsItem") || //SSCOI-45159
                this.viewModel.StateParam.Equals("AssistedTender") || this.viewModel.StateParam.Equals("ScaleBroken") ? Visibility.Visible : Visibility.Collapsed;
            this.ScaleBrokenControl.Visibility = this.viewModel.StateParam.Equals("ScaleBroken") ? Visibility.Visible : Visibility.Collapsed;
            this.DataNeededScanCardControl.Visibility = this.viewModel.StateParam.Equals("DataNeededScanCard") ? Visibility.Visible : Visibility.Collapsed;
            this.ConfirmEBTDebitAmountButton.Visibility = this.viewModel.StateParam.Equals("ConfirmDebitAmount") || this.viewModel.StateParam.Equals("ConfirmEBTAmount") ? Visibility.Visible : Visibility.Collapsed;
            this.MessageEcho.Visibility = this.viewModel.StateParam.Equals("Startup") || this.viewModel.StateParam.Equals("OutOfService") ? Visibility.Visible : Visibility.Collapsed;

            this.UnexpectedDecreaseControl.Visibility = viewModel.StateParam.Equals("SecUnexpectedDecrease") || viewModel.StateParam.Equals("PickingUpItems") ? Visibility.Visible : Visibility.Collapsed;
            this.UnexpectedIncreaseControl.Visibility = viewModel.StateParam.Equals("SecUnExpectedIncrease") || viewModel.StateParam.Equals("SecBagAreaBackup") ? Visibility.Visible : Visibility.Collapsed;
            this.WeightExpectedLabel.Visibility = this.viewModel.StateParam.Equals("SecBagBoxNotValid") || this.viewModel.StateParam.Equals("SecMisMatchWeight") ? Visibility.Visible : Visibility.Collapsed;
            this.NumberofItemsLabel.Visibility = this.WeightObservedLabel.Visibility = this.viewModel.StateParam.Equals("PickingUpItems") || this.viewModel.StateParam.Equals("SecBagBoxNotValid") ||
                this.viewModel.StateParam.Equals("SecMisMatchWeight") || this.viewModel.StateParam.Equals("SecUnexpectedDecrease") || this.viewModel.StateParam.Equals("SecUnExpectedIncrease") ? Visibility.Visible : Visibility.Collapsed;
            this.GenericButton.Visibility = this.viewModel.StateParam.Equals("DataNeededConfirm") ? Visibility.Visible : Visibility.Collapsed;
            this.DisconnectedText.Visibility = this.LoadingText.Visibility = this.viewModel.StateParam.Equals("Disconnected") ? Visibility.Visible : Visibility.Collapsed;
            this.DataNeededSelectionStackPanel.Visibility = this.viewModel.StateParam.Equals("DataNeededSelection") ? Visibility.Visible : Visibility.Collapsed;

            this.LaneImage.Visibility = this.viewModel.StateParam.Equals("Closed") || this.viewModel.StateParam.Equals("OutOfService") || this.viewModel.StateParam.Equals("Offline") ||
                this.viewModel.StateParam.Equals("Startup") || this.viewModel.StateParam.Equals("Disconnected") ? Visibility.Visible : Visibility.Collapsed;
            this.CardAnimation.Visibility = this.viewModel.StateParam.Equals("SecurityAnalysis") || this.viewModel.StateParam.Equals("CardProcessing") ? Visibility.Visible : Visibility.Collapsed;
            this.LoadingAnimation.Visibility = this.viewModel.StateParam.Equals("Processing") || this.viewModel.StateParam.Equals("RAPDataNeeded") ||
                this.viewModel.StateParam.Equals("SecBagBoxThreshold") || this.viewModel.StateParam.Equals("SecItemRemovedThreshold") ||
                this.viewModel.StateParam.Equals("SecBagBoxNotValid") || this.viewModel.StateParam.Equals("BiometricProcessing") ||
                this.viewModel.StateParam.Equals("WaitForRemoteAssistance") || this.viewModel.StateParam.Equals("VoidApproval") || this.viewModel.StateParam.Equals("SecMisMatchWeight") ||
                this.viewModel.StateParam.Equals("SecSkipBaggingThreshold") || this.viewModel.StateParam.Equals("EchoPopup") || viewModel.StateParam.Equals("SignatureProcessing") ||
                this.viewModel.StateParam.Equals("TransportItem") || this.viewModel.StateParam.Equals("TransportItemWithReward") ? Visibility.Visible : Visibility.Collapsed;
		    //this.VoidItemAnimation.Visibility = this.viewModel.StateParam.Equals("VoidItem") || this.viewModel.StateParam.Equals("VoidItemWithReward") || this.viewModel.StateParam.Equals("AM_VoidItem") ? Visibility.Visible : Visibility.Collapsed; //SSCOI-45545: Meijer does not support void thru scanning
            this.WeightAnimation.Visibility = this.viewModel.StateParam.Equals("EnterWeight") || this.viewModel.StateParam.Equals("EnterWtForPriceEmbedded") ?
                Visibility.Visible : Visibility.Collapsed;
            this.RemoveWeightAnimation.Visibility = this.viewModel.StateParam.Equals("RemoveItemFromProduceScale") ?
                Visibility.Visible : Visibility.Collapsed;
            if (!this.viewModel.StateParam.Equals("EnterWeight") && !this.viewModel.StateParam.Equals("EnterWtForPriceEmbedded"))
            {
                this.EnterWeightImage.Visibility = Visibility.Collapsed;
            }
            //SSCOI-45159 - added condition for SwipeCardAsItem
            if (this.viewModel.StateParam.Equals("SwipeCardAsItem") || this.viewModel.StateParam.Equals("UsePinPadForWIC") || this.viewModel.StateParam.Equals("UsePinPad"))//SSCOI-44290 SSCOI-44661
            {
                this.MeijerCardImage.Visibility = Visibility.Visible;
            }
            else
            {
                this.MeijerCardImage.Visibility = Visibility.Collapsed;
            }
        }
		
		// This is a helper function to update UpdateAmountPaidAmountDue visibility
		private void UpdateAmountPaidAmountDue()
        {
            //+SSCOI-47704
            this.Amount1LabelText.Visibility = Visibility.Collapsed;
            this.Amount2LabelText.Visibility = Visibility.Collapsed;
            this.Amount1.Visibility = Visibility.Collapsed;
            this.Amount2.Visibility = Visibility.Collapsed;
            //-SSCOI-47704

            if (GetPropertyBoolValue("Amount1Shown"))
            {
                this.Amount1LabelText.Visibility = Visibility.Visible;
				this.Amount1LabelText.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_497");//SSCOI-47833
                //this.Amount1LabelText.Text = GetPropertyStringValue("Amount1Label"); //SSCOI-47833
                this.Amount1.Text = GetPropertyStringValue("AmountDue");
                this.Amount1.Visibility = Visibility.Visible;
				// Start CRD 125377
                //if (!GetPropertyStringValue("ShopAndDropEnabled").Equals("0"))
                if (GetPropertyStringValue("TransactionMode").Equals("1"))
                {
                    this.Amount1LabelText.Visibility = Visibility.Collapsed;
                    this.Amount1.Visibility = Visibility.Collapsed;
                }
                // End CRD 125377
            }
            if (GetPropertyBoolValue("Amount2Shown"))
            {
                this.Amount2LabelText.Visibility = Visibility.Visible;
				this.Amount2LabelText.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_498"); //SSCOI-47188
                //this.Amount2LabelText.Text = GetPropertyStringValue("Amount2Label"); //SSCOI-47833
                this.Amount2.Text = GetPropertyStringValue("AmountPaid");
                this.Amount2.Visibility = Visibility.Visible;
				// Start CRD 125377
                //if (!GetPropertyStringValue("ShopAndDropEnabled").Equals("0"))
                if (GetPropertyStringValue("TransactionMode").Equals("1"))
                {
                    this.Amount2LabelText.Visibility = Visibility.Collapsed;
                    this.Amount2.Visibility = Visibility.Collapsed;
                }
                // End CRD 125377
            }
            HideAmountDue("Instructions");  //VSTS 131568
        }

		//+SSCOI-50040
        /// <summary>
        /// This method will update the visibility and the text of the go back button for Data Needed contexts
        /// </summary>
        private void UpdateButtonGoBackForDataNeededContext()
        {
            switch (viewModel.StateParam)
            {
                case "CmDataEntry0":
                case "DataNeededMsg":
                case "DataNeededConfirm":
                case "DataNeededScanCard":
                case "DataNeededSelection":
                    GoBackButton.Text = GetPropertyStringValue("ButtonGoBack");
                    GoBackButton.Visibility = GetPropertyBoolValue("ButtonGoBackShown") ? Visibility.Visible : Visibility.Collapsed;
                    break;
            }
        }
		//-SSCOI-50040
    
		/// <summary>
        /// Update the popup title and instruction text when TAB is configured.
        /// </summary>
        private void UpdatePopupTitleAndInstructionText()
        {
            if (GetStringPropertyBoolValue("NextGenUITABConfigured"))
            {
                PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_1021");
                Instructions.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_988");
            }
            else
            {
                PopupTitle.Property(TextBlock.TextProperty).SetResourceValue("SecUnExpectedIncreaseTitle");
                Instructions.Property(TextBlock.TextProperty).SetResourceValue("UnexpectedIncreaseMessage");
            }
        }		
		
		//+SSCOI-48082
		private void UpdateGoBack()
        {
            switch (this.viewModel.StateParam)
            {
                case "ChipCardEntry":
                    this.GoBackButton.Visibility = GetPropertyBoolValue("ButtonGoBackShown") ? Visibility.Visible : Visibility.Collapsed;
                    this.GoBackButton.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_67");
                    break;
            }   
        }
    		
		//-SSCOI-48082
        private void UpdateCMButton1Lg()//SSCOI-44290
        {
            switch (this.viewModel.StateParam)
            {
                case "MeijerDigitalVendorCouponChoice":				
                    this.CMButton1LgButton.Text = GetPropertyStringValue("CMButton1LgText");
                    this.CMButton1LgButton.Visibility = Visibility.Visible;
                    this.DataNeededSelectionStackPanel.Visibility = Visibility.Visible;
                    break;
            }
        }
        private void UpdateCMButton2Lg()//SSCOI-44290
        {
            switch (this.viewModel.StateParam)
            {
                case "MeijerDigitalVendorCouponChoice":
                    this.CMButton2LgButton.Text = GetPropertyStringValue("Coupon2Text");
                    this.CMButton2LgButton.Visibility = Visibility.Visible;
                    this.DataNeededSelectionStackPanel.Visibility = Visibility.Visible;
                    break;
            }
        }
    }
}
