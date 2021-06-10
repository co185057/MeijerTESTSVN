/*******************************************************************************
//
// Module name:  PageClient.h
//
// File version:  $Version: $1.0
//
// Description:  Header file defining exported functions from the PageClient DLL
//
// Original Author:  Alex Antonov
//
// Legal Copyright:  Copyright (c) NCR Corp. 1999
//
// Product name:  Paging System
//
// Change History:	Added Header Information, JZB
//   
// 
*********************************************************************************/


#ifndef xxPAGECLIENT 
#define xxPAGECLIENT

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef PAGING_DLL
#define PAGE_dll dllexport
#else
#define PAGE_dll dllimport
#endif

extern _declspec (PAGE_dll) INT PagingAvailable();
extern _declspec (PAGE_dll) INT SendPage(char *message, INT writeToLog);
extern _declspec (PAGE_dll) VOID Uninitialize(); //tar 229591

#ifdef __cplusplus
}
#endif

#endif