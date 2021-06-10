/////////////////////////////////////////////////////////////////////////////
//
// OposRegistry.inl
//
// Inlines of COposRegistry class.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#ifdef OPOS_INLINE 
   
OPOS_INLINE COposRegistry::COposRegistry( 
   LPCTSTR sClass, 
   LPCTSTR sName,
   CmDataCapture& cmDc ) 
   : m_sClass( sClass ), 
   m_sName( sName ), 
   m_sBaseKey( OPOS_ROOTKEY ),
   m_cmDc( cmDc ) 
   { 
   ASSERT( sClass != NULL );
   ASSERT( sName != NULL );
   }

// COposRegistry::COposRegistry
//
// Purpose: Copy Constructor.  
// Notes:   1) Fixes bug in CmRegistry, which copies the HKEY value.  
//          2) We create a copy of the CmRegistry, then re-open 
//          the new object to create a unique hKey!
//          

OPOS_INLINE COposRegistry::COposRegistry( 
   const COposRegistry& oRegistry )
   : m_sClass( oRegistry.m_sClass ), 
   m_sName( oRegistry.m_sName ), 
   m_sBaseKey( oRegistry.m_sBaseKey ),
   m_cmRegistry(  ),          // Must decouple from CmRegistry::m_hKey
   m_cmDc( oRegistry.m_cmDc ) 
   {    
   OpenKey(); 
   return;
   }

OPOS_INLINE CString COposRegistry::GetBaseKeyName( void ) const 
   { 
   CString sResult = m_sBaseKey;
   if ( m_sClass.IsEmpty() == FALSE )
      sResult += _T('\\') + m_sClass;  
   return sResult;
   }

OPOS_INLINE CString COposRegistry::GetKeyName( 
   void ) const 
   { 
   CString sResult = GetBaseKeyName();
   if ( m_sName.IsEmpty() == FALSE )
      sResult += _T('\\') + m_sName;  
   return sResult; 
   }


// COposRegistry::OpenKey 
// 
// Purpose: Open the OPOS key specified by Class and DeviceName
//



OPOS_INLINE BOOL COposRegistry::OpenKey( 
   void  ) 
   { 
   BOOL bResult = m_cmRegistry.OpenKey( HKEY_LOCAL_MACHINE, GetKeyName() ); 
   if ( bResult == FALSE )
      m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL4, 
         _T("* \"%s\" not found"), (LPCTSTR)GetKeyName() );
   return bResult;
   } 


OPOS_INLINE CString RegKeyFromGuid( 
   const IID& iidData ) 
   {
   CString sKey;
   sKey.Format( 
      _T("SOFTWARE\\Classes\\CLSID\\")
      _T("{%08.8X-%04.4X-%04.4X-%02.2X%02.2X-%02.2X%02.2X%02.2X%02.2X%02.2X%02.2X}")
      _T("\\InProcServer32"),
     iidData.Data1, iidData.Data2, iidData.Data3,
     iidData.Data4[0], iidData.Data4[1], iidData.Data4[2], iidData.Data4[3],
     iidData.Data4[4], iidData.Data4[5], iidData.Data4[6], iidData.Data4[7] );
   return sKey;
   }


// COposRegistry::OpenSubKey 
// 
// Purpose: Open the sub key specified by Class\Profile and sSubKey 
//

OPOS_INLINE BOOL COposRegistry::OpenSubKey( 
   LPCTSTR sSubKey )
   {
   m_sClass += _T('\\') + m_sName;
   m_sName = sSubKey;
   return OpenKey();
   }


// COposRegistry::RegEnumValue 
// 
// Purpose: Enumerate values from the registry
//

OPOS_INLINE long COposRegistry::RegEnumValue( 
   DWORD    dwIndex, 
   CString& sValueName, 
   CString& sValue,
   int      nCommentId  /* = 0   */ )
   {
   long lResult = ERROR_INVALID_FUNCTION;
   if ( IsOpen() != FALSE )
      {
      BYTE  tcValue[1024];
      TCHAR tcValueName[50];
      DWORD dwType;
      DWORD dwNameSize = sizeof( tcValueName );
      DWORD dwSize = sizeof( tcValue );

      lResult = ::RegEnumValue( 
         m_cmRegistry.GetHKey(),
         dwIndex, 
         tcValueName,
         &dwNameSize,
         NULL,          // Reserved
         &dwType,
         tcValue,
         &dwSize );
      if ( lResult == ERROR_SUCCESS )
         {
         sValueName = tcValueName;
         if ( dwType != REG_SZ )
            lResult = ERROR_NOT_SUPPORTED; 
         else
            ReadString( sValueName, sValue, nCommentId );
         }
      }
   return lResult;
   }

// COposRegistry::RegEnumKey
// 
// Purpose: Enumerate subkeys from the registry
//

OPOS_INLINE CString COposRegistry::RegEnumKey( 
   DWORD dwIndex )
   {
   CString sResult;
   TCHAR tcKeyName[50];
   DWORD dwSize = sizeof( tcKeyName );

   if ( IsOpen() != FALSE 
      && ::RegEnumKey( m_cmRegistry.GetHKey(), 
         dwIndex, tcKeyName, dwSize ) == ERROR_SUCCESS ) 
      {
      sResult = tcKeyName;
      }
   return sResult;
   }


#endif
