//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateRP.h
//
//
// AUTHOR:Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSTATERP
#define _SMSTATERP

protected:
  virtual SMStateBase *RPParse(MessageElement *me);
  virtual long RPInitialize(void);
  virtual long RPUnInitialize(void);

public:
  virtual SMStateBase *OnRPReceived(long byteCount);
  virtual SMStateBase *OnRPDisconnect();
  virtual SMStateBase *SendRPDisconnect();
  virtual SMStateBase *SendRPConnect();

#endif