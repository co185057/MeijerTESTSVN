/****************************************************************
 *                                                              *
 *    Module Name     : comp.c                                  *
 *                      (Signature Compression Routine)         *
 *                                                              *
 *    Language        : Archimedes "C"                          *
 *    Operating Sys   : Byte-BOS                                *
 *    Target System   : NCR 5990 Signature Plus                 *
 *                                                              *
 *    Author          : Ralph Crabtree E&M Atlanta		*
 *			Derived from B. Taft(Splatt) 5980	*
 *								*
 *    (C) Copyright NCR Corporation, 1991 			*
 *                                                              *
 ****************************************************************

 ****************************************************************
 *                                                              *
 *           M O D I F I C A T I O N   H I S T O R Y      	*
 *                                                              *
 ****************************************************************
 *  DATE       REASON                         CALL#  AUTHOR     *
 ****************************************************************
 *                                                              *
 * 05/15/92    Initial Entry                  -----  R.Crabtree *
 *		 						*
 ****************************************************************/
/* $Revision: 1 $
   $Date: 6/23/08 4:55p $
   $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/SCOTAPP/COMP.C $
 * 
 * 1     6/23/08 4:55p Sa250050
 * FastLane 4.0 E5.3 Build 291 - Core Source
 * 
 * 1     1/26/05 2:08p Bd123405
 * Initial check-in of 4.0 Patch B
* 
* 7     5/13/03 11:56a Jj100011
* Tar 237031,  unicode change for signature capture, which causes Sign
* Sig screen is not shown on Lane when tender with credit on RAP.  Undo
* this Unicode change
* 
* 4     1/18/02 6:34p Fz160000
* HD Merge 20020118
* 
* 5     1/05/02 3:32p Burkt
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
 *    Rev 1.0   06 Oct 1993 09:34:28   APENG
 * Initial revision.

*/
/***************************************************************************/
 /*
 *    Rev 1.6   03 Mar 1993 14:39:48   APENG
 * When the user sign the signature "very fast", an overflow condition may
 * occure in the routine save_point. This is because when either delta_x
 * or delta_y is more than the MAX_OFFBIT (120), a divide to get the x_step
 * or y_step may generate overflow. Random points are added to the 
 * compressed data. This problem is fixed by multiplying 1/4 of MAX_OFFBIT
 * first and then do the divide and finally multiply 4. Spec-Alias#:
 * 5000009243. (Problem fixed by Ralph Crabtree).
 * 
 *    Rev 1.5   23 Sep 1992 14:08:08   RCRABTREE
 * SPEC call 7771. Pmd.c, comp.c, and cmp_proc.c were modified to check
 * buffer overflow conditions so that the pen is disabled when there is
 * no more space for pen data.
 * 
 *    Rev 1.4   30 Jul 1992 09:15:40   RCRABTREE
 * Fixed OS corrupt due to overwriting end of buffer. SPEC 5000006921.
 * 
 *    Rev 1.3   30 Jul 1992 09:10:20   RCRABTREE
 * No change.
 * 
 *    Rev 1.2   21 Jul 1992 19:58:10   RCRABTREE
 * Made modifications to process circular buffer boundary conditions
 * correctly. SPEC 5000006868.
 * 
 *    Rev 1.1   21 Jul 1992 17:17:54   RCRABTREE
 * Compression returned a data point when compressing a stroke that consisted
 * of zero points.  SPEC 5000006867.
 * 
 *    Rev 1.0   21 Jul 1992 16:43:34   RCRABTREE
 * Initial revision.
 */


/****************************************************************
 *                                                              *
 *               I N C L U D E   F I L E S		 	*
 *                                                              *
 ****************************************************************/

#include  "comp.h"   /* include compression header file */

/****************************************************************
 *                                                              *
 *		MACROS AND DEFINES				*
 *                                                              *
 ****************************************************************/



#define  MAX_OFFSET	120	/* maximum delta (signed) represented by 8 bits */
				/*  remember +/- an nError of four due to 5-bit	*/
#define  THRESHOLD_VAL	4	/* linear fit threshold value 			*/

