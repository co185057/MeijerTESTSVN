//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmEnterBirthdate.h
//
// TITLE: Class header for store mode Restricted Items state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMEnterBirthdateSTATE
#define _SMEnterBirthdateSTATE

#include "SMSmEnterBirthdateBase.h"

#define AGE_APPROVED        1
#define AGE_NOT_APPROVED    2
#define AGE_INVALID         3

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmEnterBirthdate : public SMSmEnterBirthdateBase
{
public:
    SMSmEnterBirthdate();					  // constructor
    virtual SMStateBase * Initialize(void);
    virtual void          UnInitialize(void);

	virtual SMStateBase *DMScanner(void);
    virtual long VerifyDOB(void);
    long FindRequiredAge(void); 

protected:

	virtual SMStateBase  *PSButton2(void);         // override for button 2
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase  *PSEnterKey(void);		   // override for enter key
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *PSNumericKey(const long);

	void SetAgeRestrictedItemStatus(void);

    DECLARE_DYNCREATE(SMSmEnterBirthdate)     // MFC Runtime class/object information
};

#endif
