
#ifndef _NCRMSR_H_
#define _NCRMSR_H_

#include "NcrExt.h"

enum DIOCommandCode 
{
	NCR_IOM_BIN_STATUS= 1, 
	NCR_IOM_EJECT,	
	NCR_IOM_CAPTURE,
	NCR_IOM_ENABLENONMAGNETIC,
	// NDF-CT Added on 26/4/99 for non-magnetic support IDL->267
	NCR_IOM_FIRMWARE_VERSION, //tn. added for MTS
    NCR_IOM_POSITION
};

enum NCRExtendedCode
{
	NCR_BIN_IN= OPOSERREXT + 10,	// 210
	NCR_BIN_REMOVED,				// 211
	NCR_BIN_NOT_SUPPORTED,			// 212
	NCR_NO_CARD,					// 213
	NCR_CARD_JAM_CAP,				// 214
	NCR_NO_CARD_REMOVED,			// 215
	NCR_READ_ERROR,					// 216
	NCR_TRACKS_UNSUPPORTED,			// 217
	NCR_DEVICE_INOP,				// 218
	NCR_CARD_JAM,					// 219
	NCR_INV_CARD,					// 220
	NCR_IOE_BIN_INSERTED,			// 221
	NCR_IOE_BIN_REMOVED,			// 222
	NCR_IOE_CARD_REMOVED,			// 223
	NCR_IOE_CARD_DETECTED_ENTERING,	// 224
	NCR_IOE_CARD_INSERTED_STAGED,	// 225
	NCR_IOE_DEVICE_INOP,			// 226
	NCR_IOE_INVALID_CARD_INSERTED,	// 227
	NCR_IOE_SHUTTER_JAMMED_CLOSE,	// 228
	NCR_IOE_BIN_FULL,				// 229
	NCR_BLANK_TRACKS,				// 230
	NCR_CARD_PRESENT,				// 231
	NCR_IOE_CARD_DETECTED,			// 232
	NCR_IOE_DATA_AVAILABLE,			// 233
	NCR_IOE_CARD_CAPTURED,			// 234
	NCR_SHUTTER_FAILURE,			// 235

	// Smart Card Device Specific...
	NCR_SCIF_NOT_SUPPORTED,			// 236	
	NCR_SCIF_NO_CARD_IN_SCRW,		// 237
	NCR_SCIF_NO_SMARTCARD,			// 238
	NCR_SCIF_SMARTCARD,				// 239
	NCR_SCIF_CARD_MOVE_FAIL,		// 240
	NCR_SCIF_ILLEGAL_CARD_TYPE,		// 241

	NCR_MSR_PLATFORM_NOT_STARTED,
	
	// NDF-CT 17/4/98 Added three more events 
	// as requested by Rafael 
	NCR_IOE_POSSIBLE_CUST_TAMPERING, //243
	NCR_IOE_CARD_NOT_CAPTURED,		 //244
	NCR_IOE_EJECT_CAPTURED,			 //245

    // tn. Motorized Card Reader Send Command error

	MCR_INITIALIZE_CMD_ERROR,        //246     				
	MCR_ENABLE_CMD_ERROR,		     //247		 				
	MCR_DISABLE_CMD_ERROR,			 //248	 				
	MCR_MAGTRACKREAD_CMD_ERROR,		 //249   		
	MCR_STATUS_CMD_ERROR,			 //250	      
	MCR_EJECT_CMD_ERROR,			 //251	     
	MCR_CAPTURE_CMD_ERROR,			 //252	  
	MCR_REVISION_CMD_ERROR,			 //253	  
	MCR_RETRIEVE_CMD_ERROR,          //254	
	
	// tn. Motorized Card Reader CRCC error
	MCR_GET_CRCC_ERROR,              //255

	// tn. Motorized Card Reader get text length error
	MCR_GET_TEXTLEN_ERROR,           //256

	// tn. Motorized Card Reader get response timeout
	MCR_GET_RESPONSE_TIMEOUT,         //257

	//tn. Motorized Card Reader online status
	MCR_GET_BACK_ONLINE,              //258
	MCR_CARD_POSITION_NO_CARD_REMOVE,
	MCR_CARD_POSITION_AT_GATE,
	MCR_CARD_POSITION_AT_MCRW

};



#endif // _NCRMSR_H_
