#ifndef _IDMWRAPPER_H
#define _IDMWRAPPER_H

#include "ncrdevmgr.h"      // Need device class constants.

/**
 * \interface IDMWrapper
 * \brief Wrapper class to allow access to cash devices.
 * \warning  It is anticipated that these methods will only be called from one
 *        thread (the PSX callback) so no locks are used to protect 
 *        the argument buffer (m_pbuff).
 * \author Jeff Vales
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
class IDMWrapper 
{
public:
    IDMWrapper() {};
    virtual ~IDMWrapper() {}

    /**
     * Retrieve the current dispensable cash counts and cash unbalance status.
     * \param[out] result     - Holds cash count string.
     * \param[out] bDiscrepancy - Discrepancy flag. If true, a cash unbalance status
     *                          is detected. Otherwise false.                  
     * \param[in]  bUseCached - Performance enhancement.  If true, then
     *                          result will be the cached value of the last 
     *                          successful cash count reading.
     *
     *                          If false, then the value will be obtained from
     *                          the device.
     *                          
     * \return true if successful.  false otherwise.
     */
    virtual bool ReadCashCounts(CString &result, bool &bDiscrepancy, bool bUseCached = false) = 0;

    /**
     * Retrieve the current non-dispensable coin counts.
     * \param[out] result       Holds cash count string.
     * \return true if successful.  false otherwise.
     */
    virtual bool CoinAcceptorReadCoinCounts(CString &result) = 0;

    /**
     * Retrieve the current non-dispensable note counts.
     * \param[out] result       Holds cash count string.
     * \return true if successful.  false otherwise.
     */
    virtual bool CashAcceptorReadBillCounts(CString &result) = 0;

    /**
     * Retrieve the description for a device class.
     * \param[in]  devClass  Device class of the target device.
     * \param[out] result       Holds device description.
     * \return true if successful.  false otherwise.
     */
    virtual bool GetDeviceDescr(CString &result, 
                                long devClass, long devID=0) = 0;

    /**
     * Enable or disable a device.
     * \param[in]  enable       true to enable.  false to disable.
     * \param[in]  deviceClass  Device class of the target device.
     * \param[in]  devID - Device identifier.
     * \return true if successful.  false otherwise.
     */
    virtual bool Enable(bool enable, long deviceClass, long devID=0)=0;

    /**
     * Dispense counts of specific denominations.
     * \param[in] dispstr - String of the form 
     *      [<coinunit>':'<count>[','...]][';'<noteunit>':'<count>[','...]]
     * \param[in] devID - Device identifier.
     * \return true if successful.  false otherwise.
     */
    virtual bool CashChangerDispenseByCount(const CString &dispstr, long devID=0)=0;

    /**
     * Dispense counts of specific denominations.
     * \param[out] result - Result string of the form 
     *      [<coinunit>':'<count>[','...]][';'<noteunit>':'<count>[','...]]
     * \param[in] devID - Device identifier.
     * \return true if successful.  false otherwise.
     */
    virtual bool GetCashChangerDispensedCashList(CString &result, long devID=0)=0;

    /**
     * Set coin acceptor or coin overflow counts to zero.
     * \param[in] devID - Device identifier.
     * \return true if successful.  false otherwise.
     */
    virtual bool CoinAcceptorResetCoinCounts(long devID=0)=0;

    /**
     * Set coin acceptor or coin overflow counts to zero.
     * \param[in] devID - Device identifier.
     * \return true if successful.  false otherwise.
     */
    virtual bool CashAcceptorResetBillCounts(long devID=0)=0;

    /**
     * Retrieve a list of accepted note denominations.
     * \param[out] result - comma-separated numeric list of supported cash 
     *                      acceptor denominations.
     * \param[in] devID - Device identifier.
     * \return true if successful.  false otherwise.
     */
    virtual bool GetCashAcceptorCurrencyCashList(CString &result, long devID=0)=0;

    /**
     * Get status of CoinAcceptor.
     * \param[in] devID - Device identifier.
     * \return true if enabled.  false if disabled.
     */
    virtual bool GetCoinAcceptorEnabled(long devID=0)=0;

    /**
     * Set accepted note denominations.
     * \param[in] newList - comma-separated numeric list of supported cash 
     *                      acceptor denominations.
     * \param[in] devID - Device identifier.
     * \return true if successful.  false otherwise.
     */
    virtual bool SetCashAcceptorCurrencyAcceptList(const CString &newList, long devID=0)=0;

    /**
     * Retrieve a list of accepted coin denominations.
     * \param[out] result - comma-separated numeric list of supported coin 
     *                      acceptor denominations.
     * \param[in] devID - Device identifier.
     * \return true if successful.  false otherwise.
     */
    virtual bool GetCoinAcceptorCoinList(CString &result, long devID=0)=0;

    /**
     * Send new cash counts to the cash changer.
     * \param[in] cashCounts - New cash count string.
     * \param[in] devID - Device identifier.
     * \return true if successful.  false otherwise.
     */
    virtual bool CashChangerSetCashCounts(LPCTSTR cashCounts, long devID=0)=0; 

    /**
     * Call DirectIO on a device.
     * \param[in] cmnd - DirectIO command.
     * \param[in,out] pdata - long data sent to or returned from the call.
     * \param[in,out] sdata - string data sent to or returned from the call.
     * \param[in]  devClass  Device class of the target device.
     * \param[in] devID - Device identifier.
     * \return result code as long
     */
    virtual long DirectIO(long cmnd, long *pdata, CString &sdata, 
                  long devClass, long devID=0)=0;

    /**
     * Get result code extended
     * \param[in]  devClass  Device class of the target device.
     * \param[in] devID - Device identifier.
     * \return result code extended as long
     */
    virtual long GetResultCodeExtended(long devClass, long devID=0) =0;

    /**
     * Perform device health check.
     * \param[in]  level     Level of health check to perform.
     * \param[in]  devClass  Device class of the target device.
     * \param[in]  devID     Device identifier.
     * \return device CheckHealth result.
     */
    virtual long CheckHealth(long level, long devClass, long devID=0)=0;

    /**
     * Retrieve cash changer device information
     * \param[out]  result   Comma-separated string that contains a status
     *                       identifier for each currency container.  Position
     *                       in the list corresponds to position in list 
     *                       returned by GetCashChangerCurrencyConatinerMap.
     *
     * \param[in]  devID     Device identifier.
     * \return true on success.  false on failure.
     * \note Added for RFC 400821
     */
    virtual bool GetCashChangerLowIndicatorList(CString &result, long devID=0)=0;

    /**
     * Retrieve list denominations that correspond to the status in the list
     * returned by GetCashChangerLowIndicatorList.
     * \param[out]  result   Comma-separated string that contains the 
     *                       denomination of each currency container.
     * \param[in]  devID     Device identifier.
     * \return true on success.  false on failure.
     * \note Added for RFC 400821
     */
    virtual bool GetCashChangerCurrencyContainerMap(CString &result, long devID=0)=0;

	/**
     * Set Enter CM and Exit CM state
     * \param[in]  bNewValue -  
     *  If true, then set Device Manager to "cash management mode";
     *  If false, then set Device Manager to normal mode.
     *  Setting DM to cash management mode will block currency insertion events 
     *  from propogating up to SCOTApp.
     *                          
     * \return true if successful.  false otherwise.
     */
	virtual bool SetCMState(bool bNewValue)=0;

    /**
     * Send new purge operation count data to the cash changer.
     * \param[in] cashCounts   New purge operation counts to send to the device. 
	 * \param[in] devID - Device identifier.
     * \return true on success.  
     * \return false on failure.
	 * \note  Format of the string is [';'<noteunit>':'<count>[','...]]
	 *  NULL or empty string should clear all purge operation counts. However,
     *  as of ADD 2.1.15 the SO does not implement the NULL or empty string 
     *  option.
     *  Instead error 106 is returned from the SO.
     */
	virtual bool CashChangerSetPurgedCounts(LPCTSTR cashCounts, long devID=0)=0;

	/**
     * Retrieve the current purged operation counts.
     * \param[out] result   Holds the purged operation count result.
     * \return true on success.  
     * \return false on failure.
	 * \note  Format of the string is 
     * \note  [';'<noteunit>':'<count>[','...]]
     */
    virtual bool CashChangerGetPurgedCounts(CString &result)=0;

    virtual bool Initialize(void)=0;

    //Terminate the thread created by IDMWrapper
	virtual void TerminateDMWrapperThread()=0; 
	
	/**
     * This method removes/empty remaining coins in the hopper bins. 
     * \param[in,out]  plData    long data send to or returned from the call.
     * \param[in]      csDump    string data send to from the call.
     * \param[in]      devID     Device identifier.
     * \return true if success, otherwise false.
     */
    virtual bool CashChangerRemoveCoins(long* plData, const CString &csDump, long devID=0)=0;

    virtual long SetCoinAcceptorEnableCoins(bool bNewValue)=0;//TAR 448440
	
	/**
     * This method handles to enable/disable the fingerprint device 
     * \param[in]  bEnable - if true, FP device will be enable
     *                     - if false, FP device will be disable
     * \return true if success, otherwise false.
     * \note Added for TAR 448426
     */
    virtual bool EnableFPReader(bool bEnable)=0;

	/**
     * This method will post DM event to application.
     * \param[in] lDevClass - device class
     * \param[in] lStatusErrorCode - status error code
     * \return true if success, false on failure
     * \note Added for TAR SR752
     */
    virtual bool PostDMEvent(long lDevClass, long lStatusErrorCode) = 0;
  
    /**
     * Retrieve the current cassette counts / nondispensable cash counts.
     * \param[out] csResult - Holds the current cassette counts
     * \return true if success, false on failure
     * \note Added for Add Glory CM Support feature
     */
    virtual bool ReadCassetteCounts(CString &csResult) = 0;   

protected:
    IDMWrapper(const IDMWrapper &rhs);       ///< Disable copies.
    virtual void UnInitialize(void)=0;

    void operator=(const IDMWrapper &rhs);  ///< Disable copies.
};  

#endif // _IDMWRAPPER_H
