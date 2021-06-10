/* ======================================================================= */
/* =              Copyright (c) NCR Corporation 1989-2001                = */
/* ======================================================================= */
/* TraceLog.h 160.10 */
/****************************************************************************
                       
Author: 
   Paul M. Parks (pp230000) 
   paul@parkscomputing.com

Purpose: 


Comments: 

    The TraceLog<T> class allows logging code to be embedded in application 
    logic. This code will send information to an implementation-specific 
    Logger class when the tracing level is set to Error, Info, or Verbose; 
    and it will be efficiently ignored when the tracing level is set to 0.

    While a considerable amount of effort has gone into making this class as 
    efficient as possible, there may be cases where the tracing logic should 
    be removed with a compiler flag. Rather than wrapping all tracing code in 
    #ifdef / #endif instructions, you may use the optimizer to remove all 
    tracing code in one fell swoop. Typedef your trace class as follows:

      #ifdef NOTRACE
         // Optimize away trace code.
         typedef TraceLog<NoLogging> Trace;
      #else
         // Normal tracing.
         typedef TraceLog<Logger> Trace;
      #endif

    In this example, the Logger class is a helper class that writes trace 
    information to the screen, a file, etc. 
    Use "Trace" everywhere you would otherwise reference the TraceLog<T> 
    class. When NOTRACE is set, Trace will be a typedef for an empty TraceLog<T> 
    specialization that is easily optimized away entirely. When NOTRACE is not 
    set, the normal tracing logic is used.


   Below is an example of how to initialize and use the TraceLog<T> class.

      // BEGIN EXAMPLE
      #include <fstream>
      #include <iostream>
      #include <TraceLog.h>

      // Pull in the namespaces. 
      using namespace NCR::Util;
      using namespace std;

      // Define a class that provides a method for writing to a log. The 
      // TraceLog class will use this method to output messages to the 
      // log. This class could also 
      struct Logger
      {
         Logger() : logFile("log")
         {
         }

         ~Logger()
         {
            logFile.close();
         }

         std::ofstream logFile;


         // TraceLog passes in the message level for the current message, 
         // the context in which the message was printed, and the message 
         // itself. It is up to this method to format this information 
         // and display or otherwise record it.

         void LogPrint(int msgLevel, unsigned int depth, const char* context, const char* msg)
         {
            // Send to file.
            logFile
               << levelDesc[msgLevel]
               << _T("\t")
               << context 
               << _T(": ") 
               << msg
               << std::flush;

            // Also display on the console.
            std::cout
               << levelDesc[msgLevel]
               << _T("\t")
               << context 
               << _T(": ") 
               << msg;
         }
      };


      // This typedef is easier than typing the entire class name and 
      // template parameters.

      #ifdef NOTRACE
         // Optimize away trace code.
         typedef TraceLog<NoLogging> Trace;
      #else
         // Normal tracing.
         typedef TraceLog<Logger> Trace;
      #endif


      // Set a sane default for the level of log information. Set one level for 
      // debug builds and another for release. 
      // NOTE: This level may be changed during run time.

      #ifdef _DEBUG
         Level Trace::currentLevel = Info;
      #else
         Level Trace::currentLevel = 0;
      #endif


      // Initialize log management class. This class will 
      // create log file and provides methods for writing 
      // to the log. Any other log-management functions should 
      // be placed into this class.

      Trace::Logger_t& Trace::logger = Trace::Logger_t();


      int main()
      {
         // Establish the context in which messages are logged.
         Trace trace("main");


         // Make all messages appear.
         Trace::SetLevel(Verbose);


         // This message is written to the log only when the trace level 
         // is Info or Verbose. If the trace level is 0 it 
         // will not appear, and the call will take only a minimal amount 
         // of cycles to process.

         trace(Info, "This is an Info message.\n");


         // Show messages only at the Error level
         Trace::SetLevel(Error);


         // This message will not appear since the current trace  
         // level is Error only.

         trace(Verbose, "This is a verbose message.");


         // When the trace level is set to none, all tracing code is very 
         // efficiently ignored.
         Trace::SetLevel(0);

         trace(Info, "This will barely cost a few milliseconds.");
   

         // It's possible to force logging to a particular level within a block.
         // For example, in some applications all errors must be logged. 

         int errorCode = GetLastError();

         if (errorCode)
         {
            Trace::ForceLevel to(Error);

            // This example also shows TraceLog's streaming feature.

            trace(Error) 
               << "GetLastError() reports error "
               << hex
               << errorCode
               << "\n"
               << logflush;
         }
         // Logging returns to its previous level here.

         // Dump all the bytes that are in a block of memory, in this case 
         // the trace object itself.
         trace(NCRCAP_DEBUG, "Here's what trace looks like:\n");
         trace.ByteDump(NCRCAP_DEBUG, &trace, sizeof (trace));

         return 0;
      }

      // END EXAMPLE


Change history:

$Log: /Integration_Solution/ACS_Solution/CURRENT_DEV/Source/AfxTransBroker/ACSHook/include/TraceLog.h $
 * 
 * 2     3/29/10 4:46p Pp230000
 * Needed to update to correct revisions. Did not check in correct files
 * for 4.40.1.
 * 
 * Comment at 081009 06:19:51  by psd-pparks  
 * TAR#: 368473 
 * Applied changes that should have been in last check-in. 
 * 
 * Comment at 080830 10:22:12  by psd-pparks  
 * TAR#: 368473 
 * Fixed to automatically detect changes in DCap trace level. 
 * 
 * Comment at 080731 19:24:06  by psd-pparks  
 * TAR#: 368473 
 * 
 * Comment at 070601 14:09:49  by psd-pparks  
 * FEATURE#: 349970 
 * Removed MarshalUnmarshal.dll dependency for ACSIO package. 
 * 
 * Comment at 061024 15:38:52  by atleng-doug  
 * TAR#: 336205 
 * Initialize the output variable in the TraceLog constructor 
 * 
 * Comment at 061006 04:45:01  by psd-pparks  
 * FEATURE#: 329768 
 * Added logging to the ACSIO components, along with making the  
 * CRT threading model consistent across deliverables. 
 * 
 * Comment at 060821 12:21:39  by psd-pparks  
 * FEATURE#: BB324561 
 * Wholesale changes to data capture to increase speed  
 * 
 * Comment at 060314 15:37:09  by atleng-bzhang  
 * FEATURE#: 313768 
 * Pull CMDCap from cm archieve to the base 
 * 
 * Comment at 050628 17:06:02  by psd-pparks  
 * TAR#: 299818 
 * Changes related to conversion from ADO to DB2 CLI. 
 * 
 * Comment at 050404 04:10:39  by psd-pparks  
 * TAR#: 297367 
 * Updated to be more generally useful by all CM modules. Can be  
 * included by either EXE or DLL projects. 
 * 
 * Comment at 040928 14:59:51  by psd-pparks  
 * FEATURE#: SO273303 
 * Final big pre-SIT update. 
 * 
 * Comment at 040922 16:07:49  by psd-pparks  
 * FEATURE#: SO273303 
 * Multiple updates. 
 * 
 * Comment at 040910 09:18:28  by psd-pparks  
 * FEATURE#: SO273300 
 * Massive update. 
  
*****************************************************************************/

