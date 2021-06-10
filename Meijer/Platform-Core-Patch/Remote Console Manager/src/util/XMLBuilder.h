#pragma once
#include "ixmlbuilder.h"
#include "xmlbuilder.h"
#include "property.h"
#include "Utils.h"

class CXMLContent;

template <typename T>
void AddElement(const CObjectProperty<T> &, CXMLBuilder *);

#pragma warning( push )
#pragma warning( disable : 4251 )

class UTILS_API CXMLBuilder
    : virtual public CBaseObject
{
public:
    CXMLBuilder(void);
    virtual ~CXMLBuilder(void);

    virtual StringType GetXMLOutput();
    virtual void GetXMLOutput(CXMLContent *);
    virtual void GetXMLOutput(CXMLContent *, CXMLBuilder *)=0;

//  friend template <class T> void AddElement(const CObjectProperty<T> &);

    virtual void StartDocument();
    virtual void EndDocument();
    virtual void StartElement(const StringType &);
    virtual void EndElement(const StringType &);
    virtual void ClearAttributes();
    virtual void Characters(const StringType &);

    virtual void AddAttribute(const StringType &, StringType &);
    virtual void AddAttribute(const StringType &, CStringProperty &);
    virtual void AddAttribute(const StringType &, CShortProperty &);
    virtual void AddAttribute(const StringType &, CLongProperty &);
    virtual void AddAttribute(const StringType &, CIntProperty &);
    virtual void AddAttribute(const StringType &, CUnsignedLongProperty &);
    virtual void AddAttribute(const StringType &, const IEnum &, const IValidatable &);

    virtual void AddElement(const StringType &, StringType &);
    virtual void AddElement(const StringType &, CBoolProperty &);
    virtual void AddElement(const StringType &, CIntProperty &);
    virtual void AddElement(const StringType &, CLongProperty &);
    virtual void AddElement(const StringType &, CUnsignedLongProperty &);
    virtual void AddElement(const StringType &, CShortProperty &);
    virtual void AddElement(const StringType &, CStringProperty &);
    virtual void AddElement(const StringType &, CFloatProperty &);
    virtual void AddElement(const StringType &, const IEnum &eValue, const IValidatable &);
//  virtual void AddElement(const std::wstring &, const CEnumProperty &);

protected:

    virtual void AddAdornments();

    CString m_szRootPath;

    inline IMXWriter &contentHandler() { return *pWriter; }

private:

	wchar_t *GetString(const std::string &) const;
	wchar_t *GetString(const std::wstring &) const;
    
	CString m_szXMLOutput;

    int attrcount;

    IMXWriter      *pWriter;
    IMXAttributes  *pMXAttrs;

    CComBSTR nullAttrString;
};


template <typename T>
inline void AddElement(  CXMLBuilder *pTheDestBuilder
                       , const CObjectProperty<T> &oValue
                       , CXMLBuilder *pXMLBuilder)
{
    IMODTRACE_DEVELOP();

    if (oValue.IsValid())
    {
        CXMLContent *pContent = (CXMLContent *)oValue.Value();
        (*pXMLBuilder).GetXMLOutput(pContent, pTheDestBuilder);
    }
}
#pragma warning( pop )
