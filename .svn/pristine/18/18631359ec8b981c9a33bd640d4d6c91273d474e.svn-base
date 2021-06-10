// <copyright file="Search.xaml.cs" company="NCR Corporation">
//     Copyright (c) NCR Corporation. All rights reserved.
// </copyright>
namespace SSCOUIViews.Views
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.Collections.Specialized;
    using System.ComponentModel;
    using System.Diagnostics;
    using System.Text.RegularExpressions;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Input;
    using System.Windows.Threading;
    using FPsxWPF.Controls;
    using RPSWNET;
    using SSCOControls;
    using SSCOControls.SlidingGridPageComponents;
    using SSCOControls.TouchListBoxComponents;
    using SSCOUIModels;
    using SSCOUIModels.Commands;
    using SSCOUIModels.Controls;
    using SSCOUIModels.Helpers;
    using SSCOUIModels.Models;

    /// <summary>
    /// Interaction logic for Search class
    /// </summary>
    public partial class Search : BackgroundView
    {
        /// <summary>
        /// Data string for Favorites tab
        /// </summary>
        private const string FAVORITES_DATA = "Favorites";

        /// <summary>
        /// Backing field for IsNMIPopupPending property
        /// </summary>
        private static bool _isNMIPopupPending;

        /// <summary>
        /// The Categories collection
        /// </summary>
        private ObservableCollection<GridItem> _categoryListSource;

        /// <summary>
        /// Backing field for SearchResultsView property
        /// </summary>
        private bool _searchResultsView;

        /// <summary>
        /// Backing field for ShowAlphaNumericKeyboard property
        /// </summary>
        private bool _showAlphaNumericKeyboard;

        /// <summary>
        /// Backing field for IsMultiPick property
        /// </summary>
        private bool _isMultipick;

        /// <summary>
        /// Flag if user has started to pick items on a MultiPick mode
        /// </summary>
        private bool _isPicking = false;

        /// <summary>
        /// Flag if user selected from QuickPick
        /// </summary>
        private bool _quickPickSelection;

        /// <summary>
        /// The container for Favorites tab
        /// </summary>
        private GridItem _favoritesItem = null;

        /// <summary>
        /// Backing field for KeyInCodeOnly property
        /// </summary>
        private bool _keyInCodeOnly;

        /// <summary>
        /// Backing field for NavigateOnly property
        /// </summary>
        private bool _navigateOnly;

        /// <summary>
        /// Flag indicating if we are subscribed to layout updated of the category list
        /// </summary>
        private bool _isLayoutUpdatedSubscribed = false;

        /// <summary>
        /// Initializes a new instance of the Search class
        /// </summary>
        /// <param name="viewModel">IMainViewModel type parameter</param>
        public Search(IMainViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
            InitAlphaNumericKeyboard();

            // Initialize properties
            this.IsMultipick = MultiPickPanel.Visibility == Visibility.Visible;
            this.ShowAlphaNumericKeyboard = GenericAlphaNumericKeyboard.Visibility == Visibility.Visible;
            this.SearchResultsView = false;

            _categoryListSource = viewModel.GetPropertyValue("SearchCategories") as ObservableCollection<GridItem>;
            _categoryListSource.CollectionChanged += CategoryList_CollectionChanged;

            SearchList.SwipeEvent += SearchList_SwipeEvent;

            if (CmDataCapture.IsCaptureActive(CmDataCapture.MaskExtensive))
            {
                SearchList.PerfLogging = true;
                SearchList.LogEvent += SearchList_LogEvent;
            }

            try
            {
                ((INotifyCollectionChanged)SearchList.ItemsSource).CollectionChanged += SearchList_CollectionChanged;
                ((INotifyCollectionChanged)PickListReceipt.ItemsSource).CollectionChanged += PickListReceipt_CollectionChanged;
            }
            catch
            {
                // Just ignore any exception that may occur
            }

            ResizeSearchList();

            PopupPendingChanged += Search_PopupPendingChanged;
        }

        /// <summary>
        /// Static event fired when popup pending changes
        /// </summary>
        private static event EventHandler PopupPendingChanged;

        /// <summary>
        /// Gets or sets a value indicating whether an NMI popup needs to show.
        /// </summary>
        public static bool IsNMIPopupPending
        {
            get
            {
                return _isNMIPopupPending;
            }

            set
            {
                _isNMIPopupPending = value;

                if (PopupPendingChanged != null)
                {
                    PopupPendingChanged(null, EventArgs.Empty);
                }
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether to show Key in code only
        /// </summary>
        private bool KeyInCodeOnly
        {
            get
            {
                return _keyInCodeOnly;
            }

            set
            {
                if (value)
                {
                    // do not use Collapsed for GenericAlphaNumericKeyboard
                    GenericAlphaNumericKeyboard.Visibility = Visibility.Hidden;
                    NumericPad.InputTextBox.TextChanged += NumericPad_TextChanged;
                }
                else
                {
                    NumericPad.Visibility = Visibility.Collapsed;
                    NumericPad.InputTextBox.TextChanged -= NumericPad_TextChanged;
                }

                GenericAlphaNumericKeyboard.InputTextBox.KeyboardFocus = !value;
                NumericPad.InputTextBox.KeyboardFocus = value;
                _keyInCodeOnly = value;
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether to navigate only without keyboard and numeric pad inputs
        /// </summary>
        private bool NavigateOnly
        {
            get
            {
                return _navigateOnly;
            }

            set
            {
                if (value)
                {
                    KeyInCodeOnly = false;
                    ShowAlphaNumericKeyboard = false;
                    NumericPad.Visibility = Visibility.Collapsed;
                    GenericAlphaNumericKeyboard.Visibility = Visibility.Hidden;
                    ReturnToLookupButton.Visibility = Visibility.Collapsed;
                }

                _navigateOnly = value;
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether to show the Search input 
        /// </summary>
        /// <param name="show">true to show, otherwise hide</param>
        private bool ShowAlphaNumericKeyboard
        {
            get
            {
                return _showAlphaNumericKeyboard;
            }

            set
            {
                if (NavigateOnly)
                {
                    return;
                }

                if (KeyInCodeOnly)
                {
                    NumericPad.Visibility = value ? Visibility.Visible : Visibility.Collapsed;
                    if (this.NumericPad.InputTextBox != null)
                    {
                        if (!value)
                        {
                            this.NumericPad.InputTextBox.Clear();
                        }
                    }
                }
                else
                {
                    if (this.GenericAlphaNumericKeyboard != null)
                    {
                        this.GenericAlphaNumericKeyboard.Visibility = value ? Visibility.Visible : Visibility.Hidden; // using Hidden instead of Collapsed so retrieveCustomKeyboardProperties() can populate it
                        if (this.GenericAlphaNumericKeyboard.InputTextBox != null)
                        {
                            if (value == false)
                            {
                                this.GenericAlphaNumericKeyboard.InputTextBox.Clear();
                            }
                        }
                    }
                }

                if (viewModel.IsUNavEnabled && ReturnToLookupButton.IsFocused && value)
                {
                    FocusHelper.SetInitialFocus(this, viewModel);
                }

                if (ReturnToLookupButton != null)
                {
                    this.ReturnToLookupButton.Visibility = value ? Visibility.Collapsed : Visibility.Visible;
                }

                _showAlphaNumericKeyboard = value;
                ResizeSearchList();
                if (CmDataCapture.IsCaptureActive(CmDataCapture.MaskExtensive))
                {
                    CmDataCapture.CaptureFormat(CmDataCapture.MaskExtensive, "Search.{0} method set ShowAlphaNumericKeyboard = {1}", new StackFrame(1).GetMethod().Name, value);
                }
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether the breadcrumb will be populated with "Search results for..." and other behaviors associated with it
        /// </summary>
        private bool SearchResultsView
        {
            get
            {
                return _searchResultsView;
            }

            set
            {
                if (value)
                {
                    LevelUpButton.Visibility = Visibility.Collapsed;
                }

                if (this.GenericAlphaNumericKeyboard.InputTextBox != null)
                {
                    if (!value)
                    {
                        this.GenericAlphaNumericKeyboard.InputTextBox.Clear();
                    }
                }

                _searchResultsView = value;
                if (CmDataCapture.IsCaptureActive(CmDataCapture.MaskExtensive))
                {
                    CmDataCapture.CaptureFormat(CmDataCapture.MaskExtensive, "Search.{0} method set SearchResultsView = {1}", new StackFrame(1).GetMethod().Name, value);
                }
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether MultiPick is active
        /// </summary>
        private bool IsMultipick
        {
            get
            {
                return _isMultipick;
            }

            set
            {
                this.ReturnScanButton.Property(ImageButton.TextProperty).SetResourceValue(value ? "FinishAndAdd" : "ReturnToScan");
                this.ReturnScanButton.Style = this.FindResource(value ? "multipickFinishButtonStyle" : "returnToScanButtonStyle") as Style;
                if (MultiPickPanel != null)
                {
                    MultiPickPanel.Visibility = value ? Visibility.Visible : Visibility.Collapsed;
                }

                if (SearchList != null)
                {
                    SearchList.Width = value ? 700 : 1007;
                    if (SearchListPageIndicator != null)
                    {
                        SearchListPageIndicator.MaxWidth = value ? 290 : 620;
                        SearchListPageIndicatorGrid.Margin = value ? new Thickness(0, 0, 10, 0) : new Thickness(0, 0, 0, 0);
                    }
                }

                _isMultipick = value;
            }
        }

        /// <summary>
        /// OnStateParamChanged that accepts parameter that is set in app.config.
        /// </summary>
        /// <param name="param">String type of parameter.</param>
        public override void OnStateParamChanged(string param)
        {
            if (viewModel.Perfcheck.PickListItemSelectStarted)
            {
                viewModel.Perfcheck.EndEventLog(PerfMeasureEvents.PickListItemSelect);
            }
            else if (viewModel.Perfcheck.KeyInCodeStarted)
            {
                viewModel.Perfcheck.EndEventLog(PerfMeasureEvents.KeyInCode);
            }

            switch (param)
            {
                case "ProduceFavorites":
                    UpdateQuickPickSelection(viewModel.GetPropertyValue("IsQuickPickSelection"));
                    IsMultipick = false;
                    ResetSearchView();
                    break;
                case "MultiSelectProduceFavorites":
                    UpdateQuickPickSelection(viewModel.GetPropertyValue("IsQuickPickSelection"));
                    IsMultipick = true;
                    UpdatePicklistReceiptCount(viewModel.GetPropertyValue("LookupItemCount") as string);
                    ResetSearchView();
                    ShowNMIPopup();
                    break;
            }

            GenericAlphaNumericKeyboard.IsEnabled = true;
        }

        /// <summary>
        /// Property change event
        /// </summary>
        /// <param name="name">String type parameter</param>
        /// <param name="value">Object type parameter</param>
        public override void OnPropertyChanged(string name, object value)
        {
            switch (name)
            {
                case "ActiveStateParam":
                    GenericAlphaNumericKeyboard.IsEnabled = true;
                    break;
                case "LookupItemCount":
                    UpdatePicklistReceiptCount(value.ToString());
                    break;
                case "IsQuickPickSelection":
                    UpdateQuickPickSelection(value);
                    break;
                case "UIPicklistDisplayLevels":
                    UpdateBreadcrumb();
                    break;
                case "NextGenUIHotKeyState":
                    this.GenericAlphaNumericKeyboard.SetHotKeysButtonState();
                        this.GenericAlphaNumericKeyboard.SetShiftButtonState();
                    break;
                case "IsTransitioning":
                    if (!viewModel.IsTransitioning)
                    {
                        switch (viewModel.StateParam)
                        {
                            case "ProduceFavorites":
                            case "MultiSelectProduceFavorites":
                                SubscribeCategoryLayoutUpdated();
                                break;
                        }
                    }

                    break;
                case "NextGenUIAllowKeyInCode":
                case "NextGenUIAllowPickListSearchButton":
                    KeyInCodeOnly = !GetStringPropertyBoolValue("NextGenUIAllowPickListSearchButton") && GetStringPropertyBoolValue("NextGenUIAllowKeyInCode");
                    NavigateOnly = !GetStringPropertyBoolValue("NextGenUIAllowPickListSearchButton") && !GetStringPropertyBoolValue("NextGenUIAllowKeyInCode");
                    UpdateReturnToLookupButtonText();
                    break;
                case "ShiftKeyShown":
                    GenericAlphaNumericKeyboard.ShowShiftKey();
                    break;
                case "SymbolsKeyShown":
                    GenericAlphaNumericKeyboard.ShowSymbolsKey();
                    break;
                case "SymbolsKeyText":
                    if (viewModel.BackgroundStateParam.Equals(viewModel.StateParam))
                    {
                        GenericAlphaNumericKeyboard.UpdateSymbolsButtonText();
                    }

                    break;
                default:
                    break;
            }
        }

        /// <summary>
        /// Override method for Show
        /// </summary>
        /// <param name="isShowing">Boolean type parameter</param>
        public override void Show(bool isShowing)
        {
            if (isShowing)
            {
                OnInitialSelection();
                ShowNMIPopup();
            }

            base.Show(isShowing);
        }

        /// <summary>
        /// Override method for OnShowBehindPopup.
        /// </summary>
        public override void OnShowBehindPopup()
        {
            GenericAlphaNumericKeyboard.IsKeyboardBehindPopup = true;
        }

        /// <summary>
        /// This is fired when the category control is finally shown.
        /// </summary>
        /// <param name="sender">object sender</param>
        /// <param name="e">event arguments</param>
        private void CategoryList_LayoutUpdated(object sender, EventArgs e)
        {
            if (viewModel.IsActiveBackground)
            {
                FocusHelper.SetInitialFocus(this, viewModel);
            }
        }

        /// <summary>
        /// Event handler for when the static popup pending property changes.
        /// </summary>
        /// <param name="sender">sender is null on a static event</param>
        /// <param name="e">event arguments</param>
        private void Search_PopupPendingChanged(object sender, EventArgs e)
        {
            this.viewModel.AlwaysFirePropertyChanged = IsNMIPopupPending;
        }

        /// <summary>
        /// Initialization method of AlphaNumericKeyboard
        /// </summary>
        private void InitAlphaNumericKeyboard()
        {
            this.GenericAlphaNumericKeyboard.SetAlphaNumLineKeys("PFKBAlphaNumKeysLine");
            this.GenericAlphaNumericKeyboard.SetHotKeysLine("PFKBHotKeys");
            this.GenericAlphaNumericKeyboard.RemoveAddEvents();
            this.GenericAlphaNumericKeyboard.InitKeyboardProperties();
            this.GenericAlphaNumericKeyboard.InputTextBoxTextChanged += InputTextBox_TextChanged;
            this.GenericAlphaNumericKeyboard.EnterButton.Click += EnterButton_Click;
            this.GenericAlphaNumericKeyboard.KeyboardButtonClick += KeyboardButtonClick;
            this.GenericAlphaNumericKeyboard.InputTextBox.ClearEvent += InputTextBox_ClearEvent;
            UpdateWatermarkText();
        }

        /// <summary>
        /// Updates the Watermark text of the InputTextBox of the GenericAlphaNumericKeyboard only
        /// </summary>
        private void UpdateWatermarkText()
        {
            if (GetStringPropertyBoolValue("NextGenUIAllowKeyInCode"))
            {
                GenericAlphaNumericKeyboard.InputTextBox.Property(WatermarkTextBox.WatermarkProperty).SetResourceValue("SearchWatermark");
            }
            else
            {
                GenericAlphaNumericKeyboard.InputTextBox.Property(WatermarkTextBox.WatermarkProperty).SetResourceValue("SearchItemNameWatermark");
            }
        }

        /// <summary>
        /// InputTextBox clear event
        /// </summary>
        /// <param name="sender">Object type parameter</param>
        /// <param name="e">Event arguments type parameter</param>
        private void InputTextBox_ClearEvent(object sender, EventArgs e)
        {
            if (SearchResultsView)
            {
                viewModel.ActionCommand.Execute("Clear");
            }
        }

        /// <summary>
        /// IsNumeric boolean type method
        /// </summary>
        /// <returns>Boolean type return method</returns>
        private bool IsNumeric()
        {
            bool isNumeric = true;
            foreach (char c in this.GenericAlphaNumericKeyboard.InputTextBox.Text)
            {
                if (char.IsNumber(c))
                {
                    continue;
                }
                else
                {
                    isNumeric = false;
                    break;
                }
            }

            return isNumeric;
        }

        /// <summary>
        /// Keyboard button click event
        /// </summary>
        /// <param name="sender">Object type parameter</param>
        /// <param name="e">Routed event argument type parameter</param>
        private void KeyboardButtonClick(object sender, RoutedEventArgs e)
        {
            const string Space = " ";

            FrameworkElement textBox = this.KeyboardFocusedElement;
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

                if (key != Key.None && key != Key.Space && key != Key.Back)
                {
                    textBox.RaiseEvent(new KeyEventArgs(Keyboard.PrimaryDevice, PresentationSource.FromVisual(textBox), 0, key) { RoutedEvent = Keyboard.KeyDownEvent });
                }
                else if (key == Key.Space)
                {
                    textBox.RaiseEvent(
                        new TextCompositionEventArgs(
                            InputManager.Current.PrimaryKeyboardDevice,
                            new TextComposition(InputManager.Current, textBox, Space))
                            {
                                RoutedEvent = TextCompositionManager.TextInputEvent
                            });

                    if (!SearchResultsView)
                    {
                        foreach (char c in this.GenericAlphaNumericKeyboard.InputTextBox.Text)
                        {
                            viewModel.ActionCommand.Execute(string.Format("EnterSearchKey({0})", c));
                        }
                    }
                    else
                    {
                        viewModel.ActionCommand.Execute(string.Format("EnterSearchKey({0})", Space));
                    }
                }
                else if (key == Key.Back)
                {
                    if (SearchResultsView)
                    {
                        viewModel.ActionCommand.Execute("Backspace");
                    }

                    textBox.RaiseEvent(new KeyEventArgs(Keyboard.PrimaryDevice, PresentationSource.FromVisual(textBox), 0, key) { RoutedEvent = Keyboard.KeyDownEvent });
                }
            }
            else
            {
                if (this.GenericAlphaNumericKeyboard.InputTextBox.Text.Length == 5 && IsNumeric())  //button.Text.All(char.IsDigit) &&  
                    return; //SSCOI-47042

                textBox.RaiseEvent(
                    new TextCompositionEventArgs(
                        InputManager.Current.PrimaryKeyboardDevice,
                        new TextComposition(InputManager.Current, textBox, button.Text))
                        {
                            RoutedEvent = TextCompositionManager.TextInputEvent
                        });

                if (!char.IsNumber(this.GenericAlphaNumericKeyboard.InputTextBox.Text[0]) || SearchResultsView)
                {
                    // if the first char is non-numeral, trigger autosearch immediately
                    viewModel.ActionCommand.Execute(string.Format("EnterSearchKey({0})", viewModel.EncodeActionParam(button.Text)));
                }
                else if (!SearchResultsView && !IsNumeric())
                {
                    // if the search text has combinations of numerals and non-numerals (e.g. "12 Oz"), trigger autosearch 
                    foreach (char c in this.GenericAlphaNumericKeyboard.InputTextBox.Text)
                    {
                        viewModel.ActionCommand.Execute(string.Format("EnterSearchKey({0})", c));
                    }
                }
                else
                {
                    viewModel.ActionCommand.Execute(ActionCommand.ActionUIActivity);
                }
            }
        }

        /// <summary>
        /// Enter button click event
        /// </summary>
        /// <param name="sender">Object type parameter</param>
        /// <param name="e">Routed event argument type parameter</param>
        private void EnterButton_Click(object sender, RoutedEventArgs e)
        {
            if (!this.GenericAlphaNumericKeyboard.InputTextBox.IsWatermarkShown
                && this.GenericAlphaNumericKeyboard.InputTextBox.Text.Length > 0)
            {
                // Disable the keyboard after the enter button is clicked
                GenericAlphaNumericKeyboard.IsEnabled = false;

                viewModel.ActionCommand.Execute(string.Format("EnterCode({0})", this.GenericAlphaNumericKeyboard.InputTextBox.Text));

                // Disable the Enter button when clicked
                this.GenericAlphaNumericKeyboard.EnterButton.IsEnabled = false;
                this.GenericAlphaNumericKeyboard.InputTextBox.Clear();

                if (CmDataCapture.IsCaptureActive(CmDataCapture.MaskExtensive))
                {
                    viewModel.Perfcheck.StartEventLog(PerfMeasureEvents.KeyInCode, string.Format("Item code: {0}", this.GenericAlphaNumericKeyboard.InputTextBox.Text));
                }
            }
        }

        /// <summary>
        /// Search log event
        /// </summary>
        /// <param name="sender">Object type parameter</param>
        /// <param name="e">Log event argument type parameter</param>
        private void SearchList_LogEvent(object sender, LogEventArgs e)
        {
            if (e.LogMessage.Equals("StartItemListRender"))
            {
                viewModel.Perfcheck.StartEventLog(PerfMeasureEvents.PickListItemsRender);
            }
            else if (e.LogMessage.Equals("EndItemListRender"))
            {
                viewModel.Perfcheck.EndEventLog(PerfMeasureEvents.PickListItemsRender, string.Format("Items rendered: {0}", e.RenderedItemCount));
            }
        }

        /// <summary>
        /// CategoryList collection change event
        /// </summary>
        /// <param name="sender">Object type parameter</param>
        /// <param name="e">NotifyCollectionChange event argument type parameter</param>
        private void CategoryList_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Reset)
            {
                RefreshCategories();
            }
        }

        /// <summary>
        /// SearchList collection change event
        /// </summary>
        /// <param name="sender">Object type parameter</param>
        /// <param name="e">NotifyCollectionChange event argument type parameter</param>
        private void SearchList_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Reset)
            {
                if (SearchResultsView)
                {
                    if ((viewModel.GetPropertyValue("SearchItems") as ObservableCollection<GridItem>).Count < 1)
                    {
                        // no search results
                        IndicatorGrid.ColumnDefinitions[0].Width = GridLength.Auto;
                    }
                    else
                    {
                        if (SearchList.PageCount > 1)
                        {
                            IndicatorGrid.ColumnDefinitions[0].Width = new GridLength(1, GridUnitType.Star);
                        }
                    }
                }

                if (viewModel.IsUNavEnabled && !CategoryList.HasFocus())
                {
                    LevelUpButton.Focus();
                }
            }

            if (viewModel.Perfcheck.KeywordSearchStarted)
            {
                viewModel.Perfcheck.EndEventLog(PerfMeasureEvents.KeywordSearch, string.Format("returned {0} items", SearchList.Items.Count));
            }

            if (viewModel.Perfcheck.CollectionUpdateStarted)
            {
                viewModel.Perfcheck.EndEventLog(PerfMeasureEvents.CollectionUpdate, string.Format("Item count: {0}", SearchList.Items.Count));
            }
        }

        /// <summary>
        /// Executes the action command for the PickList item
        /// </summary>
        /// <param name="item">PickList item</param>
        private void PickGridItem(GridItem item)
        {
            if (item != null)
            {
                viewModel.ActionCommand.Execute(string.Format("SearchItem({0})", item.Data));

                viewModel.Perfcheck.StartEventLog(PerfMeasureEvents.PickListItemSelect, string.Format("Item: {0}", item.Data));

                if (IsMultipick)
                {
                    _isPicking = true;
                }

                if (item.SubData["SubCategoryDescription.Text"].Length > 0)
                {
                    ShowAlphaNumericKeyboard = false;
                }
            }
        }

        /// <summary>
        /// CategoryList touchdown event
        /// </summary>
        /// <param name="sender">Object type parameter</param>
        /// <param name="e">TouchEvent argument type parameter</param>
        private void CategoryList_TouchDown(object sender, TouchEventArgs e)
        {
            ListBoxItem item = ItemsControl.ContainerFromElement(CategoryList, e.OriginalSource as DependencyObject) as ListBoxItem;
            if (item != null)
            {
                object picklistSoundPath = new PathConverter().Convert(null, null, TryFindResource("bttnValidClickSound"), null);
                if (picklistSoundPath != null)
                {
                    ControlsAudio.PlaySound(picklistSoundPath.ToString());
                }

                ShowAlphaNumericKeyboard = false;

                GridItem catItem = (GridItem)item.DataContext;
                if (catItem != null)
                {
                    SelectCategory(catItem);

                    if (viewModel.IsUNavEnabled)
                    {
                        (item as UIElement).Focus();
                    }

                    CmDataCapture.CaptureFormat(CmDataCapture.MaskInfo, "Clicked category \"{0}\"", catItem.Data);
                }
            }
        }

        /// <summary>
        /// Select category void method
        /// </summary>
        /// <param name="item">GridItem type parameter</param>
        private void SelectCategory(GridItem item)
        {
            IndicatorGrid.ColumnDefinitions[0].Width = new GridLength(1, GridUnitType.Star);

            if (item != null)
            {
                viewModel.ActionCommand.Execute(string.Format("SearchCategory({0})", item.Data));
                viewModel.Perfcheck.StartEventLog(PerfMeasureEvents.CollectionUpdate, string.Format("Category \"{0}\"", item.Data));
                CategoryList.SelectedItem = item;
            }
        }

        /// <summary>
        /// Gets the first active category tab which has property Visible=True.
        /// </summary>
        /// <returns>GridItem type return method</returns>
        private GridItem GetDefaultActiveTab()
        {
            GridItem catItem = null;

            if (viewModel.UIPicklistDisplayLevels.TabSelected < CategoryList.Items.Count)
            {
                int index = viewModel.UIPicklistDisplayLevels.TabSelected > 0 ? viewModel.UIPicklistDisplayLevels.TabSelected : 0;
                GridItem item = (GridItem)CategoryList.Items[index];

                if (item.IsVisible)
                {
                    catItem = item;
                }
                else
                {
                    foreach (GridItem visible in CategoryList.Items)
                    {
                        if (visible.IsVisible)
                        {
                            catItem = visible;
                            break;
                        }
                    }
                }
            }

            return catItem;
        }

        /// <summary>
        /// SearchList swipe event 
        /// </summary>
        /// <param name="sender">Object type parameter</param>
        /// <param name="e">SwipeEvent argument type parameter</param>
        private void SearchList_SwipeEvent(object sender, SwipeEventArgs e)
        {
            switch (e.Action)
            {
                case SwipeAction.Right:
                case SwipeAction.Left:
                    CmDataCapture.CaptureFormat(CmDataCapture.MaskExtensive, "SearchList_SwipeEvent() Swipe direction: {0}", e.Action.ToString());
                    break;
            }
        }

        /// <summary>
        /// PickList receipt collection change event
        /// </summary>
        /// <param name="sender">Object type parameter</param>
        /// <param name="e">NotifyCollectionChange event argument type parameter</param>
        private void PickListReceipt_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (IsMultipick)
            {
                if (e.Action == NotifyCollectionChangedAction.Add
                    && (e.NewItems.Count > 0)
                    && e.NewItems[0] is CustomerReceiptItem)
                {
                    CustomerReceiptItem item = (CustomerReceiptItem)e.NewItems[0];

                    if (item.Selectable)
                    {
                        IsNMIPopupPending = true;
                    }
                }
            }

            GenericAlphaNumericKeyboard.IsEnabled = true;
        }

        /// <summary>
        /// Refresh categories void method
        /// </summary>
        private void RefreshCategories()
        {
            ObservableCollection<GridItem> collection = new ObservableCollection<GridItem>();

            foreach (GridItem item in _categoryListSource)
            {
                // Move the Favorites tab to the first column
                if (item.Data.Equals(FAVORITES_DATA))
                {
                    collection.Insert(0, item);
                    _favoritesItem = item;
                }
                else
                {
                    collection.Add(item);
                }
            }

            CategoryList.ItemsSource = collection;
            HighlightTab(this.viewModel.UIPicklistDisplayLevels.TabSelected);

            // refreshing the categories will cause the focus to disappear so restore it
            FocusCategoryItem(CategoryList.SelectedItem as GridItem);
        }

        /// <summary>
        /// Sets the UNav focus of the category tabs
        /// </summary>
        /// <param name="item">category to focus</param>
        private void FocusCategoryItem(GridItem item)
        {
            if (viewModel.IsUNavEnabled && item != null)
            {
                ListBoxItem lbItem = CategoryList.ItemContainerGenerator.ContainerFromItem(item) as ListBoxItem;
                if (lbItem != null)
                {
                    lbItem.Focus();
                }
            }
        }

        /// <summary>
        /// Updates the navigation text 
        /// </summary>
        private void UpdateBreadcrumb()
        {
            int subCount = 0;

            string category = viewModel.UIPicklistDisplayLevels.CurrentCategory;
            SearchResultsView = !string.IsNullOrEmpty(viewModel.UIPicklistDisplayLevels.SearchKey);

            if (SearchResultsView)
            {
                string searchKey = GenericAlphaNumericKeyboard.InputTextBox.Text;
                if (string.IsNullOrEmpty(searchKey) || GenericAlphaNumericKeyboard.InputTextBox.IsWatermarkShown)
                {
                    searchKey = viewModel.UIPicklistDisplayLevels.SearchKey;
                }

                this.NavigationLabel.Property(TextBlock.TextProperty).SetResourceFormattedValue("SearchResultsFor", searchKey);
            }
            else
            {
                if (string.IsNullOrEmpty(category))
                {
                    NavigationLabel.Property(TextBlock.TextProperty).Clear();
                    NavigationLabel.Text = string.Empty;
                }
                else
                {
                    subCount = category.Split(',').Length - 1;
                    NavigationLabel.Text = category.Replace(",", "\\");
                }
            }

            LevelUpButton.Visibility = subCount > 0 ? Visibility.Visible : Visibility.Collapsed;
            CategoryList.ItemContainerStyle = subCount > 0 ? this.FindResource("categoryListBoxItemStyle") as Style : this.FindResource("categoryListBoxItemStyleWithArrow") as Style;
            HighlightTab(this.viewModel.UIPicklistDisplayLevels.TabSelected);

            if (!LevelUpButton.IsFocused && !LevelUpButton.IsVisible)
            {
                FocusCategoryItem(CategoryList.SelectedItem as GridItem);
            }
        }

        /// <summary>
        /// Translate ADK TabSelected to actual Category tab selection
        /// ADK TabSelected is 1-based, NextGen tab is 0-based
        /// Note that SelectedIndex = n will not change index even when there are collapsed items
        /// Favorites item tab is always at SelectedIndex = 0 in the main category
        /// </summary>
        /// <param name="tabSelected">Integer type parameter</param>
        private void HighlightTab(int tabSelected)
        {
            if (CategoryList.Items.Count > 0)
            {
                if (tabSelected == 0)
                {
                    string category = this.viewModel.UIPicklistDisplayLevels.CurrentCategory;
                    if (!string.IsNullOrEmpty(category))
                    {
                        //// CategoryList.Items[0] should be popular text or language equivalent
                        if (category.Equals((CategoryList.Items[0] as GridItem).Text))
                        {
                            CategoryList.SelectedIndex = tabSelected;
                        }
                    }

                    return;
                }
                else if ((CategoryList.Items[0] as GridItem).Data.Equals(FAVORITES_DATA) && tabSelected > 0)
                {
                    CategoryList.SelectedIndex = tabSelected; // Since Favorites item always occupy (visible or hidden) the SelectedIndex = 0, the TabSelected is effectively equal to the 0-based index
                }
                else if (tabSelected > 0)
                {
                    CategoryList.SelectedIndex = tabSelected - 1; // if there is no favorites item, deduct 1
                }
                else
                {
                    CategoryList.SelectedIndex = tabSelected;
                }
            }
        }

        /// <summary>
        /// Resize the SearchList after new items arrive or keyboard or numeric pad collapses
        /// </summary>
        private void ResizeSearchList()
        {
            if (NavigateOnly)
            {
                return;
            }

            if (ShowAlphaNumericKeyboard)
            {
                SearchList.Height = 226;
                PickListReceipt.Height = 208;
            }
            else
            {
                SearchList.Height = 450;
                PickListReceipt.Height = 432;
            }
        }

        /// <summary>
        /// InputTextBox text change event
        /// </summary>
        /// <param name="sender">Object type parameter</param>
        /// <param name="e">TextChange event argument type parameter</param>
        private void InputTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            // toggle the button states
            if (!GenericAlphaNumericKeyboard.InputTextBox.IsWatermarkShown)
            {
                this.GenericAlphaNumericKeyboard.EnterButton.IsEnabled = IsNumeric();
                this.GenericAlphaNumericKeyboard.AlphanumericBackspaceButton.IsEnabled = true;
            }
            else
            {
                this.GenericAlphaNumericKeyboard.AlphanumericBackspaceButton.IsEnabled = false;
                this.GenericAlphaNumericKeyboard.EnterButton.IsEnabled = false;
            }
        }

        /// <summary>
        /// Shows the search input
        /// </summary>
        /// <param name="sender">Object type parameter</param>
        /// <param name="e">Routed event argument type parameter</param>
        private void ReturnToLookupButton_Click(object sender, RoutedEventArgs e)
        {
            SearchResultsView = false;
            ShowAlphaNumericKeyboard = true;
        }

        /// <summary>
        /// Subscribes to layout updated of the category list.
        /// This is the only reliable way found to indicated the category list finished drawing and could be focused.
        /// </summary>
        private void SubscribeCategoryLayoutUpdated()
        {
            if (_isLayoutUpdatedSubscribed)
            {
                return;
            }

            CategoryList.LayoutUpdated += CategoryList_LayoutUpdated;
            _isLayoutUpdatedSubscribed = true;
        }

        /// <summary>
        /// Unsubscribes to the layout updated of the category list.
        /// This must be done to keep from 'refocusing' the category list when we needed to move the focus away from it.
        /// </summary>
        private void UnsubscribeCategoryLayoutUpdated()
        {
            if (!_isLayoutUpdatedSubscribed)
            {
                return;
            }

            CategoryList.LayoutUpdated -= CategoryList_LayoutUpdated;
            _isLayoutUpdatedSubscribed = false;
        }

        /// <summary>
        /// QuickPick selection update method
        /// </summary>
        /// <param name="value">Object type parameter</param>
        private void UpdateQuickPickSelection(object value)
        {
            int retval = 0;
            int.TryParse(value as string, out retval);
            _quickPickSelection = retval == -1;
        }

        /// <summary>
        /// PickList receipt count method
        /// </summary>
        /// <param name="value">String type parameter</param>
        private void UpdatePicklistReceiptCount(string value)
        {
            int lookupItemCount = 0;
            if (MultiPickPanel.Visibility == Visibility.Visible)
            {
                string str = value.ToString();
                if (str.Length > 0)
                {
                    int.TryParse(str, out lookupItemCount);
                    this.PicklistCountText.Property(TextBlock.TextProperty).SetResourceFormattedValue(
                        lookupItemCount > 1 ? "ItemsAddedFormat" : "SingleItemAddedFormat", lookupItemCount);
                }
            }
        }

        /// <summary>
        /// Reset search view method
        /// </summary>
        private void ResetSearchView()
        {
            if (IsMultipick && _isPicking)
            {
                // shopper is multi-picking, keep the search contents even when the state changes
                // also keep the keyboard contents
            }
            else
            {
                if (!IsMultipick)
                {
                    // not in multipick mode, screen stays in search screen after clearing popup
                    HighlightTab(this.viewModel.UIPicklistDisplayLevels.TabSelected);
                }

                if (SearchResultsView)
                {
                    this.GenericAlphaNumericKeyboard.InputTextBox.Text = viewModel.UIPicklistDisplayLevels.SearchKey;
                }
                else
                {
                    this.GenericAlphaNumericKeyboard.InputTextBox.Clear();
                }

                UpdateBreadcrumb();
            }

            IndicatorGrid.ColumnDefinitions[0].Width = new GridLength(1, GridUnitType.Star);
        }

        /// <summary>
        /// Assigns the initial values of category selection and other flags
        /// This must be called only once every transition to Search view
        /// </summary>
        private void OnInitialSelection()
        {
            this.KeyInCodeOnly = !GetStringPropertyBoolValue("NextGenUIAllowPickListSearchButton") && GetStringPropertyBoolValue("NextGenUIAllowKeyInCode");
            this.NavigateOnly = !GetStringPropertyBoolValue("NextGenUIAllowPickListSearchButton") && !GetStringPropertyBoolValue("NextGenUIAllowKeyInCode");
            _isPicking = false;
            RefreshCategories();
            ResetSearchView();
            if (CategoryList.Items.Count > 0)
            {
                if (_quickPickSelection)
                {
                    ShowAlphaNumericKeyboard = false;
                    this.GenericAlphaNumericKeyboard.InputTextBox.Clear();
                }
                else
                {
                    ShowAlphaNumericKeyboard = true;
                }
            }

            if (KeyInCodeOnly)
            {
                ReturnToLookupButton.Style = this.FindResource("picklistKeyInItem") as Style;
                NumericPad.UpdateStateElements(viewModel.StateParam);
            }
            else
            {
                ReturnToLookupButton.Style = this.FindResource("picklistSearchKeyInItem") as Style;
            }
        }

        /// <summary>
        /// NMI popup boolean method
        /// </summary>
        /// <returns>Boolean type return method</returns>
        private bool ShowNMIPopup()
        {
            if (IsNMIPopupPending)
            {
                if (IsLoaded && SSCOUIViews.Properties.Settings.Default.NMI && this.viewModel.StateParam.Equals("MultiSelectProduceFavorites"))
                {
                    IsNMIPopupPending = false;
                    viewModel.ActionCommand.Execute("ViewModelSet(Context;NMIPopup)");
                    return true;
                }
            }

            return false;
        }

        /// <summary>
        /// Up tab button will get to top category
        /// </summary>
        /// <param name="sender">Object type parameter</param>
        /// <param name="e">Routed event argument type parameter</param>
        private void LevelUpButton_Click(object sender, RoutedEventArgs e)
        {
            viewModel.ActionCommand.Execute("LevelUp");
        }

        /// <summary>
        /// Return scan button click event
        /// </summary>
        /// <param name="sender">Object type parameter</param>
        /// <param name="e">Routed event argument type parameter</param>
        private void ReturnScanButton_Click(object sender, RoutedEventArgs e)
        {
            _isPicking = false;
            SearchResultsView = false;
            CategoryList.SelectedIndex = -1;
            IsNMIPopupPending = false;
        }

        /// <summary>
        /// Keyboard load method
        /// </summary>
        /// <param name="sender">Object type parameter</param>
        /// <param name="e">Routed event argument type parameter</param>
        private void Keyboard_Loaded(object sender, RoutedEventArgs e)
        {
            this.GenericAlphaNumericKeyboard.RetrieveCustomKeyboardProperties();

            if (!string.IsNullOrEmpty(viewModel.UIPicklistDisplayLevels.SearchKey))
            {
                this.GenericAlphaNumericKeyboard.InputTextBox.Text = viewModel.UIPicklistDisplayLevels.SearchKey;
            }
        }

        /// <summary>
        /// Loaded event handler for the search view
        /// </summary>
        /// <param name="sender">Sender object</param>
        /// <param name="e">Event arguments</param>
        private void BackgroundView_Loaded(object sender, RoutedEventArgs e)
        {
            ShowNMIPopup();

            SubscribeCategoryLayoutUpdated();
            GenericAlphaNumericKeyboard.ShowNumericKeypad = GetStringPropertyBoolValue("NextGenUIAllowKeyInCode");
            UpdateWatermarkText();
            UpdateReturnToLookupButtonText();

            GenericAlphaNumericKeyboard.IsEnabled = true;
        }

        /// <summary>
        /// Item clicked event handler for SearchList control
        /// </summary>
        /// <param name="sender">Sender object</param>
        /// <param name="e">Event arguments</param>
        private void SearchList_ItemClicked(object sender, EventArgs e)
        {
            GridItem item = SearchList.SelectedItem as GridItem;

            PickGridItem(item);
        }

        /// <summary>
        /// UNav click handler for CategoryList control
        /// </summary>
        /// <param name="sender">Sender object</param>
        /// <param name="e">TouchListBoxUNavEventArgs arguments</param>
        private void CategoryList_UNavClick(object sender, TouchListBoxUNavEventArgs e)
        {
            var lbItem = e.FocusedItem;

            if (lbItem != null)
            {
                GridItem item = lbItem.DataContext as GridItem;
                if (item != null)
                {
                    SelectCategory(item);
                    ShowAlphaNumericKeyboard = false;
                    CmDataCapture.CaptureFormat(CmDataCapture.MaskInfo, "Clicked category \"{0}\"", item.Data);
                }
            }
        }

        /// <summary>
        /// Updates ReturnToLookupButton text
        /// </summary>
        private void UpdateReturnToLookupButtonText()
        {
            if (KeyInCodeOnly)
            {
                ReturnToLookupButton.Property(ImageButton.TextProperty).SetResourceValue("ScotApp_569");
            }
            else
            {
                if (GetStringPropertyBoolValue("NextGenUIAllowKeyInCode"))
                {
                    ReturnToLookupButton.Property(ImageButton.TextProperty).SetResourceValue("SearchKeyInItem");
                }
                else
                {
                    ReturnToLookupButton.Property(ImageButton.TextProperty).SetResourceValue("ScotApp_1844");
                }
            }
        }

        /// <summary>
        /// Preview key down handler
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void BackgroundView_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            // this removes the handler that was focusing the category list.
            UnsubscribeCategoryLayoutUpdated();
        }

        /// <summary>
        /// NumericPad text changed handler
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void NumericPad_TextChanged(object sender, TextChangedEventArgs e)
        {
            NumericPad.OKButton.IsEnabled = !NumericPad.InputTextBox.IsWatermarkShown;
            NumericPad.NumericBackspaceButton.IsEnabled = !NumericPad.InputTextBox.IsWatermarkShown;
        }

        /// <summary>
        /// Unloaded event handler for the search view.
        /// </summary>
        /// <param name="sender">Object type parameter which represents the sending object.</param>
        /// <param name="e">RoutedEvent argument type parameter.</param>
        private void BackgroundView_Unloaded(object sender, RoutedEventArgs e)
        {
            GenericAlphaNumericKeyboard.IsKeyboardBehindPopup = false;
        }
    }
}