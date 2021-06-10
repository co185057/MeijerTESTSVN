#pragma once

#include "DataKit.h"
#include <vector>
#include "tstring.h"
#include <vector>
#include <map>
#include <set>


class EqualsOp;
class ContainsOp;
class BeginsWithOp;
class EndsWithOp;
class RegExOp;
class EqualsPropertyOp;

class Comparison
{
   NCR::Util::tstring comp;

protected:
   NCR::Util::tstring const& CompValue() const
   {
      return comp;
   }

public:
   Comparison(NCR::Util::tstring const& compInit) : comp(compInit) {}
   Comparison(Comparison const& rhs) : comp(rhs.comp) {}
   Comparison& operator=(Comparison const& rhs)
   {
      comp = rhs.comp;
      return *this;
   }
   virtual bool Execute(LPCTSTR value) const = 0;
   virtual NCR::Util::tstring ToString() const = 0;
};


template <typename Op>
class ComparisonImpl : public Comparison
{
public:
   ComparisonImpl(NCR::Util::tstring compInit) : Comparison(compInit) {}
   ComparisonImpl(ComparisonImpl const& rhs) : Comparison(rhs) {}
   ComparisonImpl& operator=(ComparisonImpl const& rhs)
   {
      *(static_cast<Comparison*>(this)) = rhs;
      return *this;
   }
   virtual bool Execute(LPCTSTR value) const;
   virtual NCR::Util::tstring ToString() const;
};

typedef ComparisonImpl<EqualsOp> Equals;
typedef ComparisonImpl<ContainsOp> Contains;
typedef ComparisonImpl<BeginsWithOp> BeginsWith;
typedef ComparisonImpl<EndsWithOp> EndsWith;
typedef ComparisonImpl<RegExOp> RegEx;
typedef ComparisonImpl<EqualsPropertyOp> EqualsProperty;


struct DetectProperty
{
   NCR::Util::tstring name;
   mutable Comparison* oper;
   bool negate;

   DetectProperty() : negate(false), oper(0) {}

   DetectProperty(
      NCR::Util::tstring const& nameInit, 
      Comparison* operInit, 
      bool negateInit = false
      ) : 
         name(nameInit), 
         oper(operInit), 
         negate(negateInit)
   {
   }

   DetectProperty(
      DetectProperty const& rhs
      ) : 
         name(rhs.name), 
         oper(rhs.oper), 
         negate(rhs.negate)
   {
      rhs.oper = 0;
   }

   DetectProperty& operator=(DetectProperty const& rhs)
   {
      name = rhs.name;
      oper = rhs.oper;
      rhs.oper = 0;
      negate = rhs.negate;

      return *this;
   }

   ~DetectProperty()
   {
      delete oper;
   }
};

typedef std::vector<DetectProperty> DetectProperties;


struct DetectField
{
   NCR::Util::tstring name;
   mutable Comparison* oper;
   bool negate;

   DetectField() : negate(false), oper(0) {}

   DetectField(
      NCR::Util::tstring const& nameInit, 
      Comparison* operInit, 
      bool negateInit = false
      ) : 
         name(nameInit), 
         oper(operInit), 
         negate(negateInit)
   {
   }

   DetectField(
      DetectField const& rhs
      ) : 
         name(rhs.name), 
         oper(rhs.oper), 
         negate(rhs.negate)
   {
      rhs.oper = 0;
   }

   DetectField& operator=(DetectField const& rhs)
   {
      name = rhs.name;
      oper = rhs.oper;
      rhs.oper = 0;
      negate = rhs.negate;

      return *this;
   }

   ~DetectField()
   {
      delete oper;
   }
};

typedef std::vector<DetectField> DetectFields;


struct DetectContext
{
   NCR::Util::tstring name;
   DetectFields fields;

   DetectContext(NCR::Util::tstring const& nameInit) : name(nameInit) {}

   DetectContext(DetectContext const& rhs) : name(rhs.name), fields(rhs.fields) {}

   DetectContext& operator=(DetectContext const& rhs)
   {
      name = rhs.name;
      fields = rhs.fields;
      return *this;
   }
};

typedef std::vector<DetectContext> DetectContexts;


