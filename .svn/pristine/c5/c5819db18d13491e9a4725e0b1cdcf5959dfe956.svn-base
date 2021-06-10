#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/SCOTAPP/PSXInterface.h 5     1/05/10 8:39p Cu185008 $
/*
 * @(#)PSXInterface.h    $Revision: 5 $ $Date: 1/05/10 8:39p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/SCOTAPP/PSXInterface.h $
 * 
 * 5     1/05/10 8:39p Cu185008
 * Migrating Core to G2.
 * 
 * 4     11/14/08 2:35p Sa250050
 * TAR387302
 * 
 * 3     10/30/08 4:17p Sm133076
 * Patch G Migration
 *
 * 10    7/10/09 6:04a pl250020
 * Fix for the P3 TAR 409017 - G2_374: Can log on to Store Mode
 * successfully even with the invalid ID/PW.
 * 
 * 9     7/10/09 5:52a pl250020
 * Fix for the P3 TAR 409017 - G2_374: Can log on to Store Mode
 * successfully even with the invalid ID/PW.
 * 
 * 8     5/29/08 1:35p Hh185033
 * Added the search capability on the picklist
 * 
 * 8    5/15/08 1:51p hh185033
 * Implementing the search capacity for the picklist
 *
 * 7     2/29/08 8:58a Ah150000
 * Hierarchical picklist
 * 
 * 6     11/27/07 5:33p Lp185019
 * SR629 (TAR 334800) - Resolve CISP Issue: RCM is tracing password by
 * default
 * 
 * 5     4/12/06 3:52p Wr185001
 * TAR 320367 - Add an interface so that SSF functions can 
 * hook into Psx Event handling.  Also Add a way that Psx Event button
 * clicks can be mapped via an API, rather than modifying the static array
 * inside PsxInterface all the time.
 * 
 * 4     1/30/06 4:11p Jv185012
 * TAR 316675:  4.2 E Build 211(Restricted Shell&NonAdmin) : XP PRO -
 * Reports do not work.  -->  Merged changes for TAR 304421 from 4.0D into
 * 4.0E.
 * 
 * 1     1/26/05 2:08p Bd123405
 * Initial check-in of 4.0 Patch B
* 
* 7     11/16/04 2:23p Dm185016
* Added launchpad connection
* 
* 6     11/15/04 3:20p Db185023
* Merge new RAP into 3.2.
* 
* 5     11/11/04 10:10a Dm185016
* Moved "RAP" prefix to header file
* 
* 4     10/26/04 1:46p Dm185016
* Added code to support use in both SCOTAPP and tester.
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
#include "psevt.h"

#if !defined(_SCOT) && !defined(_SCOTSSF_)
#include "BaseObject.h"
#define OTHER_BASE_CLASSES    , public CBaseObject
#else
#define OTHER_BASE_CLASSES 
#endif

static const _TCHAR LAUNCHPAD_PREFIX[]			= _T("LaunchPad");
static const int LAUNCHPAD_PREFIX_LENGTH		= sizeof(LAUNCHPAD_PREFIX)-1;

static const _TCHAR REMOTE_CONNECTION_PREFIX[]			= _T("RAP");
static const int REMOTE_CONNECTION_PREFIX_LENGTH		= sizeof(REMOTE_CONNECTION_PREFIX)-1;

static const TCHAR cMASKED[] = _T("[masked]");
static const TCHAR cASTERISK[] = _T("****");
static const TCHAR cEQUAL = _T('=');
static const TCHAR cSEMICOLON = _T(';');
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
    long nEvent;
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

#define VK_ALTGR (0x38|0x80)

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

	static void AddExternalPsxEventMappingEntry( LPCTSTR KeyName, 
													PSEVT evtType,
													long  wParam,
													bool  bUseWParam,
													long  lParam, 
													bool  bUseLParam,
													PMAPPINGFXN pMappingFxn );

	static bool RemoveExternalPsxEventMappingEntry( LPCTSTR KeyName );

    // SASA: Assist Key buffering prevention
    inline void SetAssistKeyQueued(bool bQueued) { m_bAssistKeyQueued = bQueued; }
    inline bool GetAssistKeyQueued(void) { return m_bAssistKeyQueued; }

protected:

    void LocalEventHandler( CPSX *pPSX, PPSXEVENT pEvent );
	void StripAndAddOperator(CString temp, CString strRemoteConnectionName);	// TAR 409017
	// SASA: Assist Key buffering prevention
    bool m_bAssistKeyQueued;
private:

    void static SetupEntry( SMappingEntry_ *pMappingEntry
                      , const COleVariant& vParam );

    bool static TabSelected( SMappingEntry_ *pMappingEntry
                           , PPSXEVENT pEvent );


    bool static ProductImage( SMappingEntry_ *pMappingEntry
                           , PPSXEVENT pEvent );

	bool static QuickPickImage( SMappingEntry_ *pMappingEntry
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

    bool static HotKey( SMappingEntry_ *pMappingEntry
                           , PPSXEVENT pEvent );

    bool static CharKey( SMappingEntry_ *pMappingEntry
                           , PPSXEVENT pEvent );

    void InitializeMapping();
    
    SMappingEntry *FindEntry(const CString& strControlName);
    SMappingEntry *FindExternalEntry(const CString& strControlName);

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
	
	static CMapping m_externalMapping;

	static CCriticalSection m_lockMap;

	static SMappingEntry m_mappingEntries[];
	CString MaskedParam(const CString& csParam);
};
