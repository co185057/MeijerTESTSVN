#include "stdafx.h"
#include "DataKit.h"
#include "Utils.h"
#include <cassert>
#include "TraceImpl.h"

namespace DataKit
{
   namespace
   {
      bool isInitialized = false;
      HMODULE lib = 0;
   }

   bool IsInitialized()
   {
      return isInitialized;
   }

   bool Initialize()
   {
      Trace trace(_T("Initialize"));

      if (!isInitialized)
      {
         HRESULT hr = DkInitialize();
         isInitialized = SUCCEEDED(hr);
      }

      return IsInitialized();
   }

   void Uninitialize()
   {
      Trace trace(_T("Uninitialize"));
		isInitialized = false;
      DkUninitialize();
   }

   HRESULT LoadContextStructures()
   {
      return DkLoadContextStructures();
   }

   HRESULT UnloadContextStructures()
   {
      return DkUnloadContextStructures();
   }

   HRESULT LoadSequenceStructures()
   {
      return DkLoadSequenceStructures();
   }

   HRESULT UnloadSequenceStructures()
   {
      return DkUnloadSequenceStructures();
   }

   LPVOID AllocContext(PCSTR contextName)
   {
      return DkAllocContext(contextName);
   }

   HLOCAL FreeContext(LPVOID buffer)
   {
      return DkFreeContext(buffer);
   }

   LPVOID AllocSequence(PCSTR sequenceName)
   {
      return DkAllocSequence(sequenceName);
   }

   HLOCAL FreeSequence(LPVOID buffer)
   {
      return DkFreeSequence(buffer);
   }

   HRESULT SetContextField(PCWSTR fieldName, _variant_t value, LPVOID buffer)
   {
      CStringA fieldNameA(fieldName);
      return DkSetContextField(fieldNameA, value, buffer);
   }

   HRESULT SetContextField(PCSTR fieldName, _variant_t value, LPVOID buffer)
   {
      return DkSetContextField(fieldName, value, buffer);
   }

   _variant_t GetContextField(PCWSTR contextName, PCWSTR fieldName, LPCVOID buffer)
   {
      CStringA contextNameA(contextName);
      CStringA fieldNameA(fieldName);
      return DkGetContextField(contextNameA, fieldNameA, buffer);
   }

   _variant_t GetContextField(PCSTR contextName, PCSTR fieldName, LPCVOID buffer)
   {
      return DkGetContextField(contextName, fieldName, buffer);
   }

   HRESULT SetSequenceField(PCWSTR fieldName, _variant_t value, LPVOID buffer)
   {
      CStringA fieldNameA(fieldName);
      return DkSetSequenceField(fieldNameA, value, buffer);
   }

   HRESULT SetSequenceField(PCSTR fieldName, _variant_t value, LPVOID buffer)
   {
      return DkSetSequenceField(fieldName, value, buffer);
   }

   _variant_t GetSequenceField(PCWSTR sequenceName, PCWSTR fieldName, LPCVOID buffer)
   {
      CStringA sequenceNameA(sequenceName);
      CStringA fieldNameA(fieldName);
      return DkGetSequenceField(sequenceNameA, fieldNameA, buffer);
   }

   _variant_t GetSequenceField(PCSTR sequenceName, PCSTR fieldName, LPCVOID buffer)
   {
      return DkGetSequenceField(sequenceName, fieldName, buffer);
   }

   LPCSTR GetSequenceName(DWORD sequenceID)
   {
      return DkGetSequenceName(sequenceID);
   }

   DWORD GetSequenceID(LPCWSTR sequenceName)
   {
      CStringA sequenceNameA(sequenceName);
      return DkGetSequenceID(sequenceNameA);
   }

   DWORD GetSequenceID(LPCSTR sequenceName)
   {
      return DkGetSequenceID(sequenceName);
   }

   HANDLE AllocSequenceIterator()
   {
      return DkAllocSequenceIterator();
   }

   void FreeSequenceIterator(HANDLE handle)
   {
      DkFreeSequenceIterator(handle);
   }

   BOOL QuerySequenceIterator(HANDLE iterator, LPCSTR* structName, DWORD* fieldCount)
   {
      return DkQuerySequenceIterator(iterator, structName, fieldCount);
   }

   HANDLE AllocContextIterator()
   {
      return DkAllocContextIterator();
   }

   void FreeContextIterator(HANDLE handle)
   {
      DkFreeContextIterator(handle);
   }

   BOOL QueryContextIterator(HANDLE iterator, LPCSTR* structName, DWORD* fieldCount)
   {
      return DkQueryContextIterator(iterator, structName, fieldCount);
   }

