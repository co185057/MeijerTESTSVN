#ifndef _PIPEIOHDR_H_
#define _PIPEIOHDR_H_


#define NT_MONITOR_PIPE   "\\\\.\\PIPE\\MONITOR"
#define NT_ORIGINATE_PIPE "\\\\.\\PIPE\\ORIGINAT"
#define TB_MONITOR_PIPE   "\\\\.\\PIPE\\MONITOR"
#define TB_ORIGINATE_PIPE "\\\\.\\PIPE\\ORIGINATE"

#define MON_ONLY_ESCAPE_ULONG 0x511B711B
#define MON_ONLY_ESCAPE_ASCII ".q.Q"


#define IOM_FNC_ECHO    0x75
#define IOM_FNC_EOP     0x11
#define IOM_FNC_LOGOFF  0x74
#define IOM_FNC_LOGON   0x73
#define IOM_FNC_VERIFY  0x72

#define IOM_FNC_CANCEL  0x82
#define IOM_FNC_CLOSE   0x04
#define IOM_FNC_CREATE  0x03
#define IOM_FNC_DELETE  0x06
#define IOM_FNC_DEVLOCK 0x83
#define IOM_FNC_GET     0x84
#define IOM_FNC_LOCK    0x0C
#define IOM_FNC_LOOKUP  0x86
#define IOM_FNC_OPEN    0x02
#define IOM_FNC_READ    0x0A
#define IOM_FNC_RENAME  0x07
#define IOM_FNC_SEEK    0x12
#define IOM_FNC_SET     0x87
#define IOM_FNC_SPECIAL 0x88
#define IOM_FNC_WRITE   0x0B
#define IOM_FNC_LOG	    0xDC

#define IOM_FNC_RDY2READ 0x99

typedef unsigned long   ulong;
typedef unsigned char   ubyte;
typedef unsigned short  ushort;

#pragma pack (1)

/*
typedef struct {
  UCHAR  msggrp;		// 4690 Message Group
  UCHAR  severity;		// 4690 Severity
  USHORT msgnum;		// 4690 Message Number
  UCHAR  event;		// 4690 Event
  UCHAR  len_data;		// Length of data in unique
  UCHAR  applname[8];	// Name the application generating the event
  UCHAR  unique[10];	// Up to 10 characters of unique data
} ADX_ERROR;

*/
typedef struct io_mon_hdr
{
  ulong  Len;           // 00 // Length of pipe data (inclusive of Len)
  ulong  XactId;        // Transaction Id is a value chosen by the "requester" and returned by the "server" in the reply message.
  ubyte  Fcode;         // Function code (see above)
  char   Unused[1];     // Reserved for future use!
  ushort Flags;         // Flags byte:
  long   RetCode;       // Return code to be passed to appl 
  long   Revision;      // Revision number, ex. 00 01  02 03
  char   Unused2[12];   // Reserved for future use!
  
} IO_MON_HDR;

typedef struct io_mon_iopack
{
  ubyte  Unit;                      // 0  // Unit number
  ubyte  Option;                    // 1  // Option
  ushort Flags;                     // 2  // Flags
  ulong  Extra1;                    // 4  // Additional data associated with
  ulong  Fnum;                      // 8  // File handle
  ulong  Data;                      // c  // Offset from 0 in DataParm or an additional parameter
  ulong  DataSize;                  //10  // Number of bytes of data
  ulong  Parm;                      //14  // Offset from 0 in DataParm or an additional parameter
  ulong  ParmSize;                  //18  // Number of bytes of parm data
  ulong  Extra;                     //1c  // More additional data associated
  ulong  ExtraSize;                 //20  // with an I/O command (works like Data/DataSize)
  ushort Flags2;                    //24
  ushort Extra2;                    //28
  char   DataParm[4];               //2a  // 1st 4 bytes of variable-len buffer that includes Data, Parm and Extra.
} IO_MON_IOPACK;

typedef struct io_mon_packet        // Definition of I/O Monitor data
{
  IO_MON_HDR     Hdr;                // Header
  ubyte          HdrExt[16];         // Reserved space for I/O Monitor for header extension (subsystem specific)
  IO_MON_IOPACK  Iop;                // Iopack data
} IO_MON_PACKET;

//***************************************************************************
// POS I/O Subsystem specific Packet definitions
//***************************************************************************
typedef struct pos_io_mon_hdr_ext   // Definition of I/O Monitor header
{                                   // extension for POS subsystem
  ubyte  ApplNum;                   // Appl Num (1...n)
  ubyte  DeviceId;                  // Device Id.
  ubyte  Port;                      // Port Number
  char  unused[13];                 // Reserved for future use!
  
} POS_IO_MON_HDR_EXT;

typedef struct pos_io_mon_packet    // Definition of POS I/O Monitor data
{
  IO_MON_HDR      Hdr;              // Header
  POS_IO_MON_HDR_EXT HdrExt;        // Header Extension
  IO_MON_IOPACK   Iop;              // Iopack data
  
} POS_IO_MON_PACKET;


#define DEV_SCANNER   0x4A
#define DEV_KEYBOARD  0x81
#define DEV_SCALE     0x6a
#define DEV_SERIAL    0x66
#define DEV_MSR       0x47
#define DEV_KEYLOCK   0x10
#define DEV_PRINTER   0x30
#define DEV_CMOS      0x85
#define DEV_GTS_INFO  0x00  //hjr


#define MAX_PIPE_DATA_LEN 1000       // tsnt firm limit
#define MAX_PIPE_BUFF_LEN sizeof(POS_IO_MON_PACKET)+MAX_PIPE_DATA_LEN

#define MAX_FILE_HANDLES  50

#define POS_CMOS_RESPONSE       0x01

#define POS_CMOS_SUBREQ_PURGE   0x01

#define POS_CMOS_SEEK_SET       0x00
#define POS_CMOS_SEEK_CUR       0x01
#define POS_CMOS_SEEK_END       0x02

#define POS_CMOS_FL_BINARY      0x00
#define POS_CMOS_FL_RDONLY      0x01
#define POS_CMOS_FL_WRONLY      0x02
#define POS_CMOS_FL_RDWR        0x03
#define POS_CMOS_FL_APPEND      0x04
#define POS_CMOS_FL_TEXT        0x08
#define POS_CMOS_FL_CREATE      0x10
#define POS_CMOS_FL_EXCL        0x20
#define POS_CMOS_FL_TRUNC       0x40

#define POS_CMOS_PERM_RDONLY    0x01
#define POS_CMOS_PERM_RDWR      0x03

#pragma pack()

#endif // _PIPEIOHDR_H_