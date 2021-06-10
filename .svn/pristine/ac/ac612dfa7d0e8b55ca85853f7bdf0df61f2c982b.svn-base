/* ======================================================================= */
/* =              Copyright (c) NCR Corporation 1989-2001                = */
/* ======================================================================= */
/* ACSIODataKit.h 4.2 */
/****************************************************************************
                       
Author: 
   Paul M. Parks (pp230000) 
   paul@parkscomputing.com

Purpose: 
   Parse SALESLNF.H and ASVGSD.H at runtime, cache structure definitions, 
   and allow library clients to manipulate PS contexts and IA sequences 
   without having to compile with the PS and IA headers.

Comments: 
   For documentation and library overview, see 
   http://pmpdevbox/wiki/perspective.aspx?action=view&page=acs:ACSIODataKit

Change history:

$Log: /Integration_Solution/ACS_Solution/CURRENT_DEV/Source/AfxTransBroker/ACSHook/include/ACSIODataKit.h $
 * 
 * 1     3/19/10 4:13p Sm133076
 * Added ACSIO headers so that ACSHookManager can use the prototypes
 * directly since it no longer uses LoadLibrary to load the corresponding
 * DLLs.
 * 
 * Comment at 090714 11:26:14  by atleng-cwatson  
 * TAR#: 409598 
 * Event notification for message arrivals was replaced with a semaphore 
 * that is incremented every time a message arrives on a channel and is 
 * decremented every time a message is retrieved. 
 * 
 * Comment at 090404 22:36:06  by psd-pparks  
 * FEATURE#: 383942 
 * Moved to general include directory since ACSIODataKit needs to be accessible  
 * to sales and other applications. Also added DkLoadContextHeader prototype. 
 * 
 * Comment at 080830 10:41:26  by psd-pparks  
 * TAR#: 383942 
 * Added API function to set name of trace instance. 
 * 
 * Comment at 080803 02:48:06  by psd-pparks  
 * TAR#: 383942 
 * Added new functions DkGetContextFieldEx and DkGetSequenceFieldEx;  
 * deprecated DkGetContextField and DkGetSequenceField, fixed potential leak  
 * conditions, updated DataKit class wrappers. 
 * 
 * Comment at 070601 21:47:42  by psd-pparks  
 * FEATURE#: 349970 
 * Removed MarshalUnmarshal.dll dependency for ACSIO package. 
 * 
 * Comment at 070403 14:36:34  by psd-pparks  
 * FEATURE#: 345996 
 * Fixed crash condition in DkGetContextField and DkGetSequenceField. 
 * 
 * Comment at 061016 10:48:48  by psd-pparks  
 * FEATURE#: 334844 
 * Added two new functions to return information about a specified  
 * structure. 
 * 
 * Comment at 061012 09:14:29  by psd-pparks  
 * FEATURE#: 334844 
 * Added methods to get information about a structure or the  
 * fields in a structure without having to go through an iterator. 
 * 
 * Comment at 061009 16:02:23  by psd-pparks  
 * FEATURE#: 334844 
 * Cleaned up assertions, added more assertions and error reports,  
 * added functions to return the number of structures and the number  
 * of fields in a structure. Added C++ classes that wrap the raw API. 
 * 
 * Comment at 061005 11:09:39  by psd-pparks  
 * FEATURE#: 334844 
 * New component for parsing and using ASVGSD.H and SALESLNF.H  
 * without having to compile with them. This removes the FastLane  
 * configuration dependency in ApplicationModel.xml, improves speed  
 * of context parsing, and allows for extensive logging in ACSIO  
 * components. 

*****************************************************************************/

#ifndef H_ACSIODATAKIT
#define H_ACSIODATAKIT

#include "windows.h"
#include "crtdbg.h"

/* The following ifdef block is the standard way of creating macros which make exporting 
from a DLL simpler. All files within this DLL are compiled with the ACSIODATAKIT_EXPORTS
symbol defined on the command line. this symbol should not be defined on any project
that uses this DLL. This way any other project whose source files include this file see 
ACSIODATAKIT_API functions as being imported from a DLL, whereas this DLL sees symbols
defined with this macro as being exported. */
#ifdef ACSIODATAKIT_EXPORTS
#define ACSIODATAKIT_API __declspec(dllexport)
#else
#define ACSIODATAKIT_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {
#endif


