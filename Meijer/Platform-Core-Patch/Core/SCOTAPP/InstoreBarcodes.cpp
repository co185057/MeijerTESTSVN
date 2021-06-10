#include "stdafx.h"
#include "commonNCR.h"
#include "common.h"
#include "commonServices.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define COMP_ID ID_DM							  // used by the trace() calls to identify this component
#define T_ID    _T("DMRq")


//constructor
CInstoreCode::CInstoreCode()
{
}

//destructor
CInstoreCode::~CInstoreCode()
{
	RemoveAllCList();
}

// Add new InstoreCode
/*
void CInstoreCode::AddInstoreCode(CString CSCustomCode)
{
//	This if statement covers the case where the customer followed the recommended
//	format of: xxiiiivpppppc  
//	where "xx" represents the prefix and is of variable length
//	where "iiii" represents the item code and is variable length
//	where "v" is the price verifier 
//	where "ppppp" is the variable part of the UPC and is of variable length
//	where "c" is the checksum digit
//	The variable part of the UPC "ppppp", the price verifier "v", the checksum digit "c"
//	will be zeroed out for security agent
//	but the original scanned UPC will be sent to Transaction Broker

	int iFieldEnd, iFieldStart; 

	CSCustomCode.MakeLower();
	if ((CSCustomCode.Find("i") != -1) && (CSCustomCode.Find("v") != -1) && (CSCustomCode.Find("p") != -1) 
		&& (CSCustomCode.Find("c") != -1))
	{

//		ilength = CSCustomCode.GetLength() + 1;

		InstoreQueueNodePtr newNodePtr = new InstoreQueueNode;

		//Save the In-Store code here
		newNodePtr->Node.csCustomCode = CSCustomCode;

		//Save length of In-Store code here
//		newNodePtr->Node.iCustomCodeLength = CSCustomCode.GetLength();

		//Find the position of last digit in the Prefix
		//and save the Prefix value, and the Prefix length
		iFieldEnd = CSCustomCode.Find("i");
		newNodePtr->Node.csPrefix = CSCustomCode.Left(iFieldEnd);
//		newNodePtr->Node.iPrefixLength = iFieldEnd;

		//Find and save "Start location of price", and "Length" of price field
		iFieldStart = CSCustomCode.Find("p");
		newNodePtr->Node.csVariablePart = CSCustomCode.Right(CSCustomCode.GetLength() - (iFieldStart -1));  //-1 to also zero-out price verifier
//		iFieldEnd = CSCustomCode.Find("c");
//		newNodePtr->Node.iPriceLength = iFieldEnd - iFieldStart;

		//Construct the Zero-out string and save it, also zero-out the Checksum and Price Verifier
		CString ZeroString;	
		for (int i=0; i < (newNodePtr->Node.csVariablePart.GetLength()); i++)
			ZeroString += "0";
		newNodePtr->Node.csZeroString = ZeroString;


		pInstoreCodeTail = newNodePtr;		
	   newNodePtr->pNextNode = NULL;                     

		if (pInstoreCodeHead == NULL)
		{
			pInstoreCodeHead = newNodePtr;
			pInstoreCodeNext = newNodePtr;
			pInstoreCodeNow = newNodePtr;

		}
		else
		{
			pInstoreCodeNow->pNextNode = newNodePtr;
			pInstoreCodeNow = pInstoreCodeTail;
		}											
	}
//	the store is using a format that does not include a price verifier "v"
//	or a checksum digit "c" the format for this custom code is
//	xxiiiiipppppp
//	where xx is the prefix to llok for
//	where iiiii is the item code and is of variable length
//	where pppppp is the variable part of the UPC number, and has variable length
//	The variable part of the UPC "pppppp" will be zeroed out for security agent
//	but the original scanned UPC will be sent to Transaction Broker
	else if ((CSCustomCode.Find("i",0) != -1) && (CSCustomCode.Find("v",0) == -1) && (CSCustomCode.Find("p",0) != -1) 
		&& (CSCustomCode.Find("c",0) == -1))
	{		
//		ilength = CSCustomCode.GetLength();

		InstoreQueueNodePtr newNodePtr = new InstoreQueueNode;

		//Save the In-Store code here
		newNodePtr->Node.csCustomCode = CSCustomCode;

		//Save length of In-Store code here
//		newNodePtr->Node.iCustomCodeLength = ilength;

		//Find the position of last digit in the Prefix
		//and save the Prefix value, and the Prefix length
		iFieldEnd = CSCustomCode.Find("i");
		newNodePtr->Node.csPrefix = CSCustomCode.Left(iFieldEnd);
//		newNodePtr->Node.iPrefixLength = iFieldEnd;

		//Find and save "Start location of price", and "Length" of price field
		iFieldStart = CSCustomCode.Find("p");
		newNodePtr->Node.csVariablePart = CSCustomCode.Right(CSCustomCode.GetLength() - iFieldStart);
//		iFieldEnd = CSCustomCode.Find("c", 0);
//		newNodePtr->Node.iPriceLength = CSCustomCode.GetLength() - iFieldStart;

		//Construct the Zero-out string and save it
		CString ZeroString;
		for (int i=1; i < (newNodePtr->Node.csVariablePart.GetLength()); i++)
			ZeroString += "0";
		newNodePtr->Node.csZeroString = ZeroString;


		pInstoreCodeTail = newNodePtr;		
	   newNodePtr->pNextNode = NULL;                     

		if (pInstoreCodeHead == NULL)
		{
			pInstoreCodeHead = newNodePtr;
			pInstoreCodeNext = newNodePtr;
			pInstoreCodeNow = newNodePtr;

		}
		else
		{
			pInstoreCodeNow->pNextNode = newNodePtr;
			pInstoreCodeNow = pInstoreCodeTail;
		}											
	}

	else
		trace(L0, "Invalid Format for InStoreBarcode:%s", CSCustomCode);
}
*/

