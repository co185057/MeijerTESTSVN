//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmVisualItems.h
//
// TITLE: Class header for store mode Visual Items state
//
//
// AUTHOR: gary miller
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMVISUALITEMSSTATE
#define _SMVISUALITEMSSTATE

#include "SMSmVisualItemsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmVisualItems : public SMSmVisualItemsBase
{
public:
    SMSmVisualItems();							  // constructor
    virtual SMStateBase  *Initialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase *PSReceiptUp(void);
    virtual SMStateBase *PSReceiptDown(void);
    //virtual SMStateBase *RAParse(MessageElement* ); sscoadk-5431

    DECLARE_DYNCREATE(SMSmVisualItems)// MFC Runtime class/object information
};

#endif