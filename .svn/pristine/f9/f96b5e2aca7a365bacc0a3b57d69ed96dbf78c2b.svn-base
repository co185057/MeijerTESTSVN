
/********************************************************************/
/*  <scale.h> header file                                           */
/*                                                                  */
/********************************************************************/

#ifndef _H_POS_SCALE
#define _H_POS_SCALE 1

/****************************************************************/
/* Values for the SUBTYPE field of ONLINE and OFFLINE messages. */
/****************************************************************/

#define PosSCALE_SUBTYPE_4687           1
#define PosSCALE_SUBTYPE_4696           2
#define PosSCALE_SUBTYPE_4698           2

/*****************************/
/* PosScale resource names  */
/*****************************/

#define PosNdisplayRequired             _T("displayRequired")
#define PosNoperMode                    _T("operMode")
#define PosNvibrationFilter             _T("vibrationFilter")
#define PosNweightMode                  _T("weightMode")
#define PosNzeroIndState                _T("zeroIndState")
#define PosNzeroRetState                _T("zeroRetState")

/****************************/
/* Scale resource classes   */
/****************************/

#define PosCDisplayRequired             _T("DisplayRequired")
#define PosCOperMode                    _T("OperMode")
#define PosCVibrationFilter             _T("VibrationFilter")
#define PosCWeightMode                  _T("WeightMode")
#define PosCZeroIndState                _T("ZeroIndState")
#define PosCZeroRetState                _T("ZeroRetState")

/****************************************************************/
/* Constants used in the Flags field of the scale data header   */
/****************************************************************/

#define POS_SCALE_IN_MOTION             0x0001
#define POS_SCALE_UNDER_ZERO            0x0002
#define POS_SCALE_OVER_CAPACITY         0x0004
#define POS_SCALE_REQUIRES_ZEROING      0x0008
#define POS_SCALE_NOT_READY             0x0010
#define POS_SCALE_DUPLICATE_WEIGHT      0x0020
#define POS_SCALE_HW_ERROR              0x0040

/****************************************************************/
/* PosScaleData structure -- Scale data                         */
/****************************************************************/

typedef struct
{
  unsigned short WtType;              /* Weight Type  */
  unsigned short Flags;               /* Flags        */
  long          WeightValue;        /* Weight Value  */
} PosScaleData;

/**************************************************/
/* Constants used only by Scales                  */
/**************************************************/

#define PosENGLISH                      0x00
#define PosMETRIC                       0x01

#define PosUSCANADA                     0x00
#define PosUK                           0x01

#endif  /* _H_POS_SCALE */