class Action
{
public:
   virtual bool Execute() const = 0;
   virtual NCR::Util::tstring ToString() const = 0;
};


class SetPropertyOp;
class ConcatLeftPropertyOp;
class ConcatRightPropertyOp;
class ResetContextOp;
class ResetControlOp;
class SendControlOp;
class SendControlToAppOp;
class SendInputOp;


template<typename Op>
class UnaryAction : public Action
{
   NCR::Util::tstring name;

public:
   UnaryAction(
      NCR::Util::tstring nameInit
      ) : name(nameInit) {}

   UnaryAction(UnaryAction const& rhs) : name(rhs.name) {}

   UnaryAction& operator=(UnaryAction const& rhs)
   {
      name = rhs.name;
      return *this;
   }

   virtual bool Execute() const;
   virtual NCR::Util::tstring ToString() const;
};


template<typename Op>
class BinaryAction : public Action
{
   NCR::Util::tstring name;
   NCR::Util::tstring value;

public:
   BinaryAction(
      NCR::Util::tstring nameInit, 
      NCR::Util::tstring valueInit
      ) : 
         name(nameInit), 
         value(valueInit) {}

   BinaryAction(BinaryAction const& rhs) : name(rhs.name), value(rhs.value) {}

   BinaryAction& operator=(BinaryAction const& rhs)
   {
      name = rhs.name;
      value = rhs.value;
      return *this;
   }

   virtual bool Execute() const;
   virtual NCR::Util::tstring ToString() const;
};


typedef BinaryAction<SetPropertyOp> SetProperty;
typedef BinaryAction<ConcatLeftPropertyOp> ConcatLeftProperty;
typedef BinaryAction<ConcatRightPropertyOp> ConcatRightProperty;

typedef UnaryAction<ResetContextOp> ResetContext;
typedef UnaryAction<ResetControlOp> ResetControl;
typedef UnaryAction<SendControlOp> SendControl;
typedef UnaryAction<SendControlToAppOp> SendControlToApp;
typedef UnaryAction<SendInputOp> SendInput;


typedef std::set<NCR::Util::tstring> ResetExclusions;

class ResetAllContexts : public Action
{
   ResetExclusions excl;

public:
   ResetAllContexts(ResetExclusions exclInit) : excl(exclInit) {}

   ResetAllContexts(ResetAllContexts const& rhs) : excl(rhs.excl) {}

   ResetAllContexts& operator=(ResetAllContexts const& rhs)
   {
      excl = rhs.excl;
      return *this;
   }

   virtual bool Execute() const;
   virtual NCR::Util::tstring ToString() const;
};


class Event
{
   mutable Action* action;

public:
   Event(Action* actionInit) : action(actionInit) {}

   /* Ownership of the Action member pointer is transferred 
   to the new object. Once an Event has been copied, that 
   Event is no longer usable. */
   Event(Event const& rhs) : action(rhs.action)
   {
      rhs.action = 0;
   }

   /* Ownership of the Action member pointer is transferred 
   to the new object. Once an Event has been copied, that 
   Event is no longer usable. */
   Event& operator=(Event const& rhs) 
   {
      action = rhs.action;
      rhs.action = 0;
      return *this; 
   }

   ~Event()
   {
      delete action;
   }

   bool Execute() const
   {
      /* Make sure we're not calling an action on an Event that 
      has transferred ownership of its action pointer. */
      _ASSERTE(action);
      return (action && action->Execute());
   }

   NCR::Util::tstring ToString() const
   {
      return action ? action->ToString() : _T("<!-- UNDEFINED -->");
   }
};

typedef std::vector<Event> Events;
typedef std::vector<NCR::Util::tstring> IgnoreContexts;


class State
{
public:
   NCR::Util::tstring name;
   NCR::Util::tstring group;
   bool ignoreDuplicate;
   unsigned int triggerDelay;
   DetectProperties properties;
   DetectContexts contexts;
   IgnoreContexts ignore;
   Events onEnter;
   Events onExit;
   _bstr_t groupBstr;
   _bstr_t ignoreDuplicateBstr;
   _bstr_t triggerDelayBstr;

