// <copyright file="AlphaNumericInput.xaml.cs" company="NCR">
//     Copyright 2016 NCR. All rights reserved.
// </copyright>

namespace SSCOUIViews.Views
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.Reflection;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Input;
    using FPsxWPF.Controls;
    using SSCOControls;
    using SSCOUIModels;
    using SSCOUIModels.Controls;
    using SSCOUIModels.Helpers;

    /// <summary>
    /// Interaction logic for AlphaNumericInput.XAML
    /// </summary>
    public partial class AlphaNumericInput : PopupView
    {
        /// <summary>
        /// List variable
        /// </summary>
        private static List<string> _stateParamToShowTitleControl = new List<string>() { "AlphaNumeric", "ExtendedAlphaNumeric", "AlphaNumericPassword" };

        /// <summary>
        /// List variable
        /// </summary>
        private static List<string> _stateParamToShowExtendedPad = new List<string>() { "ExtendedAlphaNumeric", "OperatorKeyboard", "ContextHelp", "HelpOnWay", "Keyboard0409" };

        /// <summary>
        /// List variable
        /// </summary>
        private static List<string> _contextForTitleControlShow = new List<string>() { "AlphaNumeric", "ExtendedAlphaNumeric", "AlphaNumericPassword", "CashierIDKeyboard", "CashierPasswordKeyboard" };

        /// <summary>
        /// List variable
        /// </summary>
        private static List<string> _contextForInstructionControlShow = new List<string>() { "Keyboard0409", "OperatorKeyboard", "AlphaNumericPassword", "AlphaNumericKeyboard", "StoreModeKeyboard",  };

        /// <summary>
        /// Minimum input allowed
        /// </summary>
        private int _minInput = 1;

        /// <summary>
        /// Initializes a new instance of the AlphaNumericInput class.
        /// </summary>
        /// <param name="viewModel">ViewModel object.</param>
        public AlphaNumericInput(IMainViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
            GenericAlphaNumericKeyboard.InitKeyboardProperties();
            SubscribeToEvents();
        }

        /// <summary>
        /// Handle a change in this view's Show status.
        /// </summary>
        /// <param name="isShowing">Boolean flag used to track whether or not this view is currently being displayed.</param>
        public override void Show(bool isShowing)
        {
            if (!isShowing)
            {
                GenericAlphaNumericKeyboard.ShiftButton.IsChecked = false;
            }
            else
            {
                GenericAlphaNumericKeyboard.RetrieveCustomKeyboardProperties();
                GenericAlphaNumericKeyboard.ShowShiftKey();
                OnNextGenDataChanged();
            }

            base.Show(isShowing);
        }

        /// <summary>
        /// Accepts a parameter change as configured in app.config.
        /// </summary>
        /// <param name="param">Changed parameter.</param>
        public override void OnStateParamChanged(string param)
        {
            Instructions.Visibility = _contextForInstructionControlShow.Contains(param) ? Visibility.Visible : Visibility.Collapsed;
            UpdateText("LeadthruText", TitleControl);
            UpdateText("MessageBoxNoEcho", Message);
            UpdateText("InstructionScreenTitle", Instructions);
            TitleControl.Visibility = _stateParamToShowTitleControl.Contains(viewModel.StateParam) ? Visibility.Visible : Visibility.Collapsed;
            GenericAlphaNumericKeyboard.InputTextBox.Clear();
            GenericAlphaNumericKeyboard.InputTextBox.PasswordMode = false;
            UpdateTitleStyle();
            SetDefaultKeyboard();
            UpdateGoBackButtonVisibility();

            switch (param)
            {
                case "OperatorKeyboard":
                    Instructions.Property(TextBlock.TextProperty).SetResourceValue("EnterIDText");
                    Message.Property(TextBlock.TextProperty).SetResourceValue("EnterIDMessage");
                    break;
            }

            TitleControl.Visibility = _contextForTitleControlShow.Contains(param) ? Visibility.Visible : Visibility.Collapsed;
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
                case "NextGenData":
                    OnNextGenDataChanged();
                    break;
                case "LeadthruText":
                    UpdateLeadthruText();
                    break;
                case "MessageBoxNoEcho":
                    UpdateText(name, Message);
                    break;
                case "InstructionScreenTitle":
                    UpdateText(name, Instructions);
                    break;
                case "UIEchoField":
                    _minInput = viewModel.UIEchoField.MinLength;
                    GenericAlphaNumericKeyboard.InputTextBox.MaxLength = viewModel.UIEchoField.MaxLength;
                    break;
                case "NextGenUIHotKeyState":
                    GenericAlphaNumericKeyboard.SetHotKeysButtonState();
                    GenericAlphaNumericKeyboard.SetShiftButtonState();
                    break;
                case "ButtonGoBackShown":
                    UpdateGoBackButtonVisibility();
                    break;
                case "UNav":
                    FocusHelper.SetInitialFocus(this, viewModel);
                    break;
                case "ShiftKeyShown":
                    GenericAlphaNumericKeyboard.ShowShiftKey();
                    break;
                case "SymbolsKeyShown":
                    GenericAlphaNumericKeyboard.ShowSymbolsKey();
                    break;
                case "SymbolsKeyText":
                    GenericAlphaNumericKeyboard.UpdateSymbolsButtonText();
                    break;
            }
        }

        /// <summary>
        /// Keyboard click events
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param>
        /// <param name="e">This is a parameter with a type of RoutedEventArgs</param>
        private void KeyboardButton_Click(object sender, RoutedEventArgs e)
        {
            FrameworkElement textBox = KeyboardFocusedElement;
            if (textBox == null)
            {
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

                if (key != Key.None && key != Key.Space)
                {
                    textBox.RaiseEvent(new KeyEventArgs(Keyboard.PrimaryDevice, PresentationSource.FromVisual(textBox), 0, key) { RoutedEvent = Keyboard.KeyDownEvent });
                }
                else if (key == Key.Space)
                {
                    textBox.RaiseEvent(new TextCompositionEventArgs(
                        InputManager.Current.PrimaryKeyboardDevice,
                        new TextComposition(InputManager.Current, textBox, " "))
                    {
                        RoutedEvent = TextCompositionManager.TextInputEvent
                    });
                }
            }
            else
            {
                textBox.RaiseEvent(new TextCompositionEventArgs(
                    InputManager.Current.PrimaryKeyboardDevice,
                    new TextComposition(InputManager.Current, textBox, button.Text))
                {
                    RoutedEvent = TextCompositionManager.TextInputEvent
                });
            }
        }

        /// <summary>
        /// Updates the style applied to the title and instruction controls.
        /// </summary>
        private void UpdateTitleStyle()
        {
            TitleControl.Style = viewModel.StoreMode ? FindResource("storeModeAlphanumericPopupTitleStyle") as Style : FindResource("alphaNumericPopupTitleStyle") as Style;
            Instructions.Style = viewModel.StoreMode ? FindResource("storeModeAlphanumericPopupTitleStyle") as Style : FindResource("alphaNumericPopupTitleStyle") as Style;
            Message.Style = viewModel.StoreMode ? FindResource("storeModeAlphaNumericPopupTextStyle") as Style : FindResource("alphaNumericPopupTextStyle") as Style;
        }

        /// <summary>
        /// EnterButton click events
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param>
        /// <param name="e">This is a parameter with a type of RoutedEventArgs</param>
        private void EnterButton_Click(object sender, RoutedEventArgs e)
        {
            string commandParam = string.Empty;
            if (viewModel.StateParam != null)
            {
                switch (viewModel.StateParam)
                {
                    case "ExtendedAlphaNumeric":
                        commandParam = "EnterAlphaNum({0}?@?AtSignKey?.?KeyBoardP3?-?MinusKey)";
                        break;
                    case "CashierIDKeyboard":
                    case "CashierPasswordKeyboard":
                        commandParam = "CashManagementEnterKeyboard({0})";
                        break;
                    default:
                        commandParam = "EnterKeyboard({0})";
                        break;
                }
            }

            string keyboardParam = GenericAlphaNumericKeyboard.InputTextBox.PasswordMode ?
                SecureStringHelper.Decrypt(GenericAlphaNumericKeyboard.InputTextBox.PasswordText) :
                GenericAlphaNumericKeyboard.InputTextBox.Text;

            GenericAlphaNumericKeyboard.EnterButton.CommandParameter = string.Format(
                commandParam,
                viewModel.EncodeActionParam(keyboardParam));
            GenericAlphaNumericKeyboard.InputTextBox.Text = string.Empty;
            GenericAlphaNumericKeyboard.InputTextBox.PasswordText.Clear();
        }

        /// <summary>
        /// Updates text based on property watch value.
        /// </summary>
        /// <param name="property">Property watch string key</param>
        /// <param name="tb">MeasureTextBlock control</param>
        private void UpdateText(string property, MeasureTextBlock tb)
        {
            string propertyText = GetPropertyStringValue(property);

            if (propertyText.Length > 0)
            {
                string[] headerText = propertyText.Split(':');
                tb.Text = headerText[0];
                if (headerText.Length > 1)
                {
                    tb.Text = tb.Text + ":\n" + headerText[1];
                }
            }
        }

        /// <summary>
        /// Refreshes keyboard button click event handlers.
        /// </summary>
        private void SubscribeToEvents()
        {
            GenericAlphaNumericKeyboard.KeyboardButtonClick -= KeyboardButton_Click;
            GenericAlphaNumericKeyboard.KeyboardButtonClick += KeyboardButton_Click;
            GenericAlphaNumericKeyboard.EnterButtonClick -= EnterButton_Click;
            GenericAlphaNumericKeyboard.EnterButtonClick += EnterButton_Click;
            GenericAlphaNumericKeyboard.ShiftButtonClick -= ShiftButton_Click;
            GenericAlphaNumericKeyboard.ShiftButtonClick += ShiftButton_Click;
        }

        /// <summary>
        /// Shift button method handler for click event
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param>
        /// <param name="e">This is a parameter with a type of RoutedEventArgs</param>
        private void ShiftButton_Click(object sender, RoutedEventArgs e)
        {
            ImageToggleButton button = sender as ImageToggleButton;
            if (button != null)
            {
                viewModel.ActionCommand.Execute("ShiftKey");
            }
        }

        /// <summary>
        /// Sets default keyboard control keys.
        /// </summary>
        private void SetDefaultKeyboard()
        {
            GenericAlphaNumericKeyboard.SetAlphaNumLineKeys("AlphaNumKeysLine");
            GenericAlphaNumericKeyboard.SetHotKeysLine("HotKeys");
            GenericAlphaNumericKeyboard.RemoveAddEvents();
        }

        /// <summary>
        /// Sets Cash Management-specific keyboard control keys.
        /// </summary>
        private void SetCashManagementKeyboard()
        {
            GenericAlphaNumericKeyboard.SetAlphaNumLineKeys("XMAlphaNumKeysLine");
            GenericAlphaNumericKeyboard.SetHotKeysLine("HotKeys");
            GenericAlphaNumericKeyboard.RemoveAddEvents();
        }

        /// <summary>
        /// Updates the instructional text based on property watches.
        /// </summary>
        private void UpdateLeadthruText()
        {
            switch (viewModel.StateParam)
            {
                case "CashierIDKeyboard":
                case "CashierPasswordKeyboard":
                    UpdateText("LeadthruText", Message);
                    break;
                default:
                    UpdateText("LeadthruText", TitleControl);
                    break;
            }
        }

        /// <summary>
        /// Handles the NextGenData change, which determines the ID/Password state.
        /// </summary>
        private void OnNextGenDataChanged()
        {
            string value = GetPropertyStringValue("NextGenData");
            GenericAlphaNumericKeyboard.InputTextBox.Clear();
            if (viewModel.StateParam.Equals("OperatorKeyboard"))
            {
                if (value.ToString().Equals("EnterPassword"))
                {
                    Instructions.Property(TextBlock.TextProperty).SetResourceValue("EnterPasswordText");
                    Message.Property(TextBlock.TextProperty).SetResourceValue("EnterPasswordMessage");
                    GenericAlphaNumericKeyboard.InputTextBox.PasswordMode = true;
                    FocusHelper.SetInitialFocus(this, viewModel);
                }
                else if (value.ToString().Equals("EnterID"))
                {
                    Instructions.Property(TextBlock.TextProperty).SetResourceValue("EnterIDText");
                    Message.Property(TextBlock.TextProperty).SetResourceValue("EnterIDMessage");
                    GenericAlphaNumericKeyboard.InputTextBox.PasswordMode = false;
                }
            }

            UpdateInitialFocus();
        }

        /// <summary>
        /// Sets the initial focus of the image button when it is in a data template. 
        /// This is a special case where we need to set initial focus manually here. 
        /// </summary>
        /// <param name="itemsControl">The items control with the data template</param>
        /// <param name="controlName">The name of the image button</param>
        private void SetInitialFocus(ItemsControl itemsControl, string controlName)
        {
            if (GenericAlphaNumericKeyboard.Line1Keys.Items.Count > 0)
            {
                DependencyObject dependencyObject = itemsControl.ItemContainerGenerator.ContainerFromIndex(0);

                ContentPresenter contentPresenter = dependencyObject as ContentPresenter;
                if (contentPresenter != null)
                {
                    // this is needed to fix a potential crash if the data template has not been applied yet
                    contentPresenter.ApplyTemplate();

                    object buttonControl = contentPresenter.ContentTemplate.FindName(controlName, contentPresenter);

                    ImageButton button = buttonControl as ImageButton;
                    if (button != null)
                    {
                        button.Focus();
                    }
                }
            }
        }

        /// <summary>
        /// Updates the initial focus when GoBack button is collapsed.
        /// </summary>
        private void UpdateInitialFocus()
        {
            switch (viewModel.StateParam)
            {
                case "OperatorKeyboard":
                case "AlphaNumericKeyboard":
                    if (viewModel.IsUNavEnabled && GenericAlphaNumericKeyboard.GoBackButton.Visibility == Visibility.Collapsed)
                    {
                        SetInitialFocus(GenericAlphaNumericKeyboard.Line1Keys, "LineKeys");
                    }

                    break;
                default:
                    break;
            }
        }

        /// <summary>
        /// Updates the GoBack button visibility based on the state parameter and corresponding property watch.
        /// </summary>
        private void UpdateGoBackButtonVisibility()
        {
            switch (viewModel.StateParam)
            {
                case "OperatorKeyboard":
                case "AlphaNumericKeyboard":
                    GenericAlphaNumericKeyboard.SetGoBackButtonVisibility(GetPropertyBoolValue("ButtonGoBackShown"));
                    break;
                default:
                    break;
            }
        }
    }
}
