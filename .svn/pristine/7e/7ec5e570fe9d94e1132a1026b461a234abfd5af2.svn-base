#pragma once

//
// simple string map used by Properties and other utility functions
//
typedef CAtlMap<CString, CString, CStringElementTraits<CString> > STRINGMAP, * PSTRINGMAP;
typedef STRINGMAP::CPair STRINGMAPITEM, * PSTRINGMAPITEM;


//
// Property object with context binding information
//
typedef struct _PROPERTY
{
	CString strValue;
} PROPERTY, *PPROPERTY;
typedef CAtlMap<CString, PROPERTY, CStringElementTraits<CString> > PROPERTYMAP, * PPROPERTYMAP;
typedef PROPERTYMAP::CPair PROPERTYMAPITEM, * PPROPERTYMAPITEM;

//
// Type definitions - for Contexts and Context Field Requirement structs
// These structures also hold the last saved field values
//
typedef struct _APPCONTEXTFIELD
{
	CString fieldName;
	CString strLastSavedValue;
} APPCONTEXTFIELD, * PAPPCONTEXTFIELD;

typedef CAtlMap<CString, APPCONTEXTFIELD, CStringElementTraits<CString> > APPCONTEXTFIELDMAP, * PAPPCONTEXTFIELDMAP;
typedef APPCONTEXTFIELDMAP::CPair APPCONTEXTFIELDMAPITEM, * PAPPCONTEXTFIELDMAPITEM;

typedef struct _APPCONTEXT
{
	DWORD dwLastSavedTime; // relative value in milliseconds
	BOOL bValidContents;	// if it has been reset, then the contents are not valid
	APPCONTEXTFIELDMAP fieldMap;
} APPCONTEXT, * PAPPCONTEXT;

typedef CAtlMap<CString, APPCONTEXT, CStringElementTraits<CString> > APPCONTEXTMAP, * PAPPCONTEXTMAP;
typedef APPCONTEXTMAP::CPair APPCONTEXTMAPITEM, * PAPPCONTEXTMAPITEM;

//
// Types used in conjunction with the CControls class
//
typedef struct _CONTROLFIELD
{
	CString strLastSavedValue;
} CONTROLFIELD, * PCONTROLFIELD;
typedef CAtlMap<CString, CONTROLFIELD, CStringElementTraits<CString> > CONTROLFIELDMAP, * PCONTROLFIELDMAP;
typedef CONTROLFIELDMAP::CPair CONTROLFIELDMAPITEM, * PCONTROLFIELDMAPITEM;

typedef struct _myCONTROL
{
	DWORD dwLastSavedTime;
	DWORD dwLastSentTime;
	TBC::ITBGenObjPtr spGenCtrl;
	CONTROLFIELDMAP fieldMap;
} CONTROL, * PCONTROL;
typedef CAtlMap<CString, CONTROL, CStringElementTraits<CString> > CONTROLMAP, * PCONTROLMAP;
typedef CONTROLMAP::CPair CONTROLMAPITEM, * PCONTROLMAPITEM;