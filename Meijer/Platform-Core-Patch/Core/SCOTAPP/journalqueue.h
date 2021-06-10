
struct JournalQNode
{
		long nDeviceID;
		long nDeviceClass;
		CString CSLineData;
		CString CSmsg;
		long nPrintJobId;
		bool bPrintJobDone;		   // 1 done, 0 pending
		bool bPrintJournalLine;	   // 1 to print Receipt Line
		bool bPrintCut;			   // 1 to send cut command to printer
};

struct JQueueNode;

typedef JQueueNode* pJQueueNodePtr;

struct JQueueNode
{
	JournalQNode Node;
	pJQueueNodePtr pNextNode;
};




class CJournalQ
{
	public:
		void	AddPrintJobJournal(long nDeviceID, long nDeviceClass, const CString& CSLineData, CString CSmsg, const long nJobId); // add new String print job
		void	AddPrintJobCut(const long nJobId); // add new Cut print job
		void	SetJobCompleted(const long nJobId, int nDMLastDev); // set print job done true
		void	PrintAll();
		void	ReprintFrom(const long nJobId);  // print all starting at JobID
		bool	bPrintJobStatus(const long nJobId); //status of specific print job JobId
		void	RemoveAll();	  // remove all print jobs from the queue
		void	ResetAll();
		long    nJobsInQueue();	  // total number of jobs in the queue
		long	nJobsToPrint();	  // Jobs waiting to be printed
		void	RemoveNode(JQueueNode* pByeNode);
		long	nGetPrinterRequestCompletedID();
		CJournalQ();			  // constructor
		~CJournalQ();			  // destructor

	private:
		JQueueNode* pJQueueHead;  // pointer to first print job to complete
		JQueueNode* pJQueueNow;   // pointer to current print job
		JQueueNode* pJQueueTail;  // pointer to last print job in the queue
		JQueueNode* pJQueueNext;

};


