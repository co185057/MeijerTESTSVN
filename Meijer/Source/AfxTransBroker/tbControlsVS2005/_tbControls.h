

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Wed Nov 15 13:54:01 2006
 */
/* Compiler settings for _tbControls.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
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

#ifndef ___tbControls_h__
#define ___tbControls_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITBStream_FWD_DEFINED__
#define __ITBStream_FWD_DEFINED__
typedef interface ITBStream ITBStream;
#endif 	/* __ITBStream_FWD_DEFINED__ */


#ifndef __IItemObj_FWD_DEFINED__
#define __IItemObj_FWD_DEFINED__
typedef interface IItemObj IItemObj;
#endif 	/* __IItemObj_FWD_DEFINED__ */


#ifndef __ICashDrawerObj_FWD_DEFINED__
#define __ICashDrawerObj_FWD_DEFINED__
typedef interface ICashDrawerObj ICashDrawerObj;
#endif 	/* __ICashDrawerObj_FWD_DEFINED__ */


#ifndef __ITotalObj_FWD_DEFINED__
#define __ITotalObj_FWD_DEFINED__
typedef interface ITotalObj ITotalObj;
#endif 	/* __ITotalObj_FWD_DEFINED__ */


#ifndef __ITenderObj_FWD_DEFINED__
#define __ITenderObj_FWD_DEFINED__
typedef interface ITenderObj ITenderObj;
#endif 	/* __ITenderObj_FWD_DEFINED__ */


#ifndef __IRewardObj_FWD_DEFINED__
#define __IRewardObj_FWD_DEFINED__
typedef interface IRewardObj IRewardObj;
#endif 	/* __IRewardObj_FWD_DEFINED__ */


#ifndef __IScannerObj_FWD_DEFINED__
#define __IScannerObj_FWD_DEFINED__
typedef interface IScannerObj IScannerObj;
#endif 	/* __IScannerObj_FWD_DEFINED__ */


#ifndef __IKeylockObj_FWD_DEFINED__
#define __IKeylockObj_FWD_DEFINED__
typedef interface IKeylockObj IKeylockObj;
#endif 	/* __IKeylockObj_FWD_DEFINED__ */


#ifndef __IKeyboardObj_FWD_DEFINED__
#define __IKeyboardObj_FWD_DEFINED__
typedef interface IKeyboardObj IKeyboardObj;
#endif 	/* __IKeyboardObj_FWD_DEFINED__ */


#ifndef __IMSRObj_FWD_DEFINED__
#define __IMSRObj_FWD_DEFINED__
typedef interface IMSRObj IMSRObj;
#endif 	/* __IMSRObj_FWD_DEFINED__ */


#ifndef __IMICRObj_FWD_DEFINED__
#define __IMICRObj_FWD_DEFINED__
typedef interface IMICRObj IMICRObj;
#endif 	/* __IMICRObj_FWD_DEFINED__ */


#ifndef __IPrinterObj_FWD_DEFINED__
#define __IPrinterObj_FWD_DEFINED__
typedef interface IPrinterObj IPrinterObj;
#endif 	/* __IPrinterObj_FWD_DEFINED__ */


#ifndef __IPinpadObj_FWD_DEFINED__
#define __IPinpadObj_FWD_DEFINED__
typedef interface IPinpadObj IPinpadObj;
#endif 	/* __IPinpadObj_FWD_DEFINED__ */


#ifndef __IScaleObj_FWD_DEFINED__
#define __IScaleObj_FWD_DEFINED__
typedef interface IScaleObj IScaleObj;
#endif 	/* __IScaleObj_FWD_DEFINED__ */


#ifndef __ICustomObj_FWD_DEFINED__
#define __ICustomObj_FWD_DEFINED__
typedef interface ICustomObj ICustomObj;
#endif 	/* __ICustomObj_FWD_DEFINED__ */


#ifndef __ITBGenObj_FWD_DEFINED__
#define __ITBGenObj_FWD_DEFINED__
typedef interface ITBGenObj ITBGenObj;
#endif 	/* __ITBGenObj_FWD_DEFINED__ */


#ifndef __ItbConnect_FWD_DEFINED__
#define __ItbConnect_FWD_DEFINED__
typedef interface ItbConnect ItbConnect;
#endif 	/* __ItbConnect_FWD_DEFINED__ */


#ifndef __IWindowMsg_FWD_DEFINED__
#define __IWindowMsg_FWD_DEFINED__
typedef interface IWindowMsg IWindowMsg;
#endif 	/* __IWindowMsg_FWD_DEFINED__ */


#ifndef __CItemObj_FWD_DEFINED__
#define __CItemObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class CItemObj CItemObj;
#else
typedef struct CItemObj CItemObj;
#endif /* __cplusplus */

#endif 	/* __CItemObj_FWD_DEFINED__ */


#ifndef __CCashDrawerObj_FWD_DEFINED__
#define __CCashDrawerObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class CCashDrawerObj CCashDrawerObj;
#else
typedef struct CCashDrawerObj CCashDrawerObj;
#endif /* __cplusplus */

#endif 	/* __CCashDrawerObj_FWD_DEFINED__ */


#ifndef __CTotalObj_FWD_DEFINED__
#define __CTotalObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class CTotalObj CTotalObj;
#else
typedef struct CTotalObj CTotalObj;
#endif /* __cplusplus */

#endif 	/* __CTotalObj_FWD_DEFINED__ */


#ifndef __CTenderObj_FWD_DEFINED__
#define __CTenderObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class CTenderObj CTenderObj;
#else
typedef struct CTenderObj CTenderObj;
#endif /* __cplusplus */

#endif 	/* __CTenderObj_FWD_DEFINED__ */


#ifndef __CRewardObj_FWD_DEFINED__
#define __CRewardObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class CRewardObj CRewardObj;
#else
typedef struct CRewardObj CRewardObj;
#endif /* __cplusplus */

#endif 	/* __CRewardObj_FWD_DEFINED__ */


#ifndef __CScannerObj_FWD_DEFINED__
#define __CScannerObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class CScannerObj CScannerObj;
#else
typedef struct CScannerObj CScannerObj;
#endif /* __cplusplus */

#endif 	/* __CScannerObj_FWD_DEFINED__ */


#ifndef __CKeylockObj_FWD_DEFINED__
#define __CKeylockObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class CKeylockObj CKeylockObj;
#else
typedef struct CKeylockObj CKeylockObj;
#endif /* __cplusplus */

#endif 	/* __CKeylockObj_FWD_DEFINED__ */


#ifndef __CKeyboardObj_FWD_DEFINED__
#define __CKeyboardObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class CKeyboardObj CKeyboardObj;
#else
typedef struct CKeyboardObj CKeyboardObj;
#endif /* __cplusplus */

#endif 	/* __CKeyboardObj_FWD_DEFINED__ */


#ifndef __CMSRObj_FWD_DEFINED__
#define __CMSRObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class CMSRObj CMSRObj;
#else
typedef struct CMSRObj CMSRObj;
#endif /* __cplusplus */

#endif 	/* __CMSRObj_FWD_DEFINED__ */


#ifndef __CMICRObj_FWD_DEFINED__
#define __CMICRObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class CMICRObj CMICRObj;
#else
typedef struct CMICRObj CMICRObj;
#endif /* __cplusplus */

#endif 	/* __CMICRObj_FWD_DEFINED__ */


#ifndef __CPrinterObj_FWD_DEFINED__
#define __CPrinterObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class CPrinterObj CPrinterObj;
#else
typedef struct CPrinterObj CPrinterObj;
#endif /* __cplusplus */

#endif 	/* __CPrinterObj_FWD_DEFINED__ */


#ifndef __CPinpadObj_FWD_DEFINED__
#define __CPinpadObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class CPinpadObj CPinpadObj;
#else
typedef struct CPinpadObj CPinpadObj;
#endif /* __cplusplus */

#endif 	/* __CPinpadObj_FWD_DEFINED__ */


#ifndef __CScaleObj_FWD_DEFINED__
#define __CScaleObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class CScaleObj CScaleObj;
#else
typedef struct CScaleObj CScaleObj;
#endif /* __cplusplus */

#endif 	/* __CScaleObj_FWD_DEFINED__ */


#ifndef __CCustomObj_FWD_DEFINED__
#define __CCustomObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class CCustomObj CCustomObj;
#else
typedef struct CCustomObj CCustomObj;
#endif /* __cplusplus */

#endif 	/* __CCustomObj_FWD_DEFINED__ */


#ifndef __CTBGenObj_FWD_DEFINED__
#define __CTBGenObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class CTBGenObj CTBGenObj;
#else
typedef struct CTBGenObj CTBGenObj;
#endif /* __cplusplus */

#endif 	/* __CTBGenObj_FWD_DEFINED__ */


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


#ifndef __CWindowMsg_FWD_DEFINED__
#define __CWindowMsg_FWD_DEFINED__

#ifdef __cplusplus
typedef class CWindowMsg CWindowMsg;
#else
typedef struct CWindowMsg CWindowMsg;
#endif /* __cplusplus */

#endif 	/* __CWindowMsg_FWD_DEFINED__ */


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

#ifndef __ITBStream_INTERFACE_DEFINED__
#define __ITBStream_INTERFACE_DEFINED__

