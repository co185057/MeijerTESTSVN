// <copyright file="VolumePopup.xaml.cs" company="NCR">
//     Copyright 2016 NCR. All rights reserved.
// </copyright>

namespace SSCOUIViews.Views
{
    using System;
    using System.Globalization;
    using System.Timers;
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
    using PsxNet;
    using RPSWNET;
    using SSCOControls;
    using SSCOControls.Helpers.KeyboardSimulatorComponents;
    using SSCOControls.Interfaces;
	using SSCOUIModels;
    using SSCOUIModels.Commands;
    using SSCOUIModels.Controls;
    using SSCOUIModels.Models;

    /// <summary>
    /// Interaction logic for VolumePopup.XAML
    /// </summary>
    public sealed partial class VolumePopup : PopupView, IDisposable, INavigable
    {
        /// <summary>
        /// Thumb of Volume Slider
        /// </summary>
        private Thumb _btnThumb = null;

        /// <summary>
        /// Volume Increase Button(+)
        /// </summary>
        private ImageButton _btnIncrease = null;

        /// <summary>
        /// Volume Decrease Button(-)
        /// </summary>
        private ImageButton _btnDecrease = null;

        /// <summary>
        /// Left Volume Level
        /// </summary>
        private int _lVolumeLevel = -1;

        /// <summary>
        /// Right Volume Level
        /// </summary>
        private int _rVolumeLevel = -1;

        /// <summary>
        /// Increment Volume Level config value
        /// </summary>
        private double _incrementLevel = 0;

        /// <summary>
        /// Thumb starting position
        /// </summary>
        private Point _startPos;

        /// <summary>
        /// Hold Timer
        /// </summary>
        private Timer _holdTimer;

        /// <summary>
        /// Volume Increase Flag
        /// </summary>
        private bool _isVolIncrease = true;

        /// <summary>
        /// Timer Elapsed
        /// </summary>
        private bool _timerElapsed = false;

        /// <summary>
        /// VolumeControlMuteBypass config value
        /// </summary>
        private string _volumeControlMuteBypass = string.Empty;

        /// <summary>
        /// System Volume Mute Flag
        /// </summary>
        private bool _isMute = false;

        /// <summary>
        /// Initializes a new instance of the <see cref="VolumePopup"/> class
        /// </summary>
        /// <param name="viewModel">PopupView viewModel</param>
        public VolumePopup(IMainViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
            VSlider.MouseLeave += (object sender, MouseEventArgs e) => { viewModel.ActionCommand.Execute(ActionCommand.ActionUIActivity); };

            PreviewKeyDown += Control_PreviewKeyDown;
            LostFocus += Control_LostFocus;
            GotFocus += Control_GotFocus;
        }

        /// <summary>
        /// Gets or sets a value indicating whether the up arrow is converted to Shift-Tab.
        /// </summary>
        public bool ShiftTabForUp
        {
            get { return false; }
            set { }
        }

        /// <summary>
        /// Gets or sets a value indicating whether the down arrow is converted to Tab.
        /// </summary>
        public bool TabForDown
        {
            get { return false; }
            set { }
        }

        /// <summary>
        /// Gets or sets a value indicating whether the left arrow is converted to Shift-Tab.
        /// </summary>
        public bool ShiftTabForLeft
        {
            get { return true; }
            set { }
        }

        /// <summary>
        /// Gets or sets a value indicating whether the right arrow is converted to Tab.
        /// </summary>
        public bool TabForRight
        {
            get { return true; }
            set { }
        }

        /// <summary>
        /// Gets or sets a value indicating whether the control has focus.
        /// Use this instead of IsFocused, which can be false if you have sub controls.
        /// </summary>
        public bool IsControlFocused { get; set; }

        /// <summary>
        /// Lost Focus event handler. Be sure to set IsControlFocused in this method.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        public void Control_LostFocus(object sender, RoutedEventArgs e)
        {
            IsControlFocused = false;
        }

        /// <summary>
        /// Got Focus event handler. Be sure to set IsControlFocused in this method.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        public void Control_GotFocus(object sender, RoutedEventArgs e)
        {
            IsControlFocused = true;
        }