/*
 *  Global variables used throughout the module
 */
extern  char  compress_alg;             /*  specifies compression alg.  */

ushort   point_count;			/*  number of points compressed	*/
short    y_base;			/*  last y coordinate stored	*/
short    x_base;			/*  last x coordinate stored	*/
short    dev_val;			/*  threshold value		*/
short    start_y;			/*  start point y coordinate	*/
short    start_x;			/*  start point x coordinate	*/
short    mid_y;				/*  mid point y coordinate	*/
short    mid_x;				/*  mid point x coordinate	*/
short    stop_y;			/*  stop point y coordinate	*/
short    stop_x;			/*  stop point x coordinate	*/
short    delta_y;			/*  change in y values		*/
short    delta_x;			/*  change in x values		*/
short    abs_delta_y;			/*  absoulte value of delta_y	*/
short    abs_delta_x;			/*  absolute value of delta_x	*/
short    max_offset;       		/*  maximum delta represented   */
short    max_delta;	         	/*  accuracy of the delta 5-bit */
					/*  to 8-bit accuracy		*/

/*
 *   Function Declarations
 */
static  ushort 	test_point();
static  char  	*save_point( char *, char *,  int, int, int, int);
static  char	*dump_point(int, int, char *);
static  int  	abs(int);
static  int  calc_step( int, int);


/*************************************************************************
 *
 * NAME:                abs
 *
 * INPUT PARAMETERS:    i       - integer to return absolute value of
 *
 * RETURN VALUES:       returns absolute value of i
 *
 * GLOBAL VARIABLES:    none
 *
 * FUNCTION:
 *      Abs returns the absolute value of the integer passed in.
 *
 ***********************************************************************/
#ifdef _DEBUG
static  int  abs(int i)
{
	if ( i > 0 )
		return(i);
	else
		return(-i);
}
#endif

/*************************************************************************
 *
 * NAME:                compress
 *
 * INPUT PARAMETERS:    circ_start - pointer to start of circular buffer that
 *                              contains the stroke to compress.
 *                      circ_end   - pointer to end of circular buffer that 
 *                              contains the stroke to compress.
 *                      sig_ptr    - pointer to the stroke to compress.
 *                      dest_buff  - pointer to buffer to store the compressed
 *                              data.
 *                      end_dest_buff - pointer to the end of the storage
 *                              buffer.
 *                      count_ptr     - pointer to integer containing # of
 *                                      points processed.
 *
 * RETURN VALUES:       returns the size of the compressed stroke.
 *
 * GLOBAL VARIABLES:    none
 *
 * FUNCTION:
 *      Compress takes as input the data from a single pen stroke.  The stroke
 *      data is then compressed and stored in the compression buffer.  For a
 *      complete description of the compression algorithm, see the following
 *      documentation.  Patents submitted by Crooks/Protheroe/Wagnor/Taft.
 *
 ***********************************************************************/
