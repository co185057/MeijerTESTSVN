#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the UTILS_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// UTILS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef UTILS_EXPORTS
#define UTILS_API __declspec(dllexport)
#else
#   ifdef NO_DLL
#       define UTILS_API
#   else
#       define UTILS_API __declspec(dllimport)
#   endif
#endif
