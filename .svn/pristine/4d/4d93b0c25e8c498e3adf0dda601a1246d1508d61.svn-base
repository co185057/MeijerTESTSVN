#include "stdafx.h"
#include "commonNCR.h"
#include "common.h"
#include "commonServices.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static _Tchar THIS_FILE[] = __FILE__;
#endif


#define COMP_ID ID_DM							  // used by the trace() calls to identify this component
#define T_ID    _T("DMRq")

static int NumberOfPrintJobsLeft=0;
//constructor
CReceiptQ::CReceiptQ()
{
	pRQueueHead = NULL;
	pRQueueNow = NULL;
	pRQueueTail = NULL;
}

//destructor
CReceiptQ::~CReceiptQ()
{
/*
	if (pRQueueHead != NULL)
	{
		pRQueueNow = pRQueueHead;
		while (pRQueueNow != pRQueueTail)
		{
			if (pRQueueNow->Node.bPrintBitmap)
			   DeleteFile(pRQueueNow->Node.csBmpFile);
			try
			{
				delete [] pRQueueNow->Node.lpszBarCode;
			}
			catch(...)
			{
				trace(L6, "Got an exception in deleting barcode buffer in print receipt queue");
			}
			pRQueueNow->Node.lpszBarCode = NULL;
			RemoveNode(pRQueueNow);                    
		}                                                 
		if (pRQueueNow->Node.bPrintBitmap)
		   DeleteFile(pRQueueNow->Node.csBmpFile);
		try
		{
			delete [] pRQueueNow->Node.lpszBarCode;
		}
		catch(...)
		{
			trace(L6, "Got an exception in deleting barcode buffer in print receipt queue");
		}                                               
		delete pRQueueNow;
	}                                      */

	pRQueueNow = pRQueueHead;
	if (pRQueueNow != NULL)
	{
		while (pRQueueNow != pRQueueTail)
			RemoveNode(pRQueueNow);
		delete pRQueueNow;
	}

}

// Add new ReceiptLine print job
void CReceiptQ::AddPrintJobReceipt(long nDeviceID, long nDeviceClass, const CString CSLineData, CString CSmsg, const long nJobId)
{
	CString CSSavemsg = CSmsg;
	LPTSTR savingdata;
	int length = CSLineData.GetLength() + 1;
	savingdata = new _Tchar[length];
	memset(savingdata,0,length);
	_tcscpy(savingdata,(LPCTSTR)CSLineData);

	RQueueNodePtr newNodePtr = new RQueueNode;
	newNodePtr->Node.nDeviceID = nDeviceID;
	newNodePtr->Node.nDeviceClass = nDeviceClass;

	newNodePtr->Node.CSLineData = savingdata;
			try
			{
				delete [] savingdata;
			}
			catch(...)
			{
				trace(L6, _T("Got an exception in deleting ReceiptLine buffer in AddPrintJobReceipt"));
			}

	newNodePtr->Node.CSmsg = CSSavemsg;
	newNodePtr->Node.bPrintJobDone = false;
	newNodePtr->Node.nPrintJobId = nJobId;

	newNodePtr->Node.bPrintReceiptLine = true;
	newNodePtr->Node.bPrintCut = false;
	newNodePtr->Node.bPrintBitmap = false;
	newNodePtr->Node.bPrintBarCode = false;
	pRQueueTail = newNodePtr;		
    newNodePtr->pNextNode = NULL;

	if (pRQueueHead == NULL)
	{
		pRQueueHead = newNodePtr;
		pRQueueNext = newNodePtr;
		pRQueueNow = newNodePtr;

	}
	else
	{
		pRQueueNow->pNextNode = newNodePtr;
		pRQueueNow = pRQueueTail;
	}
NumberOfPrintJobsLeft++;
}

//Add new Barcode Job

