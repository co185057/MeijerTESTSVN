#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/ContentHandler.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)ContentHandler.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
 *
 * Copyright 2003 by NCR Corporation,
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/ContentHandler.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     3/24/05 7:39p Dm185016
 * UNICODE
 * 
 * 2     2/14/05 7:10p Dm185016
 * Release Exclusive Access Event support
 * 
 * 1     1/26/05 2:33p Dm185016
 * Moved to new Repository.
* 
* 14    1/07/05 3:43p Dm185016
* Replaced char declarations with _TCHAR.
* 
* 13    11/09/04 3:25p Dm185016
* New macros for working with attributes.
* 
* 12    10/29/04 9:36a Dm185016
* Added regular expressions.
* 
* 11    10/05/04 8:32a Dm185016
* Trace object no longer shared.
* 
* 9     7/13/04 3:17p Dm185016
* Cleaned up.
* 
* 8     6/30/04 3:52p Dm185016
* Lint.
* Data Capture logging
* 
* 7     5/21/04 11:02a Dm185016
* lint
* 
* 6     4/30/04 1:00p Dm185016
* Added handler property for <include>
* 
* 5     3/19/04 3:34p Dm185016
* Memory leak fix; new definitions for RuleHandler split.
* 
* 4     1/27/04 1:47p Dm185016
* Cleanup of code.
* DLL Support
* 
* 3     1/16/04 9:33a Dm185016
* Added header
*/

//lint -emacro( {*}, SETUP_NULL_HANDLER )
//lint -emacro( {715}, DEFINE_HANDLER_START_ELEMENT )   Quit
//lint -emacro( {715}, DEFINE_HANDLER_END_ELEMENT )         Complaining
//lint -emacro( {715}, DEFINE_END_ELEMENT )                     About
//lint -emacro( {715}, DEFINE_START_ELEMENT )                       Parameters not referenced
//lint -emacro( {534}, DEFINE_HANDLER )                 Don't care about return value
#pragma warning( once : 4010 )
#include "saxcontenthandlerimpl.h"
#include <hash_map>
#include "xmlutilities.h"
#include "XMLContent.h"
#include "wstrutils.h"
#include "Loggable.h"
#include "LoggableManager.h"
#include "StringComparator.h"
#include "Singleton.h"
#include "utils.h"

#pragma warning( disable : 4251 )
#pragma warning( push )

using namespace std;

//-----------------------------------------------------------------------
// Typedef for the data structure used to hold Xpath info.
typedef StringType XPath;


//-----------------------------------------------------------------------
// Forward declaration for the following typedefs.
class CContentHandler;

//-----------------------------------------------------------------------
// Names for all parameters of the ISAXContentHandler Interface
#define CH_CHARACTERS           pwchChars
#define CH_CHARACTERS_LEN       cchChars
#define CH_NAMESPACE_URI        pwchNamespaceUri
#define CH_NAMESPACE_URI_LEN    cchNamespaceUri
#define CH_LOCAL_NAME           pwchLocalName
#define CH_LOCAL_NAME_LEN       cchLocalName
#define CH_QUALIFIED_NAME       pwchQName
#define CH_QUALIFIED_NAME_LEN   cchQName
#define CH_PATTRIBUTES          pAttributes

//-----------------------------------------------------------------------
// Macros for accessing the given string value
#define GET_CHARACTERS()\
    XMLUtilities::getString(CH_CHARACTERS, CH_CHARACTERS_LEN);
#define GET_NAMESPACE_URI()\
    XMLUtilities::getString(CH_NAMESPACE_URI, CH_NAMESPACE_URI_LEN);
#define GET_LOCAL_NAME()\
    XMLUtilities::getString(CH_LOCAL_NAME, CH_LOCAL_NAME_LEN);
#define GET_QUALIFIED_NAME()\
    XMLUtilities::getString(CH_QUALIFIED_NAME, CH_QUALIFIED_NAME_LEN);