//Trying this using CList start
void CInstoreCode::AddInstoreCodeCList(CString CSCustomCode)
{
//	This if statement covers all cases where the customer followed the recommended
//	format of: xxiiiivpppppc or xxiiiiipppppc 
//	where "xx" represents the prefix and is of variable length
//	where "iiii.." represents the item code and is variable length
//	where "v" is the price verifier 
//	where "ppppp.." is the variable part of the UPC and is of variable length
//	where "c" is the checksum digit
//	The variable part of the UPC "ppppp", the price verifier "v", the checksum digit "c"
//	will be zeroed out for security agent
//	but the original scanned UPC will be sent to Transaction Broker

	int iFieldEnd, iFieldStart; 
/*
	CSCustomCode.MakeLower();
	if ((CSCustomCode.Find("i") != -1) && (CSCustomCode.Find("v") != -1) && (CSCustomCode.Find("p") != -1) 
		&& (CSCustomCode.Find("c") != -1))
	{

//		ilength = CSCustomCode.GetLength() + 1;

		InstoreCodeNode newNode; 

		//Save the In-Store code here
//		newNodePtr->Node.csCustomCode = CSCustomCode;
		newNode.csCustomCode = CSCustomCode;

		//Save length of In-Store code here
//		newNodePtr->Node.iCustomCodeLength = CSCustomCode.GetLength();

		//Find the position of last digit in the Prefix
		//and save the Prefix value, and the Prefix length
		iFieldEnd = CSCustomCode.Find("i");
//		newNodePtr->Node.csPrefix = CSCustomCode.Left(iFieldEnd);
		newNode.csPrefix = CSCustomCode.Left(iFieldEnd);
//		newNodePtr->Node.iPrefixLength = iFieldEnd;

		//Find and save "Start location of price", and "Length" of price field
		iFieldStart = CSCustomCode.Find("p");
//		newNodePtr->Node.csVariablePart = CSCustomCode.Right(CSCustomCode.GetLength() - (iFieldStart -1));  //-1 to also zero-out price verifier
		newNode.csVariablePart = CSCustomCode.Right(CSCustomCode.GetLength() - (iFieldStart -1));  //-1 to also zero-out price verifier

		//		iFieldEnd = CSCustomCode.Find("c");
//		newNodePtr->Node.iPriceLength = iFieldEnd - iFieldStart;

		//Construct the Zero-out string and save it, also zero-out the Checksum and Price Verifier
		CString ZeroString;	
		for (int i=0; i < (newNode.csVariablePart.GetLength()); i++)
			ZeroString += "0";
		newNode.csZeroString = ZeroString;
	
		CustomCodeList.AddHead(newNode); 
	}


//	the store is using a format that does not include a price verifier "v"
//	or a checksum digit "c" the format for this custom code is
//	xxiiiiipppppp
//	where xx is the prefix to llok for
//	where iiiii is the item code and is of variable length
//	where pppppp is the variable part of the UPC number, and has variable length
//	The variable part of the UPC "pppppp" will be zeroed out for security agent
//	but the original scanned UPC will be sent to Transaction Broker
	else if ((CSCustomCode.Find("i") != -1) && (CSCustomCode.Find("v") == -1) && (CSCustomCode.Find("p") != -1) 
		&& (CSCustomCode.Find("c") == -1))
	{		
//		ilength = CSCustomCode.GetLength();

//		InstoreQueueNodePtr newNodePtr = new InstoreQueueNode;
		InstoreCodeNode newNode; 


		//Save the In-Store code here
		newNode.csCustomCode = CSCustomCode;

		//Save length of In-Store code here
//		newNodePtr->Node.iCustomCodeLength = ilength;

		//Find the position of last digit in the Prefix
		//and save the Prefix value, and the Prefix length
		iFieldEnd = CSCustomCode.Find("i");
		newNode.csPrefix = CSCustomCode.Left(iFieldEnd);
//		newNodePtr->Node.iPrefixLength = iFieldEnd;

		//Find and save "Start location of price", and "Length" of price field
		iFieldStart = CSCustomCode.Find("p");
		newNode.csVariablePart = CSCustomCode.Right(CSCustomCode.GetLength() - iFieldStart);
//		iFieldEnd = CSCustomCode.Find("c", 0);
//		newNodePtr->Node.iPriceLength = CSCustomCode.GetLength() - iFieldStart;

		//Construct the Zero-out string and save it
		CString ZeroString;
		for (int i=0; i < (newNode.csVariablePart.GetLength()); i++)
			ZeroString += "0";
		newNode.csZeroString = ZeroString;

		CustomCodeList.AddHead(newNode); 

	}	*/

   //RFC239992++
    bool bPriceEmbedded = false;

    int nCharPos = CSCustomCode.Find(_T(","));
    if(-1 < nCharPos)
    {
        CString csTemp=CSCustomCode.Mid(nCharPos);
        CSCustomCode.Delete(nCharPos,csTemp.GetLength());
        csTemp.MakeUpper();
        nCharPos = csTemp.Find(_T("Y"));
        if(-1 < nCharPos)
        {
            bPriceEmbedded = true;
        }
    }
    //RFC239992--
													


	if (CSCustomCode.Find(_T("i")) != -1) 
	{
		if (CSCustomCode.Find(_T("p")) != -1)
		{
			InstoreCodeNode newNode;
			newNode.fPriceEmbedded = bPriceEmbedded;//RFC239992
			//Save the In-Store code here
			newNode.csCustomCode = CSCustomCode;
			//Find the position of last digit in the Prefix
			//and save the Prefix value, and the Prefix length
			iFieldEnd = CSCustomCode.Find(_T("i"));
			newNode.csPrefix = CSCustomCode.Left(iFieldEnd);
			//Find and save Variable part field
			iFieldStart = CSCustomCode.Find(_T("p"));
			if (CSCustomCode.Find(_T("v")) != -1)
				iFieldStart--;
			newNode.csVariablePart = CSCustomCode.Right(CSCustomCode.GetLength() - iFieldStart);
			//Construct the Zero-out string and save it
			CString ZeroString;
			for (int i=0; i < (newNode.csVariablePart.GetLength()); i++)
				ZeroString += _T("0");
			newNode.csZeroString = ZeroString;

		CustomCodeList.AddHead(newNode); 
		}
	}

	else
		trace(L0, _T("Invalid Format for InStoreBarcode:%s"), CSCustomCode);	
}

