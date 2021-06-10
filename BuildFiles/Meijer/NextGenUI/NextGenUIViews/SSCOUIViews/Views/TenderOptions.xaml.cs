using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
using FPsxWPF.Controls;
using SSCOControls;
using SSCOUIModels.Controls;
using SSCOUIModels;
using System.Collections.Specialized;
using System.ComponentModel;
using SSCOUIModels.Helpers;

namespace SSCOUIViews.Views
{
    /// <summary>
    /// Interaction logic for TenderOptions.xaml
    /// </summary>
    public partial class TenderOptions : BackgroundView
    {
        private ObservableCollection<GridItem> cashback = null;
        private string cashbackstring = null;
        private ObservableCollection<GridItem> cashbackitems = null;

        public TenderOptions(IMainViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
        }

        private void TenderOptions_Loaded(object sender, RoutedEventArgs e)
        {
            this.cashback.CollectionChanged += this.cashback_CollectionChanged;
            GetCashBackList();

            if (viewModel.IsActiveBackground)
            {
                FocusHelper.SetInitialFocus(this, viewModel);
            }
        }

        private void TenderOptions_UnLoaded(object sender, RoutedEventArgs e)
        {
            this.cashback.CollectionChanged -= this.cashback_CollectionChanged;
        }

        private void cashback_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Reset || e.Action == NotifyCollectionChangedAction.Replace)
            {
                GetCashBackList();
            }
        }

        public override void OnPropertyChanged(string name, object value)
        {
            switch (name)
            {
                case "CashBack":
                case "EBTCashBack":
                    GetCashBackList();
                    break;
                case "UNav":
                    UpdateUNavSelection();
                    break;
            }
        }

        /// <summary>
        /// Clears or selects sliding grid item if uNav is turned on or off.
        /// </summary>
        private void UpdateUNavSelection()
        {
            if (viewModel.IsUNavEnabled)
            {
                CashBackOptions.SelectFocusedItem();
            }
            else
            {
                CashBackOptions.SelectedIndex = -1;
            }
        }

        /// <summary>
        /// GetButtonList
        /// </summary>
        private void GetCashBackList()
        {
            this.cashback = viewModel.GetPropertyValue(cashbackstring) as ObservableCollection<GridItem>;
            if (null != this.cashback)
            {
                CashBackScreen();
            }
        }

        /// <summary>
        /// OnStateParamChanged that accepts param that is set in app.config.
        /// </summary>
        /// <param name="param">String type of param.</param>
        public override void OnStateParamChanged(string param)
        {
            switch (param)
            {
                case "SelectPayment":
                    cashbackstring = "CashBack";
                    GetCashBackList();
                    break;
                case "SelectPaymentEBT":
                    cashbackstring = "EBTCashBack";
                    GetCashBackList();
                    break;
            }
        }

        /// <summary>
        /// Clicks an item on the sliding grid page
        /// </summary>
        /// <param name="listbox">sliding grid page</param>
        private void ClickGridPageItem(SSCOUISlidingGridPage listbox)
        {
            if (-1 != listbox.SelectedIndex && null != listbox.SelectedItem)
            {
                GridItem item = listbox.SelectedItem as GridItem;
                if (item.IsEnabled && item.IsVisible)
                {
                    int index = listbox.SelectedIndex + ((listbox.PageInfo.CurrentPageIndex) * 6) + 1;
                    viewModel.ActionCommand.Execute(String.Format(CultureInfo.InvariantCulture, cashbackstring + "({0})", index));
                }
            }
        }

        /// <summary>
        /// CashBackScreen() sets the buttons to be displayed in the TenderOptions.xaml
        /// </summary>
        private void CashBackScreen()
        {
            try
            {
                int pos = -1;
                cashbackitems = new ObservableCollection<GridItem>();
                String Logo = String.Empty;

                if (cashbackstring.Equals("CashBack"))
                {
                    Logo = "debitCashBackTenderLogoStyle";
                }
                else
                {
                    Logo = "ebtTenderLogoStyle";
                }
                foreach (GridItem item in cashback)
                {
                    item.Text = item.Text.Replace("\n", " ");
                    pos = item.Text.LastIndexOf('(');
                    if (-1 < pos)
                        item.Text = item.Text.Insert(pos, "\n");
                    item.Logo = Logo;
                    cashbackitems.Add(item);
                }
                CashBackOptions.GridItemsSource = cashbackitems;
            }
            catch (NullReferenceException)
            {
            }
        }

        /// <summary>
        /// Event handler for when an item is clicked on the sliding grid page.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        private void CashBackOptions_ItemClicked(object sender, EventArgs e)
        {
            SSCOUISlidingGridPage gridPage = sender as SSCOUISlidingGridPage;

            if (gridPage == null)
            {
                return;
            }

            ClickGridPageItem(gridPage);
        }
    }
}