void CReceiptQ::AddPrintJobReceipt(long nDevClass, LPCTSTR lpszBarCode, long lBCType, long lBCHeight, long lBCAlign, bool fPrintNumbers, const long nJobId)
{
	//char* savingbarcode;
	//CString tempbar = lpszBarCode;
	//int length = strlen(lpszBarCode) + 1;
	//savingbarcode = (char *) malloc(length);
	//memset(newNodePtr->Node.lpszBarCode,0,length);
	//strcpy(savingbarcode,(LPCSTR)tempbar);
	int length = _tcsclen(lpszBarCode) + 1;

	RQueueNodePtr newNodePtr = new RQueueNode;
	newNodePtr->Node.nDeviceClass = nDevClass;
	newNodePtr->Node.lpszBarCode = new _Tchar[length];
	memset(newNodePtr->Node.lpszBarCode,0,length);
	_tcscpy(newNodePtr->Node.lpszBarCode,(LPCTSTR)lpszBarCode);
	newNodePtr->Node.lBCType = lBCType;
	newNodePtr->Node.lBCHeight = lBCHeight;
	newNodePtr->Node.lBCAlign = lBCAlign;
	newNodePtr->Node.fPrintNumbers = fPrintNumbers;
	newNodePtr->Node.bPrintJobDone = false;
	newNodePtr->Node.nPrintJobId = nJobId;

	newNodePtr->Node.bPrintBarCode = true;
	newNodePtr->Node.bPrintCut = false;
	newNodePtr->Node.bPrintBitmap = false;
	newNodePtr->Node.bPrintReceiptLine = false;
	pRQueueTail = newNodePtr;		
    newNodePtr->pNextNode = NULL;

	if (pRQueueHead == NULL)


	{


		pRQueueHead = newNodePtr;
		pRQueueNext = newNodePtr;
		pRQueueNow = newNodePtr;

	}
	else
	{
		pRQueueNow->pNextNode = newNodePtr;
		pRQueueNow = pRQueueTail;
	}
NumberOfPrintJobsLeft++;
}

// Add new BitMap print job
void CReceiptQ::AddPrintJobReceipt(long nDevClass, CString csBmpFile, long nAlign, const long nJobId)
{
	time_t systime;
	time(&systime);

	_Tchar TimeInMS[100] = {0};
	_stprintf(TimeInMS,_T(".%d"), systime);
	CString savecsBmpFile = csBmpFile;
	savecsBmpFile += TimeInMS;
	savecsBmpFile += _T("ncr.ncr");
	CopyFile(csBmpFile, savecsBmpFile,false);

	RQueueNodePtr newNodePtr = new RQueueNode;
	newNodePtr->Node.nDeviceClass = nDevClass;
	newNodePtr->Node.csBmpFile = savecsBmpFile;
	newNodePtr->Node.nAlign = nAlign;
	newNodePtr->Node.bPrintJobDone = false;
	newNodePtr->Node.nPrintJobId = nJobId;

	newNodePtr->Node.bPrintBitmap = true;
	newNodePtr->Node.bPrintCut = false;
	newNodePtr->Node.bPrintBarCode = false;
	newNodePtr->Node.bPrintReceiptLine = false;
	pRQueueTail = newNodePtr;		
    newNodePtr->pNextNode = NULL;

	if (pRQueueHead == NULL)
	{
		pRQueueHead = newNodePtr;
		pRQueueNext = newNodePtr;
		pRQueueNow = newNodePtr;

	}
	else
	{
		pRQueueNow->pNextNode = newNodePtr;
		pRQueueNow = pRQueueTail;
	}
NumberOfPrintJobsLeft++;
}


// Add new ReceiptBitMap print job
void CReceiptQ::AddPrintJobReceipt(CString csBmpFile,const long nJobId)
{
	time_t systime;
	time(&systime);

	static long adtoname = 0;

	if (adtoname == 99999)
		adtoname = 0;

	adtoname++;

	_Tchar TimeInMS[100] = {0};

	_stprintf(TimeInMS,_T(".%d%d"),systime,adtoname);

	CString savecsBmpFile = csBmpFile;
//	CString savecsBmpFile;
//	savecsBmpFile += "d:\\queuedir\\";
	savecsBmpFile += TimeInMS;
	savecsBmpFile += _T("ncr.bmp");
	CopyFile(csBmpFile, savecsBmpFile,false);

	RQueueNodePtr newNodePtr = new RQueueNode;
	newNodePtr->Node.nDeviceClass = 999;       //used to identify receiptbitmap call
	newNodePtr->Node.csBmpFile = savecsBmpFile;
	newNodePtr->Node.nAlign = 999;

	newNodePtr->Node.bPrintJobDone = false;
	newNodePtr->Node.nPrintJobId = nJobId;

	newNodePtr->Node.bPrintBitmap = true;
	newNodePtr->Node.bPrintCut = false;
	newNodePtr->Node.bPrintBarCode = false;
	newNodePtr->Node.bPrintReceiptLine = false;
	pRQueueTail = newNodePtr;		
    newNodePtr->pNextNode = NULL;

	if (pRQueueHead == NULL)
	{
		pRQueueHead = newNodePtr;
		pRQueueNext = newNodePtr;
		pRQueueNow = newNodePtr;

	}
	else
	{
		pRQueueNow->pNextNode = newNodePtr;
		pRQueueNow = pRQueueTail;
	}
	NumberOfPrintJobsLeft++;
}


