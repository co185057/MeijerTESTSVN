// tbMessageMap.h: interface for the CtbMessageMap class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TRXMESSAGEMAP_H_
#define _TRXMESSAGEMAP_H_
#include <afxtempl.h>
#include "basetbDefs.h"
#include "TraceSystem.h"



template <class tMsgMapType> 
class CMsgInfo: public CObject {
	public:
        CMsgInfo(){};
	CTypedPtrList<CPtrList, tMsgMapType*> msgStateMap;
	private:
		// hide copy and assignment
		CMsgInfo(const CMsgInfo&);
		CMsgInfo& operator = (const CMsgInfo &);
};

template <class tMsgMapType> 
class CtbMessageMap : public CTypedPtrMap <CMapStringToOb, CString, CMsgInfo<tMsgMapType>*>
{
public:
	CtbMessageMap();
	virtual ~CtbMessageMap();
	BOOL Initialize(tMsgMapType *pMsgMapEntries, int maxcnt=0);
	BOOL InitializeWithWindow(tMsgMapType *pMsgMapEntries, int maxcnt);
	int		GetSize();
	
protected:
	void RemoveDoubleQuotes(CString& csOrigString);

private:
	int  m_nListSize;
   void ClearMap();
	bool ExtractStringContent(CString& csOrigString, TCHAR chDilimiter1, TCHAR chDilimiter2 = NULL);

	// hide copy and assignment
	CtbMessageMap(const CtbMessageMap&);
	CtbMessageMap& operator = (const CtbMessageMap &);
};

template <class tMsgMapType>
CtbMessageMap<tMsgMapType>::CtbMessageMap()
{
	m_nListSize = 0;				// current list size is zero
}

template <class tMsgMapType>
CtbMessageMap<tMsgMapType>::~CtbMessageMap()
{
   ClearMap();
}

template <class tMsgMapType>
void CtbMessageMap<tMsgMapType>::ClearMap()
{
   POSITION pos;
	CMsgInfo<tMsgMapType>* pmsgInfo;
	CString string;
	for( pos = GetStartPosition(); pos != NULL; ) {
		GetNextAssoc( pos, string, pmsgInfo );
		delete pmsgInfo;
	}
	// RemoveAll deletes the keys
	RemoveAll();
   m_nListSize=0;
}

template <class tMsgMapType>
int CtbMessageMap<tMsgMapType>::GetSize()
{
	return m_nListSize;
}

template <class tMsgMapType>
BOOL CtbMessageMap<tMsgMapType>::Initialize(tMsgMapType *pMsgMapEntries, int maxcnt)
{
	//_ASSERT(!m_nListSize);
   ClearMap();
	CMsgInfo<tMsgMapType> *pmi;

	for (int i=0; i<maxcnt; i++) {
		//cleanup the string message
		CString  csStringValue(pMsgMapEntries[i].pmsg);
		csStringValue.TrimLeft();
		csStringValue.TrimRight();
//      csStringValue.Remove(' ');
      csStringValue.MakeLower();



		// if msg already in our list, point to it, otherwise add it
		if (!Lookup(csStringValue, pmi)) {
			pmi = new CMsgInfo<tMsgMapType>; // the ~CtbMessageMap deletes this
			SetAt(csStringValue, pmi);
		}
		// don't allocate new memory for this.
		// just add a pointer to the existing array element
		pmi->msgStateMap.AddTail(&pMsgMapEntries[i]); // &displayMsgMap[i]);
		
		++m_nListSize;	// increment list size
	}
	_ASSERT(GetSize());
	return TRUE;	// successfully built the list
}

template <class tMsgMapType>
void CtbMessageMap<tMsgMapType>::RemoveDoubleQuotes(CString& csQuoteString)
{
	ExtractStringContent(csQuoteString, '\"');
}

template <class tMsgMapType>
bool CtbMessageMap<tMsgMapType>::ExtractStringContent(CString& csOrigString, TCHAR chDilimiter1, TCHAR chDilimiter2)
{
	bool retValue = false;
	int nOrigStringLen = csOrigString.GetLength();
	if (!nOrigStringLen) return retValue;
	if (!chDilimiter2)
		chDilimiter2 = chDilimiter1;
	if ((csOrigString[0] == chDilimiter1) && (csOrigString[nOrigStringLen - 1] == chDilimiter2))
		csOrigString = csOrigString.Mid(1,nOrigStringLen - 2);
	return retValue;
}

template <class tMsgMapType>
BOOL CtbMessageMap<tMsgMapType>::InitializeWithWindow
							(tMsgMapType *pMsgMapEntries, int maxcnt)
{
	_ASSERT(!m_nListSize);
	CMsgInfo<tMsgMapType> *pmi;
	CString  csStringValue;

	for (int i=0; i<maxcnt; i++) {
		//cleanup the string message
		csStringValue= pMsgMapEntries[i].pmsg;
		csStringValue.TrimLeft();
		csStringValue.TrimRight();

		TCHAR szWindow[20];
		_itoa(pMsgMapEntries[i].window, szWindow, 10);
		csStringValue = CString(szWindow) + csStringValue;

        // if msg already in our list, point to it, otherwise add it
		if (!Lookup(csStringValue, pmi)) {
			pmi = new CMsgInfo<tMsgMapType>; // the ~CtbMessageMap deletes this
			SetAt(csStringValue, pmi);
		}
		// don't allocate new memory for this.
		// just add a pointer to the existing array element
		pmi->msgStateMap.Add(&pMsgMapEntries[i]);
		++m_nListSize;	// increment list size
	}
	_ASSERT(GetSize());
	return TRUE;	// successfully built the list
}


#endif // _TRXMESSAGEMAP_H_
