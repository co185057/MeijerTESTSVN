// <copyright file="UNavControl.xaml.cs" company="NCR Corporation">
//     Copyright (c) NCR Corporation. All rights reserved.
// </copyright>
namespace SSCOUIViews.Controls
{
    using System;
    using System.ComponentModel;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Input;
    using SSCOControls;
    using SSCOControls.Helpers.KeyboardSimulatorComponents;
    using SSCOUIModels;

    /// <summary>
    /// Interaction logic for the <see cref="UNavControl" /> class.
    /// </summary>
    public partial class UNavControl : Grid
    {
        /// <summary>
        /// ViewModel variable.
        /// </summary>
        private IMainViewModel _viewModel;

        /// <summary>
        /// Initializes a new instance of the <see cref="UNavControl" /> class.
        /// </summary>
        public UNavControl()
        {
            InitializeComponent();
        }

        /// <summary>
        /// DataContextChanged handler for this control.
        /// </summary>
        /// <param name="sender">Object event sender</param>
        /// <param name="e">DependencyPropertyChangedEventArgs struct</param>
        private void Grid_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (_viewModel != null)
            {
                _viewModel.PropertyChanged -= new PropertyChangedEventHandler(ViewModel_PropertyChanged);
            }

            _viewModel = DataContext as IMainViewModel;
            _viewModel.PropertyChanged += new PropertyChangedEventHandler(ViewModel_PropertyChanged);
            SetZIndex(this, 1);
        }

        /// <summary>
        /// ViewModel PropertyChanged handler.
        /// </summary>
        /// <param name="sender">Object event sender</param>
        /// <param name="e">PropertyChangedEventArgs object</param>
        private void ViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch (e.PropertyName)
            {
                case "State":
                    if (_viewModel.State.Equals("OutOfTransaction") && _viewModel.PreviousState.Equals("EndOfTransaction"))
                    {
                        ResetControlState();
                    }

                    break;
                case "UNav":
                    SetControlVisibility();
                    break;
                case "StateParam":
                    if (_viewModel.StateParam.Equals("Closed"))
                    {
                        ResetControlState();
                    }

                    break;
            }
        }

        /// <summary>
        /// Handles changing control visibility by checking pertinent ViewModel properties.
        /// </summary>
        private void SetControlVisibility()
        {
            if (!_viewModel.IsUNavEnabled)
            {
                Visibility = Visibility.Collapsed;
            }
            else
            {
                Visibility = Visibility.Visible;
            }
        }

        /// <summary>
        /// Resets the control to its default state.
        /// </summary>
        private void ResetControlState()
        {
            Visibility = Visibility.Collapsed;
            MoveControlRight();
        }

        /// <summary>
        /// Moves the control to the left and shows the appropriate move buttons.
        /// </summary>
        private void MoveControlLeft()
        {
            HorizontalAlignment = HorizontalAlignment.Left;
            UNavMoveToLeftBorder.Visibility = Visibility.Hidden;
            UNavMoveToRightBorder.Visibility = Visibility.Visible;
        }

        /// <summary>
        /// Moves the control to the right and shows the appropriate move buttons.
        /// </summary>
        private void MoveControlRight()
        {
            HorizontalAlignment = HorizontalAlignment.Right;
            UNavMoveToLeftBorder.Visibility = Visibility.Visible;
            UNavMoveToRightBorder.Visibility = Visibility.Hidden;
        }

        /// <summary>
        /// Click handler for the up button.
        /// </summary>
        /// <param name="sender">Object event sender</param>
        /// <param name="e">RoutedEventArgs object</param>
        private void UNavUpButton_Click(object sender, RoutedEventArgs e)
        {
            KeyboardSimulator.Instance.SimulateKeyPress(VirtualKeyCode.UP);
        }

        /// <summary>
        /// Click handler for the right button.
        /// </summary>
        /// <param name="sender">Object event sender</param>
        /// <param name="e">RoutedEventArgs object</param>
        private void UNavRightButton_Click(object sender, RoutedEventArgs e)
        {
            KeyboardSimulator.Instance.SimulateKeyPress(VirtualKeyCode.RIGHT);
        }

        /// <summary>
        /// Click handler for the down button.
        /// </summary>
        /// <param name="sender">Object event sender</param>
        /// <param name="e">RoutedEventArgs object</param>
        private void UNavDownButton_Click(object sender, RoutedEventArgs e)
        {
            KeyboardSimulator.Instance.SimulateKeyPress(VirtualKeyCode.DOWN);
        }

        /// <summary>
        /// Click handler for the left button.
        /// </summary>
        /// <param name="sender">Object event sender</param>
        /// <param name="e">RoutedEventArgs object</param>
        private void UNavLeftButton_Click(object sender, RoutedEventArgs e)
        {
            KeyboardSimulator.Instance.SimulateKeyPress(VirtualKeyCode.LEFT);
        }

        /// <summary>
        /// Click handler for the select button.
        /// </summary>
        /// <param name="sender">Object event sender</param>
        /// <param name="e">RoutedEventArgs object</param>
        private void UNavSelectButton_Click(object sender, RoutedEventArgs e)
        {
            KeyboardSimulator.Instance.SimulateKeyPress(VirtualKeyCode.SPACE);
        }

        /// <summary>
        /// Click handler for the move left button.
        /// </summary>
        /// <param name="sender">Object event sender</param>
        /// <param name="e">RoutedEventArgs object</param>
        private void UNavMoveToLeftButton_Click(object sender, RoutedEventArgs e)
        {
            MoveControlLeft();
        }

        /// <summary>
        /// Click handler for the move right button.
        /// </summary>
        /// <param name="sender">Object event sender</param>
        /// <param name="e">RoutedEventArgs object</param>
        private void UNavMoveToRightButton_Click(object sender, RoutedEventArgs e)
        {
            MoveControlRight();
        }
    }
}