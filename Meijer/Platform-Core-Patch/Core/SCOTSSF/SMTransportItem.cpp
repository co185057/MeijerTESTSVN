//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTransportItem.CPP
//
// TITLE: Class header for Transport Item State
//
// $Header:
//
// AUTHOR:    Thanhhieu Pham
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMTransportItem.h"           // MGV this state


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("TransportItem")

IMPLEMENT_DYNCREATE(SMTransportItem, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMTransportItem::SMTransportItem()
{
  IMPLEMENT_TIMESTAMP
}
