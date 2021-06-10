////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCashManagementExBase.h
//
// TITLE: Class header for store mode CashManagement state
//
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _SmCashManagementExBaseSTATE
#define _SmCashManagementExBaseSTATE

#include "SMCMOperatorPWState.h"
#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif


SMSmCashManagementExBase : public STATE(CMOperatorPWState)
{
public:
    SMSmCashManagementExBase();
    virtual ~SMSmCashManagementExBase(); 
    virtual SMStateBase  *Initialize(void);        // required override
    virtual void UnInitialize();
    virtual SMStateBase *DMAcceptor(void); 
    virtual SMStateBase *PSParse( const MessageElement* );
    virtual SMStateBase  *PSButton8(void);        
	virtual SMStateBase *PSEnterKey(void);                  //TAR 448426
    virtual void HandleCashierIDContext(CString csContext);
    virtual SMStateBase  *PSHotKey(CString &);
    virtual SMStateBase  *PSShiftKey(void);
virtual bool storeMode(){return true;};

    virtual SMStateBase * createLevel1Message( LPCTSTR szMsg,
                                            PSTEXT pstMsg,
                                            PSTEXT prompt,
                                            long   value,
                                            long   device,
                                            bool   checkHealth,
                                            bool displayHTML=false); //SR672
    virtual SMStateBase * createLevel2Message( LPCTSTR szMsg,
                                            PSTEXT pstMsg,
                                            PSTEXT prompt,
                                            long   value,
                                            long   device,
                                            bool   checkHealth,
                                            bool displayHTML=false); //SR672
    virtual SMStateBase * createLevel3Message( LPCTSTR szMsg,
                                            PSTEXT pstMsg,
                                            PSTEXT prompt,
                                            long   value,
                                            long   device,
                                            bool   checkHealth,
                                            bool displayHTML=false); //SR672
protected:
    static enum IPSProcedures::HKStatus m_HKStatus;
    static bool m_bShiftOn;
    virtual SMStateBase *DMParse( const MessageElement* ); // TAR 448430
    virtual long DMCashAcceptorAccept(void); //TAR 451616
	virtual SMStateBase *HandleScannerData( const MessageElement* );    //TAR 448426
    static BEST bestSaved;

    SMSmCashManagementExBase(const SMSmCashManagementExBase &rhs);

    DECLARE_DYNCREATE(SMSmCashManagementExBase)// MFC Runtime class/object information

private:
    SMStoreModeStateBase *m_pSMAccessor;
    CString m_csOperatorLoggedIn;
    CString m_csPasswordLoggedIn;
};

#endif
