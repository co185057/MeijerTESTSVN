//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  PSStoreModeBase.CPP
//
// TITLE:   Display Presentation Services Store Mode funcs.
//
// This module shows all store mode screens
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "CommonNCR.h"              // NCR common includes
#include "Common.h"					// MGV common includes
#include "CommonServices.h"         // MGV common includes

#include "PSProcedures.h"           // MGV this header
//@@@INFOPOINT - Getting Strange compile errors
//#include "PSInterface.h"            // MGV low level PS header

// local defines
#define COMP_ID   ID_PS
#define T_ID      _T("PSsmBase")
#define WHOLEVEL    L7

///////////////////////////////////////////////////////////////////////

#include "PSIntList.h"


DLLEXPORT extern HDEVCONTEXT hDC;             // handle to the device context from PSThread()


