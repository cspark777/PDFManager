

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue Dec 18 17:25:26 2012
 */
/* Compiler settings for Benubird.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __Benubird_h_h__
#define __Benubird_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IBenubird_FWD_DEFINED__
#define __IBenubird_FWD_DEFINED__
typedef interface IBenubird IBenubird;
#endif 	/* __IBenubird_FWD_DEFINED__ */


#ifndef __Benubird_FWD_DEFINED__
#define __Benubird_FWD_DEFINED__

#ifdef __cplusplus
typedef class Benubird Benubird;
#else
typedef struct Benubird Benubird;
#endif /* __cplusplus */

#endif 	/* __Benubird_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Benubird_LIBRARY_DEFINED__
#define __Benubird_LIBRARY_DEFINED__

/* library Benubird */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_Benubird;

#ifndef __IBenubird_DISPINTERFACE_DEFINED__
#define __IBenubird_DISPINTERFACE_DEFINED__

/* dispinterface IBenubird */
/* [uuid] */ 


EXTERN_C const IID DIID_IBenubird;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7A558363-0BC9-4962-85CD-01E74CB64771")
    IBenubird : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IBenubirdVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBenubird * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBenubird * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBenubird * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBenubird * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBenubird * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBenubird * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBenubird * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IBenubirdVtbl;

    interface IBenubird
    {
        CONST_VTBL struct IBenubirdVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBenubird_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBenubird_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBenubird_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBenubird_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBenubird_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBenubird_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBenubird_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IBenubird_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Benubird;

#ifdef __cplusplus

class DECLSPEC_UUID("B1789690-8215-4669-A691-174D19A147A3")
Benubird;
#endif
#endif /* __Benubird_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


