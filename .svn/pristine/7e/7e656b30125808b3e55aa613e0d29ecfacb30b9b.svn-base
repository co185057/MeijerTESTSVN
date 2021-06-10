// <copyright file="SignatureCaptureBase.cs" company="NCR">
//     Copyright 2016 NCR. All rights reserved.
// </copyright>
namespace SSCOUIViews.Controls.BaseClasses
{
    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Input;
    using System.Windows.Media;
    using System.Windows.Shapes;
    using FPsxWPF;
    using SSCOControls;
    using SSCOUIModels;
    using SSCOUIModels.Commands;

    /// <summary>
    /// This is the base class used for both the signature capture and extended signature capture user controls.
    /// </summary>
    public class SignatureCaptureBase : UserControl
    {
        /// <summary>
        /// The current point being drawn
        /// </summary>
        private Point currentPoint = new Point(-1, -1);

        /// <summary>
        /// The array of signature strokes drawn
        /// </summary>
        private ArrayList signatureStrokes;

        /// <summary>
        /// The current stroke drawn
        /// </summary>
        private ArrayList currentStroke;

        /// <summary>
        /// The current path figure being drawn
        /// </summary>
        private PathFigure currentPathFigure;

        /// <summary>
        /// The shared event for duplicating touch down events.
        /// </summary>
        protected static event EventHandler TouchDownDuplicatorEvent;

        /// <summary>
        /// Event for duplicating touch up events.
        /// </summary>
        protected static event EventHandler TouchUpDuplicatorEvent;

        /// <summary>
        /// Event for duplicating the touch leave event.
        /// </summary>
        protected static event EventHandler TouchLeaveDuplicatorEvent;

        /// <summary>
        /// Event for duplicating the touch move event.
        /// </summary>
        protected static event EventHandler TouchMoveDuplicatorEvent;

        /// <summary>
        /// Gets or sets the Canvas from the XAML in the derived user controls.
        /// </summary>
        protected Canvas BaseSignatureCanvas { get; set; }

        /// <summary>
        /// Gets or sets the Path drawn on the Canvas from the XAML in the derived user controls.
        /// </summary>
        protected Path BaseSignaturePath { get; set; }

        /// <summary>
        /// Gets or sets the OK button from the XAML in the derived user controls.
        /// </summary>
        protected ImageButton BaseOKButton { get; set; }

        /// <summary>
        /// Gets or sets the 'Start Over' button from the XAML in the derived user controls.
        /// </summary>
        protected ImageButton BaseClearButton { get; set; }

        /// <summary>
        /// Gets or sets the path geometry from the derived user controls.
        /// </summary>
        protected PathGeometry PathGeometry { get; set; }

        /// <summary>
        /// Gets or sets the view model from the derived user controls.
        /// </summary>
        protected IMainViewModel ViewModel { get; set; }

        /// <summary>
        /// Adds the geometry group to the path geometry
        /// </summary>
        public void InitializeSignaturePath()
        {
            GeometryGroup geometryGroup = new GeometryGroup();
            this.BaseSignaturePath.Data = geometryGroup;
            this.PathGeometry = new PathGeometry(new PathFigureCollection());
            geometryGroup.Children.Add(this.PathGeometry);
        }

        /// <summary>
        /// Clears the signature data.
        /// </summary>
        protected void ClearSignature()
        {
            this.signatureStrokes = null;
            this.currentStroke = null;

            this.EnableSignatureButtons();

            if (this.BaseSignaturePath.Data != null)
            {
                this.InitializeSignaturePath();
            }
        }

        /// <summary>
        /// Enables the OK Button.
        /// </summary>
        protected void EnableOKButton()
        {
            this.EnableSignatureButton(this.BaseOKButton, "CMButton1Med");
        }

        /// <summary>
        /// Enables the 'Try Again' button.
        /// </summary>
        protected void EnableClearButton()
        {
            this.EnableSignatureButton(this.BaseClearButton, "CMButton2Med");
        }

        /// <summary>
        /// Calls enabling logic for both the OK and Clear buttons.
        /// </summary>
        protected void EnableSignatureButtons()
        {
            this.EnableOKButton();
            this.EnableClearButton();
        }