   State() : ignoreDuplicate(false), triggerDelay(0)
   {
   }

   State(State const& rhs) : 
      name(rhs.name),
      group(rhs.group),
      ignoreDuplicate(rhs.ignoreDuplicate),
      triggerDelay(rhs.triggerDelay),
      properties(rhs.properties),
      contexts(rhs.contexts),
      ignore(rhs.ignore),
      onEnter(rhs.onEnter),
      onExit(rhs.onExit),
      groupBstr(rhs.groupBstr),
      ignoreDuplicateBstr(rhs.ignoreDuplicateBstr),
      triggerDelayBstr(rhs.triggerDelayBstr)
   {
   }

   State& operator=(State const& rhs) 
   {
      name = rhs.name;
      group = rhs.group;
      ignoreDuplicate = rhs.ignoreDuplicate;
      triggerDelay = rhs.triggerDelay;
      properties = rhs.properties;
      contexts = rhs.contexts;
      ignore = rhs.ignore;
      onEnter = rhs.onEnter;
      onExit = rhs.onExit;
      groupBstr = rhs.groupBstr;
      ignoreDuplicateBstr = rhs.ignoreDuplicateBstr;
      triggerDelayBstr = rhs.triggerDelayBstr;
      return *this;
   }

   bool operator==(State const& rhs) const
   {
      return name == rhs.name;
   }

   bool operator!=(State const& rhs) const
   {
      return !operator==(rhs);
   }

   bool OnEnter() const
   {
      bool eventResult = true;

      for (Events::const_iterator it = onEnter.begin(); it != onEnter.end(); ++it)
      {
         _ASSERTE(eventResult);
         eventResult = eventResult && it->Execute();
      }

      return eventResult;
   }

   bool OnExit() const
   {
      bool eventResult = true;

      for (Events::const_iterator it = onExit.begin(); it != onExit.end(); ++it)
      {
         _ASSERTE(eventResult);
         eventResult = eventResult && it->Execute();
      }

      return eventResult;
   }

   LPCTSTR Name() const
   {
      return name.c_str();
   }

/* This is used for comparison purposes as a sentinel indicating no state was found. */
   static State nullState;
};

typedef std::vector<State> States;
typedef std::map<NCR::Util::tstring, States::size_type> StateMap;
typedef std::map<NCR::Util::tstring, Events> EventMap;

typedef std::vector<std::pair<NCR::Util::tstring, NCR::Util::tstring> > SequenceFields;
typedef std::map<NCR::Util::tstring, SequenceFields> SequenceMap;

typedef std::map<NCR::Util::tstring, NCR::Util::tstring> InputTextMap;


class CProperties;
class CControls;

class CAppModel
{
public:
   CAppModel();
   virtual ~CAppModel();

private:
   States states;
   StateMap stateMap;
   EventMap contextEvents;

   MSXML2::IXMLDOMDocumentPtr m_spModelDoc;
   CString m_strModelFile;

   typedef std::vector<NCR::Util::tstring> IgnoreContexts;
   IgnoreContexts ignoreContexts;

   SequenceMap sequenceMap;
   InputTextMap inputTextMap;

   // static defines
   static const _bstr_t XML_STATE;
   static const _bstr_t XML_STATE_ATTR_NAME;
   static const _bstr_t XML_STATE_IGNORE_QUERY;
   static const _bstr_t XML_STATE_IGNORE_CONTEXT_QUERY;
   static const _bstr_t XML_STATE_IGNORE_CONTEXT_ATTR_NAME;
   static const _bstr_t XML_STATE_DETECT_CONTEXT_QUERY;
   static const _bstr_t XML_STATE_DETECT_PROPERTY_QUERY;
   static const _bstr_t XML_STATE_DETECT_PROPERTY_ATTR_NAME;
   static const _bstr_t XML_STATE_DETECT_CONTEXT_FIELD_QUERY_ALL;
   static const _bstr_t XML_STATE_DETECT_CONTEXT_QUERY_ALL;
   static const _bstr_t XML_STATE_DETECT_CONTEXT_ATTR_NAME;
   static const _bstr_t XML_STATE_DETECT_CONTEXT_FIELD;
   static const _bstr_t XML_STATE_DETECT_CONTEXT_FIELD_ATTR_NAME;
   static const _bstr_t XML_STATE_DETECT_CONTEXT_FIELD_ATTR_CONTAINS;
   static const _bstr_t XML_STATE_DETECT_CONTEXT_FIELD_ATTR_EQUALS;
   static const _bstr_t XML_STATE_DETECT_CONTEXT_FIELD_ATTR_BEGINSWITH;
   static const _bstr_t XML_STATE_DETECT_CONTEXT_FIELD_ATTR_ENDSWITH;
   static const _bstr_t XML_STATE_DETECT_CONTEXT_FIELD_ATTR_REGEX;
   static const _bstr_t XML_STATE_DETECT_CONTEXT_FIELD_ATTR_EQUALS_PROPERTY;
   static const _bstr_t XML_STATE_DETECT_CONTEXT_FIELD_ATTR_NEGATE;
   static const _bstr_t XML_STATE_GROUP_QUERY;