#ifndef TRACE_H
#define TRACE_H


#include <iomanip>
#include <cassert>

// Replacement STL stream objects for Unicode/ASCII 
// single-source development.
#include "tstring.h"
#include <map>


namespace NCR
{
   namespace Util
   {
      template<typename Logger> class TraceLog
      {
      public:
         // Syntactic sugar
         typedef Logger Logger_t;
         typedef TraceLog<Logger> My_t;
         typedef std::map<DWORD, tstringstream*> OutMap;
         static OutMap outMap;

         TraceLog(const TCHAR* contextName, bool display = true) : 
            context(contextName), 
            streamLevel(0),
            displayEntryExit(display),
            output(outMap[GetCurrentThreadId()])
         {
            if (!output)
            {
               output = new tstringstream;
               outMap[GetCurrentThreadId()] = output;
            }

            // This code is duplicated to increase efficiency.
            // Don't replace this code with a common function call.
            if ( ( (GetLevel() & scopeReport) == scopeReport ) && displayEntryExit)
            {
               logger.LogPrint(scopeReport, depth, context, _T("{"));
                ++depth;
            }
         }


         // Use this constructor when you want to report scope entry and 
         // exit at tracing levels other than NCRCAP_DEBUG.
         TraceLog(int logLevel, const char* contextName, bool display = true) : 
            context(contextName), 
            streamLevel(0),
            displayEntryExit(display),
            output(outMap[GetCurrentThreadId()])
         {
            if (!output)
            {
               output = new tstringstream;
               outMap[GetCurrentThreadId()] = output;
            }

            // This code is duplicated to increase efficiency.
            // Don't replace this code with a common function call.
            if ( ( (GetLevel() & scopeReport) == scopeReport) && displayEntryExit)
            {
               logger.LogPrint(scopeReport, depth, context, _T("{"));
               ++depth;
            }
         }


         // This destructor isn't virtual because I don't intend for 
         // anyone to derive from this class. Derivation plus tracing 
         // equals bad performance.
         ~TraceLog()
         {
            // This code is duplicated to increase efficiency.
            // Don't replace this code with a common function call.
            if ( ( (currentLevel & scopeReport) == scopeReport) && displayEntryExit)
            {
               --depth;
               logger.LogPrint(scopeReport, depth, context, _T("}"));
            }
         }

         static bool InitLog(TCHAR const* init)
         {
            return !!GetLogger().InitLog(init);
         }

         static void ExitLog()
         {
            GetLogger().ExitLog();

            for (OutMap::iterator i = outMap.begin(); i != outMap.end(); ++i)
            {
               delete i->second;
            }

            outMap.clear();
         }

         static int SetLevel(int newLevel)
         {
            int prevLevel = GetLevel();
            TraceLog::currentLevel = newLevel;
            logger.SetLevel(newLevel);
            return prevLevel;
         }


         /*
         static Level ClearLevel(Level clearLevel)
         {
            Level prevLevel = TraceLog::currentLevel;
            TraceLog::currentLevel &= ~clearLevel;
            return prevLevel;
         }
         */


         static int GetLevel()
         {
            currentLevel = logger.GetLevel();
            return currentLevel;
         }


         static void SetScopeReport(int newLevel)
         {
            scopeReport = newLevel;
         }


         static const TCHAR* GetContext()
         {
            return context;
         }


         static bool IsEnabled()
         {
            return (GetLevel() > 0);
         }


         /*
         static bool IsError()
         {
            return (GetLevel() >= Error);
         }


         static bool IsInfo()
         {
            return (GetLevel() >= Info);
         }


         static bool IsVerbose()
         {
            return (GetLevel() >= Verbose);
         }


         static bool IsDebug()
         {
            return (GetLevel() >= Debug);
         }
         */

         static bool IsLevel(int msgLevel)
         {
            return (GetLevel() & msgLevel) == msgLevel;
         }


         My_t& operator()(int msgLevel)
         {
            streamLevel = msgLevel;
            return *this;
         }


         My_t& operator()(int msgLevel, const TCHAR* msg)
         {
            // A level of 0 is pointless.
            assert(msgLevel);

            streamLevel = msgLevel;

            // This code is duplicated to increase efficiency.
            // Don't replace this code with a common function call.
            if ( (GetLevel() & msgLevel) == msgLevel )
            {
               logger.LogPrint(msgLevel, depth, context, msg);
            }

            return *this;
         }


         const My_t& operator()(int msgLevel) const
         {
            return *this;
         }


         const My_t&  operator()(int msgLevel, const TCHAR* msg) const
         {
            // This code is duplicated to increase efficiency.
            // Don't replace this code with a common function call.
            if ((GetLevel() & msgLevel) == msgLevel)
            {
               logger.LogPrint(msgLevel, depth, context, msg);
            }

            return *this;
         }


         const My_t&  operator()(int msgLevel, const TCHAR* msg, const void* data, size_t dataSize) const
         {
            // This code is duplicated to increase efficiency.
            // Don't replace this code with a common function call.
            if ((GetLevel() & msgLevel) == msgLevel)
            {
               logger.LogPrint(msgLevel, depth, context, msg, data, dataSize);
            }

            return *this;
         }


         const My_t&  operator()(int msgLevel, const void* data, size_t dataSize) const
         {
            // This code is duplicated to increase efficiency.
            // Don't replace this code with a common function call.
            if ((GetLevel() & msgLevel) == msgLevel)
            {
               logger.LogPrint(msgLevel, depth, context, _T(""), data, dataSize);
            }

            return *this;
         }


         void Write(int msgLevel, const TCHAR* msg) const
         {
            // A level of 0 is pointless.
            assert(msgLevel);

            // This code is duplicated to increase efficiency.
            // Don't replace this code with a common function call.
            if ((GetLevel() & msgLevel) == msgLevel)
            {
               logger.LogPrint(msgLevel, depth, context, msg);
            }
         }


         // Provides support for standard manipulators such as hex
         My_t& operator<<(std::ios_base& (__cdecl *_F)(std::ios_base&))
         {
            // This code is duplicated to increase efficiency.
            // Don't replace this code with a common function call.
            if (currentLevel)
            {
               (*output) << _F;
            }

            return *this;
         }


         // Provides support for TraceLog manipulators error, info, verbose, logflush
         My_t& operator<<(My_t& (*f)(My_t&) )
         {
            return (*f)(*this);
         }

         // All other stream data
         template<typename T> My_t& operator<<(const T& data)
         {
            // This code is duplicated to increase efficiency.
            // Don't replace this code with a common function call.
            if ( (currentLevel & streamLevel) == streamLevel || streamLevel == 0xFFFFFFFF )
            {
               (*output) << data;
            }

            return *this;
         }


         /*
         template<typename T> My_t& operator<<(T& data)
         {
            // This code is duplicated to increase efficiency.
            // Don't replace this code with a common function call.
            if ( (currentLevel & streamLevel) == streamLevel || streamLevel == 0xFFFFFFFF )
            {
               (*output) << data;
            }

            return *this;
         }
         */


         My_t& streamlevel(int msgLevel)
         {
            streamLevel = msgLevel;
            return *this;
         }

         /*
         My_t& rawLog(const void* data, size_t size)
         {
            if ((GetLevel() & streamLevel) == streamLevel)
            {
               logger.LogPrint(streamLevel, depth, context, _T(""), data, size);
            }

            return *this;
         }
         */


         /*
         My_t& error()
         {
            streamLevel = Error;
            return *this;
         }


         My_t& info()
         {
            streamLevel = Info;
            return *this;
         }


         My_t& verbose()
         {
            streamLevel = Verbose;
            return *this;
         }


         My_t& debug()
         {
            streamLevel = NCRCAP_DEBUG;
            return *this;
         }
         */


         My_t& logflush()
         {
            // This code is duplicated to increase efficiency.
            // Don't replace this code with a common function call.
            if ((currentLevel & streamLevel) == streamLevel || streamLevel == 0xFFFFFFFF)
            {
               logger.LogPrint(streamLevel, depth, context, (*output).str().c_str());
            }

            // Empty the output buffer.
            (*output).clear();
            (*output).str(TEXT(""));

            return *this;
         }


         template<class T> void ByteDump(
            int msgLevel,
            const TCHAR* msg,
            const T* c, 
            size_t size, 
            const T* offset = 0,
            int width = 16
            )
         {
            using namespace std;

            if ((GetLevel() & msgLevel) == msgLevel)
            {
               logger.LogPrint(msgLevel, depth, context, msg, c, size);
            }

            this->logflush();
         }



         // Use this class to force logging at a certain level within a scope.
         // 
         // Ex:
         //
         // void Foo()
         // {
         //    TraceLog<LogHelper> trace("Foo");
         //
         //    if (someErrorFlagSet)
         //    {
         //       TraceLog<LogHelper>::ForceLevel to(NCRCAP_DEBUG);
         //
         //       trace(
         //          NCRCAP_DEBUG, 
         //          "This will log no matter what the previous level was.\n"
         //          );
         //    }
         //    
         //    // Logging automatically returns to its previous level here.
         // }


         class ForceLevel
         {
         public:
            ForceLevel(int newLevel) : 
               prevLevel( ((My_t::GetLevel() & newLevel) == newLevel) ? My_t::currentLevel : My_t::SetLevel(newLevel))
            {
            }

            ~ForceLevel()
            {
               My_t::SetLevel(prevLevel);
            }

         private:
            int prevLevel;
         };

         friend ForceLevel;


         // This output iterator can be used in conjunction with algorithms 
         // provided by the standard C++ library.
         // 
         // Ex:
         //
         //    void Foo()
         //    {
         //       Trace trace("Foo");
         // 
         //       char x[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
         //   
         //       trace(NCRCAP_DEBUG) << "x contains (in hex): " << hex;
         //       std::copy(x, x + sizeof (x), Trace::iterator<int>(trace(NCRCAP_DEBUG), " "));
         //       trace(NCRCAP_DEBUG) << _T("\n") << logflush;
         //    }

         template<class U>
#if _MSC_VER <= 1200
         class iterator : public std::iterator<std::output_iterator_tag, void, void> 
#else
         class iterator : public std::iterator<std::output_iterator_tag, U> 
#endif
         {
         public:
            typedef U value_type;
            typedef My_t log_type;

            iterator(log_type& O, const TCHAR* D = 0) : Ostr(&O), Delim(D) 
            {
            }

            iterator<U>& operator=(const U& X)
            {
               *Ostr << X;
               return (*this); 
            }

            iterator<U>& operator*()
            {
               return (*this); 
            }

            iterator<U>& operator++()
            {
               if (Delim != 0)
               {
                  *Ostr << Delim;
               }

               return (*this); 
            }

            iterator<U> operator++(int)
            {
               if (Delim != 0)
               {
                  *Ostr << Delim;
               }

               return (*this); 
            }

         protected:
            const TCHAR* Delim;
            log_type *Ostr;
         };



         static Logger_t& GetLogger()
         {
            return logger;
         }


      private:

         friend ForceLevel;

         tstringstream* output;
         TCHAR const* context;

         int streamLevel;
         bool displayEntryExit;

         static Logger_t logger;
         static int currentLevel;
         static unsigned int depth;
         static int scopeReport;
      };


