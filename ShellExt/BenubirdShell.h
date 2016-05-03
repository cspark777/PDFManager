

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue Dec 18 17:25:25 2012
 */
/* Compiler settings for BenubirdShell.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

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

#ifndef __BenubirdShell_h__
#define __BenubirdShell_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IBenubirdShellExt_FWD_DEFINED__
#define __IBenubirdShellExt_FWD_DEFINED__
typedef interface IBenubirdShellExt IBenubirdShellExt;
#endif 	/* __IBenubirdShellExt_FWD_DEFINED__ */


#ifndef __BenubirdShellExt_FWD_DEFINED__
#define __BenubirdShellExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class BenubirdShellExt BenubirdShellExt;
#else
typedef struct BenubirdShellExt BenubirdShellExt;
#endif /* __cplusplus */

#endif 	/* __BenubirdShellExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IBenubirdShellExt_INTERFACE_DEFINED__
#define __IBenubirdShellExt_INTERFACE_DEFINED__

/* interface IBenubirdShellExt */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IBenubirdShellExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4AE8624C-1C5D-4A8D-88E9-A8F1C0076472")
    IBenubirdShellExt : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IBenubirdShellExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBenubirdShellExt * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBenubirdShellExt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBenubirdShellExt * This);
        
        END_INTERFACE
    } IBenubirdShellExtVtbl;

    interface IBenubirdShellExt
    {
        CONST_VTBL struct IBenubirdShellExtVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBenubirdShellExt_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBenubirdShellExt_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBenubirdShellExt_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBenubirdShellExt_INTERFACE_DEFINED__ */



#ifndef __BENUBIRDSHELLLib_LIBRARY_DEFINED__
#define __BENUBIRDSHELLLib_LIBRARY_DEFINED__

/* library BENUBIRDSHELLLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_BENUBIRDSHELLLib;

EXTERN_C const CLSID CLSID_BenubirdShellExt;

#ifdef __cplusplus

class DECLSPEC_UUID("3A1530BA-0C6D-46E3-B04D-E25494A05750")
BenubirdShellExt;
#endif
#endif /* __BENUBIRDSHELLLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


