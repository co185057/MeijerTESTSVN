#if !defined(AFX_GTSINFOFIELDS_H__0FF6BB92_5F50_4AF1_BFDF_4715F53BC4DB__INCLUDED_)
#define AFX_GTSINFOFIELDS_H__0FF6BB92_5F50_4AF1_BFDF_4715F53BC4DB__INCLUDED_

// FCode
#define GTS_PIPE_ACTION_WRITE			0x0B

// DeviceID
#define GTS_KEYLOCK_MESSAGE				0x10
#define GTS_DISPLAY_MESSAGE				0x20
#define GTS_PRINTER_MESSAGE				0x30
#define GTS_SCANNER_MESSAGE				0x4A
#define GTS_RAW_MESSAGE					  0x77
#define GTS_MSR_MESSAGE           0x78
#define GTS_KEY_PRESS_MESSAGE			0x81


// Option
//		Display Message
#define GTS_2X20_MESSAGE				  0x10
#define GTS_GUIDANCE_MESSAGE			0x11
#define GTS_JOURNAL_MESSAGE				0x12
#define GTS_PIN_PAD_MESSAGE				0x13
//		RAW Message
#define GTS_ITEM_MESSAGE_OPTION			    0x00
#define GTS_TENDER_DETAIL_OPTION		    0x01
#define GTS_TRANSACTION_MESSAGE_OPTION	0x02

//		Printer Message
#define GTS_TEXT_MESSAGE				  0x00
#define GTS_BARCODE_MESSAGE				0x01
#define GTS_BITMAP_MESSAGE				0x02

// Printer Message
#define GTS_RECEIPT_MESSAGE				0x00
#define GTS_SLIP_PRINT_MESSAGE		0x01

// Prompt Type
#define GTS_STANDARD_PROMPT				0x00
#define GTS_GENERIC_PROMPT				0x01

// Data Type
#define GTS_NUMERIC_DATA				  0x00
#define GTS_ALPHANUMERIC_DATA			0x01

// Action
#define GTS_CASHIER_ENTERS_DATA		0x00
#define GTS_CUSTOMER_ENTERS_DATA	0x01
#define GTS_DISPLAYS_INFO_MSG			0x02
#define GTS_IGNORE_PROMPT 		    0x03

// Entry Need
#define GTS_REQUIRED_DATA	  	  	0x00
#define GTS_OPTIONAL_DATA	  	  	0x01

#define GTS_PRINTER_SIZE 50
#define GTS_JOURNAL_SIZE 40
#define GTS_PIN_PAD_SIZE 30
#define GTS_SCANNER_SIZE 30
#define GTS_KEY_PRESS_SIZE 30
#define GTS_MSR_SIZE 30

#define GTS_MSR_INDICATOR "MSR"
#define GTS_MSR_INDICATOR_LENGTH 3

#pragma pack( 1)

typedef struct
{
	long Length;
	char filler1[4];
	unsigned char FCode;
	char filler2[23];
	char filler3;
	unsigned char DeviceID;
	char filler4[14];
	unsigned char UnitType;
	unsigned char Option;
	char filler6[2];
	unsigned char PromptType;
	unsigned char DataType;
	unsigned char Action;
	unsigned char EntryNeed;
	char filler7[4];
	long DataOffset;
	long DataSize;
	long ParmOffset;
	long ParmSize;
	char filler8[12];
} GTS_INFO_FIELDS;
#define GTS_INFO_FIELDS_SIZE sizeof( GTS_INFO_FIELDS)

#pragma pack()

#endif