        /// <summary>
        /// PopupView Show override
        /// </summary>
        /// <param name="isShowing">Show or Hide Volume Popup</param>
        public override void Show(bool isShowing)
        {
            if (isShowing)
            {
                _holdTimer = new Timer(200);
                _holdTimer.AutoReset = true;

                if (_holdTimer != null)
                {
                    _holdTimer.Elapsed += HoldTimer_Elapsed;
                }

                VSlider.ApplyTemplate();

                _btnThumb = (VSlider.Template.FindName("SliderThumb", VSlider) as Track).Thumb;
                _btnIncrease = VSlider.Template.FindName("SliderIncreaseButton", VSlider) as ImageButton;
                _btnDecrease = VSlider.Template.FindName("SliderDecreaseButton", VSlider) as ImageButton;

                if (_btnThumb != null)
                {
                    _btnThumb.MouseEnter += Thumb_MouseEnter;
                    _btnThumb.MouseLeave += Thumb_MouseLeave;
                }

                if (_btnIncrease != null)
                {
                    _btnIncrease.TouchDown += IncreaseButton_TouchDown;
                    _btnIncrease.TouchUp += IncreaseButton_TouchUp;
                }

                if (_btnDecrease != null)
                {
                    _btnDecrease.TouchDown += DecreaseButton_TouchDown;
                    _btnDecrease.TouchUp += DecreaseButton_TouchUp;
                }

                string _volIncLevel = GetPropertyStringValue("VolumeControlIncLevel");
                _volumeControlMuteBypass = GetPropertyStringValue("NextGenUIVolumeControlMuteBypass");
                double _incLevel = 0;
                _incrementLevel = double.TryParse(_volIncLevel, NumberStyles.Integer, CultureInfo.InvariantCulture, out _incLevel) ? _incLevel / 100 : 0;

                try
                {
                    Psx.GetVolume(Psx.AudioDevice, out _lVolumeLevel, out _rVolumeLevel, out _isMute);
                }
                catch (PsxException ex)
                {
                    CmDataCapture.CaptureFormat(CmDataCapture.MaskWarning, "VolumePopup.Show() - psx exception: {0}", ex.Message);
                }

                _btnIncrease.IsEnabled = _incrementLevel == 0 ? false : true;
                if (_volumeControlMuteBypass.Equals("-1") && _lVolumeLevel <= Convert.ToInt32(_incLevel))
                {
                    _lVolumeLevel = Convert.ToInt32(_incLevel);
                    _rVolumeLevel = Convert.ToInt32(_incLevel);
                    VSlider.Value = (VSlider.Maximum * _lVolumeLevel) / 100;
                    _btnDecrease.IsEnabled = false;
                }
                else if (_lVolumeLevel == 0 || _rVolumeLevel == 0)
                {
                    VSlider.Value = (VSlider.Maximum * _lVolumeLevel) / 100;
                    _lVolumeLevel = -1;
                    _rVolumeLevel = -1;
                    ShowMuteIcon(true);
                }
                else
                {
                    ShowMuteIcon(false);
                    _btnDecrease.IsEnabled = _incrementLevel == 0 ? false : true;
                    VSlider.Value = (VSlider.Maximum * _lVolumeLevel) / 100;
                }
            }
            else
            {
                if (_btnThumb != null)
                {
                    _btnThumb.MouseLeave -= Thumb_MouseLeave;
                    _btnThumb.MouseEnter -= Thumb_MouseEnter;
                }

                if (_btnIncrease != null)
                {
                    _btnIncrease.TouchDown -= IncreaseButton_TouchDown;
                    _btnIncrease.TouchUp -= IncreaseButton_TouchUp;
                }

                if (_btnDecrease != null)
                {
                    _btnDecrease.TouchDown -= DecreaseButton_TouchDown;
                    _btnDecrease.TouchUp -= DecreaseButton_TouchUp;
                }

                if (_holdTimer != null)
                {
                    _holdTimer.Elapsed -= HoldTimer_Elapsed;
                    _holdTimer.Dispose();
                }
            }

            base.Show(isShowing);
        }