ACSIODATAKIT_API LPVOID DkAllocContext(
   PCSTR contextName
   );

ACSIODATAKIT_API HANDLE DkAllocContextFieldIterator(
   PCSTR name
   );

ACSIODATAKIT_API HANDLE DkAllocContextIterator();

ACSIODATAKIT_API LPVOID DkAllocSequence(
   PCSTR sequenceName
   );

ACSIODATAKIT_API HANDLE DkAllocSequenceFieldIterator(
   PCSTR name
   );

ACSIODATAKIT_API HANDLE DkAllocSequenceIterator();

ACSIODATAKIT_API BOOL DkCopyContext(
   LPCVOID source,
   LPVOID dest
   );

ACSIODATAKIT_API BOOL DkCopySequence(
   LPCVOID source,
   LPVOID dest
   );

ACSIODATAKIT_API HLOCAL DkFreeContext(
   LPVOID buffer
   );

ACSIODATAKIT_API void DkFreeContextFieldIterator(
   HANDLE handle
   );

ACSIODATAKIT_API void DkFreeContextIterator(
   HANDLE handle
   );

ACSIODATAKIT_API HLOCAL DkFreeSequence(
   LPVOID buffer
   );

ACSIODATAKIT_API void DkFreeSequenceFieldIterator(
   HANDLE handle
   );

ACSIODATAKIT_API void DkFreeSequenceIterator(
   HANDLE handle
   );

ACSIODATAKIT_API DWORD DkGetContextCount();

ACSIODATAKIT_API HRESULT DkGetContextFieldEx(
   PCSTR contextName, 
   PCSTR fieldName, 
   LPCVOID buffer,
   /*out*/ VARIANT* pVal
   );

/* WARNING: This function call is deprecated. Use DkGetContextFieldEx instead */
ACSIODATAKIT_API VARIANT DkGetContextField(
   PCSTR contextName, 
   PCSTR fieldName, 
   LPCVOID buffer
   );

ACSIODATAKIT_API DWORD DkGetContextFieldCount(
   PCSTR contextName
   );

ACSIODATAKIT_API BOOL DkGetContextFieldInfo(
   PCSTR contextName, 
   PCSTR fieldName, 
   /*out*/ VARTYPE* type, 
   /*out*/ DWORD* size, 
   /*out*/ DWORD* offset
   );

/* Added for Meijer SOTF7062 */
ACSIODATAKIT_API BOOL DkGetContextFieldInfoEx(
   PCSTR contextName, 
   PCSTR fieldName, 
   /*out*/ VARTYPE* type, 
   /*out*/ DWORD* size, 
   /*out*/ DWORD* offset,
   /*out*/ DWORD* index
   );

ACSIODATAKIT_API BOOL DkGetContextInfo(
   LPCSTR structName, 
   /*out*/ DWORD* fieldCount
   );

ACSIODATAKIT_API DWORD DkGetContextSize(
   PCSTR contextName
   );

ACSIODATAKIT_API DWORD DkGetSequenceCount();

ACSIODATAKIT_API HRESULT DkGetSequenceFieldEx(
   PCSTR sequenceName, 
   PCSTR fieldName, 
   LPCVOID buffer,
   /*out*/ VARIANT* pVal
   );

/* WARNING: This function call is deprecated. Use DkGetSequenceFieldEx instead */
ACSIODATAKIT_API VARIANT DkGetSequenceField(
   PCSTR sequenceName, 
   PCSTR fieldName, 
   LPCVOID buffer
   );

ACSIODATAKIT_API DWORD DkGetSequenceFieldCount(
   PCSTR sequenceName
   );

ACSIODATAKIT_API BOOL DkGetSequenceFieldInfo(
   PCSTR sequenceName, 
   PCSTR fieldName, 
   /*out*/ VARTYPE* type, 
   /*out*/ DWORD* size, 
   /*out*/ DWORD* offset
   );

