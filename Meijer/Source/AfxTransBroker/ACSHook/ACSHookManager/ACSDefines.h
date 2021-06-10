#pragma once

//
// ATL and other common include headers & imports
//
#pragma warning( push )

#include <varargs.h>
#pragma warning( disable:4005 )	// stdarg.h included in atlbase.h and generates dup def. - we need to use varargs.h
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atlfile.h>
#include <atlpath.h>
#include <atltime.h>
#include <atlcoll.h>
#include <atlsync.h>
#include <atlstr.h>
#include <atlconv.h>
#include <atlrx.h>

#pragma warning( pop )



using namespace ATL;

#import <ProgID:MSXML2.FreeThreadedDomDocument.4.0> rename_namespace("MSXML2")
#import <ProgID:tbControls.tbConnect.1> rename_namespace("TBC")
#import <ProgID:traceExe.TraceWriter.1> rename_namespace("TRC")

//
// forward declarations
//
class ACS;
class CPSAPI;
class COptions;
class CTB;
class CUtils;
class CACSIOClient;
class CACSExposedFunctions;
class CACSContext;
class CAppModel;
class CControls;
class CContexts;
class CProperties;
class CACSHook;

#include "ACSTypes.h"

//
// Global utilities class used by all
//
extern CUtils Utils;


//
// ACS Solution includes
//
#include "ACS.h"
#include "CPSAPI.H"
#include "Options.h"
#include "TB.h"
#include "Utils.h"
#include "ACSIOClient.h"
#include "ACSExposedFunctions.h"
#include "Controls.h"
#include "Contexts.h"
#include "Properties.h"
#include "ACSHook.h"


