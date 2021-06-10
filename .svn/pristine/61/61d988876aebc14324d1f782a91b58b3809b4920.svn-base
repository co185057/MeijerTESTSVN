// Change history:
//
// POS14165 Work Request:13752 Name:Robert Susanto Date:May 23, 2011

#pragma once
#include <vector>
#include <map>

class CACSContext;

class CProperties
{
protected:
   static LPCTSTR XML_PROPREF_START;
   static LPCTSTR XML_PROPREF_END;
   static const long XML_PROPREF_START_LEN;
   static const long XML_PROPREF_END_LEN;

public:
   typedef std::pair<CString, CString> ControlField;
   typedef std::vector<ControlField> ControlFieldList;
   typedef std::pair<ControlFieldList::const_iterator, ControlFieldList::const_iterator> ControlFieldListRange;
   typedef std::map<CString, ControlFieldList> MappedControls;

protected:
   PROPERTYMAP m_map;
   CCriticalSection m_cs;

   MappedControls mappedControls;

public:
   CProperties();
   virtual ~CProperties();

   CString Get(LPCTSTR szNameTag);
   bool Set(LPCTSTR pszName, LPCTSTR pszValue, LPCTSTR pszMapContext = NULL, LPCTSTR pszMapContextField = NULL);
   CString CProperties::DereferenceProperty(const CString & strPropVal);
	//POS14165 don't use the _bstr_t and _variant_t as the _variant_t will make a variantcopy of the ContextData that is sensitive to memory corruption
	//instead use the VARIANT pointer directly
   void UpdateContextBoundProperties(const BSTR contextName, const VARIANT& contextData);

   void AddMappedControl(CString const& property, CString const& control, CString const& field)
   {
      MappedControls::iterator i = mappedControls.find(property);

      if (i != mappedControls.end())
      {
         i->second.push_back(std::make_pair(control, field));
      }
      else
      {
         ControlFieldList contList;
         contList.push_back(std::make_pair(control, field));
         mappedControls.insert(std::make_pair(property, contList));
      }
   }
   
   ControlFieldListRange GetMappedControls(CString const& property)
   {
      MappedControls::const_iterator i = mappedControls.find(property);

      if (i != mappedControls.end())
      {
         return std::make_pair(i->second.begin(), i->second.end());
      }

      return std::make_pair(ControlFieldList::const_iterator(), ControlFieldList::const_iterator());
   }
};
