//////////////////////////////////////////////////////////////////////////////
//
// FILE:  InstoreBarcodes.H
//
// TITLE:   Class interface definition for instore barcodes
//
// 
// AUTHOR:  
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _InstoreBarcodes
#define _InstoreBarcodes
struct InstoreCodeNode
{
		CString csCustomCode;
		CString csPrefix;
		CString csZeroString;
		CString csVariablePart;	
		bool fPriceEmbedded;//RFC239992
};
struct InstoreQueueNode;

typedef InstoreQueueNode* InstoreQueueNodePtr;

struct InstoreQueueNode
{
	InstoreCodeNode Node;
	InstoreQueueNodePtr pNextNode;
};




class CInstoreCode
{
	public:
		void	AddInstoreCode(CString CSCustomCode); // add new Instore Custome Barcode
		void	AddInstoreCodeCList(CString CSCustomCode);
//		bool    FindMatchingPattern(CString ScannedUPC); 
		POSITION    FindMatchingPatternCList(CString ScannedUPC); 
		void	RemoveAllNodes();	  // remove all print jobs from the queue
		long    NumberOfInstoreCodesLoaded();	  // total number of jobs in the queue
		void	RemoveNode(InstoreQueueNode* pByeNode);
		CString ZeroedPrice(CString csUPC);
		CString ZeroedPriceCList(CString csUPC);
		void	RemoveAllCList();
        bool    IsPatternPriceEmbedded(CString csUPC);//RFC239992
		CInstoreCode();			  // constructor
		~CInstoreCode();			  // destructor

	private:
		CList <InstoreCodeNode, InstoreCodeNode&> CustomCodeList;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

struct MultiLabelBarcodeFormatRecord
{
    int     nIndex;             //the multi-label barcode that this record is a part of
    int     nPosition;          //the position of this barcode in the multi-label barcode
    int     nSymbology;         //barcode type
    int     nItemCodePosition;  //the 0-based position of the start of the item code in this barcode, -1 if no item code
    int     nItemCodeLength;    //length of the item code in this barcode, 0 if no item code 
    CString csPrefix;           //the prefix of this barcode
    CString csLabel;            //for use by application
    CString csDMData;           //for use by application
    CString csBarcode;          //for use by application
};

class CConfigMultiLabelBarcode
{
public:
    CConfigMultiLabelBarcode();
    ~CConfigMultiLabelBarcode();
    BOOL ProcessRecords(int nIndex, CString csInput);
    void RemoveAllRecords();
    int  GetNumberOfMultiLabelBarcodes(){ return m_nMLBarcodes; }
    BOOL IsPartOfMultiLabel(const CString csScannerData, MultiLabelBarcodeFormatRecord *pMLBFRecord);
    int  GetTotalNumberOfParts(int nIndex);


private:
    int     m_nMLBarcodes;      //count of Multi-Label Barcodes
    CList   <MultiLabelBarcodeFormatRecord, MultiLabelBarcodeFormatRecord&> m_MLBarcodeList;
};

#endif //_InstoreBarcodes