   HANDLE AllocSequenceFieldIterator(PCSTR name)
   {
      return DkAllocSequenceFieldIterator(name);
   }

   void FreeSequenceFieldIterator(HANDLE handle)
   {
      DkFreeSequenceFieldIterator(handle);
   }

   BOOL QuerySequenceFieldIterator(HANDLE iterator, LPCSTR* fieldName, VARTYPE* type, DWORD* size, DWORD* offset)
   {
      return DkQuerySequenceFieldIterator(iterator, fieldName, type, size, offset);
   }

   HANDLE AllocContextFieldIterator(PCSTR name)
   {
      return DkAllocContextFieldIterator(name);
   }

   void FreeContextFieldIterator(HANDLE handle)
   {
      DkFreeContextFieldIterator(handle);
   }

   BOOL QueryContextFieldIterator(HANDLE iterator, LPCSTR* fieldName, VARTYPE* type, DWORD* size, DWORD* offset)
   {
      return DkQueryContextFieldIterator(iterator, fieldName, type, size, offset);
   }

   BOOL CopyContext(LPCVOID source, LPVOID dest)
   {
      return DkCopyContext(source, dest);
   }

   BOOL CopySequence(LPCVOID source, LPVOID dest)
   {
      return DkCopySequence(source, dest);
   }

   DWORD GetContextSize(PCSTR contextName)
   {
      return DkGetContextSize(contextName);
   }

   DWORD GetContextSize(PCWSTR contextName)
   {
      CStringA contextNameA(contextName);
      return DkGetContextSize(contextNameA);
   }

   DWORD GetSequenceSize(PCSTR sequenceName)
   {
      return DkGetSequenceSize(sequenceName);
   }

   DWORD GetSequenceSize(PCWSTR sequenceName)
   {
      CStringA sequenceNameA(sequenceName);
      return DkGetSequenceSize(sequenceNameA);
   }

   DWORD GetContextCount(void)
   {
      return DkGetContextCount();
   }

   DWORD GetSequenceCount(void)
   {
      return DkGetSequenceCount();
   }

   DWORD GetContextFieldCount(PCSTR contextName)
   {
      return DkGetContextFieldCount(contextName);
   }

   DWORD GetContextFieldCount(PCWSTR contextName)
   {
      CStringA contextNameA(contextName);
      return DkGetContextFieldCount(contextNameA);
   }

   DWORD GetSequenceFieldCount(PCSTR sequenceName)
   {
      return DkGetSequenceFieldCount(sequenceName);
   }

   DWORD GetSequenceFieldCount(PCWSTR sequenceName)
   {
      CStringA sequenceNameA(sequenceName);
      return DkGetSequenceFieldCount(sequenceNameA);
   }

   BOOL GetContextInfo(PCSTR structName, DWORD* fieldCount)
   {
      return DkGetContextInfo(structName, fieldCount);
   }

   BOOL GetContextInfo(PCWSTR structName, DWORD* fieldCount)
   {
      CStringA structNameA(structName);
      return DkGetContextInfo(structNameA, fieldCount);
   }

   BOOL GetSequenceInfo(PCSTR structName, DWORD* fieldCount)
   {
      return DkGetSequenceInfo(structName, fieldCount);
   }

   BOOL GetSequenceInfo(PCWSTR structName, DWORD* fieldCount)
   {
      CStringA structNameA(structName);
      return DkGetSequenceInfo(structNameA, fieldCount);
   }

   BOOL GetContextFieldInfo(PCSTR contextName, PCSTR fieldName, VARTYPE* type, DWORD* size, DWORD* offset)
   {
      return DkGetContextFieldInfo(contextName, fieldName, type, size, offset);
   }

   BOOL GetContextFieldInfo(PCWSTR contextName, PCWSTR fieldName, VARTYPE* type, DWORD* size, DWORD* offset)
   {
      CStringA contextNameA(contextName);
      CStringA fieldNameA(fieldName);
      return DkGetContextFieldInfo(contextNameA, fieldNameA, type, size, offset);
   }

   BOOL GetSequenceFieldInfo(PCSTR sequenceName, PCSTR fieldName, VARTYPE* type, DWORD* size, DWORD* offset)
   {
      return DkGetSequenceFieldInfo(sequenceName, fieldName, type, size, offset);
   }

   BOOL GetSequenceFieldInfo(PCWSTR sequenceName, PCWSTR fieldName, VARTYPE* type, DWORD* size, DWORD* offset)
   {
      CStringA sequenceNameA(sequenceName);
      CStringA fieldNameA(fieldName);
      return DkGetSequenceFieldInfo(sequenceNameA, fieldNameA, type, size, offset);
   }
};