// Add new Cut job
void CReceiptQ::AddPrintJobCut(const long nJobId)
{
		RQueueNodePtr newNodePtr = new RQueueNode;
		newNodePtr->Node.nDeviceID = 0;
		newNodePtr->Node.nDeviceClass = 0;
		newNodePtr->Node.CSLineData = _T(" ");
		newNodePtr->Node.CSmsg = _T(" ");
		newNodePtr->Node.bPrintJobDone = false;
		newNodePtr->Node.nPrintJobId = nJobId;

		newNodePtr->Node.bPrintReceiptLine = false;
		newNodePtr->Node.bPrintCut = true;
		pRQueueTail = newNodePtr;		
        newNodePtr->pNextNode = NULL;

	if (pRQueueHead == NULL)
	{
		pRQueueHead = newNodePtr;
		pRQueueNext = newNodePtr;
		pRQueueNow = newNodePtr;

	}
	else
	{
		pRQueueNow->pNextNode = newNodePtr;
		pRQueueNow = pRQueueTail;
	}
	NumberOfPrintJobsLeft++;
}


void CReceiptQ::ResetAll()
{
	pRQueueNow = pRQueueHead;
	while (pRQueueNow != pRQueueTail)
	{
		pRQueueNow->Node.bPrintJobDone = false;
		pRQueueNow = pRQueueNow->pNextNode;
	}
	pRQueueNow->Node.bPrintJobDone = false;
}


// Set Job Completed.  Completed should be on top of the queue, if not error
void CReceiptQ::SetJobCompleted(const long nJobId, int nDMLastDev)
{
	pRQueueNow = pRQueueNext;
	if ((pRQueueNow->Node.nPrintJobId == nJobId) && ((pRQueueNow->Node.nDeviceID == nDMLastDev) || (pRQueueNow->Node.nDeviceClass == nDMLastDev) || (pRQueueNow->Node.nDeviceClass == 999)))  //Tar 142845
	{
		pRQueueHead = pRQueueHead->pNextNode;
		if (pRQueueNow->Node.bPrintBitmap)
		{
			DeleteFile(pRQueueNow->Node.csBmpFile);

			//delete related files if any

			int length = pRQueueNow->Node.csBmpFile.GetLength();
			LPTSTR TempString = new _Tchar[length + 1];
			// char savechar;
			LPTSTR thischar;
			int i = -2;
			_tcscpy(TempString,(LPCTSTR)pRQueueNow->Node.csBmpFile);
			thischar = TempString;
			while (*thischar != NULL)
			{  
				if (*thischar == _T('.'))
					i++;
				if (i == 2)
				{
					i=0;
					*thischar = _T('\0');
					DeleteFile(TempString);
					*thischar = _T('.');
				}
				thischar++;
			}
			thischar = NULL;
			delete [] TempString;
		}
		RemoveNode(pRQueueNow);
		pRQueueNext = pRQueueHead;
	}
	else
	{
		// should log error message
		// assume jobs executed sequentially					ResetAll();
		// ReprintFrom(pRQueueNext->Node.nPrintJobId);   //place holder
	}
	pRQueueNow = pRQueueTail;
	NumberOfPrintJobsLeft--;
}



void CReceiptQ::RemoveNode(RQueueNode* pByeNode)   //never call from outside
{
			if (pRQueueNow->Node.bPrintBarCode)
			{
				try
				{
					delete [] pRQueueNow->Node.lpszBarCode;
				}
				catch(...)
				{
					trace(L6, _T("Got an exception in deleting barcode buffer in print receipt queue"));
				}
			}


	pRQueueNow = pRQueueNow->pNextNode;
	delete pByeNode;
}

void CReceiptQ::RemoveAll()
{
	pRQueueNow = pRQueueHead;
	while (pRQueueNow != pRQueueTail)
	{
		  if (pRQueueNow->Node.bPrintBitmap)
			  DeleteFile(pRQueueNow->Node.csBmpFile);
		      RemoveNode(pRQueueNow);
	}
    if (pRQueueNow->Node.bPrintBitmap)
		DeleteFile(pRQueueNow->Node.csBmpFile);
	delete pRQueueNow;
	NumberOfPrintJobsLeft=0;
}



