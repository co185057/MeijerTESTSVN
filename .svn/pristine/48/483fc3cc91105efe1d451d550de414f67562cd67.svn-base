// HeaderGenerator.cpp : Defines the entry point for the console application.
//
//    The Header generator project will generate COM Header tlh and tli files
//    for use by the Scotapp Buildlib,  Scotssf, and Scotapp projects.  Using
//    the pre-built header files will allow the SSF to be built without installing
//    the com objects on the system.
//    The Scotapp #imports of the dlls have been changed to use includes of the 
//    .tlh files instead.
//    The header generator also modifies the generated .tlh file to change a 
//    full path reference to it's corresponding .tli file to a <> reference.
//    The post-build steps of the project will run the HeaderGenerator executable
//    and copy the files to the source/include directory.  
//    The Scotapp Jenkins build process will run the HeaderGenerator build as one of the
//    first steps for the build,  and later move the generated files to the dev kit 
//    include directory.

#include "stdafx.h"
#include "io.h"
#include "HeaderGenerator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#include "HeaderHandler.h"
#import <NCRPersonalizationU.dll> rename_namespace("PERSN")
#import <GTOU.dll> rename_namespace("GTO")
#import "C:\\scot\\controls\\vFormControlObject.dll"
#import "c:\scot\controls\WeightInfoU.dll" no_namespace, named_guids
#import <FLTCPConnectorU.dll> rename_namespace("FLTCP")

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
        _tprintf(_T("**** Running HeaderGenerator.exe ****\n"));
        _tprintf(_T("Generating COM Type Library headers \n"));
		// TODO: code your application's behavior here.
        CHeaderHandler NCRPersonalizationHeader(CString(_T("NCRPersonalizationU.dll")));
        CHeaderHandler GTOHeader(CString(_T("GTOU.dll")));
        CHeaderHandler VFormControlHeader(CString(_T("vFormControlObject.dll")));
        CHeaderHandler WeightInfoHeader(CString(_T("WeightInfoU.dll")));
        CHeaderHandler FLTCPConnectorU(CString(_T("FLTCPConnectorU.dll")));
        
	}

	return nRetCode;
}



