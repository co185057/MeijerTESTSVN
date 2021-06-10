// <copyright file="AlphaNumericKeyboard.xaml.cs" company="NCR Corporation">
//     Copyright (c) NCR Corporation. All rights reserved.
// </copyright>
namespace SSCOUIViews.Controls
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.Collections.Specialized;
    using System.ComponentModel;
    using System.Linq;
    using System.Text;
    using System.Windows;
    using System.Windows.Automation.Peers;
    using System.Windows.Automation.Provider;
    using System.Windows.Controls;
    using System.Windows.Controls.Primitives;
    using System.Windows.Data;
    using System.Windows.Documents;
    using System.Windows.Input;
    using System.Windows.Media;
    using System.Windows.Media.Imaging;
    using System.Windows.Navigation;
    using System.Windows.Shapes;
    using System.Windows.Threading;
    using FPsxWPF.Controls;
    using FPsxWPF.Helpers;
    using RPSWNET;
    using SSCOControls;
    using SSCOControls.Helpers.KeyboardSimulatorComponents;
    using SSCOUIModels;
    using SSCOUIModels.Controls;
    using SSCOUIModels.Helpers;

    /// <summary>
    /// Interaction logic for AlphaNumericKeyboard.xaml
    /// </summary>
    public partial class AlphaNumericKeyboard : Grid
    {
        /// <summary>
        /// Param list variable.
        /// </summary>
        private static List<string> ParamListForInputMasked = new List<string>() 
        { 
            "EnterId", "OperatorKeyboard", "SmCashierPassword", "XMCashierID", "XMCashierPassword", 
            "XMCashierIDKeyboard", "XMCashierPasswordKeyboard", "XMCashierIDV6HW", "XMCashierPasswordV6HW" 
        };

        /// <summary>
        /// Dependency Property for Background View Mode
        /// </summary>
        private static DependencyProperty BackgroundViewModeProperty = DependencyProperty.Register("BackgroundViewMode", typeof(bool), typeof(AlphaNumericKeyboard));

        /// <summary>
        /// Dependency property for Show NumericKeypad.
        /// </summary>
        private static DependencyProperty ShowNumericKeypadProperty = DependencyProperty.Register("ShowNumericKeypad", typeof(bool), typeof(AlphaNumericKeyboard));

        /// <summary>
        /// Minimum input length for TextBlock
        /// </summary>
        private int _MinInput = 1;

        /// <summary>
        /// View Model object
        /// </summary>
        private IMainViewModel _viewModel;

        /// <summary>
        /// Boolean value for Shift shown.
        /// </summary>
        private bool _shiftShown;

        /// <summary>
        /// Boolean value for Symbols shown.
        /// </summary>
        private bool _symbolsShown;

        /// <summary>
        /// Boolean variable for Keyboard shown behind popup.
        /// </summary>
        private bool _keyboardShownBehindPopup = false;

        /// <summary>
        /// Boolean variable for Line1 keys
        /// </summary>
        private bool _line1Updated = false;

        /// <summary>
        /// Boolean variable for Line2 keys
        /// </summary>
        private bool _line2Updated = false;

        /// <summary>
        /// Boolean variable for Line3 keys
        /// </summary>
        private bool _line3Updated = false;

        /// <summary>
        /// Collection for Line1 Keys.
        /// </summary>
        private ObservableCollection<GridItem> _Line1KeysCollection;

        /// <summary>
        /// Collection for Line2 Keys.
        /// </summary>
        private ObservableCollection<GridItem> _Line2KeysCollection;

        /// <summary>
        /// Collection for Line3 Keys.
        /// </summary>
        private ObservableCollection<GridItem> _Line3KeysCollection;

        /// <summary>
        /// Collection for Line4 Keys.
        /// </summary>
        private ObservableCollection<GridItem> _Line4KeysCollection;

        /// <summary>
        /// Collection for Hot Keys.
        /// </summary>
        private ObservableCollection<GridItem> _HotKeysCollection;

        /// <summary>
        /// Grid for Line1 AlphaNumeric Keys
        /// </summary>
        private UniformGrid _Line1AlphaNumKeys;

        /// <summary>
        /// Grid for Line2 AlphaNumeric Keys
        /// </summary>
        private UniformGrid _Line2AlphaNumKeys;

        /// <summary>
        /// Grid for Line3 AlphaNumeric Keys
        /// </summary>
        private UniformGrid _Line3AlphaNumKeys;

        /// <summary>
        /// Grid for Hot Keys
        /// </summary>
        private UniformGrid _HotKeys;

        /// <summary>
        /// Grid for Exceeded Keys
        /// </summary>
        private UniformGrid _ExceededKeysUniformGrid;

        /// <summary>
        /// Collection for Hot Keys button
        /// </summary>
        private IEnumerable<ImageToggleButton> _hotKeysButtons;

        /// <summary>
        /// Collection for Exceeded Keys
        /// </summary>
        private ObservableCollection<GridItem> _ExceededKeysCollection;

        /// <summary>
        /// Style for AlphaNumeric Keypad
        /// </summary>
        private Style _alphaNumericKeypadStyle;

        /// <summary>
        /// Style for AlphaNumeric HotKeypad 
        /// </summary>
        private Style _alphaNumericHotKeypadStyle;

        /// <summary>
        /// Style for StoreMode AlphaNumeric Keypad
        /// </summary>
        private Style _storeModeAlphaNumericKeypadStyle;

        /// <summary>
        /// Style for StoreMode AlphaNumeric HotKeypad 
        /// </summary>
        private Style _storeModeAlphaNumericHotKeypadStyle;

        /// <summary>
        /// Style for AlphaNumeric Textbox border
        /// </summary>
        private Style _alphaNumericTextboxBorderStyle;

        /// <summary>
        /// Style for AlphaNumeric Textbox Inner border shadow
        /// </summary>
        private Style _alphaNumericTextboxInnerShadowStyle;

        /// <summary>
        /// Style for Search Textbox border
        /// </summary>
        private Style _searchTextboxBorderStyle;

        /// <summary>
        /// Style for Search Textbox Inner shadow
        /// </summary>
        private Style _searchTextboxInnerShadowStyle;

        /// <summary>
        /// Style for Search Keypad button
        /// </summary>
        private Style _searchKeypadButtonStyle;

        /// <summary>
        /// Style for Search Watermark Textbox
        /// </summary>
        private Style _searchWatermarkTextboxStyle;

        /// <summary>
        /// Style for AlphaNumeric Enter button
        /// </summary>
        private Style _alphaNumericEnterButtonStyle;

        /// <summary>
        /// Style for AlphaNumeric Backspace button
        /// </summary>
        private Style _alphaNumericBackspaceButtonStyle;

        /// <summary>
        /// Style for AlphaNumeric Space button
        /// </summary>
        private Style _alphaNumericSpaceButtonStyle;

        /// <summary>
        /// Style for StoreMode AlphaNumeric Enter button
        /// </summary>
        private Style _storeModeAlphaNumericEnterButtonStyle;

        /// <summary>
        /// Style for StoreMode Backspace button
        /// </summary>
        private Style _storeModeBackspaceButtonStyle;

        /// <summary>
        /// Style for StoreMode AlphaNumeric Space button
        /// </summary>
        private Style _storeModeAlphaNumericSpaceButtonStyle;

        /// <summary>
        /// Style for Search Enter button
        /// </summary>
        private Style _searchEnterButtonStyle;

        /// <summary>
        /// Style for AlphaNumeric Go Back button
        /// </summary>
        private Style _alphaNumericGoBackButtonStyle;

        /// <summary>
        /// Style for StoreMode AlphaNumeric Go Back button
        /// </summary>
        private Style _storeModeAlphaNumericGoBackButtonStyle;

        /// <summary>
        /// Style for AlphaNumeric Textbox
        /// </summary>
        private Style _alphaNumericTextBoxStyle;

        /// <summary>
        /// Style for StoreMode AlphaNumeric Textbox
        /// </summary>
        private Style _storeModeAlphaNumericTextBoxStyle;

        /// <summary>
        /// Initializes a new instance of the <see cref="AlphaNumericKeyboard" /> class.
        /// </summary>
        public AlphaNumericKeyboard()
        {
            InitializeComponent();
            InitKeypadStyle();
        }

        /// <summary>
        /// Occurs when keyboard button is clicked.
        /// </summary>
        public event RoutedEventHandler KeyboardButtonClick;

        /// <summary>
        /// Occurs when shift button is clicked.
        /// </summary>
        public event RoutedEventHandler ShiftButtonClick;

        /// <summary>
        /// Occurs when enter button is clicked.
        /// </summary>
        public event RoutedEventHandler EnterButtonClick;

        /// <summary>
        /// Occurs when after enter button is touched.
        /// </summary>
        public event EventHandler<TouchEventArgs> EnterButtonTouchUp;

        /// <summary>
        /// Occurs when there is a text changed in the InputTextBox
        /// </summary>
        public event EventHandler<TextChangedEventArgs> InputTextBoxTextChanged;

        /// <summary>
        /// Gets or sets a value for param in the list
        /// </summary>
        /// <value>List of parameters</value>
        public static List<string> ListForInputMasked
        {
            get { return AlphaNumericKeyboard.ParamListForInputMasked; }
            set { AlphaNumericKeyboard.ParamListForInputMasked = value; }
        }

        /// <summary>
        /// Gets or sets a value indicating whether or not the Background View mode is true or false.
        /// </summary>
        /// <value>true or false</value>
        public bool BackgroundViewMode
        {
            get { return Convert.ToBoolean(GetValue(BackgroundViewModeProperty)); }
            set { SetValue(BackgroundViewModeProperty, value); }
        }

        /// <summary>
        /// Gets or sets a value indicating whether ShowNumericKeypad is true.
        /// If true, numeric keypad will be visible 
        /// else, numeric keypad is hidden
        /// </summary>
        /// <returns>Whether ShowNumericKeypad is true.</returns>
        public bool ShowNumericKeypad
        {
            get
            {
                return Convert.ToBoolean(GetValue(ShowNumericKeypadProperty));
            }

            set
            {
                SetValue(ShowNumericKeypadProperty, value);
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether the Keyboard is behind the popup or not.
        /// </summary>
        /// <value>Boolean value either True or False.</value>
        public bool IsKeyboardBehindPopup
        {
            get
            {
                return _keyboardShownBehindPopup;
            }

            set
            {
                _keyboardShownBehindPopup = value;
            }
        }

        /// <summary>
        /// Method for setting the initial style for the keypad
        /// </summary>
        public void InitKeypadStyle()
        {
            _alphaNumericKeypadStyle = this.FindResource("alphaNumericKeypadButtonStyle") as Style;
            _alphaNumericHotKeypadStyle = this.FindResource("alphaNumericHotKeypadButtonStyle") as Style;
            _storeModeAlphaNumericKeypadStyle = this.FindResource("storeModeAlphaNumericKeypadButtonStyle") as Style;
            _storeModeAlphaNumericHotKeypadStyle = this.FindResource("storeModeAlphaNumericHotKeypadButtonStyle") as Style;
            _searchKeypadButtonStyle = this.FindResource("searchKeypadButtonStyle") as Style;
            _searchWatermarkTextboxStyle = this.FindResource("itemSearchTextBoxStyle") as Style;
            _alphaNumericTextboxBorderStyle = this.FindResource("popupTextBoxBorderStyle") as Style;
            _alphaNumericTextboxInnerShadowStyle = this.FindResource("popupTextBoxInnerShadowStyle") as Style;
            _searchTextboxBorderStyle = this.FindResource("backgroundTextBoxBorderStyle") as Style;
            _searchTextboxInnerShadowStyle = this.FindResource("backgroundTextBoxInnerShadowStyle") as Style;
            _alphaNumericEnterButtonStyle = this.FindResource("alphaNumericEnterButtonStyle") as Style;
            _alphaNumericBackspaceButtonStyle = this.FindResource("backSpaceButtonStyle") as Style;
            _alphaNumericSpaceButtonStyle = this.FindResource("alphaNumericSpaceButtonStyle") as Style;
            _storeModeAlphaNumericEnterButtonStyle = this.FindResource("storeModeEnterButtonStyle") as Style;
            _storeModeBackspaceButtonStyle = this.FindResource("storeBackSpaceButtonStyle") as Style;
            _storeModeAlphaNumericSpaceButtonStyle = this.FindResource("storeModeAlphanumericSpaceButtonStyle") as Style;
            _searchEnterButtonStyle = this.FindResource("mainButtonStyle") as Style;
            _alphaNumericGoBackButtonStyle = this.FindResource("buttonGoBackStyle") as Style;
            _storeModeAlphaNumericGoBackButtonStyle = this.FindResource("storeModeButtonGoBackStyle") as Style;
            _alphaNumericTextBoxStyle = this.FindResource("alphanumericTextBoxStyle") as Style;
            _storeModeAlphaNumericTextBoxStyle = this.FindResource("storeModeAlphanumericTextBoxStyle") as Style;
        }

        /// <summary>
        /// Method for setting the initial properties for keyboard
        /// </summary>
        public void InitKeyboardProperties()
        {
            _ExceededKeysCollection = new ObservableCollection<GridItem>();
        }

        /// <summary>
        /// Method for retrieving the properties for custom keyboard
        /// </summary>
        public void RetrieveCustomKeyboardProperties()
        {
            this.InputTextBox.Clear();
            ClearKeysItemSources();

            _Line1AlphaNumKeys = UIControlFinder.FindVisualChild<UniformGrid>(Line1Keys, "Line1AlphaNumKeys") as UniformGrid;
            _Line2AlphaNumKeys = UIControlFinder.FindVisualChild<UniformGrid>(Line2Keys, "Line2AlphaNumKeys") as UniformGrid;
            _Line3AlphaNumKeys = UIControlFinder.FindVisualChild<UniformGrid>(Line3Keys, "Line3AlphaNumKeys") as UniformGrid;
            _HotKeys = UIControlFinder.FindVisualChild<UniformGrid>(HotkeysItemsControl, "HotKeys") as UniformGrid;
            _ExceededKeysUniformGrid = UIControlFinder.FindVisualChild<UniformGrid>(ExceededKeysItemsControl, "ExceededKeysUniformGrid") as UniformGrid;

            BindAlphaLineKeys();
            BindNumLineKeys();
            BindHotKeys();

            if (BackgroundViewMode)
            {
                this.InputTextBox.Style = _searchWatermarkTextboxStyle;
                this.InputTextboxBorder.Style = _searchTextboxBorderStyle;
                this.InputTextboxInnerShadow.Style = _searchTextboxInnerShadowStyle;
                this.EnterButton.Style = _searchEnterButtonStyle;
                this.AlphanumericBackspaceButton.Style = _alphaNumericBackspaceButtonStyle;
                this.SpaceButton.Style = _alphaNumericSpaceButtonStyle;
                this.GoBackButton.Style = _alphaNumericGoBackButtonStyle;
            }
            else if (_viewModel.StoreMode)
            {
                this.InputTextBox.Style = _storeModeAlphaNumericTextBoxStyle;
                this.InputTextboxBorder.Style = _alphaNumericTextboxBorderStyle;
                this.InputTextboxInnerShadow.Style = _alphaNumericTextboxInnerShadowStyle;
                this.EnterButton.Style = _storeModeAlphaNumericEnterButtonStyle;
                this.AlphanumericBackspaceButton.Style = _storeModeBackspaceButtonStyle;
                this.SpaceButton.Style = _storeModeAlphaNumericSpaceButtonStyle;
                this.GoBackButton.Style = _storeModeAlphaNumericGoBackButtonStyle;
            }
            else
            {
                this.InputTextBox.Style = _alphaNumericTextBoxStyle;
                this.InputTextboxBorder.Style = _alphaNumericTextboxBorderStyle;
                this.InputTextboxInnerShadow.Style = _alphaNumericTextboxInnerShadowStyle;
                this.EnterButton.Style = _alphaNumericEnterButtonStyle;
                this.AlphanumericBackspaceButton.Style = _alphaNumericBackspaceButtonStyle;
                this.SpaceButton.Style = _alphaNumericSpaceButtonStyle;
                this.GoBackButton.Style = _alphaNumericGoBackButtonStyle;
            }

            _hotKeysButtons = UIControlFinder.FindVisualChildren<ImageToggleButton>(this.HotkeysItemsControl);
            ResetHotkeysButtonState();
            SetHotkeyButtonState(ShiftButton, false);

            AddNumLineExtraKeysToExceededKeysCollection();
            AddExceededKeysToTheLeastAlphaNumLines();
            SetShiftKeyButtonWidth();
            SetBackspaceButtonWidth();
            UpdateSymbolsButtonText();
        }

        /// <summary>
        /// Method for getting the value for KeyCode
        /// </summary>
        /// <param name="key">This is a parameter with a type of string</param> 
        /// <returns>the value of the KeyCode, none otherwise</returns>
        public Key KeyCode(string key)
        {
            Key finalKey = Key.None;
            switch (key)
            {
                case "{":
                    key = "OPENBRACKETS";
                    break;
                case "}":
                    key = "CLOSEBRACKETS";
                    break;
                case "?":
                    key = "QUESTION";
                    break;
                case "~":
                    key = "TILDE";
                    break;
                case ")":
                    key = "D0";
                    break;
                case "!":
                    key = "D1";
                    break;
                case "#":
                    key = "D3";
                    break;
                case "$":
                    key = "D4";
                    break;
                case "%":
                    key = "D5";
                    break;
                case "^":
                    key = "D6";
                    break;
                case "&":
                    key = "D7";
                    break;
                case "*":
                    key = "D8";
                    break;
                case "(":
                    key = "D9";
                    break;
                case "+":
                    key = "PLUS";
                    break;
                case "/":
                    key = "BACKSLASH";
                    break;
                case "|":
                    key = "PIPE";
                    break;
                case ":":
                    key = "SEMICOLON";
                    break;
                case "=":
                    key = "PLUS";
                    break;
                case ",":
                    key = "COMMA";
                    break;
                case "-":
                    key = "MINUS";
                    break;
                case ".":
                    key = "PERIOD";
                    break;
                case "\"":
                    key = "QUOTES";
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
        /// Method for setting the button state for Hot Keys 
        /// </summary>
        public void SetHotKeysButtonState()
        {
            var hotkeys = _viewModel.GetPropertyValue("NextGenUIHotKeyState").ToString().Split(';');
            if (hotkeys.Length > 1)
            {
                var arrHotkey = hotkeys[0].Split('=');
                string hotkey = string.Empty;
                if (arrHotkey.Length > 1)
                {
                    hotkey = arrHotkey[1];
                }

                if (hotkey.Equals("Text"))
                {
                    ResetHotkeysButtonState();
                }
                else
                {
                    SetHotkeyButtonState(hotkey);
                }
            }
        }

        /// <summary>
        /// Method for setting AlphaNumeric Line Keys
        /// </summary>
        /// <param name="alphaNumKeysLine">This is a parameter with a type of string</param>
        public void SetAlphaNumLineKeys(string alphaNumKeysLine)
        {
            _Line1KeysCollection = _viewModel.GetPropertyValue(alphaNumKeysLine + "2") as ObservableCollection<GridItem>;
            _Line2KeysCollection = _viewModel.GetPropertyValue(alphaNumKeysLine + "3") as ObservableCollection<GridItem>;
            _Line3KeysCollection = _viewModel.GetPropertyValue(alphaNumKeysLine + "4") as ObservableCollection<GridItem>;
            _Line4KeysCollection = _viewModel.GetPropertyValue(alphaNumKeysLine + "1") as ObservableCollection<GridItem>;
        }

        /// <summary>
        /// Method for setting Hot Keys Line
        /// </summary>
        /// <param name="hotKeysLine">This is a parameter with a type of string</param>
        public void SetHotKeysLine(string hotKeysLine)
        {
            _HotKeysCollection = _viewModel.GetPropertyValue(hotKeysLine) as ObservableCollection<GridItem>;
        }

        /// <summary>
        /// Method for adding and removing event methods
        /// </summary>
        public void RemoveAddEvents()
        {
            _Line1KeysCollection.CollectionChanged -= Line1KeysCollection_CollectionChanged;
            _Line1KeysCollection.CollectionChanged += Line1KeysCollection_CollectionChanged;
            _Line2KeysCollection.CollectionChanged -= Line2KeysCollection_CollectionChanged;
            _Line2KeysCollection.CollectionChanged += Line2KeysCollection_CollectionChanged;
            _Line3KeysCollection.CollectionChanged -= Line3KeysCollection_CollectionChanged;
            _Line3KeysCollection.CollectionChanged += Line3KeysCollection_CollectionChanged;
            _Line4KeysCollection.CollectionChanged -= Line4KeysCollection_CollectionChanged;
            _Line4KeysCollection.CollectionChanged += Line4KeysCollection_CollectionChanged;
            _HotKeysCollection.CollectionChanged -= HotKeysCollection_CollectionChanged;
            _HotKeysCollection.CollectionChanged += HotKeysCollection_CollectionChanged;
        }

        /// <summary>
        /// Method for setting the visibility of the Go Back button
        /// </summary>
        /// <param name="isVisible">This is a parameter with a type of boolean</param>
        public void SetGoBackButtonVisibility(bool isVisible)
        {
            GoBackButton.Visibility = isVisible ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Method for setting the state for Shift button
        /// </summary>
        public void SetShiftButtonState()
        {
            var hotkeys = _viewModel.GetPropertyValue("NextGenUIHotKeyState").ToString().Split(';');
            if (hotkeys.Length > 1)
            {
                var arrHotkey = hotkeys[0].Split('=');
                if (arrHotkey.Length > 1)
                {
                    ShiftButton.IsEnabled = arrHotkey[1].Equals("TextSymbolOn") ? false : true;
                }
                
                arrHotkey = hotkeys[1].Split('=');
                if (arrHotkey.Length > 1)
                {
                    bool shiftOn = false;
                    bool.TryParse(arrHotkey[1], out shiftOn);
                    SetHotkeyButtonState(ShiftButton, shiftOn);
                }
            }
        }

        /// <summary>
        /// Method for setting the visibility of the Shift key button.
        /// </summary>
        public void ShowShiftKey()
        {
            _shiftShown = Convert.ToBoolean(_viewModel.GetPropertyValue("ShiftKeyShown"));
            if (!_shiftShown)
            {
                this.ShiftButton.Visibility = Visibility.Collapsed;
            }
            else
            {
                this.ShiftButton.Visibility = Visibility.Visible;
                this.ShiftButton.IsChecked = false;
            }
        }

        /// <summary>
        /// Method for Symbols Key button visibility
        /// </summary>
        public void ShowSymbolsKey()
        {
            _symbolsShown = Convert.ToBoolean(_viewModel.GetPropertyValue("SymbolsKeyShown"));
            this.SymbolsButton.Visibility = _symbolsShown ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// Updates the SymbolsButton key text value.
        /// </summary>
        public void UpdateSymbolsButtonText()
        {
            this.SymbolsButton.Text = _viewModel.GetPropertyValue("SymbolsKeyText").ToString();
        }

        /// <summary>
        /// Method for invoking Button Click.
        /// </summary>
        /// <param name="btn">This is a parameter with a type of Button</param> 
        private void InvokeButtonClick(Button btn)
        {
            ButtonAutomationPeer peer = new ButtonAutomationPeer(btn);
            IInvokeProvider invokeProvider = peer.GetPattern(PatternInterface.Invoke) as IInvokeProvider;
            if (invokeProvider != null)
            {
                invokeProvider.Invoke();
            }
        }

        /// <summary>
        /// Collection Changed event method for Line1 Keys Collection
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of NotifyCollectionChangedEventArgs</param> 
        private void Line1KeysCollection_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Replace && ReplaceBindValues(sender))
            {
                Line1Keys.ItemsSource = null;
                CmDataCapture.CaptureFormat(CmDataCapture.MaskInfo, "+BindLineKeys Line1Keys");
                BindLineKeys(_Line1KeysCollection, Line1Keys, _Line1AlphaNumKeys, "AlphaNumP1", "Line1Alpha");

                _line1Updated = true;
                AddExceededKeysToKeyboard();
            }
        }

        /// <summary>
        /// Collection Changed event method for Line2 Keys Collection
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of NotifyCollectionChangedEventArgs</param> 
        private void Line2KeysCollection_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Replace && ReplaceBindValues(sender))
            {
                Line2Keys.ItemsSource = null;
                CmDataCapture.CaptureFormat(CmDataCapture.MaskInfo, "+BindLineKeys Line2Keys");
                BindLineKeys(_Line2KeysCollection, Line2Keys, _Line2AlphaNumKeys, "AlphaNumP4", string.Empty);

                _line2Updated = true;
                AddExceededKeysToKeyboard();
            }
        }

        /// <summary>
        /// Collection Changed event method for Line3 Keys Collection
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of NotifyCollectionChangedEventArgs</param> 
        private void Line3KeysCollection_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Replace && ReplaceBindValues(sender))
            {
                Line3Keys.ItemsSource = null;
                CmDataCapture.CaptureFormat(CmDataCapture.MaskInfo, "+BindLineKeys Line3Keys");
                BindLineKeys(_Line3KeysCollection, Line3Keys, _Line3AlphaNumKeys, "AlphaNumP2", string.Empty);

                _line3Updated = true;
                AddExceededKeysToKeyboard();
            }
        }

        /// <summary>
        /// Collection Changed event method for Line4 Keys Collection
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of NotifyCollectionChangedEventArgs</param> 
        private void Line4KeysCollection_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Replace && ReplaceBindValues(sender))
            {
                _ExceededKeysCollection.Clear();
                ExceededKeysItemsControl.ItemsSource = null;
                BindNumLineKeys();
            }
        }

        /// <summary>
        /// Collection Changed event method for Hot Keys Collection
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of NotifyCollectionChangedEventArgs</param> 
        private void HotKeysCollection_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Replace && ReplaceBindValues(sender))
            {
                HotkeysItemsControl.ItemsSource = null;
                if (_viewModel.IsUNavEnabled)
                {
                    if (HotKeysHasFocus())
                    {
                        HotkeysItemsControl.Focus();
                    }
                }

                BindHotKeys();
            }
        }

        /// <summary>
        /// Checks if the HotKey buttons has focus
        /// </summary>
        /// <returns>Returns true if the HotKey button is focused. Otherwise, false</returns>
        private bool HotKeysHasFocus()
        {
            bool b = false;
            if (_hotKeysButtons != null)
            {
                foreach (UIElement item in _hotKeysButtons)
                {
                    if (item.IsFocused)
                    {
                        b = true;
                        break;
                    }
                }
            }

            return b;
        }

        /// <summary>
        /// Method for adding exceeded keys to keyboard
        /// </summary>
        private void AddExceededKeysToKeyboard()
        {
            if (_line1Updated && _line2Updated && _line3Updated)
            {
                AddNumLineExtraKeysToExceededKeysCollection();
                AddExceededKeysToTheLeastAlphaNumLines();
                SetShiftKeyButtonWidth();
                SetBackspaceButtonWidth();
                _line3Updated = false;
                _line2Updated = false;
                _line1Updated = false;
            }
        }

        /// <summary>
        /// Method indicating to replace bind values in GridItem or not
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param>  
        /// <returns>true gridItem in not null. false otherwise</returns>
        private bool ReplaceBindValues(object sender)
        {
            var gridItem = sender as ObservableCollection<GridItem>;
            if (gridItem != null && gridItem[gridItem.Count - 1].Text != null && gridItem[gridItem.Count - 1].Data != null)
            {
                return true;
            }

            return false;
        }

        /// <summary>
        /// Method for binding AlphaNumeric line keys
        /// </summary>
        private void BindAlphaLineKeys()
        {
            CmDataCapture.CaptureFormat(CmDataCapture.MaskInfo, "+BindAlphaLineKeys");
            BindLineKeys(_Line1KeysCollection, Line1Keys, _Line1AlphaNumKeys, "AlphaNumP1", "Line1Alpha");
            BindLineKeys(_Line2KeysCollection, Line2Keys, _Line2AlphaNumKeys, "AlphaNumP4", string.Empty);
            BindLineKeys(_Line3KeysCollection, Line3Keys, _Line3AlphaNumKeys, "AlphaNumP2", string.Empty);
        }

        /// <summary>
        /// Method for binding Numeric line keys
        /// </summary>
        private void BindNumLineKeys()
        {
            CmDataCapture.CaptureFormat(CmDataCapture.MaskInfo, "+BindNumLineKeys");
            BindLineKeys(_Line4KeysCollection, NumericLine1Keys, "AlphaNumP1", "Line1Numeric;Columns=3;Row=1");
            BindLineKeys(_Line4KeysCollection, NumericLine2Keys, "AlphaNumP1", "Line1Numeric;Columns=3;Row=2");
            BindLineKeys(_Line4KeysCollection, NumericLine3Keys, "AlphaNumP1", "Line1Numeric;Columns=3;Row=3");
            BindLineKeys(_Line4KeysCollection, NumericLine4Keys, "AlphaNumP1", "Line1Numeric;Columns=3;Row=4");
        }

        /// <summary>
        /// Method for binding Hot keys
        /// </summary>
        private void BindHotKeys()
        {
            CmDataCapture.CaptureFormat(CmDataCapture.MaskInfo, "+BindHotKeys");
            BindLineKeys(_HotKeysCollection, HotkeysItemsControl, _HotKeys, string.Empty, string.Empty);
        }

        /// <summary>
        /// Method for binding line keys and alphanumeric keys
        /// </summary>
        /// <param name="items">This is a parameter with a type of ObservableCollection</param> 
        /// <param name="lineKey">This is a parameter with a type of ItemsControl</param> 
        /// <param name="alphaNumKeys">This is a parameter with a type of UniformGrid</param> 
        /// <param name="propertyName">This is a parameter with a type of string</param> 
        /// <param name="converterParam">This is a parameter with a type of String</param> 
        private void BindLineKeys(ObservableCollection<GridItem> items, ItemsControl lineKey, UniformGrid alphaNumKeys, string propertyName, string converterParam)
        {
            if (items != null && alphaNumKeys != null)
            {
                if (!lineKey.Name.Equals("ExceededKeysItemsControl"))
                {
                    StoreKeysToExceededKeysCollection(items, MaxLineKeysLimit(lineKey.Name));
                }

                BindLineKeys(items, lineKey, propertyName, converterParam);
                alphaNumKeys.Columns = (lineKey.ItemsSource as ObservableCollection<GridItem>).Count;
                UpdateKeypadButtonStyle(MaxColumnsCount());
            }
        }

        /// <summary>
        /// Method for binding line keys 
        /// </summary>
        /// <param name="items">This is a parameter with a type of ObservableCollection</param> 
        /// <param name="lineKey">This is a parameter with a type of ItemsControl</param> 
        /// <param name="propertyName">This is a parameter with a type of string</param> 
        /// <param name="converterParam">This is a parameter with a type of String</param> 
        private void BindLineKeys(ObservableCollection<GridItem> items, ItemsControl lineKey, string propertyName, string converterParam)
        {
            if (items != null)
            {
                MultiBinding mb = new MultiBinding();
                mb.ConverterParameter = converterParam;
                mb.Converter = new AlphaNumericKeysHandler();
                mb.Bindings.Add(new Binding() { Source = items });
                mb.Bindings.Add(new Binding() { Source = _viewModel.GetPropertyValue(propertyName) });
                mb.Bindings.Add(new Binding() { Source = (int)MaxLineKeysLimit(lineKey.Name) });
                BindingOperations.SetBinding(lineKey, ItemsControl.ItemsSourceProperty, mb);

                CmDataCapture.CaptureFormat(CmDataCapture.MaskInfo, "+BindLineKeys success. LineKey:{0}", lineKey.Name);
            }
        }

        /// <summary>
        /// Method for adding keys to exceeded keys collection
        /// </summary>
        /// <param name="items">This is a parameter with a type of ObservableCollection</param> 
        /// <param name="maxKeysLimit">This is a parameter with a type of integer</param> 
        private void StoreKeysToExceededKeysCollection(ObservableCollection<GridItem> items, int maxKeysLimit)
        {
            ObservableCollection<GridItem> val = items;

            if (val != null)
            {
                foreach (GridItem item in val)
                {
                    if (item != null && item.Text != null
                        && val.IndexOf(item) >= maxKeysLimit
                        && !_ExceededKeysCollection.Contains(item))
                    {
                        _ExceededKeysCollection.Add(item);
                    }
                }
            }
        }

        /// <summary>
        /// Method for determining the maximum number of line keys
        /// </summary>
        /// <param name="lineKeyName">The name of the ItemsControl</param> 
        /// <returns>max keys allowed</returns> 
        private int MaxLineKeysLimit(string lineKeyName)
        {
            switch (lineKeyName)
            {
                case "Line1Keys":
                case "Line2Keys":
                    return 11;
                case "Line3Keys":
                case "NumericLine4Keys":
                    return 10;
                default:
                    return 11;
            }
        }

        /// <summary>
        /// Click event method for Hot Keys
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of RoutedEventArgs</param>
        private void HotKeys_Click(object sender, RoutedEventArgs e)
        {
            ImageToggleButton button = sender as ImageToggleButton;
            if (button != null)
            {
                GridItem item = button.DataContext as GridItem;
                if (item != null)
                {
                    if (BackgroundViewMode)
                    {
                        _viewModel.ActionCommand.Execute(string.Format("PFKBHotKeysClick({0})", item.Data));
                    }
                    else
                    {
                        _viewModel.ActionCommand.Execute(string.Format("HotKeysClick({0})", item.Data));
                    }
                }
            }
        }

        /// <summary>
        /// Loaded event handler for Hot Keys
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of RoutedEventArgs</param>
        private void HotKeys_Loaded(object sender, RoutedEventArgs e)
        {
            SetHotKeysButtonState();
        }

        /// <summary>
        /// Loaded event handler for Symbols button.
        /// </summary>
        /// <param name="sender">Sending object.</param> 
        /// <param name="e">Event arguments.</param>
        private void SymbolsButton_Loaded(object sender, RoutedEventArgs e)
        {
            if (!_keyboardShownBehindPopup)
            {
                ShowSymbolsKey();
            }
        }

        /// <summary>
        /// Loaded event handler for Shift button
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of RoutedEventArgs</param>
        private void ShiftButton_Loaded(object sender, RoutedEventArgs e)
        {
            if (!_keyboardShownBehindPopup)
            {
                ShowShiftKey();
            }

            SetShiftButtonState();
        }

        /// <summary>
        /// Method for resetting the button state for Hot Keys
        /// </summary>
        private void ResetHotkeysButtonState()
        {
            if (_hotKeysButtons != null)
            {
                foreach (ImageToggleButton btn in _hotKeysButtons)
                {
                    SetHotkeyButtonState(btn, false);
                }
            }
        }

        /// <summary>
        /// Method for setting the button state for Hot Keys with hotkey as parameter
        /// </summary>
        /// <param name="hotKey">This is a parameter with a type of string</param>
        private void SetHotkeyButtonState(string hotKey)
        {
            if (_hotKeysButtons != null)
            {
                foreach (ImageToggleButton btn in _hotKeysButtons)
                {
                    if (!btn.Tag.ToString().Equals(string.Empty) && hotKey.Contains(btn.Tag.ToString().Substring(2)))
                    {
                        SetHotkeyButtonState(btn, true);
                    }
                    else
                    {
                        SetHotkeyButtonState(btn, false);
                    }
                }
            }
        }

        /// <summary>
        /// Method for setting the button state for Hot Keys with hotkey as parameter plus IsChecked parameter
        /// </summary>
        /// <param name="hotKey">This is a parameter with a type of ImageToggleButton</param>
        /// <param name="IsChecked">This is a parameter with a type of boolean</param>
        private void SetHotkeyButtonState(ImageToggleButton hotKey, bool IsChecked)
        {
            if (hotKey != null)
            {
                hotKey.IsChecked = IsChecked;
                if (_viewModel.StoreMode)
                {
                    hotKey.Background = IsChecked && hotKey.IsEnabled ? TryFindResource("buttonSelectedBackgroundColorBrush") as Brush : TryFindResource("storeModeMainButtonBackgroundColorBrush") as Brush;
                }
                else
                {
                    hotKey.Background = IsChecked && hotKey.IsEnabled ? TryFindResource("buttonSelectedBackgroundColorBrush") as Brush : TryFindResource("mainButtonBackgroundColorBrush") as Brush;
                }
            }
        }

        /// <summary>
        /// Method for adding exceeded keys to the least AlphaNumeric lines
        /// </summary>
        private void AddExceededKeysToTheLeastAlphaNumLines()
        {
            ObservableCollection<GridItem> val = _ExceededKeysCollection;
            ObservableCollection<GridItem> retVal = new ObservableCollection<GridItem>();

            if (val != null)
            {
                foreach (GridItem item in val)
                {
                    if (item != null && item.Text != null)
                    {
                        if (AddToTheLeastItemsControlCount(item))
                        {
                            retVal.Add(item);
                        }
                    }
                }

                if (retVal.Count > 0)
                {
                    CmDataCapture.CaptureFormat(CmDataCapture.MaskInfo, "+ExceededKeys");
                    BindLineKeys(retVal, ExceededKeysItemsControl, _ExceededKeysUniformGrid, string.Empty, string.Empty);
                }
            }
        }

        /// <summary>
        /// Method for adding to the least items control count
        /// </summary>
        /// <param name="item">This is a parameter with a type of GridItem</param>
        /// <returns>true if Item Source is not null, false otherwise</returns>
        private bool AddToTheLeastItemsControlCount(GridItem item)
        {
            var Line1KeysItemsSource = Line1Keys.ItemsSource;
            var Line2KeysItemsSource = Line2Keys.ItemsSource;
            var Line3KeysItemsSource = Line3Keys.ItemsSource;

            bool addToUniformGrid = false;

            if (Line1KeysItemsSource != null
                && Line2KeysItemsSource != null
                && Line3KeysItemsSource != null)
            {
                int line1KeysCount = _Line1KeysCollection.Count;
                int line2KeysCount = _Line2KeysCollection.Count;
                int line3KeysCount = _Line3KeysCollection.Count;

                if (line1KeysCount >= 11
                    && line2KeysCount >= 11
                    && line3KeysCount >= 10
                    && !ExceededKeysItemsControl.Items.Contains(item))
                {
                    addToUniformGrid = true;

                    if (_HotKeysCollection.Count > 4)
                    {
                        ShiftButton.Width = 58;
                    }
                }
                else
                {
                    ObservableCollection<GridItem> LineKeysCollectionWithExtraKeys = new ObservableCollection<GridItem>();

                    if (line3KeysCount < 10 && !(Line3KeysItemsSource as ObservableCollection<GridItem>).Contains(item))
                    {
                        LineKeysCollectionWithExtraKeys = Line3Keys.ItemsSource as ObservableCollection<GridItem>;
                        LineKeysCollectionWithExtraKeys.Add(item);
                        BindLineKeys(LineKeysCollectionWithExtraKeys, Line3Keys, _Line3AlphaNumKeys, "AlphaNumP2", string.Empty);
                    }
                    else if (line1KeysCount <= line2KeysCount && !(Line1KeysItemsSource as ObservableCollection<GridItem>).Contains(item))
                    {
                        LineKeysCollectionWithExtraKeys = Line1Keys.ItemsSource as ObservableCollection<GridItem>;
                        LineKeysCollectionWithExtraKeys.Add(item);
                        BindLineKeys(LineKeysCollectionWithExtraKeys, Line1Keys, _Line1AlphaNumKeys, "AlphaNumP1", "Line1Alpha");
                    }
                    else if (line2KeysCount < line1KeysCount && !(Line2KeysItemsSource as ObservableCollection<GridItem>).Contains(item))
                    {
                        LineKeysCollectionWithExtraKeys = Line2Keys.ItemsSource as ObservableCollection<GridItem>;
                        LineKeysCollectionWithExtraKeys.Add(item);
                        BindLineKeys(LineKeysCollectionWithExtraKeys, Line2Keys, _Line2AlphaNumKeys, "AlphaNumP4", string.Empty);
                    }
                }
            }

            return addToUniformGrid;
        }

        /// <summary>
        /// Method for adding numeric line extra keys to exceeded keys collection
        /// </summary>
        private void AddNumLineExtraKeysToExceededKeysCollection()
        {
            if (_Line4KeysCollection != null && _Line4KeysCollection.Count > 0)
            {
                if (_Line4KeysCollection.Count >= 11
                    && !_ExceededKeysCollection.Contains(_Line4KeysCollection[10] as GridItem))
                {
                    _ExceededKeysCollection.Add(_Line4KeysCollection[10] as GridItem);
                }

                if (_Line4KeysCollection.Count >= 12
                    && !_ExceededKeysCollection.Contains(_Line4KeysCollection[11] as GridItem))
                {
                    _ExceededKeysCollection.Add(_Line4KeysCollection[11] as GridItem);
                }
            }
        }

        /// <summary>
        /// Method to count the number of children in the StackPanel
        /// </summary>
        /// <param name="sp">This is a parameter with a type of StackPanel</param>
        /// <returns>integer value, the number of children</returns>
        private int ChildrenCount(StackPanel sp)
        {
            int count = 0;
            for (int i = 0; i < sp.Children.Count; i++)
            {
                if ((sp.Children[i] as FrameworkElement).Visibility == Visibility.Visible)
                {
                    count++;
                }
            }

            return count;
        }

        /// <summary>
        /// Check if a character exist in a collection
        /// </summary>
        /// <param name="data">This is a parameter with a type of string</param>
        /// <returns>true if there are character/s in the collection, false otherwise</returns>
        private bool IsCharacterExist(string data)
        {
            ObservableCollection<GridItem>[] items = { _Line1KeysCollection, _Line2KeysCollection, _Line3KeysCollection, _Line4KeysCollection };
            for (int i = 0; i < items.Length; i++)
            {
                foreach (GridItem item in items[i])
                {
                    if (item.Text != null && item.Text.Equals(data))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        /// <summary>
        /// Method for counting the maximum number of columns
        /// </summary>
        /// <returns>integer value for the number of columns</returns>
        private int MaxColumnsCount()
        {
            if (_Line1AlphaNumKeys != null && _Line2AlphaNumKeys != null && _Line3AlphaNumKeys != null)
            {
                int[] columnsCount = { _Line1AlphaNumKeys.Columns, _Line2AlphaNumKeys.Columns, _Line3AlphaNumKeys.Columns };
                return columnsCount.Max();
            }

            return 0;
        }

        /// <summary>
        /// Update the Keypad button style to either AlphaNumericKeypad, NumericLineKeypad, AlphaNumericHotKeypad
        /// </summary>
        /// <param name="columns">This is a parameter with a type of integer</param>
        private void UpdateKeypadButtonStyle(int columns)
        {
            if (_Line1AlphaNumKeys != null && _Line2AlphaNumKeys != null && _Line3AlphaNumKeys != null)
            {
                if (_Line1KeysCollection != null && _Line2KeysCollection != null && _Line3KeysCollection != null && _Line4KeysCollection != null && columns > 0)
                {
                    if (BackgroundViewMode)
                    {
                        UpdateKeypadButtonStyle(new Style { TargetType = typeof(ImageButton) }, _searchKeypadButtonStyle, columns, "alphaNumericKeypadButtonStyle");
                        UpdateKeypadButtonStyle(new Style { TargetType = typeof(ImageButton) }, _searchKeypadButtonStyle, columns, "numericLineKeypadButtonZeroStyle");
                    }
                    else if (_viewModel.StoreMode)
                    {
                        UpdateKeypadButtonStyle(new Style { TargetType = typeof(ImageButton) }, _storeModeAlphaNumericKeypadStyle, columns, "alphaNumericKeypadButtonStyle");
                        UpdateKeypadButtonStyle(new Style { TargetType = typeof(ImageButton) }, _storeModeAlphaNumericKeypadStyle, columns, "numericLineKeypadButtonZeroStyle");
                    }
                    else
                    {
                        UpdateKeypadButtonStyle(new Style { TargetType = typeof(ImageButton) }, _alphaNumericKeypadStyle, columns, "alphaNumericKeypadButtonStyle");
                        UpdateKeypadButtonStyle(new Style { TargetType = typeof(ImageButton) }, _alphaNumericKeypadStyle, columns, "numericLineKeypadButtonZeroStyle");
                    }

                    if (_viewModel.StoreMode)
                    {
                        UpdateKeypadButtonStyle(new Style { TargetType = typeof(ImageToggleButton) }, _storeModeAlphaNumericHotKeypadStyle, columns, "alphaNumericHotKeypadButtonStyle");
                    }
                    else
                    {
                        UpdateKeypadButtonStyle(new Style { TargetType = typeof(ImageToggleButton) }, _alphaNumericHotKeypadStyle, columns, "alphaNumericHotKeypadButtonStyle");
                    }
                }
            }
        }

        /// <summary>
        /// Method for setting the width of the Shift Key button
        /// </summary>
        private void SetShiftKeyButtonWidth()
        {
            if (_ExceededKeysCollection.Count > 2 || _HotKeysCollection.Count > 2)
            {
                this.ShiftButton.Width = 58;
            }
            else
            {
                this.ShiftButton.Width = 100;
            }
        }

        /// <summary>
        /// Method for setting the width of the Backspace button.
        /// </summary>
        private void SetBackspaceButtonWidth()
        {
            if (_ExceededKeysCollection.Count > 2 || _HotKeysCollection.Count > 2)
            {
                AlphanumericBackspaceButton.Width = 58;
            }
            else
            {
                AlphanumericBackspaceButton.Width = 100;
            }
        }

        /// <summary>
        /// Unloaded event method/handler for Keyboard
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param>
        /// <param name="e">This is a parameter with a type of RoutedEventArgs</param>
        private void Keyboard_UnLoaded(object sender, RoutedEventArgs e)
        {
            ClearKeysItemSources();
        }

        /// <summary>
        /// Method for clearing the Keys Item Sources
        /// </summary>
        private void ClearKeysItemSources()
        {
            Line1Keys.ItemsSource = null;
            Line2Keys.ItemsSource = null;
            Line3Keys.ItemsSource = null;
            ExceededKeysItemsControl.ItemsSource = null;
            HotkeysItemsControl.ItemsSource = null;
        }

        /// <summary>
        /// Update the Keypad button thickness if it is NumericLineKeypad button zero.
        /// </summary>
        /// <param name="newStyle">This is a parameter with a type of Style</param>
        /// <param name="baseOnStyle">This is a parameter with a type of style</param>
        /// <param name="columns">This is a parameter with a type of integer</param>
        /// <param name="resourceKey">This is a parameter with a type of string</param>
        private void UpdateKeypadButtonStyle(Style newStyle, Style baseOnStyle, int columns, string resourceKey)
        {
            newStyle.BasedOn = baseOnStyle;
            newStyle.Setters.Add(new Setter(Button.MarginProperty, new Thickness(5.0)));

            if (resourceKey.Equals("numericLineKeypadButtonZeroStyle"))
            {
                newStyle.Setters.Add(new Setter(Button.MarginProperty, new Thickness(18, 5, 5, 5)));
            }

            this.Resources[resourceKey] = newStyle;
        }

        /// <summary>
        /// Data Context Changed event method for grid.
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of DependencyPropertyChangedEventArgs</param> 
        private void Grid_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            _viewModel = DataContext as IMainViewModel;
        }

        /// <summary>
        /// Click Event method for Enter button.
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of RoutedEventArgs</param> 
        private void EnterButton_Click(object sender, RoutedEventArgs e)
        {
            RoutedEventHandler tempHandler = EnterButtonClick;
            if (tempHandler != null)
            {
                tempHandler(sender, e);
            }
        }

        /// <summary>
        /// Touch Up event method for Enter button.
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of TouchEventArgs</param>
        private void EnterButton_TouchUp(object sender, TouchEventArgs e)
        {
            EventHandler<TouchEventArgs> tempHandler = EnterButtonTouchUp;
            if (tempHandler != null)
            {
                tempHandler(sender, e);
            }
        }

        /// <summary>
        /// Text Changed event method for Input TextBox.
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of TextChangedEventArgs</param>
        private void InputTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            EventHandler<TextChangedEventArgs> tempHandler = InputTextBoxTextChanged;
            if (tempHandler != null)
            {
                tempHandler(sender, e);
            }
            else
            {
                HandleBaseInputTextBox_TextChange();
            }

            if (_viewModel.IsUNavEnabled && AlphanumericBackspaceButton.IsFocused && !AlphanumericBackspaceButton.IsEnabled)
            {
                KeyboardSimulator.Instance.SimulateKeyPress(VirtualKeyCode.TAB);
            }
        }

        /// <summary>
        /// Text Changed method for handling Base Input TextBox.
        /// </summary>
        private void HandleBaseInputTextBox_TextChange()
        {
            this.AlphanumericBackspaceButton.IsEnabled = this.InputTextBox.Text.Length > 0 && !this.InputTextBox.IsWatermarkShown;
            this.EnterButton.IsEnabled = this.InputTextBox.Text.Length >= _MinInput && !this.InputTextBox.IsWatermarkShown;
        }

        /// <summary>
        /// Boolean method which returns a value for key either true or false
        /// </summary>
        /// <param name="key">This is a parameter with a type of Key</param> 
        /// <returns>true if key has value, false otherwise</returns>
        private bool IsSymbols(Key key)
        {
            bool result = false;
            switch (key)
            {
                case Key.OemOpenBrackets:
                case Key.OemCloseBrackets:
                case Key.OemQuestion:
                case Key.OemTilde:
                case Key.D0:
                case Key.D1:
                case Key.D3:
                case Key.D4:
                case Key.D5:
                case Key.D6:
                case Key.D7:
                case Key.D8:
                case Key.D9:
                case Key.OemPlus:
                case Key.OemBackslash:
                case Key.OemPipe:
                case Key.OemSemicolon:
                case Key.OemQuotes:
                    result = true;
                    break;
            }

            return result;
        }

        /// <summary>
        /// Preview touch down handler.
        /// TODO: Fix disabling the keyboard keys for 2 popups in a row more correctly.
        /// </summary>
        /// <param name="sender">object sender</param>
        /// <param name="e">event arguments</param>
        private void Grid_PreviewTouchDown(object sender, TouchEventArgs e)
        {
            // This fixes a strange case of being able to touch the disabled keyboard buttons behind the NMI popup.
            if (_viewModel.StateParam.Equals("NMIPopup"))
            {
                e.Handled = true;
                return;
            }
        }

        /// <summary>
        /// Click event method for Keyboard buttons.
        /// </summary>
        /// <param name="sender">This is a parameter with a type of object</param> 
        /// <param name="e">This is a parameter with a type of RoutedEventArgs</param>
        private void KeyboardButton_Click(object sender, RoutedEventArgs e)
        {
            FrameworkElement textBox = null;
            View view = UIControlFinder.FindAncestorOrSelf<View>(this);
            if (view != null)
            {
                textBox = view.KeyboardFocusedElement;
            }

            if (textBox == null)
            {
                return;
            }

            bool shiftOn = ShiftButton.IsChecked.Value;
            ImageButton button = sender as ImageButton;
            if (button != null)
            {
                if (!string.IsNullOrEmpty(button.Text))
                {
                    if (!char.IsDigit(button.Text, 0))
                    {
                        Key key = KeyCode(button.Text);
                        if (shiftOn && IsSymbols(key))
                        {
                            textBox.RaiseEvent(
                                new TextCompositionEventArgs(
                                    InputManager.Current.PrimaryKeyboardDevice,
                                    new TextComposition(
                                        InputManager.Current,
                                        textBox,
                                        string.Empty))
                                        {
                                            RoutedEvent = TextCompositionManager.TextInputEvent
                                        });
                            return;
                        }
                    }
                }

                button.IsInputMasked = ParamListForInputMasked.Contains(_viewModel.StateParam);
            }

            RoutedEventHandler tempHandler = KeyboardButtonClick;
            if (tempHandler != null)
            {
                tempHandler(sender, e);
            }
            else
            {
                HandleBaseKeyboardButton_Click(button, textBox);
            }
        }

        /// <summary>
        /// Click event method for handling Base Keyboard buttons.
        /// </summary>
        /// <param name="button">This is a parameter with a type of ImageButton</param> 
        /// <param name="textBox">This is a parameter with a type of FrameworkElement</param>
        private void HandleBaseKeyboardButton_Click(ImageButton button, FrameworkElement textBox)
        {
            if (button.CommandParameter != null)
            {
                Key key = KeyCode(button.CommandParameter.ToString());
                if (key != Key.None)
                {
                    textBox.RaiseEvent(new KeyEventArgs(Keyboard.PrimaryDevice, PresentationSource.FromVisual(textBox), 0, key) { RoutedEvent = Keyboard.KeyDownEvent });
                }
            }
            else
            {
                textBox.RaiseEvent(
                    new TextCompositionEventArgs(
                        InputManager.Current.PrimaryKeyboardDevice,
                        new TextComposition(
                            InputManager.Current, textBox, button.Text))
                            {
                                RoutedEvent = TextCompositionManager.TextInputEvent
                            });
            }
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
                _viewModel.ActionCommand.Execute("ShiftKey");
            }
        }

        /// <summary>
        /// Symbols button method handler for click event.
        /// </summary>
        /// <param name="sender">Event sender</param>
        /// <param name="e">Event arguments</param>
        private void SymbolsButton_Click(object sender, RoutedEventArgs e)
        {
            ImageButton button = sender as ImageButton;
            if (button != null)
            {
                _viewModel.ActionCommand.Execute("SymbolsKey");
            }
        }

        /// <summary>
        /// Reconfigures when show numeric keypad changes.
        /// </summary>
        private void ShowNumericKeypadChanged()
        {
            if (Convert.ToBoolean(ShowNumericKeypad))
            {
                NumericRow1.Visibility = Visibility.Visible;
                NumericRow2.Visibility = Visibility.Visible;
                NumericRow3.Visibility = Visibility.Visible;
                NumericRow4.Visibility = Visibility.Visible;
                EnterButton.Visibility = Visibility.Visible;
                GenericAlphaNumericKeyboard.ColumnDefinitions[0].Width = new GridLength(770);
                GenericAlphaNumericKeyboard.ColumnDefinitions[1].Width = new GridLength(8);
                GenericAlphaNumericKeyboard.ColumnDefinitions[2].Width = new GridLength(218);
            }
            else
            {
                NumericRow1.Visibility = Visibility.Collapsed;
                NumericRow2.Visibility = Visibility.Collapsed;
                NumericRow3.Visibility = Visibility.Collapsed;
                NumericRow4.Visibility = Visibility.Collapsed;
                EnterButton.Visibility = Visibility.Collapsed;
                GenericAlphaNumericKeyboard.ColumnDefinitions[0].Width = new GridLength(1, GridUnitType.Star);
                GenericAlphaNumericKeyboard.ColumnDefinitions[1].Width = new GridLength(0);
                GenericAlphaNumericKeyboard.ColumnDefinitions[2].Width = new GridLength(0);
            }
        }

        /// <summary>
        /// Event handler for Loaded event.
        /// </summary>
        /// <param name="sender">Sending object</param>
        /// <param name="e">Event arguments</param>
        private void AlphaNumericKeyboard_Loaded(object sender, RoutedEventArgs e)
        {
            ShowNumericKeypadChanged();
        }
    }
}