   static const _bstr_t XML_STATE_DETECT;
   static const _bstr_t XML_STATE_DETECT_CONTEXT;
   static const _bstr_t XML_STATE_DETECT_PROPERTY;

   static const _bstr_t XML_STATE_DETECT_PROPERTY_ATTR_CONTAINS;
   static const _bstr_t XML_STATE_DETECT_PROPERTY_ATTR_EQUALS;
   static const _bstr_t XML_STATE_DETECT_PROPERTY_ATTR_BEGINSWITH;
   static const _bstr_t XML_STATE_DETECT_PROPERTY_ATTR_ENDSWITH;
   static const _bstr_t XML_STATE_DETECT_PROPERTY_ATTR_REGEX;
   static const _bstr_t XML_STATE_DETECT_PROPERTY_ATTR_NEGATE;

   static const _bstr_t XML_CONTEXT_FIELD_RELATIVE;
   static const _bstr_t XML_CONTEXT_QUERY;
   static const _bstr_t XML_CONTEXT_ATTR_NAME;
   static const _bstr_t XML_CONTEXT_FIELD;
   static const _bstr_t XML_CONTEXT_FIELD_ATTR_NAME;
   static const _bstr_t XML_CONTEXT_FIELD_ATTR_POSITION;
   static const _bstr_t XML_CONTEXT_FIELD_ATTR_TYPE;
   static const _bstr_t XML_CONTEXT_FIELD_ATTR_POINTER;
   static const _bstr_t XML_PROPERTY_QUERY;
   static const _bstr_t XML_PROPERTY_ATTR_NAME;
   static const _bstr_t XML_PROPERTY_ATTR_VALUE;
   static const _bstr_t XML_PROPERTY_ATTR_MAPCONTEXT;
   static const _bstr_t XML_PROPERTY_ATTR_MAPCONTEXTFIELD;
   static const _bstr_t XML_PROPERTY_ALL_CONTEXT;

   static const _bstr_t XML_PROPERTIES_QUERY;
   static const _bstr_t XML_PROPERTIES_PROPERTY_NAME;
   static const _bstr_t XML_PROPERTY_ATTR_ALL_REFS;
   static const _bstr_t XML_INPUT_ATTR_ALL_REFS;
   static const _bstr_t XML_CONTROL_ATTR_ALL_REFS;
   static const _bstr_t XML_RESETCONTEXT_ATTR_ALL_REFS;
   static const _bstr_t XML_RESETCONTROL_ATTR_ALL_REFS;
   static const _bstr_t XML_RESETALLCONTEXTS_ATTR_ALL_REFS;

