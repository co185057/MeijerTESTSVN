using System;
using System.Collections.Generic;
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
using SSCOUIModels.Models;
using SSCOUIModels;
using System.ComponentModel;
using System.Globalization;
using System.Collections;
using System.Text;
using FPsxWPF;
using RPSWNET;
using SSCOControls;
using SSCOUIModels.Commands;
using SSCOUIViews.Controls.BaseClasses;

namespace SSCOUIViews.Controls
{
    /// <summary>
    /// Interaction logic for TenderStep.xaml
    /// </summary>
    public partial class SignatureCapture : SignatureCaptureBase
    {
        public SignatureCapture()
        {
            InitializeComponent();

            this.BaseClearButton = this.ClearButton;
            this.BaseOKButton = this.OKButton;
            this.BaseSignatureCanvas = this.SignatureCanvas;
            this.BaseSignaturePath = this.SignaturePath;

            InitializeSignaturePath();

            TouchMoveDuplicatorEvent += SignatureCapture_TouchMoveDuplicatorEvent;
            TouchDownDuplicatorEvent += SignatureCapture_TouchDownDuplicatorEvent;
            TouchUpDuplicatorEvent += SignatureCapture_TouchUpDuplicatorEvent;
            TouchLeaveDuplicatorEvent += SignatureCapture_TouchLeaveDuplicatorEvent;
        }

        void SignatureCapture_TouchLeaveDuplicatorEvent(object sender, EventArgs e)
        {
            DuplicateTouchLeave(sender, e);
        }

        void SignatureCapture_TouchUpDuplicatorEvent(object sender, EventArgs e)
        {
            DuplicateTouchUp(sender, e);
        }

        void SignatureCapture_TouchDownDuplicatorEvent(object sender, EventArgs e)
        {
            DuplicateTouchDown(sender, e);
        }

        void SignatureCapture_TouchMoveDuplicatorEvent(object sender, EventArgs e)
        {
            DuplicateTouchMove(sender, e);
        }

        public void RefreshUI()
        {
            SignatureButtons.Visibility = Visibility.Visible;
            if (IsShowingSignatureCapture())
            {
                this.Instruction.Property(TextBlock.TextProperty).SetResourceValue("SignBelow");
                SignatureArea.Visibility = Visibility.Visible;
                SignatureButtons.Visibility = Visibility.Visible;
                if (ViewModel.StateParam.Equals("RequestSig"))
                {
                    ClearSignature();
                }
                EnableSignatureButtons();
            }
            else
            {
                this.Instruction.Property(TextBlock.TextProperty).SetResourceValue("SignSignature");
                SignatureArea.Visibility = Visibility.Collapsed;
                if (ViewModel.StateParam.Equals("RequestSig"))
                {
                    SignatureButtons.Visibility = Visibility.Collapsed;
                }
            }
        }

        private void Grid_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            ViewModel = DataContext as IMainViewModel;
            if (this.ViewModel != null)
            {
                this.ViewModel.PropertyChanged -= new PropertyChangedEventHandler(this.ViewModel_PropertyChanged);
            }
            this.ViewModel = DataContext as IMainViewModel;
            this.ViewModel.PropertyChanged += new PropertyChangedEventHandler(this.ViewModel_PropertyChanged);
        }

        private void ViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName.Equals("CMButton1MedShown") || e.PropertyName.Equals("CMButton1MedEnabled"))
            {
                EnableOKButton();
            }
            else if (e.PropertyName.Equals("CMButton2MedShown") || e.PropertyName.Equals("CMButton2MedEnabled"))
            {
                EnableClearButton();
            }
        }
    }
}
