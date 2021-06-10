/////////////////////////////////////////////////////////////////////////////
//
// MotionSensorWrapper.cpp
//
//   Implementation of IDispatch wrapper class for CMotionSensorWrapper
//
//   Copyright (c) 2003 NCR.  All rights reserved.
//
//
//   Date                   Modification                          Author
// --------|---------------------------------------------------|-------------
// 24Oct03  Initial version.                                    S. McDaniel
//
/////////////////////////////////////////////////////////////////////////////

//TAR251360  Modified this class to be like the other wrapper classes, less
//tightly linked to the actual CO's methods, and using progid, in the .cpp, to
//create the control.  This should make this device less prone to churn through
//OCX or SO changes.

#include "..\stdafx.h"
#include "MotionSensorWrapper.h"

#define COMP_ID ID_DM      // used by the trace() calls to identify this component
#define T_ID    _T("DMX")

/////////////////////////////////////////////////////////////////////////////
// CMotionSensorWrapper

/////////////////////////////////////////////////////////////////////////////
// CMotionSensorWrapper


BOOL CMotionSensorWrapper::Create( 
   CWnd* pParentWnd, 
   UINT nID )
	{
   return TOPOSMotionSensor<CGenericOposIF>::Create(NULL, 0, CRect(0,0,0,0), pParentWnd, nID );
   }

/////////////////////////////////////////////////////////////////////////////
// CMotionSensorWrapper properties


/////////////////////////////////////////////////////////////////////////////
// CMotionSensorWrapper operations


/* virtual */
void CMotionSensorWrapper::OnOposStatusEvent( 
   long nStatus )
   {
      switch ( nStatus ) 
	 {
		case MOT_EXCEED_MAX:
        case MOT_STATUS_FAILURE:
		// want to simulate a motion / no_motion so the tag deactivate will go away
		 // send a failure message to the event viewer
		ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_DEVFACTORY_MOTION_SENSOR_FAILED,_T("%d"), nStatus);
      	case MOT_STATUS_OK:
             PostDmEventToApplication( DM_STATUS, DMCLASS_MOTION_SENSOR, m_nDeviceId, nStatus );
			 break;
		case MOT_BLOCKED:
			if ( 1 == m_nDeviceId )		//coupon sensor
			 {
			m_bIsSensorBlocked = true;
			ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_DEVFACTORY_MOTION_SENSOR_FAILED,_T("%d"), nStatus);
			// If the coupon sensor is blocked (full) and coupon processing is enabled, 
			// post error to application.
			if ( m_bPostCouponEventFlag )	
			{
				PostDmEventToApplication( DM_STATUS, DMCLASS_MOTION_SENSOR, m_nDeviceId, nStatus );
			}
			break;
		 }
		 else
		 {
			return;
		 }
		 case MOT_MOTION:
		 case MOT_NO_MOTION:
			if ( 1 == m_nDeviceId )		//coupon sensor
			{
				// NO_MOTION sent when coupon sensor is no longer blocked.
				m_bIsSensorBlocked = false;  			
				if ( !m_bPostCouponEventFlag )	
				{
				return;	// Coupon event processing is disabled, ignore event.
				}		
			 }
			 PostDmEventToApplication( DM_STATUS, DMCLASS_MOTION_SENSOR, m_nDeviceId, nStatus );
			 break;
    	 default:
         	ASSERT( FALSE );  // Power Notify someday???
       		break;
  	    }		
   return;   

   }


/**
 * Method:      EnableCouponProcessIng
 * Description:  
 * Parameters:  bFlag - If true, forward coupon sensor events to the base                    
 *                      application.  
 *                      If false, ignore coupon sensor events.
 * Returns:
 * Notes:       Added for SR631.1 - FL Receive coupon events only when coupons   
 *              expected
 */
void CMotionSensorWrapper::EnableCouponProcessing(bool bFlag)
{
	m_bPostCouponEventFlag = bFlag;	
	//post MOT_BLOCKED to the application if sensor is blocked while entering the InsertCoupon state
	if (m_bIsSensorBlocked)
	{
		PostDmEventToApplication( DM_STATUS, DMCLASS_MOTION_SENSOR, m_nDeviceId, MOT_BLOCKED );
	}
}



/**
 * Method:      CheckHealth
 * Description: Performed CheckHealth and check if the coupon sensor is blocked
 * Parameters:  lLevel – determine what level of CheckHealth to be performed
 * Returns:
 * Notes:       Added for SR631.1 - FL Receive coupon events only when coupons   
 *              expected
 */

long CMotionSensorWrapper::CheckHealth(long lLevel)
{
	long rc = TOPOSMotionSensor<CGenericOposIF>::CheckHealth(lLevel);
	if(rc != OPOS_SUCCESS)
	{
		return rc;		
	}	

	if(m_bIsSensorBlocked)
	{
		return MOT_BLOCKED;	//return 5 when coupon is still blocked after Checkhealth
	}
	return rc;
}
