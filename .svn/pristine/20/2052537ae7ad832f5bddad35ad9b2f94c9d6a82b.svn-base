// TBOposIO.h : 
//


#ifndef _TBOPOSIO_H_
#define _TBOPOSIO_H_

#include "tbmsgobj.h"

#pragma pack(1)

typedef struct io_opos_hdr
{
	BYTE	deviceID;
	BYTE	port;
	BYTE	unit;
	BYTE	reserved;
} OPOSIO_HDR;


#define SCANNER_DATA_OFFSET		12
typedef struct io_opos_scanner
{
	OPOSIO_HDR	hdr;
	DWORD		barcodeType;
	DWORD		dataLen;
	BYTE		Data[4];
} OPOSIO_SCANNER;


// Structures for support of LineDisplay
#define LD_DESCRIPTOR_CHANGE	0
#define LD_TEXT_CHANGE			1

// Unit values for LineDisplay
#define DISP_AUD_CUSTOMER		1
#define DISP_AUD_OPERATOR		0

typedef struct io_opos_linedisplay_hdr
{
	OPOSIO_HDR	hdr;
	BYTE		msgType;
	BYTE		reserved[3];
} OPOSIO_LINEDISPLAY_HDR;

#define LD_WINDOW_DATA_OFFSET   24
typedef struct io_opos_linedisplay_window
{
	OPOSIO_LINEDISPLAY_HDR ldhdr;
	long	window;
	long	rows;
	long	columns;
	DWORD	dataLen;
	BYTE	Data[4];
} OPOSIO_LD_WINDOW;

#define LD_DESCRIPTOR_DATA_OFFSET 12
typedef struct io_opos_linedisplay_descriptor
{
	OPOSIO_LINEDISPLAY_HDR ldhdr;
	DWORD dataLen;
	BYTE  Data[4];
} OPOSIO_LD_DESCRIPTOR;


// Structures for support of CashDrawer

#define CASHDRAWER_OPEN		1
#define CASHDRAWER_CLOSED	0
#define CASHDRAWER_LENGTH	8
typedef struct io_opos_cashdrawer
{
	OPOSIO_HDR	hdr;
	BYTE		drawerStatus;
	BYTE		reserved[3];
} OPOSIO_CASHDRAWER;

// Structures for support of POSPrinter

#define PTRFUNC_CUTPAPER	1
#define PTRFUNC_PRINT		2
#define PTRFUNC_BARCODE		3
#define PTRFUNC_BITMAP		4
#define PTRFUNC_SETBITMAP	5
#define PTRFUNC_SETLOGO		6

#define PTRFUNC_SLIPSTATUS 7
#define SLIPSTATUS_BEGININSERTION 0
#define SLIPSTATUS_ENDINSERTION   1
#define SLIPSTATUS_BEGINREMOVAL   2
#define SLIPSTATUS_ENDREMOVAL     3

#define PTRFUNC_PROPCHANGE  8

#define MAX_PTR_MESSAGE		512
#define PTR_DATA_OFFSET		32
typedef struct io_opos_printer
{
	OPOSIO_HDR	hdr;
	long		func;
	long		funcArg;		// See comments below
	long		height;
	long		width;
	long		alignment;
	long		textPosition;
	long		dataLen;
	BYTE		Data[4];
} OPOSIO_PRINTER;

// The use of funcArg depends on the function
//    PTRFUNC_CUTPAPER:  Percentage for cut
//    PTRFUNC_PRINT:  Not Used
//    PTRFUNC_BARCODE:  Symbology
//    PTRFUNC_BITMAP:  Not Used
//    PTRFUNC_SETBITMAP:  Bitmap number (1 or 2)
//    PTRFUNC_SETLOGO:  Location for logo


// Structures for support of POSKeyboard
// NOTE: flags are the same as defined for keybd_event in the MSDN lib
//
#define KEYSTROKE_LEN 16

typedef struct io_opos_keyboard
{
	OPOSIO_HDR	hdr;
	long		virtualKeyCode;
	long		scanKeyCode;
	long		flags;
} OPOSIO_KEYSTROKE;

// Structures for support of MSR
// NOTE: The Data will consist of the MSR Tracks concatenated together
//
#define MSR_TRACKDATA_OFFSET 16

typedef struct io_opos_msrtracks
{
	OPOSIO_HDR	hdr;
	long		track1Len;
	long		track2Len;
	long		track3Len;
	BYTE		Data[4];
} OPOSIO_MSRTRACKS;


// Structures for support of Scale
#define SCALE_MSG_SIZE	8

typedef struct io_opos_scale
{
	OPOSIO_HDR	hdr;
	long		weight;
} OPOSIO_SCALE;


#define MICR_DATA_OFFSET		12
typedef struct io_opos_micr
{
	OPOSIO_HDR	hdr;
	DWORD		dataLen;
	BYTE		Data[4];
} OPOSIO_MICR;

#define MAXIMUM_BUFFER_SIZE (8192*3)

#define CASHDRAWER_LENGTH_SOCKET	17
typedef struct io_opos_cashdrawer_socket
{
    tSpecHeaderV01 Spec;
	OPOSIO_HDR	hdr;
	BYTE		drawerStatus;
	BYTE		reserved[3];
} OPOSIO_CASHDRAWER_SOCKET;


typedef struct io_opos_linedisplay_descriptor_socket
{
	tSpecHeaderV01 Spec;
	BYTE  Data[4];
} OPOSIO_LD_DESCRIPTOR_SOCKET;

typedef struct io_opos_micr_socket
{
    tSpecHeaderV01 Spec;
	BYTE		Data[4];
} OPOSIO_MICR_SOCKET;

typedef struct io_opos_printer_socket
{
	tSpecHeaderV01 Spec;
	BYTE		Data[4];
} OPOSIO_PRINTER_SOCKET;
#pragma pack()

#endif // _TBOPOSIO_H_