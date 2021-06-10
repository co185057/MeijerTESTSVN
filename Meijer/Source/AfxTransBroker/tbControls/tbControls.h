
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0347 */
/* at Wed May 14 09:37:16 2003
 */
/* Compiler settings for _tbControls.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __tbControls_h__
#define __tbControls_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IItemObj_FWD_DEFINED__
#define __IItemObj_FWD_DEFINED__
typedef interface IItemObj IItemObj;
#endif 	/* __IItemObj_FWD_DEFINED__ */


#ifndef __IScannerMsg_FWD_DEFINED__
#define __IScannerMsg_FWD_DEFINED__
typedef interface IScannerMsg IScannerMsg;
#endif 	/* __IScannerMsg_FWD_DEFINED__ */


#ifndef __ItbConnect_FWD_DEFINED__
#define __ItbConnect_FWD_DEFINED__
typedef interface ItbConnect ItbConnect;
#endif 	/* __ItbConnect_FWD_DEFINED__ */


#ifndef ___IItemObjEvents_FWD_DEFINED__
#define ___IItemObjEvents_FWD_DEFINED__
typedef interface _IItemObjEvents _IItemObjEvents;
#endif 	/* ___IItemObjEvents_FWD_DEFINED__ */


#ifndef __CItemObj_FWD_DEFINED__
#define __CItemObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class CItemObj CItemObj;
#else
typedef struct CItemObj CItemObj;
#endif /* __cplusplus */

#endif 	/* __CItemObj_FWD_DEFINED__ */


#ifndef ___IScannerMsgEvents_FWD_DEFINED__
#define ___IScannerMsgEvents_FWD_DEFINED__
typedef interface _IScannerMsgEvents _IScannerMsgEvents;
#endif 	/* ___IScannerMsgEvents_FWD_DEFINED__ */


#ifndef __CScannerMsg_FWD_DEFINED__
#define __CScannerMsg_FWD_DEFINED__

#ifdef __cplusplus
typedef class CScannerMsg CScannerMsg;
#else
typedef struct CScannerMsg CScannerMsg;
#endif /* __cplusplus */

#endif 	/* __CScannerMsg_FWD_DEFINED__ */


#ifndef ___ItbConnectEvents_FWD_DEFINED__
#define ___ItbConnectEvents_FWD_DEFINED__
typedef interface _ItbConnectEvents _ItbConnectEvents;
#endif 	/* ___ItbConnectEvents_FWD_DEFINED__ */


#ifndef __CtbConnect_FWD_DEFINED__
#define __CtbConnect_FWD_DEFINED__

#ifdef __cplusplus
typedef class CtbConnect CtbConnect;
#else
typedef struct CtbConnect CtbConnect;
#endif /* __cplusplus */

#endif 	/* __CtbConnect_FWD_DEFINED__ */


/* header files for imported files */
#include "prsht.h"
#include "mshtml.h"
#include "mshtmhst.h"
#include "exdisp.h"
#include "objsafe.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IItemObj_INTERFACE_DEFINED__
#define __IItemObj_INTERFACE_DEFINED__

