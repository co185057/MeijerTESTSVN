#ifndef _GLORYADAPTER_H_
#define _GLORYADAPTER_H_

/**
 * \class CGloryAdapter
 * \brief Implementation of IDMWrapper interface that uses a CDeviceManagerEx and 
 *        CCashRecyclerwrapper object provided by the caller dedicated to Glory.
 * \author Jeffrey Yu and Renato Taer
 * \note  Copyright (c) NCR Corp. 2012
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
*/

#include "IDMWrapper.h" 

// Refer to IDMWrapper.h for method descriptions.
class CGloryAdapter : public IDMWrapper  
{
public:
     /**
     * Takes an existing IDMWrapper and 'decorates' it with 
     * additional functionality with CCashRecyclerWrapper and CDMXAdapter
     * \param[in]  *devmgr    CDeviceManagerEx object.
     * \param[in]  *recycler  CashRecyclerWrapper object.
	 * \param[in]  *pWrapper  CDMXAdapter object
     */
    CGloryAdapter(CDeviceManagerEx &devmgr, 
		CCashRecyclerWrapper *pRecycler, IDMWrapper *pWrapper);

    virtual ~CGloryAdapter();
    virtual bool Initialize(void);
    virtual void UnInitialize(void);
    virtual bool GetDeviceDescr(CString &result, long devClass, long devID=0);
    virtual bool ReadCashCounts(CString &result, bool &bDiscrepancy, bool bUseCached = false);  
    virtual bool CoinAcceptorReadCoinCounts(CString &result);
    virtual bool CashAcceptorReadBillCounts(CString &result) ;
    virtual bool Enable(bool enable, long deviceClass, long devID=0);
    virtual bool CashChangerDispenseByCount(const CString &dispstr, long devID=0);
    virtual bool GetCashChangerDispensedCashList(CString &result, long devID=0);
    virtual bool CoinAcceptorResetCoinCounts(long devID=0);
    virtual bool CashAcceptorResetBillCounts(long devID=0);
    virtual bool GetCashAcceptorCurrencyCashList(CString &result, long devID=0);
    virtual bool GetCoinAcceptorEnabled(long devID=0);
    virtual bool SetCashAcceptorCurrencyAcceptList(const CString &newList, long devID=0);
    virtual bool GetCoinAcceptorCoinList(CString &result, long devID=0);
    virtual bool CashChangerSetCashCounts(LPCTSTR cashCounts, long devID=0); 
    virtual long DirectIO(long cmnd, long *pdata, CString &sdata, 
                  long devClass, long devID=0);
    virtual long GetResultCodeExtended(long devClass, long devID=0) ;
    virtual long CheckHealth(long level, long devClass, long devID=0);
    virtual bool GetCashChangerLowIndicatorList(CString &result, long devID=0);
    virtual bool GetCashChangerCurrencyContainerMap(CString &result, long devID=0);
    virtual bool SetCMState(bool bNewValue);
    virtual bool CashChangerSetPurgedCounts(LPCTSTR cashCounts, long devID=0);
    virtual bool CashChangerGetPurgedCounts(CString &result);
    virtual void TerminateDMWrapperThread(); 
    virtual bool CashChangerRemoveCoins(long* plData, const CString &csDump, long devID=0);
    virtual long SetCoinAcceptorEnableCoins(bool bNewValue);
    virtual bool EnableFPReader(bool bEnable);
    virtual bool PostDMEvent(long lDevClass, long lStatusErrorCode);  
    virtual bool ReadCassetteCounts(CString &result);
protected:
    CString m_csLastCashCount;
    CDeviceManagerEx *m_pDevMgr; 
    IDMWrapper *m_pWrapper;               
    CCashRecyclerWrapper *m_pRecycler;
};

#endif // !defined _GLORYADAPTER_H_
