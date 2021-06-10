#ifndef _SIGCAPUTILITY_H_
#define _SIGCAPUTILITY_H_
#include "SigCap.h"
#include "comp.h"
#include "opos.h"


#define SCOT_DEFAULT_SIGFORM      _T("SCOT.SigcapForm")

// Compression Code

typedef struct 
{
  BYTE b05;      // Always 0x05
  BYTE bVli;     // Determined at the end
  BYTE bFc;      // Function Code
  BYTE bStatus;  // 0x80, 0x00 or error
  BYTE bSeq;     // Sequence Number
} MSG_HEADER_5991;

typedef struct 
{
  BYTE bFormat;
  BYTE bReserved1;
  BYTE bReserved2;
  BYTE bRecordType : 4;
  BYTE bResolution : 4;
} SIG_HEADER_5991;

class CSigCapUtility
{
   
public:

   CSigCapUtility();
   virtual ~CSigCapUtility();

   int FindNumPoints(const BSTR bstrStrokeData, int nLength );

};
#endif // _SIGCAPUTILITY_H_