int compress(char *circ_start,
	     char *circ_end,
	     CO_ORD_BUF  *sig_ptr,
	     COMP_STROKE_BUF  *dest_buff,
	     char  *end_dest_buff,
	     unsigned int *count_ptr )

   {

	int num_entries;
	char *current_ptr;
	CO_ORD_DATA  *start_ptr, *mid_ptr, *stop_ptr, *last_stop;

	/*  Bad code here, but efficient.  Compress_alg has values 0 - 3,
	    for compress_alg = 0, we need max_offset = 120 and dev_val = 4.
	                       1                        60               2
	                       2                        30               1
	                       3                        15               0
            Algorithm 3 has the best signature, since no data is discarded,
	    but results in big signature records.  Algorithm 0 is the release
	    1.0 default algorithm.

	    Values of 120, 60, 30, and 15 are chosen as follows.

	    In algorithm 0 there is an 8 bit delta, -128 < delta < 128, and
 	    the delta is packed into 5 bits.  Since the i3 least significant
	    bits are discarded, the max delta is only +/- 120.  See Sam Wagner's
	    patent for a more lengthy, and boring, description.  

	    In algorithm 1 there is a 7 bit delta, -64 < delta < 64, packed
	    in 5 bits. Again, the 3 lsbs are discard, resulting in a max
	    delta of 60.  Algorithm 2 and 3 are the same.
	*/

	dev_val = 4 >> compress_alg;   /*  set comp. algorithm variables.  */
	max_offset = 120 >> compress_alg;
	max_delta = compress_alg;

					/*  set initial coordinate pointers */
	start_ptr = &sig_ptr->co_ord;	/*  make sure we are not at end  */
	if ((char *)(start_ptr + 1) > circ_end)
		start_ptr = (CO_ORD_DATA *)circ_start;
	mid_ptr = start_ptr;
	stop_ptr = start_ptr;
					/*  and load number of co-ords	*/
	num_entries = sig_ptr->vli / sizeof(CO_ORD_DATA);
	if ( num_entries == 0 )		/* if no points, just exit	*/
		return(0);

	/*  get address of first delta	*/
	current_ptr = (char *)&dest_buff->data;	
	if ( current_ptr > end_dest_buff )
		return(0);		/* out of room			*/

	/*  Build the start of stroke header.  <vli, y absolute, x absolute>	*/
	dest_buff->stroke_header.sh_flags = STROK_HDR_FLG;
	dest_buff->stroke_header.sh_vli = 0;
	y_base = start_ptr->Y_VAL;
	x_base = start_ptr->X_VAL;
	dest_buff->start_pos_abs.y_abs_high = (char)(y_base >> 8);
	dest_buff->start_pos_abs.y_abs_low = (char)y_base;
	dest_buff->start_pos_abs.x_abs_high = (char)(x_base >> 8);
	dest_buff->start_pos_abs.x_abs_low = (char)x_base;
   
	point_count = 1;		/*  set number of points stored	*/
	(*count_ptr)++;			/*  bump point count  */

	if ( --num_entries <= 0 ) {	/*  if only one point, then exit	*/
		dest_buff->stroke_header.sh_vli = 1;
		return(current_ptr - (char *)dest_buff);
		}

	/*
	 *  This is what it's all about...
	 *  all together now. Everyone compress. Credits to Splatt.
	 */

	do {
		/*  Increment stop to the next point.  The first point to test
		    is the point after start.  */

		last_stop = stop_ptr;		/*  save for backing up  */
		/*  Bump the pointers. Remember, it is circular.  */
		stop_ptr++;
		if ( (char *)(stop_ptr + 1) > circ_end )
			stop_ptr = (CO_ORD_DATA *)circ_start;
		mid_ptr = start_ptr + 1;
		if ( (char *)(mid_ptr + 1) > circ_end )
			mid_ptr = (CO_ORD_DATA *)circ_start;

		/*  Determine if all points between start and stop can be
		    thrown away.  If so, increment stop to the next point
		    and perform the same test.  When a point fails, store the
		    previous stop point (the last point that passed the test. */

		while(mid_ptr != stop_ptr) {
			/*  extract the co-ord values	*/
			start_y = start_ptr->Y_VAL;
			start_x = start_ptr->X_VAL;
			mid_y = mid_ptr->Y_VAL;
			mid_x = mid_ptr->X_VAL;
			stop_y = stop_ptr->Y_VAL;
			stop_x = stop_ptr->X_VAL;
			/*  Check the point for a keeper	*/
			if ( test_point() == KEEP) {
				/*  yes, so keep the last point which passed	*/
				mid_ptr = last_stop;
				current_ptr = save_point(current_ptr, end_dest_buff,
						start_ptr->Y_VAL, start_ptr->X_VAL,
						mid_ptr->Y_VAL, mid_ptr->X_VAL);
				start_ptr = mid_ptr;	/*  restart from the last saved	*/
				break;
				}
			else {				/*  try the next point	*/
				mid_ptr++;
				if ( (char *)(mid_ptr + 1) > circ_end )
					mid_ptr = (CO_ORD_DATA *)circ_start;
				}
			}
		num_entries--;				/*  one less point to test	*/
		(*count_ptr)++;			        /*  bump point count  */
		} while( num_entries );

	/*  save the last point in the buffer	*/
	current_ptr = save_point(current_ptr, end_dest_buff,
			start_ptr->Y_VAL, start_ptr->X_VAL,
			stop_ptr->Y_VAL, stop_ptr->X_VAL);
	/*  fill in the stroke vli	*/
	dest_buff->stroke_header.sh_flags += (uchar)(point_count >> 8);
	dest_buff->stroke_header.sh_vli = (uchar)(point_count & 0xff);
	return(current_ptr - (char *)dest_buff);	/*  return # of bytes	*/
}


