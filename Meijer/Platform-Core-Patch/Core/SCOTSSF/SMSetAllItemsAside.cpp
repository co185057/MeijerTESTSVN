//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSetAllItemsAside.cpp
//
// TITLE: Class implimentation for set items aside State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSetAllItemsAside.h"


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SetAllItemsAside")

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMSetAllItemsAside, CObject)// MFC Runtime class/object information


////////////////////////////////////////
SMSetAllItemsAside::SMSetAllItemsAside()
{
  IMPLEMENT_TIMESTAMP
}