//Trying this using CList end



POSITION CInstoreCode::FindMatchingPatternCList(CString Barcode)
{
	if (CustomCodeList.IsEmpty()) return NULL;
//		return FALSE;

	POSITION pos = CustomCodeList.GetHeadPosition();
	while (pos)
	{
		if ((Barcode.Left(CustomCodeList.GetAt(pos).csPrefix.GetLength()) == CustomCodeList.GetAt(pos).csPrefix) &&
		(Barcode.GetLength() == CustomCodeList.GetAt(pos).csCustomCode.GetLength()))
			return pos; //return TRUE;
		CustomCodeList.GetNext(pos);
	}

	return pos;   //FALSE;
}

/*
CString CInstoreCode::ZeroedPrice(CString csUPC)
{
	bool bMatchFound;
	bMatchFound = FindMatchingPattern(csUPC);
	if (bMatchFound)
		csUPC = csUPC.Left((pInstoreCodeNow->Node.csCustomCode.GetLength()) - (pInstoreCodeNow->Node.csVariablePart.GetLength()));
	    csUPC += pInstoreCodeNow->Node.csZeroString;
	return (csUPC);
}
*/

CString CInstoreCode::ZeroedPriceCList(CString csUPC)
{
//	bool bMatchFound;
//	bMatchFound = FindMatchingPatternCList(csUPC);
//	if (FindMatchingPatternCList(csUPC))
//	{
		POSITION pos = FindMatchingPatternCList(csUPC);					//CustomCodeList.GetHeadPosition();
//		for (int i=0;i < CustomCodeList.GetCount();i++)
		if (pos)
		{
			if ((csUPC.Left(CustomCodeList.GetAt(pos).csPrefix.GetLength()) == CustomCodeList.GetAt(pos).csPrefix) &&
			(csUPC.GetLength() == CustomCodeList.GetAt(pos).csCustomCode.GetLength()))
			{
				csUPC = csUPC.Left((CustomCodeList.GetAt(pos).csCustomCode.GetLength()) - (CustomCodeList.GetAt(pos).csVariablePart.GetLength()));
				csUPC += CustomCodeList.GetAt(pos).csZeroString;
				return (csUPC);
			}
//			CustomCodeList.GetNext(pos);
		}
		return (csUPC);
//	}

}


