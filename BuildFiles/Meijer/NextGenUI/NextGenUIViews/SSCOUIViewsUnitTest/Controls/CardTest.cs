// <copyright file="CardTest.cs" company="NCR">
//     Copyright (c) NCR. All rights reserved.
// </copyright>
namespace SSCOUIViewsUnitTest.Controls
{
    using System;
    using System.Collections.ObjectModel;
    using System.Windows;
    using System.Windows.Media;
    using FPsxWPF.Controls;
    using Microsoft.VisualStudio.TestTools.UnitTesting;
    using SSCOUIViews.Controls;

    /// <summary>
    /// Unit tests for the Views Card control.
    /// </summary>
    [TestClass]
    public class CardTest
    {
        /// <summary>
        /// Card Control under test.
        /// </summary>
        private Card _cardUnderTest;

        /// <summary>
        /// Card Control private object.
        /// </summary>
        private PrivateObject _cardPrivateObject;

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

            _cardUnderTest = new Card();
            _cardPrivateObject = new PrivateObject(_cardUnderTest);
        }

        /// <summary>
        /// Makes sure the animation position is reset after the bound Cart List Item changes.
        /// </summary>
        [TestMethod]
        public void TestCartListItemChanged_NoQuickPick_AnimationPositionReset()
        {
            // arrange
            _cardPrivateObject.Invoke("HideCardImage");
            TranslateTransform translateTransform = (TranslateTransform)_cardUnderTest.AnimationGrid.RenderTransform;
            Assert.AreEqual(-200, translateTransform.Y);

            // act
            CartListItem cartListItem = new CartListItem();
            _cardUnderTest.DataContext = cartListItem;

            // assert
            Assert.AreEqual(null, _cardUnderTest.AnimationGrid.RenderTransform);
        }

        /// <summary>
        /// Makes sure the animation position is reset after the bound Cart List Item changes.
        /// This tests the case when there are quick pick items.
        /// </summary>
        [TestMethod]
        public void TestCartListItemChanged_QuickPick_AnimationPositionReset()
        {
            // arrange
            ObservableCollection<GridItem> quickPickItems = new ObservableCollection<GridItem>();
            quickPickItems.Add(new GridItem());
            _cardPrivateObject.SetField("_quickPickCollection", quickPickItems);
            _cardPrivateObject.Invoke("HideCardImage");
            TranslateTransform translateTransform = (TranslateTransform)_cardUnderTest.QuickPickAnimationGrid.RenderTransform;
            Assert.AreEqual(-100, translateTransform.Y);

            // act
            CartListItem cartListItem = new CartListItem();
            _cardUnderTest.DataContext = cartListItem;

            // assert
            Assert.AreEqual(null, _cardUnderTest.QuickPickAnimationGrid.RenderTransform);
        }
    }
}
