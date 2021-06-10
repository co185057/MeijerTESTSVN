/////////////////////////////////////////////////////////////////////////////
//
// DeviceMap.h
//
// Declaration of TOposDeviceMap Template class.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#ifndef DEVICE_MAP_H
#define DEVICE_MAP_H


template<class DEVICE_TYPE>
class TOposDeviceMap : public CCriticalSection
   {
   public:
      TOposDeviceMap();
      ~TOposDeviceMap();
      void AddReference( int nPort, DEVICE_TYPE* pDevice );
      void DeleteReference( int nPort );
      DEVICE_TYPE* Lookup( int nPort );
   private:
      class CDevCounter
         {
         private:
            CDevCounter( DEVICE_TYPE* pDevice ) 
               : m_pDevice( pDevice ), m_nCount( 0 ) { }
            DEVICE_TYPE*       m_pDevice;
            long        m_nCount;
            friend class TOposDeviceMap<DEVICE_TYPE>;
         };
      CTypedPtrMap < CMapWordToPtr, int, CDevCounter* > m_DeviceMap;
      static BOOL   m_bEnsureSingleton;
   };



// TOposDeviceMap<DEVICE_TYPE>::m_bEnsureSingleton
//
// Purpose: Global variable to ensure that only one device map exists for 
//          the specified type of device. 

template<class DEVICE_TYPE>
BOOL TOposDeviceMap<DEVICE_TYPE>::m_bEnsureSingleton = FALSE;

// TOposDeviceMap<DEVICE_TYPE>::TOposDeviceMap
//
// Purpose: Constructor

template<class DEVICE_TYPE>
inline TOposDeviceMap<DEVICE_TYPE>::TOposDeviceMap(
   void)
   {
   ASSERT( m_bEnsureSingleton == FALSE);
   m_bEnsureSingleton = TRUE;
   }

// TOposDeviceMap<DEVICE_TYPE>::~TOposDeviceMap
//
// Purpose: Destructor

template<class DEVICE_TYPE>
inline TOposDeviceMap<DEVICE_TYPE>::~TOposDeviceMap(
   void)
   {
   ASSERT( m_bEnsureSingleton != FALSE);
   m_bEnsureSingleton = FALSE;
   }


// TOposDeviceMap<DEVICE_TYPE>::AddReference
//
// Purpose: Increment the reference to a device. 

template<class DEVICE_TYPE>
inline void TOposDeviceMap<DEVICE_TYPE>::AddReference( 
   int nPort, 
   DEVICE_TYPE* pDevice)
   {
   ASSERT( pDevice != NULL );
   CDevCounter* pCounter = NULL;
   CSingleLock csAccess( this, TRUE );
   m_DeviceMap.Lookup( nPort, pCounter );
   if ( pCounter == NULL )
      {
      pCounter = new CDevCounter( pDevice );
      m_DeviceMap[ nPort ] = pCounter;
      }
   ASSERT( pCounter->m_pDevice == pDevice );
   pCounter->m_nCount++;
   return;
   }

// TOposDeviceMap<DEVICE_TYPE>::DeleteReference
//
// Purpose: Decrement the reference to a device. If reduced to zero, delete
//          the device.

template<class DEVICE_TYPE>
inline void TOposDeviceMap<DEVICE_TYPE>::DeleteReference( 
   int nPort )
   {
   CDevCounter* pCounter = NULL;
   CSingleLock csAccess( this, TRUE );
   m_DeviceMap.Lookup( nPort, pCounter );
   ASSERT( pCounter != NULL );   
   ASSERT( pCounter->m_nCount != 0 );
   if ( --pCounter->m_nCount == 0)
      {
      delete pCounter->m_pDevice;
      delete pCounter;
      m_DeviceMap[ nPort ] = NULL;
      }
   return;
   }

// TOposDeviceMap<DEVICE_TYPE>::Lookup
//
// Purpose: Find a device by port number. 

template<class DEVICE_TYPE>
inline DEVICE_TYPE* TOposDeviceMap<DEVICE_TYPE>::Lookup( 
   int nPort ) 
   {
   DEVICE_TYPE* pResult = NULL;
   CDevCounter* pCounter = NULL;
   CSingleLock csAccess( this, TRUE );
   m_DeviceMap.Lookup( nPort, pCounter );
   if ( pCounter != NULL )
      {
      pResult = pCounter->m_pDevice;
      ASSERT( pResult != NULL );
      pCounter->m_nCount++;
      }
   return pResult;
   }


#endif
