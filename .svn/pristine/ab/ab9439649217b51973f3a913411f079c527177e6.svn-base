#ifndef _CSCOT_SIGCAP_H
#define _CSCOT_SIGCAP_H
#include "SCOTDevice.h"
#include "SigCapUtility.h"

class CSCOTSigCap : public CSCOTDevice
{
public:
  CSCOTSigCap() {};
  virtual ~CSCOTSigCap() {} ;
  virtual long Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID) = 0;
  virtual long UnInitialize() = 0;
  virtual long Disable() = 0;
  virtual long Enable() = 0;
  virtual void DataEvent(long Status) {};
  virtual void ErrorEvent(LONG ResultCode, 
                          LONG ResultCodeExtended, 
                          LONG ErrorLocus, 
                          LONG* pErrorResponse) {};

  virtual long BeginCapture(LPCTSTR szCaptureForm,
                            HWND hDrawWnd = NULL) = 0;
  virtual long EndCapture() = 0;
  virtual long Get5991Format(BSTR* pData) = 0;
  virtual long ClearDisplay() { return 0; };
  virtual bool IsOnScreenCapture() const { return false;};

  virtual CString GetcsSigData() const { return _T("");};
  virtual void SetcsSigData(const CString &csData){};
  virtual long GetWidth() const { return 0;};
  virtual long GetHeight() const { return 0;};

  BOOL m_bHaveValidSig;
  HWND m_hDispWnd;


};
#endif // _ISCOT_DEVICE_H