/* Added for Meijer SOTF7062 */
ACSIODATAKIT_API BOOL DkGetSequenceFieldInfoEx(
   PCSTR sequenceName, 
   PCSTR fieldName, 
   /*out*/ VARTYPE* type, 
   /*out*/ DWORD* size, 
   /*out*/ DWORD* offset,
   /*out*/ DWORD* index
   );

ACSIODATAKIT_API BOOL DkGetSequenceInfo(
   LPCSTR structName, 
   DWORD* fieldCount
   );

ACSIODATAKIT_API DWORD DkGetSequenceSize(
   PCSTR sequenceName
   );

ACSIODATAKIT_API DWORD DkGetSequenceID(
   LPCSTR sequenceName
   );

ACSIODATAKIT_API LPCSTR DkGetSequenceName(
   DWORD sequenceID
   );

ACSIODATAKIT_API HRESULT DkInitialize();

ACSIODATAKIT_API HRESULT DkLoadContextStructures();

ACSIODATAKIT_API HRESULT DkLoadSequenceStructures();

ACSIODATAKIT_API BOOL DkQueryContextFieldIterator(
   HANDLE iterator, 
   /*out*/ LPCSTR* fieldName, 
   /*out*/ VARTYPE* type, 
   /*out*/ DWORD* size, 
   /*out*/ DWORD* offset
   );

/* Added for Meijer SOTF7062 */
ACSIODATAKIT_API BOOL DkQueryContextFieldIteratorEx(
   HANDLE iterator, 
   /*out*/ LPCSTR* fieldName, 
   /*out*/ VARTYPE* type, 
   /*out*/ DWORD* size, 
   /*out*/ DWORD* offset,
   /*out*/ DWORD* index
   );

ACSIODATAKIT_API BOOL DkQueryContextIterator(
   HANDLE iterator, 
   /*out*/ LPCSTR* structName, 
   /*out*/ DWORD* fieldCount
   );

ACSIODATAKIT_API BOOL DkQuerySequenceFieldIterator(
   HANDLE iterator, 
   /*out*/ LPCSTR* fieldName, 
   /*out*/ VARTYPE* type, 
   /*out*/ DWORD* size, 
   /*out*/ DWORD* offset
   );

/* Added for Meijer SOTF7062 */
ACSIODATAKIT_API BOOL DkQuerySequenceFieldIteratorEx(
   HANDLE iterator, 
   /*out*/ LPCSTR* fieldName, 
   /*out*/ VARTYPE* type, 
   /*out*/ DWORD* size, 
   /*out*/ DWORD* offset,
   /*out*/ DWORD* index
   );

ACSIODATAKIT_API BOOL DkQuerySequenceIterator(
   HANDLE iterator, 
   /*out*/ LPCSTR* structName, 
   /*out*/ DWORD* fieldCount
   );

ACSIODATAKIT_API HRESULT DkSetContextField(
   PCSTR fieldName, 
   VARIANT value, 
   LPVOID buffer
   );

ACSIODATAKIT_API HRESULT DkSetSequenceField(
   PCSTR fieldName, 
   VARIANT value, 
   LPVOID buffer
   );

ACSIODATAKIT_API void DkUninitialize();

ACSIODATAKIT_API HRESULT DkUnloadContextStructures();

ACSIODATAKIT_API HRESULT DkUnloadSequenceStructures();

/* Set the name of the trace instance for this run of the library. Returns TRUE if the 
name was successfully prior to the creation of the trace instance, FALSE otherwise. */
ACSIODATAKIT_API BOOL DkSetTraceInstanceName(PCSTR name);

ACSIODATAKIT_API HRESULT DkLoadContextHeader(PCSTR configPathInit);
// ACSIODATAKIT_API HRESULT DkLoadSequenceHeader(PCSTR configPathInit);



/* These typedefs are useful for dynamically loading the DLL and setting function 
pointers with GetProcAddress */

typedef HRESULT (*DkInitialize_ptr_t)(void);
typedef void (*DkUninitialize_ptr_t)(void);

typedef HRESULT (*DkLoadContextStructures_ptr_t)(void);
typedef HRESULT (*DkUnloadContextStructures_ptr_t)(void);