   static const _bstr_t XML_ONENTER;
   static const _bstr_t XML_ONEXIT;
   static const _bstr_t XML_ONCONTEXTEVENT;
   static const _bstr_t XML_ON_SETPROP;
   static const _bstr_t XML_ON_SETPROP_ATTR_NAME;
   static const _bstr_t XML_ON_SETPROP_ATTR_VALUE;
   static const _bstr_t XML_ON_SETPROP_ATTR_ACTION;
   static const _bstr_t XML_ON_SETPROP_ATTR_ACTION_ENUM_SET;
   static const _bstr_t XML_ON_SETPROP_ATTR_ACTION_ENUM_CONCATLEFT;
   static const _bstr_t XML_ON_SETPROP_ATTR_ACTION_ENUM_CONCATRIGHT;
   static const _bstr_t XML_ON_SENDINPUT;
   static const _bstr_t XML_ON_SENDINPUT_ATTR_NAME;
   static const _bstr_t XML_ON_SENDCONTROL;
   static const _bstr_t XML_ON_SENDCONTROL_ATTR_NAME;
   static const _bstr_t XML_ON_SENDCONTROLTOAPP;
   static const _bstr_t XML_ON_SENDCONTROLTOAPP_ATTR_NAME;

   static const _bstr_t XML_ON_RESETCONTEXT;
   static const _bstr_t XML_ON_RESETCONTEXT_ATTR_NAME;

   static const _bstr_t XML_ON_RESETCONTROL;
   static const _bstr_t XML_ON_RESETCONTROL_ATTR_NAME;

   static const _bstr_t XML_ON_RESETALLCONTEXTS;
   static const _bstr_t XML_ON_RESETALLCONTEXTS_EXCLUDE;
   static const _bstr_t XML_ON_RESETALLCONTEXTS_EXCLUDE_ATTR_NAME;
   static const _bstr_t XML_ON_RESETALLCONTEXTS_EXCLUDE_ATTR_ALL_REF;

   static const _bstr_t XML_EVENTS_ONCONTEXTEVENT_QUERY_ALL;
   static const _bstr_t XML_EVENTS_ONCONTEXTEVENT_ATTR_NAME;

   static const _bstr_t XML_CONTROL_QUERY;
   static const _bstr_t XML_CONTROL_ATTR_NAME;
   static const _bstr_t XML_CONTROL_CONTROLFIELDS_ALL_CONTEXT;
   static const _bstr_t XML_CONTROL_CONTROLFIELDS_ALL_PROP;
   static const _bstr_t XML_CONTROL_CONTROLFIELDS_ALL;
   static const _bstr_t XML_CONTROL_CONTROLFIELDS_ATTR_NAME;
   static const _bstr_t XML_CONTROL_CONTROLFIELDS_ATTR_MAPCONTEXT;
   static const _bstr_t XML_CONTROL_CONTROLFIELDS_ATTR_MAPCONTEXTFIELD;
   static const _bstr_t XML_CONTROL_CONTROLFIELDS_ATTR_MAPPROPERTY;

   static const _bstr_t XML_INPUT_QUERY;
   static const _bstr_t XML_INPUT_ATTR_NAME;
   static const _bstr_t XML_INPUT_TEXT;
   static const _bstr_t XML_INPUT_TEXT_ATTR_VALUE;
   static const _bstr_t XML_INPUT_SEQ;
   static const _bstr_t XML_INPUT_SEQ_ATTR_ID;
   static const _bstr_t XML_INPUT_SEQ_SEQFIELD;
   static const _bstr_t XML_INPUT_SEQ_SEQFIELD_ATTR_NAME;
   static const _bstr_t XML_INPUT_SEQ_SEQFIELD_ATTR_TYPE;
   static const _bstr_t XML_INPUT_SEQ_SEQFIELD_ATTR_LENGTH;
   static const _bstr_t XML_INPUT_SEQ_SEQFIELD_ATTR_MAPPROPERTY;
   static const _bstr_t XML_INPUT_MACRO;
   static const _bstr_t XML_INPUT_MACROITEM;
   static const _bstr_t XML_INPUT_MACROITEM_ATTR_TYPE; 
   static const _bstr_t XML_INPUT_MACROITEM_ATTR_VALUE;

   static LPCTSTR XML_HEX_START;
   static LPCTSTR XML_HEX_END;
   static const long XML_HEX_START_LEN;
   static const long XML_HEX_END_LEN;

   //
   // Internal utility methods for initial processing and  
   // initialization of Application Model
   //
protected:


   MSXML2::IXMLDOMDocumentPtr LoadAndXsdValidateXml(LPCTSTR pszFileName);
   MSXML2::IXMLDOMNodePtr GetContextField(_bstr_t & bstrContextName, _bstr_t & bstrFieldName);
   MSXML2::IXMLDOMNodePtr GetContext(_bstr_t & bstrName);