/*************************************************************************
 *
 * NAME:                test_point
 *
 * INPUT PARAMETERS:    none
 *
 * RETURN VALUES:       returns KEEP if the point should be kept, and LOSE if
 *                      the point can be discarded.
 *
 * GLOBAL VARIABLES:    start_x - start point x coordinate
 *                      start_y - start point y coordinate
 *                      mid_x   - test point x coordinate
 *                      mid_y   - test point y coordinate
 *                      stop_x  - stop point x coordinate
 *                      stop_y  - stop point y coordinate
 *
 * FUNCTION:
 *      This function contructs an imaginary box between <start_x,stop_x>
 *      and <stop_x,stop_y>.  If the point <mid_x,mid_y> lies the box, this
 *      point can be discarded and the function returns LOSE.   Otherwise,
 *      the point must be kept and KEEP is returned.
 *
 ***********************************************************************/
     
static  ushort  test_point()
{
	ushort   result;
	short    scaled_y_low;
	short    scaled_y_high;
	short    scaled_y_check;

	/********************************************************* 
	 *                                                       *
	 *  Use the "linear fit" method for testing the need to  *
	 *  retain the point under test. This method utilizes    *
	 *  a linear equation to determine the slope of a line   *
	 *  between the start and end points. The point being    *
	 *  tested must fall on this line to be discarded.       *
	 *                                                       *
	 *  Advantage: Simpler and faster than "perpendicular    *
	 *  method".                                             *
	 *                                                       *
	 *  Disadvantage: Does not allow for an adjustable       *
	 *  "sensitivity".                                       *
	 *                                                       *
	 ********************************************************/

	/*   Let the testing begin....      */

	delta_y = start_y - stop_y;
	delta_x = start_x - stop_x;
	abs_delta_y = abs(delta_y);
	abs_delta_x = abs(delta_x);

	/*  if deltas are to large, save the point	*/
	if ((abs_delta_y > max_offset) || (abs_delta_x > max_offset))

		return(KEEP);

	if ((start_x == stop_x) && (abs(start_x - mid_x) <= dev_val)) {
		/*
		*  We have your basic vertical line here, if midpoint
		*  is on the same line, between start and stop, lose it dude! 
		*/
		if ( ((start_y <= mid_y) && (mid_y <= stop_y)) ||  \
		     ((start_y >= mid_y) && (mid_y >= stop_y)) )
			 result = LOSE;
		else
			result = KEEP;
		}

	else if ( (start_y == stop_y) && (abs(start_y - mid_y) <= dev_val) ) {
		/*
		*  We have your basic horizontal line here, if midpoint
		*  is on the same line, between start and stop, lose it dude! 
		*/
		if ( ((start_x <= mid_x) && (mid_x <= stop_x)) ||  \
		     ((start_x >= mid_x) && (mid_x >= stop_x)) )
			result = LOSE;
		else
			result = KEEP;
		}

	else {
		/*   adjust points inorder to have a postitive delta	*/
		if (start_y > stop_y) { 
			scaled_y_check = start_y;
			start_y = stop_y;
			stop_y = scaled_y_check;

			scaled_y_check = start_x;
			start_x = stop_x;
			stop_x = scaled_y_check;
			}
		/* Transpose start point to logical 0,0 */
		stop_x -= start_x;
		mid_x -= start_x;
		start_x = 0;

		stop_y -= start_y;
		mid_y -= start_y;
		start_y = 0;

		result = KEEP;		/* initialize for a keeper	*/

		/*  run the parallel line test	*/
		if (abs_delta_x >= abs_delta_y) {
			/* line more horizontal, change in Y is more important */
			if ( abs((delta_x * mid_y) - (delta_y * mid_x)) <= \
			     abs(delta_x * dev_val) )
				result = LOSE;
			}
		else {
			/* line more vertical, change in X is more important */
			if ( abs((delta_y * mid_x) - (delta_x * mid_y)) <= \
			     abs(delta_y * dev_val) )
				result = LOSE;
			}

		/*  run the perpendicular test	*/
		if (result == LOSE) {
			result = KEEP;
			scaled_y_low = (-delta_x) * mid_x;
			scaled_y_high = (delta_x * (stop_x -mid_x)) + (delta_y * stop_y);
			scaled_y_check = delta_y * mid_y;
			if (delta_y < 0) { 	/* negate all scaled y values */
				scaled_y_low = (-scaled_y_low);
				scaled_y_high = (-scaled_y_high);
				scaled_y_check = (-scaled_y_check);
				}
			if ((scaled_y_check >= scaled_y_low) && \
			    (scaled_y_check <= scaled_y_high))
				result = LOSE;
			}

		}
	return(result);
}

