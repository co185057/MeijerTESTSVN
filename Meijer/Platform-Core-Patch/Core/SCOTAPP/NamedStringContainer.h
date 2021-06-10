// NamedStringContainer.h: interface for the CNamedStringContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAMEDSTRINGCONTAINER_H__2190C3F7_E8C8_11D5_8D8F_00E029301EFC__INCLUDED_)
#define AFX_NAMEDSTRINGCONTAINER_H__2190C3F7_E8C8_11D5_8D8F_00E029301EFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _SCOTSSF_
class DLLIMPORT
#elif defined _SCOT
class DLLEXPORT
#else
class 
#endif 
CNamedStringContainer  
{
public:
	CNamedStringContainer();
	virtual ~CNamedStringContainer();
public:
   // Mutators.

   // Populate all fields from a string created with ToString();
   virtual bool FromString(CString sDataEntry);

   // Const accessor methods.
   virtual CString ToString() const;
  //RFQ 2605 - Make ExtractString and InsertString public for easier customization
   virtual CString ExtractString(CString sStringName) const;
   virtual bool InsertString(CString sStringName, CString sStringValue);

protected:

   virtual bool FromContents() = 0;
   CString m_sContents;
};

#endif // !defined(AFX_NAMEDSTRINGCONTAINER_H__2190C3F7_E8C8_11D5_8D8F_00E029301EFC__INCLUDED_)
