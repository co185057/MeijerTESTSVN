//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    MobileCartAudit.h
//
// TITLE:   Mobile cart audit session object
//
// AUTHOR:  TDonnan
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// MobileCartAudit
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CMobileCartAudit
#define _CMobileCartAudit

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include "comutil.h"

// #include "MobileCartAuditBase.h"

#include "QueuedActionsContainer.h"
#include "QueuedActionRescanItem.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

CMobileCartAudit  // : public CMobileCartAuditBase
{
public:

    CMobileCartAudit();

    ~CMobileCartAudit();

    virtual BOOL Initialize( const _bstr_t & a_bstrtTicketDocumentXml );

    virtual void UnInitialize();

public:

    virtual BOOL SendStartAuditRequest();

    virtual BOOL SendExitAuditRequest();

    virtual BOOL ProcessStartAuditResponse( const _bstr_t & a_bstrtXml );

    virtual BOOL SendAddAuditItemRequest( int a_nQuantity, const _bstr_t & a_bstrtBarcode );

    virtual BOOL SendAddAuditItemRequest( const _bstr_t & a_bstrtScanInfo );
        
    virtual BOOL ProcessAddAuditItemResponse( const _bstr_t & a_bstrtXml );

    virtual BOOL SendEndAuditRequest( const _bstr_t & a_bstrtAction );

    virtual BOOL ProcessEndAuditResponse( const _bstr_t & a_bstrtXml );

    virtual BOOL SendSwitchToFullAuditRequest( const _bstr_t & a_bstrtOnErrorBool );
        
    virtual BOOL ProcessSwitchToFullAuditResponse( const _bstr_t & a_bstrtXml );

    virtual BOOL SendContinueAfterErrorRequest( const _bstr_t & a_bstrtBarcode );

    virtual BOOL SendContinueAfterFullRescanError( void );
        
    virtual BOOL ProcessContinueAfterErrorResponse( const _bstr_t & a_bstrtXml );

    virtual BOOL RequestAbortAudit();

    virtual BOOL ReturnToPartialAuditScreen();

    virtual void Reset();

    virtual _bstr_t TraceObjectState(void);

    virtual void QueueFailedAuditItem();

public:

    _bstr_t StoreNumber()               const   { return m_bstrtStoreNumber;            }
    _bstr_t TerminalNumber()            const   { return m_bstrtTerminalNumber;         }
    _bstr_t LanguageId()                const   { return m_bstrtLanguageId;             }
    _bstr_t CartId()                    const   { return m_bstrtCartId;                 }
    _bstr_t LoyaltyId()                 const   { return m_bstrtLoyaltyId;              }
    _bstr_t SessionId()                 const   { return m_bstrtSessionId;              }

    _bstr_t GetMaskedLoyaltyId()        const;
                                        
    _bstr_t Result()                    const   { return m_bstrtResult;                 }
    _bstr_t ResultMessage()             const   { return m_bstrtResultMessage;          }
    _bstr_t RescanType()                const   { return m_bstrtRescanType;             }
    _bstr_t RescanStatus()              const   { return m_bstrtRescanStatus;           }
    _bstr_t CustomerName()              const   { return m_bstrtCustomerName;           }
    _bstr_t NumberOfVisits()            const   { return m_bstrtNumberOfVisits;         }
    _bstr_t FirstVisit()                const   { return m_bstrtFirstVisit;             }
    _bstr_t CardNumber()                const   { return m_bstrtCardNumber;             }
                                        
    _bstr_t GetMaskedCardNumber()       const;

    BOOL    GetFirstVisit()             const;
    LONG    GetNumberOfVisits()         const;

    _bstr_t NumberOfItemsInTicket()     const   { return m_bstrtNumberOfItemsInTicket;  }
    _bstr_t TotalTicketAmount()         const   { return m_bstrtTotalTicketAmount;      }
    _bstr_t NumItemsToRescan()          const   { return m_bstrtNumItemsToRescan;       }
    _bstr_t AmountToRescan()            const   { return m_bstrtAmountToRescan;         }

    LONG    GetTotalTicketAmount()      const ;
    LONG    GetAmountToRescan()         const ;
                                        
    _bstr_t RescanSkipAllowed()         const   { return m_bstrtRescanSkipAllowed;      }
    _bstr_t RescanAbortAllowed()        const   { return m_bstrtRescanAbortAllowed;     }

    _bstr_t ItemId()                    const   { return m_bstrtItemId;                 }
    _bstr_t ItemQuantity()              const   { return m_bstrtItemQuantity;           }
    _bstr_t ItemBarcode()               const   { return m_bstrtItemBarcode;            }
    _bstr_t ItemDescription()           const   { return m_bstrtItemDescription;        }
    _bstr_t ItemPrice()                 const   { return m_bstrtItemPrice;              }
    _bstr_t RemainingItems()            const   { return m_bstrtRemainingItems;         }
    _bstr_t RemainingAmount()           const   { return m_bstrtRemainingAmount;        }

    LONG    GetItemPrice()              const;
    LONG    GetRemainingAmount()        const;

    _bstr_t NumItemsScanned()           const { return m_bstrtNumItemsScanned;          }
    _bstr_t ScannedItemsAmount()        const { return m_bstrtScannedItemsAmount;       }

    LONG    GetScannedItemsAmount()     const;

    _bstr_t DataNeededSection()         const { return m_bstrDataNeededSection;         }
    _bstr_t DataNeededViewString()      const { return m_bstrDataNeededViewString;      }

    _bstr_t MaskValue( const _bstr_t & a_bstrValue ) const;

    CQueuedActionsContainer* m_MobileAuditItemsQueue;  //queue of Mobile items for the transaction
    IQueuedAction* GetNextAuditFailureItem();
    int GetNumberOfAuditFailureItems();
    void ClearAuditFailureItems();

protected:

    void SetDataNeededInfo( _bstr_t a_bstrtRequestName, _bstr_t a_bstrtResponse, _bstr_t a_bstrtResult);

    LONG BstrMoneyToLong( const _bstr_t & a_bstrValue ) const;

protected:

    _bstr_t m_bstrtStoreNumber;
    _bstr_t m_bstrtTerminalNumber;
    _bstr_t m_bstrtLanguageId;
    _bstr_t m_bstrtCartId;     // Header/TransactionNumber
    _bstr_t m_bstrtLoyaltyId;  // Header/CardNumber
    _bstr_t m_bstrtSessionId;

    _bstr_t m_bstrtResult;
    _bstr_t m_bstrtResultMessage;
    _bstr_t m_bstrtRescanType;
    _bstr_t m_bstrtRescanStatus;
    //
    // ticket info
    //
    _bstr_t m_bstrtCustomerName;
    _bstr_t m_bstrtNumberOfVisits; 
    _bstr_t m_bstrtFirstVisit;     
    _bstr_t m_bstrtCardNumber;     

    _bstr_t m_bstrtNumberOfItemsInTicket;
    _bstr_t m_bstrtTotalTicketAmount;
    _bstr_t m_bstrtNumItemsToRescan;
    _bstr_t m_bstrtAmountToRescan;
    _bstr_t m_bstrtRescanSkipAllowed;
    _bstr_t m_bstrtRescanAbortAllowed;
    //
    // scan item info
    //
    _bstr_t m_bstrtItemId;
    _bstr_t m_bstrtItemQuantity;
    _bstr_t m_bstrtItemBarcode;
    _bstr_t m_bstrtItemDescription;
    _bstr_t m_bstrtItemPrice;
    _bstr_t m_bstrtRemainingItems;
    _bstr_t m_bstrtRemainingAmount;
    _bstr_t m_bstrtNumItemsScanned;        
    _bstr_t m_bstrtScannedItemsAmount; //
    // data needed info
    //
    _bstr_t m_bstrDataNeededSection;
    _bstr_t m_bstrDataNeededViewString;

    _bstr_t m_bstrLastScanInfo;
    long m_bstrLastBarcodeType;

    BOOL m_bUnknownItem;

    //DECLARE_DYNCREATE(CMobileCartAudit)// MFC Runtime class/object information
};
#endif