      template<class L> inline TraceLog<L>& error(TraceLog<L>& s)
      {
         return s.error();
      }

      
      template<class L> inline TraceLog<L>& info(TraceLog<L>& s)
      {
         return s.info();
      }

      
      template<class L> inline TraceLog<L>& verbose(TraceLog<L>& s)
      {
         return s.verbose();
      }

      template<class L> inline TraceLog<L>& logflush(TraceLog<L>& s)
      {
         return s.logflush();
      }

      template<class L> inline TraceLog<L>& endl(TraceLog<L>& s)
      {
         return s.logflush();
      }

      /*
      class raw
      {
         const void* data;
         size_t size;

      public:
         raw(const void* dataInit, size_t sizeInit) : data(dataInit), size(sizeInit)
         {
         }

         template<class L> TraceLog<L>& operator<<(TraceLog<L>& log) const
         {
            return log.rawLog(data, size);
         }
      };

      template<class L> inline TraceLog<L>& operator<<(TraceLog<L>& s, raw const& rawData)
      {
         return s.rawLog(data, size);
      }
      */

      // This is a marker class to allow the optimizer to remove all 
      // tracing code. 
      class NoLogging
      {
      };


      // To quickly optimize away all tracing code, typedef your Trace 
      // class as TraceLog<NoLogging>. Your tracing code will stay in your 
      // source, but the release-build optimizer will remove almost all 
      // assembly code that would have been generated for it.
      template<> class TraceLog<NoLogging>
      {
      public:
#if _MSC_VER <= 1200
         typedef Logger Logger_t;
#else
         typedef NoLogging Logger_t;
#endif
         typedef TraceLog<NoLogging> My_t;
         TraceLog(const char*, bool = true) {}
         TraceLog(int, const char*, bool = true){}
         ~TraceLog(){}
         static bool InitLog(TCHAR const*) { return true; }
         static void ExitLog() {}
         static int SetLevel(int) { return 0; }
         static int GetLevel() { return 0; }
         static void SetScopeReport(int) {}
         static const TCHAR* GetContext() { return 0; }
         static bool IsEnabled() { return false; }
         static bool IsError() { return false; }
         static bool IsInfo() { return false; }
         static bool IsVerbose() { return false; }
         static bool IsDebug() { return false; }
         static bool IsLevel(int) { return false; }
         My_t& operator()(int) { return *this; }
         My_t& operator()(int, const TCHAR*) { return *this; }
         const My_t& operator()(int) const { return *this; } 
         const My_t& operator()(int, const TCHAR*) const { return *this; } 
         void Write(int, const TCHAR*) const { }
         My_t& operator<<(std::ios_base& (__cdecl *)(std::ios_base&)) { return *this; }
         My_t& operator<<(My_t& (*)(My_t&) ) { return *this; }
         template<typename T> My_t& operator<<(const T&) { return *this; }
         template<typename T> My_t& operator<<(T&) { return *this; }
         My_t& streamlevel(int) { return *this; }
         My_t& error() { return *this; }
         My_t& info() { return *this; }
         My_t& verbose() { return *this; }
         My_t& logflush() { return *this; }
         template<class T> void ByteDump(int, const TCHAR*, const T*,  size_t,  const T* = 0, int = 16 ) {}

         class ForceLevel { public: ForceLevel(int) {} ~ForceLevel() {} };
         template<class U>
#if _MSC_VER <= 1200
         class iterator : public std::iterator<std::output_iterator_tag, void, void> 
#else
         class iterator : public std::iterator<std::output_iterator_tag, U> 
#endif
         {
         public:
            typedef U value_type;
            typedef My_t log_type;
            iterator(log_type&, const TCHAR* D = 0) {}
            iterator<U>& operator=(const U&) { return (*this); }
            iterator<U>& operator*() { return (*this); }
            iterator<U>& operator++() { return (*this); }
            iterator<U> operator++(int) { return (*this); }
         };
         static Logger_t& GetLogger() { return logger; }
         static unsigned int depth;

      private:
         static int currentLevel;
#if _MSC_VER <= 1200
         static Logger logger;
#else
         static Logger_t logger; 
#endif
      };


#if _MSC_VER <= 1200
      template<typename Logger> TraceLog<Logger>::Logger_t TraceLog<Logger>::logger;
      template<typename Logger> TraceLog<Logger>::OutMap TraceLog<Logger>::outMap;
#else
      template<typename Logger> typename TraceLog<Logger>::Logger_t TraceLog<Logger>::logger;
      template<typename Logger> typename TraceLog<Logger>::OutMap TraceLog<Logger>::outMap;
#endif
      // template<typename Logger> int TraceLog<Logger>::currentLevel = 0;
      // template<typename Logger> int TraceLog<Logger>::scopeReport = 0;
      template<typename Logger> unsigned int TraceLog<Logger>::depth = 0;
   }
}


#endif // #ifndef TRACE_H
