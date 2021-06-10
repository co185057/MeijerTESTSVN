// <copyright file="NumericKeypadTest.cs" company="NCR">
//     Copyright (c) NCR. All rights reserved.
// </copyright>
namespace SSCOUIViewsUnitTest.Controls
{
    using System;
    using System.Windows;
    using Microsoft.VisualStudio.TestTools.UnitTesting;
    using Moq;
    using SSCOControls;
    using SSCOUIModels;
    using SSCOUIViews.Controls;

    /// <summary>
    /// Unit tests for the Numeric Keypad Control.
    /// </summary>
    [TestClass]
    public class NumericKeypadTest
    {
        /// <summary>
        /// Weight Details Control under test.
        /// </summary>
        private NumericKeypad _numericKeypadUnderTest;

        /// <summary>
        /// Weight Details private object.
        /// </summary>
        private PrivateObject _numericKeypadPrivateObject;

        /// <summary>
        /// Main view model mock
        /// </summary>
        private Mock<IMainViewModel> _mockedIMainViewModel;

        /// <summary>
        /// Gets or sets the application
        /// </summary>
        private Application App { get; set; }

        /// <summary>
        /// Initializes each test with an App and a new object under test.
        /// </summary>
        [TestInitialize]
        public void TestInitialize()
        {
            if (Application.Current == null)
            {
                App = new Application();
            }
            else
            {
                App = Application.Current;
            }

            App.Resources.MergedDictionaries.Add(Application.LoadComponent(new Uri("SSCOUISkin;component/Skin.xaml", UriKind.Relative)) as ResourceDictionary);

            _numericKeypadUnderTest = new NumericKeypad();
            _numericKeypadPrivateObject = new PrivateObject(_numericKeypadUnderTest);

            _mockedIMainViewModel = new Mock<IMainViewModel>();

            _mockedIMainViewModel.SetupGet(x => x.StateParam).Returns("Welcome");
            _mockedIMainViewModel.SetupGet(x => x.State).Returns("InTransaction");

            _numericKeypadUnderTest.DataContext = (IMainViewModel)_mockedIMainViewModel.Object;
        }

        /// <summary>
        /// Tests decimal places are automatically entered.
        /// </summary>
        [TestMethod]
        public void TestNumericKeypad_AutoEnterDecimal_DecimalPlacesEntered()
        {
            // arrange
            _numericKeypadUnderTest.AutoEnterDecimalPoint = true;
            _numericKeypadUnderTest.AutoEnterDecimalPointDigits = 3;

            // act
            _numericKeypadPrivateObject.Invoke("CalculateDecimalValue", new object[] { "1000" });

            // assert
            Assert.AreEqual("1.000", _numericKeypadUnderTest.InputTextBox.Text);
        }
    }
}
