#include "stdafx.h"
#include <fcntl.h>
#include <stdio.h>

//#define NO_BINARY_CONVERSION

#include "opos.h"
#include "CMClass.h"

#ifndef HT_STANDALONE
  #include "CommonNCR.h"
  #include "CommonServices.h"

  #define COMP_ID ID_HT
  #define T_ID _T("HT")
#else
  #define IDW_HTPWINDOW 101
#endif

#include "HTProcedures.h"

#define HARDTOTALS_DEVICE_NAME _T("ScotHTotals")

#define HT_DEF_FILESIZE        2048


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Template Collection class helper to delete elements
// in the map whenever RemoveKey or RemoveAll is called
// This function has been deprecated in VC7.0.  Destructors are 
// to be called instead.
#if ( _MFC_VER < 0x700 )
template <> void AFXAPI DestructElements <PHTFILEINFO> ( PHTFILEINFO* ppFileInfo, int nCount )
{    
  for ( int i = 0; i < nCount; i++, ppFileInfo++ )
  {
    if (*ppFileInfo)
      delete *ppFileInfo;
  }
}
#endif

HTProcedures::HTProcedures()
{
  m_bCreated = false;
	m_bOpened  = false;
  m_bClaimed = false;
	m_bEnabled = false;
  m_FileInfoMap.RemoveAll();

}

HTProcedures::~HTProcedures()
{
  UnInitialize();
}