        /// <summary>
        /// Event handler for processing and duplicating touch down events.
        /// </summary>
        /// <param name="sender">event sender</param>
        /// <param name="e">event arguments</param>
        protected void SignatureCanvas_TouchDown(object sender, TouchEventArgs e)
        {
            Point touchPoint = e.GetTouchPoint(this.BaseSignatureCanvas).Position;
            this.ProcessTouchDown(touchPoint);

            if (TouchDownDuplicatorEvent != null)
            {
                TouchDuplicatorEventArgs eventArgs = new TouchDuplicatorEventArgs() { OriginalCanvas = this.BaseSignatureCanvas, TouchEventArguments = e };
                TouchDownDuplicatorEvent(this, eventArgs);
            }
        }

        /// <summary>
        /// Event handler for processing and duplicating touch up events.
        /// </summary>
        /// <param name="sender">event sender</param>
        /// <param name="e">event arguments</param>
        protected void SignatureCanvas_TouchUp(object sender, TouchEventArgs e)
        {
            this.ProcessTouchUp();

            if (TouchUpDuplicatorEvent != null)
            {
                TouchDuplicatorEventArgs eventArgs = new TouchDuplicatorEventArgs() { OriginalCanvas = this.BaseSignatureCanvas, TouchEventArguments = e };
                TouchUpDuplicatorEvent(this, eventArgs);
            }
        }

        /// <summary>
        /// Event handler for processing and duplicating the touch leave event.
        /// </summary>
        /// <param name="sender">event sender</param>
        /// <param name="e">event arguments</param>
        protected void SignatureCanvas_TouchLeave(object sender, TouchEventArgs e)
        {
            this.ProcessTouchLeave();

            if (TouchLeaveDuplicatorEvent != null)
            {
                TouchDuplicatorEventArgs eventArgs = new TouchDuplicatorEventArgs() { OriginalCanvas = this.BaseSignatureCanvas, TouchEventArguments = e };
                TouchLeaveDuplicatorEvent(this, eventArgs);
            }
        }

        /// <summary>
        /// Event handler for duplicating the touch move event.
        /// </summary>
        /// <param name="sender">event sender</param>
        /// <param name="e">event arguments</param>
        protected void SignatureCanvas_TouchMove(object sender, TouchEventArgs e)
        {
            Point touchPoint = e.GetTouchPoint(this.BaseSignatureCanvas).Position;
            this.ProcessTouchMove(touchPoint);

            if (TouchMoveDuplicatorEvent != null)
            {
                TouchDuplicatorEventArgs eventArgs = new TouchDuplicatorEventArgs() { OriginalCanvas = this.BaseSignatureCanvas, TouchEventArguments = e };
                TouchMoveDuplicatorEvent(this, eventArgs);
            }
        }

        /// <summary>
        /// Click handler for the 'Try Again' button.
        /// </summary>
        /// <param name="sender">event sender</param>
        /// <param name="e">event arguments</param>
        protected void Clear_Click(object sender, RoutedEventArgs e)
        {
            this.ClearSignature();
            this.ViewModel.ActionCommand.Execute("CMButton2Med");
        }

        /// <summary>
        /// Click handler for the OK button.
        /// </summary>
        /// <param name="sender">button that fired the click</param>
        /// <param name="e">event arguments</param>
        protected void OK_Click(object sender, RoutedEventArgs e)
        {
            if (this.IsShowingSignatureCapture())
            {
                string actionString = string.Format(ActionCommand.ActionSignature + "({0})", this.CreateSignatureData());
                this.ViewModel.ActionCommand.Execute(actionString);
            }
            else
            {
                this.ViewModel.ActionCommand.Execute("CMButton1Med");
            }
        }

        /// <summary>
        /// Returns true if signature capture should be showing. Otherwise false.
        /// </summary>
        /// <returns>true or false</returns>
        protected bool IsShowingSignatureCapture()
        {
            return FPsx.ConvertFromOleBool((string)this.ViewModel.GetPropertyValue("ShowSigCapture"));
        }

        /// <summary>
        /// Event handler for duplicating the touch leave event.
        /// </summary>
        /// <param name="sender">event sender</param>
        /// <param name="e">event arguments</param>
        protected void DuplicateTouchLeave(object sender, EventArgs e)
        {
            TouchDuplicatorEventArgs touchEventArgs = e as TouchDuplicatorEventArgs;

            if (e == null)
            {
                return;
            }

            if (!this.Equals(sender))
            {
                this.ProcessTouchLeave();
            }
        }

        /// <summary>
        /// Event handler for duplicating the touch up event.
        /// </summary>
        /// <param name="sender">event sender</param>
        /// <param name="e">event arguments</param>
        protected void DuplicateTouchUp(object sender, EventArgs e)
        {
            TouchDuplicatorEventArgs touchEventArgs = e as TouchDuplicatorEventArgs;

            if (e == null)
            {
                return;
            }

            if (!this.Equals(sender))
            {
                this.ProcessTouchUp();
            }
        }