/* interface IItemObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IItemObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2DFDB3BF-5971-4251-AE7E-4173468732B0")
    IItemObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsCoupon( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IsCoupon( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsRestricted( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IsRestricted( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsVoided( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IsVoided( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LinkedItem( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LinkedItem( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NeedPrice( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NeedPrice( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NeedQuantity( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NeedQuantity( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NeedWeight( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NeedWeight( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NotForSale( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NotForSale( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PIMItem( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PIMItem( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SecurityTag( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SecurityTag( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UnknownItem( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_UnknownItem( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_VisualVerify( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_VisualVerify( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrorDescription( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ErrorDescription( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ExtendedPrice( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ExtendedPrice( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [displaybind][defaultbind][bindable][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ItemCode( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [displaybind][defaultbind][bindable][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ItemCode( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ItemDescription( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ItemDescription( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Quantity( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Quantity( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UnitPrice( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_UnitPrice( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Weight( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Weight( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Department( 
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Department( 
            /* [in] */ ULONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_EntryID( 
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_EntryID( 
            /* [in] */ ULONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RestrictedAge( 
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RestrictedAge( 
            /* [in] */ ULONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TareCode( 
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TareCode( 
            /* [in] */ ULONG newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IItemObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IItemObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IItemObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IItemObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IItemObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IItemObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IItemObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IItemObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsCoupon )( 
            IItemObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsCoupon )( 
            IItemObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsRestricted )( 
            IItemObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsRestricted )( 
            IItemObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsVoided )( 
            IItemObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsVoided )( 
            IItemObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LinkedItem )( 
            IItemObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LinkedItem )( 
            IItemObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NeedPrice )( 
            IItemObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_NeedPrice )( 
            IItemObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NeedQuantity )( 
            IItemObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_NeedQuantity )( 
            IItemObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NeedWeight )( 
            IItemObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_NeedWeight )( 
            IItemObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NotForSale )( 
            IItemObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_NotForSale )( 
            IItemObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PIMItem )( 
            IItemObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PIMItem )( 
            IItemObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SecurityTag )( 
            IItemObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SecurityTag )( 
            IItemObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_UnknownItem )( 
            IItemObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_UnknownItem )( 
            IItemObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_VisualVerify )( 
            IItemObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_VisualVerify )( 
            IItemObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrorDescription )( 
            IItemObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ErrorDescription )( 
            IItemObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ExtendedPrice )( 
            IItemObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ExtendedPrice )( 
            IItemObj * This,
            /* [in] */ BSTR newVal);
        
        /* [displaybind][defaultbind][bindable][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ItemCode )( 
            IItemObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [displaybind][defaultbind][bindable][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ItemCode )( 
            IItemObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ItemDescription )( 
            IItemObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ItemDescription )( 
            IItemObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Quantity )( 
            IItemObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Quantity )( 
            IItemObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_UnitPrice )( 
            IItemObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_UnitPrice )( 
            IItemObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Weight )( 
            IItemObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Weight )( 
            IItemObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Department )( 
            IItemObj * This,
            /* [retval][out] */ ULONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Department )( 
            IItemObj * This,
            /* [in] */ ULONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_EntryID )( 
            IItemObj * This,
            /* [retval][out] */ ULONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_EntryID )( 
            IItemObj * This,
            /* [in] */ ULONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RestrictedAge )( 
            IItemObj * This,
            /* [retval][out] */ ULONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RestrictedAge )( 
            IItemObj * This,
            /* [in] */ ULONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TareCode )( 
            IItemObj * This,
            /* [retval][out] */ ULONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TareCode )( 
            IItemObj * This,
            /* [in] */ ULONG newVal);
        
        END_INTERFACE
    } IItemObjVtbl;

    interface IItemObj
    {
        CONST_VTBL struct IItemObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IItemObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IItemObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IItemObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IItemObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IItemObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IItemObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IItemObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IItemObj_get_IsCoupon(This,pVal)	\
    (This)->lpVtbl -> get_IsCoupon(This,pVal)

#define IItemObj_put_IsCoupon(This,newVal)	\
    (This)->lpVtbl -> put_IsCoupon(This,newVal)

#define IItemObj_get_IsRestricted(This,pVal)	\
    (This)->lpVtbl -> get_IsRestricted(This,pVal)

#define IItemObj_put_IsRestricted(This,newVal)	\
    (This)->lpVtbl -> put_IsRestricted(This,newVal)

#define IItemObj_get_IsVoided(This,pVal)	\
    (This)->lpVtbl -> get_IsVoided(This,pVal)

#define IItemObj_put_IsVoided(This,newVal)	\
    (This)->lpVtbl -> put_IsVoided(This,newVal)

#define IItemObj_get_LinkedItem(This,pVal)	\
    (This)->lpVtbl -> get_LinkedItem(This,pVal)

#define IItemObj_put_LinkedItem(This,newVal)	\
    (This)->lpVtbl -> put_LinkedItem(This,newVal)

#define IItemObj_get_NeedPrice(This,pVal)	\
    (This)->lpVtbl -> get_NeedPrice(This,pVal)

#define IItemObj_put_NeedPrice(This,newVal)	\
    (This)->lpVtbl -> put_NeedPrice(This,newVal)

#define IItemObj_get_NeedQuantity(This,pVal)	\
    (This)->lpVtbl -> get_NeedQuantity(This,pVal)

#define IItemObj_put_NeedQuantity(This,newVal)	\
    (This)->lpVtbl -> put_NeedQuantity(This,newVal)

#define IItemObj_get_NeedWeight(This,pVal)	\
    (This)->lpVtbl -> get_NeedWeight(This,pVal)

#define IItemObj_put_NeedWeight(This,newVal)	\
    (This)->lpVtbl -> put_NeedWeight(This,newVal)

#define IItemObj_get_NotForSale(This,pVal)	\
    (This)->lpVtbl -> get_NotForSale(This,pVal)

#define IItemObj_put_NotForSale(This,newVal)	\
    (This)->lpVtbl -> put_NotForSale(This,newVal)

#define IItemObj_get_PIMItem(This,pVal)	\
    (This)->lpVtbl -> get_PIMItem(This,pVal)

#define IItemObj_put_PIMItem(This,newVal)	\
    (This)->lpVtbl -> put_PIMItem(This,newVal)

#define IItemObj_get_SecurityTag(This,pVal)	\
    (This)->lpVtbl -> get_SecurityTag(This,pVal)

#define IItemObj_put_SecurityTag(This,newVal)	\
    (This)->lpVtbl -> put_SecurityTag(This,newVal)

#define IItemObj_get_UnknownItem(This,pVal)	\
    (This)->lpVtbl -> get_UnknownItem(This,pVal)

#define IItemObj_put_UnknownItem(This,newVal)	\
    (This)->lpVtbl -> put_UnknownItem(This,newVal)

#define IItemObj_get_VisualVerify(This,pVal)	\
    (This)->lpVtbl -> get_VisualVerify(This,pVal)

#define IItemObj_put_VisualVerify(This,newVal)	\
    (This)->lpVtbl -> put_VisualVerify(This,newVal)

#define IItemObj_get_ErrorDescription(This,pVal)	\
    (This)->lpVtbl -> get_ErrorDescription(This,pVal)

#define IItemObj_put_ErrorDescription(This,newVal)	\
    (This)->lpVtbl -> put_ErrorDescription(This,newVal)

#define IItemObj_get_ExtendedPrice(This,pVal)	\
    (This)->lpVtbl -> get_ExtendedPrice(This,pVal)

#define IItemObj_put_ExtendedPrice(This,newVal)	\
    (This)->lpVtbl -> put_ExtendedPrice(This,newVal)

#define IItemObj_get_ItemCode(This,pVal)	\
    (This)->lpVtbl -> get_ItemCode(This,pVal)

#define IItemObj_put_ItemCode(This,newVal)	\
    (This)->lpVtbl -> put_ItemCode(This,newVal)

#define IItemObj_get_ItemDescription(This,pVal)	\
    (This)->lpVtbl -> get_ItemDescription(This,pVal)

#define IItemObj_put_ItemDescription(This,newVal)	\
    (This)->lpVtbl -> put_ItemDescription(This,newVal)

#define IItemObj_get_Quantity(This,pVal)	\
    (This)->lpVtbl -> get_Quantity(This,pVal)

#define IItemObj_put_Quantity(This,newVal)	\
    (This)->lpVtbl -> put_Quantity(This,newVal)

#define IItemObj_get_UnitPrice(This,pVal)	\
    (This)->lpVtbl -> get_UnitPrice(This,pVal)

#define IItemObj_put_UnitPrice(This,newVal)	\
    (This)->lpVtbl -> put_UnitPrice(This,newVal)

#define IItemObj_get_Weight(This,pVal)	\
    (This)->lpVtbl -> get_Weight(This,pVal)

#define IItemObj_put_Weight(This,newVal)	\
    (This)->lpVtbl -> put_Weight(This,newVal)

#define IItemObj_get_Department(This,pVal)	\
    (This)->lpVtbl -> get_Department(This,pVal)

#define IItemObj_put_Department(This,newVal)	\
    (This)->lpVtbl -> put_Department(This,newVal)

#define IItemObj_get_EntryID(This,pVal)	\
    (This)->lpVtbl -> get_EntryID(This,pVal)

#define IItemObj_put_EntryID(This,newVal)	\
    (This)->lpVtbl -> put_EntryID(This,newVal)

#define IItemObj_get_RestrictedAge(This,pVal)	\
    (This)->lpVtbl -> get_RestrictedAge(This,pVal)

#define IItemObj_put_RestrictedAge(This,newVal)	\
    (This)->lpVtbl -> put_RestrictedAge(This,newVal)

#define IItemObj_get_TareCode(This,pVal)	\
    (This)->lpVtbl -> get_TareCode(This,pVal)

#define IItemObj_put_TareCode(This,newVal)	\
    (This)->lpVtbl -> put_TareCode(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_IsCoupon_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IItemObj_get_IsCoupon_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_IsCoupon_Proxy( 
    IItemObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IItemObj_put_IsCoupon_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_IsRestricted_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IItemObj_get_IsRestricted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_IsRestricted_Proxy( 
    IItemObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IItemObj_put_IsRestricted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_IsVoided_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IItemObj_get_IsVoided_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_IsVoided_Proxy( 
    IItemObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IItemObj_put_IsVoided_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_LinkedItem_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IItemObj_get_LinkedItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_LinkedItem_Proxy( 
    IItemObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IItemObj_put_LinkedItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_NeedPrice_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IItemObj_get_NeedPrice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_NeedPrice_Proxy( 
    IItemObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IItemObj_put_NeedPrice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_NeedQuantity_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IItemObj_get_NeedQuantity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_NeedQuantity_Proxy( 
    IItemObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IItemObj_put_NeedQuantity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_NeedWeight_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IItemObj_get_NeedWeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_NeedWeight_Proxy( 
    IItemObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IItemObj_put_NeedWeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_NotForSale_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IItemObj_get_NotForSale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_NotForSale_Proxy( 
    IItemObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IItemObj_put_NotForSale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_PIMItem_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IItemObj_get_PIMItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_PIMItem_Proxy( 
    IItemObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IItemObj_put_PIMItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_SecurityTag_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IItemObj_get_SecurityTag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_SecurityTag_Proxy( 
    IItemObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IItemObj_put_SecurityTag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_UnknownItem_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IItemObj_get_UnknownItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_UnknownItem_Proxy( 
    IItemObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IItemObj_put_UnknownItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_VisualVerify_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IItemObj_get_VisualVerify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_VisualVerify_Proxy( 
    IItemObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IItemObj_put_VisualVerify_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_ErrorDescription_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IItemObj_get_ErrorDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_ErrorDescription_Proxy( 
    IItemObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IItemObj_put_ErrorDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_ExtendedPrice_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IItemObj_get_ExtendedPrice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_ExtendedPrice_Proxy( 
    IItemObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IItemObj_put_ExtendedPrice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [displaybind][defaultbind][bindable][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_ItemCode_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IItemObj_get_ItemCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [displaybind][defaultbind][bindable][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_ItemCode_Proxy( 
    IItemObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IItemObj_put_ItemCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_ItemDescription_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IItemObj_get_ItemDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_ItemDescription_Proxy( 
    IItemObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IItemObj_put_ItemDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_Quantity_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IItemObj_get_Quantity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_Quantity_Proxy( 
    IItemObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IItemObj_put_Quantity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_UnitPrice_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IItemObj_get_UnitPrice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_UnitPrice_Proxy( 
    IItemObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IItemObj_put_UnitPrice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_Weight_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IItemObj_get_Weight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_Weight_Proxy( 
    IItemObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IItemObj_put_Weight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_Department_Proxy( 
    IItemObj * This,
    /* [retval][out] */ ULONG *pVal);


void __RPC_STUB IItemObj_get_Department_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_Department_Proxy( 
    IItemObj * This,
    /* [in] */ ULONG newVal);


void __RPC_STUB IItemObj_put_Department_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_EntryID_Proxy( 
    IItemObj * This,
    /* [retval][out] */ ULONG *pVal);


void __RPC_STUB IItemObj_get_EntryID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_EntryID_Proxy( 
    IItemObj * This,
    /* [in] */ ULONG newVal);


void __RPC_STUB IItemObj_put_EntryID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_RestrictedAge_Proxy( 
    IItemObj * This,
    /* [retval][out] */ ULONG *pVal);


void __RPC_STUB IItemObj_get_RestrictedAge_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_RestrictedAge_Proxy( 
    IItemObj * This,
    /* [in] */ ULONG newVal);


void __RPC_STUB IItemObj_put_RestrictedAge_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_TareCode_Proxy( 
    IItemObj * This,
    /* [retval][out] */ ULONG *pVal);


void __RPC_STUB IItemObj_get_TareCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_TareCode_Proxy( 
    IItemObj * This,
    /* [in] */ ULONG newVal);


void __RPC_STUB IItemObj_put_TareCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IItemObj_INTERFACE_DEFINED__ */


#ifndef __IScannerMsg_INTERFACE_DEFINED__
#define __IScannerMsg_INTERFACE_DEFINED__

/* interface IScannerMsg */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IScannerMsg;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5616F764-2D3E-4BE1-9679-4CF0E33350B9")
    IScannerMsg : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IScannerMsgVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IScannerMsg * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IScannerMsg * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IScannerMsg * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IScannerMsg * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IScannerMsg * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IScannerMsg * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IScannerMsg * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IScannerMsgVtbl;

    interface IScannerMsg
    {
        CONST_VTBL struct IScannerMsgVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IScannerMsg_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IScannerMsg_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IScannerMsg_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IScannerMsg_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IScannerMsg_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IScannerMsg_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IScannerMsg_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IScannerMsg_INTERFACE_DEFINED__ */


#ifndef __ItbConnect_INTERFACE_DEFINED__
#define __ItbConnect_INTERFACE_DEFINED__

/* interface ItbConnect */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ItbConnect;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A4D47620-0FA3-4C43-B3F8-0F726CE22E51")
    ItbConnect : public IDispatch
    {
    public:
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE Connect( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ItbConnectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ItbConnect * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ItbConnect * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ItbConnect * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ItbConnect * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ItbConnect * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ItbConnect * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ItbConnect * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ItbConnect * This);
        
        END_INTERFACE
    } ItbConnectVtbl;

    interface ItbConnect
    {
        CONST_VTBL struct ItbConnectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ItbConnect_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ItbConnect_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ItbConnect_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ItbConnect_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ItbConnect_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ItbConnect_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ItbConnect_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ItbConnect_Connect(This)	\
    (This)->lpVtbl -> Connect(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE ItbConnect_Connect_Proxy( 
    ItbConnect * This);


void __RPC_STUB ItbConnect_Connect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ItbConnect_INTERFACE_DEFINED__ */



#ifndef __tbControls_LIBRARY_DEFINED__
#define __tbControls_LIBRARY_DEFINED__

/* library tbControls */
/* [helpstring][uuid][version] */ 


EXTERN_C const IID LIBID_tbControls;

#ifndef ___IItemObjEvents_DISPINTERFACE_DEFINED__
#define ___IItemObjEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IItemObjEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IItemObjEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("791461A8-4B4B-46B2-8838-2768FF0FD612")
    _IItemObjEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IItemObjEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IItemObjEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IItemObjEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IItemObjEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IItemObjEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IItemObjEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IItemObjEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IItemObjEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IItemObjEventsVtbl;

    interface _IItemObjEvents
    {
        CONST_VTBL struct _IItemObjEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IItemObjEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IItemObjEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IItemObjEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IItemObjEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IItemObjEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IItemObjEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IItemObjEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IItemObjEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CItemObj;

#ifdef __cplusplus

class DECLSPEC_UUID("C8A5701C-082A-4E05-A4B2-E53E08BC0EFB")
CItemObj;
#endif

#ifndef ___IScannerMsgEvents_DISPINTERFACE_DEFINED__
#define ___IScannerMsgEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IScannerMsgEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IScannerMsgEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("0A8C70B7-DB07-4DFE-820A-CA26A140174F")
    _IScannerMsgEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IScannerMsgEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IScannerMsgEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IScannerMsgEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IScannerMsgEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IScannerMsgEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IScannerMsgEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IScannerMsgEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IScannerMsgEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IScannerMsgEventsVtbl;

    interface _IScannerMsgEvents
    {
        CONST_VTBL struct _IScannerMsgEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IScannerMsgEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IScannerMsgEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IScannerMsgEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IScannerMsgEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IScannerMsgEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IScannerMsgEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IScannerMsgEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IScannerMsgEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CScannerMsg;

#ifdef __cplusplus

class DECLSPEC_UUID("3BF3D131-50F2-48B9-951B-CFF1E4643B49")
CScannerMsg;
#endif

#ifndef ___ItbConnectEvents_DISPINTERFACE_DEFINED__
#define ___ItbConnectEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ItbConnectEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__ItbConnectEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("F82352BA-F302-41C9-99FF-9E0930C2A3BC")
    _ItbConnectEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ItbConnectEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ItbConnectEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ItbConnectEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ItbConnectEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ItbConnectEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ItbConnectEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ItbConnectEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ItbConnectEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _ItbConnectEventsVtbl;

    interface _ItbConnectEvents
    {
        CONST_VTBL struct _ItbConnectEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ItbConnectEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _ItbConnectEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _ItbConnectEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _ItbConnectEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _ItbConnectEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _ItbConnectEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _ItbConnectEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ItbConnectEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CtbConnect;

#ifdef __cplusplus

class DECLSPEC_UUID("FEA44A8A-E5BC-4BB5-A2AC-34C7A0B29703")
CtbConnect;
#endif
#endif /* __tbControls_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