        /// <summary>
        /// Preview key down handler. Typically used for arrow and space keys.
        /// Be sure to check IsControlFocused before running code.
        /// </summary>
        /// <param name="sender">sender object</param>
        /// <param name="e">event arguments</param>
        public void Control_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            switch (e.Key)
            {
                case Key.Left:
                    if (ShiftTabForLeft)
                    {
                        PressShiftTab();
                        e.Handled = true;
                    }

                    break;
                case Key.Right:
                    if (TabForRight)
                    {
                        PressTab();
                        e.Handled = true;
                    }

                    break;
                case Key.Space:
                    if (_btnIncrease.IsKeyboardFocused)
                    {
                        IncreaseVolumeLevel();
                        StopTimer(_btnIncrease);
                        e.Handled = true;
                    }
                    else if (_btnDecrease.IsKeyboardFocused)
                    {
                        DecreaseVolumeLevel();
                        StopTimer(_btnDecrease);
                        e.Handled = true;
                    }

                    break;
                case Key.Up:
                    if (_btnIncrease.IsKeyboardFocused || _btnDecrease.IsKeyboardFocused)
                    {
                        PressShiftTab();
                        e.Handled = true;
                    }
                    else if (VSlider.IsKeyboardFocused)
                    {
                        IncreaseVolumeLevel();
                        StopTimer(_btnIncrease);
                        e.Handled = true;
                    }

                    break;
                case Key.Down:
                    if (_btnIncrease.IsKeyboardFocused || _btnDecrease.IsKeyboardFocused)
                    {
                        PressTab();
                        e.Handled = true;
                    }
                    else if (VSlider.IsKeyboardFocused)
                    {
                        DecreaseVolumeLevel();
                        StopTimer(_btnDecrease);
                        e.Handled = true;
                    }

                    break;
            }
        }

        /// <summary>
        /// Dispose Implementation of IDisposable
        /// </summary>
        #region IDisposable
        public void Dispose()
        {
            if (_holdTimer != null)
            {
                _holdTimer.Dispose();
            }
        }
        #endregion

        /// <summary>
        /// Method to press the Shift-Tab key combination.
        /// </summary>
        private void PressShiftTab()
        {
            KeyboardSimulator.Instance.SimulateModifiedKeyStroke(VirtualKeyCode.LSHIFT, VirtualKeyCode.TAB);
        }

        /// <summary>
        /// Method to press the Tab key.
        /// </summary>
        private void PressTab()
        {
            KeyboardSimulator.Instance.SimulateKeyPress(VirtualKeyCode.TAB);
        }

        /// <summary>
        /// volumeGoBack_Click Event Handler closes the Volume Popup
        /// </summary>
        /// <param name="sender">object sender</param>
        /// <param name="e">event e</param>
        private void VolumeGoBack_Click(object sender, RoutedEventArgs e)
        {
            viewModel.ActionCommand.Execute("ViewModelSet(Context;)");
        }

        /// <summary>
        /// IncreaseButton_TouchDown Event Handler invokes the IncreaseVolumeLevel method
        /// </summary>
        /// <param name="sender">object sender</param>
        /// <param name="e">event e</param>
        private void IncreaseButton_TouchDown(object sender, TouchEventArgs e)
        {
            IncreaseVolumeLevel();
        }

        /// <summary>
        /// DecreaseButton_TouchDown Event Handler invokes the DecreaseVolumeLevel method
        /// </summary>
        /// <param name="sender">object sender</param>
        /// <param name="e">event e</param>
        private void DecreaseButton_TouchDown(object sender, TouchEventArgs e)
        {
            DecreaseVolumeLevel();
        }

        /// <summary>
        /// IncreaseButton_TouchUp Event Handler invokes the StopTimer method 
        /// </summary>
        /// <param name="sender">object sender</param>
        /// <param name="e">event e</param>
        private void IncreaseButton_TouchUp(object sender, TouchEventArgs e)
        {
            StopTimer(_btnIncrease);
        }