//-----------------------------------------------------------------------
#define DECLARE_START_DOCUMENT() \
    virtual HRESULT STDMETHODCALLTYPE startDocument( void);
    
#define DECLARE_END_DOCUMENT() \
    virtual HRESULT STDMETHODCALLTYPE endDocument( void);

#define DECLARE_HANDLER_OWNER(ID, OWNER)\
    public:\
        C##ID():m_pOwner(NULL){}\
        ~C##ID(){m_pOwner=NULL;}\
        friend class OWNER;\
    private:\
        OWNER *m_pOwner;\

//-----------------------------------------------------------------------
// Macro used to declare the given handler class.  This handler expects
//      no attributes and only needs to define the characters() method
//      from the ISAXContentHandler interface.-
// @param ID The Class name of the handler minus the initial 'C'
//-----------------------------------------------------------------------
#define DECLARE_HANDLER(ID) \
    DECLARE_HANDLER_BEGIN(ID) \
    DECLARE_HANDLER_CHARACTERS() \
    DECLARE_HANDLER_END(ID)

#define DECLARE_HANDLER_WITH_OWNER(ID, OWNER) \
    DECLARE_HANDLER_BEGIN(ID) \
    DECLARE_HANDLER_CHARACTERS() \
    DECLARE_HANDLER_OWNER(ID, OWNER)\
    DECLARE_HANDLER_END(ID)


//-----------------------------------------------------------------------
// Macro used to begin the Class definition for a complex handler that
//      needs to override more than just the characters() method from 
//      the ISAXContentHandler interface.
// @param ID The Class name of the handler minus the initial 'C'
//-----------------------------------------------------------------------
#define DECLARE_HANDLER_BEGIN(ID) \
    class C##ID : public CContentHandler\
    {\
    public:

//-----------------------------------------------------------------------
// Macro used to declare the startElement() method from 
//      the ISAXContentHandler interface.  Used for base handlers only.
//-----------------------------------------------------------------------
#define DECLARE_START_ELEMENT() \
    virtual HRESULT __stdcall startElement (\
        /*[in]*/ wchar_t __RPC_FAR * CH_NAMESPACE_URI,\
        /*[in]*/ int CH_NAMESPACE_URI_LEN,\
        /*[in]*/ wchar_t __RPC_FAR * CH_LOCAL_NAME,\
        /*[in]*/ int CH_LOCAL_NAME_LEN,\
        /*[in]*/ wchar_t __RPC_FAR * CH_QUALIFIED_NAME,\
        /*[in]*/ int CH_QUALIFIED_NAME_LEN,\
        /*[in]*/ struct ISAXAttributes * pAttributes );

//-----------------------------------------------------------------------
// Macro used to declare the startElement() method from 
//      the ISAXContentHandler interface.  Used for enclosed handlers only.
//-----------------------------------------------------------------------
#define DECLARE_HANDLER_START_ELEMENT() \
    public:\
        DECLARE_START_ELEMENT()
    
//-----------------------------------------------------------------------
// Macro used to declare the endElement() method from 
//      the ISAXContentHandler interface.  Used for base handlers only.
//-----------------------------------------------------------------------
#define DECLARE_END_ELEMENT() \
    virtual HRESULT __stdcall endElement (\
        /*[in]*/ wchar_t __RPC_FAR * CH_NAMESPACE_URI,\
        /*[in]*/ int CH_NAMESPACE_URI_LEN,\
        /*[in]*/ wchar_t __RPC_FAR * CH_LOCAL_NAME,\
        /*[in]*/ int CH_LOCAL_NAME_LEN,\
        /*[in]*/ wchar_t __RPC_FAR * CH_QUALIFIED_NAME,\
        /*[in]*/ int CH_QUALIFIED_NAME_LEN );

//-----------------------------------------------------------------------
// Macro used to declare the startElement() method from 
//      the ISAXContentHandler interface.  Used for enclosed handlers only.
//-----------------------------------------------------------------------
#define DECLARE_HANDLER_END_ELEMENT() \
    public:\
        DECLARE_END_ELEMENT()

//-----------------------------------------------------------------------
// Macro used to declare the characters() method from 
//      the ISAXContentHandler interface.  Used for base handlers only.
//-----------------------------------------------------------------------
#define DECLARE_CHARACTERS() \
        virtual HRESULT STDMETHODCALLTYPE characters(\
            /* [in] */ wchar_t __RPC_FAR *CH_CHARACTERS,\
            /* [in] */ int CH_CHARACTERS_LEN);

//-----------------------------------------------------------------------
// Macro used to declare the startElement() method from 
//      the ISAXContentHandler interface.  Used for enclosed handlers only.
//-----------------------------------------------------------------------
#define DECLARE_HANDLER_CHARACTERS() \
    DECLARE_CHARACTERS()

#define DECLARE_HANDLER_PROPERTY(PROP_TYPE, PROP_NAME)\
	private:\
		PROP_TYPE PROP_NAME;

//-----------------------------------------------------------------------
// Macro used to terminate the Class definition for a complex handler that
//      needed to override more than just the characters() method from 
//      the ISAXContentHandler interface.
// @param ID The Class name of the handler minus the initial 'C'
//-----------------------------------------------------------------------
#define DECLARE_HANDLER_END(ID) \
    };\
    void ID(const StringType &);\
