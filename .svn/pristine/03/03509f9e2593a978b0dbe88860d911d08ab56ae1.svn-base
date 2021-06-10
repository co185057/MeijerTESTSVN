#ifndef _DMXADAPTER_H
#define _DMXADAPTER_H

#include "IDMWrapper.h"

//class CLog;
class CDeviceManagerEx;
class TraceObject;

/**
 * \class DMXAdapter
 * \brief Implementation of IDMWrapper interface that uses a CDeviceManagerEx
 *        object provided by the caller. 
 * \note  Copyright (c) NCR Corp. 2010
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
class CDMXAdapter : public IDMWrapper 
{
public:
    CDMXAdapter(CDeviceManagerEx &devmgr, TraceObject &logger);
    virtual ~CDMXAdapter();
    virtual bool ReadCashCounts(CString &result,bool &bDiscrepancy, bool bUseCached = false);
    virtual bool CoinAcceptorReadCoinCounts(CString &result);
    virtual bool CashAcceptorReadBillCounts(CString &result);
    virtual bool GetDeviceDescr(CString &result, long devClass, long devID=0);
    virtual bool Enable(bool enable, long deviceClass, long devID=0);
    virtual bool CashChangerDispenseByCount(const CString &dispstr, 
                                            long devID=0);
    virtual bool GetCashChangerDispensedCashList(CString &result, long devID=0);
    virtual bool CoinAcceptorResetCoinCounts(long devID=0);
    virtual bool CashAcceptorResetBillCounts(long devID=0);
    virtual bool GetCashAcceptorCurrencyCashList(CString &result, long devID=0);
    virtual bool GetCoinAcceptorEnabled(long devID=0);
    virtual bool SetCashAcceptorCurrencyAcceptList(const CString &newList, 
                                                   long devID=0);
    virtual bool GetCoinAcceptorCoinList(CString &result, long devID=0);
    virtual bool CashChangerSetCashCounts(LPCTSTR cashCounts, long devID=0); 
    virtual long DirectIO(long cmnd, long *pdata, CString &sdata, 
                          long devClass, long devID=0);
    virtual long GetResultCodeExtended(long devClass, long devID=0) ;
    virtual long CheckHealth(long level, long devClass, long devID=0);
    virtual bool GetCashChangerLowIndicatorList(CString &result, long devID=0);
    virtual bool GetCashChangerCurrencyContainerMap(CString &result, 
                                                    long devID=0);
    virtual bool SetCMState(bool bNewValue);
    virtual bool CashChangerSetPurgedCounts(LPCTSTR cashCounts, long devID=0);
    virtual bool CashChangerGetPurgedCounts(CString &result);
    
    virtual bool Initialize(void);
    virtual void TerminateDMWrapperThread(); 

	virtual bool CashChangerRemoveCoins(long* plData, const CString &csData, long devID=0);
    virtual long SetCoinAcceptorEnableCoins(bool bNewValue);//TAR 448440
	virtual bool EnableFPReader(bool bEnable);  //TAR 448426
    virtual bool PostDMEvent(long lDevClass, long lStatusErrorCode);    // SR752
    virtual bool ReadCassetteCounts(CString &result);   // Add Glory CM Support
protected:
    CDMXAdapter();                           
    CDMXAdapter(const CDMXAdapter &rhs);    ///< Disable copies.
    virtual void UnInitialize(void);

    void operator=(const CDMXAdapter &rhs); ///< Disable copies.

    CDeviceManagerEx *m_pDevMgr; 

    CString m_sLastCashCount;
};  

/**
 * \brief Class to make life w/ BSTR a little easier.
 */
class BSTRBlob
{
public:
    BSTRBlob();
    virtual ~BSTRBlob();

    operator BSTR();
    BSTR * GetBSTRptr(void);

private:
    BSTR m_bsData;
};

#endif // _DMXADAPTER_H