long HTProcedures::Initialize(CWnd* pWnd)
{
  long rc = 0;

#ifdef NO_BINARY_CONVERSION
  m_bUseBinaryConversion = false;
#else
  m_bUseBinaryConversion = true;
#endif

  if (!m_bCreated)
  {
    BOOL bRC;

    if(pWnd == NULL)
    {
      pWnd = AfxGetMainWnd();
    }
    if (pWnd)
    {
      bRC = Create(NULL,
                   _T("SCOTHTProcedures"), 
                   WS_DISABLED, 
                   CRect(0,0,0,0), 
                   pWnd, 
                   IDW_HTPWINDOW );
    }
    else
    {
      // We are probably not in MFC.  Let's check and if not, initialize MFC
      if (bRC = AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
      {
        // Lets create our own!
        WNDCLASS wndcls;

        memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL defaults

        wndcls.style         = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc   = ::DefWindowProc;
        wndcls.hInstance     = AfxGetInstanceHandle();
        wndcls.hIcon         = NULL; 
        wndcls.hCursor       = NULL;
        wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
        wndcls.lpszMenuName  = NULL;
        wndcls.lpszClassName = _T("SCOTHTProcedures");

        // Register new class
        if(bRC = AfxRegisterClass(&wndcls))
        {
          bRC = CreateEx(0,
                         _T("SCOTHTProcedures"), 
                         _T("SCOTHTProcedures"), 
                         WS_DISABLED, 
                         CRect(0,0,0,0), 
                         NULL, 
                         NULL);
        }

        if (bRC)
        {
          // If we are here, then we are not an MFC Win app, but
          // we were able to create our window.  So now initialize OLE
          AfxEnableControlContainer();
          OleInitialize(NULL);
        }
      }
    }

    if (bRC)
    {
      if (m_HardTotals.Create(NULL, NULL, CRect(0,0,0,0), this, 102))
      {
        m_bCreated = true;
        rc = m_HardTotals.Open(HARDTOTALS_DEVICE_NAME);
        if (rc == OPOS_SUCCESS)
        {
          m_bOpened = true;
        
          rc = m_HardTotals.ClaimDevice(0);  //TAR253061 - Using OPOSTotals.ocx now from 1.7
          if (rc == OPOS_SUCCESS)
          {
            m_bClaimed = true;
            m_HardTotals.SetDeviceEnabled(TRUE);
            m_bEnabled = true;
            if (m_bUseBinaryConversion)
              m_HardTotals.SetBinaryConversion(OPOS_BC_NIBBLE);
          }
        }
        else
          rc = -1;
      }
      else
      {
        rc = -1;
      }
    }
  }

  if (rc != 0)
    UnInitialize(); 

  return rc;
}


long HTProcedures::UnInitialize(void)
{
  long rc = 0;

  if (m_bEnabled)
  {
    m_HardTotals.SetDeviceEnabled(FALSE);
    m_bEnabled = false;
  }

  if (m_bClaimed)
  {
    m_HardTotals.ReleaseDevice(); 	//TAR253061 - Using OPOSTotals.ocx now from 1.7
    m_bClaimed = false;
  }

  if (m_bOpened)
  {
    m_HardTotals.Close();
    m_bOpened = false;
  }



  if (m_bCreated)
  {
    m_HardTotals.DestroyWindow();
    m_bCreated = false;
  }

  m_FileInfoMap.RemoveAll();

  return rc;
}


long HTProcedures::Read(long handle, void * buffer, long offset, long count)
{
  long rc;

  if (buffer)
  {
    BSTR buf = SysAllocStringLen(NULL, count*2);

    rc = m_HardTotals.Read(handle, 
                           &buf, 
                           m_bUseBinaryConversion ? offset : offset*2, 
                           m_bUseBinaryConversion ? count  : count*2);
    if (rc == OPOS_SUCCESS)
    {
      CString work = buf;
      ConvertOposAsciiToBinary(work, OPOS_BC_NIBBLE);
      memcpy(buffer, (LPCTSTR) work, count);
    }
    SysFreeString(buf);
  }
  else
    rc = OPOS_E_ILLEGAL;

  return rc;
}


long HTProcedures::Write(long handle, const void * buffer, long offset, long count)
{
  long rc;

  if (buffer)
  {
    CString data;

    memcpy(data.GetBuffer(count), buffer, count);
    data.ReleaseBuffer(count);

    ConvertBinaryToOposAscii(data, OPOS_BC_NIBBLE);

    rc = m_HardTotals.Write(handle, 
                            data, 
                            m_bUseBinaryConversion ? offset : offset*2, 
                            m_bUseBinaryConversion ? count  : count*2);
  }
  else
    rc = OPOS_E_ILLEGAL;

  return rc;
}

long HTProcedures::DosOpen(LPCTSTR fileName, int flags, long fileSize)
{
  // Although many different flag combos can be specified, we don't care
  // about any of the read/write/rdonly/wronly/append flags, as we don't have a good
  // way to enforce them. 

  bool bWantCreate = false, bMustCreate = false, bWantTrunc = false;

  if (flags & _O_CREAT)
  {
    bWantCreate = true;
    if (flags & _O_EXCL)
      bMustCreate = true;
  }

  if (flags & _O_TRUNC)
    bWantTrunc = true;

  long rc, openRC;
  long handle;
  long dosFileSize;

  rc = m_HardTotals.Find(fileName, &handle, &dosFileSize);

  if (rc == OPOS_SUCCESS)
  {
    if (!m_bUseBinaryConversion)
      dosFileSize /= 2;

    if (bMustCreate)
      openRC = -1;
    else
      openRC = handle;
  }
  else
  {
    if (!bWantCreate)
      openRC = -1;
    else
    {
      // if Find above didn't work,
      // then the Datamatics SO stomps on fileSize
      if (fileSize == 0)
        dosFileSize = HT_DEF_FILESIZE;
	  else
        dosFileSize = fileSize;

      // if we aren't using binary conversion, then we need
      // twice as much space to store in CMOS, so double our size
      rc = m_HardTotals.Create(fileName, 
                               &handle, 
                               m_bUseBinaryConversion ? dosFileSize : dosFileSize * 2, 
                               FALSE);
      if (rc == OPOS_SUCCESS)
      {
        openRC = handle;
      }
      else
        openRC = -1;
    }
  }

  if (openRC >= 0)
  {
    m_HardTotals.ClaimFile(handle, 0);

    if (bWantTrunc)
      m_HardTotals.SetAll(handle, 0);

    PHTFILEINFO pInfo = new HTFILEINFO;

    memset(pInfo, 0, sizeof(HTFILEINFO));
    pInfo->fileSize    = dosFileSize;
    pInfo->fileEnd     = dosFileSize - 1;
    pInfo->filePointer = 0;
    pInfo->fileFlags   = flags;

    m_FileInfoMap.SetAt(handle, pInfo);
  }

  return openRC;

}


long HTProcedures::DosClose(long handle)
{
  PHTFILEINFO pInfo;

  if (m_FileInfoMap.Lookup(handle, pInfo))
  {
    m_FileInfoMap.RemoveKey(handle);

    return m_HardTotals.ReleaseFile(handle);
  }
  else
    return -1;
}


long HTProcedures::DosRead(long handle, void *buffer, unsigned int count)
{
  PHTFILEINFO pInfo;
  long rc = -1;

  if (m_FileInfoMap.Lookup(handle, pInfo))
  {
    long nextPos = pInfo->filePointer + count;

    // If we ask to read too much, lower the count to the right amount.
    // The application will see it is EOF because the ret value < count;

    if (nextPos > pInfo->fileSize)
      count -= (nextPos - pInfo->fileSize);

    rc = Read(handle, buffer, pInfo->filePointer, count);
    if (rc == OPOS_SUCCESS)
    {
      rc = pInfo->numBytesRead = count;
      pInfo->filePointer += count;
    }
    else
    {
      pInfo->lastOposErr = rc;
      rc = -1;
    }
  }
  else
    rc = -1;

  return rc;
}


long HTProcedures::DosWrite(long handle, const void *buffer, unsigned int count)
{
  PHTFILEINFO pInfo;
  long rc = -1;

  if (m_FileInfoMap.Lookup(handle, pInfo))
  {
    long nextPos = pInfo->filePointer + count;

    // If we ask to write too much, return error as per DOS's _write()
    if (nextPos <= pInfo->fileSize)
    {
      rc = Write(handle, buffer, pInfo->filePointer, count);
      if (rc == OPOS_SUCCESS)
      {
        rc = pInfo->numBytesWritten = count;
        pInfo->filePointer += count;
      }
      else
      {
        pInfo->lastOposErr = rc;
        rc = -1;
      }
    }
    else
      rc = -1;
  }
  else
    rc = -1;

  return rc;

}


long HTProcedures::DosSeek(long handle, long offset, int origin)
{
  long rc = -1;
  PHTFILEINFO pInfo;

  if (m_FileInfoMap.Lookup(handle, pInfo))
  {
    long newPos;

    switch(origin)
    {
      case SEEK_SET:  newPos = offset;
                      break;
      case SEEK_CUR:  newPos = pInfo->filePointer + offset;
                      break;
      case SEEK_END:  newPos = pInfo->fileSize - offset;
                      break;
      default:        newPos = -1;
    }
    
    if ((newPos >= 0) && (newPos <= pInfo->fileSize))
    {
      rc = pInfo->filePointer = newPos;
    }
  }

  return rc;
}

long HTProcedures::PurgeAll()
{
  long index;
  unsigned short numFiles = 0, numFilesDeleted = 0;

  CString fileName;

  numFiles = (short) m_HardTotals.GetNumberOfFiles();
  for (index = 0; index < numFiles; index++)
  {
    BSTR fileNameBstr = SysAllocStringLen(NULL, 128);
    if (m_HardTotals.FindByIndex(0, &fileNameBstr) == OPOS_SUCCESS)
    {
      fileName = fileNameBstr;
      if (m_HardTotals.Delete(fileName) == OPOS_SUCCESS)
        numFilesDeleted++;
    }
    SysFreeString(fileNameBstr);
  }

  return ((numFiles << 16) + numFilesDeleted);
}


long HTProcedures::DosTell(long handle)
{
  long rc = -1;
  PHTFILEINFO pInfo;

  if (m_FileInfoMap.Lookup(handle, pInfo))
  {
    rc = pInfo->filePointer;
  }

  return rc;
}

long HTProcedures::Delete(LPCTSTR fileName)
{
  return m_HardTotals.Delete(fileName);
}
