using System;
using System.Collections.Generic;
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
using SSCOUIModels.Controls;
using SSCOUIModels;

namespace SSCOUIViews.Views
{
    /// <summary>
    /// Interaction logic for OwnBag.xaml
    /// </summary>
    public partial class OwnBag : PopupView
    {
        /// <summary>
        /// Get Variable of ShowVolumeControl
        /// </summary>
        private bool GetUIShowVolumeControl
        {
            get
            {
                if (null != viewModel && null != viewModel.GetPropertyValue("UIShowVolumeControl") && viewModel.GetPropertyValue("UIShowVolumeControl").Equals("-1"))
                {
                    return true;
                }

                return false;
            }
        }

        /// <summary>
        /// Get Variable of DualLanguage 
        /// </summary>
        private bool GetUIDualLanguage
        {
            get
            {
                if (viewModel.GetPropertyValue("UIDualLanguage").Equals("-1"))
                {
                    return true;
                }

                return false;
            }
        }

        /// <summary>
        /// Get Variable of Allow Language during transaction 
        /// </summary>
        private bool GetUIAllowLanguageSelectionDuringTransaction
        {
            get
            {
                if (!viewModel.GetPropertyValue("UIAllowLanguageSelectionDuringTransaction").Equals("0"))
                {
                    return true;
                }

                return false;
            }
        }

        public OwnBag(IMainViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
        }

        /// <summary>
        /// Call methods for Xoffset Alignment
        /// </summary>
        public void XoffsetAlignment()
        {
            switch (viewModel.BackgroundStateParam)
            {
                case "Welcome":
                case "Startup":
                case "OutOfService":
                case "AttractMultiLanguage":
                    this.UpdateXoffsetAlignmentAttract();
                    break;
                case "ProduceFavorites":
                case "Scan":
                case "QuickPick":
                case "ScanWithReward":
                case "QuickPickWithReward":
                case "Finish":
                    this.UpdateXoffsetAlignment();
                    break;
            }

            this.UpdateLayout();
        }

        /// <summary>
        /// Xoffset Alignment of OwnBag depend on Button position
        /// </summary>
        private void UpdateXoffsetAlignment()
        {
            int XoffSetValue = 0;

            if (!GetUIShowVolumeControl && !GetUIAllowLanguageSelectionDuringTransaction)
            {
                XoffSetValue = XoffsetWhenLanguageOffAndVolumeOff;
            }
            else if ((!GetUIShowVolumeControl && GetUIAllowLanguageSelectionDuringTransaction) || (GetUIShowVolumeControl && !GetUIAllowLanguageSelectionDuringTransaction))
            {
                XoffSetValue = XoffsetWhenDualLanguageOffAndVolumeOn;
            }
            else if (GetUIShowVolumeControl && GetUIAllowLanguageSelectionDuringTransaction)
            {
                XoffSetValue = XoffsetWhenLanguageOnAndVolumeOff;
            }

            this.XOffset = XoffSetValue;
        }

        /// <summary>
        /// Xoffset Alignment of OwnBag depend on Button position(Attract
        /// </summary>
        private void UpdateXoffsetAlignmentAttract()
        {
            int XoffSetValue = 0;
            if (!GetUIShowVolumeControl && !GetUIDualLanguage)
            {
                XoffSetValue = XoffsetWhenLanguageOffAndVolumeOff;
            }
            else if ((!GetUIShowVolumeControl && GetUIDualLanguage) || (GetUIShowVolumeControl && !GetUIDualLanguage))
            {
                XoffSetValue = XoffsetWhenDualLanguageOffAndVolumeOn;
            }
            else if (GetUIShowVolumeControl && GetUIDualLanguage)
            {
                XoffSetValue = XoffsetWhenLanguageOnAndVolumeOff;
            }

            this.XOffset = XoffSetValue;
        }

        private void PopupView_Loaded(object sender, RoutedEventArgs e)
        {
            this.XoffsetAlignment();
        }

        public override void OnStateParamChanged(string param)
        {
            this.XoffsetAlignment();
        }

        private const int XoffsetWhenDualLanguageOffAndVolumeOn = 120;
        private const int XoffsetWhenLanguageOnAndVolumeOff = 220;
        private const int XoffsetWhenLanguageOffAndVolumeOff = 15;
    }
}
