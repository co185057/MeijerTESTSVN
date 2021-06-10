
/********************************************************************/
/*  <msr.h> header file                                             */
/*                                                                  */
/********************************************************************/

#ifndef _H_POS_MSR
#define _H_POS_MSR 1

/* PosNtrack resource values */

#define PosTRACK_ALL              0xFFFFFFFF
#define PosTRACK_1                0x00000001
#define PosTRACK_2                0x00000002
#define PosTRACK_3                0x00000004
#define PosTRACK_J                0x00000008

/************************************************************/
/* Constants used in the DFlags field of the MSR data block */
/************************************************************/

#define POS_MSR_TRK_ERROR         0x0001

/****************************************************************/
/* PosMsrDataHdr structure -- Scanner label data header         */
/****************************************************************/

typedef struct
{
  unsigned long  Track;               /* Track Information */
  unsigned short Flags;               /* Flags            */
  unsigned short Length;              /* Data Length      */
} PosMsrDataHdr;

#endif  /* _H_POS_MSR */
