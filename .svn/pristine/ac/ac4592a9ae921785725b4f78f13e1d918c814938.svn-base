#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/exe - TestRCM/PSXInterface.h 3     10/21/08 4:06p Sm133076 $
/*
 * @(#)PSXInterface.h    $Revision: 3 $ $Date: 10/21/08 4:06p $
 *
 * Copyright 2004 by NCR Corporation,
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/exe - TestRCM/PSXInterface.h $
 * 
 * 3     10/21/08 4:06p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:14p Dm185016
 * Moved to new Repository.
* 
* 2     10/29/04 8:49a Dm185016
* Single source with SCOTAPP.
* 
* 3     10/14/04 2:00p Dm185016
* Fixed problems recognizing the buttons
* 
* 2     10/08/04 1:39p Dm185016
*/

#include "singleton.h"
#include "StringComparator.h"
#include <set>
#include <map>

#if !defined(_SCOT) && !defined(_SCOTSSF_)
#include "BaseObject.h"
#include "KeyValueHandler.h"
#define OTHER_BASE_CLASSES    , public CBaseObject
class CRemoteConsoleMgrWrapper;

#include <set>

typedef std::set<CString> CConnections;
#else
#define OTHER_BASE_CLASSES 
#endif


#define USE_VALUE    0
#define IGNORE_VALUE 0xffffffff
#define NO_VALUE     0xfffffffe

#define BEGIN_MAPPING(NAME)\
    SMappingEntry NAME[] = {

// Use this macro to explicitly set the event type, wparm, and lparm
#define MAP_ENTRY_N_W_L(KEY, TYPE, WPARAM, LPARAM)\
    { KEY, TYPE, WPARAM, USE_VALUE, LPARAM, USE_VALUE, NULL },

// Use this macro to explicitly set only the event type
#define MAP_ENTRY_N_0_0(KEY, TYPE)\
    { KEY, TYPE, NO_VALUE, IGNORE_VALUE, NO_VALUE, IGNORE_VALUE, NULL },

// Use this macro to explicitly set the event type and wparm
#define MAP_ENTRY_N_W_0(KEY, TYPE, WPARAM)\
    { KEY, TYPE, WPARAM, USE_VALUE, NO_VALUE, IGNORE_VALUE, NULL },

// Use this macro to explicitly set the event type and lparm
#define MAP_ENTRY_N_0_L(KEY, TYPE, LPARAM)\
    { KEY, TYPE, NO_VALUE, IGNORE_VALUE, LPARAM, USE_VALUE, NULL },

// Use this macro to explicitly set the event type, wparm, and lparm
#define MAP_ENTRY_N_0_0_METHOD(KEY, TYPE, METHOD)\
    { KEY, TYPE, 0, IGNORE_VALUE, 0, IGNORE_VALUE, METHOD },

// Must end the mapping
#define END_MAPPING()\
    { NULL, PS_UKEVT, NO_VALUE, IGNORE_VALUE, NO_VALUE, IGNORE_VALUE, NULL }\
};

// Forward define required
class CPSX;

struct _SPSXEvent
{
    CString strRemoteConnectionName;
    CString strControlName;
    CString strContextName;
    CString strEventName;
    _variant_t vParam;
    PSEVT nEvent;
    long lParam;
    long wParam;
};
typedef struct _SPSXEvent SPSXEvent;
typedef SPSXEvent *PPSXEVENT;

// Typedef for functions needing to hook into the PSX event handler processing.
typedef bool (*PPSXEVENTHANDLER)( CPSX *pPSX
                                , PPSXEVENT pEvent );

// Typedef used for saving event handler chain
typedef std::set<PPSXEVENTHANDLER> CEventHandlers;
typedef CEventHandlers::iterator CEventHandlersIterator;

