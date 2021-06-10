// <copyright file="CashManagementTable.xaml.cs" company="NCR">
//     Copyright 2016 NCR. All rights reserved.
// </copyright>
namespace SSCOUIViews.Controls
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.ComponentModel;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Media;
    using FPsxWPF.Controls;
    using RPSWNET;
    using SSCOControls;
    using SSCOUIModels;
    using SSCOUIModels.Models;
    using SSCOUIViews.Controls.Helpers;

    /// <summary>
    /// Interaction logic for CashManagementTable.xaml
    /// </summary>
    public partial class CashManagementTable : DataGrid
    {
        /// <summary>
        /// The denomination of the loader
        /// </summary>
        private const string ITEM_DENOMINATION = "ITEM_DESCRIPTION";

        /// <summary>
        /// The count of notes in the loader
        /// </summary>
        private const string ITEM_COUNT = "ITEM_MSG";

        /// <summary>
        /// The total amount in the loader
        /// </summary>
        private const string ITEM_AMOUNT = "ITEM_SMTEXT";

        /// <summary>
        /// The maximum numbers of rows
        /// </summary>
        private const int MAX_ROWS = 7;

        /// <summary>
        /// The number of empty rows
        /// </summary>
        private int emptyRows;

        /// <summary>
        /// Initializes a new instance of the CashManagementTable class.
        /// </summary>
        public CashManagementTable()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Populates the loader status table
        /// </summary>
        /// <param name="loaderStatusObject">Object containing the loader status</param>
        /// <returns name="loaderItems">Collection of loader status items</returns>
        public ObservableCollection<LoaderStatusItem> PopulateLoaderTable(ObservableCollection<ReceiptItem> loaderStatusObject)
        {
            ObservableCollection<LoaderStatusItem> loaderItems = new ObservableCollection<LoaderStatusItem>();
            emptyRows = 7;

            if (loaderStatusObject != null)
            {
                if (loaderStatusObject.Count > 0)
                {
                    loaderItems = ConvertReceiptToLoaderItem(loaderStatusObject);
                    emptyRows = MAX_ROWS - loaderStatusObject.Count;
                }
            }

            FillLoaderItems(emptyRows, ref loaderItems);

            return loaderItems;
        }

        /// <summary>
        /// Converts the receipt item to a loader status item
        /// </summary>
        /// <param name="receiptObject">Object containing the receipt item</param>
        /// <returns name="loaderStatusItems">Collection of loader status items</returns>
        private ObservableCollection<LoaderStatusItem> ConvertReceiptToLoaderItem(ObservableCollection<ReceiptItem> receiptObject)
        {
            ObservableCollection<LoaderStatusItem> loaderStatusItems = new ObservableCollection<LoaderStatusItem>();

            int noOfRows = receiptObject.Count;

            for (int rowIndex = 0; rowIndex < noOfRows; rowIndex++)
            {
                var keyValues = receiptObject[rowIndex].Variables;
                LoaderStatusItem loaderItem = new LoaderStatusItem();

                if (keyValues.ContainsKey(ITEM_DENOMINATION))
                {
                    loaderItem.ItemDenomination = keyValues[ITEM_DENOMINATION];
                }

                if (keyValues.ContainsKey(ITEM_COUNT))
                {
                    loaderItem.ItemCount = keyValues[ITEM_COUNT];
                }

                if (keyValues.ContainsKey(ITEM_AMOUNT))
                {
                    loaderItem.ItemAmount = keyValues[ITEM_AMOUNT];
                }

                loaderStatusItems.Add(loaderItem);
            }

            return loaderStatusItems;
        }

        /// <summary>
        /// Fills the loader status collection with the loader status objects
        /// </summary>
        /// <param name="emptyRows">Number of empty rows</param>
        /// <param name="loaderItems">Collection of loader status items</param>
        private void FillLoaderItems(int emptyRows, ref ObservableCollection<LoaderStatusItem> loaderItems)
        {
            for (int i = 0; i < emptyRows; i++)
            {
                loaderItems.Add(new LoaderStatusItem());
            }
        }
    }
}
