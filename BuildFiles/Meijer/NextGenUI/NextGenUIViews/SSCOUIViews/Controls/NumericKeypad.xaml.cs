// <copyright file="NumericKeypad.xaml.cs" company="NCR">
//     Copyright 2016 NCR. All rights reserved.
// </copyright>

namespace SSCOUIViews.Controls
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Globalization;
    using System.Text.RegularExpressions;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Controls.Primitives;
    using System.Windows.Data;
    using System.Windows.Documents;
    using System.Windows.Input;
    using System.Windows.Media;
    using System.Windows.Media.Imaging;
    using System.Windows.Navigation;
    using System.Windows.Shapes;
    using RPSWNET;
    using SSCOControls;
    using SSCOControls.Helpers.KeyboardSimulatorComponents;
    using SSCOUIModels;
    using SSCOUIModels.Commands;
    using SSCOUIModels.Controls;
    using SSCOUIModels.Helpers;
    using SSCOUIModels.Models;

    /// <summary>
    /// Interaction logic for NumericKeypad.xaml
    /// </summary>
    public partial class NumericKeypad : Grid
    {
        /// <summary>
        /// Dependency property for AutoEnterDecimalPointProperty.
        /// </summary>
        public static readonly DependencyProperty AutoEnterDecimalPointProperty =
            DependencyProperty.Register("AutoEnterDecimalPoint", typeof(bool), typeof(NumericKeypad), new PropertyMetadata(false));

        /// <summary>
        /// Dependency property for AutoEnterDecimalPointDigitsProperty.
        /// </summary>
        public static readonly DependencyProperty AutoEnterDecimalPointDigitsProperty =
            DependencyProperty.Register("AutoEnterDecimalPointDigits", typeof(int), typeof(NumericKeypad), new PropertyMetadata(2));

        /// <summary>
        /// Dependency property for TelephoneType.
        /// </summary>
        public static DependencyProperty TelephoneTypeProperty = DependencyProperty.Register("TelephoneType", typeof(bool), typeof(NumericKeypad));

        /// <summary>
        /// Dependency property for Background view mode.
        /// </summary>
        public static DependencyProperty BackgroundViewModeProperty = DependencyProperty.Register("BackgroundViewMode", typeof(bool), typeof(NumericKeypad));

        /// <summary>
        /// Dependency property for AllowZeroInput.
        /// </summary>
        public static DependencyProperty AllowZeroInputProperty = DependencyProperty.Register("AllowZeroInput", typeof(bool), typeof(NumericKeypad));

        /// <summary>
        /// Price text block.
        /// </summary>
        private static PriceTextBlock _priceTextBlock = new PriceTextBlock();

        /// <summary>
        /// View model
        /// </summary>
        private IMainViewModel _viewModel;

        /// <summary>
        /// Input value.
        /// </summary>
        private string _inputValue = string.Empty;

        /// <summary>
        /// Min input size.
        /// </summary>
        private int _minInput = 1;

        /// <summary>
        /// Initializes a new instance of the <see cref="NumericKeypad" /> class.
        /// </summary>
        public NumericKeypad()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Gets or sets a value indicating whether property used to determine if decimal point is automatically entered.
        /// Should be used for non-currency style entries only (example, enter weight).
        /// </summary>
        public bool AutoEnterDecimalPoint
        {
            get
            {
                return Convert.ToBoolean(GetValue(AutoEnterDecimalPointProperty));
            }

            set
            {
                SetValue(AutoEnterDecimalPointProperty, value);
            }
        }

        /// <summary>
        /// Gets or sets the number of digits after the decimal point.
        /// </summary>
        public int AutoEnterDecimalPointDigits
        {
            get
            {
                return Convert.ToInt32(GetValue(AutoEnterDecimalPointDigitsProperty));
            }

            set
            {
                SetValue(AutoEnterDecimalPointDigitsProperty, value < 1 ? 1 : value);
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether TelephoneType is true.
        /// If true, keypad number starts with 1 2 3
        /// else, keypad number starts with 7 8 9
        /// </summary>
        /// <returns>Whether TelephoneType is true.</returns>
        public bool TelephoneType
        {
            get
            {
                return Convert.ToBoolean(GetValue(TelephoneTypeProperty));
            }

            set
            {
                SetValue(TelephoneTypeProperty, value);
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether background view mode is on.
        /// </summary>
        public bool BackgroundViewMode
        {
            get
            {
                return Convert.ToBoolean(GetValue(BackgroundViewModeProperty));
            }

            set
            {
                SetValue(BackgroundViewModeProperty, value);
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether to allow zero input.
        /// </summary>
        public bool AllowZeroInput
        {
            get
            {
                return Convert.ToBoolean(GetValue(AllowZeroInputProperty));
            }

            set
            {
                SetValue(AllowZeroInputProperty, value);
            }
        }

        /// <summary>
        /// Update elements of the specified state.
        /// </summary>
        /// <param name="stateParam">State parameter</param>
        public void UpdateStateElements(string stateParam)
        {
            switch (stateParam)
            {
                case "SmCashierPassword":
                case "SmDataEntryNumericSmall":
                case "SmDataEntryWithListBG":
                case "SmEnterBirthdate":
                case "SmLoadLift":
                case "SmmKeyInItemCode":
                case "SmmKeyInWtTol":
                case "AM_EnterCouponValue":
                case "AM_EnterQuantity":
                case "AM_KeyInCode":
                case "AM_KeyInPrice":
                case "AM_KeyInQuantity":
                    if (BackgroundViewMode)
                    {
                        MainKeypadGrid.RowDefinitions[0].Height = new GridLength(48);
                        WaterMarkTextBoxBorder.Width = 286;
                        WaterMarkTextBoxBorder.Height = 48;
                        WaterMarkTextBoxBorder.Margin = new Thickness(0);
                        InputTextBox.Width = 286;
                        InputTextBox.Height = 48;
                        InputTextBox.Padding = new Thickness(0, 0, 0, 0);
                        InputTextBox.Watermark = string.Empty;
                        InputTextBox.TextAlignment = TextAlignment.Left;
                        KeypadGrid.Margin = new Thickness(31, 19, 30, 0);
                        KeypadGrid.Width = 225;
                        KeypadButton0.Width = 140;
                        NumericBackspaceButton.Width = 65;
                        firstRow.Width = 225;
                        secondRow.Width = 225;
                        thirdRow.Width = 225;
                        lastRow.Width = 225;
                        OKButton.Width = 215;
                        OKButton.Property(ImageButton.TextProperty).SetResourceValue("Enter");
                        UpdateAdditionalKeys();
                        UpdateStyles(true);
                    }

                    break;
                case "CmDataEntryWithKeyBoard":
                case "EnterCouponValue":
                case "EnterId":
                case "EnterQuantity":
                case "SellBags":
                case "KeyInCode":
                case "mPerksEntry":				
                    if (!BackgroundViewMode)
                    {
                        MainKeypadGrid.RowDefinitions[0].Height = new GridLength(64);
                        WaterMarkTextBoxBorder.Width = 236;
                        WaterMarkTextBoxBorder.Height = 54;
                        WaterMarkTextBoxBorder.Margin = new Thickness(5, 0, 5, 5);
                        InputTextBox.Width = 236;
                        InputTextBox.Height = 54;
                        InputTextBox.Padding = new Thickness(12, 0, 0, 0);
                        InputTextBox.Watermark = string.Empty;
                        InputTextBox.TextAlignment = TextAlignment.Left;
                        KeypadGrid.Margin = new Thickness(0);
                        KeypadGrid.Width = 246;
                        KeypadButton0.Width = 72;
                        NumericBackspaceButton.Width = 72;
                        firstRow.Width = 246;
                        secondRow.Width = 246;
                        thirdRow.Width = 246;
                        lastRow.Width = 246;
                        OKButton.Width = 72;
                    OKButton.Property(ImageButton.TextProperty).SetResourceValue("Enter"); //SSCOI-46449
                        DecimalMarkKey.Visibility = Visibility.Collapsed;
                        MinusKey.Visibility = Visibility.Collapsed;
                        UpdateStyles(_viewModel.StoreMode);
                    }

                    break;
                case "CMCashAddV6HW":
                case "CMCashRemoveV6HW":
                    if (BackgroundViewMode)
                    {
                        MainKeypadGrid.RowDefinitions[0].Height = new GridLength(64);
                        WaterMarkTextBoxBorder.Width = 236;
                        WaterMarkTextBoxBorder.Height = 54;
                        WaterMarkTextBoxBorder.Margin = new Thickness(5, 0, 5, 5);
                        InputTextBox.Width = 236;
                        InputTextBox.Height = 54;
                        InputTextBox.Padding = new Thickness(12, 0, 0, 0);
                        InputTextBox.Watermark = string.Empty;
                        InputTextBox.TextAlignment = TextAlignment.Left;
                        KeypadGrid.Margin = new Thickness(0);
                        KeypadGrid.Width = 246;
                        KeypadButton0.Width = 72;
                        NumericBackspaceButton.Width = 72;
                        firstRow.Width = 246;
                        secondRow.Width = 246;
                        thirdRow.Width = 246;
                        lastRow.Width = 246;
                        OKButton.Width = 72;
                        OKButton.Property(ImageButton.TextProperty).SetResourceValue("XM_OKV6HW");
                        DecimalMarkKey.Visibility = Visibility.Collapsed;
                        MinusKey.Visibility = Visibility.Collapsed;
                        UpdateStyles(true);
                    }

                    break;
                case "XMCashRemove":
                case "XMCashRemoveBNR":
                case "XMCashRemoveGlory":
                case "XMCashReplenish":
                    if (BackgroundViewMode)
                    {
                        MainKeypadGrid.RowDefinitions[0].Height = new GridLength(48);
                        WaterMarkTextBoxBorder.Width = 286;
                        WaterMarkTextBoxBorder.Height = 48;
                        WaterMarkTextBoxBorder.Margin = new Thickness(0);
                        InputTextBox.Width = 286;
                        InputTextBox.Height = 48;
                        InputTextBox.Padding = new Thickness(0, 0, 0, 0);
                        InputTextBox.Watermark = string.Empty;
                        InputTextBox.TextAlignment = TextAlignment.Left;
                        KeypadGrid.RowDefinitions[0].Height = new GridLength(60);
                        KeypadGrid.RowDefinitions[1].Height = new GridLength(60);
                        KeypadGrid.RowDefinitions[2].Height = new GridLength(60);
                        KeypadGrid.Margin = new Thickness(31, 5, 30, 0);
                        KeypadGrid.Width = 225;
                        KeypadButton0.Width = 140;
                        NumericBackspaceButton.Width = 65;
                        firstRow.Width = 225;
                        secondRow.Width = 225;
                        thirdRow.Width = 225;
                        lastRow.Width = 225;
                        OKButton.Width = 215;
                        OKButton.Property(ImageButton.TextProperty).SetResourceValue("Enter");
                        UpdateAdditionalKeys();
                        UpdateStyles(true);
                    }

                    break;
                case "XMCashierID":
                case "XMCashierPassword":
                case "XMCashierIDV6HW":
                case "XMCashierPasswordV6HW":
                    if (!BackgroundViewMode)
                    {
                        MainKeypadGrid.RowDefinitions[0].Height = new GridLength(64);
                        WaterMarkTextBoxBorder.Width = 236;
                        WaterMarkTextBoxBorder.Height = 54;
                        WaterMarkTextBoxBorder.Margin = new Thickness(5, 0, 5, 5);
                        InputTextBox.Width = 236;
                        InputTextBox.Height = 54;
                        InputTextBox.Padding = new Thickness(12, 0, 0, 0);
                        InputTextBox.Watermark = string.Empty;
                        InputTextBox.TextAlignment = TextAlignment.Left;
                        KeypadGrid.Margin = new Thickness(0);
                        KeypadGrid.Width = 246;
                        KeypadButton0.Width = 153;
                        NumericBackspaceButton.Width = 72;
                        firstRow.Width = 246;
                        secondRow.Width = 246;
                        thirdRow.Width = 246;
                        lastRow.Width = 246;
                        OKButton.Width = 236;
                        OKButton.Property(ImageButton.TextProperty).SetResourceValue("XM_Enter");
                        UpdateAdditionalKeys();
                        UpdateStyles(true);
                    }

                    break;
                case "ProduceFavorites":
                case "MultiSelectProduceFavorites":
                    if (BackgroundViewMode)
                    {
                        MainKeypadGrid.RowDefinitions[0].Height = new GridLength(64);
                        WaterMarkTextBoxBorder.Width = 800;
                        WaterMarkTextBoxBorder.Height = 54;
                        InputTextBox.Width = 800;
                        InputTextBox.Height = 54;
                        InputTextBox.Padding = new Thickness(56, 0, 0, 0);
                        InputTextBox.TextAlignment = TextAlignment.Center;
                        InputTextBox.Property(WatermarkTextBox.WatermarkProperty).SetResourceValue("SearchItemCodeWatermark");
                        KeypadGrid.Margin = new Thickness(0);
                        KeypadGrid.Width = 400;
                        KeypadButton0.Width = 72;
                        NumericBackspaceButton.Width = 72;
                        firstRow.Width = 246;
                        secondRow.Width = 246;
                        thirdRow.Width = 246;
                        lastRow.Width = 246;
                        OKButton.Width = 72;
                        OKButton.Property(ImageButton.TextProperty).SetResourceValue("OK");
                        UpdateAdditionalKeys();
                        UpdateStyles(false);
                    }

                    break;
            }
        }

        /// <summary>
        /// Translates string into Key enum value.
        /// </summary>
        /// <param name="key">input string</param>
        /// <returns>Key code</returns>
        public Key KeyCode(string key)
        {
            Key finalKey = Key.None;
            switch (key)
            {
                case ".":
                    key = "DECIMAL";
                    break;
                case "-":
                    key = "MINUS";
                    break;
            }

            try
            {
                finalKey = (Key)new KeyConverter().ConvertFromString(key);
            }
            catch (Exception)
            {
                finalKey = Key.None;
            }

            return finalKey;
        }

        /// <summary>
        /// Update buttons.
        /// </summary>
        /// <param name="storeMode">Store mode</param>
        /// <param name="buttonGrid">Button grid</param>
        /// <param name="buttonStyle">Button style</param>
        public void UpdateButtons(bool storeMode, UniformGrid buttonGrid, Style buttonStyle)
        {
            foreach (ImageButton button in UIControlFinder.FindVisualChildren<ImageButton>(buttonGrid))
            {
                button.Style = buttonStyle;
            }
        }

        /// <summary>
        /// Event handler for data context changed
        /// </summary>
        /// <param name="sender">Sending object</param>
        /// <param name="e">Event arguments</param>
        private void Grid_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            _viewModel = DataContext as IMainViewModel;
            _viewModel.PropertyChanged += ViewModel_PropertyChanged;
        }

        /// <summary>
        /// Event handler for Loaded event.
        /// </summary>
        /// <param name="sender">Sending object</param>
        /// <param name="e">Event arguments</param>
        private void NumericKeypad_Loaded(object sender, RoutedEventArgs e)
        {
            TelephoneTypeChanged();
        }

        /// <summary>
        /// Event handler for Unloaded event.
        /// </summary>
        /// <param name="sender">Sending object</param>
        /// <param name="e">Event arguments</param>
        private void NumericKeypad_Unloaded(object sender, RoutedEventArgs e)
        {
            InputTextBox.Text = string.Empty;
            InputTextBox.PasswordText.Clear();
        }

        /// <summary>
        /// Reconfigures when telephone type changes.
        /// </summary>
        private void TelephoneTypeChanged()
        {
            if (Convert.ToBoolean(TelephoneType))
            {
                firstRow.SetValue(Grid.RowProperty, 0);
                thirdRow.SetValue(Grid.RowProperty, 2);
            }
            else
            {
                firstRow.SetValue(Grid.RowProperty, 2);
                thirdRow.SetValue(Grid.RowProperty, 0);
            }
        }

        /// <summary>
        /// Event handler for Property Changed event.
        /// </summary>
        /// <param name="sender">Sending object</param>
        /// <param name="e">Event arguments</param>
        private void ViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (IsStateParamValid())
            {
                switch (e.PropertyName)
                {
                    case "StateParam":
                        OnStateParamChanged(_viewModel.StateParam);
                        break;
                    case "UIEchoField":
                        UpdateUIEchoField();
                        break;
                    case "NextGenData":
                        UpdateInstructions();
                        break;
                    case "MessageBoxEcho":
                        UpdateInstructionTextBox();
                        break;
                    case "StoreMode":
                        UpdateUIEchoField();
                        UpdateInstructions();
                        break;
                }
            }
			
            if (_viewModel.StateParam.Equals("mPerksEntry"))
                InputTextBox.MaxLength = 12;
            else
                InputTextBox.MaxLength = _viewModel.UIEchoField.MaxLength;
            UpdateNumericTextBoxStyle();			
        }

        /// <summary>
        /// Event handler for State Param Changed event.
        /// </summary>
        /// <param name="param">Parameter that changed</param>
        private void OnStateParamChanged(string param)
        {
            _inputValue = string.Empty;
            InputTextBox.Clear();
            OKButton.IsEnabled = false;
            InputTextBox.PasswordMode = false;
            _minInput = 1;
            UpdateUIEchoField();
            UpdateInstructions();
            UpdateElements();
            DecimalMarkKey.IsEnabled = true;
            MinusKey.IsEnabled = true;

            if (param.Equals("XMCashierPassword") || param.Equals("XMCashierPasswordV6HW"))
            {
                InputTextBox.PasswordMode = true;
            }
            else if (param.Equals("SmmKeyInWtTol"))
            {
                UpdateDecimalMarkKeyText();
            }
        }

        /// <summary>
        /// Event handler for keyboard button Click event.
        /// </summary>
        /// <param name="sender">Sending object</param>
        /// <param name="e">Event arguments</param>
        private void KeyboardButton_Click(object sender, RoutedEventArgs e)
        {
            WatermarkTextBox textBox = null;
            View view = UIControlFinder.FindAncestorOrSelf<View>(this);

            if (view == null)
            {
                CmDataCapture.CaptureFormat(CmDataCapture.MaskWarning, "KeyboardButton_Click ignored from sender [{0}]. View is null.", sender.ToString());
                return;
            }

            textBox = view.KeyboardFocusedElement as WatermarkTextBox;

            if (textBox == null)
            {
                CmDataCapture.CaptureFormat(CmDataCapture.MaskWarning, "KeyboardButton_Click ignored from sender [{0}]. TextBox is null.", sender.ToString());
                return;
            }

            ImageButton button = sender as ImageButton;
            if (button.CommandParameter != null)
            {
                Key key = Key.None;
                try
                {
                    key = (Key)new KeyConverter().ConvertFromString(button.CommandParameter.ToString());
                }
                catch (InvalidCastException)
                {
                }

                if (key != Key.None)
                {
                    if (key.Equals(Key.Back))
                    {
                        if (_inputValue.Length > 0)
                        {
                            _inputValue = _inputValue.Substring(0, _inputValue.Length - 1);
                        }

                        if (InputTextBox.Text[InputTextBox.Text.Length - 1] == '.')
                        {
                            DecimalMarkKey.IsEnabled = true;
                        }
                    }

                    int textBoxValue;
                    int.TryParse(_inputValue, out textBoxValue);

                    if ((_viewModel.StateParam.Equals("EnterCouponValue") || _viewModel.StateParam.Equals("AM_EnterCouponValue") || _viewModel.StateParam.Equals("AM_KeyInPrice") || IsCurrencyEnabled()) && (_inputValue.Length == 0 || textBoxValue < 1))
                    {
                        textBox.Clear();
                    }

                    textBox.RaiseEvent(new KeyEventArgs(Keyboard.PrimaryDevice, PresentationSource.FromVisual(textBox), 0, key) { RoutedEvent = Keyboard.KeyDownEvent });

                    if (AutoEnterDecimalPoint && _inputValue.Length > 0)
                    {
                        OKButton.IsEnabled = CalculateDecimalValue(string.Empty) > 0;
                    }
                    else if ((_viewModel.StateParam.Equals("EnterCouponValue") || _viewModel.StateParam.Equals("AM_EnterCouponValue") || _viewModel.StateParam.Equals("AM_KeyInPrice") || IsCurrencyEnabled()) && _inputValue.Length > 0)
                    {
                        OKButton.IsEnabled = CalculateTotalAmount(string.Empty) > 0;
                    }
                }
            }
            else
            {
                if (char.IsDigit(button.Text[0]) && _inputValue.Length < textBox.MaxLength)
                {
                    if (!((_viewModel.StateParam.Equals("EnterCouponValue") || _viewModel.StateParam.Equals("AM_EnterCouponValue") || _viewModel.StateParam.Equals("AM_KeyInPrice") || IsCurrencyEnabled())
                        && _inputValue.Length == 0 && button.Text.Equals("0") && !AllowZeroInput))
                    {
                        textBox.RaiseEvent(
                            new TextCompositionEventArgs(
                                InputManager.Current.PrimaryKeyboardDevice,
                                new TextComposition(InputManager.Current, textBox, button.Text))
                                {
                                    RoutedEvent = TextCompositionManager.TextInputEvent
                                });

                        if (AutoEnterDecimalPoint)
                        {
                            if (AllowZeroInput)
                            {
                                OKButton.IsEnabled = CalculateDecimalValue(button.Text) >= 0;
                            }
                            else
                            {
                                OKButton.IsEnabled = CalculateDecimalValue(button.Text) > 0;
                            }
                        }
                        else if (_viewModel.StateParam.Equals("EnterCouponValue") || _viewModel.StateParam.Equals("AM_EnterCouponValue") || _viewModel.StateParam.Equals("AM_KeyInPrice") || IsCurrencyEnabled())
                        {
                            if (AllowZeroInput)
                            {
                                OKButton.IsEnabled = CalculateTotalAmount(button.Text) >= 0;
                            }
                            else
                            {
                                OKButton.IsEnabled = CalculateTotalAmount(button.Text) > 0;
                            }
                        }
                    }
                }
                else
                {
                    textBox.RaiseEvent(
                        new TextCompositionEventArgs(
                            InputManager.Current.PrimaryKeyboardDevice,
                            new TextComposition(InputManager.Current, textBox, button.Text))
                            {
                                RoutedEvent = TextCompositionManager.TextInputEvent
                            });
                    Key key = KeyCode(button.Text);
                    if (key == Key.Decimal)
                    {
                        DecimalMarkKey.IsEnabled = false;
                    }
                    else if (key == Key.OemMinus)
                    {
                        MinusKey.IsEnabled = false;
                    }
                }

                _viewModel.ActionCommand.Execute(ActionCommand.ActionUIActivity);
            }

            button.IsInputMasked = AlphaNumericKeyboard.ListForInputMasked.Contains(_viewModel.StateParam);
        }

        /// <summary>
        /// Update UIEchoField
        /// </summary>
        private void UpdateUIEchoField()
        {
            _minInput = _viewModel.UIEchoField.MinLength;
            InputTextBox.MaxLength = _viewModel.StateParam.Equals("mPerksEntry") ? 12 : _viewModel.UIEchoField.MaxLength;
            UpdateNumericTextBoxStyle();
        }

        /// <summary>
        /// Determine whether currency is enabled.
        /// </summary>
        /// <returns>Whether currency is enabled.</returns>
        private bool IsCurrencyEnabled()
        {
            switch (_viewModel.StateParam)
            {
                case "CMCashAddV6HW":
                case "CMCashRemoveV6HW":
                case "CmDataEntryWithKeyBoard":
                case "SmDataEntryNumericSmall":
                case "SmDataEntryWithListBG":
                case "XMCashRemove":
                case "XMCashRemoveBNR":
                case "XMCashRemoveGlory":
                case "XMCashReplenish":
                    return _viewModel.UIEchoField.CurrencyEnabled ? true : false;
            }

            return false;
        }

        /// <summary>
        /// Determine whether tri decimal is enabled.
        /// </summary>
        /// <returns>Whether tri decimal is enabled.</returns>
        private bool IsTrimDecimalEnabled()
        {
            switch (_viewModel.StateParam)
            {
                case "CMCashAddV6HW":
                case "CMCashRemoveV6HW":
                case "XMCashRemove":
                case "XMCashRemoveBNR":
                case "XMCashRemoveGlory":
                case "XMCashReplenish":
                    return _viewModel.UIEchoField.TrimDecimalEnabled ? true : false;
            }

            return false;
        }

        /// <summary>
        /// Update numeric textbox style.
        /// </summary>
        private void UpdateNumericTextBoxStyle()
        {
            switch (_viewModel.StateParam)
            {
                case "ProduceFavorites":
                case "MultiSelectProduceFavorites":
                    if (BackgroundViewMode)
                    {
                        InputTextBox.Style = FindResource("smallNumericTextBoxStyle") as Style;
                        double fontSize = InputTextBox.FontSize;

                        // this will lock the value to the size when a popup with numeric pad occurs over this state
                        InputTextBox.FontSize = fontSize;
                    }

                    break;
                default:
                    if (InputTextBox.MaxLength >= 7 && InputTextBox.MaxLength < 10)
                    {
                        InputTextBox.Style = FindResource("mediumNumericTextBoxStyle") as Style;
                    }
                    else if (InputTextBox.MaxLength >= 10)
                    {
                        //+SSCOI-45677
                        if (_viewModel.StateParam.Equals("mPerksEntry"))
                        {
                            InputTextBox.Style = FindResource("mPerksNumericTextBoxStyle") as Style;
                        }
                        else
                        {
                            InputTextBox.Style = FindResource("smallNumericTextBoxStyle") as Style;
                        }
					}
                    else
                    {
                        InputTextBox.Style = FindResource("numericTextBoxStyle") as Style;
                    }

                    break;
            }
        }

        /// <summary>
        /// Update instructions.
        /// </summary>
        private void UpdateInstructions()
        {
            if (_viewModel.StateParam.Equals("EnterId") || _viewModel.StateParam.Equals("SmCashierPassword"))
            {
                InputTextBox.Clear();
                if (_viewModel.GetPropertyValue("NextGenData").Equals("EnterPassword"))
                {
                    OKButton.IsEnabled = false;
                    _minInput = 1;
                    InputTextBox.PasswordMode = true;
                }
                else if (_viewModel.GetPropertyValue("NextGenData").Equals("EnterID"))
                {
                    InputTextBox.PasswordMode = false;
                }
                else
                {
                    this.InputTextBox.PasswordMode = false; //SSCOI-46400
                }
            }
        }

        /// <summary>
        /// Update instructions textbox.
        /// </summary>
        private void UpdateInstructionTextBox()
        {
            if (string.IsNullOrWhiteSpace(_viewModel.GetPropertyValue("MessageBoxEcho").ToString()))
            {
                InputTextBox.Clear();
            }
        }

        /// <summary>
        /// Update elements
        /// </summary>
        private void UpdateElements()
        {
            switch (_viewModel.StateParam)
            {
                // list of excluded state param from automatic update
                case "ProduceFavorites":
                case "MultiSelectProduceFavorites":
                    break;
                default:
                    UpdateStateElements(_viewModel.StateParam);
                    break;
            }
        }

        /// <summary>
        /// Update additional keys
        /// </summary>
        private void UpdateAdditionalKeys()
        {
            DecimalMarkKey.Visibility = Visibility.Collapsed;
            MinusKey.Visibility = Visibility.Collapsed;

            if (_viewModel.StateParam.Equals("SmmKeyInWtTol"))
            {
                DecimalMarkKey.Visibility = Visibility.Visible;
                KeypadButton0.Width = 65;
            }
            else if (_viewModel.StateParam.Equals("SmLoadLift"))
            {
                MinusKey.Visibility = Visibility.Visible;
                KeypadButton0.Width = 65;
            }
        }

        /// <summary>
        /// Update styles.
        /// </summary>
        /// <param name="storeMode">Store mode</param>
        private void UpdateStyles(bool storeMode)
        {
            if (storeMode)
            {
                NumericBackspaceButton.Style = FindResource("storeBackSpaceButtonStyle") as Style;
                OKButton.Style = FindResource("storeNumericOkButtonStyle") as Style;
                KeypadButton0.Style = FindResource("storeNumericMainButtonStyle") as Style;
                DecimalMarkKey.Style = FindResource("storeNumericMainButtonStyle") as Style;
                MinusKey.Style = FindResource("storeNumericMainButtonStyle") as Style;
            }
            else
            {
                switch (_viewModel.StateParam)
                {
                    case "ProduceFavorites":
                    case "MultiSelectProduceFavorites":
                        NumericBackspaceButton.Style = FindResource("backSpaceButtonStyle") as Style;
                        OKButton.Style = FindResource("numericOkButtonStyle") as Style;
                        KeypadButton0.Style = FindResource("searchKeypadButtonStyle") as Style;
                        DecimalMarkKey.Style = FindResource("mainButtonStyle") as Style;
                        MinusKey.Style = FindResource("mainButtonStyle") as Style;
                        break;
                    default:
                        NumericBackspaceButton.Style = FindResource("backSpaceButtonStyle") as Style;
                        OKButton.Style = FindResource("numericOkButtonStyle") as Style;
                        KeypadButton0.Style = FindResource("mainButtonStyle") as Style;
                        DecimalMarkKey.Style = FindResource("mainButtonStyle") as Style;
                        MinusKey.Style = FindResource("mainButtonStyle") as Style;
                        break;
                }
            }

            UpdateButtonStyles(storeMode);
        }

        /// <summary>
        /// Update button styles.
        /// </summary>
        /// <param name="storeMode">Store mode</param>
        private void UpdateButtonStyles(bool storeMode)
        {
            Style buttonStyle;
            UniformGrid[] rows = { firstRow, secondRow, thirdRow };

            if (storeMode)
            {
                buttonStyle = FindResource("storeNumericMainButtonStyle") as Style;
            }
            else
            {
                switch (_viewModel.StateParam)
                {
                    case "ProduceFavorites":
                    case "MultiSelectProduceFavorites":
                        buttonStyle = FindResource("searchNumpadButtonStyle") as Style;
                        break;
                    default:
                        buttonStyle = FindResource("mainButtonStyle") as Style;
                        break;
                }
            }

            for (int i = 0; i < rows.Length; i++)
            {
                UpdateButtons(storeMode, rows[i], buttonStyle);
            }
        }

        /// <summary>
        /// Update DecimalMarkKey's text.
        /// </summary>
        private void UpdateDecimalMarkKeyText()
        {
            string decimalSeparator = NumberFormatInfo.CurrentInfo.CurrencyDecimalSeparator;
            if (!string.IsNullOrEmpty(decimalSeparator))
            {
                DecimalMarkKey.Text = decimalSeparator;
            }
        }

        /// <summary>
        /// Calculate total amount.
        /// </summary>
        /// <param name="initialAmount">Initial amount.</param>
        /// <returns>Total amount.</returns>
        private double CalculateTotalAmount(string initialAmount)
        {
            _inputValue += initialAmount;

            _priceTextBlock.IsTrimDecimal = IsCurrencyEnabled() && IsTrimDecimalEnabled() ? true : false;
            _priceTextBlock.Value = _inputValue;

            InputTextBox.Text = _priceTextBlock.Text;
            InputTextBox.Select(InputTextBox.Text.Length, 0);

            double input = 0;
            double.TryParse(_priceTextBlock.Text.Replace(RegionInfo.CurrentRegion.CurrencySymbol, string.Empty), NumberStyles.Any, CultureInfo.CurrentCulture, out input);
            return input;
        }

        /// <summary>
        /// Calculate decimal value for current entry and set the formatted input textbox value.
        /// </summary>
        /// <param name="input">Input digit.</param>
        /// <returns>Decimal value.</returns>
        private double CalculateDecimalValue(string input)
        {
            _inputValue += input;
            double value = 0;

            if (_inputValue != null && _inputValue.Length > 0)
            {
                // Validate and parse input.
                Regex regex = new Regex(@"[0-9|\-\(\)]+");
                string inputString = string.Empty;
                foreach (Match m in regex.Matches(_inputValue))
                {
                    inputString += m.Value;
                }

                // Create format string to match configured number of digits after the decimal point.
                string format = "#0.";
                for (int i = 0; i < AutoEnterDecimalPointDigits; i++)
                {
                    format += "0";
                }

                // Format value using configured number of digits after the decimal point.
                double.TryParse(inputString, NumberStyles.Any, CultureInfo.InvariantCulture, out value);
                value /= Math.Pow(10, AutoEnterDecimalPointDigits);
                _inputValue = value.ToString(format);
            }

            InputTextBox.Text = _inputValue;
            InputTextBox.Select(InputTextBox.Text.Length, 0);
            return value;
        }

        /// <summary>
        /// Event handler for key down event.
        /// </summary>
        /// <param name="sender">Sending object</param>
        /// <param name="e">Event arguments</param>
        private void InputTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            e.Handled = !(e.Key == Key.System || e.Key == Key.Back || char.IsDigit((char)KeyInterop.VirtualKeyFromKey(e.Key)));
        }

        /// <summary>
        /// Event handler for text changed event.
        /// </summary>
        /// <param name="sender">Sending object</param>
        /// <param name="e">Event arguments</param>
        private void InputTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (_viewModel.StateParam.Equals("EnterId")
                || _viewModel.StateParam.Equals("SellBags")
                || _viewModel.StateParam.Equals("KeyInCode")
                || _viewModel.State.Equals("Store"))
            {
                OKButton.IsEnabled = InputTextBox.Text.Length > 0;
            }

            if (AutoEnterDecimalPoint && InputTextBox.Text.Length <= 0)
            {
                OKButton.IsEnabled = false;
                _inputValue = string.Empty;
            }
            else if (_viewModel.StateParam.Equals("CmDataEntryWithKeyBoard") && _viewModel.BackgroundStateParam.Equals("CmPrePayFuelAmount"))
            {
                int input = 0;
                if (InputTextBox.Text.Length > 0)
                {
                    int.TryParse(InputTextBox.Text, out input);
                }

                OKButton.IsEnabled = input > 0;
            }
            else if (_viewModel.StateParam.Equals("CmDataEntryWithKeyBoard") || (_viewModel.StateParam.Equals("SmDataEntryWithListBG") || _viewModel.StateParam.Equals("SmDataEntryNumericSmall") && !IsCurrencyEnabled()))
            {
                OKButton.IsEnabled = InputTextBox.Text.Length >= _minInput;
            }
            else if (_viewModel.StateParam.Equals("EnterQuantity") || _viewModel.StateParam.Equals("AM_EnterQuantity") || _viewModel.StateParam.Equals("AM_KeyInQuantity"))
            {
                int input = 0;
                if (InputTextBox.Text.Length > 0)
                {
                    int.TryParse(InputTextBox.Text, out input);
                }

                OKButton.IsEnabled = input > 0;
            }
            else if ((_viewModel.StateParam.Equals("EnterCouponValue") || _viewModel.StateParam.Equals("AM_EnterCouponValue") || _viewModel.StateParam.Equals("AM_KeyInPrice") || IsCurrencyEnabled()) && InputTextBox.Text.Length <= 0)
            {
                OKButton.IsEnabled = false;
                _inputValue = string.Empty;
            }
			else if (_viewModel.StateParam.Equals("mPerksEntry"))
            {
                if (!string.IsNullOrEmpty(InputTextBox.Text.Trim()))
                {
                    InputTextBox.TextChanged -= InputTextBox_TextChanged;
                    var actualText = InputTextBox.Text.Replace("-", "");
                    var length = actualText.Trim().Length;
                    if (length == 10)
                        InputTextBox.Text = Regex.Replace(actualText, @"(\d{3})(\d{3})(\d{4})", "$1-$2-$3");
                    else if (length == 7)
                        InputTextBox.Text = Regex.Replace(actualText, @"(\d{3})(\d{3})(\d{1})", "$1-$2-$3");
                    else if (length == 4)
                        InputTextBox.Text = Regex.Replace(actualText, @"(\d{3})(\d{1})", "$1-$2");
                    else
                    { //do nothing
                    }
                    InputTextBox.TextChanged += InputTextBox_TextChanged;
                    CmDataCapture.CaptureFormat(CmDataCapture.MaskInfo, "MPerks:" + InputTextBox.Text);
                }
                OKButton.IsEnabled = InputTextBox.Text.Length >= 12;
            }
            else if (_viewModel.StateParam.Equals("SmmKeyInWtTol"))
            {
                if (InputTextBox.Text.Length == 0)
                {
                    DecimalMarkKey.IsEnabled = true;
                }
            }
            else if (_viewModel.StateParam.Equals("SmLoadLift"))
            {
                double input = 0;
                double.TryParse(InputTextBox.Text, NumberStyles.Any, CultureInfo.CurrentCulture, out input);
                OKButton.IsEnabled = input != 0;
                MinusKey.IsEnabled = InputTextBox.Text.Length == 0;
            }

            NumericBackspaceButton.IsEnabled = InputTextBox.Text.Length > 0;

            if (_viewModel.IsUNavEnabled && NumericBackspaceButton.IsFocused && !NumericBackspaceButton.IsEnabled)
            {
                KeyboardSimulator.Instance.SimulateKeyPress(VirtualKeyCode.TAB);
            }
        }

        /// <summary>
        /// Validates the ActiveStateParam for the NumericKeypad
        /// </summary>
        /// <returns>Returns true if valid, else false if invalid</returns>
        private bool IsStateParamValid()
        {
            switch (_viewModel.ActiveStateParam)
            {
                case "SmCashierPassword":
                case "SmDataEntryNumericSmall":
                case "SmDataEntryWithListBG":
                case "SmEnterBirthdate":
                case "SmLoadLift":
                case "SmmKeyInItemCode":
                case "SmmKeyInWtTol":
                case "AM_EnterCouponValue":
                case "AM_EnterQuantity":
                case "AM_KeyInCode":
                case "AM_KeyInPrice":
                case "AM_KeyInQuantity":
                case "CmDataEntryWithKeyBoard":
                case "EnterCouponValue":
                case "EnterId":
                case "EnterQuantity":
                case "SellBags":
                case "CMCashAddV6HW":
                case "CMCashRemoveV6HW":
                case "XMCashRemove":
                case "XMCashRemoveBNR":
                case "XMCashRemoveGlory":
                case "XMCashReplenish":
                case "XMCashierID":
                case "XMCashierPassword":
                case "XMCashierIDV6HW":
                case "XMCashierPasswordV6HW":
                case "ProduceFavorites":
                case "MultiSelectProduceFavorites":
                case "KeyInCode":
                case "mPerksEntry": // SSCOI-46450
                    return true;
                default:
                    return false;
            }
        }

        /// <summary>
        /// Event handler for Click event.
        /// </summary>
        /// <param name="sender">Sending object</param>
        /// <param name="e">Event arguments</param>
        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            IValueConverter converter = null;
            string converterParameter = string.Empty;
            string textBoxValue = string.Empty;

            if (InputTextBox.PasswordMode)
            {
                textBoxValue = SecureStringHelper.Decrypt(InputTextBox.PasswordText);
            }
            else
            {
                textBoxValue = InputTextBox.Text;
            }

            if (_viewModel.StateParam.Equals("EnterCouponValue") || _viewModel.StateParam.Equals("AM_EnterCouponValue") || _viewModel.StateParam.Equals("AM_KeyInPrice") || IsCurrencyEnabled())
            {
                converter = new FormatCurrencyConverter();
            }
            else if (_viewModel.StateParam.Equals("SmmKeyInWtTol"))
            {
                converter = new FormatDecimalSeparatorConverter();
            }
            else
            {
                converter = new FormatConverter();
            }

            if (_viewModel.StoreMode)
            {
                if (_viewModel.StateParam.StartsWith("XMCashRemove") || _viewModel.StateParam.Equals("XMCashReplenish"))
                {
                    converterParameter = "CashManagementKeypad({0})";
                }
                else
                {
                    switch (_viewModel.StateParam)
                    {
                        case "CMCashAddV6HW":
                        case "CMCashRemoveV6HW":
                            converterParameter = "CashManagementR6Keypad({0})";
                            break;
                        case "XMCashierIDV6HW":
                        case "XMCashierPasswordV6HW":
                            converterParameter = "StoreCMEnterNumericR6({0})";
                            break;
                        case "EnterId":
                            converterParameter = "EnterNumeric({0})";
                            break;
                        case "SmmKeyInItemCode":
                            converterParameter = "StoreSmmEnterNumeric({0})";
                            break;
                        case "SmmKeyInWtTol":
                            converterParameter = "StoreEnterNumeric({0},.,KeyBoardP3)";
                            break;
                        case "SmLoadLift":
                            converterParameter = "StoreEnterNumeric({0},-,MinusKey)";
                            break;
                        default:
                            converterParameter = "StoreEnterNumeric({0})";
                            break;
                    }
                }
            }
            else
            {
                switch (_viewModel.StateParam)
                {
                    case "ProduceFavorites":
                    case "MultiSelectProduceFavorites":
                        converterParameter = "EnterCode({0})";
                        break;
                    default:
                        converterParameter = "EnterNumeric({0})";
                        break;
                }
            }
            if (_viewModel.StateParam.Equals("mPerksEntry"))
                textBoxValue = textBoxValue.Replace("-", "");
            string commandValue = converter.Convert(textBoxValue, null, converterParameter, null).ToString();
            OKButton.CommandParameter = commandValue;
            InputTextBox.Text = string.Empty;
            InputTextBox.Clear();
            InputTextBox.PasswordText.Clear();
        }
    }
}
