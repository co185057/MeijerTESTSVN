#ifndef _HARD_TOTALS_INCLUDED
#define _HARD_TOTALS_INCLUDED

//
#include "HardTotals.h"	// Added by ClassView


typedef struct
{
  long fileSize;
  long fileEnd;    // Always equals fileSize - 1;
  long filePointer;
  int  fileFlags;
  long numBytesRead;
  long numBytesWritten;
  long lastOposErr;
  long lastOposExtErr;
} HTFILEINFO;

typedef HTFILEINFO *PHTFILEINFO;

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
      HTProcedures : public CWnd
{
public:
	long Read(long handle, void* buffer, long offset, long count);
	long Write(long handle, const void* buffer, long offset, long count);
	long Delete(LPCTSTR fileName);

  // The Dos... functions emulate the behavior of a virtual FAT drive
  // for CMOS - file pointers, seeking, etc.  
  
  // Default file size if 0 = 2K (4K if Service object doesn't support Binary conversion)
	long DosOpen(LPCTSTR fileName, int flags, long fileSize = 0);
	long DosClose(long handle);
  long DosRead(long handle, void *buffer, unsigned int count);
  long DosWrite(long handle, const void *buffer, unsigned int count);
  long DosSeek(long handle, long offset, int origin);
  long DosTell(long handle);

	long PurgeAll();

  HTProcedures();
  virtual ~HTProcedures();
  long Initialize(CWnd* pWnd = NULL); // called by MP at start up
  long UnInitialize(void);            // called by MP at shut down

protected:
	CHardTotals m_HardTotals;

	bool m_bCreated;
	bool m_bOpened;
  bool m_bClaimed;
	bool m_bEnabled;
	bool m_bUseBinaryConversion;

  CMap<long, long, PHTFILEINFO, PHTFILEINFO> m_FileInfoMap;
};

#endif