///<summary>Singleton class responsible for receiving events from PSX and distributing
///to registered handlers</summary>
class CPSXInterface
    : public CSingleton<CPSXInterface>
    OTHER_BASE_CLASSES
{

public:

    struct SMappingEntry_;
    typedef bool (*PMAPPINGFXN)( SMappingEntry_ *pMappingEntry
                               , PPSXEVENT pEvent );


    // Structure to contain the parameter values to be used for a control
    struct SMappingEntry_
    {
        _TCHAR     *keyname;        // Name of the control for this mapping entry
        PSEVT       evtType;        // Type of key
        long        wparm;          // wParm value to use
        long        wparmUsage;     // If this is USE_VALUE, then above value is valid.
                                    // If it is IGNORE_VALUE, then do not use above value.
        long        lparm;          // lParm value to use
        long        lparmUsage;     // If this is USE_VALUE, then above value is valid.
                                    // If it is IGNORE_VALUE, then do not use above value.
        PMAPPINGFXN pMappingFxn;    // Pointer to a function to use for this entry
    };

    typedef struct SMappingEntry_ SMappingEntry;
    typedef SMappingEntry_ *PMAPPINGENTRY;
    typedef std::map <CString, PMAPPINGENTRY, StringComparator> CMapping;

    friend class CSingleton<CPSXInterface>;

	virtual ~CPSXInterface() throw();

    ///
    ///<summary>This function is called by the UI subsystem when an event occurs
    ///                  (i.e. button click)</summary>
    ///<param name="pPSX">pointer to an instance of PSX object</param>
    ///<param name="strRemoteConnectionName">remote connection name</param>
    ///<param name="strControlName">name of control that generated the event</param>
    ///<param name="strContextName">name of context</param>
    ///<param name="strEventName">name of event - Click, ...</param>
    ///<param name="vParam">extra event specific parameter</param>
    ///
    static void EventHandler( CPSX *pPSX
                            , const CString& strRemoteConnectionName
                            , const CString& strControlName
                            , const CString& strContextName
                            , const CString& strEventName
                            , const COleVariant& vParam );

    void CrackPSXEvent( CPSX *pPSX, PPSXEVENT pEvent );

    ///<summary>This method adds an event handler to the chain to allow other
    ///objects to share the PSX instance created by SCOTAPP. </summary>
    ///<param name="pEventHandler">Pointer to the event handler being registered</param>
    static void Register(PPSXEVENTHANDLER pEventHandler);

    ///<summary>This method removes an event handler from the chain to prevent it
    ///from receiving events. </summary>
    ///<param name="pEventHandler">Pointer to the event handler being removed</param>
    static void Unregister(PPSXEVENTHANDLER pEventHandler);

protected:

    void LocalEventHandler( CPSX *pPSX
                          , PPSXEVENT pEvent );

private:

    void static SetupEntry( SMappingEntry_ *pMappingEntry
                      , const COleVariant& vParam );

    bool static TabSelected( SMappingEntry_ *pMappingEntry
                           , PPSXEVENT pEvent );


    bool static ProductImage( SMappingEntry_ *pMappingEntry
                           , PPSXEVENT pEvent );


    bool static TenderImage( SMappingEntry_ *pMappingEntry
                           , PPSXEVENT pEvent );


    bool static LanguageImage( SMappingEntry_ *pMappingEntry
                           , PPSXEVENT pEvent );


    bool static TareList( SMappingEntry_ *pMappingEntry
                           , PPSXEVENT pEvent );


    bool static CashBackList( SMappingEntry_ *pMappingEntry
                           , PPSXEVENT pEvent );


    bool static EBTCashBackList( SMappingEntry_ *pMappingEntry
                           , PPSXEVENT pEvent );

    void InitializeMapping();
    
    SMappingEntry *FindEntry(const CString& strControlName);

private:

    ///<summary>No-arg constructor for this class.  This constructor is only called 
    ///by the template base class for creation of the managed instance of this class.</summary>
    CPSXInterface();

    // hide copy and assignment
	CPSXInterface(const CPSXInterface&);
	CPSXInterface& operator = (const CPSXInterface &);

    CEventHandlers m_vHandlers;

    bool m_bInitialized;

    CMapping m_mapping;

	static SMappingEntry m_mappingEntries[];

#if !defined(_SCOT) && !defined(_SCOTSSF_)

public:

    void SetControl(CRemoteConsoleMgrWrapper *pControl);
    CRemoteConsoleMgrWrapper &control();

	class SignonParms
		  : public CKeyValueHandler<SignonParms>
	{
		  DECLARE_KV_HANDLER_MAP();

		  DECLARE_KV_HANDLER(UserId);
		  DECLARE_KV_HANDLER(UserPwd);

	public:
		  void TraceThis(const _TCHAR *pPairs) throw();
		  CString m_csUserId;
		  CString m_csUserPwd;

	};

protected:
	
    virtual bool DisconnectFromRemote( PPSXEVENT pEvent );
	virtual bool ConnectToRemote( PPSXEVENT pEvent );
	virtual bool ForwardInput( PPSXEVENT pEvent );
	virtual bool SignOn( PPSXEVENT pEvent );
	virtual bool SignOff( PPSXEVENT pEvent );

private:

    CRemoteConsoleMgrWrapper *m_pControl;

    CConnections m_connections;

#endif
};
