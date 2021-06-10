
struct ReceiptQNode
{
		long nDeviceID;
		long nDeviceClass;
		CString CSLineData;
		CString CSmsg;
		long nPrintJobId;
		bool bPrintJobDone;		   // 1 done, 0 pending
		bool bPrintReceiptLine;	   // 1 to print Receipt Line
		bool bPrintCut;			   // 1 to send cut command to printer
		//barcode
		LPTSTR lpszBarCode;
		long  lBCType;
		long  lBCHeight;
		long  lBCAlign;
		bool  fPrintNumbers;
		bool bPrintBarCode;		   // 1 to print Barcode
		//bitmap
		CString csBmpFile;
		long  nAlign;
		bool bPrintBitmap;		   // 1 to print bitmap
		ReceiptQNode()	{ lpszBarCode = NULL;};
};
struct RQueueNode;

typedef RQueueNode* RQueueNodePtr;

struct RQueueNode
{
	ReceiptQNode Node;
	RQueueNodePtr pNextNode;
};




class CReceiptQ
{
	public:
		void	AddPrintJobReceipt(long nDeviceID, long nDeviceClass, const CString CSLineData, CString CSmsg, const long nJobId); // add new String print job
		void    AddPrintJobReceipt(long nDevClass, LPCTSTR lpszBarCode, long lBCType, long lBCHeight, long lBCAlign, bool fPrintNumbers, const long nJobId); //add new printbitmap job
		void	AddPrintJobReceipt(CString csBmpFile,const long nJobId); // add new ReceiptBitmap job
		void	AddPrintJobCut(const long nJobId); // add new Cut print job
		void	SetJobCompleted(const long nJobId, int nDMLastDev); // set print job done true
		void	PrintAll();
		void	ReprintFrom(const long nJobId);  // print all starting at JobID
		bool	bPrintJobStatus(const long nJobId); //status of specific print job JobId
		void	RemoveAll();	  // remove all print jobs from the queue
		void	ResetAll();
		long    nJobsInQueue();	  // total number of jobs in the queue
		long	nJobsToPrint();	  // Jobs waiting to be printed
		void	RemoveNode(RQueueNode* pByeNode);
		long	nGetPrinterRequestCompletedID();
		//bitmap
		void	AddPrintJobReceipt(long nDevClass, CString csBmpFile, long nAlign, const long nJobId); // add new Bitmap print job
		CReceiptQ();			  // constructor
		~CReceiptQ();			  // destructor

	private:
		RQueueNode* pRQueueHead;  // pointer to first print job to complete
		RQueueNode* pRQueueNow;   // pointer to current print job
		RQueueNode* pRQueueTail;  // pointer to last print job in the queue
		RQueueNode* pRQueueNext;

};


