//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMProduceFavorites.cpp
//
// TITLE: Class implementation for produce favorites state
//
// TIME:  08/8/00
//
// AUTHOR:    Dale Ehrahrdt
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // common includes

#include "SMState.h"                // base state
#include "SMProduceFavorites.h"           // this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("ProduceFavorites")


//DLLEXPORT char* PRS_CMKEYBOARD="wCMKeyboard"; // Std KeyBoard
											                   

IMPLEMENT_DYNCREATE(SMProduceFavorites, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMProduceFavorites::SMProduceFavorites()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMProduceFavorites::Initialize()
{
  return SMProduceFavoritesBase::Initialize();
}
