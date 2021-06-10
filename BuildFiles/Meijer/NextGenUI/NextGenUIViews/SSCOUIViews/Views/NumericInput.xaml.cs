// <copyright file="NumericInput.xaml.cs" company="NCR Corporation">
//     Copyright (c) NCR Corporation. All rights reserved.
// </copyright>
namespace SSCOUIViews.Views
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Globalization;
    using System.Text.RegularExpressions;
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
    using SSCOControls.Helpers.KeyboardSimulatorComponents;

    /// <summary>
    /// Interaction logic for NumericInput.xaml
    /// </summary>
    public partial class NumericInput : PopupView
    {
        /// <summary>
        ///  Initializes a new instance of the NumericInput class.
        /// </summary>
        /// <param name="viewModel">Parameter that inherits the interface of MainViewModel</param>
        public NumericInput(IMainViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
        }

        /// <summary>
        /// Method for state parameter changes
        /// </summary>
        /// <param name="param">State change string parameter</param>
        public override void OnStateParamChanged(String param)
        {
            UpdateInstructions();
            UpdateCurrentItem();
            UpdateLeadthruText();
            UpdateGoBackText();
            UpdateGoBackStyle();
            UpdateTitleText();
			UpdateAmountDueAmountPaid(); //SSCOI-48303
            Amount1.Visibility = Visibility.Collapsed;
            Amount2.Visibility = Visibility.Collapsed;
            Amount1LabelText.Visibility = Visibility.Collapsed;
            Amount2LabelText.Visibility = Visibility.Collapsed;
            mPerksTitleControl.Visibility = Visibility.Collapsed;
            mPerksInstructions.Visibility = Visibility.Collapsed;

            switch (param)
            {
                case "EnterId":
                    ShowImageOnPopup(false);
                    this.TitleControl.Text = GetPropertyStringValue("LeadthruText");
                    this.TitleControl.Visibility = Visibility.Visible; //SSCOI-46863

                    //SSCOI-48097
                    //this.GoBackButton.Visibility = viewModel.BackgroundStateParam.Equals("XMConfirmAction") ? Visibility.Collapsed : Visibility.Visible;

                    break;
                case "EnterQuantity":
                    if (CheckItemImageExists())
                    {
                        ShowImageOnPopup(true);
                        checkItemDescriptionExists();
                    }
                    else
                    {
                        ShowImageOnPopup(false);
                    }
                    TitleControl.Visibility = Visibility.Visible;
                    this.TitleControl.Property(TextBlock.TextProperty).SetResourceValue("EnterQuantityTitle");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("EnterQuantity");
                    //this.GoBackButton.Visibility = Visibility.Collapsed; //SSCOI-48634
                    break;

                case "SellBags":
                    ShowImageOnPopup(false);
                    TitleControl.Text = GetPropertyStringValue("LeadthruText");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("SellBags");
                    break;

                case "CmDataEntryWithKeyBoard":
                    ShowImageOnPopup(false);
                    // +SSCOI-47878
                    if (!GetPropertyStringValue("GiftPhonePricePrompt").Equals("0"))
                    {
                        this.Instructions.Visibility = Visibility.Collapsed;
                        this.UpdateAmountDueAmountPaid();
                    }
                    // -SSCOI-47878
                    this.TitleControl.Text = GetPropertyStringValue("LeadthruText");
                    break;

                case "EnterCouponValue":
                    ShowImageOnPopup(false);
                    this.TitleControl.Property(TextBlock.TextProperty).SetResourceValue("EnterCouponValueTitle");
                    this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("EnterCouponValue");
                    break;
                //SSCOI-44290
                case "mPerksEntry":
                    mPerksTitleControl.Visibility = Visibility.Visible;
                    mPerksInstructions.Visibility = Visibility.Visible;
                    mPerksTitleControl.Text = GetPropertyStringValue("LeadthruText");
                    mPerksInstructions.Text = GetPropertyStringValue("MessageBoxNoEcho");
                    GoBackButton.Visibility = Visibility.Visible;
                    GoBackButton.Text = GetPropertyStringValue("ButtonGoBack");
                    Grid_WithImage.Visibility = Visibility.Collapsed;
                    //+SSCOI-45676
                    //Amount1.Visibility = Visibility.Visible;
                    //Amount1.Text = GetPropertyStringValue("AmountDue");
                    //Amount2.Visibility = Visibility.Visible;
                    //Amount2.Text = GetPropertyStringValue("AmountPaid");
                    //Amount1LabelText.Visibility = Visibility.Visible;
                    //Amount2LabelText.Visibility = Visibility.Visible;
                    //Amount1LabelText.Text = GetPropertyStringValue("Amount1Label");
                    //Amount2LabelText.Text = GetPropertyStringValue("Amount2Label");
                    //-SSCOI-45676
                    TitleControl.Visibility = Visibility.Collapsed;
                    Instructions.Visibility = Visibility.Collapsed;
                    break;
                case "XMCashierIDV6HW":
                case "XMCashierPasswordV6HW":
                    ShowImageOnPopup(false);
                    break;
				//SSCOI-50040
                case "KeyInCode":
                    ShowImageOnPopup(false);
                    TitleControl.Text = GetPropertyStringValue("LeadthruText");
                    Instructions.Property(TextBlock.TextProperty).SetResourceValue("KeyInCodeLeadthru");
                    break;					
                default:
                    break;
            }

        }

        /// <summary>
        /// Method for property change
        /// </summary>
        /// <param name="name">string type parameter for property change</param>
        /// <param name="value">object type parameter</param>
        public override void OnPropertyChanged(string name, object value)
        {
            switch (name)
            {
                case "NextGenData":
                    UpdateInstructions();
                    UpdateGoBackButtonVisibility();
                    break;
                case "MessageBoxNoEcho":
                    UpdateInstructions();
                    break;
                case "CurrentItem":
                    UpdateCurrentItem();
                    break;
                case "LeadthruText":
                    UpdateLeadthruText();
                    break;
                case "ButtonGoBack":
                    UpdateGoBackText();
                    break;
                case "ButtonGoBackShown":
                case "XMButton8V6HWShown":
                    UpdateGoBackButtonVisibility();
                    break;
                case "Title1Text":
                    UpdateTitleText();
                    break;
                case "UNav":
                    FocusHelper.SetInitialFocus(this, viewModel);
                    break;
                default:
                    break;
 	
            }
        }

        /// <summary>
        /// Void method for updating CurrentItem
        /// </summary>
        private void UpdateCurrentItem()
        {
            switch (viewModel.StateParam)
            {
                case "EnterQuantity":
                    if (Grid_WithImage.Visibility == Visibility.Collapsed && CheckItemImageExists())
                    {
                        ShowImageOnPopup(true);
                        checkItemDescriptionExists();
                    }
                    break;

                default:
                    this.TitleControl.Property(TextBlock.TextProperty).Clear();
                    break;
            }
        }

        /// <summary>
        /// Void method for updating LeadThruText
        /// </summary>
        private void UpdateLeadthruText()
        {
            switch (this.viewModel.StateParam)
            {
                case "XMCashierIDV6HW":
                    this.Instructions.Text = GetPropertyStringValue("LeadthruText");
                    break;
				case "CmDataEntryWithKeyBoard": //SSCOI-46718
                    this.TitleControl.Text = GetPropertyStringValue("LeadthruText");
                    this.TitleControl.Visibility = Visibility.Visible; 
                    break;	
                default:
                    TitleControl.Text = GetPropertyStringValue("LeadthruText");
                    break;
            }
        }

        /// <summary>
        /// Void method for updating GoBackButton Text
        /// </summary>
        private void UpdateGoBackText()
        {
            switch (viewModel.StateParam)
            {
                case "CmDataEntryWithKeyBoard":
                    GoBackButton.Text = GetPropertyStringValue("ButtonGoBack");
                    break;
                default:
                    GoBackButton.Property(ImageButton.TextProperty).SetResourceValue("GoBack");
                    break;
            }
        }

        /// <summary>
        /// Void method for updating GoBackButton's Style
        /// </summary>
        private void UpdateGoBackStyle()
        {
            switch (this.viewModel.StateParam)
            {
                case "KeyInCode":
                    GoBackButton.Style = this.FindResource("buttonGoBackStyle") as Style;
                    GoBackButton.HorizontalAlignment = HorizontalAlignment.Center;
                    break;
                default:
                    GoBackButton.Style = this.FindResource("numericGoBackButtonStyle") as Style;
                    break;
            }
        }

        /// <summary>
        /// Void method for updating Title Text
        /// </summary>
        private void UpdateTitleText()
        {
            this.TitleControl.Text = GetPropertyStringValue("Title1Text");
        }

        /// <summary>
        /// Void method for updating Instructions
        /// </summary>
        private void UpdateInstructions()
        {
			 NumericKeypadVisibility();
            switch (this.viewModel.StateParam)
            {
                case "EnterId":
                    if (GetPropertyStringValue("NextGenData").Equals("EnterPassword"))
                    {
                        this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("EnterPassword");
                        FocusHelper.SetInitialFocus(this, viewModel);
                    }
                    else if (GetPropertyStringValue("NextGenData").Equals("EnterID"))
                    {
                        if (!GetPropertyBoolValue("NumericP1Shown"))
                            this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("ScotApp_9188");
                        else
                        this.Instructions.Property(TextBlock.TextProperty).SetResourceValue("EnterId");
                    }
                    break;

                case "CmDataEntryWithKeyBoard":
                    this.Instructions.Text = GetPropertyStringValue("MessageBoxNoEcho");
                    break;
                //SSCOI-44290
                case "mPerksEntry":
                    Instructions.Text = GetPropertyStringValue("MessageBoxNoEcho");
					this.NumericKeyboard.InputTextBox.PasswordMode = false; //SSCOI-46400
                    break;
                case "XMCashierIDV6HW":
                case "XMCashierPasswordV6HW":
                    this.Instructions.Text = GetPropertyStringValue("LeadthruText");
                    break;
            }
        }

        /// <summary>
        /// Click event method of GoBackButton
        /// </summary>
        /// <param name="sender">object type parameter</param>
        /// <param name="e">RoutedEventArgs type parameter</param>
        private void GoBackButton_Click(object sender, RoutedEventArgs e)
        {
            string btnExecute = "ButtonGoBack";

            switch (viewModel.StateParam)
            {
                case "EnterCouponValue":
                    if (viewModel.BackgroundStateParam.Equals("Search"))
                    {
                        btnExecute = "SearchPopupGoBack";
                    }

                    break;
                case "XMCashierIDV6HW":
                case "XMCashierPasswordV6HW":
                    btnExecute = "XMButton8V6HW";
                    break;
            }

            viewModel.ActionCommand.Execute(btnExecute);
        }

        /// <summary>
        /// Void method for showing ImageOnPopup
        /// </summary>
        /// <param name="show">boolean type parameter</param>
        private void ShowImageOnPopup(bool show)
        {
            if (show)
            {
                Instructions.Visibility = Visibility.Collapsed;
                Grid_WithImage.Visibility = Visibility.Visible;
            }
            else
            {
                Instructions.Visibility = Visibility.Visible;
                Grid_WithImage.Visibility = Visibility.Collapsed;
                if ((viewModel.StateParam.Equals("XMCashierIDV6HW") || viewModel.StateParam.Equals("XMCashierPasswordV6HW")))
                {
                    GoBackButton.Visibility = GetPropertyBoolValue("XMButton8Shown") ? Visibility.Visible : Visibility.Hidden;
                }
                else
                {
                    GoBackButton.Visibility = GetPropertyBoolValue("ButtonGoBackShown")? Visibility.Visible : Visibility.Collapsed; // SSCOI-45156
                }
            }
            UpdateGoBackButtonVisibility();
        }

        /// <summary>
        /// Void method for checking ItemDescription if exists
        /// </summary>
        private void checkItemDescriptionExists()
        {
            if (String.IsNullOrEmpty(ItemImageName.Text))
            {
                ItemImageUPC.Visibility = Visibility.Visible;
                ItemImageName.Visibility = Visibility.Collapsed;
            }
            else
            {
                ItemImageUPC.Visibility = Visibility.Collapsed;
                ItemImageName.Visibility = Visibility.Visible;
            }
        }

        /// <summary>
        /// Boolean method for checking ItemImage if exists
        /// </summary>
        /// <returns>Returns true if it exist then false if otherwise</returns>
        private bool CheckItemImageExists()
        {
            if (itemImage.Source != null)
            {
                return true;
            }
            return false;
        }

        /// <summary>
        /// Updates the ApplyChanges button visibility based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateGoBackButtonVisibility()
        {
            Visibility goBackButtonVisibility = Visibility.Visible;
            switch (viewModel.StateParam)
            {
                case "EnterQuantity":
                    goBackButtonVisibility = CheckItemImageExists() ? Visibility.Collapsed : Visibility.Visible;
					if(CheckItemImageExists())
					{
						GoBackButton.Visibility = goBackButtonVisibility;//SSCOI-47585	
					}
                    break;
                case "EnterId":
				case "CmDataEntryWithKeyBoard": // SSCOI-46523
                    goBackButtonVisibility = GetPropertyBoolValue("ButtonGoBackShown") ? Visibility.Visible : Visibility.Hidden;
                    break;
                case "XMCashierIDV6HW":
                case "XMCashierPasswordV6HW":
                    goBackButtonVisibility = GetPropertyBoolValue("XMButton8V6HWShown") ? Visibility.Visible : Visibility.Hidden;
                    break;
                default:
                    break;
            }
            GoBackButton.Visibility = goBackButtonVisibility; //SSCOI-46722
        }

        private void NumericKeypadVisibility()
        {
            if (!GetPropertyBoolValue("NumericP1Shown") && (viewModel.StateParam.Equals("EnterId"))) //SSCOI-45568
            {
                NumericKey.Visibility = Visibility.Collapsed;
                this.MinHeight = 150;
            }
            else
            {
                NumericKey.Visibility = Visibility.Visible;
            }
        }

        /// <summary>
        /// Updates the Amount Due and Amount Paid if it exists
        /// </summary>
        private void UpdateAmountDueAmountPaid()
        {
            if ( GetPropertyBoolValue("Amount1Shown"))
            {
                this.Amount1LabelText.Text = GetPropertyStringValue("Amount1Label");
                this.Amount1LabelText.Visibility = Visibility.Visible;
                this.Amount1.Text = GetPropertyStringValue("AmountDue");
                this.Amount1.Visibility = Visibility.Visible;
            }
            if ( GetPropertyBoolValue("Amount2Shown"))
            {
                this.Amount2LabelText.Text = GetPropertyStringValue("Amount2Label");
                this.Amount2LabelText.Visibility = Visibility.Visible;
                this.Amount2.Text = GetPropertyStringValue("AmountPaid");
                this.Amount2.Visibility = Visibility.Visible;
            }
        }
    }
}
