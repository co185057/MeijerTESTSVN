//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	  SMStateRPBase.h
//
//
// AUTHOR:    Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSTATERPBASE
#define _SMSTATERPBASE

#define RP_RECEIVED               1001
#define RP_DISCONNECTED           1000

protected:
  virtual SMStateBase *RPParse(MessageElement *me);
  virtual long RPInitialize(void);
  virtual long RPUnInitialize(void);

public:
  virtual SMStateBase *OnRPReceived(long byteCount);
  virtual SMStateBase *OnRPDisconnect();
  virtual SMStateBase *SendRPDisconnect();
  virtual SMStateBase *SendRPConnect();
  bool m_bDisplayReport;
  bool m_bInitialMenu;
  static CString m_csCurrentReport;
  static CString m_csRPT;

#endif