void CReceiptQ::PrintAll()
{
	trace(L6, _T("+PrintAll"));
	RQueueNode* pRQueueTemp1 = pRQueueHead;  // temporary pointers for printall
	RQueueNode* pRQueueTemp2 = pRQueueHead;
	RQueueNode* pRQueueTemp3 = pRQueueTail;
	pRQueueHead = NULL;   // force creation of new queue
	pRQueueTail = NULL;
	pRQueueNow = NULL;
	NumberOfPrintJobsLeft=0;
	LPTSTR mesg = _T("DMI::ReceiptLine");     // we know this is for receipt prints
	while (pRQueueTemp2 != NULL)
	{

		if (pRQueueTemp2->Node.bPrintCut == true)
			dm.ReceiptCut();

		if (pRQueueTemp2->Node.bPrintReceiptLine == true)
		{
			dm.PrintLine(pRQueueTemp2->Node.nDeviceClass, pRQueueTemp2->Node.CSLineData);
		}

		if (pRQueueTemp2->Node.bPrintBarCode == true)
		{
			dm.PrintBarcode(pRQueueTemp2->Node.lpszBarCode,pRQueueTemp2->Node.lBCType,pRQueueTemp2->Node.lBCHeight,pRQueueTemp2->Node.lBCAlign,pRQueueTemp2->Node.fPrintNumbers,pRQueueTemp2->Node.nDeviceClass);
			try
			{
				delete [] pRQueueTemp2->Node.lpszBarCode;
			}
			catch(...)
			{
				trace(L6, _T("Got an exception in deleting barcode buffer in print receipt queue"));
			}
		}

		if (pRQueueTemp2->Node.bPrintBitmap == true)
		{
			if (pRQueueTemp2->Node.nDeviceClass == 999)  //receiptbitmap found
				dm.ReceiptBitMap(pRQueueTemp2->Node.csBmpFile);
			else
				dm.PrintBitmap(pRQueueTemp2->Node.csBmpFile,pRQueueTemp2->Node.nAlign,pRQueueTemp2->Node.nDeviceClass);
			DeleteFile(pRQueueTemp2->Node.csBmpFile);
		}
		pRQueueTemp2 = pRQueueTemp2->pNextNode;
		delete pRQueueTemp1;
		pRQueueTemp1 = pRQueueTemp2;
			
	}
	trace(L6, _T("-PrintAll"));
}
	    
void CReceiptQ::ReprintFrom(const long nJobId)
{
	ResetAll();
	pRQueueNow = pRQueueHead;
	while ((pRQueueNow->Node.nPrintJobId != nJobId) && (pRQueueNow != NULL))
	{ if (pRQueueNow->Node.bPrintBitmap)
		  DeleteFile(pRQueueNow->Node.csBmpFile);
	  RemoveNode(pRQueueNow);
	}
	pRQueueHead = pRQueueNow;
	pRQueueNext = pRQueueNow;
	PrintAll();
}

bool CReceiptQ::bPrintJobStatus(long nJobId)
{
		pRQueueNow = pRQueueHead;
		while (pRQueueNow != pRQueueTail)		//Tar 142845
		{
			if (pRQueueNow->Node.nPrintJobId != nJobId)
				pRQueueNow = pRQueueNow->pNextNode;
			else
				return pRQueueNow->Node.bPrintJobDone;
		}
		
		if (pRQueueNow->Node.nPrintJobId == nJobId)
			return pRQueueNow->Node.bPrintJobDone;
		else
		{
	//Tar 142845		pRQueueNow = pRQueueTail;
			return true;
		}
}

long CReceiptQ::nJobsInQueue()
{
	return (NumberOfPrintJobsLeft);
}

long CReceiptQ::nJobsToPrint()
{
	long Number = 0;
	pRQueueNow = pRQueueHead;
	while (pRQueueNow != pRQueueTail)
	{
		if (pRQueueNow->Node.bPrintJobDone != true)
			Number++;
		pRQueueNow = pRQueueNow->pNextNode;
	}
	if (pRQueueNow->Node.bPrintJobDone != true)
	return (++Number);
	return (Number);
}

// get Print nJobId

long CReceiptQ::nGetPrinterRequestCompletedID()
{
	return (dm.GetPrinterRequestCompletedID());
}
		