typedef HRESULT (*DkLoadSequenceStructures_ptr_t)(void);
typedef HRESULT (*DkUnloadSequenceStructures_ptr_t)(void);

typedef LPVOID (*DkAllocContext_ptr_t)(PCSTR /*contextName*/);
typedef HLOCAL (*DkFreeContext_ptr_t)(LPVOID /*buffer*/);

typedef LPVOID (*DkAllocSequence_ptr_t)(PCSTR /*sequenceName*/);
typedef HLOCAL (*DkFreeSequence_ptr_t)(LPVOID /*buffer*/);

typedef HRESULT (*DkSetContextField_ptr_t)(PCSTR /*fieldName*/, VARIANT /*value*/, LPVOID /*buffer*/);
typedef HRESULT (*DkGetContextFieldEx_ptr_t)(PCSTR /*contextName*/, PCSTR /*fieldName*/, LPCVOID /*buffer*/, VARIANT* /*pVal*/);
typedef VARIANT (*DkGetContextField_ptr_t)(PCSTR /*contextName*/, PCSTR /*fieldName*/, LPCVOID /*buffer*/);

typedef HRESULT (*DkSetSequenceField_ptr_t)(PCSTR /*fieldName*/, VARIANT /*value*/, LPVOID /*buffer*/);
typedef HRESULT (*DkGetSequenceFieldEx_ptr_t)(PCSTR /*sequenceName*/, PCSTR /*fieldName*/, LPCVOID /*buffer*/, VARIANT* /*pVal*/);
typedef VARIANT (*DkGetSequenceField_ptr_t)(PCSTR /*sequenceName*/, PCSTR /*fieldName*/, LPCVOID /*buffer*/);

typedef LPCSTR (*DkGetSequenceName_ptr_t)(DWORD /*sequenceID*/);
typedef DWORD (*DkGetSequenceID_ptr_t)(LPCSTR /*sequenceName*/);

typedef HANDLE (*DkAllocSequenceIterator_ptr_t)();
typedef void (*DkFreeSequenceIterator_ptr_t)(HANDLE /*handle*/);
typedef BOOL (*DkQuerySequenceIterator_ptr_t)(HANDLE /*iterator*/, LPCSTR* /*structName*/, DWORD* /*fieldCount*/);

typedef HANDLE (*DkAllocContextIterator_ptr_t)();
typedef void (*DkFreeContextIterator_ptr_t)(HANDLE /*handle*/);
typedef BOOL (*DkQueryContextIterator_ptr_t)(HANDLE /*iterator*/, LPCSTR* /*structName*/, DWORD* /*fieldCount*/);

typedef HANDLE (*DkAllocSequenceFieldIterator_ptr_t)(PCSTR /*name*/);
typedef void (*DkFreeSequenceFieldIterator_ptr_t)(HANDLE /*handle*/);
typedef BOOL (*DkQuerySequenceFieldIterator_ptr_t)(HANDLE /*iterator*/, LPCSTR* /*fieldName*/, VARTYPE* /*type*/, DWORD* /*size*/, DWORD* /*offset*/);

/* Added for Meijer SOTF7062 */
typedef BOOL (*DkQuerySequenceFieldIteratorEx_ptr_t)(HANDLE /*iterator*/, LPCSTR* /*fieldName*/, VARTYPE* /*type*/, DWORD* /*size*/, DWORD* /*offset*/, DWORD* /*index*/);

typedef HANDLE (*DkAllocContextFieldIterator_ptr_t)(PCSTR /*name*/);
typedef void (*DkFreeContextFieldIterator_ptr_t)(HANDLE /*handle*/);
typedef BOOL (*DkQueryContextFieldIterator_ptr_t)(HANDLE /*iterator*/, LPCSTR* /*fieldName*/, VARTYPE* /*type*/, DWORD* /*size*/, DWORD* /*offset*/);

/* Added for Meijer SOTF7062 */
typedef BOOL (*DkQueryContextFieldIteratorEx_ptr_t)(HANDLE /*iterator*/, LPCSTR* /*fieldName*/, VARTYPE* /*type*/, DWORD* /*size*/, DWORD* /*offset*/, DWORD* /*index*/);