        /// <summary>
        /// Event handler for duplicating the Touch Down event
        /// </summary>
        /// <param name="sender">event sender</param>
        /// <param name="e">event arguments</param>
        protected void DuplicateTouchDown(object sender, EventArgs e)
        {
            TouchDuplicatorEventArgs touchEventArgs = e as TouchDuplicatorEventArgs;

            if (e == null)
            {
                return;
            }

            if (!this.Equals(sender))
            {
                Point duplicatorPoint = this.GetDuplicatorPoint(touchEventArgs);

                this.ProcessTouchDown(duplicatorPoint);
            }
        }

        /// <summary>
        /// Event handler for duplicating the touch move event.
        /// </summary>
        /// <param name="sender">event sender</param>
        /// <param name="e">event arguments</param>
        protected void DuplicateTouchMove(object sender, EventArgs e)
        {
            TouchDuplicatorEventArgs touchEventArgs = e as TouchDuplicatorEventArgs;

            if (e == null)
            {
                return;
            }

            if (!this.Equals(sender))
            {
                Point duplicatorPoint = this.GetDuplicatorPoint(touchEventArgs);

                this.ProcessTouchMove(duplicatorPoint);
            }
        }

        /// <summary>
        /// Converts the on screen drawing to a string of coordinates
        /// </summary>
        /// <returns>A string of XY coordinates representing the signature</returns>
        private string CreateSignatureData()
        {
            StringBuilder strSignature = new StringBuilder();

            PresentationSource source = PresentationSource.FromVisual(this);
            double dpiX = 0.0, dpiY = 0.0;
            if (source != null)
            {
                dpiX = 96.0 * source.CompositionTarget.TransformToDevice.M11;
                dpiY = 96.0 * source.CompositionTarget.TransformToDevice.M22;
            }

            strSignature.Append(dpiX);
            strSignature.Append(' ');
            strSignature.Append(dpiY);
            strSignature.Append('|');

            strSignature.Append(this.BaseSignatureCanvas.ActualWidth);
            strSignature.Append(' ');
            strSignature.Append(this.BaseSignatureCanvas.ActualHeight);

            if (this.signatureStrokes != null)
            {
                foreach (ArrayList stroke in this.signatureStrokes)
                {
                    strSignature.Append('|');
                    for (int i = 0; i < stroke.Count; i++)
                    {
                        if (i != 0)
                        {
                            strSignature.Append(' ');
                        }

                        strSignature.Append(((Point)stroke[i]).X);
                        strSignature.Append(' ');
                        strSignature.Append(((Point)stroke[i]).Y);
                    }
                }
            }

            return strSignature.ToString();
        }

        /// <summary>
        /// Adds the current point to the current stroke.
        /// </summary>
        private void AddCurrentPointToCurrentStroke()
        {
            if (this.currentStroke == null)
            {
                this.currentStroke = new ArrayList();
            }

            this.currentStroke.Add(this.currentPoint);
            if (this.currentStroke.Count == 1)
            {
                this.currentPathFigure = new PathFigure(this.currentPoint, new List<PathSegment>(), false);
                this.PathGeometry.Figures.Add(this.currentPathFigure);

                this.currentPathFigure.StartPoint = this.currentPoint;
                if (this.signatureStrokes == null || this.signatureStrokes.Count == 0)
                {
                    this.ViewModel.ActionCommand.Execute(string.Format(ActionCommand.ActionSignature + "({0})", "start"));
                }

                this.EnableSignatureButtons();
            }
            else
            {
                LineSegment line = new LineSegment(this.currentPoint, true);
                line.Freeze();
                this.currentPathFigure.Segments.Add(line);
            }
        }

        /// <summary>
        /// Adds the current stroke to the array of signature strokes.
        /// </summary>
        private void AddCurrentStrokeToSignatureStrokes()
        {
            if (this.currentStroke != null && this.currentStroke.Count > 0)
            {
                if (this.currentStroke.Count == 1)
                {
                    Point lastPoint = (Point)this.currentStroke[this.currentStroke.Count - 1];
                    EllipseGeometry ellipseGeometry = new EllipseGeometry(lastPoint, 1, 1);
                    ((GeometryGroup)this.BaseSignaturePath.Data).Children.Add(ellipseGeometry);
                }

                if (this.signatureStrokes == null)
                {
                    this.signatureStrokes = new ArrayList();
                }

                this.signatureStrokes.Add(this.currentStroke);

                this.currentStroke = null;
                this.currentPathFigure.Freeze();
            }

            this.currentPoint = new Point(-1, -1);
        }

