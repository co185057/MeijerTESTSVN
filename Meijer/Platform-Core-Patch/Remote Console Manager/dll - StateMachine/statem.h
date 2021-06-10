#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the STATEM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// STATEM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef STATEM_EXPORTS
#define STATEM_API __declspec(dllexport)
#else
#   ifdef NO_DLL
#       define STATEM_API
#   else
#       define STATEM_API __declspec(dllimport)
#   endif
#endif
