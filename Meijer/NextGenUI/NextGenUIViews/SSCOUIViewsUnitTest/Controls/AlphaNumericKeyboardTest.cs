// <copyright file="AlphaNumericKeyboardTest.cs" company="NCR">
//     Copyright (c) NCR. All rights reserved.
// </copyright>
namespace SSCOUIViewsUnitTest.Controls
{
    using System;
    using System.Windows;
    using Microsoft.VisualStudio.TestTools.UnitTesting;
    using SSCOUIViews.Controls;

    /// <summary>
    /// Unit tests for the Alpha Numeric Keyboard Control
    /// </summary>
    [TestClass]
    public class AlphaNumericKeyboardTest
    {
        /// <summary>
        /// Alpha Numeric Keyboard Control under test.
        /// </summary>
        private AlphaNumericKeyboard _alphaNumKeyboardUnderTest;

        /// <summary>
        /// Alpha Numeric Keyboard private object.
        /// </summary>
        private PrivateObject _alphaNumKeyboardPrivateObject;

        /// <summary>
        /// Gets or sets the application
        /// </summary>
        private Application App { get; set; }

        /// <summary>
        /// Initializes each test with an App, a new object under test, and a mocked view model.
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

            _alphaNumKeyboardUnderTest = new AlphaNumericKeyboard();
            _alphaNumKeyboardPrivateObject = new PrivateObject(_alphaNumKeyboardUnderTest);
        }

        /// <summary>
        /// Checks the numeric keys max length
        /// </summary>
        [TestMethod]
        public void TestAlphaNumKeyboard_GetMaxNumericKeys_IsTen()
        {
            // act
            int result = (int)_alphaNumKeyboardPrivateObject.Invoke("MaxLineKeysLimit", new object[] { "NumericLine4Keys" });

            // assert
            Assert.AreEqual(10, result);
        }
    }
}
