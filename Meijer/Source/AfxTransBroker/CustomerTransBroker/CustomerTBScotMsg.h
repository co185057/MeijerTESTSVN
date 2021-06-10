//**********************************************************************************
//	CustomerTBScotMsg.h   :   declaration file for transaction broker structures and # defines
//
//	The Transaction Broker will notify the application of events coming from the host application.  
//	All notifications from the Transaction Broker will be posted as windows messages.
//	                                                                              
//	$Header:
//
//	CHANGES: Start                                                                
//	CHANGES: End                                                                  
//                                                                                
//                                                                                
//***********************************************************************************

#ifndef  _CustomerTBScotMsg_H_
#define	_CustomerTBScotMsg_H_

#include "BaseTBScotMsg.h"
#include "SingleInstance.h"

class CCustomerTBScotMsg : public CBaseTBScotMsg, public CSingleInstance<CCustomerTBScotMsg>
{
    friend class CSingleInstance<CCustomerTBScotMsg>;
public:
    virtual ~CCustomerTBScotMsg();

    virtual long SendReturnStateMsg( long lRetStat, RetStateMsgHandlerType i_pfn=NULL, const TCHAR *szMessage=NULL);

protected:
   	CCustomerTBScotMsg();	// should only be called in this class
    
private:
    CCustomerTBScotMsg(const CCustomerTBScotMsg &);              // hide copy const
    CCustomerTBScotMsg & operator =(const CCustomerTBScotMsg &); // hide assignment
};



#endif // _CustomerTBScotMsg_H_