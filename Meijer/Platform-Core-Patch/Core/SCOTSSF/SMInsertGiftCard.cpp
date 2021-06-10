//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMInsertGiftCard.CPP
//
// TITLE:	Class implementation for InsertGiftCard state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMInsertGiftCard.h"           // MGV this state


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("InsertGiftCard")

IMPLEMENT_DYNCREATE(SMInsertGiftCard, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMInsertGiftCard::SMInsertGiftCard()
{
  IMPLEMENT_TIMESTAMP
}