public:\
    friend class C##ID;\
private:\
    C##ID hndlr##ID;


#define DEFINE_START_DOCUMENT(BASE) \
    HRESULT STDMETHODCALLTYPE BASE::startDocument( void)
    
#define DEFINE_END_DOCUMENT(BASE) \
    HRESULT STDMETHODCALLTYPE BASE::endDocument( void)

//-----------------------------------------------------------------------
// Macro used to help hide the details of the ISAXContentHandler interface.
// @param BASE The name of the enclosing class for this handler.
// @param ID The Class name of the handler minus the initial 'C'
//-----------------------------------------------------------------------
#define DEFINE_CHARACTERS(BASE, ID) \
    HRESULT STDMETHODCALLTYPE BASE::C##ID::characters (\
        /* [in] */ wchar_t __RPC_FAR *CH_CHARACTERS,\
        /* [in] */ int CH_CHARACTERS_LEN)

//-----------------------------------------------------------------------
// Macro used to generate the implementation of the characters() method
//      from the ISAXContentHandler interface.
// @param BASE The name of the enclosing class for this handler.
// @param ID The Class name of the handler minus the initial 'C'
//-----------------------------------------------------------------------
#define DEFINE_HANDLER(BASE, ID, INSTANCE) \
    DEFINE_CHARACTERS(BASE, ID)\
    {\
        /*\
        IMODTRACE_DEVELOP();\
        */\
        StringType szValue = GET_CHARACTERS();\
        /*\
        ITRACE_DEVELOP(_T("szValue:  ") + szValue);\
        */\
        /*\
        BASE::instance()->INSTANCE().ID.Convert(szValue);\
        */\
        BASE::instance()->INSTANCE().Save##ID(szValue);\
        return S_OK;\
    }
#define DEFINE_HANDLER_LOCAL(BASE, ID, INSTANCE) \
    DEFINE_CHARACTERS(BASE, ID)\
    {\
        StringType szValue = GET_CHARACTERS();\
        (*m_pOwner).INSTANCE().Save##ID(szValue);\
        return S_OK;\
    }

