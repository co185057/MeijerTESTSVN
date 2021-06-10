using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using SSCOUIModels;
using RPSWNET;

namespace SSCOUIViews.Controls
{
    /// <summary>
    /// Interaction logic for CurrencyGauge.xaml
    /// </summary>
    public partial class CurrencyGauge : Grid
    {
        public CurrencyGauge()
        {
            InitializeComponent();
        }

        private void Grid_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            this.viewModel = DataContext as IMainViewModel;
            this.viewModel.PropertyChanged += ViewModel_PropertyChanged;
        }

        private void ViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch (this.viewModel.ActiveStateParam)
            {
                case "CMCashAddV6HW":
                case "CMCashRemoveV6HW":
                case "XMCashStatusV6HW":
                    if (e.PropertyName.Equals("StateParam") || e.PropertyName.StartsWith(Gauge))
                    {
                        Redraw();
                    }
                    break;
            }
        }

        private void Redraw()
        {
            ParseTransactionVariableData(this.viewModel.GetPropertyValue(Gauge).ToString());
            UpdateGaugeLayout();
            UpdateGaugeVisibility();
            UpdateCounts();
            UpdateStyle();
            UpdateEnabledStatus();
            UpdateSelectedStatus();
        }

        private void UpdateGaugeLayout()
        {
            Grid grid = IsActiveCoinBox() ? GenericGauge : ActiveCoinBoxGauge;
            grid.Visibility = Visibility.Collapsed;
        }

        private void UpdateGaugeVisibility()
        {
            Grid grid = IsActiveCoinBox() ? ActiveCoinBoxGauge : GenericGauge;
            grid.Visibility = GetStringTransactionVariableData(denomKey).Equals(string.Empty) ? Visibility.Collapsed : Visibility.Visible;
        }

        private void UpdateCounts()
        {
            SSCOControls.MeasureTextBlock changeInCount = IsActiveCoinBox() ? ActiveCoinBoxGaugeChangeInCount : GenericGaugeChangeInCount;
            SSCOControls.MeasureTextBlock changeInValue = IsActiveCoinBox() ? ActiveCoinBoxGaugeChangeInValue : GenericGaugeChangeInValue;
            SSCOControls.MeasureTextBlock currentCount = IsActiveCoinBox() ? ActiveCoinBoxGaugeCurrentCount : GenericGaugeCurrentCount;
            SSCOControls.MeasureTextBlock currentValue = IsActiveCoinBox() ? ActiveCoinBoxGaugeCurrentValue : GenericGaugeCurrentValue;
            SSCOControls.MeasureTextBlock denomination = IsActiveCoinBox() ? ActiveCoinBoxGaugeDenomination : GenericGaugeDenomination;

            currentCount.Text = GetStringTransactionVariableData(countKey);
            currentValue.Text = GetStringTransactionVariableData(valueKey);
            denomination.Text = GetStringTransactionVariableData(denomKey);

            // Cash Management hides change in count/value controls if values are 0.
            int count;
            string value = GetStringTransactionVariableData(changeInCountKey);
            if (Int32.TryParse(value, out count))
            {
                if(0 == count)
                {
                    changeInCount.Text = changeInValue.Text = string.Empty;
                }
                else
                {
                    changeInCount.Text = value;
                    changeInValue.Text = GetStringTransactionVariableData(changeInValueKey);
                }
            }
        }

        private void UpdateStyle()
        {
            SSCOControls.ImageButton body = IsActiveCoinBox() ? ActiveCoinBoxGaugeBody : GenericGaugeBody;

            if (GetBooleanTransactionVariableData(fillVisibleKey))
            {
                body.Style = (IsGenericRecycler() || IsNoteLoader()) ?
                    this.FindResource("cashManagementR6GaugeRoundedBodyDisabledAlertStyle") as Style :
                    this.FindResource("cashManagementR6GaugeBodyAlertStyle") as Style;
            }
            else
            {
                body.Style = (IsGenericRecycler() || IsNoteLoader()) ?
                    this.FindResource("cashManagementR6GaugeRoundedBodyDisabledStyle") as Style :
                    this.FindResource("cashManagementR6GaugeBodyStyle") as Style;
            }
        }

        private void UpdateEnabledStatus()
        {
            if (IsGenericRecycler() || IsNoteLoader())
            {
                if (GetBooleanTransactionVariableData(enabledKey))
                {
                    GenericGaugeBody.Style = GetBooleanTransactionVariableData(fillVisibleKey) ?
                        this.FindResource("cashManagementR6GaugeRoundedBodyEnabledAlertStyle") as Style :
                        this.FindResource("cashManagementR6GaugeRoundedBodyEnabledStyle") as Style;
                }
                else
                {
                    GenericGaugeBody.Style = GetBooleanTransactionVariableData(fillVisibleKey) ?
                        this.FindResource("cashManagementR6GaugeRoundedBodyDisabledAlertStyle") as Style :
                        this.FindResource("cashManagementR6GaugeRoundedBodyDisabledStyle") as Style;
                }

                GenericGaugeBody.IsEnabled = GetBooleanTransactionVariableData(enabledKey);
                GenericGaugeDenomination.Style = GetBooleanTransactionVariableData(enabledKey) ?
                    this.FindResource("cashManagementR6DenominationEnabledStyle") as Style :
                    this.FindResource("cashManagementR6DenominationDisabledStyle") as Style;

            }
        }

        private void UpdateSelectedStatus()
        {
            GaugeHighlight.Visibility = GetBooleanTransactionVariableData(selectedKey) ? Visibility.Visible : Visibility.Collapsed;
        }

        private void GenericGaugeBody_Click(object sender, RoutedEventArgs e)
        {
            if (IsNoteLoader())
            {
                this.viewModel.ActionCommand.Execute(string.Format("XMNoteLoaderV6HW"));
            }
            else
            {
                this.viewModel.ActionCommand.Execute(string.Format("XMNoteButtonListV6HW({0})", "50" + GaugeNumber));
            }
            
        }

        private void GenericGaugeValues_TouchDown(object sender, RoutedEventArgs e)
        {
            if (GenericGaugeBody.IsEnabled)
            {
                GenericGaugeBody.RaiseEvent(e);
            }
        }

        private void ParseTransactionVariableData(string varStr)
        {
            string[] vars = varStr.Split(KeyValueDelimiter);
            string[] pair = null;

            if (null != properties)
            {
                properties.Clear();
                for (int i = 0; i < vars.Length; i++)
                {
                    pair = vars[i].Split(ValueDelimiter);
                    if (2 == pair.Length)
                    {
                        properties.Add(pair[0], pair[1]);
                    }
                }
            }
        }

        private bool GetBooleanTransactionVariableData(string key)
        {
            string value;
            bool retVal = false;
            if (null != properties && properties.TryGetValue(key, out value))
            {
                int outValue;
                if (Int32.TryParse(value, out outValue))
                {
                    retVal = 1 == outValue;
                }
            }
            return retVal;
        }

        private string GetStringTransactionVariableData(string key)
        {
            string value;
            string retVal = string.Empty;
            if (null != properties && properties.TryGetValue(key, out value))
            {
                retVal = value;
            }
            return retVal;
        }

        private bool IsActiveCoinBox()
        {
            return GaugeType.Equals("Box") && CurrencyType.Equals("Coins");
        }

        private bool IsGenericRecycler()
        {
            return GaugeType.Equals("Generic");
        }

        private bool IsNoteLoader()
        {
            return GaugeType.Equals("Loader") && CurrencyType.Equals("Notes");
        }

        public string Gauge
        {
            get
            {
                return Convert.ToString(GetValue(GaugeProperty));
            }
            set
            {
                SetValue(GaugeProperty, value);
            }
        }

        public static DependencyProperty GaugeProperty = DependencyProperty.Register("Gauge", typeof(string), typeof(CurrencyGauge));

        public string GaugeNumber
        {
            get
            {
                return Convert.ToString(GetValue(GaugeNumberProperty));
            }
            set
            {
                SetValue(GaugeNumberProperty, value);
            }
        }

        public static DependencyProperty GaugeNumberProperty = DependencyProperty.Register("GaugeNumber", typeof(string), typeof(CurrencyGauge));

        public string GaugeType
        {
            get
            {
                return Convert.ToString(GetValue(GaugeTypeProperty));
            }
            set
            {
                SetValue(GaugeTypeProperty, value);
            }
        }

        public static DependencyProperty GaugeTypeProperty = DependencyProperty.Register("GaugeType", typeof(string), typeof(CurrencyGauge));

        public string CurrencyType
        {
            get
            {
                return Convert.ToString(GetValue(CurrencyTypeProperty));
            }
            set
            {
                SetValue(CurrencyTypeProperty, value);
            }
        }

        public static DependencyProperty CurrencyTypeProperty = DependencyProperty.Register("CurrencyType", typeof(string), typeof(CurrencyGauge));

        private IMainViewModel viewModel;
        private const char KeyValueDelimiter = ';';
        private const char ValueDelimiter = '=';
        private const string changeInCountKey = "change_in_count";
        private const string changeInValueKey = "change_in_value";
        private const string countKey = "count";
        private const string denomKey = "denom";
        private const string enabledKey = "enabled";
        private const string fillVisibleKey = "fill_visible";
        private const string selectedKey = "selected";
        private const string valueKey = "value";
        
        private Dictionary<string, string> properties = new Dictionary<string, string>();
    }
}