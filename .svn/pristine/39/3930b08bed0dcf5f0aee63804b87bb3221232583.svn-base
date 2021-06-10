/////////////////////////////////////////////////////////////////////////////
//
// COposRegistry.h
//
// Declaration of COposRegistry class.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//
#ifndef OPOSREGISTRY_H
#define OPOSREGISTRY_H

#define REG_DEFAULT_INT    0x80000000
#define REG_DEFAULT_STRING  _T("??")

class COposRegistry            
   {
   public:
      // Allow OPOS service to register as threading model.
      static HRESULT  RegisterThreadingModelSO( const IID& iidData );
      static BOOL GetFileVersionInfo( const IID&  iidData, CString& sVersion, CmDataCapture& cmDc );
#ifdef _DEBUG
      static void AssertThreadingModelSO( const IID& iidData ); 
#endif
   public:
      COposRegistry( LPCTSTR sClass, LPCTSTR sName, CmDataCapture& cmDc );
      COposRegistry( const COposRegistry& oRegistry );
      COposRegistry& operator=( const COposRegistry& oRegistry )
         {
         m_sClass = oRegistry.m_sClass;
         m_sName  = oRegistry.m_sName;
         m_sBaseKey = oRegistry.m_sBaseKey;
         OpenKey(); 
         return *this;
         }
      void SetBaseKey( LPCTSTR sKey ) { m_sBaseKey = sKey; }
      CString GetKeyName( void ) const ;
      CString GetBaseKeyName( void ) const; 
      CString GetSoName( void ) ;
      CString GetName( void ) const { return m_sName; }
      CString GetClass( void ) const { return m_sClass; }
      BOOL OpenKey( void ) ;
      BOOL OpenKeyCO( void ) ;
      void Close( void ) { m_cmRegistry.CloseKey(); }
      BOOL Redirect( const CString& sClass = _T(""), int nCommentId = 0 );
      CString  WriteSoDescription( LPCTSTR sDescription, int nVersion );
      // Everybody uses Port
      int ReadPort( int nCommentId = 0, LPCTSTR sPrefix = _T("COM") );
      BOOL ReadBool( LPCTSTR  pszValue,  int nCommentId = 0, BOOL bDefault = FALSE );
      BOOL ReadString( LPCTSTR  pszValue, CString& sResult, 
         int nCommentId = 0, LPCTSTR szDefault = REG_DEFAULT_STRING );
      int ReadInt( LPCTSTR  pszValue, int nCommentId = 0, int nDefault = REG_DEFAULT_INT );
      void WriteRegString( LPCTSTR pszValue, LPCTSTR sResult, int nCommentId = 0);
      void WriteRegInt( LPCTSTR pszValue, int nResult, int nCommentId = 0, LPCTSTR sPrefix = _T("") );
      BOOL IsOpen( void ) { return m_cmRegistry.IsOpen(); }
      BOOL OpenSubKey( LPCTSTR sSubKey );
      LONG RegEnumValue( DWORD dwIndex, CString& sValueName, CString& sValue,
         int nCommentId = 0 );
      CString RegEnumKey( DWORD dwIndex );
      int ReadIntRange( LPCTSTR szValue, int nDefault = REG_DEFAULT_INT,
         int nMin = INT_MIN, int nMax = INT_MAX, int nComment = 0 );
      DWORD ReadDwordRange( LPCTSTR szValue, DWORD nDefault = REG_DEFAULT_INT,
         DWORD nMin = 0, DWORD nMax = ULONG_MAX, int nComment = 0 );
   protected:
      CmDataCapture& m_cmDc;
      CmRegistry     m_cmRegistry;
   private:
      CString        m_sClass;
      CString        m_sName;
      CString        m_sBaseKey;
   };

extern CString RegKeyFromGuid( const IID& iidData );

#ifdef _OPOS_ENABLE_INLINES
#define OPOS_INLINE inline
#include "OposRegistry.inl"
#endif

#endif