//-----------------------------------------------------------------------
// Macro used to generate the implementation of the characters() method
//      from the ISAXContentHandler interface.
// @param BASE The name of the enclosing class for this handler.
// @param ID The Class name of the handler minus the initial 'C'
// @param INSTANCE The name of the method that returns the content
//      object associated with this handler
// @param OBJECT The Class name of the object to be modified minus the initial 'C'
//-----------------------------------------------------------------------
#define DEFINE_MULTI_HANDLER(BASE, ID, INSTANCE, OBJECT) \
    DEFINE_CHARACTERS(BASE, ID)\
    {\
        /*\
        IMODTRACE_DEVELOP();\
        */\
        std::wstring szValue = GET_CHARACTERS();\
        /*\
        ITRACE_DEVELOP(L"szValue:  " + szValue);\
        */\
        C##OBJECT &object = *BASE::instance()->INSTANCE().Current##OBJECT();\
        object.ID.Convert(szValue);\
        return S_OK;\
    }

//-----------------------------------------------------------------------
// Macro used to define a complete definition of the startElement() method from 
//      the ISAXContentHandler interface.  The body of the defined function
//      allocates a new data structure to be used by the handler.
// @param BASE The name of the enclosing class for this handler.
// @param OBJECT The class of the object minus the starting 'C'.
//-----------------------------------------------------------------------
#define DEFINE_START_ELEMENT_BASIC(BASE, OBJECT) \
    DEFINE_START_ELEMENT(BASE) \
    {\
        m_pContent = new C##OBJECT;\
        m_pContent->SetTraceObject(GetTraceHandle());\
        return S_OK;\
    }

//-----------------------------------------------------------------------
// Macro used to start the definition of the startElement() method from 
//      the ISAXContentHandler interface.  This version is used for base
//      handlers only, ie, those that may enclose other handler.
//      This is the macro corresponding
//      to the DECLARE_HANDLER_START_ELEMENT macro above.  It must be 
//      followed by a function body { return S_OK; } at a minimum.
//-----------------------------------------------------------------------
#define DEFINE_START_ELEMENT(BASE) \
    HRESULT __stdcall BASE::startElement (\
        /*[in]*/ wchar_t __RPC_FAR * CH_NAMESPACE_URI,\
        /*[in]*/ int CH_NAMESPACE_URI_LEN,\
        /*[in]*/ wchar_t __RPC_FAR * CH_LOCAL_NAME,\
        /*[in]*/ int CH_LOCAL_NAME_LEN,\
        /*[in]*/ wchar_t __RPC_FAR * CH_QUALIFIED_NAME,\
        /*[in]*/ int CH_QUALIFIED_NAME_LEN,\
        /*[in]*/ struct ISAXAttributes * pAttributes )

//-----------------------------------------------------------------------
// Macro used to start the definition of the the startElement() method from 
//      the ISAXContentHandler interface.  This version is used for internal
//      handlers only, ie, those that are enclosed within a base handler.
//      This is the macro corresponding
//      to the DECLARE_HANDLER_START_ELEMENT macro above.  It must be 
//      followed by a function body { return S_OK; } at a minimum.
//-----------------------------------------------------------------------
#define DEFINE_HANDLER_START_ELEMENT(BASE, ID) \
    HRESULT __stdcall BASE::C##ID::startElement (\
        /*[in]*/ wchar_t __RPC_FAR * CH_NAMESPACE_URI,\
        /*[in]*/ int CH_NAMESPACE_URI_LEN,\
        /*[in]*/ wchar_t __RPC_FAR * CH_LOCAL_NAME,\
        /*[in]*/ int CH_LOCAL_NAME_LEN,\
        /*[in]*/ wchar_t __RPC_FAR * CH_QUALIFIED_NAME,\
        /*[in]*/ int CH_QUALIFIED_NAME_LEN,\
        /*[in]*/ struct ISAXAttributes * pAttributes )

