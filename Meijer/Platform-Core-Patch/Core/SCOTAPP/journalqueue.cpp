#include "stdafx.h"
#include "commonNCR.h"
#include "common.h"
#include "commonServices.h"
//DMInterface jdmi;

//#include "printerqueue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




//constructor
CJournalQ::CJournalQ()
{
	pJQueueHead = NULL;
	pJQueueNow = NULL;
	pJQueueTail = NULL;

}

//destructor
CJournalQ::~CJournalQ()
{
	pJQueueNow = pJQueueHead;
	if (pJQueueNow != NULL)
	{
		while (pJQueueNow != pJQueueTail)
			RemoveNode(pJQueueNow);
		delete pJQueueNow;
	}
}

// Add new ReceiptLine print job
void CJournalQ::AddPrintJobJournal(long nDeviceID, long nDeviceClass, const CString& CSLineData, CString CSmsg, const long nJobId)
{
	LPTSTR savingdata;
	int length = CSLineData.GetLength() + 1;
	savingdata = new _TCHAR[length];
	memset(savingdata,0,length);
	_tcscpy(savingdata,(LPCTSTR)CSLineData);

	CString CSSavemsg = CSmsg;
	pJQueueNodePtr newNodePtr = new JQueueNode;

	newNodePtr->Node.nDeviceID = nDeviceID;
	newNodePtr->Node.nDeviceClass = nDeviceClass;
	newNodePtr->Node.CSLineData = savingdata;
	delete [] savingdata;
	newNodePtr->Node.CSmsg = CSSavemsg;
	newNodePtr->Node.bPrintJobDone = false;
	newNodePtr->Node.nPrintJobId = nJobId;

	newNodePtr->Node.bPrintJournalLine = true;
	newNodePtr->Node.bPrintCut = false;
	pJQueueTail = newNodePtr;		
    newNodePtr->pNextNode = NULL;

	if (pJQueueHead == NULL)
	{
		pJQueueHead = newNodePtr;
		pJQueueNext = newNodePtr;
		pJQueueNow = newNodePtr;

	}
	else
	{
		pJQueueNow->pNextNode = newNodePtr;
		pJQueueNow = pJQueueTail;
	}
}




// Add new BitMap print job
void CJournalQ::AddPrintJobCut(const long nJobId)
{
		pJQueueNodePtr newNodePtr = new JQueueNode;
		newNodePtr->Node.nDeviceID = 0;
		newNodePtr->Node.nDeviceClass = 0;
		newNodePtr->Node.CSLineData = _T(" ");
		newNodePtr->Node.CSmsg = _T(" ");
		newNodePtr->Node.bPrintJobDone = false;
		newNodePtr->Node.nPrintJobId = nJobId;

		newNodePtr->Node.bPrintJournalLine = false;
		newNodePtr->Node.bPrintCut = true;
		pJQueueTail = newNodePtr;		
        newNodePtr->pNextNode = NULL;

	if (pJQueueHead == NULL)
	{
		pJQueueHead = newNodePtr;
		pJQueueNext = newNodePtr;
		pJQueueNow = newNodePtr;

	}
	else
	{
		pJQueueNow->pNextNode = newNodePtr;
		pJQueueNow = pJQueueTail;
	}
}


void CJournalQ::ResetAll()
{
	pJQueueNow = pJQueueHead;
	while (pJQueueNow != pJQueueTail)
	{
		pJQueueNow->Node.bPrintJobDone = false;
		pJQueueNow = pJQueueNow->pNextNode;
	}
	pJQueueNow->Node.bPrintJobDone = false;
}


// Set Job Completed
void CJournalQ::SetJobCompleted(const long nJobId, int nDMLastDev)
{
		pJQueueNow = pJQueueNext;
			if ((pJQueueNow->Node.nPrintJobId == nJobId) && (pJQueueNow->Node.nDeviceID == nDMLastDev))
				{
				  pJQueueHead = pJQueueHead->pNextNode;
				  RemoveNode(pJQueueNow);
				  pJQueueNext = pJQueueHead;
				}
			else
				{
					// should log error message
					// Tar 142846  ResetAll();
					// Tar 142846  ReprintFrom(pJQueueNext->Node.nPrintJobId);   //place holder
				}

	    pJQueueNow = pJQueueTail;

			
}



