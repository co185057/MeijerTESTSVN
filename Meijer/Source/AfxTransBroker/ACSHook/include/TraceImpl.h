/* ======================================================================= */
/* =              Copyright (c) NCR Corporation 1989-2001                = */
/* ======================================================================= */
/* TraceImpl.h 160.8 */
/****************************************************************************
                       
Author: 
   Paul M. Parks (pp230000) 
   paul@parkscomputing.com

Purpose: 


Comments: 


Change history:

$Log: /Integration_Solution/ACS_Solution/CURRENT_DEV/Source/AfxTransBroker/ACSHook/include/TraceImpl.h $
 * 
 * 2     3/29/10 4:46p Pp230000
 * Needed to update to correct revisions. Did not check in correct files
 * for 4.40.1.
 * 
 * Comment at 081009 06:20:42  by psd-pparks  
 * TAR#: 368473 
 * Applied changes that should have been in last check-in. 
 * 
 * Comment at 080830 10:23:02  by psd-pparks  
 * TAR#: 368473 
 * Fixed to automatically detect changes in DCap trace level. 
 * 
 * Comment at 080731 19:24:43  by psd-pparks  
 * TAR#: 368473 
 * 
 * Comment at 070601 14:08:46  by psd-pparks  
 * FEATURE#: 349970 
 * Removed MarshalUnmarshal.dll dependency for ACSIO package. 
 * 
 * Comment at 061006 04:45:32  by psd-pparks  
 * FEATURE#: 329768 
 * Added logging to the ACSIO components, along with making the  
 * CRT threading model consistent across deliverables. 
 * 
 * Comment at 060821 12:21:07  by psd-pparks  
 * FEATURE#: BB324561 
 * Wholesale changes to data capture to increase speed  
 * 
 * Comment at 060314 15:36:24  by atleng-bzhang  
 * FEATURE#: 313768 
 * Pull CMDCap from cm archieve to the base 
 * 
 * Comment at 050404 03:59:36  by psd-pparks  
 * TAR#: 297367 
 * This header is being moved to a common directory for use by multiple  
 * modules. 
 * 
 * Comment at 050310 02:01:18  by psd-pparks  
 * TAR#: 293711 
 * Changed LogPrint function so that it can't send a string  
 * longer than 1024 characters to CMDCAP. This was causing  
 * exceptions when it happened. 
 * 
 * Comment at 041022 15:59:10  by psd-pparks  
 * TAR#: 283462 
 * Improved CMDCap logging. Now matches formats of other applications. 
 * 
 * Comment at 041004 14:43:02  by psd-pparks  
 * FEATURE#: SO273300 
 * Changed #elif to #else in _DEBUG check. (This time,I really made the change.) 
 * 
 * Comment at 041004 14:38:47  by psd-pparks  
 * FEATURE#: SO273300 
 * Changed #elif to #else in _DEBUG check. 
 * 
 * Comment at 040928 14:27:51  by psd-pparks  
 * FEATURE#: SO273303 
 * TraceLog logger implementation for NCR Data Capture. 
  
*****************************************************************************/

#ifndef H_TRACEIMPL
#define H_TRACEIMPL

#include "DCapApi.h"
#include "TraceLog.h"
#include "tstring.h"
#include <map>

class Tracer 
{
   typedef std::map<DWORD, std::vector<char> > TempVecs;

public:
   Tracer() : hCap(0)
   {
   }
   
   ~Tracer()
   {
      DcFlush(hCap);
      DcCloseInstance(hCap);
   }
   
   HANDLE InitLog(
      WCHAR const* wModuleName,
      WCHAR const* wCmdLine = 0
      )
   {
      if (!wCmdLine)
      {
         wCmdLine = wModuleName;
      }

      std::vector<char>& cmdLine = tempContexts[GetCurrentThreadId()];
      std::vector<char>& moduleName = tempMessages[GetCurrentThreadId()];

      int cmdLen = (int)wcslen(wCmdLine);

      if (cmdLen >= cmdLine.size())
      {
         cmdLine.resize(cmdLen + 1);
      }

      int moduleLen = (int)wcslen(wModuleName);

      if (moduleLen >= moduleName.size())
      {
         moduleName.resize(moduleLen + 1);
      }

      char const* unmappable = "?";

      ::WideCharToMultiByte(
         CP_ACP, 
         0, 
         wCmdLine, 
         cmdLen, 
         &cmdLine[0], 
         static_cast<int>(cmdLine.size()),
         unmappable,
         0
         );

      ::WideCharToMultiByte(
         CP_ACP, 
         0, 
         wModuleName, 
         moduleLen, 
         &moduleName[0], 
         static_cast<int>(moduleName.size()),
         unmappable,
         0
         );

      cmdLine[cmdLen] = 0;
      moduleName[moduleLen] = 0;

      std::string announce = std::string() + "*** " + &moduleName[0] + " Starting ***";

      hCap = DcCreateInstance(&moduleName[0], announce.c_str());

      if (hCap)
      {
         NCR::Util::TraceLog<Tracer>::SetLevel(DcGetMask(hCap));
      }

      return hCap;
   }