/* interface ITBStream */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITBStream;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3FF844BD-E48C-4E60-B5AF-12827E1F9C22")
    ITBStream : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StreamOut( 
            /* [out][in] */ LONG *cb,
            /* [retval][out] */ BYTE **pBuffer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ParseStream( 
            /* [in] */ VARIANT Var) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ParseBuffer( 
            /* [in] */ BYTE *pBuffer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMessageA( 
            /* [out] */ LONG *o_pTBMsgObj) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITBStreamVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITBStream * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITBStream * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITBStream * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITBStream * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITBStream * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITBStream * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITBStream * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StreamOut )( 
            ITBStream * This,
            /* [out][in] */ LONG *cb,
            /* [retval][out] */ BYTE **pBuffer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ParseStream )( 
            ITBStream * This,
            /* [in] */ VARIANT Var);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ParseBuffer )( 
            ITBStream * This,
            /* [in] */ BYTE *pBuffer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMessageA )( 
            ITBStream * This,
            /* [out] */ LONG *o_pTBMsgObj);
        
        END_INTERFACE
    } ITBStreamVtbl;

    interface ITBStream
    {
        CONST_VTBL struct ITBStreamVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITBStream_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITBStream_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITBStream_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITBStream_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITBStream_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITBStream_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITBStream_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITBStream_StreamOut(This,cb,pBuffer)	\
    (This)->lpVtbl -> StreamOut(This,cb,pBuffer)

#define ITBStream_ParseStream(This,Var)	\
    (This)->lpVtbl -> ParseStream(This,Var)

#define ITBStream_ParseBuffer(This,pBuffer)	\
    (This)->lpVtbl -> ParseBuffer(This,pBuffer)

#define ITBStream_GetMessageA(This,o_pTBMsgObj)	\
    (This)->lpVtbl -> GetMessageA(This,o_pTBMsgObj)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITBStream_StreamOut_Proxy( 
    ITBStream * This,
    /* [out][in] */ LONG *cb,
    /* [retval][out] */ BYTE **pBuffer);


void __RPC_STUB ITBStream_StreamOut_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITBStream_ParseStream_Proxy( 
    ITBStream * This,
    /* [in] */ VARIANT Var);


void __RPC_STUB ITBStream_ParseStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITBStream_ParseBuffer_Proxy( 
    ITBStream * This,
    /* [in] */ BYTE *pBuffer);


void __RPC_STUB ITBStream_ParseBuffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITBStream_GetMessageA_Proxy( 
    ITBStream * This,
    /* [out] */ LONG *o_pTBMsgObj);


void __RPC_STUB ITBStream_GetMessageA_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITBStream_INTERFACE_DEFINED__ */


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
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Department( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_EntryID( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_EntryID( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RestrictedAge( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RestrictedAge( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TareCode( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TareCode( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TaxExempt( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TaxExempt( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_EndorsementRequired( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_EndorsementRequired( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
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
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Department )( 
            IItemObj * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_EntryID )( 
            IItemObj * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_EntryID )( 
            IItemObj * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RestrictedAge )( 
            IItemObj * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RestrictedAge )( 
            IItemObj * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TareCode )( 
            IItemObj * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TareCode )( 
            IItemObj * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TaxExempt )( 
            IItemObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TaxExempt )( 
            IItemObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_EndorsementRequired )( 
            IItemObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_EndorsementRequired )( 
            IItemObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IItemObj * This);
        
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

#define IItemObj_get_TaxExempt(This,pVal)	\
    (This)->lpVtbl -> get_TaxExempt(This,pVal)

#define IItemObj_put_TaxExempt(This,newVal)	\
    (This)->lpVtbl -> put_TaxExempt(This,newVal)

#define IItemObj_get_EndorsementRequired(This,pVal)	\
    (This)->lpVtbl -> get_EndorsementRequired(This,pVal)

#define IItemObj_put_EndorsementRequired(This,newVal)	\
    (This)->lpVtbl -> put_EndorsementRequired(This,newVal)

#define IItemObj_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

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
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB IItemObj_get_Department_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_Department_Proxy( 
    IItemObj * This,
    /* [in] */ LONG newVal);


void __RPC_STUB IItemObj_put_Department_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_EntryID_Proxy( 
    IItemObj * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB IItemObj_get_EntryID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_EntryID_Proxy( 
    IItemObj * This,
    /* [in] */ LONG newVal);


void __RPC_STUB IItemObj_put_EntryID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_RestrictedAge_Proxy( 
    IItemObj * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB IItemObj_get_RestrictedAge_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_RestrictedAge_Proxy( 
    IItemObj * This,
    /* [in] */ LONG newVal);


void __RPC_STUB IItemObj_put_RestrictedAge_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_TareCode_Proxy( 
    IItemObj * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB IItemObj_get_TareCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_TareCode_Proxy( 
    IItemObj * This,
    /* [in] */ LONG newVal);


void __RPC_STUB IItemObj_put_TareCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_TaxExempt_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IItemObj_get_TaxExempt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_TaxExempt_Proxy( 
    IItemObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IItemObj_put_TaxExempt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IItemObj_get_EndorsementRequired_Proxy( 
    IItemObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IItemObj_get_EndorsementRequired_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IItemObj_put_EndorsementRequired_Proxy( 
    IItemObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IItemObj_put_EndorsementRequired_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IItemObj_Reset_Proxy( 
    IItemObj * This);


void __RPC_STUB IItemObj_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IItemObj_INTERFACE_DEFINED__ */


/* interface __MIDL_itf__tbControls_0746 */
/* [local] */ 

typedef 
enum CashDrawerStations
    {	DRAWER_A	= 1,
	DRAWER_B	= 2,
	DRAWER_UNDEFINED	= 153
    } 	CashDrawerStations;

typedef 
enum CashDrawerStates
    {	CD_OPEN	= 1,
	CD_CLOSED	= 2,
	CD_UNKNOWN	= 153
    } 	CashDrawerStates;



extern RPC_IF_HANDLE __MIDL_itf__tbControls_0746_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf__tbControls_0746_v0_0_s_ifspec;

#ifndef __ICashDrawerObj_INTERFACE_DEFINED__
#define __ICashDrawerObj_INTERFACE_DEFINED__

/* interface ICashDrawerObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICashDrawerObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3FF844BD-E48C-4E60-B5AF-12827E1F9C21")
    ICashDrawerObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Station( 
            /* [retval][out] */ CashDrawerStations *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Station( 
            /* [in] */ CashDrawerStations newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Status( 
            /* [retval][out] */ CashDrawerStates *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Status( 
            /* [in] */ CashDrawerStates newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICashDrawerObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICashDrawerObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICashDrawerObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICashDrawerObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICashDrawerObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICashDrawerObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICashDrawerObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICashDrawerObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Station )( 
            ICashDrawerObj * This,
            /* [retval][out] */ CashDrawerStations *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Station )( 
            ICashDrawerObj * This,
            /* [in] */ CashDrawerStations newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Status )( 
            ICashDrawerObj * This,
            /* [retval][out] */ CashDrawerStates *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Status )( 
            ICashDrawerObj * This,
            /* [in] */ CashDrawerStates newVal);
        
        END_INTERFACE
    } ICashDrawerObjVtbl;

    interface ICashDrawerObj
    {
        CONST_VTBL struct ICashDrawerObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICashDrawerObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICashDrawerObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICashDrawerObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICashDrawerObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICashDrawerObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICashDrawerObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICashDrawerObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICashDrawerObj_get_Station(This,pVal)	\
    (This)->lpVtbl -> get_Station(This,pVal)

#define ICashDrawerObj_put_Station(This,newVal)	\
    (This)->lpVtbl -> put_Station(This,newVal)

#define ICashDrawerObj_get_Status(This,pVal)	\
    (This)->lpVtbl -> get_Status(This,pVal)

#define ICashDrawerObj_put_Status(This,newVal)	\
    (This)->lpVtbl -> put_Status(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICashDrawerObj_get_Station_Proxy( 
    ICashDrawerObj * This,
    /* [retval][out] */ CashDrawerStations *pVal);


void __RPC_STUB ICashDrawerObj_get_Station_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ICashDrawerObj_put_Station_Proxy( 
    ICashDrawerObj * This,
    /* [in] */ CashDrawerStations newVal);


void __RPC_STUB ICashDrawerObj_put_Station_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICashDrawerObj_get_Status_Proxy( 
    ICashDrawerObj * This,
    /* [retval][out] */ CashDrawerStates *pVal);


void __RPC_STUB ICashDrawerObj_get_Status_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ICashDrawerObj_put_Status_Proxy( 
    ICashDrawerObj * This,
    /* [in] */ CashDrawerStates newVal);


void __RPC_STUB ICashDrawerObj_put_Status_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICashDrawerObj_INTERFACE_DEFINED__ */


#ifndef __ITotalObj_INTERFACE_DEFINED__
#define __ITotalObj_INTERFACE_DEFINED__

/* interface ITotalObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITotalObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EB2C6ECD-2D7D-4626-B8FE-264738AA036E")
    ITotalObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TaxDue( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TaxDue( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TotalDue( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TotalDue( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TotalDiscount( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TotalDiscount( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrorDescription( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ErrorDescription( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FoodStampAmount( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FoodStampAmount( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RewardPoints( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RewardPoints( 
            /* [in] */ LONG newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITotalObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITotalObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITotalObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITotalObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITotalObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITotalObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITotalObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITotalObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TaxDue )( 
            ITotalObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TaxDue )( 
            ITotalObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TotalDue )( 
            ITotalObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TotalDue )( 
            ITotalObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TotalDiscount )( 
            ITotalObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TotalDiscount )( 
            ITotalObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrorDescription )( 
            ITotalObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ErrorDescription )( 
            ITotalObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FoodStampAmount )( 
            ITotalObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FoodStampAmount )( 
            ITotalObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RewardPoints )( 
            ITotalObj * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RewardPoints )( 
            ITotalObj * This,
            /* [in] */ LONG newVal);
        
        END_INTERFACE
    } ITotalObjVtbl;

    interface ITotalObj
    {
        CONST_VTBL struct ITotalObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITotalObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITotalObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITotalObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITotalObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITotalObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITotalObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITotalObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITotalObj_get_TaxDue(This,pVal)	\
    (This)->lpVtbl -> get_TaxDue(This,pVal)

#define ITotalObj_put_TaxDue(This,newVal)	\
    (This)->lpVtbl -> put_TaxDue(This,newVal)

#define ITotalObj_get_TotalDue(This,pVal)	\
    (This)->lpVtbl -> get_TotalDue(This,pVal)

#define ITotalObj_put_TotalDue(This,newVal)	\
    (This)->lpVtbl -> put_TotalDue(This,newVal)

#define ITotalObj_get_TotalDiscount(This,pVal)	\
    (This)->lpVtbl -> get_TotalDiscount(This,pVal)

#define ITotalObj_put_TotalDiscount(This,newVal)	\
    (This)->lpVtbl -> put_TotalDiscount(This,newVal)

#define ITotalObj_get_ErrorDescription(This,pVal)	\
    (This)->lpVtbl -> get_ErrorDescription(This,pVal)

#define ITotalObj_put_ErrorDescription(This,newVal)	\
    (This)->lpVtbl -> put_ErrorDescription(This,newVal)

#define ITotalObj_get_FoodStampAmount(This,pVal)	\
    (This)->lpVtbl -> get_FoodStampAmount(This,pVal)

#define ITotalObj_put_FoodStampAmount(This,newVal)	\
    (This)->lpVtbl -> put_FoodStampAmount(This,newVal)

#define ITotalObj_get_RewardPoints(This,pVal)	\
    (This)->lpVtbl -> get_RewardPoints(This,pVal)

#define ITotalObj_put_RewardPoints(This,newVal)	\
    (This)->lpVtbl -> put_RewardPoints(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITotalObj_get_TaxDue_Proxy( 
    ITotalObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ITotalObj_get_TaxDue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITotalObj_put_TaxDue_Proxy( 
    ITotalObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITotalObj_put_TaxDue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITotalObj_get_TotalDue_Proxy( 
    ITotalObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ITotalObj_get_TotalDue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITotalObj_put_TotalDue_Proxy( 
    ITotalObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITotalObj_put_TotalDue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITotalObj_get_TotalDiscount_Proxy( 
    ITotalObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ITotalObj_get_TotalDiscount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITotalObj_put_TotalDiscount_Proxy( 
    ITotalObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITotalObj_put_TotalDiscount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITotalObj_get_ErrorDescription_Proxy( 
    ITotalObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ITotalObj_get_ErrorDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITotalObj_put_ErrorDescription_Proxy( 
    ITotalObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITotalObj_put_ErrorDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITotalObj_get_FoodStampAmount_Proxy( 
    ITotalObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ITotalObj_get_FoodStampAmount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITotalObj_put_FoodStampAmount_Proxy( 
    ITotalObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITotalObj_put_FoodStampAmount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITotalObj_get_RewardPoints_Proxy( 
    ITotalObj * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB ITotalObj_get_RewardPoints_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITotalObj_put_RewardPoints_Proxy( 
    ITotalObj * This,
    /* [in] */ LONG newVal);


void __RPC_STUB ITotalObj_put_RewardPoints_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITotalObj_INTERFACE_DEFINED__ */


#ifndef __ITenderObj_INTERFACE_DEFINED__
#define __ITenderObj_INTERFACE_DEFINED__

/* interface ITenderObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITenderObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5520ED1C-ABB0-4C35-8DF0-6598F441051F")
    ITenderObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Description( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Description( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Type( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Amount( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Amount( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Balance( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Balance( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ChangeDue( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ChangeDue( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Accepted( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Accepted( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Voided( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Voided( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CardRemainingBalance( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CardRemainingBalance( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrorDescription( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ErrorDescription( 
            /* [in] */ BSTR newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITenderObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITenderObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITenderObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITenderObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITenderObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITenderObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITenderObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITenderObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Description )( 
            ITenderObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Description )( 
            ITenderObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            ITenderObj * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Type )( 
            ITenderObj * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Amount )( 
            ITenderObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Amount )( 
            ITenderObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Balance )( 
            ITenderObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Balance )( 
            ITenderObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ChangeDue )( 
            ITenderObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ChangeDue )( 
            ITenderObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Accepted )( 
            ITenderObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Accepted )( 
            ITenderObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Voided )( 
            ITenderObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Voided )( 
            ITenderObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CardRemainingBalance )( 
            ITenderObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CardRemainingBalance )( 
            ITenderObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrorDescription )( 
            ITenderObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ErrorDescription )( 
            ITenderObj * This,
            /* [in] */ BSTR newVal);
        
        END_INTERFACE
    } ITenderObjVtbl;

    interface ITenderObj
    {
        CONST_VTBL struct ITenderObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITenderObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITenderObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITenderObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITenderObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITenderObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITenderObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITenderObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITenderObj_get_Description(This,pVal)	\
    (This)->lpVtbl -> get_Description(This,pVal)

#define ITenderObj_put_Description(This,newVal)	\
    (This)->lpVtbl -> put_Description(This,newVal)

#define ITenderObj_get_Type(This,pVal)	\
    (This)->lpVtbl -> get_Type(This,pVal)

#define ITenderObj_put_Type(This,newVal)	\
    (This)->lpVtbl -> put_Type(This,newVal)

#define ITenderObj_get_Amount(This,pVal)	\
    (This)->lpVtbl -> get_Amount(This,pVal)

#define ITenderObj_put_Amount(This,newVal)	\
    (This)->lpVtbl -> put_Amount(This,newVal)

#define ITenderObj_get_Balance(This,pVal)	\
    (This)->lpVtbl -> get_Balance(This,pVal)

#define ITenderObj_put_Balance(This,newVal)	\
    (This)->lpVtbl -> put_Balance(This,newVal)

#define ITenderObj_get_ChangeDue(This,pVal)	\
    (This)->lpVtbl -> get_ChangeDue(This,pVal)

#define ITenderObj_put_ChangeDue(This,newVal)	\
    (This)->lpVtbl -> put_ChangeDue(This,newVal)

#define ITenderObj_get_Accepted(This,pVal)	\
    (This)->lpVtbl -> get_Accepted(This,pVal)

#define ITenderObj_put_Accepted(This,newVal)	\
    (This)->lpVtbl -> put_Accepted(This,newVal)

#define ITenderObj_get_Voided(This,pVal)	\
    (This)->lpVtbl -> get_Voided(This,pVal)

#define ITenderObj_put_Voided(This,newVal)	\
    (This)->lpVtbl -> put_Voided(This,newVal)

#define ITenderObj_get_CardRemainingBalance(This,pVal)	\
    (This)->lpVtbl -> get_CardRemainingBalance(This,pVal)

#define ITenderObj_put_CardRemainingBalance(This,newVal)	\
    (This)->lpVtbl -> put_CardRemainingBalance(This,newVal)

#define ITenderObj_get_ErrorDescription(This,pVal)	\
    (This)->lpVtbl -> get_ErrorDescription(This,pVal)

#define ITenderObj_put_ErrorDescription(This,newVal)	\
    (This)->lpVtbl -> put_ErrorDescription(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITenderObj_get_Description_Proxy( 
    ITenderObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ITenderObj_get_Description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITenderObj_put_Description_Proxy( 
    ITenderObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITenderObj_put_Description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITenderObj_get_Type_Proxy( 
    ITenderObj * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB ITenderObj_get_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITenderObj_put_Type_Proxy( 
    ITenderObj * This,
    /* [in] */ LONG newVal);


void __RPC_STUB ITenderObj_put_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITenderObj_get_Amount_Proxy( 
    ITenderObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ITenderObj_get_Amount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITenderObj_put_Amount_Proxy( 
    ITenderObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITenderObj_put_Amount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITenderObj_get_Balance_Proxy( 
    ITenderObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ITenderObj_get_Balance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITenderObj_put_Balance_Proxy( 
    ITenderObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITenderObj_put_Balance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITenderObj_get_ChangeDue_Proxy( 
    ITenderObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ITenderObj_get_ChangeDue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITenderObj_put_ChangeDue_Proxy( 
    ITenderObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITenderObj_put_ChangeDue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITenderObj_get_Accepted_Proxy( 
    ITenderObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB ITenderObj_get_Accepted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITenderObj_put_Accepted_Proxy( 
    ITenderObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB ITenderObj_put_Accepted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITenderObj_get_Voided_Proxy( 
    ITenderObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB ITenderObj_get_Voided_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITenderObj_put_Voided_Proxy( 
    ITenderObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB ITenderObj_put_Voided_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITenderObj_get_CardRemainingBalance_Proxy( 
    ITenderObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ITenderObj_get_CardRemainingBalance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITenderObj_put_CardRemainingBalance_Proxy( 
    ITenderObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITenderObj_put_CardRemainingBalance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITenderObj_get_ErrorDescription_Proxy( 
    ITenderObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ITenderObj_get_ErrorDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITenderObj_put_ErrorDescription_Proxy( 
    ITenderObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITenderObj_put_ErrorDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITenderObj_INTERFACE_DEFINED__ */


/* interface __MIDL_itf__tbControls_0749 */
/* [local] */ 

typedef 
enum tRewardLinePosType
    {	BOTTOM_OF_RECEIPT	= 1,
	ABOVE_REF_ENTRYID	= 2,
	BELOW_REF_ENTRYID	= 3,
	POS_NONE	= 4
    } 	tRewardLinePosType;



extern RPC_IF_HANDLE __MIDL_itf__tbControls_0749_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf__tbControls_0749_v0_0_s_ifspec;

#ifndef __IRewardObj_INTERFACE_DEFINED__
#define __IRewardObj_INTERFACE_DEFINED__

/* interface IRewardObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IRewardObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("83398A76-DB40-412F-AB39-34EC9A187C59")
    IRewardObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RewardValue( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RewardValue( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RewardPosition( 
            /* [retval][out] */ tRewardLinePosType *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RewardPosition( 
            /* [in] */ tRewardLinePosType newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsVoided( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IsVoided( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_EntryId( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_EntryId( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UpdateRewardTotal( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_UpdateRewardTotal( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Description( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Description( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AssociatedEntryId( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AssociatedEntryId( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ShowValue( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ShowValue( 
            /* [in] */ SHORT newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRewardObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRewardObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRewardObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRewardObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRewardObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRewardObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRewardObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRewardObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RewardValue )( 
            IRewardObj * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RewardValue )( 
            IRewardObj * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RewardPosition )( 
            IRewardObj * This,
            /* [retval][out] */ tRewardLinePosType *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RewardPosition )( 
            IRewardObj * This,
            /* [in] */ tRewardLinePosType newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsVoided )( 
            IRewardObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsVoided )( 
            IRewardObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_EntryId )( 
            IRewardObj * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_EntryId )( 
            IRewardObj * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_UpdateRewardTotal )( 
            IRewardObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_UpdateRewardTotal )( 
            IRewardObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Description )( 
            IRewardObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Description )( 
            IRewardObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AssociatedEntryId )( 
            IRewardObj * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AssociatedEntryId )( 
            IRewardObj * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShowValue )( 
            IRewardObj * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShowValue )( 
            IRewardObj * This,
            /* [in] */ SHORT newVal);
        
        END_INTERFACE
    } IRewardObjVtbl;

    interface IRewardObj
    {
        CONST_VTBL struct IRewardObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRewardObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRewardObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRewardObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRewardObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRewardObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRewardObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRewardObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRewardObj_get_RewardValue(This,pVal)	\
    (This)->lpVtbl -> get_RewardValue(This,pVal)

#define IRewardObj_put_RewardValue(This,newVal)	\
    (This)->lpVtbl -> put_RewardValue(This,newVal)

#define IRewardObj_get_RewardPosition(This,pVal)	\
    (This)->lpVtbl -> get_RewardPosition(This,pVal)

#define IRewardObj_put_RewardPosition(This,newVal)	\
    (This)->lpVtbl -> put_RewardPosition(This,newVal)

#define IRewardObj_get_IsVoided(This,pVal)	\
    (This)->lpVtbl -> get_IsVoided(This,pVal)

#define IRewardObj_put_IsVoided(This,newVal)	\
    (This)->lpVtbl -> put_IsVoided(This,newVal)

#define IRewardObj_get_EntryId(This,pVal)	\
    (This)->lpVtbl -> get_EntryId(This,pVal)

#define IRewardObj_put_EntryId(This,newVal)	\
    (This)->lpVtbl -> put_EntryId(This,newVal)

#define IRewardObj_get_UpdateRewardTotal(This,pVal)	\
    (This)->lpVtbl -> get_UpdateRewardTotal(This,pVal)

#define IRewardObj_put_UpdateRewardTotal(This,newVal)	\
    (This)->lpVtbl -> put_UpdateRewardTotal(This,newVal)

#define IRewardObj_get_Description(This,pVal)	\
    (This)->lpVtbl -> get_Description(This,pVal)

#define IRewardObj_put_Description(This,newVal)	\
    (This)->lpVtbl -> put_Description(This,newVal)

#define IRewardObj_get_AssociatedEntryId(This,pVal)	\
    (This)->lpVtbl -> get_AssociatedEntryId(This,pVal)

#define IRewardObj_put_AssociatedEntryId(This,newVal)	\
    (This)->lpVtbl -> put_AssociatedEntryId(This,newVal)

#define IRewardObj_get_ShowValue(This,pVal)	\
    (This)->lpVtbl -> get_ShowValue(This,pVal)

#define IRewardObj_put_ShowValue(This,newVal)	\
    (This)->lpVtbl -> put_ShowValue(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRewardObj_get_RewardValue_Proxy( 
    IRewardObj * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB IRewardObj_get_RewardValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRewardObj_put_RewardValue_Proxy( 
    IRewardObj * This,
    /* [in] */ LONG newVal);


void __RPC_STUB IRewardObj_put_RewardValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRewardObj_get_RewardPosition_Proxy( 
    IRewardObj * This,
    /* [retval][out] */ tRewardLinePosType *pVal);


void __RPC_STUB IRewardObj_get_RewardPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRewardObj_put_RewardPosition_Proxy( 
    IRewardObj * This,
    /* [in] */ tRewardLinePosType newVal);


void __RPC_STUB IRewardObj_put_RewardPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRewardObj_get_IsVoided_Proxy( 
    IRewardObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IRewardObj_get_IsVoided_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRewardObj_put_IsVoided_Proxy( 
    IRewardObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IRewardObj_put_IsVoided_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRewardObj_get_EntryId_Proxy( 
    IRewardObj * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB IRewardObj_get_EntryId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRewardObj_put_EntryId_Proxy( 
    IRewardObj * This,
    /* [in] */ LONG newVal);


void __RPC_STUB IRewardObj_put_EntryId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRewardObj_get_UpdateRewardTotal_Proxy( 
    IRewardObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IRewardObj_get_UpdateRewardTotal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRewardObj_put_UpdateRewardTotal_Proxy( 
    IRewardObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IRewardObj_put_UpdateRewardTotal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRewardObj_get_Description_Proxy( 
    IRewardObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IRewardObj_get_Description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRewardObj_put_Description_Proxy( 
    IRewardObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IRewardObj_put_Description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRewardObj_get_AssociatedEntryId_Proxy( 
    IRewardObj * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB IRewardObj_get_AssociatedEntryId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRewardObj_put_AssociatedEntryId_Proxy( 
    IRewardObj * This,
    /* [in] */ LONG newVal);


void __RPC_STUB IRewardObj_put_AssociatedEntryId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IRewardObj_get_ShowValue_Proxy( 
    IRewardObj * This,
    /* [retval][out] */ SHORT *pVal);


void __RPC_STUB IRewardObj_get_ShowValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IRewardObj_put_ShowValue_Proxy( 
    IRewardObj * This,
    /* [in] */ SHORT newVal);


void __RPC_STUB IRewardObj_put_ShowValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRewardObj_INTERFACE_DEFINED__ */


/* interface __MIDL_itf__tbControls_0750 */
/* [local] */ 

typedef 
enum BarcodeTypes
    {	BT_UPCA	= 1,
	BT_UPCA_SUP	= 2,
	BT_UPCE	= 3,
	BT_UPCE_SUP	= 4,
	BT_UPCD1	= 5,
	BT_UPCD2	= 6,
	BT_UPCD3	= 7,
	BT_UPCD4	= 8,
	BT_UPCD5	= 9,
	BT_EAN8	= 10,
	BT_EAN8_SUP	= 11,
	BT_EAN13	= 12,
	BT_EAN13_SUP	= 13,
	BT_EAN128	= 14,
	BT_STD25	= 15,
	BT_CODABAR	= 16,
	BT_CODE39	= 17,
	BT_CODE93	= 18,
	BT_CODE128	= 19,
	BT_OCRA	= 20,
	BT_OCRB	= 20,
	BT_PDF417	= 64,
	BT_MAXICODE	= 65,
	BT_UNKNOWN	= 153
    } 	BarcodeTypes;



extern RPC_IF_HANDLE __MIDL_itf__tbControls_0750_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf__tbControls_0750_v0_0_s_ifspec;

#ifndef __IScannerObj_INTERFACE_DEFINED__
#define __IScannerObj_INTERFACE_DEFINED__

/* interface IScannerObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IScannerObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BA1AC205-4D3E-43C3-99D0-C1A3D749BF93")
    IScannerObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BarcodeType( 
            /* [retval][out] */ BarcodeTypes *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BarcodeType( 
            /* [in] */ BarcodeTypes newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Data( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Data( 
            /* [in] */ BSTR newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IScannerObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IScannerObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IScannerObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IScannerObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IScannerObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IScannerObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IScannerObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IScannerObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BarcodeType )( 
            IScannerObj * This,
            /* [retval][out] */ BarcodeTypes *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BarcodeType )( 
            IScannerObj * This,
            /* [in] */ BarcodeTypes newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Data )( 
            IScannerObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Data )( 
            IScannerObj * This,
            /* [in] */ BSTR newVal);
        
        END_INTERFACE
    } IScannerObjVtbl;

    interface IScannerObj
    {
        CONST_VTBL struct IScannerObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IScannerObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IScannerObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IScannerObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IScannerObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IScannerObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IScannerObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IScannerObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IScannerObj_get_BarcodeType(This,pVal)	\
    (This)->lpVtbl -> get_BarcodeType(This,pVal)

#define IScannerObj_put_BarcodeType(This,newVal)	\
    (This)->lpVtbl -> put_BarcodeType(This,newVal)

#define IScannerObj_get_Data(This,pVal)	\
    (This)->lpVtbl -> get_Data(This,pVal)

#define IScannerObj_put_Data(This,newVal)	\
    (This)->lpVtbl -> put_Data(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IScannerObj_get_BarcodeType_Proxy( 
    IScannerObj * This,
    /* [retval][out] */ BarcodeTypes *pVal);


void __RPC_STUB IScannerObj_get_BarcodeType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IScannerObj_put_BarcodeType_Proxy( 
    IScannerObj * This,
    /* [in] */ BarcodeTypes newVal);


void __RPC_STUB IScannerObj_put_BarcodeType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IScannerObj_get_Data_Proxy( 
    IScannerObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IScannerObj_get_Data_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IScannerObj_put_Data_Proxy( 
    IScannerObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IScannerObj_put_Data_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IScannerObj_INTERFACE_DEFINED__ */


#ifndef __IKeylockObj_INTERFACE_DEFINED__
#define __IKeylockObj_INTERFACE_DEFINED__

/* interface IKeylockObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IKeylockObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("83DBC6EC-6015-408C-9249-0FBA333CB7D7")
    IKeylockObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Position( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Position( 
            /* [in] */ BYTE newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IKeylockObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKeylockObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKeylockObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKeylockObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IKeylockObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IKeylockObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IKeylockObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IKeylockObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Position )( 
            IKeylockObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Position )( 
            IKeylockObj * This,
            /* [in] */ BYTE newVal);
        
        END_INTERFACE
    } IKeylockObjVtbl;

    interface IKeylockObj
    {
        CONST_VTBL struct IKeylockObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKeylockObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IKeylockObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IKeylockObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IKeylockObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IKeylockObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IKeylockObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IKeylockObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IKeylockObj_get_Position(This,pVal)	\
    (This)->lpVtbl -> get_Position(This,pVal)

#define IKeylockObj_put_Position(This,newVal)	\
    (This)->lpVtbl -> put_Position(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IKeylockObj_get_Position_Proxy( 
    IKeylockObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IKeylockObj_get_Position_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IKeylockObj_put_Position_Proxy( 
    IKeylockObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IKeylockObj_put_Position_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IKeylockObj_INTERFACE_DEFINED__ */


/* interface __MIDL_itf__tbControls_0752 */
/* [local] */ 

typedef 
enum CombinationKeyFlags
    {	KEY_PRESSED_NONE	= 0,
	SHIFT_KEY_PESSED	= 1,
	CTRL_KEY_PRESSED	= 2,
	ALT_KEY_PRESSED	= 4
    } 	CombinationKeyFlags;

typedef 
enum KeyEventType
    {	KEY_DOWN	= 0,
	EXTENDED_KEY	= 1,
	KEY_UP	= 2
    } 	KeyEventType;



extern RPC_IF_HANDLE __MIDL_itf__tbControls_0752_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf__tbControls_0752_v0_0_s_ifspec;

#ifndef __IKeyboardObj_INTERFACE_DEFINED__
#define __IKeyboardObj_INTERFACE_DEFINED__

/* interface IKeyboardObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IKeyboardObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6E77AFE9-47D1-494F-9506-97B889C8ABE7")
    IKeyboardObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_KeyCode( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_KeyCode( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_KeyEvent( 
            /* [retval][out] */ KeyEventType *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_KeyEvent( 
            /* [in] */ KeyEventType newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CombinationKeys( 
            /* [retval][out] */ CombinationKeyFlags *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CombinationKeys( 
            /* [in] */ CombinationKeyFlags newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IKeyboardObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKeyboardObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKeyboardObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKeyboardObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IKeyboardObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IKeyboardObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IKeyboardObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IKeyboardObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_KeyCode )( 
            IKeyboardObj * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_KeyCode )( 
            IKeyboardObj * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_KeyEvent )( 
            IKeyboardObj * This,
            /* [retval][out] */ KeyEventType *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_KeyEvent )( 
            IKeyboardObj * This,
            /* [in] */ KeyEventType newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CombinationKeys )( 
            IKeyboardObj * This,
            /* [retval][out] */ CombinationKeyFlags *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CombinationKeys )( 
            IKeyboardObj * This,
            /* [in] */ CombinationKeyFlags newVal);
        
        END_INTERFACE
    } IKeyboardObjVtbl;

    interface IKeyboardObj
    {
        CONST_VTBL struct IKeyboardObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKeyboardObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IKeyboardObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IKeyboardObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IKeyboardObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IKeyboardObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IKeyboardObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IKeyboardObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IKeyboardObj_get_KeyCode(This,pVal)	\
    (This)->lpVtbl -> get_KeyCode(This,pVal)

#define IKeyboardObj_put_KeyCode(This,newVal)	\
    (This)->lpVtbl -> put_KeyCode(This,newVal)

#define IKeyboardObj_get_KeyEvent(This,pVal)	\
    (This)->lpVtbl -> get_KeyEvent(This,pVal)

#define IKeyboardObj_put_KeyEvent(This,newVal)	\
    (This)->lpVtbl -> put_KeyEvent(This,newVal)

#define IKeyboardObj_get_CombinationKeys(This,pVal)	\
    (This)->lpVtbl -> get_CombinationKeys(This,pVal)

#define IKeyboardObj_put_CombinationKeys(This,newVal)	\
    (This)->lpVtbl -> put_CombinationKeys(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IKeyboardObj_get_KeyCode_Proxy( 
    IKeyboardObj * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB IKeyboardObj_get_KeyCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IKeyboardObj_put_KeyCode_Proxy( 
    IKeyboardObj * This,
    /* [in] */ LONG newVal);


void __RPC_STUB IKeyboardObj_put_KeyCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IKeyboardObj_get_KeyEvent_Proxy( 
    IKeyboardObj * This,
    /* [retval][out] */ KeyEventType *pVal);


void __RPC_STUB IKeyboardObj_get_KeyEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IKeyboardObj_put_KeyEvent_Proxy( 
    IKeyboardObj * This,
    /* [in] */ KeyEventType newVal);


void __RPC_STUB IKeyboardObj_put_KeyEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IKeyboardObj_get_CombinationKeys_Proxy( 
    IKeyboardObj * This,
    /* [retval][out] */ CombinationKeyFlags *pVal);


void __RPC_STUB IKeyboardObj_get_CombinationKeys_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IKeyboardObj_put_CombinationKeys_Proxy( 
    IKeyboardObj * This,
    /* [in] */ CombinationKeyFlags newVal);


void __RPC_STUB IKeyboardObj_put_CombinationKeys_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IKeyboardObj_INTERFACE_DEFINED__ */


#ifndef __IMSRObj_INTERFACE_DEFINED__
#define __IMSRObj_INTERFACE_DEFINED__

/* interface IMSRObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IMSRObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BF8A79FF-565D-4BB2-9FCC-5110A7E21295")
    IMSRObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Track1( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Track2( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Track3( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetTrack123( 
            /* [in] */ BYTE Track1Position,
            /* [in] */ BYTE Track2Position,
            /* [in] */ BYTE Track3Position,
            /* [in] */ BSTR Track123) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMSRObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMSRObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMSRObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMSRObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMSRObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMSRObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMSRObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMSRObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Track1 )( 
            IMSRObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Track2 )( 
            IMSRObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Track3 )( 
            IMSRObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetTrack123 )( 
            IMSRObj * This,
            /* [in] */ BYTE Track1Position,
            /* [in] */ BYTE Track2Position,
            /* [in] */ BYTE Track3Position,
            /* [in] */ BSTR Track123);
        
        END_INTERFACE
    } IMSRObjVtbl;

    interface IMSRObj
    {
        CONST_VTBL struct IMSRObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMSRObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMSRObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMSRObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMSRObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMSRObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMSRObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMSRObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMSRObj_get_Track1(This,pVal)	\
    (This)->lpVtbl -> get_Track1(This,pVal)

#define IMSRObj_get_Track2(This,pVal)	\
    (This)->lpVtbl -> get_Track2(This,pVal)

#define IMSRObj_get_Track3(This,pVal)	\
    (This)->lpVtbl -> get_Track3(This,pVal)

#define IMSRObj_SetTrack123(This,Track1Position,Track2Position,Track3Position,Track123)	\
    (This)->lpVtbl -> SetTrack123(This,Track1Position,Track2Position,Track3Position,Track123)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMSRObj_get_Track1_Proxy( 
    IMSRObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IMSRObj_get_Track1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMSRObj_get_Track2_Proxy( 
    IMSRObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IMSRObj_get_Track2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMSRObj_get_Track3_Proxy( 
    IMSRObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IMSRObj_get_Track3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMSRObj_SetTrack123_Proxy( 
    IMSRObj * This,
    /* [in] */ BYTE Track1Position,
    /* [in] */ BYTE Track2Position,
    /* [in] */ BYTE Track3Position,
    /* [in] */ BSTR Track123);


void __RPC_STUB IMSRObj_SetTrack123_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMSRObj_INTERFACE_DEFINED__ */


/* interface __MIDL_itf__tbControls_0754 */
/* [local] */ 

typedef 
enum CheckTypes
    {	CHECK_PERSONAL	= 1,
	CHECK_BUSINESS	= 2,
	CHECK_UNKNOWN	= 153
    } 	CheckTypes;

typedef 
enum CountryCodes
    {	CC_US	= 1,
	CC_CAN	= 2,
	CC_MEX	= 3,
	CC_UNK	= 153
    } 	CountryCodes;



extern RPC_IF_HANDLE __MIDL_itf__tbControls_0754_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf__tbControls_0754_v0_0_s_ifspec;

#ifndef __IMICRObj_INTERFACE_DEFINED__
#define __IMICRObj_INTERFACE_DEFINED__

/* interface IMICRObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IMICRObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C6F93406-B84F-45D3-BFB2-BB422C48D68F")
    IMICRObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TransitNumber( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TransitNumber( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AccountNumber( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AccountNumber( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BankNumber( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BankNumber( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SerialNumber( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SerialNumber( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CheckType( 
            /* [retval][out] */ CheckTypes *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CheckType( 
            /* [in] */ CheckTypes newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CountryCode( 
            /* [retval][out] */ CountryCodes *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CountryCode( 
            /* [in] */ CountryCodes newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Data( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Data( 
            /* [in] */ BSTR newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMICRObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMICRObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMICRObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMICRObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMICRObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMICRObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMICRObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMICRObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TransitNumber )( 
            IMICRObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TransitNumber )( 
            IMICRObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AccountNumber )( 
            IMICRObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AccountNumber )( 
            IMICRObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BankNumber )( 
            IMICRObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BankNumber )( 
            IMICRObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SerialNumber )( 
            IMICRObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SerialNumber )( 
            IMICRObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CheckType )( 
            IMICRObj * This,
            /* [retval][out] */ CheckTypes *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CheckType )( 
            IMICRObj * This,
            /* [in] */ CheckTypes newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CountryCode )( 
            IMICRObj * This,
            /* [retval][out] */ CountryCodes *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CountryCode )( 
            IMICRObj * This,
            /* [in] */ CountryCodes newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Data )( 
            IMICRObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Data )( 
            IMICRObj * This,
            /* [in] */ BSTR newVal);
        
        END_INTERFACE
    } IMICRObjVtbl;

    interface IMICRObj
    {
        CONST_VTBL struct IMICRObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMICRObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMICRObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMICRObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMICRObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMICRObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMICRObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMICRObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMICRObj_get_TransitNumber(This,pVal)	\
    (This)->lpVtbl -> get_TransitNumber(This,pVal)

#define IMICRObj_put_TransitNumber(This,newVal)	\
    (This)->lpVtbl -> put_TransitNumber(This,newVal)

#define IMICRObj_get_AccountNumber(This,pVal)	\
    (This)->lpVtbl -> get_AccountNumber(This,pVal)

#define IMICRObj_put_AccountNumber(This,newVal)	\
    (This)->lpVtbl -> put_AccountNumber(This,newVal)

#define IMICRObj_get_BankNumber(This,pVal)	\
    (This)->lpVtbl -> get_BankNumber(This,pVal)

#define IMICRObj_put_BankNumber(This,newVal)	\
    (This)->lpVtbl -> put_BankNumber(This,newVal)

#define IMICRObj_get_SerialNumber(This,pVal)	\
    (This)->lpVtbl -> get_SerialNumber(This,pVal)

#define IMICRObj_put_SerialNumber(This,newVal)	\
    (This)->lpVtbl -> put_SerialNumber(This,newVal)

#define IMICRObj_get_CheckType(This,pVal)	\
    (This)->lpVtbl -> get_CheckType(This,pVal)

#define IMICRObj_put_CheckType(This,newVal)	\
    (This)->lpVtbl -> put_CheckType(This,newVal)

#define IMICRObj_get_CountryCode(This,pVal)	\
    (This)->lpVtbl -> get_CountryCode(This,pVal)

#define IMICRObj_put_CountryCode(This,newVal)	\
    (This)->lpVtbl -> put_CountryCode(This,newVal)

#define IMICRObj_get_Data(This,pVal)	\
    (This)->lpVtbl -> get_Data(This,pVal)

#define IMICRObj_put_Data(This,newVal)	\
    (This)->lpVtbl -> put_Data(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMICRObj_get_TransitNumber_Proxy( 
    IMICRObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IMICRObj_get_TransitNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMICRObj_put_TransitNumber_Proxy( 
    IMICRObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IMICRObj_put_TransitNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMICRObj_get_AccountNumber_Proxy( 
    IMICRObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IMICRObj_get_AccountNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMICRObj_put_AccountNumber_Proxy( 
    IMICRObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IMICRObj_put_AccountNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMICRObj_get_BankNumber_Proxy( 
    IMICRObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IMICRObj_get_BankNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMICRObj_put_BankNumber_Proxy( 
    IMICRObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IMICRObj_put_BankNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMICRObj_get_SerialNumber_Proxy( 
    IMICRObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IMICRObj_get_SerialNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMICRObj_put_SerialNumber_Proxy( 
    IMICRObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IMICRObj_put_SerialNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMICRObj_get_CheckType_Proxy( 
    IMICRObj * This,
    /* [retval][out] */ CheckTypes *pVal);


void __RPC_STUB IMICRObj_get_CheckType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMICRObj_put_CheckType_Proxy( 
    IMICRObj * This,
    /* [in] */ CheckTypes newVal);


void __RPC_STUB IMICRObj_put_CheckType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMICRObj_get_CountryCode_Proxy( 
    IMICRObj * This,
    /* [retval][out] */ CountryCodes *pVal);


void __RPC_STUB IMICRObj_get_CountryCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMICRObj_put_CountryCode_Proxy( 
    IMICRObj * This,
    /* [in] */ CountryCodes newVal);


void __RPC_STUB IMICRObj_put_CountryCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMICRObj_get_Data_Proxy( 
    IMICRObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IMICRObj_get_Data_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMICRObj_put_Data_Proxy( 
    IMICRObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IMICRObj_put_Data_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMICRObj_INTERFACE_DEFINED__ */


#ifndef __IPrinterObj_INTERFACE_DEFINED__
#define __IPrinterObj_INTERFACE_DEFINED__

/* interface IPrinterObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPrinterObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("098D843A-FDDC-4708-9174-23134176A419")
    IPrinterObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Function( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Function( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Arguments( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Arguments( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Height( 
            /* [in] */ SHORT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Width( 
            /* [in] */ SHORT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Alignment( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Alignment( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TextPosition( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TextPosition( 
            /* [in] */ SHORT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_StationId( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_StationId( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Data( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Data( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BarcodeType( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BarcodeType( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BarcodeCharacterPrint( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BarcodeCharacterPrint( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BitmapLogoNumber( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BitmapLogoNumber( 
            /* [in] */ BYTE newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPrinterObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPrinterObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPrinterObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPrinterObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPrinterObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPrinterObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPrinterObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPrinterObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Function )( 
            IPrinterObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Function )( 
            IPrinterObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Arguments )( 
            IPrinterObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Arguments )( 
            IPrinterObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IPrinterObj * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IPrinterObj * This,
            /* [in] */ SHORT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IPrinterObj * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IPrinterObj * This,
            /* [in] */ SHORT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Alignment )( 
            IPrinterObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Alignment )( 
            IPrinterObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TextPosition )( 
            IPrinterObj * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TextPosition )( 
            IPrinterObj * This,
            /* [in] */ SHORT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_StationId )( 
            IPrinterObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_StationId )( 
            IPrinterObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Data )( 
            IPrinterObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Data )( 
            IPrinterObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BarcodeType )( 
            IPrinterObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BarcodeType )( 
            IPrinterObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BarcodeCharacterPrint )( 
            IPrinterObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BarcodeCharacterPrint )( 
            IPrinterObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BitmapLogoNumber )( 
            IPrinterObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BitmapLogoNumber )( 
            IPrinterObj * This,
            /* [in] */ BYTE newVal);
        
        END_INTERFACE
    } IPrinterObjVtbl;

    interface IPrinterObj
    {
        CONST_VTBL struct IPrinterObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPrinterObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPrinterObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPrinterObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPrinterObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPrinterObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPrinterObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPrinterObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPrinterObj_get_Function(This,pVal)	\
    (This)->lpVtbl -> get_Function(This,pVal)

#define IPrinterObj_put_Function(This,newVal)	\
    (This)->lpVtbl -> put_Function(This,newVal)

#define IPrinterObj_get_Arguments(This,pVal)	\
    (This)->lpVtbl -> get_Arguments(This,pVal)

#define IPrinterObj_put_Arguments(This,newVal)	\
    (This)->lpVtbl -> put_Arguments(This,newVal)

#define IPrinterObj_get_Height(This,pVal)	\
    (This)->lpVtbl -> get_Height(This,pVal)

#define IPrinterObj_put_Height(This,newVal)	\
    (This)->lpVtbl -> put_Height(This,newVal)

#define IPrinterObj_get_Width(This,pVal)	\
    (This)->lpVtbl -> get_Width(This,pVal)

#define IPrinterObj_put_Width(This,newVal)	\
    (This)->lpVtbl -> put_Width(This,newVal)

#define IPrinterObj_get_Alignment(This,pVal)	\
    (This)->lpVtbl -> get_Alignment(This,pVal)

#define IPrinterObj_put_Alignment(This,newVal)	\
    (This)->lpVtbl -> put_Alignment(This,newVal)

#define IPrinterObj_get_TextPosition(This,pVal)	\
    (This)->lpVtbl -> get_TextPosition(This,pVal)

#define IPrinterObj_put_TextPosition(This,newVal)	\
    (This)->lpVtbl -> put_TextPosition(This,newVal)

#define IPrinterObj_get_StationId(This,pVal)	\
    (This)->lpVtbl -> get_StationId(This,pVal)

#define IPrinterObj_put_StationId(This,newVal)	\
    (This)->lpVtbl -> put_StationId(This,newVal)

#define IPrinterObj_get_Data(This,pVal)	\
    (This)->lpVtbl -> get_Data(This,pVal)

#define IPrinterObj_put_Data(This,newVal)	\
    (This)->lpVtbl -> put_Data(This,newVal)

#define IPrinterObj_get_BarcodeType(This,pVal)	\
    (This)->lpVtbl -> get_BarcodeType(This,pVal)

#define IPrinterObj_put_BarcodeType(This,newVal)	\
    (This)->lpVtbl -> put_BarcodeType(This,newVal)

#define IPrinterObj_get_BarcodeCharacterPrint(This,pVal)	\
    (This)->lpVtbl -> get_BarcodeCharacterPrint(This,pVal)

#define IPrinterObj_put_BarcodeCharacterPrint(This,newVal)	\
    (This)->lpVtbl -> put_BarcodeCharacterPrint(This,newVal)

#define IPrinterObj_get_BitmapLogoNumber(This,pVal)	\
    (This)->lpVtbl -> get_BitmapLogoNumber(This,pVal)

#define IPrinterObj_put_BitmapLogoNumber(This,newVal)	\
    (This)->lpVtbl -> put_BitmapLogoNumber(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPrinterObj_get_Function_Proxy( 
    IPrinterObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IPrinterObj_get_Function_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPrinterObj_put_Function_Proxy( 
    IPrinterObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IPrinterObj_put_Function_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPrinterObj_get_Arguments_Proxy( 
    IPrinterObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IPrinterObj_get_Arguments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPrinterObj_put_Arguments_Proxy( 
    IPrinterObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IPrinterObj_put_Arguments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPrinterObj_get_Height_Proxy( 
    IPrinterObj * This,
    /* [retval][out] */ SHORT *pVal);


void __RPC_STUB IPrinterObj_get_Height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPrinterObj_put_Height_Proxy( 
    IPrinterObj * This,
    /* [in] */ SHORT newVal);


void __RPC_STUB IPrinterObj_put_Height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPrinterObj_get_Width_Proxy( 
    IPrinterObj * This,
    /* [retval][out] */ SHORT *pVal);


void __RPC_STUB IPrinterObj_get_Width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPrinterObj_put_Width_Proxy( 
    IPrinterObj * This,
    /* [in] */ SHORT newVal);


void __RPC_STUB IPrinterObj_put_Width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPrinterObj_get_Alignment_Proxy( 
    IPrinterObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IPrinterObj_get_Alignment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPrinterObj_put_Alignment_Proxy( 
    IPrinterObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IPrinterObj_put_Alignment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPrinterObj_get_TextPosition_Proxy( 
    IPrinterObj * This,
    /* [retval][out] */ SHORT *pVal);


void __RPC_STUB IPrinterObj_get_TextPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPrinterObj_put_TextPosition_Proxy( 
    IPrinterObj * This,
    /* [in] */ SHORT newVal);


void __RPC_STUB IPrinterObj_put_TextPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPrinterObj_get_StationId_Proxy( 
    IPrinterObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IPrinterObj_get_StationId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPrinterObj_put_StationId_Proxy( 
    IPrinterObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IPrinterObj_put_StationId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPrinterObj_get_Data_Proxy( 
    IPrinterObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IPrinterObj_get_Data_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPrinterObj_put_Data_Proxy( 
    IPrinterObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IPrinterObj_put_Data_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPrinterObj_get_BarcodeType_Proxy( 
    IPrinterObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IPrinterObj_get_BarcodeType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPrinterObj_put_BarcodeType_Proxy( 
    IPrinterObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IPrinterObj_put_BarcodeType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPrinterObj_get_BarcodeCharacterPrint_Proxy( 
    IPrinterObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IPrinterObj_get_BarcodeCharacterPrint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPrinterObj_put_BarcodeCharacterPrint_Proxy( 
    IPrinterObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IPrinterObj_put_BarcodeCharacterPrint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPrinterObj_get_BitmapLogoNumber_Proxy( 
    IPrinterObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB IPrinterObj_get_BitmapLogoNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPrinterObj_put_BitmapLogoNumber_Proxy( 
    IPrinterObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB IPrinterObj_put_BitmapLogoNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPrinterObj_INTERFACE_DEFINED__ */


/* interface __MIDL_itf__tbControls_0756 */
/* [local] */ 

typedef 
enum TransactionTypes
    {	TT_CREDIT	= 2,
	TT_DEBIT	= 3,
	TT_LOYALTY	= 4,
	TT_DEBIT_SAVINGS	= 5,
	TT_EBT	= 7,
	TT_GIFT	= 9,
	TT_UNDEFINED	= 153
    } 	TransactionTypes;

typedef 
enum PinEncodingList
    {	PE_MASTER_SESSION	= 0,
	PE_DUKPT	= 1,
	PE_UNDEFINED	= 153
    } 	PinEncodingList;

typedef 
enum PinBlockFormats
    {	PB_ANSI	= 0,
	PB_DIEBOLD	= 1,
	PB_UNDEFINED	= 153
    } 	PinBlockFormats;



extern RPC_IF_HANDLE __MIDL_itf__tbControls_0756_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf__tbControls_0756_v0_0_s_ifspec;

#ifndef __IPinpadObj_INTERFACE_DEFINED__
#define __IPinpadObj_INTERFACE_DEFINED__

/* interface IPinpadObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPinpadObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1A737844-D845-4E97-AD01-A3FFBAD13839")
    IPinpadObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PrimaryAccountNumber( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PrimaryAccountNumber( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TransactionType( 
            /* [retval][out] */ TransactionTypes *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TransactionType( 
            /* [in] */ TransactionTypes newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PinEncoding( 
            /* [retval][out] */ PinEncodingList *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PinEncoding( 
            /* [in] */ PinEncodingList newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPinpadObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPinpadObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPinpadObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPinpadObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPinpadObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPinpadObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPinpadObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPinpadObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PrimaryAccountNumber )( 
            IPinpadObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PrimaryAccountNumber )( 
            IPinpadObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TransactionType )( 
            IPinpadObj * This,
            /* [retval][out] */ TransactionTypes *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TransactionType )( 
            IPinpadObj * This,
            /* [in] */ TransactionTypes newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PinEncoding )( 
            IPinpadObj * This,
            /* [retval][out] */ PinEncodingList *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PinEncoding )( 
            IPinpadObj * This,
            /* [in] */ PinEncodingList newVal);
        
        END_INTERFACE
    } IPinpadObjVtbl;

    interface IPinpadObj
    {
        CONST_VTBL struct IPinpadObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPinpadObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPinpadObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPinpadObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPinpadObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPinpadObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPinpadObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPinpadObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPinpadObj_get_PrimaryAccountNumber(This,pVal)	\
    (This)->lpVtbl -> get_PrimaryAccountNumber(This,pVal)

#define IPinpadObj_put_PrimaryAccountNumber(This,newVal)	\
    (This)->lpVtbl -> put_PrimaryAccountNumber(This,newVal)

#define IPinpadObj_get_TransactionType(This,pVal)	\
    (This)->lpVtbl -> get_TransactionType(This,pVal)

#define IPinpadObj_put_TransactionType(This,newVal)	\
    (This)->lpVtbl -> put_TransactionType(This,newVal)

#define IPinpadObj_get_PinEncoding(This,pVal)	\
    (This)->lpVtbl -> get_PinEncoding(This,pVal)

#define IPinpadObj_put_PinEncoding(This,newVal)	\
    (This)->lpVtbl -> put_PinEncoding(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPinpadObj_get_PrimaryAccountNumber_Proxy( 
    IPinpadObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IPinpadObj_get_PrimaryAccountNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPinpadObj_put_PrimaryAccountNumber_Proxy( 
    IPinpadObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IPinpadObj_put_PrimaryAccountNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPinpadObj_get_TransactionType_Proxy( 
    IPinpadObj * This,
    /* [retval][out] */ TransactionTypes *pVal);


void __RPC_STUB IPinpadObj_get_TransactionType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPinpadObj_put_TransactionType_Proxy( 
    IPinpadObj * This,
    /* [in] */ TransactionTypes newVal);


void __RPC_STUB IPinpadObj_put_TransactionType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPinpadObj_get_PinEncoding_Proxy( 
    IPinpadObj * This,
    /* [retval][out] */ PinEncodingList *pVal);


void __RPC_STUB IPinpadObj_get_PinEncoding_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPinpadObj_put_PinEncoding_Proxy( 
    IPinpadObj * This,
    /* [in] */ PinEncodingList newVal);


void __RPC_STUB IPinpadObj_put_PinEncoding_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPinpadObj_INTERFACE_DEFINED__ */


#ifndef __IScaleObj_INTERFACE_DEFINED__
#define __IScaleObj_INTERFACE_DEFINED__

/* interface IScaleObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IScaleObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EA8C9BF5-9DBF-486E-8C16-3673E72D4B1C")
    IScaleObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Weight( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Weight( 
            /* [in] */ LONG newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IScaleObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IScaleObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IScaleObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IScaleObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IScaleObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IScaleObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IScaleObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IScaleObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Weight )( 
            IScaleObj * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Weight )( 
            IScaleObj * This,
            /* [in] */ LONG newVal);
        
        END_INTERFACE
    } IScaleObjVtbl;

    interface IScaleObj
    {
        CONST_VTBL struct IScaleObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IScaleObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IScaleObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IScaleObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IScaleObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IScaleObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IScaleObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IScaleObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IScaleObj_get_Weight(This,pVal)	\
    (This)->lpVtbl -> get_Weight(This,pVal)

#define IScaleObj_put_Weight(This,newVal)	\
    (This)->lpVtbl -> put_Weight(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IScaleObj_get_Weight_Proxy( 
    IScaleObj * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB IScaleObj_get_Weight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IScaleObj_put_Weight_Proxy( 
    IScaleObj * This,
    /* [in] */ LONG newVal);


void __RPC_STUB IScaleObj_put_Weight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IScaleObj_INTERFACE_DEFINED__ */


#ifndef __ICustomObj_INTERFACE_DEFINED__
#define __ICustomObj_INTERFACE_DEFINED__

/* interface ICustomObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICustomObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("94ED1368-53C2-44FA-8641-951D31DEE2B5")
    ICustomObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CustomerData( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CustomerData( 
            /* [in] */ VARIANT newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICustomObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICustomObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICustomObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICustomObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICustomObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICustomObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICustomObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICustomObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CustomerData )( 
            ICustomObj * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CustomerData )( 
            ICustomObj * This,
            /* [in] */ VARIANT newVal);
        
        END_INTERFACE
    } ICustomObjVtbl;

    interface ICustomObj
    {
        CONST_VTBL struct ICustomObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICustomObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICustomObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICustomObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICustomObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICustomObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICustomObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICustomObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICustomObj_get_CustomerData(This,pVal)	\
    (This)->lpVtbl -> get_CustomerData(This,pVal)

#define ICustomObj_put_CustomerData(This,newVal)	\
    (This)->lpVtbl -> put_CustomerData(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ICustomObj_get_CustomerData_Proxy( 
    ICustomObj * This,
    /* [retval][out] */ VARIANT *pVal);


void __RPC_STUB ICustomObj_get_CustomerData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ICustomObj_put_CustomerData_Proxy( 
    ICustomObj * This,
    /* [in] */ VARIANT newVal);


void __RPC_STUB ICustomObj_put_CustomerData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICustomObj_INTERFACE_DEFINED__ */


/* interface __MIDL_itf__tbControls_0759 */
/* [local] */ 

typedef 
enum ResetOptions
    {	RESET_FIELD_DATA	= 0,
	RESET_XML_DOC	= 1
    } 	ResetOptions;



extern RPC_IF_HANDLE __MIDL_itf__tbControls_0759_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf__tbControls_0759_v0_0_s_ifspec;

#ifndef __ITBGenObj_INTERFACE_DEFINED__
#define __ITBGenObj_INTERFACE_DEFINED__

/* interface ITBGenObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITBGenObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5D8CF3FE-492B-445E-A6D9-B673BC610D55")
    ITBGenObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetValue( 
            /* [in] */ BSTR Name,
            /* [in] */ VARIANT vValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetValue( 
            /* [in] */ BSTR Name,
            /* [retval][out] */ VARIANT *vValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Reset( 
            /* [in] */ ResetOptions option) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_xml( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_xml( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MsgId( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MsgId( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsXMLOutput( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IsXMLOutput( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Validate( 
            /* [retval][out] */ BYTE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Validate( 
            /* [in] */ BYTE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetValue2( 
            /* [in] */ BSTR Name,
            /* [retval][out] */ VARIANT *vValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITBGenObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITBGenObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITBGenObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITBGenObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITBGenObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITBGenObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITBGenObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITBGenObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetValue )( 
            ITBGenObj * This,
            /* [in] */ BSTR Name,
            /* [in] */ VARIANT vValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetValue )( 
            ITBGenObj * This,
            /* [in] */ BSTR Name,
            /* [retval][out] */ VARIANT *vValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Reset )( 
            ITBGenObj * This,
            /* [in] */ ResetOptions option);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_xml )( 
            ITBGenObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_xml )( 
            ITBGenObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MsgId )( 
            ITBGenObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MsgId )( 
            ITBGenObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsXMLOutput )( 
            ITBGenObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsXMLOutput )( 
            ITBGenObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Validate )( 
            ITBGenObj * This,
            /* [retval][out] */ BYTE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Validate )( 
            ITBGenObj * This,
            /* [in] */ BYTE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            ITBGenObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            ITBGenObj * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetValue2 )( 
            ITBGenObj * This,
            /* [in] */ BSTR Name,
            /* [retval][out] */ VARIANT *vValue);
        
        END_INTERFACE
    } ITBGenObjVtbl;

    interface ITBGenObj
    {
        CONST_VTBL struct ITBGenObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITBGenObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITBGenObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITBGenObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITBGenObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITBGenObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITBGenObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITBGenObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITBGenObj_SetValue(This,Name,vValue)	\
    (This)->lpVtbl -> SetValue(This,Name,vValue)

#define ITBGenObj_GetValue(This,Name,vValue)	\
    (This)->lpVtbl -> GetValue(This,Name,vValue)

#define ITBGenObj_Reset(This,option)	\
    (This)->lpVtbl -> Reset(This,option)

#define ITBGenObj_get_xml(This,pVal)	\
    (This)->lpVtbl -> get_xml(This,pVal)

#define ITBGenObj_put_xml(This,newVal)	\
    (This)->lpVtbl -> put_xml(This,newVal)

#define ITBGenObj_get_MsgId(This,pVal)	\
    (This)->lpVtbl -> get_MsgId(This,pVal)

#define ITBGenObj_put_MsgId(This,newVal)	\
    (This)->lpVtbl -> put_MsgId(This,newVal)

#define ITBGenObj_get_IsXMLOutput(This,pVal)	\
    (This)->lpVtbl -> get_IsXMLOutput(This,pVal)

#define ITBGenObj_put_IsXMLOutput(This,newVal)	\
    (This)->lpVtbl -> put_IsXMLOutput(This,newVal)

#define ITBGenObj_get_Validate(This,pVal)	\
    (This)->lpVtbl -> get_Validate(This,pVal)

#define ITBGenObj_put_Validate(This,newVal)	\
    (This)->lpVtbl -> put_Validate(This,newVal)

#define ITBGenObj_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define ITBGenObj_put_Name(This,newVal)	\
    (This)->lpVtbl -> put_Name(This,newVal)

#define ITBGenObj_GetValue2(This,Name,vValue)	\
    (This)->lpVtbl -> GetValue2(This,Name,vValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITBGenObj_SetValue_Proxy( 
    ITBGenObj * This,
    /* [in] */ BSTR Name,
    /* [in] */ VARIANT vValue);


void __RPC_STUB ITBGenObj_SetValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITBGenObj_GetValue_Proxy( 
    ITBGenObj * This,
    /* [in] */ BSTR Name,
    /* [retval][out] */ VARIANT *vValue);


void __RPC_STUB ITBGenObj_GetValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITBGenObj_Reset_Proxy( 
    ITBGenObj * This,
    /* [in] */ ResetOptions option);


void __RPC_STUB ITBGenObj_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITBGenObj_get_xml_Proxy( 
    ITBGenObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ITBGenObj_get_xml_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITBGenObj_put_xml_Proxy( 
    ITBGenObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITBGenObj_put_xml_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITBGenObj_get_MsgId_Proxy( 
    ITBGenObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB ITBGenObj_get_MsgId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITBGenObj_put_MsgId_Proxy( 
    ITBGenObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB ITBGenObj_put_MsgId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITBGenObj_get_IsXMLOutput_Proxy( 
    ITBGenObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB ITBGenObj_get_IsXMLOutput_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITBGenObj_put_IsXMLOutput_Proxy( 
    ITBGenObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB ITBGenObj_put_IsXMLOutput_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITBGenObj_get_Validate_Proxy( 
    ITBGenObj * This,
    /* [retval][out] */ BYTE *pVal);


void __RPC_STUB ITBGenObj_get_Validate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITBGenObj_put_Validate_Proxy( 
    ITBGenObj * This,
    /* [in] */ BYTE newVal);


void __RPC_STUB ITBGenObj_put_Validate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITBGenObj_get_Name_Proxy( 
    ITBGenObj * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ITBGenObj_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITBGenObj_put_Name_Proxy( 
    ITBGenObj * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ITBGenObj_put_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITBGenObj_GetValue2_Proxy( 
    ITBGenObj * This,
    /* [in] */ BSTR Name,
    /* [retval][out] */ VARIANT *vValue);


void __RPC_STUB ITBGenObj_GetValue2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITBGenObj_INTERFACE_DEFINED__ */


/* interface __MIDL_itf__tbControls_0760 */
/* [local] */ 

typedef 
enum tbConnectReceiveEvent
    {	ReceiveEvent	= 0,
	ReceiveItemObjEvent	= 1,
	ReceiveCashDrawerObjEvent	= 2,
	ReceiveTotalObjEvent	= 3,
	ReceiveTenderObjEvent	= 4,
	ReceiveRewardObjEvent	= 5,
	ReceiveScannerObjEvent	= 6,
	ReceiveKeyboardObjEvent	= 7,
	ReceiveKeylockObjEvent	= 8,
	ReceiveMSRObjEvent	= 9,
	ReceiveMICRObjEvent	= 10,
	ReceivePrinterObjEvent	= 11,
	ReceivePinpadObjEvent	= 12,
	ReceiveScaleObjEvent	= 13,
	ReceiveCustomObjEvent	= 14,
	ReceiveTBGenObjEvent	= 15
    } 	tbConnectReceiveEvent;



extern RPC_IF_HANDLE __MIDL_itf__tbControls_0760_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf__tbControls_0760_v0_0_s_ifspec;

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
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Send( 
            /* [in] */ ITBStream *pObj) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ConnectAsTB( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetAck( 
            /* [in] */ LONG nAck) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SubscribeTo( 
            /* [in] */ BSTR szCommaDelimHexDeviceIds) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Disconnect( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCallback( 
            /* [in] */ VARIANT vCallbackProc) = 0;
        
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
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Send )( 
            ItbConnect * This,
            /* [in] */ ITBStream *pObj);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ConnectAsTB )( 
            ItbConnect * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetAck )( 
            ItbConnect * This,
            /* [in] */ LONG nAck);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SubscribeTo )( 
            ItbConnect * This,
            /* [in] */ BSTR szCommaDelimHexDeviceIds);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            ItbConnect * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCallback )( 
            ItbConnect * This,
            /* [in] */ VARIANT vCallbackProc);
        
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

#define ItbConnect_Send(This,pObj)	\
    (This)->lpVtbl -> Send(This,pObj)

#define ItbConnect_ConnectAsTB(This)	\
    (This)->lpVtbl -> ConnectAsTB(This)

#define ItbConnect_SetAck(This,nAck)	\
    (This)->lpVtbl -> SetAck(This,nAck)

#define ItbConnect_SubscribeTo(This,szCommaDelimHexDeviceIds)	\
    (This)->lpVtbl -> SubscribeTo(This,szCommaDelimHexDeviceIds)

#define ItbConnect_Disconnect(This)	\
    (This)->lpVtbl -> Disconnect(This)

#define ItbConnect_SetCallback(This,vCallbackProc)	\
    (This)->lpVtbl -> SetCallback(This,vCallbackProc)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE ItbConnect_Connect_Proxy( 
    ItbConnect * This);


void __RPC_STUB ItbConnect_Connect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ItbConnect_Send_Proxy( 
    ItbConnect * This,
    /* [in] */ ITBStream *pObj);


void __RPC_STUB ItbConnect_Send_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ItbConnect_ConnectAsTB_Proxy( 
    ItbConnect * This);


void __RPC_STUB ItbConnect_ConnectAsTB_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ItbConnect_SetAck_Proxy( 
    ItbConnect * This,
    /* [in] */ LONG nAck);


void __RPC_STUB ItbConnect_SetAck_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ItbConnect_SubscribeTo_Proxy( 
    ItbConnect * This,
    /* [in] */ BSTR szCommaDelimHexDeviceIds);


void __RPC_STUB ItbConnect_SubscribeTo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ItbConnect_Disconnect_Proxy( 
    ItbConnect * This);


void __RPC_STUB ItbConnect_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ItbConnect_SetCallback_Proxy( 
    ItbConnect * This,
    /* [in] */ VARIANT vCallbackProc);


void __RPC_STUB ItbConnect_SetCallback_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ItbConnect_INTERFACE_DEFINED__ */


#ifndef __IWindowMsg_INTERFACE_DEFINED__
#define __IWindowMsg_INTERFACE_DEFINED__

/* interface IWindowMsg */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWindowMsg;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("13979CBF-F2A8-471D-BF62-72E7A19B88C7")
    IWindowMsg : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IWindowMsgVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWindowMsg * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWindowMsg * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWindowMsg * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWindowMsg * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWindowMsg * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWindowMsg * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWindowMsg * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IWindowMsgVtbl;

    interface IWindowMsg
    {
        CONST_VTBL struct IWindowMsgVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWindowMsg_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWindowMsg_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWindowMsg_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWindowMsg_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWindowMsg_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWindowMsg_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWindowMsg_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWindowMsg_INTERFACE_DEFINED__ */



#ifndef __tbControls_LIBRARY_DEFINED__
#define __tbControls_LIBRARY_DEFINED__

/* library tbControls */
/* [helpstring][uuid][version] */ 


EXTERN_C const IID LIBID_tbControls;

EXTERN_C const CLSID CLSID_CItemObj;

#ifdef __cplusplus

class DECLSPEC_UUID("C8A5701C-082A-4E05-A4B2-E53E08BC0EFB")
CItemObj;
#endif

EXTERN_C const CLSID CLSID_CCashDrawerObj;

#ifdef __cplusplus

class DECLSPEC_UUID("FF5BEFA5-8E50-4528-8F04-3340D66E6F80")
CCashDrawerObj;
#endif

EXTERN_C const CLSID CLSID_CTotalObj;

#ifdef __cplusplus

class DECLSPEC_UUID("E1354D9A-EBFF-479C-9EA2-2C41D6C2BB1E")
CTotalObj;
#endif

EXTERN_C const CLSID CLSID_CTenderObj;

#ifdef __cplusplus

class DECLSPEC_UUID("79D27133-06DF-46F9-8B5D-526D85332082")
CTenderObj;
#endif

EXTERN_C const CLSID CLSID_CRewardObj;

#ifdef __cplusplus

class DECLSPEC_UUID("CC493BBE-BBB2-4E5F-92E7-83170080B195")
CRewardObj;
#endif

EXTERN_C const CLSID CLSID_CScannerObj;

#ifdef __cplusplus

class DECLSPEC_UUID("2735E4A9-F209-4534-B7B9-E60D7D663A02")
CScannerObj;
#endif

EXTERN_C const CLSID CLSID_CKeylockObj;

#ifdef __cplusplus

class DECLSPEC_UUID("23439E29-A334-434B-963D-1AEC4A5A8A0B")
CKeylockObj;
#endif

EXTERN_C const CLSID CLSID_CKeyboardObj;

#ifdef __cplusplus

class DECLSPEC_UUID("080D4835-9415-4279-B2B7-8DC4E716AB0D")
CKeyboardObj;
#endif

EXTERN_C const CLSID CLSID_CMSRObj;

#ifdef __cplusplus

class DECLSPEC_UUID("155E348A-B10F-4DFB-90B6-4626518C9136")
CMSRObj;
#endif

EXTERN_C const CLSID CLSID_CMICRObj;

#ifdef __cplusplus

class DECLSPEC_UUID("B5CF4A4A-4EB0-402B-BA77-7CB8E13DEEC4")
CMICRObj;
#endif

EXTERN_C const CLSID CLSID_CPrinterObj;

#ifdef __cplusplus

class DECLSPEC_UUID("C2D1E10E-3DB1-4FE7-8160-FBEAB907B533")
CPrinterObj;
#endif

EXTERN_C const CLSID CLSID_CPinpadObj;

#ifdef __cplusplus

class DECLSPEC_UUID("AD4AE7F5-390C-4916-AC73-224FFBF7AC63")
CPinpadObj;
#endif

EXTERN_C const CLSID CLSID_CScaleObj;

#ifdef __cplusplus

class DECLSPEC_UUID("FB06D951-F350-413B-81CA-F613D234F339")
CScaleObj;
#endif

EXTERN_C const CLSID CLSID_CCustomObj;

#ifdef __cplusplus

class DECLSPEC_UUID("CF2B0DA9-5461-4544-8494-001C909E095C")
CCustomObj;
#endif

EXTERN_C const CLSID CLSID_CTBGenObj;

#ifdef __cplusplus

class DECLSPEC_UUID("E3052DB2-D582-4A89-A354-AD9705641442")
CTBGenObj;
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

EXTERN_C const CLSID CLSID_CWindowMsg;

#ifdef __cplusplus

class DECLSPEC_UUID("8BA51575-F22F-426D-A4D2-5D3301F086F4")
CWindowMsg;
#endif
#endif /* __tbControls_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


