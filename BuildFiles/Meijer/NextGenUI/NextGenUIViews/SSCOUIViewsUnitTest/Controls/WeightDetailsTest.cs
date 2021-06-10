// <copyright file="WeightDetailsTest.cs" company="NCR">
//     Copyright (c) NCR. All rights reserved.
// </copyright>
namespace SSCOUIViewsUnitTest.Controls
{
    using System;
    using System.ComponentModel;
    using System.Globalization;
    using System.Windows;
    using Microsoft.VisualStudio.TestTools.UnitTesting;
    using Moq;
    using SSCOUIModels;
    using SSCOUIViews.Controls;

    /// <summary>
    /// Unit tests for the Weight Details Control.
    /// </summary>
    [TestClass]
    public class WeightDetailsTest
    {
        /// <summary>
        /// Weight Details Control under test.
        /// </summary>
        private WeightDetails _weightDetailsUnderTest;

        /// <summary>
        /// Weight Details private object.
        /// </summary>
        private PrivateObject _weightDetailsPrivateObject;

        /// <summary>
        /// Main view model mock
        /// </summary>
        private Mock<IMainViewModel> _mockedIMainViewModel;

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

            _weightDetailsUnderTest = new WeightDetails();
            _weightDetailsPrivateObject = new PrivateObject(_weightDetailsUnderTest);

            _mockedIMainViewModel = new Mock<IMainViewModel>();

            _mockedIMainViewModel.SetupGet(x => x.StateParam).Returns("Welcome");
            _mockedIMainViewModel.SetupGet(x => x.State).Returns("InTransaction");
            _mockedIMainViewModel.SetupGet(x => x.Language).Returns(0x0409);
            _mockedIMainViewModel.Setup(x => x.GetPropertyValue("Weight")).Returns(0);
            _mockedIMainViewModel.Setup(x => x.GetPropertyValue("ScaleLogo")).Returns(true);
            _mockedIMainViewModel.Setup(x => x.GetPropertyValue("ScaleInfo")).Returns(string.Empty);

            _weightDetailsUnderTest.DataContext = (IMainViewModel)_mockedIMainViewModel.Object;
        }

        /// <summary>
        /// Refreshes the weight and checks the text.
        /// </summary>
        [TestMethod]
        public void TestWeightDetails_WeightChanged_TextMatches()
        {
            // arrange
            _mockedIMainViewModel.Setup(x => x.GetPropertyValue("Weight")).Returns(123);

            // act
            _weightDetailsPrivateObject.Invoke("OnRefreshWeight");

            // assert
            Assert.AreEqual("123", _weightDetailsUnderTest.WeightTextBlock.Text);
        }

        /// <summary>
        /// Changes the Language property and makes sure the text flow direction updates.
        /// </summary>
        [TestMethod]
        public void TestWeightDetails_LanguagePropertyChanged_FlowDirectionUpdated()
        {
            TestPropertyChanged_FlowDirectionUpdated("Language");
        }

        /// <summary>
        /// Changes the Languages property and makes sure the text flow direction updates.
        /// </summary>
        [TestMethod]
        public void TestWeightDetails_LanguagesPropertyChanged_FlowDirectionUpdated()
        {
            TestPropertyChanged_FlowDirectionUpdated("Languages");
        }

        /// <summary>
        /// Changes the CMButton5MidiListShown property and makes sure the text flow direction updates.
        /// </summary>
        [TestMethod]
        public void TestWeightDetails_CMButton5MidiListShownPropertyChanged_FlowDirectionUpdated()
        {
            TestPropertyChanged_FlowDirectionUpdated("CMButton5MidiListShown");
        }

        /// <summary>
        /// Changes the LanguageWelcomeShown property and makes sure the text flow direction updates.
        /// </summary>
        [TestMethod]
        public void TestWeightDetails_LanguageWelcomeShownPropertyChanged_FlowDirectionUpdated()
        {
            TestPropertyChanged_FlowDirectionUpdated("LanguageWelcomeShown");
        }

