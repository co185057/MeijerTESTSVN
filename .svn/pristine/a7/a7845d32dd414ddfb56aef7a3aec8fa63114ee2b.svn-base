//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMLookUpItem.h
//
// TITLE: Class header for lookup item(type item description) state
//
// TIME:  01/14/98
//
// AUTHOR:    Feng Zhang
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LOOKUPITEMSTATE
#define _LOOKUPITEMSTATE

#include "SMLookUpItemBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMLookUpItem : public SMLookUpItemBase
{
public:
    SMLookUpItem();
    virtual SMStateBase  *Initialize(void);
    virtual void     UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSSpaceKey(void);
    virtual SMStateBase  *PSEnterKey(void);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *DMScanner(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *PSShiftKey(void);
    virtual SMStateBase  *PSAltGrKey(void);
    virtual SMStateBase  *PSHotKey(CString &);
    virtual SMStateBase  *PSCharKey(TCHAR c);
    virtual void SetPSContext();


  DECLARE_DYNCREATE(SMLookUpItem) // MFC Runtime class/object information
		
};

#endif