/*************************************************************************
 *
 * NAME:                calc_step
 *
 * INPUT PARAMETERS:    delta -      smallest delta
 *                      abs_delta -  largest delta.
 *
 * RETURN VALUES:       step value
 *
 * GLOBAL VARIABLES:    max_offset
 *
 * FUNCTION:
 *      calculates the step value used for calculating deltas.
 *      a routine is used to guard against overflow.
 *      calculating delta / num_to_insert.
 *
 ***********************************************************************/

static  int  calc_step( int delta, int abs_delta)
{
	int shift, step;

	if ( max_offset == 120 )	/*  if a big offset, divide along */
		shift = 2;		/*  way. avoids overflow. see spec */
	else if ( max_offset == 60 )	/*  5000009243.                  */
		shift = 1;
	else
		shift = 0;
	step = (delta * (max_offset >> shift)) / abs_delta;
	return (step << shift);
}


/*************************************************************************
 *
 * NAME:                save_point
 *
 * INPUT PARAMETERS:    dest_ptr - addresss to store the data.
 *                      end_dest - pointer to end of buffer.
 *                      start_y  - start point y coordinate
 *                      start_x  - start point x coordinate
 *                      mid_y    - stop point y coordinate
 *                      mid_x    - stop point x coordinate
 *
 * RETURN VALUES:       address to store the next point.
 *
 * GLOBAL VARIABLES:    x_base   - x coordinate of last point stored.
 *		     	y_base   - y coordinate of last point stored.
 *
 * FUNCTION:
 *      Save_point stores the points required to construct a line between
 *      <start_x,start_y> and <mid_x,mid_y>.  Points are stored as five bit
 *      deltas from the last point stored.  Due to nError, the maximum delta 
 *      for any two points is max_offset.  As a result, more than one point
 *      may be stored.
 *
 ***********************************************************************/

