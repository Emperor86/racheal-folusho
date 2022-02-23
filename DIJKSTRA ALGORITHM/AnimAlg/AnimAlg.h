/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Dec 22 12:48:37 2003
 */
/* Compiler settings for D:\NetSET 5.1\AnimAlg\AnimAlg.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
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

#ifndef __AnimAlg_h__
#define __AnimAlg_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IDijkstra_FWD_DEFINED__
#define __IDijkstra_FWD_DEFINED__
typedef interface IDijkstra IDijkstra;
#endif 	/* __IDijkstra_FWD_DEFINED__ */


#ifndef ___IDijkstraEvents_FWD_DEFINED__
#define ___IDijkstraEvents_FWD_DEFINED__
typedef interface _IDijkstraEvents _IDijkstraEvents;
#endif 	/* ___IDijkstraEvents_FWD_DEFINED__ */


#ifndef __Dijkstra_FWD_DEFINED__
#define __Dijkstra_FWD_DEFINED__

#ifdef __cplusplus
typedef class Dijkstra Dijkstra;
#else
typedef struct Dijkstra Dijkstra;
#endif /* __cplusplus */

#endif 	/* __Dijkstra_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IDijkstra_INTERFACE_DEFINED__
#define __IDijkstra_INTERFACE_DEFINED__

/* interface IDijkstra */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDijkstra;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5DDF46FF-095C-4A62-9BBF-B20A2966DC8A")
    IDijkstra : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ long nrvert,
            /* [in] */ short bidir) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartAddNodes( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartAddEdges( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ShortestPath( 
            /* [in] */ long node1,
            /* [in] */ long node2) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDijkstraVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDijkstra __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDijkstra __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDijkstra __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDijkstra __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDijkstra __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDijkstra __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDijkstra __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IDijkstra __RPC_FAR * This,
            /* [in] */ long nrvert,
            /* [in] */ short bidir);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartAddNodes )( 
            IDijkstra __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartAddEdges )( 
            IDijkstra __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShortestPath )( 
            IDijkstra __RPC_FAR * This,
            /* [in] */ long node1,
            /* [in] */ long node2);
        
        END_INTERFACE
    } IDijkstraVtbl;

    interface IDijkstra
    {
        CONST_VTBL struct IDijkstraVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDijkstra_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDijkstra_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDijkstra_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDijkstra_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDijkstra_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDijkstra_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDijkstra_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDijkstra_Initialize(This,nrvert,bidir)	\
    (This)->lpVtbl -> Initialize(This,nrvert,bidir)

#define IDijkstra_StartAddNodes(This)	\
    (This)->lpVtbl -> StartAddNodes(This)

#define IDijkstra_StartAddEdges(This)	\
    (This)->lpVtbl -> StartAddEdges(This)

#define IDijkstra_ShortestPath(This,node1,node2)	\
    (This)->lpVtbl -> ShortestPath(This,node1,node2)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDijkstra_Initialize_Proxy( 
    IDijkstra __RPC_FAR * This,
    /* [in] */ long nrvert,
    /* [in] */ short bidir);


void __RPC_STUB IDijkstra_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDijkstra_StartAddNodes_Proxy( 
    IDijkstra __RPC_FAR * This);


void __RPC_STUB IDijkstra_StartAddNodes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDijkstra_StartAddEdges_Proxy( 
    IDijkstra __RPC_FAR * This);


void __RPC_STUB IDijkstra_StartAddEdges_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDijkstra_ShortestPath_Proxy( 
    IDijkstra __RPC_FAR * This,
    /* [in] */ long node1,
    /* [in] */ long node2);


void __RPC_STUB IDijkstra_ShortestPath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDijkstra_INTERFACE_DEFINED__ */



#ifndef __ANIMALGLib_LIBRARY_DEFINED__
#define __ANIMALGLib_LIBRARY_DEFINED__

/* library ANIMALGLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ANIMALGLib;

#ifndef ___IDijkstraEvents_DISPINTERFACE_DEFINED__
#define ___IDijkstraEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IDijkstraEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IDijkstraEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D5A6541C-CEC7-4D54-8CF6-CDAE911830D5")
    _IDijkstraEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IDijkstraEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _IDijkstraEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _IDijkstraEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _IDijkstraEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _IDijkstraEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _IDijkstraEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _IDijkstraEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _IDijkstraEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _IDijkstraEventsVtbl;

    interface _IDijkstraEvents
    {
        CONST_VTBL struct _IDijkstraEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IDijkstraEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IDijkstraEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IDijkstraEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IDijkstraEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IDijkstraEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IDijkstraEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IDijkstraEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IDijkstraEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Dijkstra;

#ifdef __cplusplus

class DECLSPEC_UUID("6F02AF52-EC0B-4188-A452-3F1E39F1B347")
Dijkstra;
#endif
#endif /* __ANIMALGLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
