// Change history:
//
// POS14165 Work Request:13752 Name:Robert Susanto Date:May 23, 2011

#pragma once
#include <vector>
#include <map>

class CACSContext;

class CContexts
{
	friend class CAppModel;

public:
	CContexts(void);
	~CContexts(void);

   typedef std::pair<CString, CString> FieldProp;
   typedef std::vector<FieldProp> FieldPropList;
   typedef std::pair<FieldPropList::const_iterator, FieldPropList::const_iterator> FieldPropListRange;
   typedef std::map<CString, FieldPropList> MappedProps;

   typedef std::pair<CString, CString> ControlField;
   typedef std::pair<CString, ControlField> FieldControl;
   typedef std::vector<FieldControl> FieldControlList;
   typedef std::pair<FieldControlList::const_iterator, FieldControlList::const_iterator> FieldControlListRange;
   typedef std::map<CString, FieldControlList> MappedControls;

private:
	APPCONTEXTMAP m_contextMap;
	CCriticalSection m_cs;

   MappedProps mappedProps;
   MappedControls mappedControls;

   APPCONTEXT & GetAddContext(LPCTSTR pszContext);
	APPCONTEXTFIELD & GetAddContextField(LPCTSTR pszContext, LPCTSTR pszContextField);

public:
	bool IsContextApplicable(BSTR contextName);
	//POS14165 don't use the _bstr_t and _variant_t as the _variant_t will make a variantcopy of the ContextData that is sensitive to memory corruption
	//instead use the VARIANT pointer directly
	bool SaveContext(const BSTR contextName, const VARIANT& contextData);
	const PAPPCONTEXTFIELD GetContextField(LPCTSTR pszContext, LPCTSTR pszContextField);
	const PAPPCONTEXT GetContext(LPCTSTR pszContext);
	inline APPCONTEXTMAP & GetContextMap(void) { return m_contextMap; };
	bool ResetContext(LPCTSTR pszContext);
	bool ResetContexts(CSimpleArray<CString> & strContextNames);
	void ResetAllContexts(void);

   void AddMappedProperty(CString const& context, CString const& field, CString const& property)
   {
      MappedProps::iterator i = mappedProps.find(context);

      if (i != mappedProps.end())
      {
         i->second.push_back(std::make_pair(field, property));
      }
      else
      {
         FieldPropList propList;
         propList.push_back(std::make_pair(field, property));
         mappedProps.insert(std::make_pair(context, propList));
      }
   }
   
   FieldPropListRange GetMappedProperties(CString const& context)
   {
      MappedProps::const_iterator i = mappedProps.find(context);

      if (i != mappedProps.end())
      {
         return std::make_pair(i->second.begin(), i->second.end());
      }

      return std::make_pair(FieldPropList::const_iterator(), FieldPropList::const_iterator());
   }

   void AddMappedControl(CString const& context, CString const& field, CString const& control, CString const& controlField)
   {
      MappedControls::iterator i = mappedControls.find(context);

      if (i != mappedControls.end())
      {
         i->second.push_back(std::make_pair(field, std::make_pair(control, controlField)));
      }
      else
      {
         FieldControlList controlList;
         controlList.push_back(std::make_pair(field, std::make_pair(control, controlField)));
         mappedControls.insert(std::make_pair(context, controlList));
      }
   }
   
   FieldControlListRange GetMappedControls(CString const& context)
   {
      MappedControls::const_iterator i = mappedControls.find(context);

      if (i != mappedControls.end())
      {
         return std::make_pair(i->second.begin(), i->second.end());
      }

      return std::make_pair(FieldControlList::const_iterator(), FieldControlList::const_iterator());
   }
};