typedef BOOL (*DkCopyContext_ptr_t)(LPCVOID /*source*/, LPVOID /*dest*/);
typedef BOOL (*DkCopySequence_ptr_t)(LPCVOID /*source*/, LPVOID /*dest*/);

typedef DWORD (*DkGetContextSize_ptr_t)(PCSTR /*contextName*/);
typedef DWORD (*DkGetSequenceSize_ptr_t)(PCSTR /*sequenceName*/);

typedef DWORD (*DkGetContextCount_ptr_t)(void);
typedef DWORD (*DkGetSequenceCount_ptr_t)(void);

typedef DWORD (*DkGetContextFieldCount_ptr_t)(PCSTR /*contextName*/);
typedef DWORD (*DkGetSequenceFieldCount_ptr_t)(PCSTR /*sequenceName*/);

typedef BOOL (*DkGetContextFieldInfo_ptr_t)(PCSTR /*contextName*/, PCSTR /*fieldName*/, VARTYPE* /*type*/, DWORD* /*size*/, DWORD* /*offset*/);
typedef BOOL (*DkGetSequenceFieldInfo_ptr_t)(PCSTR /*sequenceName*/, PCSTR /*fieldName*/, VARTYPE* /*type*/, DWORD* /*size*/, DWORD* /*offset*/);

/* Added for Meijer SOTF7062 */
typedef BOOL (*DkGetContextFieldInfoEx_ptr_t)(PCSTR /*contextName*/, PCSTR /*fieldName*/, VARTYPE* /*type*/, DWORD* /*size*/, DWORD* /*offset*/, DWORD* /*index*/);

/* Added for Meijer SOTF7062 */
typedef BOOL (*DkGetSequenceFieldInfoEx_ptr_t)(PCSTR /*sequenceName*/, PCSTR /*fieldName*/, VARTYPE* /*type*/, DWORD* /*size*/, DWORD* /*offset*/, DWORD* /*index*/);

typedef BOOL (*DkSetTraceInstanceName_ptr)(PCSTR /*name*/);

typedef HRESULT (*DkLoadContextHeader_ptr)(PCSTR /*configPathInit*/);
// typedef HRESULT (*DkLoadSequenceHeader_ptr)(PCSTR /*configPathInit*/);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <comutil.h>
#include <cassert>
#include <string>

/* C++ classes that wrap the raw C API and make it easier and safer to use. 
If you decide to change any of these classes, please be aware that they 
may be used inside the API code. */

namespace DataKit
{
   class DkException : public std::runtime_error 
   {
   public:
      HRESULT result;

      explicit DkException(const std::string& s, HRESULT resultInit) : runtime_error(s), result(resultInit) 
      {
      }

      virtual ~DkException()
      {
      }
   };

   /* Initialize the ACSIODataKit library and ensure that it is properly 
   unitialized when the destructor is called. */
   class InitException : public DkException
   {
   public:
      explicit InitException(const std::string& s, HRESULT resultInit) : DkException(s, resultInit) 
      {
      }

      virtual ~InitException()
      {
      }
   };

   class Initializer
   {
      HRESULT result;

   public:
      Initializer() : result(DkInitialize())
      {
         if (!SUCCEEDED(result))
         {
            throw InitException("Failure calling DkInitialize", result);
         }
      }

      ~Initializer()
      {
         DkUninitialize();
      }

      operator bool()
      {
         return !!SUCCEEDED(result);
      }
   };

   /* Load context structure definitions and ensure that they are 
   unloaded by the destructor. */
   class ContextException : public DkException
   {
   public:
      HRESULT result;

      explicit ContextException(const std::string& s, HRESULT resultInit) : DkException(s, resultInit) 
      {
      }

      virtual ~ContextException()
      {
      }
   };

   class ContextStructureLoader
   {
      HRESULT result;

   public:
      ContextStructureLoader() : result(DkLoadContextStructures())
      {
         if (!SUCCEEDED(result))
         {
            throw ContextException("Failure calling DkLoadContextStructures", result);
         }
      }

