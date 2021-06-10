// SolutionMultiBitmap.h: interface for the CSolutionMultiBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLUTIONMULTIBITMAP_H__B7EF987B_6967_43C7_A34D_931C7C9D6BFD__INCLUDED_)
#define AFX_SOLUTIONMULTIBITMAP_H__B7EF987B_6967_43C7_A34D_931C7C9D6BFD__INCLUDED_

#include "SolutionTBDefs.h"

// SA250050: Mechanism for dynamic printer bitmap management through SetBitmap
// Requirements:
//				- Configuration flag must be set to enable this mechanism: 
//                   PrinterMultiBitmapEnabled
//				- support up to 20 bitmap indexes (configurable)
//				- queue all SetBitmap requests to be processed by FL when it's ready
//				- must be thread safe
//				- mechanism for FastLane to be notified OR to poll for bitmap updates
//				- mechanism for FastLane to pop bitmap update requests for processing
//				- (optional) persist bitmap requests that have not been processed
//				- (optional) implement alternative mechanism to load bitmaps, for example
//				     monitoring a POS folder for bitmaps being added/deleted, this will 
//				     be customer specific and we will only implement a stub 
class CSolutionMultiBitmap
{
public:
    CSolutionMultiBitmap();
    ~CSolutionMultiBitmap();

public:
    typedef struct _MBMP_REQ
    {
        long lIndex;
        long lStation;
	    CString csFilename;
        long lWidth;
        long lAlign;
    } MBMP_REQ, * PMBMP_REQ;

private:
// document definitions
    static LPCOLESTR MBMP;
    static LPCOLESTR MBMP_SETBMP;
    static LPCOLESTR MBMP_SETBMP_NUM;
    static LPCOLESTR MBMP_SETBMP_STN;
    static LPCOLESTR MBMP_SETBMP_FILE;
    static LPCOLESTR MBMP_SETBMP_WIDTH;
    static LPCOLESTR MBMP_SETBMP_ALIGN;
    static LPCOLESTR MBMP_HIST;
    static LPCOLESTR MBMP_UP;
    static LPCOLESTR MBMP_UP_PRN;
    static LPCOLESTR MBMP_UP_PRN_FL;
    static LPCOLESTR MBMP_UP_PRN_RAP;
    static LPCOLESTR MBMP_UP_STAT;  
    static LPCOLESTR MBMP_UP_STAT_PEND;  
    static LPCOLESTR MBMP_UP_STAT_OK;  
    static LPCOLESTR MBMP_UP_STAT_FAIL;  
    static LPCOLESTR MBMP_UP_REQ;
    static LPCOLESTR MBMP_UP_EXEC;
    static LPCOLESTR MBMP_UP_RES;

// document queries
    static LPCTSTR MBMP_QRY_PENDING_FL;
    static LPCTSTR MBMP_QRY_PENDING_RAP;
    static LPCTSTR MBMP_QRY_FAILED_FL;
    static LPCTSTR MBMP_QRY_FAILED_RAP;
    static LPCTSTR MBMP_QRY_PENDINGANDFAILED_FL;
    static LPCTSTR MBMP_QRY_PENDINGANDFAILED_RAP;
    static LPCTSTR MBMP_QRY_GETREQ;
    static LPCTSTR MBMP_QRY_GETREQUPD_FL;
    static LPCTSTR MBMP_QRY_GETREQUPD_RAP;
    static LPCTSTR MBMP_QRY_GETHISTUPD_FL;
    static LPCTSTR MBMP_QRY_GETHISTUPD_RAP;
	static LPCTSTR MBMP_QRY_GETUPDREQ_FL;

private:
    CCriticalSection m_critDoc;
	MSXML2::IXMLDOMDocument2Ptr m_spDoc;
    
public:
    virtual bool Init();
	virtual bool IsEnabled();
    virtual bool SyncRAP();
    virtual bool AddRequest(long lIndex, long lStation, CString csFilePath, 
                        long lWidth, long lAlign);
    virtual bool HaveFastLaneRequests();
    virtual bool GetFastLaneRequest(MBMP_REQ & req);
    virtual bool UpdateFastLaneRequest(long lIndex, long lStation, bool bSucceeded, CString csResult);
    
    virtual bool HaveRAPRequests();
    virtual bool GetRAPRequest(MBMP_REQ & req);
    virtual bool UpdateRAPRequest(long lIndex, long lStation, bool bSucceeded, CString csResult);
    
	virtual	void AddRecentUpdatedBitmaps();

protected:
    virtual void Save();
    virtual bool HaveRequests(bool bFastLane);
	virtual bool GetRequest(MBMP_REQ & req, bool bFastLane);
    virtual bool UpdateRequest(long lIndex, long lStation, bool bSucceeded, CString csResult, bool bFastLane);
    virtual CString RequestToString(MSXML2::IXMLDOMNodePtr spNode);

// internal helper methods
private: 
    
    void CreateDoc();
    MSXML2::IXMLDOMNodePtr GetRequestNode(long lIndex, long lStation);
    MSXML2::IXMLDOMNodePtr GetUpdateNode(long lIndex, long lStation, bool bFastLane);
    MSXML2::IXMLDOMNodePtr ClearAndReturnUpdateHistory(long lIndex, long lStation, bool bFastLane);
    void InitRequestUpdateSection(MSXML2::IXMLDOMNodePtr spNode);
    void SetAttr(MSXML2::IXMLDOMNodePtr spNode, LPCOLESTR strName, LPCOLESTR strValue = OLESTR(""));
    void SetAttr(MSXML2::IXMLDOMNodePtr spNode, LPCOLESTR strName, _variant_t vtValue);
    CString GetAttr(MSXML2::IXMLDOMNodePtr spNode, LPCOLESTR strName);
};




#endif // !defined(AFX_SOLUTIONMULTIBITMAP_H__B7EF987B_6967_43C7_A34D_931C7C9D6BFD__INCLUDED_)