        /// <summary>
        /// Changes the NextGenData property and makes sure the text flow direction updates.
        /// </summary>
        [TestMethod]
        public void TestWeightDetails_NextGenDataPropertyChanged_FlowDirectionUpdated()
        {
            TestPropertyChanged_FlowDirectionUpdated("NextGenData");
        }

        /// <summary>
        /// Changes the StateParam property and makes sure the text scale information updates.
        /// </summary>
        [TestMethod]
        public void TestWeightDetails_StateParamPropertyChanged_ScaleInfoUpdated()
        {
            TestPropertyChanged_ScaleInfoUpdated("StateParam");
        }

        /// <summary>
        /// Changes the ScaleInfo property and makes sure the text scale information updates.
        /// </summary>
        [TestMethod]
        public void TestWeightDetails_ScaleInfoPropertyChanged_ScaleInfoUpdated()
        {
            TestPropertyChanged_ScaleInfoUpdated("ScaleInfo");
        }

        /// <summary>
        /// Changes the SMScaleImage property and makes sure the text scale information updates.
        /// </summary>
        [TestMethod]
        public void TestWeightDetails_SMScaleImagePropertyChanged_ScaleInfoUpdated()
        {
            TestPropertyChanged_ScaleInfoUpdated("SMScaleImage");
        }

        /// <summary>
        /// Changes the StateParam property and makes sure the weight updates.
        /// </summary>
        [TestMethod]
        public void TestWeightDetails_StateParamPropertyChanged_WeightUpdated()
        {
            TestPropertyChanged_WeightUpdated("StateParam");
        }

        /// <summary>
        /// Changes the Weight property and makes sure the weight updates.
        /// </summary>
        [TestMethod]
        public void TestWeightDetails_WeightPropertyChanged_WeightUpdated()
        {
            TestPropertyChanged_WeightUpdated("Weight");
        }

        /// <summary>
        /// Changes a property and makes sure the text flow direction updates.
        /// </summary>
        /// <param name="propertyName">The property to change</param>
        private void TestPropertyChanged_FlowDirectionUpdated(string propertyName)
        {
            // arrange
            _weightDetailsUnderTest.WeightTextBlock.FlowDirection = FlowDirection.RightToLeft;

            // act
            _weightDetailsPrivateObject.Invoke("HandlePropertyChanged", new object[] { propertyName, true });

            // assert
            Assert.AreEqual(FlowDirection.LeftToRight, _weightDetailsUnderTest.WeightTextBlock.FlowDirection);
        }

        /// <summary>
        /// Changes a property and makes sure the text scale information updates.
        /// </summary>
        /// <param name="propertyName">The property to change</param>
        private void TestPropertyChanged_ScaleInfoUpdated(string propertyName)
        {
            // arrange
            _mockedIMainViewModel.Setup(x => x.GetPropertyValue("ScaleInfo")).Returns("Min=1lb Max=2lb");
            Assert.AreEqual(string.Empty, _weightDetailsUnderTest.ScaleInfo.Text);

            // act
            _weightDetailsPrivateObject.Invoke("HandlePropertyChanged", new object[] { propertyName, true });

            // assert
            Assert.AreEqual("Min=1lb Max=2lb", _weightDetailsUnderTest.ScaleInfo.Text);
        }

        /// <summary>
        /// Changes a Weight property and makes sure the weight updates.
        /// </summary>
        /// <param name="propertyName">The property to change</param>
        private void TestPropertyChanged_WeightUpdated(string propertyName)
        {
            // arrange
            _mockedIMainViewModel.Setup(x => x.GetPropertyValue("Weight")).Returns(1.5);
            Assert.AreEqual("0", _weightDetailsUnderTest.WeightTextBlock.Text);

            // act
            _weightDetailsPrivateObject.Invoke("HandlePropertyChanged", new object[] { propertyName, true });

            // assert
            Assert.AreEqual("1.5", _weightDetailsUnderTest.WeightTextBlock.Text);
        }
    }
}
