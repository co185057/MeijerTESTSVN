#pragma once

#include <comutil.h>
#include <string>
#include <exception>
#include "ACSIODataKit.h"

namespace DataKit
{
   bool IsInitialized();
   bool Initialize();
   void Uninitialize();

   HRESULT LoadContextStructures();
   HRESULT UnloadContextStructures();

   HRESULT LoadSequenceStructures();
   HRESULT UnloadSequenceStructures();

   LPVOID AllocContext(PCSTR contextName);
   HLOCAL FreeContext(LPVOID buffer);

   LPVOID AllocSequence(PCSTR sequenceName);
   HLOCAL FreeSequence(LPVOID buffer);

   HRESULT SetContextField(PCWSTR fieldName, _variant_t value, LPVOID buffer);
   HRESULT SetContextField(PCSTR fieldName, _variant_t value, LPVOID buffer);
   _variant_t GetContextField(PCWSTR contextName, PCWSTR fieldName, LPCVOID buffer);
   _variant_t GetContextField(PCSTR contextName, PCSTR fieldName, LPCVOID buffer);

   HRESULT SetSequenceField(PCWSTR fieldName, _variant_t value, LPVOID buffer);
   HRESULT SetSequenceField(PCSTR fieldName, _variant_t value, LPVOID buffer);
   _variant_t GetSequenceField(PCWSTR sequenceName, PCWSTR fieldName, LPCVOID buffer);
   _variant_t GetSequenceField(PCSTR sequenceName, PCSTR fieldName, LPCVOID buffer);

   LPCSTR GetSequenceName(DWORD sequenceID);
   DWORD GetSequenceID(LPCWSTR sequenceName);
   DWORD GetSequenceID(LPCSTR sequenceName);

   HANDLE AllocSequenceIterator();
   void FreeSequenceIterator(HANDLE handle);
   BOOL QuerySequenceIterator(HANDLE iterator, LPCSTR* structName, DWORD* fieldCount);

   HANDLE AllocContextIterator();
   void FreeContextIterator(HANDLE handle);
   BOOL QueryContextIterator(HANDLE iterator, LPCSTR* structName, DWORD* fieldCount);

   HANDLE AllocSequenceFieldIterator(PCSTR name);
   void FreeSequenceFieldIterator(HANDLE handle);
   BOOL QuerySequenceFieldIterator(HANDLE iterator, LPCSTR* fieldName, VARTYPE* type, DWORD* size, DWORD* offset);

   HANDLE AllocContextFieldIterator(PCSTR name);
   void FreeContextFieldIterator(HANDLE handle);
   BOOL QueryContextFieldIterator(HANDLE iterator, LPCSTR* fieldName, VARTYPE* type, DWORD* size, DWORD* offset);

   BOOL CopyContext(LPCVOID source, LPVOID dest);
   BOOL CopySequence(LPCVOID source, LPVOID dest);

   DWORD GetContextSize(PCSTR contextName);
   DWORD GetContextSize(PCWSTR contextName);

   DWORD GetSequenceSize(PCSTR sequenceName);
   DWORD GetSequenceSize(PCWSTR sequenceName);

   DWORD GetContextCount(void);
   DWORD GetSequenceCount(void);

   DWORD GetContextFieldCount(PCSTR contextName);
   DWORD GetContextFieldCount(PCWSTR contextName);

   DWORD GetSequenceFieldCount(PCSTR sequenceName);
   DWORD GetSequenceFieldCount(PCWSTR sequenceName);

   BOOL GetContextInfo(PCSTR structName, DWORD* fieldCount);
   BOOL GetContextInfo(PCWSTR structName, DWORD* fieldCount);

   BOOL GetSequenceInfo(PCSTR structName, DWORD* fieldCount);
   BOOL GetSequenceInfo(PCWSTR structName, DWORD* fieldCount);

   BOOL GetContextFieldInfo(PCSTR contextName, PCSTR fieldName, VARTYPE* type, DWORD* size, DWORD* offset);
   BOOL GetContextFieldInfo(PCWSTR contextName, PCWSTR fieldName, VARTYPE* type, DWORD* size, DWORD* offset);

   BOOL GetSequenceFieldInfo(PCSTR sequenceName, PCSTR fieldName, VARTYPE* type, DWORD* size, DWORD* offset);
   BOOL GetSequenceFieldInfo(PCWSTR sequenceName, PCWSTR fieldName, VARTYPE* type, DWORD* size, DWORD* offset);

   class ContextBuffer
   {
      std::string name;
      LPVOID data;

   public:
      ContextBuffer(PCSTR nameInit) : name(nameInit), data(AllocContext(nameInit))
      {
         _ASSERT(data);
      }

      ContextBuffer(ContextBuffer const& rhs) : name(rhs.name), data(AllocContext(name.c_str()))
      {
         BOOL success = CopyContext(rhs.data, data);
         _ASSERT(success);
      }

      ContextBuffer& operator=(ContextBuffer const& rhs)
      {
         name = rhs.name;
         data = AllocContext(name.c_str());
         
         _ASSERT(data);

         BOOL success = CopyContext(rhs.data, data);
         _ASSERT(success);
      }

      ~ContextBuffer()
      {
         if (data)
         {
            FreeContext(data);
         }
      }

      PCSTR Name()
      {
         return name.c_str();
      }

      HRESULT Set(PCSTR fieldName, _variant_t value)
      {
         return SetContextField(fieldName, value, data);
      }

      HRESULT Set(PCWSTR fieldName, _variant_t value)
      {
         return SetContextField(fieldName, value, data);
      }
   };


   class SequenceBuffer
   {
      std::string name;
      LPVOID data;

   public:
      SequenceBuffer(PCSTR nameInit) : name(nameInit), data(AllocSequence(nameInit))
      {
         _ASSERT(data);
      }

      SequenceBuffer(SequenceBuffer const& rhs) : name(rhs.name), data(AllocSequence(name.c_str()))
      {
         BOOL success = CopySequence(rhs.data, data);
         _ASSERT(success);
      }

      SequenceBuffer& operator=(SequenceBuffer const& rhs)
      {
         name = rhs.name;
         data = AllocSequence(name.c_str());

         _ASSERT(data);

         BOOL success = CopySequence(rhs.data, data);
         _ASSERT(success);
      }

      ~SequenceBuffer()
      {
         if (data)
         {
            FreeSequence(data);
         }
      }

      operator LPVOID()
      {
         return data;
      }

      PCSTR Name()
      {
         return name.c_str();
      }

      DWORD ID()
      {
         return GetSequenceID(name.c_str());
      }

      HRESULT Set(PCSTR fieldName, _variant_t value)
      {
         return SetSequenceField(fieldName, value, data);
      }

      HRESULT Set(PCWSTR fieldName, _variant_t value)
      {
         return SetSequenceField(fieldName, value, data);
      }
   };
}