        /// <summary>
        /// DecreaseButton_TouchUp Event Handler invokes the StopTimer method
        /// </summary>
        /// <param name="sender">object sender</param>
        /// <param name="e">event e</param>
        private void DecreaseButton_TouchUp(object sender, TouchEventArgs e)
        {
            StopTimer(_btnDecrease);
        }

        /// <summary>
        /// IncreaseVolumeLevel starts the HoldTimer and increases the Volume Level based on Increment Level
        /// </summary>
        private void IncreaseVolumeLevel()
        {
            if (_incrementLevel > 0)
            {
                _holdTimer.Start();
                _timerElapsed = false;
                _isVolIncrease = true;
                ChangeVolumeLevel();
                VSlider.Value = (VSlider.Maximum * _lVolumeLevel) / 100;
            }
        }

        /// <summary>
        /// DecreaseVolumeLevel starts the HoldTimer and decreases the Volume Level based on Increment Level
        /// </summary>
        private void DecreaseVolumeLevel()
        {
            if (this.VolumeSlideHolder() && _incrementLevel > 0)
            {
                _holdTimer.Start();
                _timerElapsed = false;
                _isVolIncrease = false;
                ChangeVolumeLevel();
                VSlider.Value = (VSlider.Maximum * _lVolumeLevel) / 100;
            }
        }

        /// <summary>
        /// StopTimer stops the HoldTimer and plays the Button ClickSound Audio based on the button pressed
        /// </summary>
        /// <param name="button">Image button</param>
        private void StopTimer(ImageButton button)
        {
            _holdTimer.Stop();
            if (_timerElapsed)
            {
                if (button.Name.Equals("_btnIncrease"))
                {
                    ControlsAudio.PlaySound(_btnIncrease.ClickSound);
                }
                else if (button.Name.Equals("_btnDecrease"))
                {
                    ControlsAudio.PlaySound(_btnDecrease.ClickSound);
                }
            }
        }

        /// <summary>
        /// ChangeVolumeLevel Increase or Decrease the Volume Level by Increment Level and show MuteIcon based on VolumeControlMuteBypass
        /// </summary>
        private void ChangeVolumeLevel()
        {
            if (_isVolIncrease)
            {
                if (_lVolumeLevel == -1 && _rVolumeLevel == -1)
                {
                    _lVolumeLevel = Convert.ToInt32(_incrementLevel * 100);
                    _rVolumeLevel = Convert.ToInt32(_incrementLevel * 100);
                    ShowMuteIcon(false);
                }
                else if (_lVolumeLevel >= 100 && _rVolumeLevel >= 100)
                {
                    if (_volumeControlMuteBypass.Equals("-1"))
                    {
                        _lVolumeLevel = Convert.ToInt32(_incrementLevel * 100);
                        _rVolumeLevel = Convert.ToInt32(_incrementLevel * 100);
                        _btnDecrease.IsEnabled = false;
                    }
                    else
                    {
                        _lVolumeLevel = -1;
                        _rVolumeLevel = -1;
                        ShowMuteIcon(true);
                    }
                }
                else
                {
                    _btnDecrease.IsEnabled = true;
                    _lVolumeLevel += Convert.ToInt32(_incrementLevel * 100);
                    _rVolumeLevel += Convert.ToInt32(_incrementLevel * 100);
                }

                if (!_isMute)
                {
                    viewModel.ActionCommand.Execute(string.Format(CultureInfo.InvariantCulture, "Volume({0})", _lVolumeLevel));
                }
            }
            else
            {
                if (_lVolumeLevel > 0 && _rVolumeLevel > 0)
                {
                    _lVolumeLevel -= Convert.ToInt32(_incrementLevel * 100);
                    _rVolumeLevel -= Convert.ToInt32(_incrementLevel * 100);
                    if (_lVolumeLevel <= 0 && _rVolumeLevel <= 0)
                    {
                        _lVolumeLevel = -1;
                        _rVolumeLevel = -1;
                        ShowMuteIcon(true);
                    }
                    else if (_volumeControlMuteBypass.Equals("-1") && _lVolumeLevel == (_incrementLevel * 100))
                    {
                        _btnDecrease.IsEnabled = false;
                    }

                    if (!_isMute)
                    {
                        viewModel.ActionCommand.Execute(string.Format(CultureInfo.InvariantCulture, "Volume({0})", _lVolumeLevel));
                    }
                }
            }
        }

