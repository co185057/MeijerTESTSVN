//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectFromList.h
//
// TITLE: Class implementation for Select From List Screen
//
// TIME:  07/12/98
//
// AUTHOR:   Feng Zhang

//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELECTFROMLISTSTATE
#define _SELECTFROMLISTSTATE

#include "SMSelectFromListBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMSelectFromList : public SMSelectFromListBase
{
public:
    SMSelectFromList();               // constructor
    virtual SMStateBase  *Initialize(void);
    virtual void     UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton4(void);
    virtual SMStateBase  *PSListLookup(CString);
    virtual SMStateBase  *PSNumericKey(const long);
    virtual SMStateBase  *PSEnterKey(void);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *PSCancelKey(void);
    virtual SMStateBase  *DMScanner(void);
    virtual SMStateBase  *PSButtonGoBack(void);

    DECLARE_DYNCREATE(SMSelectFromList)// MFC Runtime class/object information

};

#endif
