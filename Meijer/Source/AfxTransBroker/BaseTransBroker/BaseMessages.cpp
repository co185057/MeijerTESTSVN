//////////////////////////////////////////////////////////////////////
//
// CustomerMHMessages.cpp: implementation of the CBaseMessages class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMessages.h"
#include "CustomerMHOptions.h"
#include "CustomerTrxBroker.h"
#include "CustomerCurrentTransaction.h"
#include "customertbdefs.h"


#define T_ID    _T("CBaseMessages")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBaseMessages::CBaseMessages()
{
    m_pSCOTStrTableList[SCOTMSGS] = NULL;
    m_pSCOTStrTableList[SCUDMSGS] = NULL;
    m_pSCOTStrTableList[SCUDMSGS2] = NULL;
    //+RFQ 3267 and 3265
    m_pSCOTStrTableList[SCUDMSGS3] = NULL;
    m_pSCOTStrTableList[SCUDMSGS4] = NULL;

    m_bMultipleSecondaryLanguageSupport = CCustomerMHOptions::instance()->BoolOptionValue( _T("MultipleTBSecondaryLanguageSupport") );
    //-RFQ 3267 and 3265
    ReadInSCOTMessageFiles();
}

CBaseMessages::~CBaseMessages()
{
    m_pSCOTStrTableList[SCOTMSGS]->RemoveAll();
    if (m_pSCOTStrTableList[SCOTMSGS])
        delete m_pSCOTStrTableList[SCOTMSGS];
    
    m_pSCOTStrTableList[SCUDMSGS]->RemoveAll();
    if (m_pSCOTStrTableList[SCUDMSGS])
        delete m_pSCOTStrTableList[SCUDMSGS];
    
    m_pSCOTStrTableList[SCUDMSGS2]->RemoveAll();
    if (m_pSCOTStrTableList[SCUDMSGS2])
        delete m_pSCOTStrTableList[SCUDMSGS2];
    
    //+RFQ 3267 and 3265
    m_pSCOTStrTableList[SCUDMSGS3]->RemoveAll();
    if (m_pSCOTStrTableList[SCUDMSGS3])
        delete m_pSCOTStrTableList[SCUDMSGS3];

    m_pSCOTStrTableList[SCUDMSGS4]->RemoveAll();
    if (m_pSCOTStrTableList[SCUDMSGS4])
        delete m_pSCOTStrTableList[SCUDMSGS4];
    //-RFQ 3267 and 3265
    
}

