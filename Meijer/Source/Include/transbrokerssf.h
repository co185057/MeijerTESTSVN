//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  TransBrokerSSF.h
//
// TITLE:   Transaction Broker - SSF Interface File
// 
// AUTHOR: 
//
// Copyright (c) NCR Corp. 2003
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _TransBrokerSSF_H_
#define _TransBrokerSSF_H_

#include "TransBroker.h"
#include "..\AFXTransBroker\CustomerTransBroker\CustomerTBSharedDefs.h"

#ifdef   __cplusplus
   extern   "C" {
#endif   /* __cplusplus */
    
#ifdef  _TRANSBROKER_PROVIDER_
   #define  APIExpose   __declspec(dllexport)
#else
   #define  APIExpose   __declspec(dllimport)
#endif


#ifdef   __cplusplus
   }    /* End of extern "C" {   */
#endif  /* __cplusplus           */

#endif  /* _TransBrokerSSF_H_    */

