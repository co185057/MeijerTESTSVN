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
bool CNamedStringContainer::FromString(CString sData)
   {
   m_sContents = sData;
   return FromContents();
   }

CString CNamedStringContainer::ExtractString(CString sStringName) const
   {
   CString sString;
   bool bStatus = false;

   sStringName.MakeUpper();
   int iFind = m_sContents.Find(sStringName);

   if (iFind >= 0)
      {
      sString = m_sContents.Mid(iFind);
      iFind = sString.Find("=");
      ASSERT(iFind > 0);
      if (iFind > 0)
         {
         sString = sString.Mid(iFind + 1);
         iFind = sString.Find(";");
//         ASSERT(iFind > 0);
         if (iFind > 0)
            {
            sString = sString.Left(iFind);
            bStatus = true;
            }
         }
      if (!bStatus)
         sString.Empty();
      }
   return sString;
   }

bool CNamedStringContainer::InsertString(CString sStringName, CString sStringValue)
   {
   bool bStatus = false;

   if (!sStringName.IsEmpty() && !sStringValue.IsEmpty() &&
      sStringName.FindOneOf(";=") == -1 && sStringValue.Find(";") == -1)
      {
      sStringName.MakeUpper();
      m_sContents += sStringName;
      m_sContents += "=";
      m_sContents += sStringValue;
      m_sContents += ";";
      bStatus = true;
      }
   return bStatus;
   }