      ~ContextStructureLoader()
      {
         DkUnloadContextStructures();
      }

      operator bool()
      {
         return !!SUCCEEDED(result);
      }
   };


   /* Load sequence structure definitions and ensure that they are 
   unloaded by the destructor. */
   class SequenceException : public DkException
   {
   public:
      HRESULT result;

      explicit SequenceException(const std::string& s, HRESULT resultInit) : DkException(s, resultInit) 
      {
      }

      virtual ~SequenceException()
      {
      }
   };

   class SequenceStructureLoader
   {
      HRESULT result;

   public:
      SequenceStructureLoader() : result(DkLoadSequenceStructures())
      {
         if (!SUCCEEDED(result))
         {
            throw SequenceException("Failure calling DkLoadSequenceStructures", result);
         }
      }

      ~SequenceStructureLoader()
      {
         DkUnloadSequenceStructures();
      }

      operator bool()
      {
         return !!SUCCEEDED(result);
      }
   };


   typedef HANDLE (*DkAllocFieldIterator_ptr_t)(PCSTR /*name*/);
   typedef void (*DkFreeFieldIterator_ptr_t)(HANDLE /*handle*/);
   typedef BOOL (*DkQueryFieldIterator_ptr_t)(HANDLE /*iterator*/, LPCSTR* /*fieldName*/, VARTYPE* /*type*/, DWORD* /*size*/, DWORD* /*offset*/);
   typedef DWORD (*DkGetFieldCount_ptr_t)(PCSTR /*structName*/);
   
   template<
      DkAllocFieldIterator_ptr_t DkAllocIterator, 
      DkFreeFieldIterator_ptr_t DkFreeIterator, 
      DkQueryFieldIterator_ptr_t DkQueryIterator,
      DkGetFieldCount_ptr_t DkGetFieldCount
      >
   class FieldEnum
   {
      mutable HANDLE iter;

      std::string structName;
      mutable std::string fieldName;
      mutable VARTYPE type;
      mutable DWORD size;
      mutable DWORD offset;

   public:
      FieldEnum(PCSTR structNameInit) : 
         iter(0), 
         structName(structNameInit),
         type(VT_EMPTY),
         size(0),
         offset(0)
      {
      }

      FieldEnum(FieldEnum const& rhs) : 
         iter(0), 
         structName(rhs.structName),
         type(VT_EMPTY),
         size(0),
         offset(0)
      {
      }

      ~FieldEnum()
      {
         if (iter)
         {
            DkFreeIterator(iter);
         }
      }

      FieldEnum& operator=(FieldEnum const& rhs)
      {
         if (iter)
         {
            DkFreeIterator(iter);
         iter = 0;
         }

         structName = rhs.structName;
         fieldName.erase();
         type = VT_EMPTY;
         size = 0;
         offset = 0;
         return *this;
      }

      BOOL Next() const
      {
         if (!iter)
         {
            iter = DkAllocIterator(structName.c_str());
         }

         BOOL retVal = !!iter;
         
         if (iter)
         {
            fieldName.erase();

            LPCSTR pFieldName = 0;
            VARTYPE localType = VT_EMPTY;
            DWORD localSize = 0;
            DWORD localOffset;

            retVal = DkQueryIterator(iter, &pFieldName, &localType, &localSize, &localOffset);
         
            if (retVal)
            {
               fieldName = pFieldName;
            }

            type = localType;
            size = localSize;
            offset = localOffset;
         }

         return retVal;
      }

      HRESULT Reset() const
      {
         DkFreeIterator(iter);
         iter = DkAllocIterator(structName.c_str());

         fieldName.erase();
         type = VT_EMPTY;
         size = 0;
         offset = 0;

         return iter ? S_OK : E_FAIL;
      }

      LPCSTR Name() const
      {
         return fieldName.c_str();
      }

      VARTYPE Type() const
      {
         return type;
      }

      DWORD Size() const
      {
         return size;
      }

      DWORD Offset() const
      {
         return offset;
      }

      DWORD Count() const
      {
         return DkGetFieldCount(structName.c_str());
      }
   };