/*************************************************************************
* ReadInSCOTMessageFiles - reads in FL msg files enabling shared us by TB 
*       - defined in TBBaseOpts.dat
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseMessages::ReadInSCOTMessageFiles(void)
{
    tbtraceIO(_T("ReadInSCOTMessageFiles"));

    // Read in Scot Messages file
    CString csFileName = CCustomerMHOptions::instance()->OptionValue(_T("SCOTMessageFile"));
    ReadInMainFiles(csFileName, SCOTMSGS);
    
    // Read in SCUD Messages file
    csFileName = CCustomerMHOptions::instance()->OptionValue(_T("SCUDMessageFile"));
    ReadInMainFiles(csFileName, SCUDMSGS);
    
    // Read in SCUD2 Messages file
    csFileName = CCustomerMHOptions::instance()->OptionValue(_T("SCUDMessage2File"));
    ReadInMainFiles(csFileName, SCUDMSGS2);

    //+RFQ 3267 and 3265
    // Read in SCUD3 Messages file
    csFileName = CCustomerMHOptions::instance()->OptionValue(_T("SCUDMessage3File"));
    ReadInMainFiles(csFileName, SCUDMSGS3);

    // Read in SCUD4 Messages file
    csFileName = CCustomerMHOptions::instance()->OptionValue(_T("SCUDMessage4File"));
    ReadInMainFiles(csFileName, SCUDMSGS4);
    //-RFQ 3267 and 3265
}

/*************************************************************************
* ReadInMainFiles - does the actual work reading in the files and saving
*       the data to a list.
*
* Parameters: 
*  CString csFileName - the name of the file to read in (full path info)
*  MsgFileType nMsgType - the enum value - used to differentiate between lists
*
* Returns: void
*************************************************************************/
void CBaseMessages::ReadInMainFiles(CString csFileName, MsgFileType nMsgType)
{
    tbtrace(TM_ENTRY, _T("+ReadInMainFiles(%s, %d)"), csFileName, nMsgType);
    CFileFind FileSearchObj;
    BOOL bRetStatus = TRUE;
    
    if (FileSearchObj.FindFile(csFileName))	// make sure file exists
    {
        if (m_pSCOTStrTableList[nMsgType])
        {
            m_pSCOTStrTableList[nMsgType]->RemoveAll();
            delete m_pSCOTStrTableList[nMsgType];
        }
        m_pSCOTStrTableList[nMsgType] = (COverrideSCOTStringList *) new CSCOTStringList(csFileName, nMsgType);
        ASSERT(m_pSCOTStrTableList[nMsgType]);
        bRetStatus = m_pSCOTStrTableList[nMsgType]->Initialize();
        
        if (bRetStatus)
        {
            int nSize = m_pSCOTStrTableList[nMsgType]->GetSize() - 1;	// 1 for oth element which is not used, SCOT indexing starts from 1
            tbtrace(TM_INFO, _T("TB built a list of %d strings from file :%s\n"), nSize, csFileName);
            //Loading the .000 and the .xxx files
            if (!ReadInSCOTMessageOverrideFiles(csFileName, nMsgType))
                tbtrace(TM_ERROR, _T("Error occurred reading in override files"));
        }
        FileSearchObj.Close();
    }
    else 
    {
        tbtrace(TM_ERROR, _T("ReadInSCOTMessageFiles - could not find files [%s]"), csFileName);
    }

    tbtrace(TM_ENTRY, _T("-ReadInMainFiles()"));
}

/*************************************************************************
* ReadInSCOTMessageOverrideFiles - after having read in the main option
*       file, this function attempts to load the override files (the
*       .000 and .xxx files).
*
* Parameters: 
*  CString csFileName - the name of the file to read in (full path info)
*  MsgFileType nMsgType - the enum value - used to differentiate between lists
*
* Returns: BOOL - TRUE=success; FALSE=failure
*************************************************************************/
BOOL CBaseMessages::ReadInSCOTMessageOverrideFiles(CString csFileName, MsgFileType nMsgType)
{
    tbtraceIO(_T("ReadInSCOTMessageOverrideFiles"));
    
    BOOL bRetStatus = TRUE;
    int iTerminalNumber = CCustomerMHOptions::instance()->GetTerminalNumber();

    // Don't override messages unless we have a terminal number.
    if (iTerminalNumber > 0)
    {
        CString csNewFileName, csExtension;
        csExtension = csFileName.Right(4);
        csNewFileName = csFileName.Left(csFileName.GetLength() - 4);
        
        // Replace the ".DAT" with ".000" and ".xxx"
        // Make sure we do have a ".DAT"
        if (csExtension.CompareNoCase(_T(".DAT")) == 0)
        {
            CString csTmpFileName;
            csTmpFileName.Format(_T("%s.%03d"),csNewFileName, (int)0);
            int tablesize = m_pSCOTStrTableList[nMsgType]->GetSize();
            //Call Refreshlist to override the data from the file copy from the server
            bRetStatus = m_pSCOTStrTableList[nMsgType]->RefreshList(csTmpFileName, nMsgType, tablesize);
            
            csTmpFileName.Format(_T("%s.%03d"),csNewFileName,iTerminalNumber);
            tablesize = m_pSCOTStrTableList[nMsgType]->GetSize();
            //Call Refreshlist to override the data from the file copy from the server
            bRetStatus = m_pSCOTStrTableList[nMsgType]->RefreshList(csTmpFileName, nMsgType, tablesize);
        }
    }
    else
    {
        tbtrace(TM_WARNING, _T("Cannot read in override files - no terminal number configured!"));
        bRetStatus = FALSE;
    }
    
    return bRetStatus;
}

