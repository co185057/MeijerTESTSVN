//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectContainer.cpp
//
// TITLE: Class impliementation for void item state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSelectContainer.h"      // MGV this

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SelectContainer")

IMPLEMENT_DYNCREATE(SMSelectContainer, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSelectContainer::SMSelectContainer()
{
  IMPLEMENT_TIMESTAMP
}

