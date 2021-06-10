// <copyright file="CancelYesNo.xaml.cs" company="NCR Corporation">
//     Copyright (c) NCR Corporation. All rights reserved.
// </copyright>
namespace SSCOUIViews.Views
{
    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
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
    using FPsxWPF.Controls;
    using SSCOUIModels;
    using SSCOUIModels.Controls;
    using SSCOUIModels.Helpers;
    using SSCOUIModels.Models;

    /// <summary>
    /// Interaction logic for CancelYesNo.xaml
    /// </summary>
    public partial class CancelYesNo : PopupView
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="CancelYesNo" /> class.
        /// </summary>
        /// <param name="viewModel">Object parameter for IMainViewModel</param>
        public CancelYesNo(IMainViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
        }

        /// <summary>
        /// Method for handling context change with param as parameter
        /// </summary>
        /// <param name="param">This is a parameter with a type of string</param>
        public override void OnStateParamChanged(string param)
        {
            UpdateLeadthruText();
        }

        /// <summary>
        /// Method for handling property changes
        /// </summary>
        /// <param name="name">This is a parameter with a type of string</param>
        /// <param name="value">This is a parameter with a type of RoutedEventArgs</param>
        public override void OnPropertyChanged(string name, object value)
        {
            switch (name)
            {
                case "LeadthruText":
                    UpdateLeadthruText();
                    break;
                case "UNav":
                    FocusHelper.SetInitialFocus(this, viewModel);
                    break;
            }
        }

        /// <summary>
        /// Method for updating the LeadthruText
        /// </summary>
        private void UpdateLeadthruText()
        {
            string leadthruText = GetPropertyStringValue("LeadthruText");
            if (viewModel.StateParam.Equals("ConfirmAbort"))
            {
                ConfirmAbortLeadthruText.Text = leadthruText;
            }
            else if (viewModel.StateParam.Equals("ConfirmVoid"))
            {
                ConfirmVoidLeadthruText.Text = leadthruText;
            }
        }
    }
}
