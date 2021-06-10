/****************************************************************
 *                                                              *
 *    Module Name     : comp.h                                  *
 *                      (Signature Compression Routine)         *
 *                                                              *
 *    Language        : Archimedes "C"                          *
 *    Operating Sys   : Byte-BOS                                *
 *    Target System   : NCR 5990 Signature Plus                 *
 *                                                              *
 *    Author          : B.Taft,(a.k.a. SPLATT) E&M Cambridge    *
 *								*
 *    (C) Copyright NCR Corporation, 1991 			*
 *                                                              *
 ****************************************************************

 ****************************************************************
 *                                                              *
 *           M O D I F I C A T I O N   H I S T O R Y	        *
 *                                                              *
 ****************************************************************
 *  DATE       REASON                        CALL#  AUTHOR      *
 ****************************************************************
 *                                                              *
 * 07/19/92    Initial Entry                 -----  R. Crabtree *
 *                                                              *
 ****************************************************************

 $Revision: 1 $
 $Date: 6/23/08 4:55p $
 $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/SCOTAPP/COMP.H $
 * 
 * 1     6/23/08 4:55p Sa250050
 * FastLane 4.0 E5.3 Build 291 - Core Source
 * 
 * 1     1/26/05 2:08p Bd123405
 * Initial check-in of 4.0 Patch B
* 
* 5     5/13/03 11:56a Jj100011
* Tar 237031,  unicode change for signature capture, which causes Sign
* Sig screen is not shown on Lane when tender with credit on RAP.  Undo
* this Unicode change
* 
* 1     1/27/00 6:03a Barnhs
* Needed to do 5991 format Sig Cap Compression
* 
* 8     1/18/00 10:27a Buildpc
* upgraded to VC 6.0
* 
* 7     1/14/00 11:44a Ij200001
* changed from version 1.1 to 2.0
* 
* 5     12/08/99 4:32p Sm200059
* Updated the DeviceManager code to add a property in the DeviceManager
* to set the PIN block format to either ANSI or Diebold.
* 
* Dec 08 1999
* -Sameer.
* 
* 4     4/27/99 11:16a Antona
* Changed printer.h(new macros and CSting definition for PrintNormal()),
* printer.cpp( function PrintNormal()),
*  devmgrctrlprinter.cpp(function PrinterPrintString). 
* 
* 
* 3     4/06/99 2:53p Scot
* 
* 2     4/05/99 10:37a Scot
* 
* 1     3/11/99 10:06a Burkt
* Original Cambridge 5991 Compression Algorithm
 * 
 *    Rev 1.0   21 Jul 1992 16:45:28   RCRABTREE
 * Initial revision.
 */


/****************************************************************
 *                                                              *
 *                      D E F I N E S				*
 *                                                              *
 ***************************************************************/

#define  LOSE           0           /* results of the co-ord */
#define  KEEP           1           /*   validity test */
#define  STROK_HDR_FLG  0x80        /* stroke header bit for 32-bit abs form */

#define  SIG_HEAD_BYTE1_VAL   0x01  /* byte one of canned sig header */
#define  SIG_HEAD_BYTE2_VAL   0x11  /* byte two of canned sig header */
#define  SIG_HEAD_BYTE3_VAL   0x11  /* byte three of canned sig header */
#define  SIG_HEAD_FT_VAL      0x00  /* form type of canned sig header */

#define  DP_COMP_FATAL	    0x20	/*  fatal dead_pool error	*/

#define	 TRUE	1
#define	 FALSE	0
#define	 NULL	0
#define  MAX_SESSION  2

/****************************************************************
 *                                                              *
 *          S T R U C T U R E S   A N D   U N I O N S    		 *
 *                                                              *
 ***************************************************************/

typedef  unsigned  char  uchar;
typedef  unsigned  long  ulong;
typedef  unsigned  short ushort;

/*	structure for the session table.  The id is the identifier for table
	lookup ( NULL is free ).  */

struct	session {
    char  *id;		/*  entry id for lookup  */
    char  *current_ptr;	/*  current pointer in compression buffer  */
    } ;

/*	structure of a point as defined by the personality module	*/

struct   co_ord_data {
	short   Y_VAL;       /* 'y' co-ordinate value */
	short   X_VAL;       /* 'x' co-ordinate value */
	};

typedef  struct   co_ord_data    CO_ORD_DATA;

/*	structure of a stroke header record for a compressed signature	*/

struct   stroke_header {
	uchar    sh_flags;   /* stroke record flags */
	uchar    sh_vli;     /* stroke record length */
	};

typedef  struct   stroke_header   STROKE_HEADER;

/*	structure for absolute point coordinates, rather than deltas, that 
	follow the stroke header.	*/

struct   start_pos_abs {
	char     y_abs_low;      /* 'y' co-ordinate value */
	char     y_abs_high; 
	char     x_abs_low;      /* 'x' co-ordinate value */
	char     x_abs_high; 
	};

typedef  struct   start_pos_abs   START_POS_ABS;

/*	structure of the signature header record.	*/

struct   sig_header {
	uchar    sigh_b1;    /* sig header byte 1, always 0x01 */
	uchar    sigh_b2;    /* sig header byte 2, always 0x11 */
	uchar    sigh_b3;    /* sig header byte 3, always 0x11 */
	uchar    sigh_ft;    /* sig header form type, currently 0x00 */
	};

typedef  struct   sig_header   SIG_HEADER;
#define  CMP_HDR_SIZE  4	/*  size of signature header	*/

/*	structure of a compressed stroke in memory	*/

struct	comp_stroke_buf {
	STROKE_HEADER	stroke_header;	/*  stroke header record  */
	START_POS_ABS	start_pos_abs;	/*  absolute point coordinates  */
	unsigned  char	data;		/*  delta points, variable #	*/
    } ;

typedef  struct   comp_stroke_buf   COMP_STROKE_BUF;

/*	structure for the signature/script buffer for a stroke of uncompressed
	data.	*/

struct	co_ord_buf {
	short vli;		/*  the number of byte of data, noninclusive */
	CO_ORD_DATA  co_ord;	/*  point coordinates for the stroke  */
	} ;

typedef  struct   co_ord_buf   CO_ORD_BUF;

/*	structure for a signature that has been compressed	*/

struct  comp_buf_hdr {
	unsigned  short  vli;		/*  vli of the record, noninclusive */
	SIG_HEADER  sigh;		/*  the signature header	*/
	COMP_STROKE_BUF  stroke;	/*  and stroke data, repeatable  */	
	} ;

typedef  struct   comp_buf_hdr   COMP_BUF_HDR;