//-----------------------------------------------------------------------
// Macro used to start the definition of the endElement() method from 
//      the ISAXContentHandler interface.  This version is used for base
//      handlers only, ie, those that may enclose other handler.
//      This is the macro corresponding
//      to the DECLARE_HANDLER_END_ELEMENT macro above.  It must be 
//      followed by a function body { return S_OK; } at a minimum.
//-----------------------------------------------------------------------
#define DEFINE_END_ELEMENT(BASE)\
    HRESULT __stdcall BASE::endElement (\
    /*[in]*/ wchar_t __RPC_FAR * CH_NAMESPACE_URI,\
    /*[in]*/ int CH_NAMESPACE_URI_LEN,\
    /*[in]*/ wchar_t __RPC_FAR * CH_LOCAL_NAME,\
    /*[in]*/ int CH_LOCAL_NAME_LEN,\
    /*[in]*/ wchar_t __RPC_FAR * CH_QUALIFIED_NAME,\
    /*[in]*/ int CH_QUALIFIED_NAME_LEN)

//-----------------------------------------------------------------------
// Macro used to start the definition of the endElement() method from 
//      the ISAXContentHandler interface.  This version is used for base
//      handlers only, ie, those that may enclose other handler.
//      This is the macro corresponding
//      to the DECLARE_HANDLER_END_ELEMENT macro above.  It must be 
//      followed by a function body { return S_OK; } at a minimum.
//-----------------------------------------------------------------------
#define DEFINE_HANDLER_END_ELEMENT(BASE, ID)\
    HRESULT __stdcall BASE::C##ID::endElement (\
    /*[in]*/ wchar_t __RPC_FAR * CH_NAMESPACE_URI,\
    /*[in]*/ int CH_NAMESPACE_URI_LEN,\
    /*[in]*/ wchar_t __RPC_FAR * CH_LOCAL_NAME,\
    /*[in]*/ int CH_LOCAL_NAME_LEN,\
    /*[in]*/ wchar_t __RPC_FAR * CH_QUALIFIED_NAME,\
    /*[in]*/ int CH_QUALIFIED_NAME_LEN)

#define GET_ATTRIBUTE(TEXT) \
    XMLUtilities::getAttribute(\
    (const wchar_t *)CH_NAMESPACE_URI,\
    CH_NAMESPACE_URI_LEN,\
    (const wchar_t *)TEXT,\
    (int)wcslen(TEXT),\
    pAttributes);

//-----------------------------------------------------------------------
// Macro used to obtain and save the value of a given attribute,
// @param BASE The name of the enclosing class for this handler.
// @param ATTR The name of the attribute to be saved.  There must be
//      a method with the same name as the attribute that accepts a 
//      std::string parameter containing the value.
// @param TEXT The XML document name of the attribute to be saved.
//-----------------------------------------------------------------------
#define SAVE_ATTRIBUTE(BASE, ATTR, TEXT, INSTANCE) \
    {\
        StringType attrval = GET_ATTRIBUTE(TEXT);\
        if (attrval.size() > 0)\
        {\
            BASE::instance()->INSTANCE().ATTR.Convert(attrval);\
            /*\
            ITRACE_DEVELOP(L" Attribute value:  " + attrval);\
            */\
        }\
    }
#define SAVE_ATTRIBUTE_HANDLER(BASE, ATTR, TEXT)\
    {\
        StringType attrval = GET_ATTRIBUTE(TEXT);\
        if (attrval.size() > 0)\
        {\
            BASE::content_type pTable = (BASE::content_type)BASE::instance()->m_pContent;\
            pTable->ATTR.Convert(attrval);\
        }\
    }

//-----------------------------------------------------------------------
// Macro used to obtain and save the value of a given attribute,
// @param BASE The name of the enclosing class for this handler.
// @param ATTR The name of the attribute to be saved.  There must be
//      a method with the same name as the attribute that accepts a 
//      std::string parameter containing the value.
// @param TEXT The XML document name of the attribute to be saved.
//-----------------------------------------------------------------------
#define SAVE_MULTI_ATTRIBUTE(BASE, ATTR, TEXT, INSTANCE, OBJECT) \
    {\
        std::wstring attrval = XMLUtilities::getAttribute(\
                                (const wchar_t *)CH_NAMESPACE_URI,\
                                CH_NAMESPACE_URI_LEN,\
                                (const wchar_t *)TEXT,\
                                (int)wcslen(TEXT),\
                                pAttributes);\
        if (attrval.size() > 0)\
        {\
            C##OBJECT &object = *BASE::instance()->INSTANCE().Current##OBJECT();\
            object.ATTR.Convert(attrval);\
            /*\
            ITRACE_DEVELOP(L" Attribute value:  " + attrval);\
            */\
        }\
    }