static  char  *save_point(char  *dest_ptr, char *end_dest,
		  int start_y, int start_x,
		  int mid_y, int mid_x)
{
	int  y_step, x_step, num_to_insert;
	short    y_diff;
	short    x_diff;
	short	 shift_mask;
	short      nError;

	nError = 0x04 >> max_delta;
	shift_mask = (short)0xfff8 >> max_delta;
	delta_x = mid_x - start_x;
	delta_y = mid_y - start_y;
	abs_delta_x = abs(delta_x);
	abs_delta_y = abs(delta_y);
	num_to_insert = 0;	 /* assume none to insert */

	if ((abs_delta_y > max_offset) && (abs_delta_y > abs_delta_x))
	{ /* gotta add points, y is largest (driving) delta */
		num_to_insert = abs_delta_y / max_offset;
/*		x_step = (delta_x * (max_offset >> 2)) / abs_delta_y;
		x_step <<= 2;
*/
		x_step = calc_step(delta_x, abs_delta_y);
/*		x_step = (delta_x * max_offset) / abs_delta_y;   */
/*		x_step = delta_x / num_to_insert;  */
	}

	else if (abs_delta_x > max_offset)
	{ /* gotta add points, x is largest (driving) delta */
		num_to_insert = abs_delta_x / max_offset;
/*		y_step = (delta_y * (max_offset >> 2)) / abs_delta_x;
		y_step <<= 2;
*/
		y_step = calc_step(delta_y, abs_delta_x);
/*		y_step = (delta_y * max_offset) / abs_delta_x;  */
/*		y_step = delta_y / num_to_insert;  */
	}
	while (num_to_insert)
	{ /* add points til satisfied */
		if (--num_to_insert)
		{ /* not the last one to insert, use maximum delta */
			if (abs_delta_y > abs_delta_x)
			{ /* y is driving factor */
				if (delta_y > 0)
					start_y += max_offset;
				else
					start_y -= max_offset;
				start_x += x_step;
			}
			else
			{ /* x is driving factor */
				if (delta_x > 0)
					start_x += max_offset;
				else
					start_x -= max_offset;
				start_y += y_step;
			}
		}
		else 
		{ /* last point to insert, locate center */
			start_y += (mid_y - start_y) / 2;
			start_x += (mid_x - start_x) / 2;
		}
		/*  determine delta and set base to current point  */
		y_diff = start_y - y_base;
		x_diff = start_x - x_base;
		y_base += (short)((y_diff & shift_mask) | nError);
		x_base += (short)((x_diff & shift_mask) | nError);
		if ( (dest_ptr+2) < end_dest ) {	/*  check overflow */
			point_count++;
			dest_ptr = dump_point(y_diff, x_diff, dest_ptr);
			}
	}
	/*  determine delta and set base to current point  */
	y_diff = mid_y - y_base;
	x_diff = mid_x - x_base;
	y_base += (short)((y_diff & shift_mask) | nError);
	x_base += (short)((x_diff & shift_mask) | nError);
	if ( (dest_ptr+2) < end_dest ) {	/*  check overflow */
		point_count++;
		dest_ptr = dump_point(y_diff, x_diff, dest_ptr);
		}
	return(dest_ptr);
}


/*************************************************************************
 *
 * NAME:                dump_point
 *
 * INPUT PARAMETERS:    dest_ptr - addresss to store the data.
 *                      y  - y delta value to store.
 *                      x  - x delta value to store.
 *
 * RETURN VALUES:       address to store the next point.
 *
 * GLOBAL VARIABLES:    none
 *
 * FUNCTION:
 *      Dump_point saves the x and y values in the data buffer.  Values
 *      are stored as five bit deltas.
 *
 ***********************************************************************/

static  char  *dump_point(int y, int x, char *dest_ptr)
{

	x <<= max_delta;	/*   adjust, since code drops 3 bits  */
	y <<= max_delta;
	switch (point_count & 3)
	{
		case 2:   /* msb of Y aligns with destination bit 7 (high bit) */
			*dest_ptr = (y & 0xF8) | ((x >> 5) & 7);
			dest_ptr++;
			*dest_ptr = ((x << 3) &0xC0);                  
			break;

		case 3:   /* msb of Y aligns with destination bit 5 */
			dest_ptr--;
			*dest_ptr += (((y >> 2) & 0x3E) | ((x >> 7) & 1)); 
			dest_ptr++;
			*dest_ptr = ((x << 1) & 0xF0);
			break;

		case 0:   /* msb of Y aligns with destination bit 3 */
			dest_ptr--;
			*dest_ptr += ((y >> 4) & 0x0F); 
			dest_ptr++;
			*dest_ptr = (((y << 4) & 0x80) | ((x >> 1) & 0x7C));
			break;

		default:   /* msb of Y aligns with destination bit 1 */
			dest_ptr--;
			*dest_ptr += ((y >> 6) & 0x03); 
			dest_ptr++;
			*dest_ptr = (((y << 2) & 0xE0) | ((x >> 3) & 0x1F));
			break;
	}  /* end of switch */
	dest_ptr++;
	return(dest_ptr);
}