   HANDLE InitLog(
      CHAR const* moduleName,
      CHAR const* cmdLine = 0
      )
   {
      if (!cmdLine)
      {
         cmdLine = moduleName;
      }

      std::string announce = std::string() + "*** " + moduleName + " Starting ***";

      hCap = DcCreateInstance(moduleName, announce.c_str());

      if (hCap)
      {
         NCR::Util::TraceLog<Tracer>::SetLevel(DcGetMask(hCap));
      }

      return hCap;
   }

   void ExitLog()
   {
      DcFlush(hCap);
      DcCloseInstance(hCap);
   }

   void LogPrint(
      int msgLevel, 
      unsigned int depth, 
      const WCHAR* context, 
      const WCHAR* msg,
      void const* data = 0,
      size_t dataSize = 0
      )
   {
      std::vector<char>& tempContext = tempContexts[GetCurrentThreadId()];
      std::vector<char>& tempMessage = tempMessages[GetCurrentThreadId()];

      int contextLen = (int)wcslen(context);

      if (contextLen >= tempContext.size())
      {
         tempContext.resize(contextLen + 1);
      }

      int messageLen = (int)wcslen(msg);

      if (messageLen >= tempMessage.size())
      {
         tempMessage.resize(messageLen + 1);
      }

      char const* unmappable = "?";

      ::WideCharToMultiByte(
         CP_ACP, 
         0, 
         context, 
         contextLen, 
         &tempContext[0], 
         static_cast<int>(tempContext.size()),
         unmappable,
         0
         );

      ::WideCharToMultiByte(
         CP_ACP, 
         0, 
         msg, 
         messageLen, 
         &tempMessage[0], 
         static_cast<int>(tempMessage.size()),
         unmappable,
         0
         );

      tempContext[contextLen] = 0;
      tempMessage[messageLen] = 0;

      LogPrint(msgLevel, depth, &tempContext[0], &tempMessage[0], data, dataSize);
   }


   void LogPrint(
      int msgLevel, 
      unsigned int depth, 
      const CHAR* context, 
      const CHAR* msg,
      void const* data = 0,
      size_t dataSize = 0
      )
   {
      if (data && dataSize)
      {
         DcByteDump(hCap, msgLevel, context, data, (DWORD)dataSize, msg);
      }
      else
      {
         DcWrite(hCap, msgLevel, context, msg);
      }
   }


   void SetLevel(int newLevel)
   {
      DcSetMask(hCap, newLevel);
   }


   int GetLevel() const
   {
      return DcGetMask(hCap);
   }


private:
   HANDLE hCap;
   TempVecs tempContexts;
   TempVecs tempMessages;
};


template<typename T>
class Timer
{
private:
   int level;
   T& trace;
   DWORD start;
   DWORD end;

public:
   Timer(int levelInit, T& traceInit) : level(levelInit), trace(traceInit), start(GetTickCount()), end(0)
   {
   }

   ~Timer()
   {
      end = GetTickCount();
      trace(level) << TEXT("Operation completed in ") << (end - start) << TEXT(" milliseconds.") << logflush;
   }
};


// The NOTRACE compiler setting will allow the optimizer to remove 
// all tracing code.
#ifdef NOTRACE
// Optimize away trace code.
typedef NCR::Util::TraceLog<NCR::Util::NoLogging> Trace;
#else
// Normal tracing.
typedef NCR::Util::TraceLog<Tracer> Trace;
#endif

template<typename Logger> int NCR::Util::TraceLog<Logger>::currentLevel = DCM_CAPTURE;
template<typename Logger> int NCR::Util::TraceLog<Logger>::scopeReport = DCM_DEBUG;

typedef Timer<Trace> TimerTrace;

#endif // #ifndef H_TRACEIMPL
