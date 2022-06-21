

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for testasyncoperation.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
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
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __testasyncoperation_h__
#define __testasyncoperation_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITestAsyncOperation_FWD_DEFINED__
#define __ITestAsyncOperation_FWD_DEFINED__
typedef interface ITestAsyncOperation ITestAsyncOperation;

#endif 	/* __ITestAsyncOperation_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "FabricCommon.h"

	/* C style interface */

    typedef struct ITestAsyncOperationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITestAsyncOperation * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITestAsyncOperation * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITestAsyncOperation * This);
        
        HRESULT ( STDMETHODCALLTYPE *BeginTestOperation )( 
            ITestAsyncOperation * This,
            int arg1,
            IFabricAsyncOperationCallback *callback,
            IFabricAsyncOperationContext **context);
        
        HRESULT ( STDMETHODCALLTYPE *EndTestOperation )( 
            ITestAsyncOperation * This,
            IFabricAsyncOperationContext *context,
            int *operation_result_1,
            double *operation_result_2);
        
        HRESULT ( STDMETHODCALLTYPE *BeginTestOperationWithNoBeginArgs )( 
            ITestAsyncOperation * This,
            IFabricAsyncOperationCallback *callback,
            IFabricAsyncOperationContext **context);
        
        HRESULT ( STDMETHODCALLTYPE *EndTestOperationWithNoBeginArgs )( 
            ITestAsyncOperation * This,
            IFabricAsyncOperationContext *context,
            int *operation_result_1,
            double *operation_result_2);
        
        HRESULT ( STDMETHODCALLTYPE *BeginTestOperationWithNoEndArgs )( 
            ITestAsyncOperation * This,
            int arg1,
            IFabricAsyncOperationCallback *callback,
            IFabricAsyncOperationContext **context);
        
        HRESULT ( STDMETHODCALLTYPE *EndTestOperationWithNoEndArgs )( 
            ITestAsyncOperation * This,
            IFabricAsyncOperationContext *context);
        
        HRESULT ( STDMETHODCALLTYPE *BeginTestOperationWithNoArgs )( 
            ITestAsyncOperation * This,
            IFabricAsyncOperationCallback *callback,
            IFabricAsyncOperationContext **context);
        
        HRESULT ( STDMETHODCALLTYPE *EndTestOperationWithNoArgs )( 
            ITestAsyncOperation * This,
            IFabricAsyncOperationContext *context);
        
        END_INTERFACE
    } ITestAsyncOperationVtbl;

    interface ITestAsyncOperation
    {
        CONST_VTBL struct ITestAsyncOperationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITestAsyncOperation_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITestAsyncOperation_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITestAsyncOperation_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITestAsyncOperation_BeginTestOperation(This,arg1,callback,context)	\
    ( (This)->lpVtbl -> BeginTestOperation(This,arg1,callback,context) ) 

#define ITestAsyncOperation_EndTestOperation(This,context,operation_result_1,operation_result_2)	\
    ( (This)->lpVtbl -> EndTestOperation(This,context,operation_result_1,operation_result_2) ) 

#define ITestAsyncOperation_BeginTestOperationWithNoBeginArgs(This,callback,context)	\
    ( (This)->lpVtbl -> BeginTestOperationWithNoBeginArgs(This,callback,context) ) 

#define ITestAsyncOperation_EndTestOperationWithNoBeginArgs(This,context,operation_result_1,operation_result_2)	\
    ( (This)->lpVtbl -> EndTestOperationWithNoBeginArgs(This,context,operation_result_1,operation_result_2) ) 

#define ITestAsyncOperation_BeginTestOperationWithNoEndArgs(This,arg1,callback,context)	\
    ( (This)->lpVtbl -> BeginTestOperationWithNoEndArgs(This,arg1,callback,context) ) 

#define ITestAsyncOperation_EndTestOperationWithNoEndArgs(This,context)	\
    ( (This)->lpVtbl -> EndTestOperationWithNoEndArgs(This,context) ) 

#define ITestAsyncOperation_BeginTestOperationWithNoArgs(This,callback,context)	\
    ( (This)->lpVtbl -> BeginTestOperationWithNoArgs(This,callback,context) ) 

#define ITestAsyncOperation_EndTestOperationWithNoArgs(This,context)	\
    ( (This)->lpVtbl -> EndTestOperationWithNoArgs(This,context) ) 

/* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITestAsyncOperation_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */


#endif


