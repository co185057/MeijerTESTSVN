// SCOTSigCap.cpp: implementation of the CSCOTSigCap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "opos.h"
#include <malloc.h>
#include "SigCapUtility.h"



// Include stuff needed to do 5991 Compressed format.
// Since no one really understands this stuff, we will just use
// as much provided code as possible.

extern "C" 
{
  #include "Comp.c"
}

#include "Common.h"                 // common includes
#include "SCOTSigCap.h"
#include "DMProcedures.h"           // DM procedures

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define COMP_ID ID_OD
#define T_ID    _T("SCOT SigCap")

#define SIG_VALID_PTS_THRESHOLD   10 // This is what DM uses currently also

char  compress_alg = 0x01;      /*  defines compression algorithm  */


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSCOTSigCap::CSCOTSigCap()
{
#ifndef _CPPUNIT
  m_bHaveValidSig = FALSE;
  m_hDispWnd = NULL;
#endif //_CPPUNIT
}

CSCOTSigCap::~CSCOTSigCap()
{

}