//RFC239992++
bool CInstoreCode::IsPatternPriceEmbedded(CString csUPC)
{
   bool fResult=false;
   POSITION pos = FindMatchingPatternCList(csUPC);
   if(pos)
   {
       fResult = CustomCodeList.GetAt(pos).fPriceEmbedded;
   }
   return fResult;
}
//RFC239992--




long CInstoreCode::NumberOfInstoreCodesLoaded()
{
	return (CustomCodeList.GetCount());
}

/*
void CInstoreCode::RemoveNode(InstoreQueueNode* pByeNode)   //never call from outside
{
	pInstoreCodeNow = pInstoreCodeNow->pNextNode;
	delete pByeNode;

				try
				{
					delete pByeNode;
				}
				catch(...)
				{
					trace(L6, "Got an exception in deleting barcode node in InstoreBarcodes");
				}

} */

void CInstoreCode::RemoveAllCList()
{
	CustomCodeList.RemoveAll();
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

//constructor
CConfigMultiLabelBarcode::CConfigMultiLabelBarcode() : m_nMLBarcodes(0)
{

}

//destructor
CConfigMultiLabelBarcode::~CConfigMultiLabelBarcode()
{
    m_nMLBarcodes = 0;
	RemoveAllRecords();
}

void CConfigMultiLabelBarcode::RemoveAllRecords()
{
    m_nMLBarcodes = 0;
    //no pointers in saved records so just call RemoveAll()
    m_MLBarcodeList.RemoveAll();
}

BOOL CConfigMultiLabelBarcode::ProcessRecords(int nIndex, CString csInput)
{
    int nIdx,nPosition=0;

    CString csBuffer;
    CString csTemp=csInput;
    csTemp.Replace(_T("\t"),_T(" ")); //replace any tabs with spaces
    csTemp.TrimLeft();
    csTemp.TrimRight();
    while((nIdx = csTemp.Find(_T("  "))) != -1) //delete any extra spaces
        csTemp.Delete(nIdx,1);
    
    do {

        MultiLabelBarcodeFormatRecord MLBFRecord;
        MLBFRecord.csPrefix.Empty();
        MLBFRecord.csLabel.Empty();
        MLBFRecord.csDMData.Empty();
        MLBFRecord.csBarcode.Empty();
        MLBFRecord.nIndex = nIndex;
        MLBFRecord.nPosition = nPosition++;
        
        nIdx = csTemp.Find(_T(' '));
        if(nIdx == -1)
            return FALSE;
        MLBFRecord.nSymbology = _ttoi(csTemp);

        csTemp = csTemp.Right(csTemp.GetLength() - (nIdx + 1));

        nIdx = csTemp.FindOneOf(_T("xi "));
        if(nIdx == -1)
            return FALSE;
        MLBFRecord.csPrefix = csTemp.Left(nIdx);

        nIdx = csTemp.Find(_T(' '));

        if(nIdx != -1)
            csBuffer = csTemp.Left(nIdx);
        else
            csBuffer = csTemp;

        MLBFRecord.nItemCodePosition = csBuffer.Find(_T('i'));
        if(MLBFRecord.nItemCodePosition != -1)
            MLBFRecord.nItemCodeLength = csBuffer.ReverseFind(_T('i')) - MLBFRecord.nItemCodePosition + 1;
        else
            MLBFRecord.nItemCodeLength = 0;

        if(nIdx != -1)
            csTemp = csTemp.Right(csTemp.GetLength() - (nIdx + 1));


        m_MLBarcodeList.AddTail(MLBFRecord);

    } while(nIdx != -1);
   

    ++m_nMLBarcodes;
    return TRUE;
}


BOOL CConfigMultiLabelBarcode::IsPartOfMultiLabel(const CString csScannerData, MultiLabelBarcodeFormatRecord *pMLBFRecord)
{
    if(m_MLBarcodeList.IsEmpty())
        return FALSE;

    CString csData1,csData2,csData3;
    CString csFullBarCode,csLabel;
    int nBarCodeType, iFind;

    // 3 Fields: LastData|0xe1|Label|0xe1|Type
    csData1 = csScannerData;
    csData2 = "";
    csData3 = "";

        // Get First Field
    iFind=csData1.Find((_TCHAR)DM_FS);
    ASSERT (iFind != -1);
    csFullBarCode = csData1.Left(iFind);

    // Get Second Field
    csData2 = csData1.Mid(iFind+1);
    ASSERT (csData2.GetLength()>0);
    iFind=csData2.Find((_TCHAR)DM_FS);
    ASSERT (iFind != -1);
    csLabel = csData2.Left(iFind);

    // Get Third Field
    csData3 = csData2.Mid(iFind+1);
    ASSERT (csData3.GetLength()>0);
    nBarCodeType = _ttol(csData3);

	POSITION pos = m_MLBarcodeList.GetHeadPosition();
	while (pos)
	{
		if ((csLabel.Left(m_MLBarcodeList.GetAt(pos).csPrefix.GetLength()) == m_MLBarcodeList.GetAt(pos).csPrefix) && 
            (nBarCodeType == m_MLBarcodeList.GetAt(pos).nSymbology) )
        {
            pMLBFRecord->csPrefix = m_MLBarcodeList.GetAt(pos).csPrefix; 
            pMLBFRecord->nIndex = m_MLBarcodeList.GetAt(pos).nIndex;
            pMLBFRecord->nItemCodeLength = m_MLBarcodeList.GetAt(pos).nItemCodeLength;
            pMLBFRecord->nItemCodePosition = m_MLBarcodeList.GetAt(pos).nItemCodePosition;
            pMLBFRecord->nPosition = m_MLBarcodeList.GetAt(pos).nPosition;
            pMLBFRecord->nSymbology = m_MLBarcodeList.GetAt(pos).nSymbology;
            pMLBFRecord->csLabel = csLabel;
            pMLBFRecord->csDMData = csScannerData;
            pMLBFRecord->csBarcode = csFullBarCode;

            return TRUE;
        }
		m_MLBarcodeList.GetNext(pos);
	}

	return FALSE;   //FALSE;

}

int CConfigMultiLabelBarcode::GetTotalNumberOfParts(int nIndex)
{
    int iReturn = 0;

    POSITION pos = m_MLBarcodeList.GetHeadPosition();
	while (pos)
	{
        if( m_MLBarcodeList.GetAt(pos).nIndex == nIndex )
        {
            if(m_MLBarcodeList.GetAt(pos).nPosition + 1 > iReturn)
                iReturn = m_MLBarcodeList.GetAt(pos).nPosition + 1;
        }
  		m_MLBarcodeList.GetNext(pos);
    }
    return iReturn;
}