        /// <summary>
        /// Enables a button based on state.
        /// </summary>
        /// <param name="button">The button to enable or disable</param>
        /// <param name="viewModelPropertyPrefixName">The prefix of the property name</param>
        private void EnableSignatureButton(ImageButton button, string viewModelPropertyPrefixName)
        {
            bool isShown = (bool)ViewModel.GetPropertyValue(viewModelPropertyPrefixName + "Shown");
            bool isEnabled = (bool)ViewModel.GetPropertyValue(viewModelPropertyPrefixName + "Enabled");

            button.IsEnabled = (isShown && isEnabled) || (this.IsSigned() || !this.IsShowingSignatureCapture());
        }

        /// <summary>
        /// Handles the touch down by adding the current point to the current stroke.
        /// </summary>
        /// <param name="touchPoint">The point that was touched</param>
        private void ProcessTouchDown(Point touchPoint)
        {
            if (this.IsValidPoint(touchPoint))
            {
                this.currentPoint = touchPoint;
                this.AddCurrentPointToCurrentStroke();
            }
        }

        /// <summary>
        /// Processes the touch up event
        /// </summary>
        private void ProcessTouchUp()
        {
            this.AddCurrentStrokeToSignatureStrokes();
        }

        /// <summary>
        /// Processes the touch leave event.
        /// </summary>
        private void ProcessTouchLeave()
        {
            this.AddCurrentStrokeToSignatureStrokes();
        }

        /// <summary>
        /// Processes the touch move event.
        /// </summary>
        /// <param name="touchPoint">Point that was touched</param>
        private void ProcessTouchMove(Point touchPoint)
        {
            if (this.IsValidPoint(touchPoint))
            {
                this.currentPoint = new Point(touchPoint.X, touchPoint.Y);
                this.AddCurrentPointToCurrentStroke();
            }
            else
            {
                this.AddCurrentStrokeToSignatureStrokes();
            }
        }

        /// <summary>
        /// Checks to see if there is signature data captured.
        /// </summary>
        /// <returns>true if signed, false if not</returns>
        private bool IsSigned()
        {
            return ((this.currentStroke != null) && this.currentStroke.Count > 0)
                || (this.signatureStrokes != null && this.signatureStrokes.Count > 0);
        }

        /// <summary>
        /// Returns whether or not a point is within the touch bounds.
        /// </summary>
        /// <param name="p">The point touched</param>
        /// <returns>true if in bounds, false if not</returns>
        private bool IsValidPoint(Point p)
        {
            if (p.X < 0 || p.X > this.BaseSignatureCanvas.ActualWidth ||
                p.Y < 0 || p.Y > this.BaseSignatureCanvas.ActualHeight)
            {
                return false;
            }

            return true;
        }

        /// <summary>
        /// Gets the duplicated touch point scaled to the target signature capture.
        /// </summary>
        /// <param name="touchEventArgs">event arguments from the duplicated touch event</param>
        /// <returns>The scaled duplicated point</returns>
        private Point GetDuplicatorPoint(TouchDuplicatorEventArgs touchEventArgs)
        {
            Point originalPoint = touchEventArgs.TouchEventArguments.GetTouchPoint(touchEventArgs.OriginalCanvas).Position;

            double horizontalScale = this.BaseSignatureCanvas.ActualWidth / touchEventArgs.OriginalCanvas.ActualWidth;
            double verticalScale = this.BaseSignatureCanvas.ActualHeight / touchEventArgs.OriginalCanvas.ActualHeight;

            Point duplicatorPoint = new Point(originalPoint.X * horizontalScale, originalPoint.Y * verticalScale);
            return duplicatorPoint;
        }

        /// <summary>
        /// Class for representing event arguments when duplicating touch events.
        /// </summary>
        protected class TouchDuplicatorEventArgs : EventArgs
        {
            /// <summary>
            /// Gets or sets the source canvas that was originally touched.
            /// </summary>
            public Canvas OriginalCanvas { get; set; }

            /// <summary>
            /// Gets or sets the touch event arguments from the originally touched canvas.
            /// </summary>
            public TouchEventArgs TouchEventArguments { get; set; }
        }
    }
}