   /* The ContextFieldEnum type is used to query, one at a time, information about
   the fields in the specified structure. */
   typedef FieldEnum<
      DkAllocContextFieldIterator, 
      DkFreeContextFieldIterator, 
      DkQueryContextFieldIterator,
      DkGetContextFieldCount
      > ContextFieldEnum;

   /* The SequenceFieldEnum type is used to query, one at a time, information about
   the fields in the specified structure. */
   typedef FieldEnum<
      DkAllocSequenceFieldIterator, 
      DkFreeSequenceFieldIterator, 
      DkQuerySequenceFieldIterator,
      DkGetSequenceFieldCount
      > SequenceFieldEnum;


   typedef HANDLE (*DkAllocIterator_ptr_t)();
   typedef void (*DkFreeIterator_ptr_t)(HANDLE /*handle*/);
   typedef BOOL (*DkQueryIterator_ptr_t)(HANDLE /*iterator*/, LPCSTR* /*structName*/, DWORD* /*fieldCount*/);
   typedef DWORD (*DkGetCount_ptr_t)(void);


   template<
      DkAllocIterator_ptr_t DkAllocIterator, 
      DkFreeIterator_ptr_t DkFreeIterator, 
      DkQueryIterator_ptr_t DkQueryIterator,
      DkGetCount_ptr_t DkGetCount
      >
   class StructureEnum
   {
      mutable HANDLE iter;

      mutable std::string structName;
      mutable DWORD fieldCount;

   public:
      StructureEnum() : iter(0)
      {
      }

      StructureEnum(StructureEnum const& rhs) : iter(0), fieldCount(0)
      {
         structName.erase();
      }

      ~StructureEnum()
      {
         if (iter)
         {
            DkFreeIterator(iter);
         }
      }

      BOOL Next() const
      {
         if (!iter)
         {
            iter = DkAllocIterator();
         }

         BOOL retVal = !!iter;
         
         if (iter)
         {
            structName.erase();

            LPCSTR pStructName = 0;
            DWORD localFieldCount = 0;

            retVal = DkQueryIterator(iter, &pStructName, &localFieldCount);
         
            if (retVal)
            {
               structName = pStructName;
            }

            fieldCount = localFieldCount;
         }

         return retVal;
      }

      HRESULT Reset() const
      {
         DkFreeIterator(iter);
         iter = DkAllocIterator();

         structName.erase();
         fieldCount = 0;

         return iter ? S_OK : E_FAIL;
      }

      LPCSTR Name() const
      {
         return structName.c_str();
      }

      DWORD FieldCount() const
      {
         return fieldCount;
      }

      DWORD Count() const
      {
         return DkGetCount();
      }
   };


   /* The ContextStructureEnum type is used to query, one at a time, the names 
   of the structures loaded from SALESLNF.H */
   typedef StructureEnum<
      DkAllocContextIterator, 
      DkFreeContextIterator, 
      DkQueryContextIterator,
      DkGetContextCount
      > ContextStructureEnum;

   /* The SequenceStructureEnum type is used to query, one at a time, the names 
   of the structures loaded from ASVGSD.H */
   typedef StructureEnum<
      DkAllocSequenceIterator, 
      DkFreeSequenceIterator, 
      DkQuerySequenceIterator,
      DkGetSequenceCount
      > SequenceStructureEnum;



   typedef HRESULT (*DkGetField_ptr_t)(PCSTR, PCSTR, LPCVOID, VARIANT*);

   /* Generic structure reader and writer class behaves the same way when 
   specialized for contexts or structures. The only differences are the API 
   functions that are called. Specializations follow below. */
   template<DkGetField_ptr_t DkGetField> 
   class Reader
   {
   private:
      std::string name;
      LPVOID data;

   protected:
      std::string& Name_()
      {
         return name;
      }

      LPVOID& Data_()
      {
         return data;
      }

   public:
      Reader(PCSTR nameInit, LPCVOID dataInit) : name(nameInit), data(const_cast<LPVOID>(dataInit))
      {
         assert(data);
      }