   void DumpModel(DWORD mask);
   bool CompileModel();
   bool ParseStates();
   Events& ParseEvents(Events& events, MSXML2::IXMLDOMNodeListPtr eventNodes);
   BOOL CompileCompleteContextAndFieldMap(void);
   BOOL ValidateXmlElementRelationships(void);
   _variant_t GetAttr(const MSXML2::IXMLDOMNodePtr & spNode, const _bstr_t & bstrName); 
   BOOL HasAttr(const MSXML2::IXMLDOMNodePtr & spNode, const _bstr_t & bstrName);
   CString REParseErrorToString(REParseError repe);

public:
   //
   // Context processing functions
   //
   State const& StateDetect(CString contextName);
   bool ExecuteOnContextEvent(LPCTSTR context);
   BOOL SetIgnoreContexts(const CString & strState);
   bool IsIgnoreContext(BSTR contextName);

   States const& GetStates() const
   {
      return states;
   }

   SequenceMap const& GetSequenceMap() const
   {
      return sequenceMap;
   }

   InputTextMap const& GetInputTextMap() const
   {
      return inputTextMap;
   }

public:
   // Public, hardwired states
   static const _bstr_t XML_STATE_CONNECTED;
   static const _bstr_t XML_STATE_DISCONNECTED;
   static const _bstr_t XML_STATE_DISCONNECTED_UNEXPECTED;

   // Public State attributes
   static const _bstr_t XML_STATE_ATTR_GROUP;
   static const _bstr_t XML_STATE_ATTR_IGNOREDUPLICATE;
   static const _bstr_t XML_STATE_ATTR_TRIGGERDELAY;

   typedef std::map<_bstr_t, _bstr_t> AttrMap;

   //
   // Utilities for Application Model merging
   //
protected:
   MSXML2::IXMLDOMDocumentPtr m_spMergeDoc;

   _bstr_t stateGroup;
   _bstr_t stateIgnoreDuplicate;
   _bstr_t stateTriggerDelay;

   State const* currentState;
   State const* previousState;

public:
   BOOL AppModelMergeBegin(LPCTSTR pszFileName);	// load the first document
   BOOL AppModelMergeAdd(LPCTSTR pszFileName);	// merge document on existing document
   BOOL AppModelMergeEnd(LPCTSTR pszFileName);	// write out the completed document to specified file


   //
   // Application Model processing functions
   //
   BOOL SetApplicationModel(LPCTSTR pszFileName);
   BOOL InitializeProperties(CProperties & properties);
   BOOL InitializeControls(CControls & controls);
   NCR::Util::tstring ConvertByteStreamsToText(NCR::Util::tstring const& text);
   _bstr_t GetStateName(const MSXML2::IXMLDOMNodePtr & spState);
   void GetStateAttributes(LPCTSTR pszState, CAppModel::AttrMap& attrMap);
   MSXML2::IXMLDOMNodePtr GetProperty(const _bstr_t & bstrName);
   MSXML2::IXMLDOMNodeListPtr GetProperties(void);
   _bstr_t GetPropertyName(const MSXML2::IXMLDOMNodePtr & spProperty);
   bool ExecuteOnStateExit();
   bool ExecuteOnStateEnter();


   MSXML2::IXMLDOMNodePtr GetInput(const _bstr_t & bstrName);
   MSXML2::IXMLDOMNodePtr GetControl(const _bstr_t & bstrName);

   LPCTSTR GetInputTextData(NCR::Util::tstring const& inputName);
   bool IsInputText(NCR::Util::tstring const& inputName);
   bool IsInputSequence(NCR::Util::tstring const& inputName);
   bool PopulateInputSequenceData(NCR::Util::tstring const& inputName, DataKit::SequenceBuffer& sequence);

   _bstr_t const& GetStateGroup() const
   {
      return stateGroup;
   }

   _bstr_t const& GetIgnoreDuplicate() const
   {
      return stateIgnoreDuplicate;
   }

   _bstr_t const& GetTriggerDelay() const
   {
      return stateTriggerDelay;
   }
};

