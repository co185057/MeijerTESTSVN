// NamedStringContainer.cpp: implementation of the CNamedStringContainer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NamedStringContainer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNamedStringContainer::CNamedStringContainer()
{

}

CNamedStringContainer::~CNamedStringContainer()
{

}

// Const accessor methods.
CString CNamedStringContainer::ToString() const
   {
   return m_sContents;
   }

// Populate and validate all fields from a string created with ToString();
// Make sure all key values are upper case
bool CNamedStringContainer::FromString(CString sData)
{
    bool bRetValue = true;
    int iLen = sData.GetLength();
    int iSemicolon;
    m_sContents.Empty();
    while (iLen > 0)
    {
        iSemicolon = FindContentsSeparator(sData);
        CString csPair = _T("");
        if (iSemicolon > 0 )
        {
            {
                csPair = sData.Left(iSemicolon);
                sData = sData.Mid(iSemicolon+1);
            }
        }
        else
        {
            csPair = sData;
            sData.Empty();
        }

        iLen = sData.GetLength();

        if (!csPair.IsEmpty())
        {   
            int iEqualSign = csPair.ReverseFind(_T('='));
            if (iEqualSign > 0)  //We have a pair
            {
                CString csKey = csPair.Mid(0, iEqualSign);
                csKey.TrimLeft();
                csKey.TrimRight();
                CString csValue = csPair.Mid(iEqualSign + 1);
                InsertString(csKey, csValue);
            }
            else
            {
                csPair.TrimRight();  //Might be end of datastring,  don't complain about space after semicolon.
                if (!csPair.IsEmpty())
                {
                    //indicate there is a problem with the string, but continue
                    bRetValue = false;
                }
            }
        }
    }
    
    return bRetValue && FromContents();
}

CString CNamedStringContainer::ExtractString(CString sStringName) const
   {
   CString csStringRemaining = m_sContents;
   CString csPair = _T("");
   CString csValue = _T("");

   int iFindSeparator = -1;
   int iFindEqual = -1;

   bool bStatus = false;
   bool bFoundKeyName = false;

   sStringName.MakeUpper();

   while (!csStringRemaining.IsEmpty() && !bFoundKeyName)
   {
       csPair = csStringRemaining;
       iFindSeparator = FindContentsSeparator(csPair);
       if (iFindSeparator > 0)
       {
           csPair = csPair.Mid(0, iFindSeparator);
           csStringRemaining = csStringRemaining.Mid(iFindSeparator+1);
       }
       else
       {
           csStringRemaining.Empty();
       }

       iFindEqual = csPair.Find(_T("="));
       if (iFindEqual >=0)
       {
           CString csKeyName = csPair.Left(iFindEqual);
           csKeyName.TrimLeft();
           csKeyName.TrimRight();
           if (sStringName.Compare(csKeyName) == 0)
           {
               bFoundKeyName = true;
               csValue = csPair.Mid(iFindEqual + 1);
           }
       }
   }

   //SSCOP-3583 Convert "/;" back to ";"
   csValue.Replace(_T("/;"), _T(";"));
   return csValue;
   }

bool CNamedStringContainer::InsertString(CString sStringName, CString sStringValue) 
   {
   bool bStatus = false;


   if (!sStringName.IsEmpty() && !sStringValue.IsEmpty())
      {
	    // SSCOP-3583 added '/' before the ';' to indicate that the ';' is part of the stringvalue
        sStringValue.Replace(_T("/;"), _T(";"));  //return all semicolons to original form to prevent double replace
		sStringValue.Replace(_T(";"), _T("/;"));
        sStringName.MakeUpper();

        CString csNewPair = sStringName + _T("=");
        int iPairPos = m_sContents.Find(csNewPair);
        csNewPair += sStringValue + _T(";");
        
        if (iPairPos < 0)
        {
            m_sContents += csNewPair;
        }
        else
        {
            CString csBeginning = m_sContents.Left(iPairPos);
            CString csEnd = m_sContents.Mid(iPairPos);
            int iEndOfPair = FindContentsSeparator(csEnd);
            if (iEndOfPair > 0)
            {
                csEnd = csEnd.Mid(iEndOfPair+1);
            }
            m_sContents = csBeginning + csNewPair + csEnd;
        }
        
        bStatus = true;
      }
   return bStatus;
   }

//SSCOP-3583 This method will return the zero-based index of the first separator in the StringContents object
int CNamedStringContainer::FindContentsSeparator(CString sStringContents) const
{
	int iFind = sStringContents.Find(_T(";"));
	int iTemp = iFind;

	while (iTemp>0 ? (sStringContents.GetAt(iTemp-1) == _T('/')):false)
	{
		sStringContents = sStringContents.Mid(iTemp + 1);
		iTemp = sStringContents.Find(_T(";"));
		iFind = iFind + (iTemp >= 0 ? iTemp + 1 : 0);
	}
	return iFind;
}
