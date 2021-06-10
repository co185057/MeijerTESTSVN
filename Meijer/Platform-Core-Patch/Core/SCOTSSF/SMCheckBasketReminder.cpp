//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCheckBasketReminder.cpp
//
// TITLE: Class implimentation for Check Basket Reminder State
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes
 
#include "SMState.h"                // MGV base state
#include "SMCheckBasketReminder.h"          // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("CheckBasketReminder")

IMPLEMENT_DYNCREATE(SMCheckBasketReminder, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMCheckBasketReminder::SMCheckBasketReminder()
{
  IMPLEMENT_TIMESTAMP
}
