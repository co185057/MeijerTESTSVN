//////////////////////////////////////////////////////////////////////////////////////////////////
// Start of JZB.SSF.5.0.0

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmGetSpecialFuncValue.h
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmGetSpecialFuncValueSTATE
#define _SmGetSpecialFuncValueSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

SMSmGetSpecialFuncValue : public STATE(StoreModeState)
{
public:
    SMSmGetSpecialFuncValue();					  // constructor
    
    SMStateBase  *Initialize(void);
    void UnInitialize(void);
    SMStateBase * PSEnterKey();
    SMStateBase * PSNumericKey(const long lKey);
    SMStateBase * PSDecimalKey(const long lKey);
    SMStateBase * PSClearKey();
    SMStateBase *PSButton8(void);
        
    DECLARE_DYNCREATE(SMSmGetSpecialFuncValue)// MFC Runtime class/object information
};

#endif
// end of JZB.SSF.5.0.0
/////////////////////////////////////////////////////////////////////