/*************************************************************************
* GetStringValueFromID - given a scudefs.h string id, lookup and return the
*       string associated with it. 
*
* Parameters: 
*  int nStringID - the scudefs.h defined string ID
*
* Returns: CString - the string value, empty if ID not found
*************************************************************************/
CString CBaseMessages::GetStringValueFromID(int nStringID, bool bForceTranslate)
{
    tbtrace(TM_ENTRY, _T("+GetStringValueFromID(%d, %d)"), nStringID, bForceTranslate);
    CCustomerTrxBroker *pTB = CCustomerTrxBroker::instance();
    
    CString csValue = _T("");
    MsgFileType nMessageType = SCOTMSGS;    
    
    ASSERT(nStringID);
    
    if (nStringID > SCUD_BASE)
    {
        nStringID -= SCUD_BASE;
        
        // Figure out which SCUDMSG file to look in - right now TB only has the
        // ability to read from the SCUDMSGS and SCUDMSG2 files. If a Customer TB
        // needs to read from SCUDMSGS3, etc, this function will have to be
        // overridden and enhanced.
        
        // The language code passed in to TBStart is the ISO 639 language code. This
        // will be NULL (if multi-language not supported) or the 2-digit language code
        // If, in store mode or assist mode, then always use the primary language.
        if( !bForceTranslate && ((pTB->IsInStoreMode()) || (pTB->InAssistMode())) )
        {
            nMessageType = SCUDMSGS;
        }
        else  // customer mode - check on the selected language
        {
            CCustomerMHOptions *pOpts = CCustomerMHOptions::instance();
            CString csCurrLanguage = CCustomerCurrentTransaction::instance()->GetLanguageUsed();
            CString csPrimaryLanguage = pOpts->OptionValue(_T("ISO639PrimaryLanguageCode"));
            // if the current language is not empty (it will be when dual language is not
            // supported), and it does not equal the primary, then use SCUDMSG2
            if(!csCurrLanguage.IsEmpty() && (csCurrLanguage != csPrimaryLanguage))
            {
                //+RFQ 3267 and 3265
                if(m_bMultipleSecondaryLanguageSupport)
                {
                    CString csLangMapping;
                    CString csLangMappingValue;
                    //loop through the languages to find the mapping
                    //no need to start at 1 because that is the primary
                    //and this is not going to be the primary language
                    for(int i=2; i< MAXSUPPORTEDLIST; i++)
                    {
                        csLangMapping.Format(_T("SCUDMessage%dFileLanguage"), i);
                        csLangMappingValue = pOpts->OptionValue(csLangMapping);
                        csLangMappingValue.MakeUpper();
                        if(csLangMappingValue == csCurrLanguage)
                        {
                            nMessageType = (MsgFileType) i;
                            break;
                        }
        
                    }
                }else
                {
                    nMessageType = SCUDMSGS2;
                }
                //-RFQ 3267 and 3265
            }else
                nMessageType = SCUDMSGS;
        }
    }
    
    try
    {
        csValue = (*(m_pSCOTStrTableList[nMessageType]))[nStringID];
    }
    catch (...)
    {
       ASSERT(0);
       csValue = _T("");
       tbtrace(TM_ERROR, _T("Referring an array with invalid index: %d\n"),nStringID);
    }
    
    if (!csValue.GetLength())
    {
        tbtrace(TM_WARNING, _T("Missing string id %d"), nStringID);
    }
    
    tbtrace(TM_ENTRY, _T("-GetStringValueFromID()"));
    return csValue;
}