        /// <summary>
        /// Thumb_MouseLeave Event Handler move the Volume Slider position and increase or decrease the Volume Level based on Increment Level
        /// </summary>
        /// <param name="sender">object sender</param>
        /// <param name="e">event e</param>
        private void Thumb_MouseLeave(object sender, MouseEventArgs e)
        {
            if (e.LeftButton.Equals(MouseButtonState.Pressed) && _incrementLevel > 0)
            {
                _isVolIncrease = e.GetPosition(_btnThumb).Y < _startPos.Y;
                if (!_isVolIncrease)
                {
                    if (this.VolumeSlideHolder())
                    {
                        ChangeVolumeLevel();
                        VSlider.Value = (VSlider.Maximum * _lVolumeLevel) / 100;
                        ControlsAudio.PlaySound(_btnIncrease.ClickSound);
                    }
                    else
                    {
                        _btnDecrease.IsEnabled = false;
                    }
                }
                else
                {
                    if (_lVolumeLevel < 100 && _rVolumeLevel < 100)
                    {
                        ChangeVolumeLevel();
                        VSlider.Value = (VSlider.Maximum * _lVolumeLevel) / 100;
                        ControlsAudio.PlaySound(_btnIncrease.ClickSound);
                    }
                }
            }
        }

        /// <summary>
        /// VolumeSlideHolder gets the VolumeControlMuteBypass
        /// </summary>
        /// <returns>
        /// False if _volumeControlMuteBypass is configured to Y.
        /// </returns>
        private bool VolumeSlideHolder()
        {
            bool retVal = false;
            string VolumeMinimumLevel = GetPropertyStringValue("VolumeControlIncLevel");
            int _minLevel = 0;
            int.TryParse(VolumeMinimumLevel, NumberStyles.Integer, CultureInfo.InvariantCulture, out _minLevel);
            if (_volumeControlMuteBypass.Equals("-1"))
            {
                if (_minLevel < _lVolumeLevel)
                {
                    retVal = true;
                }
            }
            else
            {
                retVal = true;
            }

            return retVal;
        }

        /// <summary>
        /// Thumb_MouseEnter Event Handler gets Thumb position.
        /// </summary>
        /// <param name="sender">object sender</param>
        /// <param name="e">event e</param>
        private void Thumb_MouseEnter(object sender, MouseEventArgs e)
        {
            _startPos = e.GetPosition(_btnThumb);
        }

        /// <summary>
        /// HoldTimer_Elapsed Event Handler stops the HoldTimer and adjust the VSlider Value based on volume level.
        /// </summary>
        /// <param name="sender">object sender</param>
        /// <param name="e">event e</param>
        private void HoldTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            _timerElapsed = true;
            if (_lVolumeLevel == -1 || _lVolumeLevel == 100)
            {
                _holdTimer.Stop();
            }

            VSlider.Dispatcher.BeginInvoke(new System.Windows.Forms.MethodInvoker(delegate()
            {
                VSlider.Value = (VSlider.Maximum * _lVolumeLevel) / 100;
            }));
        }

        /// <summary>
        /// ShowMuteIcon - show or hide Mute Icon and disable/enable decrease button at the same time.
        /// </summary>
        /// <param name="show">Show or Hide Mute Icon</param>
        private void ShowMuteIcon(bool show)
        {
            _btnDecrease.IsEnabled = show ? false : true;
            _btnThumb.Style = show ? FindResource("MuteThumbStyle") as Style : FindResource("SliderThumbStyle") as Style;
        }

        /// <summary>
        /// PopupView Loaded Event Handler
        /// </summary>
        /// <param name="sender">object sender</param>
        /// <param name="e">event e</param>
        private void PopupView_Loaded(object sender, RoutedEventArgs e)
        {
            if (viewModel.IsUNavEnabled)
            {
                VSlider.Focus();
            }
        }
    }
}