#define CALL_LINKED_START_ELEMENT(LINK) \
    LINK::instance()->startElement (\
        CH_NAMESPACE_URI,\
        CH_NAMESPACE_URI_LEN,\
        CH_LOCAL_NAME,\
        CH_LOCAL_NAME_LEN,\
        CH_QUALIFIED_NAME,\
        CH_QUALIFIED_NAME_LEN,\
        pAttributes )

//------------------------------------------------------------------------------------
// Typedefs for managing the hash_map of ContentHandler pointers
//
typedef CContentHandler *PHANDLER;

//typedef std::hash_map <std::string, CContentNode<PHANDLER> * > CHMap;
//typedef std::map <std::string, CContentNode<PHANDLER> * > CHMap;
//typedef std::pair <std::string, CContentNode<PHANDLER> *> CHPair;
typedef std::map <StringType, PHANDLER, StringComparator > CHMap;
typedef void (*PRELEASE_FXN)(void);
typedef std::vector <PRELEASE_FXN> CHVector;


//-----------------------------------------------------------------------
// Macro used to register a handler
// @param ID The Xpath expression associated with this handler
// @param HND The name of class of the handler (minus the initial 'C')
//-----------------------------------------------------------------------
//lint -emacro( {534}, SETUP_HANDLER ) Ignoring return value from insert
#define SETUP_HANDLER(ID, HND) \
{\
    if (xpathLoggable().IsDebug()) ITRACE_DEVELOP(_T("Adding Handler for:  ") + IString(ID));\
    if (containsHandler(ID))\
    {\
        ITRACE_INFORMATIONAL(_T("Replacing handler already registered:  ")ID);\
        handlers.erase(ID);\
    }\
    handlers.insert(std::make_pair(ID, CSingleton<C##HND>::instance()));\
    CSingleton<C##HND>::instance()->SetTraceObject(GetTraceHandle());\
    CSingleton<C##HND>::instance()->registerHandlers();\
    m_vSingletonHandlers.push_back(&CSingleton<C##HND>::Release);\
}


//-----------------------------------------------------------------------
// Macro used to register a handler
// @param ID The Xpath expression associated with this handler
// @param HND The name of class of the handler (minus the initial 'C')
//-----------------------------------------------------------------------
//lint -emacro( {534}, SETUP_DECLARED_HANDLER ) Ignoring return value from insert
#define SETUP_DECLARED_HANDLER(ID, HND) \
{\
    if (xpathLoggable().IsDebug()) ITRACE_DEVELOP(_T("Adding Handler for:  ") + IString(ID));\
    if (containsHandler(ID))\
    {\
        ITRACE_INFORMATIONAL(_T("Replacing handler already registered:  ")ID);\
        handlers.erase(ID);\
    }\
    handlers.insert(std::make_pair(ID, &hndlr##HND));\
    hndlr##HND.SetTraceObject(GetTraceHandle());\
    hndlr##HND.registerHandlers();\
}


//-----------------------------------------------------------------------
// Macro used to register a handler
// @param ID The Xpath expression associated with this handler
// @param HND The name of class of the handler (minus the initial 'C')
//-----------------------------------------------------------------------
//lint -emacro( {534}, SETUP_DECLARED_HANDLER ) Ignoring return value from insert
#define SETUP_STATIC_HANDLER(ID, HNDADDR) \
{\
    if (xpathLoggable().IsDebug()) ITRACE_DEVELOP(_T("Adding Handler for:  ") + IString(ID));\
    if (containsHandler(ID))\
    {\
        ITRACE_INFORMATIONAL(_T("Replacing handler already registered:  ")ID);\
        handlers.erase(ID);\
    }\
    handlers.insert(std::make_pair(ID, HNDADDR));\
    hndlr##HND.SetTraceObject(GetTraceHandle());\
    hndlr##HND.registerHandlers();\
}


//-----------------------------------------------------------------------
// Macro used to register a null action handler with the given xpath expression
// @param ID The Xpath expression associated with this handler
// @param HND The name of class of the handler (minus the initial 'C')
//-----------------------------------------------------------------------
//lint -emacro( {534}, SETUP_NULL_HANDLER ) Ignoring return value from insert
#define SETUP_NULL_HANDLER(ID) \
{\
    if (xpathLoggable().IsDebug()) ITRACE_DEVELOP(_T("Adding Handler for:  ") + IString(ID));\
    if (containsHandler(ID))\
    {\
        ITRACE_INFORMATIONAL(_T("Replacing handler already registered:  ")ID);\
        handlers.erase(ID);\
    }\
    handlers.insert(std::make_pair(ID, &nullHandler));\
    nullHandler.SetTraceObject(GetTraceHandle());\
}


//-----------------------------------------------------------------------
// Macro used to register a null action handler with the given xpath expression
// @param ID The Xpath expression associated with this handler
// @param HND The name of class of the handler (minus the initial 'C')
//-----------------------------------------------------------------------
//lint -emacro( {534}, SETUP_NULL_HANDLER_USING_REGEX ) Ignoring return value from insert
#define SETUP_NULL_HANDLER_USING_REGEX(ID) \
{\
    if (xpathLoggable().IsDebug()) ITRACE_DEVELOP(_T("Adding Handler for:  ") + IString(ID));\
    if (containsRegex(ID))\
    {\
        ITRACE_INFORMATIONAL(_T("Replacing handler already registered:  ")ID);\
        regexs.erase(ID);\
    }\
    regexs.insert(std::make_pair(ID, &nullHandler));\
    nullHandler.SetTraceObject(GetTraceHandle());\
}



//-----------------------------------------------------------------------
typedef CHMap::iterator CHMapIterator;


// Need to ensure that these are defined appropriately
#ifndef _L
    #define __L(x) L##x
    #define _L(x) __L(x)
#endif

//-----------------------------------------------------------------------
// This class provides the needed methods for handling the parsing of
// XML documents from the Retalix POS Automation interface.  It extends
// the basic ISAXContentHandler interface provided and provides a lookup
// table that matches the XPath expression identifying the tag names 
// with a handler registered to process that tag.
class UTILS_API CContentHandler :
    public SAXContentHandlerImpl
{
public:
    CContentHandler(void);
    virtual ~CContentHandler(void);

    virtual void registerHandlers(void) {}
    virtual void unregisterHandlers(void) {}

    virtual CXMLContent *Content();

    virtual StringType DebugString() const;

    DECLARE_START_DOCUMENT()

    ILoggable &loggable() const;

    static CHMap &getHandlers();

    static void ReleaseHandlers();

protected:

    CContentHandler *getHandler(const _TCHAR *) const; 
    CContentHandler *getHandlerUsingRegex(const _TCHAR *) const; 
    
    bool containsHandler(const _TCHAR *) const; 
    bool containsRegex(const _TCHAR *) const; 

//-----------------------------------------------------------------------------
// Data members given protected access for use by derivatives
protected:
    static CContentHandler nullHandler;

    static CHMap handlers;
    static CHMap regexs;
    int maxHandlers;

    static CHVector m_vSingletonHandlers;

    static XPath xpath;

    CXMLContent *m_pContent;

private:

    CContentHandler(const CContentHandler &);               // Hide the Copy construcor
    CContentHandler &operator =(const CContentHandler &);   // Hide the assignment operator

    mutable ILoggable *m_pLoggable;
};

#pragma warning( pop )
