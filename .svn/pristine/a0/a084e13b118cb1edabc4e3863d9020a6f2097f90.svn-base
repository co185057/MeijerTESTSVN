// PSConfigData.cpp: implementation of the CPSConfigData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "common.h"
#include "PSConfigData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPSConfigData::CPSConfigData()
{

}

CPSConfigData::~CPSConfigData()
{

}

void CPSConfigData::GetFormattedString(LPCTSTR sKey, LPCTSTR sValueName, CString&sValue, LPCTSTR sDefault, bool bPrimLang) const
   {
   GetString(sKey, sValueName, sValue, sDefault);
   FormatString(sValue, bPrimLang);
   }

void CPSConfigData::FormatString(CString&sValue, bool bPrimLang) const
   {

   if (!sValue.IsEmpty())
      {
      CString sTemp = sValue;
      CString sFormatted;
      int iFind;

      do
         {
         iFind = sTemp.Find(_T("%"));
         if (iFind != -1)
            {
            // Found a % sign...
            CString sNum = sTemp.Mid(iFind + 1).SpanIncluding(_T("0123456789"));

            if (iFind)
               {
               // It's not the next char, so insert all the stuff to the left of the %.
               sFormatted += sTemp.Left(iFind);
               sTemp = sTemp.Mid(iFind);
               }

            if (sNum.IsEmpty())
               {
               // If it's a % with some non-numeric char following it, 
               // then add the char after it and take out the %.
               sFormatted += sTemp.Mid(1,1);
               sTemp = sTemp.Mid(2);
               }
            else
               {
               // Found a formatting number...
               DWORD dwNum = _tcstoul((LPCTSTR)sNum, NULL, 10);
               sTemp = sTemp.Mid(sNum.GetLength() + 1);
			   //TD 04/17/02 TAR 201912 Need to add SCOT_LANGUAGE_PRIMARY so
			   //that all RAP intervention screens are in Primary language
               if (bPrimLang)
                  {
                  sFormatted += ps.GetPSText(dwNum, SCOT_LANGUAGE_PRIMARY);
                  }
               else // TAR 268133 - Allow possibility of getting in selected language
                  {
                  sFormatted += ps.GetPSText(dwNum);
                  }
               }
            }
         else if (!sTemp.IsEmpty())
            {
            sFormatted += sTemp;
            }
         } while (iFind != -1);
      sValue = sFormatted;
      }
   return;
   }