void CJournalQ::RemoveNode(JQueueNode* pByeNode)
{
	pJQueueNow = pJQueueNow->pNextNode;
	delete pByeNode;
}

void CJournalQ::RemoveAll()
{
	pJQueueNow = pJQueueHead;
	while (pJQueueNow != pJQueueTail)
		RemoveNode(pJQueueNow);
	delete pJQueueNow;
}



void CJournalQ::PrintAll()
{
		JQueueNode* pJQueueTemp1 = pJQueueHead;  // temporary pointers for printall
		JQueueNode* pJQueueTemp2 = pJQueueHead;
		JQueueNode* pJQueueTemp3 = pJQueueTail;
		pJQueueHead = NULL;   // force creation of new pJQueue
		pJQueueTail = NULL;
		pJQueueNow = NULL;
		LPTSTR mesg = _T("DMI::JournalLine");     // we know this is for receipt prints

	while (pJQueueTemp2 != NULL)
	{

		if (pJQueueTemp2->Node.bPrintCut == true)
			dm.ReceiptCut();

		if (pJQueueTemp2->Node.bPrintJournalLine == true)
		{
			// do dmiPrintReceiptLine, get new nJobId and save it
			//jdmi.PrinterPrintString(pJQueueTemp2->Node.nDeviceID,pJQueueTemp2->Node.nDeviceClass,pJQueueTemp2->Node.CSLineData,mesg); 
		     dm.PrintLine(pJQueueTemp2->Node.nDeviceClass, pJQueueTemp2->Node.CSLineData);
			pJQueueTemp2 = pJQueueTemp2->pNextNode;
			delete pJQueueTemp1;
			pJQueueTemp1 = pJQueueTemp2;
		}	
	}
}
	    
void CJournalQ::ReprintFrom(const long nJobId)
{
	ResetAll();
	pJQueueNow = pJQueueHead;
	while ((pJQueueNow->Node.nPrintJobId != nJobId) && (pJQueueNow != NULL))
		RemoveNode(pJQueueNow);
	pJQueueHead = pJQueueNow;
	pJQueueNext = pJQueueNow;
	PrintAll();
}

bool CJournalQ::bPrintJobStatus(const long nJobId)
{
		pJQueueNow = pJQueueHead;
		while (pJQueueNow != pJQueueTail)    //Tar 142846
		{
			if (pJQueueNow->Node.nPrintJobId != nJobId)
				pJQueueNow = pJQueueNow->pNextNode;
			else
				return pJQueueNow->Node.bPrintJobDone;
		}
		
		if (pJQueueNow->Node.nPrintJobId == nJobId)
			return pJQueueNow->Node.bPrintJobDone;
		else
		{
//Tar 142846			pJQueueNow = pJQueueTail;
			return true;
		}
}

long CJournalQ::nJobsInQueue()
{
	long Number = 0;
	pJQueueNow = pJQueueHead;
	while (pJQueueNow != pJQueueTail)
	{
		pJQueueNow = pJQueueNow->pNextNode;
		Number++;
	}

	return (++Number);
}

long CJournalQ::nJobsToPrint()
{
	long Number = 0;
	pJQueueNow = pJQueueHead;
	while (pJQueueNow != pJQueueTail)
	{
		if (pJQueueNow->Node.bPrintJobDone != true)
			Number++;
		pJQueueNow = pJQueueNow->pNextNode;
	}
	if (pJQueueNow->Node.bPrintJobDone != true)
	return (++Number);
	return (Number);
}

// get Print JobId

long CJournalQ::nGetPrinterRequestCompletedID()
{
	return (dm.GetPrinterRequestCompletedID());
}
		