      Reader(std::string const& nameInit, LPCVOID dataInit) : name(nameInit), data(const_cast<LPVOID>(dataInit))
      {
         assert(data);
      }

      Reader(Reader const& rhs) : name(rhs.name), data(rhs.data)
      {
         assert(data);
      }

      virtual Reader& operator=(Reader const& rhs)
      {
         name = rhs.name;
         data = rhs.data;
         assert(data);
         return *this;
      }

      virtual ~Reader()
      {
      }

      operator LPCVOID() const
      {
         return Data();
      }

      LPCVOID Data() const
      {
         return data;
      }

      PCSTR Name() const
      {
         return name.c_str();
      }

      virtual HRESULT Get(PCSTR fieldName, /*out*/ VARIANT* pVal) const
      {
         return DkGetField(name.c_str(), fieldName, data, pVal);
      }

      virtual _variant_t Get(PCSTR fieldName) const
      {
         _variant_t value;
         Get(fieldName, &value);
         return value.Detach();
      }
   };


   typedef LPVOID (*DkAlloc_ptr_t)(PCSTR);
   typedef HLOCAL (*DkFree_ptr_t)(LPVOID);
   typedef BOOL (*DkCopy_ptr_t)(LPCVOID, LPVOID);
   typedef HRESULT (*DkSetField_ptr_t)(PCSTR, VARIANT, LPVOID);

   template<
      DkAlloc_ptr_t DkAlloc, 
      DkFree_ptr_t DkFree, 
      DkCopy_ptr_t DkCopy, 
      DkGetField_ptr_t DkGetField, 
      DkSetField_ptr_t DkSetField
      > 
   class Writer : public Reader<DkGetField>
   {
   public:
      Writer(PCSTR nameInit, LPCVOID source = 0) : Reader<DkGetField>(nameInit, DkAlloc(nameInit))
      {
         assert(Data_());

         if (source)
         {
            DkCopy(source, Data_());
         }
      }

      Writer(Writer const& rhs) : Reader<DkGetField>(rhs.Name(), DkAlloc(rhs.Name()))
      {
         BOOL success = DkCopy(rhs.Data(), Data_());
         assert(success);
      }

      Writer(Reader const& rhs) : Reader<DkGetField>(rhs.Name(), DkAlloc(rhs.Name()))
      {
         assert(Data_());
         BOOL success = DkCopy(rhs.Data(), Data_());
         assert(success);
      }

      Writer& operator=(Writer const& rhs)
      {
         Name_() = rhs.Name();
         Data_() = DkAlloc(rhs.Name());
         
         assert(Data_());

         BOOL success = DkCopy(rhs.Data(), Data_());
         assert(success);

         return *this;
      }

      Writer& operator=(LPCVOID source)
      {
         assert(Data_());

         BOOL success = DkCopy(source, Data_());
         assert(success);

         return *this;
      }

      virtual ~Writer()
      {
         DkFree(Data_());
      }

      HRESULT Set(PCSTR fieldName, _variant_t value)
      {
         return DkSetField(fieldName, value, Data_());
      }
   };


   /* The ContextReader type is used to read field values from a structure buffer. 
   The buffer need not have been allocated by DkAllocContext. */
   typedef Reader<
      DkGetContextFieldEx
      > ContextReader;

   /* The ContextWriter type allocates a structure buffer with DkAllocContext. 
   It does not accept buffers allocated externally*/
   typedef Writer<
      DkAllocContext, 
      DkFreeContext, 
      DkCopyContext, 
      DkGetContextFieldEx, 
      DkSetContextField
      > ContextWriter;

   /* The SequenceReader type is used to read field values from a structure buffer. 
   The buffer need not have been allocated by DkAllocSequence. */
   typedef Reader<
      DkGetSequenceFieldEx
      > SequenceReader;

   /* The SequenceWriter type allocates a structure buffer with DkAllocSequence. 
   It does not accept buffers allocated externally*/
   typedef Writer<
      DkAllocSequence, 
      DkFreeSequence, 
      DkCopySequence, 
      DkGetSequenceFieldEx, 
      DkSetSequenceField
      > SequenceWriter;

}

#endif

#endif // H_ACSIODATAKIT
