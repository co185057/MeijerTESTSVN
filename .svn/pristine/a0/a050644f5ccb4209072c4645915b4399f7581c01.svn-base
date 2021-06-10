//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmVisualItems.CPP
//
// TITLE: Class implementation for store mode Visual Items state
//
//
// AUTHOR:  gary miller
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmVisualItems.h"        // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmVisualItems")

IMPLEMENT_DYNCREATE(SMSmVisualItems, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////
SMSmVisualItems::SMSmVisualItems()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmVisualItems::Initialize(void)
{
	SMStateBase  *pRetState = SMSmVisualItemsBase::Initialize();
	bStoreModeApprovals = false;
	return pRetState;
}

SMStateBase * SMSmVisualItems::PSButton1(void)
{
    return SMSmVisualItemsBase::PSButton1();
}

SMStateBase * SMSmVisualItems::PSButton2(void)
{
    return SMSmVisualItemsBase::PSButton2();
}

SMStateBase * SMSmVisualItems::PSButton8(void)
{
    return SMSmVisualItemsBase::PSButton8();
}

SMStateBase * SMSmVisualItems::PSReceiptUp(void)
{
    return SMSmVisualItemsBase::PSReceiptUp();
}

SMStateBase * SMSmVisualItems::PSReceiptDown(void)
{
    return SMSmVisualItemsBase::PSReceiptDown();
}

/*sscoadk-5431
SMStateBase * SMSmVisualItems::RAParse(MessageElement* me)
{
    return SMSmVisualItemsBase::RAParse(me);
}
*/
