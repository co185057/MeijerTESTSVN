 //////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // FILE:  SMSmSelectModeOfOperation.CPP
    //
    // TITLE: Class implementation for store mode System Functions state
    //
    // AUTHOR:  Renato Taer
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    #include <stdafx.h>                 // MS MFC always needed first
    #include <afxtempl.h>               // MS MFC template header
    
    #include "Common.h"                 // MGV common includes
    
    #include "SMSmSelectModeOfOperation.h"
    
    #define COMP_ID ID_SM               // base state component
    #define T_ID    _T("SmSelectModeOfOperation")
    
    
    IMPLEMENT_DYNCREATE(SMSmSelectModeOfOperation, CObject)// MFC Runtime class/object information

    
    DEFINE_TIMESTAMP
    
    //////////////////////////////////////////
    SMSmSelectModeOfOperation::SMSmSelectModeOfOperation()
    {
      IMPLEMENT_TIMESTAMP
    }