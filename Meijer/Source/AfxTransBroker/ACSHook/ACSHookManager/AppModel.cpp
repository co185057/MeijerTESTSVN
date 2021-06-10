// ACSHookModel.cpp : CAppModel method implementation
#include "stdafx.h"
#include "AppModel.h"
#include "ACSContext.h"
#include "Controls.h"
#include "Properties.h"
#include "Contexts.h"
#include "DataKit.h"
#include "Utils.h"
#include "TraceImpl.h"
#include <algorithm>

using namespace NCR::Util;


/* This is used for comparison purposes as a sentinel indicating no state was found. */
State State::nullState = State();



bool Equals::Execute(LPCTSTR value) const
{
   tstring const& comp = CompValue();
   return (comp == value);
}

tstring Equals::ToString() const
{
   tstringstream out;
   out << _T("Equals=\"") << CompValue() << _T("\"");
   return out.str();
}


bool Contains::Execute(LPCTSTR v) const
{
   tstring value(v);
   tstring const& comp = CompValue();
   return (value.find(comp) != tstring::npos);
}

tstring Contains::ToString() const
{
   tstringstream out;
   out << _T("Contains=\"") << CompValue() << _T("\"");
   return out.str();
}


bool BeginsWith::Execute(LPCTSTR v) const
{
   tstring value(v);
   tstring const& comp = CompValue();
   return (value.size() >= comp.size()) && (value.find(comp) == 0);
}

tstring BeginsWith::ToString() const
{
   tstringstream out;
   out << _T("BeginsWith=\"") << CompValue() << _T("\"");
   return out.str();
}


bool EndsWith::Execute(LPCTSTR v) const
{
   tstring value(v);
   tstring const& comp = CompValue();
   return (value.size() >= comp.size()) && (value.rfind(comp) == (value.size() - comp.size()));
}

tstring EndsWith::ToString() const
{
   tstringstream out;
   out << _T("EndsWith=\"") << CompValue() << _T("\"");
   return out.str();
}


bool RegEx::Execute(LPCTSTR v) const
{
   bool result = false;
   CAtlRegExp<> regEx;
   REParseError repe = regEx.Parse(CompValue().c_str());

   if (repe == REPARSE_ERROR_OK)
   {
      CAtlREMatchContext<> mc;
      result = !!regEx.Match(v, &mc);
   }
   else
   {
      // trace(DCM_WARNING, _T("[State:%s][Property:%s] RegEx parse error: '%s'"), state.name.c_str(), prop.name.c_str(), REParseErrorToString(repe).GetString());
      result = false;
   }

   return result;
}

tstring RegEx::ToString() const
{
   tstringstream out;
   out << _T("RegEx=\"") << CompValue() << _T("\"");
   return out.str();
}


bool EqualsProperty::Execute(LPCTSTR value) const
{
   CString comp(Utils.Properties().Get(CompValue().c_str()));
   return (comp == value);
}

tstring EqualsProperty::ToString() const
{
   tstringstream out;
   out << _T("EqualsProperty=\"") << CompValue() << _T("\"");
   return out.str();
}




bool SetProperty::Execute() const
{
   return Utils.Properties().Set(name.c_str(), value.c_str());
}

NCR::Util::tstring SetProperty::ToString() const
{
   NCR::Util::tstringstream out;
   out << _T("<SetProperty Action=\"Set\" Property=\"") << name << _T("\" Value=\"") << value << _T("\" />");
   return out.str();
}


bool ConcatLeftProperty::Execute() const
{
   CString newValue = Utils.Properties().DereferenceProperty(value.c_str());
   newValue += Utils.Properties().Get(name.c_str());
   
   return Utils.Properties().Set(
      name.c_str(), 
      newValue
      );
}

NCR::Util::tstring ConcatLeftProperty::ToString() const
{
   NCR::Util::tstringstream out;
   out << _T("<SetProperty Action=\"ConcatLeft\" Property=\"") << name << _T("\" Value=\"") << value << _T("\" />");
   return out.str();
}


bool ConcatRightProperty::Execute() const
{
   CString newValue = Utils.Properties().Get(name.c_str());
   newValue += Utils.Properties().DereferenceProperty(value.c_str());
   return Utils.Properties().Set(
      name.c_str(), 
      newValue
      );
}

NCR::Util::tstring ConcatRightProperty::ToString() const
{
   NCR::Util::tstringstream out;
   out << _T("<SetProperty Action=\"ConcatRight\" Property=\"") << name << _T("\" Value=\"") << value << _T("\" />");
   return out.str();
}


bool ResetContext::Execute() const
{
   return Utils.Contexts().ResetContext(name.c_str());
   // trace(DCM_INFO, _T("ResetContext '%s' was issued."), name.c_str());
}

NCR::Util::tstring ResetContext::ToString() const
{
   NCR::Util::tstringstream out;
   out << _T("<ResetContext Context=\"") << name << _T("\" />");
   return out.str();
}


bool ResetControl::Execute() const
{
   return Utils.Controls().ResetControl(name.c_str());
   // trace(DCM_INFO, _T("ResetControl '%s' was issued."), name.c_str());
}

NCR::Util::tstring ResetControl::ToString() const
{
   NCR::Util::tstringstream out;
   out << _T("<ResetControl Control=\"") << name << _T("\" />");
   return out.str();
}


bool SendControl::Execute() const
{
   return Utils.ACSHook().SendControlToTB(name.c_str());
   // trace(DCM_INFO, _T("SendControlToTB '%s' was issued."), name.c_str());
}

NCR::Util::tstring SendControl::ToString() const
{
   NCR::Util::tstringstream out;
   out << _T("<SendControl Control=\"") << name << _T("\" />");
   return out.str();
}


bool SendControlToApp::Execute() const
{
   return Utils.ACSHook().SendControlToApp(name.c_str());
   // trace(DCM_INFO, _T("SendControlToTB '%s' was issued."), name.c_str());
}

NCR::Util::tstring SendControlToApp::ToString() const
{
   NCR::Util::tstringstream out;
   out << _T("<SendControlToApp Control=\"") << name << _T("\" />");
   return out.str();
}


bool SendInput::Execute() const
{
   return Utils.ACSHook().SendInput(name.c_str());
   // trace(DCM_INFO, _T("SendInput '%s' was issued."), name.c_str());
}

NCR::Util::tstring SendInput::ToString() const
{
   NCR::Util::tstringstream out;
   out << _T("<SendInput Input=\"") << name << _T("\" />");
   return out.str();
}


bool ResetAllContexts::Execute() const
{
   bool retVal = true;

   if (excl.empty())
   {
      Utils.Contexts().ResetAllContexts();
      retVal = true;
   }
   else
   {
      // loop through all contexts and reset their content
      APPCONTEXTMAP & ctxMap = Utils.Contexts().GetContextMap();
      POSITION pos = ctxMap.GetStartPosition();
      APPCONTEXTMAPITEM * pcmi = NULL;

      while( pos && (pcmi = ctxMap.GetNext(pos)) )
      {
         // reset if not excluded
         if( excl.find(pcmi->m_key.GetString()) != excl.end() )
         {
            retVal = retVal && Utils.Contexts().ResetContext(pcmi->m_key);
         }
      }
   }

   return retVal;
}

NCR::Util::tstring ResetAllContexts::ToString() const
{
   NCR::Util::tstringstream out;

   if (excl.empty())
   {
      out << _T("<ResetAllContexts/>");
   }
   else
   {
      out << _T("<ResetAllContexts>");

      for (ResetExclusions::const_iterator it = excl.begin(); it != excl.end(); ++it)
      {
         out << _T("<Exclude Context=\"") << *it << _T("\" />");
      }

      out << _T("</ResetAllContexts>");
   }

   return out.str();
}


CAppModel::CAppModel() : 
   m_spModelDoc(NULL), 
   m_strModelFile(_T("")),
   currentState(&State::nullState),
   previousState(&State::nullState)
{

}

CAppModel::~CAppModel() 
{
   m_spModelDoc = NULL;
   m_strModelFile = _T("");
}


BOOL CAppModel::AppModelMergeBegin(LPCTSTR pszFileName)
{
   Trace trace(_T("CAppModel::AppModelMergeBegin"));
   trace(DCM_INFO) 
		<< _T("AppModelMergeBegin('") << pszFileName
		<< _T("')") << NCR::Util::endl;
   if( m_spMergeDoc != NULL )
   {
      trace(DCM_WARNING, _T("The document object used to begin a Model merge was not empty. Destroying prior contents."));
      m_spMergeDoc == NULL;
   }
   m_spMergeDoc = LoadAndXsdValidateXml(pszFileName);
   return m_spMergeDoc != NULL;
};	

BOOL CAppModel::AppModelMergeEnd(LPCTSTR pszFileName)
{
   Trace trace(_T("CAppModel::AppModelMergeEnd"));
   trace(DCM_INFO) 
		<< _T("AppModelMergeEnd('") << pszFileName
		<< _T("')") << NCR::Util::endl;

	if( m_spMergeDoc == NULL )
   {
      trace(DCM_ERROR, _T("AppModelMergeBegin() must be called successfully before saving a merged document."));
   }

   try
   {
      // a file name is not specified simply reset the merge
      if( pszFileName && *pszFileName )
      {
         // if the file exists and has the read only flag set change it
         DWORD dwAttr = ::GetFileAttributes(pszFileName);
         if( dwAttr != INVALID_FILE_ATTRIBUTES)
         {
            if( dwAttr & FILE_ATTRIBUTE_READONLY )
            {
               trace(DCM_ERROR) 
						<< _T("Existing '") << pszFileName
						<< _T("' file detected with read only flag. Removing it.") << NCR::Util::endl;

               ::SetFileAttributes(pszFileName, dwAttr & ~FILE_ATTRIBUTE_READONLY);
            }
            trace(DCM_INFO)
					<< _T("Deleting existing file '") << pszFileName
					<< _T("'") << NCR::Util::endl;
            ::DeleteFile(pszFileName);
         }
         m_spMergeDoc->save(_bstr_t(pszFileName));
      }

      m_spMergeDoc = NULL;
      return TRUE;
   }
   catch(_com_error & cex)
   {
      trace(DCM_ERROR)
			<< _T("An exception occured while saving the merged model to file '") << pszFileName
			<< _T("'. Error [0x") << cex.Error()
			<< _T("]: ") << cex.ErrorMessage() << NCR::Util::endl;
      return FALSE;
   }
}


BOOL CAppModel::AppModelMergeAdd(LPCTSTR pszFileName)
{
   Trace trace(_T("CAppModel::AppModelMergeAdd"));
   trace(DCM_INFO) 
		<< _T("AppModelMergeAdd('") << pszFileName
		<< _T("')") << NCR::Util::endl;

   if( m_spMergeDoc == NULL )
   {
      trace(DCM_ERROR, _T("AppModelMergeBegin() must be called successfully before merging another document."));
      return FALSE;
   }

   try
   {
      if( !ATLPath::FileExists(pszFileName) )
      {
         trace(DCM_WARNING) 
				<< _T("The merge model file '") << pszFileName
				<< _T("' was not found.") << NCR::Util::endl;
         return FALSE;
      }
      // TODO: Implement merge logic here
      return TRUE;
   }
   catch(_com_error & cex)
   {
      trace(DCM_ERROR)
			<< _T("An exception occured while merging in model '") << pszFileName
			<< _T("'. Error [0x") << cex.Error()
			<< _T("]: ") << cex.ErrorMessage() << NCR::Util::endl;
		return FALSE;
   }
}

_variant_t CAppModel::GetAttr(const MSXML2::IXMLDOMNodePtr & spNode, const _bstr_t & bstrName)
{
   MSXML2::IXMLDOMAttributePtr spAttr = spNode->attributes->getNamedItem(bstrName);

   _variant_t vtRet;  // initialized to VT_EMPTY
   if( spAttr != NULL )
   {
      vtRet = spAttr->nodeValue;  
   }
   return vtRet;
}

BOOL CAppModel::HasAttr(const MSXML2::IXMLDOMNodePtr & spNode, const _bstr_t & bstrName)
{
   return spNode->attributes->getNamedItem(bstrName) != NULL;
}

CString CAppModel::REParseErrorToString(REParseError repe)
{
   CString strError = _T("");
   switch(repe)
   {
   case REPARSE_ERROR_OK: strError = _T("No error occurred"); break;
   case REPARSE_ERROR_OUTOFMEMORY: strError = _T("Out of memory"); break;
   case REPARSE_ERROR_BRACE_EXPECTED: strError = _T("A closing brace was expected"); break;
   case REPARSE_ERROR_PAREN_EXPECTED: strError = _T("A closing parenthesis was expected"); break;
   case REPARSE_ERROR_BRACKET_EXPECTED: strError = _T("A closing bracket was expected"); break;
   case REPARSE_ERROR_UNEXPECTED: strError = _T("An unspecified fatal error occurred"); break;
   case REPARSE_ERROR_EMPTY_RANGE: strError = _T("A range expression was empty"); break;
   case REPARSE_ERROR_INVALID_GROUP: strError = _T("A backreference was made to a group that did not exist"); break;
   case REPARSE_ERROR_INVALID_RANGE: strError = _T("An invalid range was specified"); break;
   case REPARSE_ERROR_EMPTY_REPEATOP: strError = _T("A possibly empty * or + was detected"); break;
   case REPARSE_ERROR_INVALID_INPUT: strError = _T("The input string was invalid"); break;
   };
   return strError;
}

State const& CAppModel::StateDetect(CString contextName)
{
   Trace trace(_T("CAppModel::StateDetect"));
   trace(DCM_INFO, _T("In StateDetect"));
   
   static CString value;
   static CString comp;
   bool match = false;
   bool propMatch = true;
   bool contMatch = true;

   trace(DCM_INFO, _T("Iterating node list"));

   for (States::const_iterator siter = states.begin(); siter != states.end(); ++siter)
   {
      propMatch = true;
      contMatch = true;

      State const& state = *siter;
      DetectProperties const& properties = state.properties;

      for (DetectProperties::const_iterator piter = properties.begin(); propMatch && piter != properties.end(); ++piter)
      {
         DetectProperty const& prop = *piter;
         value = Utils.Properties().Get(prop.name.c_str());
         propMatch = prop.oper->Execute(value);

         if (prop.negate)
         {
            propMatch = !propMatch;
         }
      }

      if (!propMatch)
      {
         continue;
      }
      
      DetectContexts const& contexts = state.contexts;

      for (DetectContexts::const_iterator citer = contexts.begin(); contMatch && citer != contexts.end(); ++citer)
      {
         DetectContext const& cont = *citer;
         PAPPCONTEXT pCtx = Utils.Contexts().GetContext(cont.name.c_str());

         if (pCtx)
         {
            if (pCtx->bValidContents)
            {
               for (DetectFields::const_iterator fiter = cont.fields.begin(); contMatch && fiter != cont.fields.end(); ++fiter)
               {
                  DetectField const& field = *fiter;
                  PAPPCONTEXTFIELD pField = Utils.Contexts().GetContextField(cont.name.c_str(), field.name.c_str());

                  if (pField)
                  {
                     value = pField->strLastSavedValue;
                     contMatch = field.oper->Execute(value);

                     if (field.negate)
                     {
                        contMatch = !contMatch;
                     }
                  }
                  else
                  {
                     trace(DCM_WARNING) 
								<< _T("[State:") << state.name.c_str()
								<< _T("][Context:") << cont.name.c_str()
								<< _T("] DetectContext field '") << field.name.c_str()
								<< _T("' not found in the map. Ignoring detection criteria.") << NCR::Util::endl;
                  }
               }
            }
            else
            {
               /* this context has not 'occured' and the last saved value is stale */
               /*
               if (trace.IsLevel(DCM_DEBUG))
               {
                  trace(DCM_DEBUG, _T("[State:'%s'] StateDetectContext('%s'): Saved context is reset."), state.name.c_str(), cont.name.c_str());
               }
               */

               contMatch = false;;
            }
         }
         else
         {
            trace(DCM_WARNING)
					<< _T("[State:") << state.name.c_str()
					<< _T("] Could not find the context entry '") << cont.name.c_str()
					<< _T("' in the map. Ignoring detection criteria.") << NCR::Util::endl;
            _ASSERT(FALSE);
         }
      }

      match = propMatch && contMatch;

      if (match)
      {
         trace(DCM_INFO) 
				<< _T("Out of StateDetect -- Found '") << state.name.c_str()
				<< _T("'") << NCR::Util::endl;
         previousState = currentState;
         currentState = &state;
         return state;
      }
   }

   trace(DCM_INFO, _T("Out of StateDetect -- Not Found"));
   return State::nullState;
}


void CAppModel::DumpModel(DWORD mask)
{
   Trace trace(_T("DumpModel"));
   trace(mask, _T("Dumping model..."));
   
   for (States::const_iterator siter = states.begin(); siter != states.end(); ++siter)
   {
      State const& state = *siter;
      
      trace(mask) << _T("<State Name=\"") << state.name << _T("\" ");

      if (!state.group.empty())
      {
         trace(mask) << _T("Group=\"") << state.group << _T("\" ");
      }

      if (state.ignoreDuplicate)
      {
         trace(mask) << _T("IgnoreDuplicate=\"true\" ");
      }

      if (state.triggerDelay)
      {
         trace(mask) << _T("TriggerDelay=\"") << state.triggerDelay << _T("\" ");
      }

      trace(mask) << _T(">") << endl;

      DetectProperties const& properties = state.properties;
      DetectContexts const& contexts = state.contexts;

      if (!properties.empty() || !contexts.empty())
      {
         trace(mask) << _T("<Detect>") << endl;
      }

      for (DetectProperties::const_iterator piter = properties.begin(); piter != properties.end(); ++piter)
      {
         DetectProperty const& prop = *piter;
         trace(mask) << _T("<DetectProperty Property=\"") << prop.name << _T("\" ") << prop.oper->ToString();
         
         if (prop.negate)
         {
            trace(mask) << _T("Negate=\"true\" ");
         }

         trace(mask) << _T(" />") << endl;
      }

      for (DetectContexts::const_iterator citer = contexts.begin(); citer != contexts.end(); ++citer)
      {
         DetectContext const& cont = *citer;
         trace(mask) << _T("<DetectContext Context=\"") << cont.name << _T("\" >") << endl;

         for (DetectFields::const_iterator fiter = cont.fields.begin(); fiter != cont.fields.end(); ++fiter)
         {
            DetectField const& field = *fiter;
            trace(mask) << _T("<Field Name=\"") << field.name << _T("\" ") << field.oper->ToString();

            if (field.negate)
            {
               trace(mask) << _T("Negate=\"true\" ");
            }

            trace(mask) << _T("/>") << endl;
         }

         trace(mask) << _T("</DetectContext>") << endl;;
      }

      if (!properties.empty() || !contexts.empty())
      {
         trace(mask) << _T("</Detect>") << endl;
      }

      if (!state.onEnter.empty())
      {
         trace(mask, _T("<OnStateEnter>"));

         for (Events::const_iterator it = state.onEnter.begin(); it != state.onEnter.end(); ++it)
         {
            trace(mask, it->ToString().c_str());
         }

         trace(mask, _T("</OnStateEnter>"));
      }

      if (!state.onExit.empty())
      {
         trace(mask, _T("<OnStateExit>"));

         for (Events::const_iterator it = state.onExit.begin(); it != state.onExit.end(); ++it)
         {
            trace(mask, it->ToString().c_str());
         }

         trace(mask, _T("</OnStateExit>"));
      }

      trace(mask) << _T("</State>") << endl;
   }

   for (EventMap::const_iterator cevi = contextEvents.begin(); cevi != contextEvents.end(); ++cevi)
   {
      trace(mask) << _T("<OnContextEvent Context=\"") << cevi->first << _T("\">") << endl;
      
      for (Events::const_iterator ei = cevi->second.begin(); ei != cevi->second.end(); ++ei)
      {
         trace(mask, ei->ToString().c_str());
      }
      
      trace(mask, _T("</OnContextEvent>"));
   }


   /* TODO: To complete this debugging function, we need to add output of the Properties 
   and Inputs nodes. */
}

bool CAppModel::CompileModel()
{
   ParseStates();
   
   MSXML2::IXMLDOMNodeListPtr spNodes = m_spModelDoc->selectNodes(XML_EVENTS_ONCONTEXTEVENT_QUERY_ALL);
   MSXML2::IXMLDOMNodePtr spNode = NULL;
   MSXML2::IXMLDOMAttributePtr spAttr = NULL;
   _bstr_t tmp;

   while( (spNode = spNodes->nextNode()) != NULL )
   {
      spAttr = spNode->attributes->getNamedItem(XML_EVENTS_ONCONTEXTEVENT_ATTR_NAME);

      if (spAttr != NULL)
      {
         tmp = spAttr->nodeValue;
         EventMap::_Pairib result = contextEvents.insert(make_pair((LPCTSTR)tmp, Events()));

         if (result.second)
         {
            Events& events = result.first->second;
            ParseEvents(events, spNode->childNodes);
         }
      }
   }


   spNodes = m_spModelDoc->selectNodes(XML_INPUT_QUERY);
   MSXML2::IXMLDOMNodeListPtr inputNodes = NULL;
   MSXML2::IXMLDOMNodeListPtr seqNodes = NULL;
   _bstr_t nodeName;
   tstring inputName;
   tstring fieldName;
   tstring propertyName;
   tstring value;

   while( (spNode = spNodes->nextNode()) != NULL )
   {
      spAttr = spNode->attributes->getNamedItem(XML_INPUT_ATTR_NAME);

      if (spAttr != NULL)
      {
         tmp = spAttr->nodeValue;
         inputName = (LPCTSTR)tmp;
         inputNodes = spNode->childNodes;

         while( (spNode = inputNodes->nextNode()) != NULL )
         {
            nodeName = spNode->nodeName;

            if (nodeName == XML_INPUT_TEXT)
            {
               spAttr = spNode->attributes->getNamedItem(XML_INPUT_TEXT_ATTR_VALUE);

               if (spAttr != NULL)
               {
                  tmp = spAttr->nodeValue;
                  value = ConvertByteStreamsToText((LPCTSTR)tmp);

                  inputTextMap.insert(make_pair(inputName, value));
               }
            }
            else if (nodeName == XML_INPUT_SEQ)
            {
               SequenceMap::_Pairib result = sequenceMap.insert(make_pair(inputName, SequenceFields()));
               SequenceFields& fields = result.first->second;
               seqNodes = spNode->childNodes;

               while( (spNode = seqNodes->nextNode()) != NULL )
               {
                  nodeName = spNode->nodeName;

                  if (nodeName == XML_INPUT_SEQ_SEQFIELD)
                  {
                     spAttr = spNode->attributes->getNamedItem(XML_INPUT_SEQ_SEQFIELD_ATTR_NAME);

                     if (spAttr != NULL)
                     {
                        tmp = spAttr->nodeValue;
                        fieldName = (LPCTSTR)tmp;
                        spAttr = spNode->attributes->getNamedItem(XML_INPUT_SEQ_SEQFIELD_ATTR_MAPPROPERTY);

                        if (spAttr != NULL)
                        {
                           tmp = spAttr->nodeValue;
                           propertyName = (LPCTSTR)tmp;

                           fields.push_back(make_pair(fieldName, propertyName));
                        }
                     }
                  }
               }
            }
         }
      }
   }

   return true;
}

bool CAppModel::ParseStates()
{
   Trace trace(_T("CAppModel::CompileModel"));
   MSXML2::IXMLDOMNodeListPtr spStates = m_spModelDoc->selectNodes(XML_STATE);

   MSXML2::IXMLDOMNodePtr spState = NULL;
   MSXML2::IXMLDOMAttributePtr spAttr = NULL;

   while( (spState = spStates->nextNode()) != NULL )
   {
      spAttr = spState->attributes->getNamedItem(XML_STATE_ATTR_NAME);

      if (spAttr != NULL)
      {
         states.push_back(State());
         State& state = states.back();

         _bstr_t tmp = spAttr->nodeValue;
         state.name = (LPCTSTR)tmp;
         stateMap[state.name] = states.size() - 1;

         if ((spAttr = spState->attributes->getNamedItem(XML_STATE_ATTR_GROUP)) != NULL)
         {
            tmp = spAttr->nodeValue;
            state.groupBstr = tmp;
            state.group = (LPCTSTR)tmp;
         }

         if ((spAttr = spState->attributes->getNamedItem(XML_STATE_ATTR_IGNOREDUPLICATE)) != NULL)
         {
            _variant_t value = spAttr->nodeValue;
            state.ignoreDuplicateBstr = value;
            value.ChangeType(VT_BOOL);
            state.ignoreDuplicate = !!value.boolVal;
         }

         if ((spAttr = spState->attributes->getNamedItem(XML_STATE_ATTR_TRIGGERDELAY)) != NULL)
         {
            variant_t value = spAttr->nodeValue;
            state.triggerDelayBstr = value;
            value.ChangeType(VT_UI4);
            state.triggerDelay = value.uintVal;
         }

         MSXML2::IXMLDOMNodeListPtr nodes = spState->childNodes;
         MSXML2::IXMLDOMNodePtr node = NULL;

         while ( (node = nodes->nextNode()) != NULL )
         {
            _bstr_t nodeName = node->nodeName;

            if (nodeName == XML_STATE_DETECT)
            {
               MSXML2::IXMLDOMNodeListPtr detectNodes = node->childNodes;
               MSXML2::IXMLDOMNodePtr detectNode = NULL;

               while ( (detectNode = detectNodes->nextNode()) != NULL)
               {
                  nodeName = detectNode->nodeName;

                  if (nodeName == XML_STATE_DETECT_CONTEXT 
                     && (spAttr = detectNode->attributes->getNamedItem(XML_STATE_DETECT_CONTEXT_ATTR_NAME)) != NULL)
                  {
                     tmp = spAttr->nodeValue;
                     state.contexts.push_back(DetectContext((LPCTSTR)tmp));
                     DetectContext& context = state.contexts.back();

                     MSXML2::IXMLDOMNodeListPtr fieldNodes = detectNode->childNodes;
                     MSXML2::IXMLDOMNodePtr fieldNode = NULL;

                     while ( (fieldNode = fieldNodes->nextNode()) != NULL)
                     {
                        nodeName = fieldNode->nodeName;

                        if (nodeName == XML_STATE_DETECT_CONTEXT_FIELD)
                        {
                           if ((spAttr = fieldNode->attributes->getNamedItem(XML_STATE_DETECT_CONTEXT_FIELD_ATTR_NAME)) != NULL)
                           {
                              tmp = spAttr->nodeValue;
                              tstring fieldName = (LPCTSTR)tmp;
                              bool negate = false;

                              if ((spAttr = fieldNode->attributes->getNamedItem(XML_STATE_DETECT_CONTEXT_FIELD_ATTR_NEGATE)) != NULL)
                              {
                                 _variant_t value = spAttr->nodeValue;
                                 value.ChangeType(VT_BOOL);
                                 negate = !!value.boolVal;
                              }

                              if ((spAttr = fieldNode->attributes->getNamedItem(XML_STATE_DETECT_CONTEXT_FIELD_ATTR_CONTAINS)) != NULL)
                              {
                                 tmp = spAttr->nodeValue;
                                 context.fields.push_back(DetectField(fieldName, new Contains((LPCTSTR)tmp), negate));
                              }
                              else if ((spAttr = fieldNode->attributes->getNamedItem(XML_STATE_DETECT_CONTEXT_FIELD_ATTR_EQUALS)) != NULL)
                              {
                                 tmp = spAttr->nodeValue;
                                 context.fields.push_back(DetectField(fieldName, new Equals((LPCTSTR)tmp), negate));
                              }
                              else if ((spAttr = fieldNode->attributes->getNamedItem(XML_STATE_DETECT_CONTEXT_FIELD_ATTR_BEGINSWITH)) != NULL)
                              {
                                 tmp = spAttr->nodeValue;
                                 context.fields.push_back(DetectField(fieldName, new BeginsWith((LPCTSTR)tmp), negate));
                              }
                              else if ((spAttr = fieldNode->attributes->getNamedItem(XML_STATE_DETECT_CONTEXT_FIELD_ATTR_ENDSWITH)) != NULL)
                              {
                                 tmp = spAttr->nodeValue;
                                 context.fields.push_back(DetectField(fieldName, new EndsWith((LPCTSTR)tmp), negate));
                              }
                              else if ((spAttr = fieldNode->attributes->getNamedItem(XML_STATE_DETECT_CONTEXT_FIELD_ATTR_REGEX)) != NULL)
                              {
                                 tmp = spAttr->nodeValue;
                                 context.fields.push_back(DetectField(fieldName, new RegEx((LPCTSTR)tmp), negate));
                              }
                              else if ((spAttr = fieldNode->attributes->getNamedItem(XML_STATE_DETECT_CONTEXT_FIELD_ATTR_EQUALS_PROPERTY)) != NULL)
                              {
                                 tmp = spAttr->nodeValue;
                                 context.fields.push_back(DetectField(fieldName, new EqualsProperty((LPCTSTR)tmp), negate));
                              }

                           }
                        }
                     }
                  }
                  else if (nodeName == XML_STATE_DETECT_PROPERTY)
                  {
                     if ((spAttr = detectNode->attributes->getNamedItem(XML_STATE_DETECT_PROPERTY_ATTR_NAME)) != NULL)
                     {
                        tmp = spAttr->nodeValue;
                        tstring propertyName = (LPCTSTR)tmp;
                        bool negate = false;

                        if ((spAttr = detectNode->attributes->getNamedItem(XML_STATE_DETECT_PROPERTY_ATTR_NEGATE)) != NULL)
                        {
                           _variant_t value = spAttr->nodeValue;
                           value.ChangeType(VT_BOOL);
                           negate = !!value.boolVal;
                        }

                        if ((spAttr = detectNode->attributes->getNamedItem(XML_STATE_DETECT_PROPERTY_ATTR_CONTAINS)) != NULL)
                        {
                           tmp = spAttr->nodeValue;
                           state.properties.push_back(DetectProperty(propertyName, new Contains((LPCTSTR)tmp), negate));
                        }
                        else if ((spAttr = detectNode->attributes->getNamedItem(XML_STATE_DETECT_PROPERTY_ATTR_EQUALS)) != NULL)
                        {
                           tmp = spAttr->nodeValue;
                           state.properties.push_back(DetectProperty(propertyName, new Equals((LPCTSTR)tmp), negate));
                        }
                        else if ((spAttr = detectNode->attributes->getNamedItem(XML_STATE_DETECT_PROPERTY_ATTR_BEGINSWITH)) != NULL)
                        {
                           tmp = spAttr->nodeValue;
                           state.properties.push_back(DetectProperty(propertyName, new BeginsWith((LPCTSTR)tmp), negate));
                        }
                        else if ((spAttr = detectNode->attributes->getNamedItem(XML_STATE_DETECT_PROPERTY_ATTR_ENDSWITH)) != NULL)
                        {
                           tmp = spAttr->nodeValue;
                           state.properties.push_back(DetectProperty(propertyName, new EndsWith((LPCTSTR)tmp), negate));
                        }
                        else if ((spAttr = detectNode->attributes->getNamedItem(XML_STATE_DETECT_PROPERTY_ATTR_REGEX)) != NULL)
                        {
                           tmp = spAttr->nodeValue;
                           state.properties.push_back(DetectProperty(propertyName, new RegEx((LPCTSTR)tmp), negate));
                        }
                     }
                  }
               }
            }
            else if (nodeName == XML_STATE_IGNORE_QUERY)
            {
               MSXML2::IXMLDOMNodeListPtr ignoreNodes = node->childNodes;
               MSXML2::IXMLDOMNodePtr ignoreNode = NULL;

               while ( (ignoreNode = ignoreNodes->nextNode()) != NULL)
               {
                  nodeName = ignoreNode->nodeName;

                  if (nodeName == XML_STATE_IGNORE_CONTEXT_QUERY 
                     && (spAttr = ignoreNode->attributes->getNamedItem(XML_STATE_IGNORE_CONTEXT_ATTR_NAME)) != NULL)
                  {
                     tmp = spAttr->nodeValue;
                     tstring tmps = (LPCTSTR)tmp;
                     state.ignore.push_back(tmps);
                     tmps = state.ignore.back();
                  }
               }
            }
            else if (nodeName == XML_ONENTER)
            {
               ParseEvents(state.onEnter, node->childNodes);
            }
            else if (nodeName == XML_ONEXIT)
            {
               ParseEvents(state.onExit, node->childNodes);
            }
         }
      }
   }

   return true;
}

Events& CAppModel::ParseEvents(Events& events, MSXML2::IXMLDOMNodeListPtr eventNodes)
{
   MSXML2::IXMLDOMAttributePtr spAttr = NULL;
   MSXML2::IXMLDOMNodePtr eventNode = NULL;
   MSXML2::IXMLDOMAttributePtr valueAttr = NULL;
   MSXML2::IXMLDOMAttributePtr actionAttr = NULL;
   _bstr_t tmp;
   _bstr_t value;
   _bstr_t action;

   while ( (eventNode = eventNodes->nextNode()) != NULL)
   {
      _bstr_t nodeName = eventNode->nodeName;

      if (nodeName == XML_ON_SETPROP 
         && (spAttr = eventNode->attributes->getNamedItem(XML_ON_SETPROP_ATTR_NAME)) != NULL
         && (valueAttr = eventNode->attributes->getNamedItem(XML_ON_SETPROP_ATTR_VALUE)) != NULL
         && (actionAttr = eventNode->attributes->getNamedItem(XML_ON_SETPROP_ATTR_ACTION)) != NULL
         )
      {
         tmp = spAttr->nodeValue;
         value = valueAttr->nodeValue;
         action = actionAttr->nodeValue;

         if (action == XML_ON_SETPROP_ATTR_ACTION_ENUM_SET)
         {
            events.push_back(Event(new SetProperty((LPCTSTR)tmp, (LPCTSTR)value)));
         }
         else if (action == XML_ON_SETPROP_ATTR_ACTION_ENUM_CONCATLEFT)
         {
            events.push_back(Event(new ConcatLeftProperty((LPCTSTR)tmp, (LPCTSTR)value)));
         }
         else if (action == XML_ON_SETPROP_ATTR_ACTION_ENUM_CONCATRIGHT)
         {
            events.push_back(Event(new ConcatRightProperty((LPCTSTR)tmp, (LPCTSTR)value)));
         }
      }
      else if (nodeName == XML_ON_SENDINPUT 
         && (spAttr = eventNode->attributes->getNamedItem(XML_ON_SENDINPUT_ATTR_NAME)) != NULL
         )
      {
         tmp = spAttr->nodeValue;
         events.push_back(Event(new SendInput((LPCTSTR)tmp)));
      }
      else if (nodeName == XML_ON_SENDCONTROL 
         && (spAttr = eventNode->attributes->getNamedItem(XML_ON_SENDCONTROL_ATTR_NAME)) != NULL
         )
      {
         tmp = spAttr->nodeValue;
         events.push_back(Event(new SendControl((LPCTSTR)tmp)));
      }
      else if (nodeName == XML_ON_SENDCONTROLTOAPP 
         && (spAttr = eventNode->attributes->getNamedItem(XML_ON_SENDCONTROLTOAPP_ATTR_NAME)) != NULL
         )
      {
         tmp = spAttr->nodeValue;
         events.push_back(Event(new SendControlToApp((LPCTSTR)tmp)));
      }
      else if (nodeName == XML_ON_RESETCONTEXT 
         && (spAttr = eventNode->attributes->getNamedItem(XML_ON_RESETCONTEXT_ATTR_NAME)) != NULL
         )
      {
         tmp = spAttr->nodeValue;
         events.push_back(Event(new ResetContext((LPCTSTR)tmp)));
      }
      else if (nodeName == XML_ON_RESETCONTROL 
         && (spAttr = eventNode->attributes->getNamedItem(XML_ON_RESETCONTROL_ATTR_NAME)) != NULL
         )
      {
         tmp = spAttr->nodeValue;
         events.push_back(Event(new ResetControl((LPCTSTR)tmp)));
      }
      else if (nodeName == XML_ON_RESETALLCONTEXTS) 
      {
         MSXML2::IXMLDOMNodeListPtr resetNodes = eventNode->childNodes;
         MSXML2::IXMLDOMNodePtr resetNode = NULL;
         ResetExclusions excl;

         while ( (resetNode = resetNodes->nextNode()) != NULL)
         {
            nodeName = resetNode->nodeName;

            if (nodeName == XML_ON_RESETALLCONTEXTS_EXCLUDE 
               && (spAttr = eventNode->attributes->getNamedItem(XML_ON_RESETALLCONTEXTS_EXCLUDE_ATTR_NAME)) != NULL
               )
            {
               tmp = spAttr->nodeValue;
               excl.insert((LPCTSTR)tmp);
            }
         }

         events.push_back(Event(new ResetAllContexts(excl)));
      }
   }

   return events;
}

BOOL CAppModel::ValidateXmlElementRelationships(void)
{
   Trace trace(_T("CAppModel::ValidateXmlElementRelationships"));
   trace(DCM_INFO, _T("Validating all State, Context Detection, Context Definition and Property initialization."));

   MSXML2::IXMLDOMNodeListPtr spList = NULL;
   MSXML2::IXMLDOMNodePtr spNode = NULL;
   BOOL bFailed = FALSE;

   //
   // (1.1) Ensure that for each DetectContext Field specified there is a matching ContextField entry.
   // (/ApplicationModel/States/State/Detect/DetectContext/Field)
   trace(DCM_INFO, _T("Validating all referenced context fields have been defined..."));
   spList = m_spModelDoc->selectNodes(XML_STATE_DETECT_CONTEXT_FIELD_QUERY_ALL);
   spList->reset();
   while( (spNode = spList->nextNode()) != NULL )
   {
      // get the field's name 
      _bstr_t bstrFieldName = GetAttr(spNode, XML_STATE_DETECT_CONTEXT_FIELD_ATTR_NAME);

      // get the field's context - move up to the context and get its Name attribute
      _bstr_t bstrCtxName = GetAttr(spNode->parentNode, XML_STATE_DETECT_CONTEXT_ATTR_NAME);

      if (DataKit::GetContextFieldInfo((LPCTSTR)bstrCtxName, (LPCTSTR)bstrFieldName, NULL, NULL, NULL))
      {
         Utils.Contexts().GetAddContextField((LPCTSTR)bstrCtxName, (LPCTSTR)bstrFieldName);
      }
      else
      // if( GetContextField(bstrCtxName, bstrFieldName) == NULL )
      {
         // get the state name the field was found in; 
         _bstr_t bstrState = GetAttr(spNode->parentNode->parentNode->parentNode, XML_STATE_ATTR_NAME);
         trace(DCM_ERROR)
            << _T("[State='") << (LPCTSTR)bstrState
				<< _T("'][DetectContext='") << (LPCTSTR)bstrCtxName
				<< _T("']The context Field '") << (LPCTSTR)bstrFieldName
				<< _T("' referenced has not been defined.") << endl;
         bFailed = TRUE;
      }
   }


   //
   // (1.2.1) Ensure that for each DetectContext specified there is a matching Context entry.
   // (/ApplicationModel/States/State/Detect/DetectContext)
   trace(DCM_INFO, _T("Validating all referenced DetectContext contexts have been defined..."));
   spList = m_spModelDoc->selectNodes(XML_STATE_DETECT_CONTEXT_QUERY_ALL);
   spList->reset();
   while( (spNode = spList->nextNode()) != NULL )
   {
      // get the context name - see if it exists 
      _bstr_t bstrCtxName = GetAttr(spNode, XML_STATE_DETECT_CONTEXT_ATTR_NAME);

      /*
      if( GetContext(bstrCtxName) == NULL )
      {
         // get the state name the field was found in; 
         _bstr_t bstrState = GetAttr(spNode->parentNode->parentNode, XML_STATE_ATTR_NAME);
         trace(DCM_ERROR, 
            _T("[State='%s'] The context '%s' referenced in DetectContext has not been defined."), 
            (LPCTSTR)bstrState, (LPCTSTR)bstrCtxName);
         bFailed = TRUE;
      }
      */

      Utils.Contexts().GetAddContext((LPCTSTR)bstrCtxName);
   }


   //
   // (1.2.2) Ensure that for each OnContextEvent specified there is a matching Context entry.
   // (/ApplicationModel/Events/OnContextEvent)
   trace(DCM_INFO, _T("Validating all referenced OnContextEvent contexts have been defined..."));
   spList = m_spModelDoc->selectNodes(XML_EVENTS_ONCONTEXTEVENT_QUERY_ALL);
   spList->reset();
   while( (spNode = spList->nextNode()) != NULL )
   {
      // get the context name - see if it exists 
      _bstr_t bstrCtxName = GetAttr(spNode, XML_EVENTS_ONCONTEXTEVENT_ATTR_NAME);

      /*
      if( GetContext(bstrCtxName) == NULL )
      {
         // get the state name the field was found in; 
         _bstr_t bstrState = GetAttr(spNode->parentNode->parentNode, XML_STATE_ATTR_NAME);
         trace(DCM_ERROR, 
            _T("[State='%s'] The context '%s' referenced in OnContextEvent has not been defined."), 
            (LPCTSTR)bstrState, (LPCTSTR)bstrCtxName);
         bFailed = TRUE;
      }
      */

      Utils.Contexts().GetAddContext((LPCTSTR)bstrCtxName);
   }


   //
   // (1.3) Ensure that for each Control ControlField specified there is a matching ContextField entry.
   // (/ApplicationModel/Controls/Control/ControlField[exists(@MapContext)]
   //
   trace(DCM_INFO, _T("Validating all MapContext contexts binding Control Fields bindings have been defined..."));
   spList = m_spModelDoc->selectNodes(XML_CONTROL_CONTROLFIELDS_ALL_CONTEXT);
   spList->reset();
   while( (spNode = spList->nextNode()) != NULL )
   {
      _bstr_t bstrCtxName = GetAttr(spNode, XML_CONTROL_CONTROLFIELDS_ATTR_MAPCONTEXT);
      _bstr_t bstrFieldName = GetAttr(spNode, XML_CONTROL_CONTROLFIELDS_ATTR_MAPCONTEXTFIELD);

      if (DataKit::GetContextFieldInfo((LPCTSTR)bstrCtxName, (LPCTSTR)bstrFieldName, NULL, NULL, NULL))
      {
         Utils.Contexts().GetAddContextField((LPCTSTR)bstrCtxName, (LPCTSTR)bstrFieldName);
      }
      else
      {
         // Get the Control the referenced field was defined in
         _bstr_t bstrControl = GetAttr(spNode->parentNode, XML_CONTROL_ATTR_NAME);
         trace(DCM_ERROR)
            << _T("[Control='") << (LPCTSTR)bstrControl
				<< _T("'][Context='") << (LPCTSTR)bstrCtxName
				<< _T("']The context Field '") << (LPCTSTR)bstrFieldName
				<< _T("' referenced has not been defined.") << endl;
         bFailed = TRUE;
      }
   }

   //
   // (1.4) Ensure that for each Context bound property, there is a MapContext & MapContextField pair.
   // (/ApplicationModel/Properties/Property[exists(@MapContext)]
   //
   trace(DCM_INFO, _T("Validating all MapContext contexts binding Properties have been defined..."));
   spList = m_spModelDoc->selectNodes(XML_PROPERTY_ALL_CONTEXT);
   spList->reset();
   while( (spNode = spList->nextNode()) != NULL )
   {
      // Get the Control the referenced field was defined in
      _bstr_t bstrProperty = GetAttr(spNode, XML_PROPERTY_ATTR_NAME);
      _bstr_t bstrCtxName = GetAttr(spNode, XML_PROPERTY_ATTR_MAPCONTEXT);
      _bstr_t bstrFieldName = GetAttr(spNode, XML_PROPERTY_ATTR_MAPCONTEXTFIELD);

      if (DataKit::GetContextFieldInfo((LPCTSTR)bstrCtxName, (LPCTSTR)bstrFieldName, NULL, NULL, NULL))
      {
         Utils.Contexts().GetAddContextField((LPCTSTR)bstrCtxName, (LPCTSTR)bstrFieldName);
         // Utils.Contexts().AddMappedProperty((LPCTSTR)bstrCtxName, (LPCTSTR)bstrFieldName, (LPCTSTR)bstrProperty);
      }
      else
      {
         trace(DCM_ERROR)
            << _T("[Property='") << (LPCTSTR)bstrProperty
				<< _T("'][Context='") << (LPCTSTR)bstrCtxName
				<< _T("']The context field '") << (LPCTSTR)bstrFieldName
				<< _T("' referenced has not been defined.") << endl;
            
         bFailed = TRUE;
      }
   }

   //
   // (2) Ensure that all referenced properties have been properly initialized
   //     Properties in DetectProperty && ControlField/@MapProperty
   trace(DCM_INFO, _T("Validating all referenced Properties have been defined and initialized. Including /DetectProperty, /SetProperty, /ControlField/@MapProperty..."));

   spList = m_spModelDoc->selectNodes(XML_PROPERTY_ATTR_ALL_REFS);
   spList->reset();
   while( (spNode = spList->nextNode()) != NULL )
   {
      // get the property Name
      _bstr_t bstrPropName = spNode->nodeValue;

      if( GetProperty(bstrPropName) == NULL )
      {
         trace(DCM_ERROR)
				<< _T("The referenced Property '") << (LPCTSTR)bstrPropName
				<< _T("' has not been defined and initialized.") << endl;
         bFailed = TRUE;
      }
   }

   //
   // (3) Ensure all Inputs referenced are defined.
   //
   trace(DCM_INFO, _T("Validating all referenced Inputs have been defined..."));
   spList = m_spModelDoc->selectNodes(XML_INPUT_ATTR_ALL_REFS);
   spList->reset();
   while( (spNode = spList->nextNode()) != NULL )
   {
      // get the input name
      _bstr_t bstrInputName = spNode->nodeValue;

      if( GetInput(bstrInputName) == NULL )
      {
         trace(DCM_ERROR)
				<< _T("The referenced Input '") << (LPCTSTR)bstrInputName
				<< _T("' has not been defined.") << endl;
         bFailed = TRUE;
      }
   }

   //
   // (4) Ensure all Controls referenced are defined.
   //
   trace(DCM_INFO, _T("Validating all referenced Controls have been defined..."));
   spList = m_spModelDoc->selectNodes(XML_CONTROL_ATTR_ALL_REFS);
   spList->reset();
   while( (spNode = spList->nextNode()) != NULL )
   {
      // get the control name
      _bstr_t bstrControlName = spNode->nodeValue;

      if( GetControl(bstrControlName) == NULL )
      {
         trace(DCM_ERROR) 
				<< _T("The referenced Control '") << (LPCTSTR)bstrControlName
				<< _T("' has not been defined.") << endl;
         bFailed = TRUE;
      }
   }

   //
   // (5) Ensure all ResetContext contexts referenced are defined.
   //
   trace(DCM_INFO, _T("Validating all referenced ResetContext Contexts have been defined..."));
   spList = m_spModelDoc->selectNodes(XML_RESETCONTEXT_ATTR_ALL_REFS);
   spList->reset();
   while( (spNode = spList->nextNode()) != NULL )
   {
      // get the control name
      _bstr_t bstrCtxName = spNode->nodeValue;

      /*
      if( GetContext(bstrCtxName) == NULL )
      {
         trace(DCM_ERROR, _T("The referenced ResetContext Context '%s' has not been defined."), (LPCTSTR)bstrCtxName);
         bFailed = TRUE;
      }
      */

      Utils.Contexts().GetAddContext((LPCTSTR)bstrCtxName);
   }

   //
   // (5.1) Ensure all ResetControl controls referenced are defined.
   //
   trace(DCM_INFO, _T("Validating all referenced ResetControl Controls have been defined..."));
   spList = m_spModelDoc->selectNodes(XML_RESETCONTROL_ATTR_ALL_REFS);
   spList->reset();
   while( (spNode = spList->nextNode()) != NULL )
   {
      // get the control name
      _bstr_t bstrControlName = spNode->nodeValue;

      if( GetControl(bstrControlName) == NULL )
      {
         trace(DCM_ERROR)
				<< _T("The referenced ResetControl Control '") << (LPCTSTR)bstrControlName
				<< _T("' has not been defined.") << endl;
         bFailed = TRUE;
      }
   }

   //
   // (5.2) Ensure all ResetAllContexts/Exclude contexts referenced have been defined.
   //
   trace(DCM_INFO, _T("Validating all ResetAllContexts/Exclude contexts referenced have been defined..."));
   spList = m_spModelDoc->selectNodes(XML_RESETALLCONTEXTS_ATTR_ALL_REFS);
   spList->reset();
   while( (spNode = spList->nextNode()) != NULL )
   {
      // get the control name
      _bstr_t bstrCtxName = spNode->nodeValue;

      /*
      if( GetContext(bstrCtxName) == NULL )
      {
         trace(DCM_ERROR, _T("The referenced ResetAllContexts/Exclude Context '%s' has not been defined."), (LPCTSTR)bstrCtxName);
         bFailed = TRUE;
      }
      */

      Utils.Contexts().GetAddContext((LPCTSTR)bstrCtxName);
   }

   //
   // (6) Verify that all DetectProperty nodes have either Equals or Contains specified
   //
   trace(DCM_INFO, _T("Verify that all DetectProperty nodes have either Equals/Contains/BeginsWith/EndsWith/RegEx specified..."));
   spList = m_spModelDoc->selectNodes(XML_STATE + OLESTR("/") + XML_STATE_DETECT_PROPERTY_QUERY);
   spNode = NULL;
   spList->reset();
   while( (spNode = spList->nextNode()) != NULL )
   {
      if( !HasAttr(spNode, XML_STATE_DETECT_PROPERTY_ATTR_CONTAINS)  &&
         !HasAttr(spNode, XML_STATE_DETECT_PROPERTY_ATTR_EQUALS)    &&
         !HasAttr(spNode, XML_STATE_DETECT_PROPERTY_ATTR_BEGINSWITH)&&
         !HasAttr(spNode, XML_STATE_DETECT_PROPERTY_ATTR_ENDSWITH)  &&
         !HasAttr(spNode, XML_STATE_DETECT_PROPERTY_ATTR_REGEX)		)
      {
         trace(DCM_ERROR) 
				<< _T("DetectProperty '") << (LPCTSTR)(_bstr_t)GetAttr(spNode, XML_STATE_DETECT_PROPERTY_ATTR_NAME)
				<< _T("' for state '") << (LPCTSTR)(_bstr_t)GetAttr(spNode->parentNode->parentNode, XML_STATE_ATTR_NAME)
				<< _T("' has neither 'Equals/Contains/BeginsWith/EndsWith/RegEx' specified.") << endl;
            
         bFailed = TRUE;
      }
   }

   //
   // (7) Verify that all ControlFields have either (MapProperty) XOR (MapContext && MapContextField)
   //
   trace(DCM_INFO, _T("Verify that all ControlFields have either (MapProperty) XOR (MapContext && MapContextField)..."));
   spList = m_spModelDoc->selectNodes(XML_CONTROL_CONTROLFIELDS_ALL);
   spNode = NULL;
   spList->reset();
   while( (spNode = spList->nextNode()) != NULL )
   {
      _bstr_t bstrControl = GetAttr(spNode->parentNode, XML_CONTROL_ATTR_NAME);

      BOOL bMapCtx = HasAttr(spNode, XML_CONTROL_CONTROLFIELDS_ATTR_MAPCONTEXT);
      BOOL bMapField = HasAttr(spNode, XML_CONTROL_CONTROLFIELDS_ATTR_MAPCONTEXTFIELD);
      BOOL bMapProp = HasAttr(spNode, XML_CONTROL_CONTROLFIELDS_ATTR_MAPPROPERTY);

      _bstr_t bstrMapCtx = bMapCtx ? GetAttr(spNode, XML_CONTROL_CONTROLFIELDS_ATTR_MAPCONTEXT) : OLESTR("");
      _bstr_t bstrMapField = bMapField ? GetAttr(spNode, XML_CONTROL_CONTROLFIELDS_ATTR_MAPCONTEXTFIELD) : OLESTR("");
      _bstr_t bstrMapProp = bMapProp ? GetAttr(spNode, XML_CONTROL_CONTROLFIELDS_ATTR_MAPPROPERTY) : OLESTR("");

      // prepare potential error string
      CString strErrMsg = _T("");
      CString strFormat = _T("");
      strFormat.Format(_T("[Control:%s][MapContext:%s][MapContextField:%s][MapProperty:%s] "), 
         (LPCTSTR)bstrControl, (LPCTSTR)bstrMapCtx, (LPCTSTR)bstrMapField, (LPCTSTR)bstrMapProp);

      if( bMapCtx )
      {
         if( bMapField )
         {
            if( bMapProp )
            {
               strErrMsg = _T("Both Property and Context bindings cannot be specified for a Control Field.");
            }
         }
         else // no field mapping
         {
            if( bMapProp )
            {
               strErrMsg = _T("Both Property and Context bindings cannot be specified for a Control Field.");
            }
            else
            {
               strErrMsg = _T("A Context Field binding was not specified for the Context binding");
            }
         }
      }
      else // no map context
      {
         if( bMapField )
         {
            if( bMapProp )
            {
               strErrMsg = _T("A dangling Context Field property has been specified with the MapProperty.");
            }
            else
            {
               strErrMsg = _T("A Context binding was not specified for the Context Field binding.");
            }
         }
         else // no map field
         {
            if( !bMapProp )
            {
               strErrMsg = _T("Neither Context or Property binding information has been specified.");
            }
         }
      }

      if( strErrMsg.GetLength() > 0 )
      {
         trace(DCM_ERROR, strFormat + strErrMsg);
         bFailed = TRUE;
      }
   }

   // 
   // (8) Verify that all referenced Group states exist
   // 
   trace(DCM_INFO, _T("Verify that all referenced Group states exist"));
   spList = m_spModelDoc->selectNodes(XML_STATE_GROUP_QUERY);
   spNode = NULL;
   spList->reset();
   _bstr_t bstrQuery;
   while( (spNode = spList->nextNode()) != NULL )
   {
      _bstr_t bstrGroupState = GetAttr(spNode, XML_STATE_ATTR_GROUP);
      bstrQuery = XML_STATE + OLESTR("[@Name='") + bstrGroupState + OLESTR("']");

      if( m_spModelDoc->selectSingleNode(bstrQuery) == NULL )
      {
         trace(DCM_ERROR)
				<< _T("[State: ") << (LPCTSTR)((_bstr_t)GetAttr(spNode, XML_STATE_ATTR_NAME))
				<< _T("] Group state '") << (LPCTSTR)bstrGroupState
				<< _T("' referenced in this state definition does not exist.") << endl;
         bFailed = TRUE;
      }
   }

   return !bFailed;
}

MSXML2::IXMLDOMDocumentPtr CAppModel::LoadAndXsdValidateXml(LPCTSTR pszFileName)
{
   Trace trace(_T("CAppModel::LoadAndXsdValidateXml"));
   trace(DCM_INFO)
		<< _T("Loading and validating the Application Model XML file '") << pszFileName
		<< _T("'") << endl;
   //
   // First of all, if it's an empty string then we're trying 
   // to unload the configuration file; This should be allowed.
   MSXML2::IXMLDOMDocumentPtr spDoc = NULL;
   if( pszFileName && _tcslen(pszFileName) > 0 )
   {
      // make sure the file exists
      if( !PathFileExists(pszFileName) )
      {
         trace(DCM_ERROR)
				<< _T("The Application Model file specified does not exist: '") << pszFileName
				<< _T("'") << endl;
      }
      else
      {
         HRESULT hr = spDoc.CreateInstance(__uuidof(MSXML2::FreeThreadedDOMDocument60));
         _ASSERT(SUCCEEDED(hr));

         spDoc->async = VARIANT_FALSE;
         spDoc->preserveWhiteSpace = VARIANT_TRUE;
         spDoc->validateOnParse = VARIANT_TRUE;

         // 
         // Check the parameter is both a file and conforms to the XML standard
         //
         VARIANT_BOOL vbRet = spDoc->load(_bstr_t((pszFileName)));
         if( VARIANT_FALSE == vbRet )
         {
            trace(DCM_ERROR)
					<< _T("XML Validation Error: (Line:") << spDoc->parseError->line
					<< _T(") (Col:") << spDoc->parseError->linepos
					<< _T(") ") << (LPCTSTR)spDoc->parseError->reason << endl;
            spDoc = NULL;
         }
      }
   }
   return spDoc;
}

BOOL CAppModel::InitializeControls(CControls & controls)
{
   Trace trace(_T("CAppModel::InitializeControls"));
   trace(DCM_INFO, _T("+CAppModel::InitializeControls()"));
   BOOL bRet = TRUE;
   try
   {
      MSXML2::IXMLDOMNodeListPtr spList = NULL;
      MSXML2::IXMLDOMNodePtr spNode = NULL;
      //
      // Add all context bound controls and fields to map
      //
      spList = m_spModelDoc->selectNodes(XML_CONTROL_CONTROLFIELDS_ALL_CONTEXT);
      spList->reset();
      while( (spNode = spList->nextNode()) != NULL )
      {
         _bstr_t bstrControl = GetAttr(spNode->parentNode, XML_CONTROL_ATTR_NAME);
         _bstr_t bstrField = GetAttr(spNode, XML_CONTROL_CONTROLFIELDS_ATTR_NAME);
         _bstr_t bstrMapCtx = GetAttr(spNode, XML_CONTROL_CONTROLFIELDS_ATTR_MAPCONTEXT);
         _bstr_t bstrMapCtxField = GetAttr(spNode, XML_CONTROL_CONTROLFIELDS_ATTR_MAPCONTEXTFIELD);

         controls.AddContextControlField(bstrControl, bstrField, bstrMapCtx, bstrMapCtxField);
      }

      //
      // Add all property bound controls and fields to map
      //
      spList = m_spModelDoc->selectNodes(XML_CONTROL_CONTROLFIELDS_ALL_PROP);
      spList->reset();
      while( (spNode = spList->nextNode()) != NULL )
      {
         _bstr_t bstrControl = GetAttr(spNode->parentNode, XML_CONTROL_ATTR_NAME);
         _bstr_t bstrField = GetAttr(spNode, XML_CONTROL_CONTROLFIELDS_ATTR_NAME);
         _bstr_t bstrMapProperty = GetAttr(spNode, XML_CONTROL_CONTROLFIELDS_ATTR_MAPPROPERTY);

         controls.AddPropertyControlField(bstrControl, bstrField, bstrMapProperty);
      }
   }
   catch(_com_error & cex)
   {
      trace(DCM_ERROR) 
			<< _T("Failed to initialize the Controls.  Error [0x") << std::hex << cex.Error() << std::dec
			<< _T("]: ") << cex.ErrorMessage()
			<< _T(".") << endl;
      bRet = FALSE;
   }
   trace(DCM_INFO, _T("-CAppModel::InitializeControls()"));
   return bRet;
}

BOOL CAppModel::InitializeProperties(CProperties & properties)
{
   Trace trace(_T("CAppModel::InitializeProperties"));
   trace(DCM_INFO, _T("+CAppModel::InitializeProperties()"));
   BOOL bRet = TRUE;
   try
   {
      //
      // Extract the value pairs into a local string map
      //
      MSXML2::IXMLDOMNodeListPtr spPairs = m_spModelDoc->selectNodes(XML_PROPERTY_QUERY);
      MSXML2::IXMLDOMNodePtr spPair = NULL;
      spPairs->reset();
      while( (spPair = spPairs->nextNode()) != NULL )
      {
         try
         {
            properties.Set(
               (LPCTSTR)(_bstr_t)GetAttr(spPair, XML_PROPERTY_ATTR_NAME),
               (LPCTSTR)(_bstr_t)GetAttr(spPair, XML_PROPERTY_ATTR_VALUE),
               HasAttr(spPair, XML_PROPERTY_ATTR_MAPCONTEXT) ? 
               (LPCTSTR)(_bstr_t)GetAttr(spPair, XML_PROPERTY_ATTR_MAPCONTEXT) : NULL,
               HasAttr(spPair, XML_PROPERTY_ATTR_MAPCONTEXTFIELD) ? 
               (LPCTSTR)(_bstr_t)GetAttr(spPair, XML_PROPERTY_ATTR_MAPCONTEXTFIELD) : NULL);
         }
         catch(_com_error & cex)
         {
            trace(DCM_ERROR)
					<< _T("Failed to process a name/value pair: ") << cex.ErrorMessage() << endl;
         }
      }
   }
   catch(_com_error & cex)
   {
      trace(DCM_ERROR)
			<< _T("Failed to load the name/value pairs from node '") << XML_PROPERTY_QUERY
			<< _T("'. Error: '") << cex.ErrorMessage()
			<< _T("'") << endl;
      bRet = FALSE;
   }
   trace(DCM_INFO, _T("-CAppModel::InitializeProperties()"));
   return bRet;
}


BOOL CAppModel::SetApplicationModel(LPCTSTR pszFileName)
{
   Trace trace(_T("CAppModel::SetApplicationModel"));
   trace(DCM_INFO, _T("SetApplicationModel"));
   BOOL bRet = TRUE;


   MSXML2::IXMLDOMDocumentPtr spDoc = LoadAndXsdValidateXml(pszFileName);

   //
   // set the new file if it's not a request to reset to blank or a failed load
   //
   if( spDoc != NULL )
   {
      m_spModelDoc = spDoc;
      m_strModelFile = pszFileName;
      trace(DCM_INFO)
			<< _T("Loaded a new XML Application Model: '") << m_strModelFile.GetString()
			<< _T("'") << endl;
   }
   else if( _tcslen(pszFileName) )
   {
      trace(DCM_INFO, _T("Cleared the current XML Application Model"));
      m_strModelFile = _T("");
      m_spModelDoc = NULL;
   }
   else
   {
      // something went wrong - it's been logged
      bRet = FALSE;
   }

   if( bRet && (m_spModelDoc != NULL) )
   {
      bRet = ValidateXmlElementRelationships();
      if( bRet )
      {
         bRet = CompileModel();			

         if (trace.IsLevel(DCM_DEBUG))
         {
            DumpModel(DCM_DEBUG);
         }
      }
   }

   return bRet;
}


bool CAppModel::ExecuteOnStateExit()
{
   return previousState->OnExit();
}


bool CAppModel::ExecuteOnStateEnter()
{
   return currentState->OnEnter();
}


bool CAppModel::ExecuteOnContextEvent(LPCTSTR context)
{
   bool eventResult = true;
   EventMap::const_iterator ci = contextEvents.find(context);

   if (ci != contextEvents.end())
   {
      for (Events::const_iterator it = ci->second.begin(); it != ci->second.end(); ++it)
      {
         _ASSERTE(eventResult);
         eventResult = eventResult && it->Execute();
      }
   }

   return eventResult;
}


LPCTSTR CAppModel::GetInputTextData(tstring const& inputName)
{
   LPCTSTR text = 0;
   InputTextMap::const_iterator it = inputTextMap.find(inputName);

   if (it != inputTextMap.end())
   {
      text = it->second.c_str();
   }

   return text;
}

bool CAppModel::IsInputText(tstring const& inputName)
{
   return inputTextMap.find(inputName) != inputTextMap.end();
}

bool CAppModel::IsInputSequence(tstring const& inputName)
{
   return sequenceMap.find(inputName) != sequenceMap.end();
}

bool CAppModel::PopulateInputSequenceData(NCR::Util::tstring const& inputName, DataKit::SequenceBuffer& sequence)
{
   Trace trace(_T("CAppModel::PopulateInputSequenceData"));
   _ASSERT(!inputName.empty());

   trace(DCM_DEBUG, _T("PopulateInputSequenceData()"));

   SequenceMap::const_iterator smi = sequenceMap.find(inputName);

   if (smi != sequenceMap.end())
   {
      SequenceFields const& sf = smi->second;

      for (SequenceFields::const_iterator sfi = sf.begin(); sfi != sf.end(); ++sfi)
      {
         sequence.Set(sfi->first.c_str(), Utils.Properties().Get(sfi->second.c_str()).GetString());
      }
   }

   return true;
}


MSXML2::IXMLDOMNodePtr CAppModel::GetControl(const _bstr_t & bstrInput)
{
   CString query;
   query.Format(_T("/ApplicationModel/Controls/Control[@Name = '%s']"), (LPCTSTR)bstrInput);

   /*
   _bstr_t bstrQuery = XML_CONTROL_QUERY + OLESTR("[@") + XML_CONTROL_ATTR_NAME;
   bstrQuery += OLESTR("='") + bstrInput + OLESTR("']");
   */
   return m_spModelDoc->selectSingleNode((LPCTSTR)query);
}


MSXML2::IXMLDOMNodePtr CAppModel::GetInput(const _bstr_t & bstrInput)
{
   CString query;
   query.Format(_T("/ApplicationModel/Inputs/Input[@Name = '%s']"), (LPCTSTR)bstrInput);

   /*
   _bstr_t bstrQuery = XML_INPUT_QUERY + OLESTR("[@") + XML_INPUT_ATTR_NAME;
   bstrQuery += OLESTR("='") + bstrInput + OLESTR("']");
   */
   return m_spModelDoc->selectSingleNode((LPCTSTR)query);
}

MSXML2::IXMLDOMNodeListPtr CAppModel::GetProperties(void)
{
   MSXML2::IXMLDOMNodeListPtr spList = m_spModelDoc->selectNodes(XML_PROPERTY_QUERY);
   if( NULL != spList )
   {
      spList->reset();
   }
   return spList;
}

MSXML2::IXMLDOMNodePtr CAppModel::GetProperty(const _bstr_t & bstrName)
{
   CString query;
   query.Format(_T("/ApplicationModel/Properties/Property[@Name = '%s']/@InitialValue"), (LPCTSTR)bstrName);

   /*
   _bstr_t bstrQuery = XML_PROPERTY_QUERY + OLESTR("[@") + XML_PROPERTY_ATTR_NAME;
   bstrQuery += OLESTR("='") + bstrName + OLESTR("']/@") + XML_PROPERTY_ATTR_VALUE;
   */
   return m_spModelDoc->selectSingleNode((LPCTSTR)query);
}

_bstr_t CAppModel::GetPropertyName(const MSXML2::IXMLDOMNodePtr & spProperty)
{
   return GetAttr(spProperty, XML_PROPERTY_ATTR_NAME);
}

tstring CAppModel::ConvertByteStreamsToText(tstring const& text)
{
   int iPos = 0;
   int iStart = 0;
   int iEnd = 0;
   CString str = text.c_str();

   // find the first Hex start token match
   while( (iStart = str.Find(XML_HEX_START, iPos)) >= iPos )
   {
      // find the first Hex end token match
      if( (iEnd = str.Find(XML_HEX_END, iStart)) >= iStart )
      {
         // extract the characters in between and convert them to their ASCII equivalents
         CString strHex = str.Mid(iStart+XML_HEX_START_LEN, iEnd - iStart - XML_HEX_START_LEN);
         _ASSERT(!(strHex.GetLength() % 2));

         str.Delete(iStart, iEnd - iStart + XML_HEX_END_LEN);
         // there should be an even number of hex characters

         strHex.MakeUpper();
         LPCTSTR psz = strHex.LockBuffer();

         // how is this affected while we're in unicode? not sure yet.
         // for now we'll just get the character value and create a byte array 
         // which we'll insert directly in the text stream. So if we're in double byte
         // characters, then 4 bytes of WCHAR will be converted to a single byte and
         // 2 bytes of CHAR in the case of ANSI text.
         for(int i = 0; i < strHex.GetLength(); i+=2)
         {
            BYTE by = 0;

            // high order nibble
            if( psz[i] >= _T('0') && psz[i] <= _T('9') )
            {
               by = (psz[i] - _T('0')) << 4;
            }
            else if( psz[i] >= _T('A') && psz[i] <= _T('Z') )
            {
               by = (psz[i] - _T('A') + 10) << 4;
            }
            else
            {
               _ASSERT(FALSE);
            }

            // low order nibble
            if( psz[i+1] >= _T('0') && psz[i+1] <= _T('9') )
            {
               by = by | (psz[i+1] - _T('0'));
            }
            else if( psz[i+1] >= _T('A') && psz[i+1] <= _T('Z') )
            {
               by = by | (psz[i+1] - _T('A') + 10);
            }
            else
            {
               _ASSERT(FALSE);
            }
            str.Insert(iStart,by); // will insert the byte in the string and will convert it to Unicode if required.
            // i.e.  if there are two byte 'beef' then the string appended will be 00BE 00EF
            //       in the case of unicode
            iStart++;
         }
         strHex.UnlockBuffer();
         iPos = iStart;
      }
      iPos = iStart + XML_HEX_START_LEN;
   }

   return tstring((LPCTSTR)str);
}

_bstr_t CAppModel::GetStateName(const MSXML2::IXMLDOMNodePtr & spState)
{
   return GetAttr(spState, XML_STATE_ATTR_NAME);
}


bool CAppModel::IsIgnoreContext(BSTR contextName)
{
   bool retVal = std::find(currentState->ignore.begin(), currentState->ignore.end(), contextName) != currentState->ignore.end();
   return retVal;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CAppModel constants initialization section
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const _bstr_t CAppModel::XML_STATE_CONNECTED = _bstr_t(OLESTR("CONNECTED"));
const _bstr_t CAppModel::XML_STATE_DISCONNECTED = _bstr_t(OLESTR("DISCONNECTED"));
const _bstr_t CAppModel::XML_STATE_DISCONNECTED_UNEXPECTED = _bstr_t(OLESTR("DISCONNECTED_UNEXPECTED"));

const _bstr_t CAppModel::XML_STATE = _bstr_t(OLESTR("/ApplicationModel/States/State"));
const _bstr_t CAppModel::XML_STATE_ATTR_NAME = _bstr_t(OLESTR("Name"));
const _bstr_t CAppModel::XML_STATE_ATTR_GROUP = _bstr_t(OLESTR("Group"));
const _bstr_t CAppModel::XML_STATE_ATTR_IGNOREDUPLICATE = _bstr_t(OLESTR("IgnoreDuplicate"));
const _bstr_t CAppModel::XML_STATE_ATTR_TRIGGERDELAY = _bstr_t(OLESTR("TriggerDelay"));
const _bstr_t CAppModel::XML_STATE_IGNORE_QUERY = _bstr_t(OLESTR("Ignore"));
const _bstr_t CAppModel::XML_STATE_IGNORE_CONTEXT_QUERY = _bstr_t(OLESTR("Context"));
const _bstr_t CAppModel::XML_STATE_IGNORE_CONTEXT_ATTR_NAME = _bstr_t(OLESTR("Name"));
const _bstr_t CAppModel::XML_STATE_DETECT = _bstr_t(OLESTR("Detect"));
const _bstr_t CAppModel::XML_STATE_DETECT_CONTEXT = _bstr_t(OLESTR("DetectContext"));
const _bstr_t CAppModel::XML_STATE_DETECT_PROPERTY = _bstr_t(OLESTR("DetectProperty"));
const _bstr_t CAppModel::XML_STATE_DETECT_CONTEXT_QUERY = _bstr_t(OLESTR("Detect/DetectContext"));
const _bstr_t CAppModel::XML_STATE_DETECT_CONTEXT_ATTR_NAME = _bstr_t(OLESTR("Context"));
const _bstr_t CAppModel::XML_STATE_DETECT_CONTEXT_FIELD = _bstr_t(OLESTR("Field"));
const _bstr_t CAppModel::XML_STATE_DETECT_CONTEXT_FIELD_ATTR_NAME = _bstr_t(OLESTR("Name"));
const _bstr_t CAppModel::XML_STATE_DETECT_CONTEXT_FIELD_ATTR_CONTAINS = _bstr_t(OLESTR("Contains"));
const _bstr_t CAppModel::XML_STATE_DETECT_CONTEXT_FIELD_ATTR_EQUALS = _bstr_t(OLESTR("Equals"));
const _bstr_t CAppModel::XML_STATE_DETECT_CONTEXT_FIELD_ATTR_BEGINSWITH = _bstr_t(OLESTR("BeginsWith"));
const _bstr_t CAppModel::XML_STATE_DETECT_CONTEXT_FIELD_ATTR_ENDSWITH = _bstr_t(OLESTR("EndsWith"));
const _bstr_t CAppModel::XML_STATE_DETECT_CONTEXT_FIELD_ATTR_REGEX = _bstr_t(OLESTR("RegEx"));
const _bstr_t CAppModel::XML_STATE_DETECT_CONTEXT_FIELD_ATTR_EQUALS_PROPERTY = _bstr_t(OLESTR("EqualsProperty"));
const _bstr_t CAppModel::XML_STATE_DETECT_CONTEXT_FIELD_ATTR_NEGATE = _bstr_t(OLESTR("Negate"));
const _bstr_t CAppModel::XML_STATE_DETECT_PROPERTY_QUERY = _bstr_t(OLESTR("Detect/DetectProperty"));
const _bstr_t CAppModel::XML_STATE_DETECT_PROPERTY_ATTR_NAME = _bstr_t(OLESTR("Property"));
const _bstr_t CAppModel::XML_STATE_DETECT_PROPERTY_ATTR_CONTAINS = _bstr_t(OLESTR("Contains"));
const _bstr_t CAppModel::XML_STATE_DETECT_PROPERTY_ATTR_EQUALS = _bstr_t(OLESTR("Equals"));
const _bstr_t CAppModel::XML_STATE_DETECT_PROPERTY_ATTR_BEGINSWITH = _bstr_t(OLESTR("BeginsWith"));
const _bstr_t CAppModel::XML_STATE_DETECT_PROPERTY_ATTR_ENDSWITH = _bstr_t(OLESTR("EndsWith"));
const _bstr_t CAppModel::XML_STATE_DETECT_PROPERTY_ATTR_REGEX = _bstr_t(OLESTR("RegEx"));
const _bstr_t CAppModel::XML_STATE_DETECT_PROPERTY_ATTR_NEGATE = _bstr_t(OLESTR("Negate"));
const _bstr_t CAppModel::XML_STATE_DETECT_CONTEXT_QUERY_ALL = _bstr_t(OLESTR("/ApplicationModel/States/State/Detect/DetectContext"));
const _bstr_t CAppModel::XML_STATE_DETECT_CONTEXT_FIELD_QUERY_ALL = _bstr_t(OLESTR("/ApplicationModel/States/State/Detect/DetectContext/Field"));
const _bstr_t CAppModel::XML_STATE_GROUP_QUERY = _bstr_t(OLESTR("/ApplicationModel/States/State[@Group]"));

const _bstr_t CAppModel::XML_CONTEXT_QUERY = _bstr_t(OLESTR("/ApplicationModel/Contexts/Context"));
const _bstr_t CAppModel::XML_CONTEXT_ATTR_NAME = _bstr_t(OLESTR("Name"));
const _bstr_t CAppModel::XML_CONTEXT_FIELD = _bstr_t(OLESTR("/ApplicationModel/Contexts/Context/ContextField"));
const _bstr_t CAppModel::XML_CONTEXT_FIELD_RELATIVE = _bstr_t(OLESTR("ContextField"));
const _bstr_t CAppModel::XML_CONTEXT_FIELD_ATTR_NAME = _bstr_t(OLESTR("Name"));
const _bstr_t CAppModel::XML_CONTEXT_FIELD_ATTR_POSITION = _bstr_t(OLESTR("Position"));
const _bstr_t CAppModel::XML_CONTEXT_FIELD_ATTR_TYPE = _bstr_t(OLESTR("Type"));
const _bstr_t CAppModel::XML_CONTEXT_FIELD_ATTR_POINTER = _bstr_t(OLESTR("Pointer"));
const _bstr_t CAppModel::XML_PROPERTY_QUERY = _bstr_t(OLESTR("/ApplicationModel/Properties/Property"));
const _bstr_t CAppModel::XML_PROPERTY_ATTR_NAME = _bstr_t(OLESTR("Name"));
const _bstr_t CAppModel::XML_PROPERTY_ATTR_VALUE = _bstr_t(OLESTR("InitialValue"));
const _bstr_t CAppModel::XML_PROPERTY_ATTR_MAPCONTEXT = _bstr_t(OLESTR("MapContext"));
const _bstr_t CAppModel::XML_PROPERTY_ATTR_MAPCONTEXTFIELD = _bstr_t(OLESTR("MapContextField"));
const _bstr_t CAppModel::XML_PROPERTY_ALL_CONTEXT = _bstr_t(OLESTR("/ApplicationModel/Properties/Property[@MapContext]"));
const _bstr_t CAppModel::XML_PROPERTIES_QUERY = _bstr_t(OLESTR("/ApplicationModel/Properties"));
const _bstr_t CAppModel::XML_PROPERTIES_PROPERTY_NAME = _bstr_t(OLESTR("Property"));
const _bstr_t CAppModel::XML_PROPERTY_ATTR_ALL_REFS = _bstr_t(OLESTR("/ApplicationModel/States/State/Detect/DetectProperty/@Property|/ApplicationModel/States/State//SetProperty/@Property|/ApplicationModel/Controls/Control/ControlField[@MapProperty]/@MapProperty|/ApplicationModel/Events//SetProperty/@Property|/ApplicationModel/Inputs/Input/Sequence/SequenceField/@MapProperty"));
const _bstr_t CAppModel::XML_INPUT_ATTR_ALL_REFS = _bstr_t(OLESTR("/ApplicationModel/States/State//SendInput/@Input|/ApplicationModel/Events//SendInput/@Input"));
const _bstr_t CAppModel::XML_CONTROL_ATTR_ALL_REFS = _bstr_t(OLESTR("/ApplicationModel/States/State//SendControl/@Control|/ApplicationModel/Events//SendControl/@Control|/ApplicationModel/Events//SendControlToApp/@Control"));
const _bstr_t CAppModel::XML_RESETCONTEXT_ATTR_ALL_REFS = _bstr_t(OLESTR("/ApplicationModel/States/State//ResetContext/@Context|/ApplicationModel/Events//ResetContext/@Context"));
const _bstr_t CAppModel::XML_RESETCONTROL_ATTR_ALL_REFS = _bstr_t(OLESTR("/ApplicationModel/States/State//ResetControl/@Control|/ApplicationModel/Events//ResetControl/@Control"));
const _bstr_t CAppModel::XML_RESETALLCONTEXTS_ATTR_ALL_REFS = _bstr_t(OLESTR("/ApplicationModel/States/State//ResetAllContexts/Exclude/@Context|/ApplicationModel/Events//ResetAllContexts/Exclude/@Context"));

const _bstr_t CAppModel::XML_ONENTER = _bstr_t(OLESTR("OnStateEnter"));
const _bstr_t CAppModel::XML_ONEXIT = _bstr_t(OLESTR("OnStateExit"));
const _bstr_t CAppModel::XML_ONCONTEXTEVENT = _bstr_t(OLESTR("OnContextEvent"));
const _bstr_t CAppModel::XML_ON_SETPROP = _bstr_t(OLESTR("SetProperty"));
const _bstr_t CAppModel::XML_ON_SETPROP_ATTR_NAME = _bstr_t(OLESTR("Property"));
const _bstr_t CAppModel::XML_ON_SETPROP_ATTR_VALUE = _bstr_t(OLESTR("Value"));
const _bstr_t CAppModel::XML_ON_SETPROP_ATTR_ACTION = _bstr_t(OLESTR("Action"));
const _bstr_t CAppModel::XML_ON_SETPROP_ATTR_ACTION_ENUM_SET = _bstr_t(OLESTR("Set"));
const _bstr_t CAppModel::XML_ON_SETPROP_ATTR_ACTION_ENUM_CONCATLEFT = _bstr_t(OLESTR("ConcatLeft"));
const _bstr_t CAppModel::XML_ON_SETPROP_ATTR_ACTION_ENUM_CONCATRIGHT = _bstr_t(OLESTR("ConcatRight"));
const _bstr_t CAppModel::XML_ON_SENDINPUT = _bstr_t(OLESTR("SendInput"));
const _bstr_t CAppModel::XML_ON_SENDINPUT_ATTR_NAME = _bstr_t(OLESTR("Input"));
const _bstr_t CAppModel::XML_ON_SENDCONTROL = _bstr_t(OLESTR("SendControl"));
const _bstr_t CAppModel::XML_ON_SENDCONTROL_ATTR_NAME = _bstr_t(OLESTR("Control"));
const _bstr_t CAppModel::XML_ON_SENDCONTROLTOAPP = _bstr_t(OLESTR("SendControlToApp"));
const _bstr_t CAppModel::XML_ON_SENDCONTROLTOAPP_ATTR_NAME = _bstr_t(OLESTR("Control"));

const _bstr_t CAppModel::XML_ON_RESETCONTEXT = _bstr_t(OLESTR("ResetContext"));
const _bstr_t CAppModel::XML_ON_RESETCONTEXT_ATTR_NAME = _bstr_t(OLESTR("Context"));
const _bstr_t CAppModel::XML_ON_RESETCONTROL = _bstr_t(OLESTR("ResetControl"));
const _bstr_t CAppModel::XML_ON_RESETCONTROL_ATTR_NAME = _bstr_t(OLESTR("Control"));

const _bstr_t CAppModel::XML_ON_RESETALLCONTEXTS = _bstr_t(OLESTR("ResetAllContexts"));
const _bstr_t CAppModel::XML_ON_RESETALLCONTEXTS_EXCLUDE = _bstr_t(OLESTR("Exclude"));
const _bstr_t CAppModel::XML_ON_RESETALLCONTEXTS_EXCLUDE_ATTR_NAME = _bstr_t(OLESTR("Context"));
const _bstr_t CAppModel::XML_ON_RESETALLCONTEXTS_EXCLUDE_ATTR_ALL_REF = _bstr_t(OLESTR("Exclude/@Context"));

const _bstr_t CAppModel::XML_EVENTS_ONCONTEXTEVENT_QUERY_ALL = _bstr_t(OLESTR("/ApplicationModel/Events/OnContextEvent"));
const _bstr_t CAppModel::XML_EVENTS_ONCONTEXTEVENT_ATTR_NAME = _bstr_t(OLESTR("Context"));

const _bstr_t CAppModel::XML_CONTROL_QUERY = _bstr_t(OLESTR("/ApplicationModel/Controls/Control"));
const _bstr_t CAppModel::XML_CONTROL_ATTR_NAME = _bstr_t(OLESTR("Name"));
const _bstr_t CAppModel::XML_CONTROL_CONTROLFIELDS_ALL_CONTEXT = _bstr_t(OLESTR("/ApplicationModel/Controls/Control/ControlField[@MapContext]"));
const _bstr_t CAppModel::XML_CONTROL_CONTROLFIELDS_ALL_PROP = _bstr_t(OLESTR("/ApplicationModel/Controls/Control/ControlField[@MapProperty]"));
const _bstr_t CAppModel::XML_CONTROL_CONTROLFIELDS_ALL = _bstr_t(OLESTR("/ApplicationModel/Controls/Control/ControlField"));
const _bstr_t CAppModel::XML_CONTROL_CONTROLFIELDS_ATTR_NAME = _bstr_t(OLESTR("Name"));
const _bstr_t CAppModel::XML_CONTROL_CONTROLFIELDS_ATTR_MAPCONTEXT = _bstr_t(OLESTR("MapContext"));
const _bstr_t CAppModel::XML_CONTROL_CONTROLFIELDS_ATTR_MAPCONTEXTFIELD = _bstr_t(OLESTR("MapContextField"));
const _bstr_t CAppModel::XML_CONTROL_CONTROLFIELDS_ATTR_MAPPROPERTY = _bstr_t(OLESTR("MapProperty"));


const _bstr_t CAppModel::XML_INPUT_QUERY = _bstr_t(OLESTR("/ApplicationModel/Inputs/Input"));
const _bstr_t CAppModel::XML_INPUT_ATTR_NAME = _bstr_t(OLESTR("Name"));
const _bstr_t CAppModel::XML_INPUT_TEXT = _bstr_t(OLESTR("Text"));
const _bstr_t CAppModel::XML_INPUT_TEXT_ATTR_VALUE = _bstr_t(OLESTR("Value"));
const _bstr_t CAppModel::XML_INPUT_SEQ = _bstr_t(OLESTR("Sequence"));
const _bstr_t CAppModel::XML_INPUT_SEQ_SEQFIELD = _bstr_t(OLESTR("SequenceField"));
const _bstr_t CAppModel::XML_INPUT_SEQ_SEQFIELD_ATTR_NAME = _bstr_t(OLESTR("Name"));
const _bstr_t CAppModel::XML_INPUT_SEQ_SEQFIELD_ATTR_MAPPROPERTY = _bstr_t(OLESTR("MapProperty"));

const _bstr_t CAppModel::XML_INPUT_MACRO = _bstr_t(OLESTR("InputMacro"));
const _bstr_t CAppModel::XML_INPUT_MACROITEM = _bstr_t(OLESTR("InputMacro"));
const _bstr_t CAppModel::XML_INPUT_MACROITEM_ATTR_TYPE = _bstr_t(OLESTR("Type"));
const _bstr_t CAppModel::XML_INPUT_MACROITEM_ATTR_VALUE = _bstr_t(OLESTR("Value"));

LPCTSTR CAppModel::XML_HEX_START = _T("{{");
LPCTSTR CAppModel::XML_HEX_END = _T("}}");
const long CAppModel::XML_HEX_START_LEN = (long)_tcslen(XML_HEX_START);
const long CAppModel::XML_HEX_END_LEN = (long)_tcslen(XML_HEX_END);



/* I'm just keeping this around for reference until everything is changed to the 
new design and fully debugged. */
#if 0
BOOL CAppModel::ExecuteOnAction(MSXML2::IXMLDOMNodePtr spRoot, const CString & strEvent)
{
   Trace trace(_T("CAppModel::ExecuteOnAction"));
   trace(DCM_INFO, _T("In ExecuteOnAction"));
   try
   {
      //
      // TODO: rewrite this to perform actions in sequence
      //
      MSXML2::IXMLDOMNodeListPtr spList =  spRoot->childNodes;
      MSXML2::IXMLDOMNodePtr spNode = NULL;
      spList->reset();

      while( (spNode = spList->nextNode()) )
      {
         if( spNode->nodeName == XML_ON_SETPROP )
         {
            // get the property name
            static _bstr_t bstrAction = GetAttr(spNode, XML_ON_SETPROP_ATTR_ACTION);
            static _bstr_t bstrName;
            static _bstr_t bstrValue;

            trace(DCM_INFO) 
					<< _T("[") << strEvent.GetString()
					<< _T("] SetProperty Action='") << (LPCTSTR)bstrAction
					<< _T("' on Property '") << (LPCTSTR)bstrName
					<< _T("'.") << endl;

            // perform the appropriate property action
            if( bstrAction == XML_ON_SETPROP_ATTR_ACTION_ENUM_SET )
            {
               bstrName = GetAttr(spNode, XML_ON_SETPROP_ATTR_NAME);
               bstrValue = GetAttr(spNode, XML_ON_SETPROP_ATTR_VALUE);
               Utils.Properties().Set((LPCTSTR)bstrName, (LPCTSTR)bstrValue);
            }
            else if( bstrAction == XML_ON_SETPROP_ATTR_ACTION_ENUM_CONCATLEFT )
            {
               bstrName = GetAttr(spNode, XML_ON_SETPROP_ATTR_NAME);
               bstrValue = GetAttr(spNode, XML_ON_SETPROP_ATTR_VALUE);
               Utils.Properties().Set(
                  (LPCTSTR)bstrName, 
                  bstrValue + Utils.Properties().Get((LPCTSTR)bstrName).GetString());
            }
            else if( bstrAction == XML_ON_SETPROP_ATTR_ACTION_ENUM_CONCATRIGHT )
            {
               bstrName = GetAttr(spNode, XML_ON_SETPROP_ATTR_NAME);
               bstrValue = GetAttr(spNode, XML_ON_SETPROP_ATTR_VALUE);
               Utils.Properties().Set(
                  (LPCTSTR)bstrName, 
                  Utils.Properties().Get((LPCTSTR)bstrName) + (LPCTSTR)bstrValue
                  );
            }
            else
            {
               trace(DCM_WARNING)
						<< _T("[") << strEvent.GetString()
						<< _T("]: The SetProperty action '") << (LPCTSTR)bstrAction
						<< _T("' specified is not recognized. Ignoring SetProperty request.") << endl;
                  
               _ASSERT(FALSE);
            }
         }
         else if( spNode->nodeName == XML_ON_SENDINPUT)
         {
            // get the name of the input
            _bstr_t bstrInput = GetAttr(spNode, XML_ON_SENDINPUT_ATTR_NAME);

            if( !Utils.ACSHook().SendInput(bstrInput) )
            {
               trace(DCM_ERROR)
						<< _T("[") << strEvent.GetString()
						<< _T("] Failed to SendInput '") << (LPCTSTR)bstrInput
						<< _T("'.") << endl;
               _ASSERT(FALSE);
            }
            else
            {
               trace(DCM_INFO)
						<< _T("[") << strEvent.GetString()
						<< _T("] SendInput '") << (LPCTSTR)bstrInput
						<< _T("' was issued.") << endl;
            }				
         }
         else if( spNode->nodeName == XML_ON_SENDCONTROL )
         {
            // get the name of the control
            _bstr_t bstrControl = GetAttr(spNode, XML_ON_SENDCONTROL_ATTR_NAME);

            if( !Utils.ACSHook().SendControlToTB(bstrControl) )
            {
               trace(DCM_ERROR) 
						<< _T("[") << strEvent.GetString()
						<< _T("] Failed to send control '") << (LPCTSTR)bstrControl
						<< _T("'.") << endl;
               _ASSERT(FALSE);
            }
            else
            {
               trace(DCM_INFO) 
						<< _T("[") << strEvent.GetString()
						<< _T("] SendControlToTB '") << (LPCTSTR)bstrControl
						<< _T("' was issued.") << endl;
            }				
         }
         else if( spNode->nodeName == XML_ON_SENDCONTROLTOAPP )
         {
            // get the name of the control
            _bstr_t bstrControl = GetAttr(spNode, XML_ON_SENDCONTROLTOAPP_ATTR_NAME);

            if( !Utils.ACSHook().SendControlToApp(bstrControl) )
            {
               trace(DCM_ERROR) 
						<< _T("[") << strEvent.GetString()
						<< _T("] Failed to send control '") << (LPCTSTR)bstrControl
						<< _T("'.") << endl;
               _ASSERT(FALSE);
            }
            else
            {
               trace(DCM_INFO) 
						<< _T("[") << strEvent.GetString()
						<< _T("] SendControlToapp '") << (LPCTSTR)bstrControl
						<< _T("' was issued.") << endl;
            }				
         }
         else if( spNode->nodeName == XML_ON_RESETCONTEXT )
         {
            // get the context name
            _bstr_t bstrContext = GetAttr(spNode, XML_ON_RESETCONTEXT_ATTR_NAME);

            if( !Utils.Contexts().ResetContext(bstrContext) )
            {
               trace(DCM_ERROR) 
						<< _T("[") << strEvent.GetString()
						<< _T("] Failed to reset context '") << (LPCTSTR)bstrContext
						<< _T("'.") << endl;
               _ASSERT(FALSE);
            }
            else
            {
               trace(DCM_INFO) 
						<< _T("[") << strEvent.GetString()
						<< _T("] ResetContext '") << (LPCTSTR)bstrContext
						<< _T("' was issued.") << endl;
            }	
         }
         else if( spNode->nodeName == XML_ON_RESETALLCONTEXTS )
         {
            CSimpleArray<CString> mapEx;
            CSimpleArray<CString> mapReset;
            mapEx.RemoveAll();
            mapReset.RemoveAll();
            // get the context exclusion list if there is one
            MSXML2::IXMLDOMNodeListPtr spExList = spNode->selectNodes(XML_ON_RESETALLCONTEXTS_EXCLUDE_ATTR_ALL_REF);
            MSXML2::IXMLDOMNodePtr spEx = NULL;
            spExList->reset();
            while( (spEx = spExList->nextNode()) )
            {
               mapEx.Add((LPCTSTR)(_bstr_t)spEx->nodeValue);
            }

            if( mapEx.GetSize() )
            {
               // loop through all contexts and reset their content
               APPCONTEXTMAP & ctxMap = Utils.Contexts().GetContextMap();

               POSITION pos = ctxMap.GetStartPosition();
               APPCONTEXTMAPITEM * pcmi = NULL;
               while( pos && (pcmi = ctxMap.GetNext(pos)) )
               {
                  // set if not excluded
                  if( -1 == mapEx.Find(pcmi->m_key.GetString()) )
                  {
                     mapReset.Add(pcmi->m_key.GetString());
                  }
               }
               Utils.Contexts().ResetContexts(mapReset);
            }
            else
            {
               Utils.Contexts().ResetAllContexts();
            }
         }
         else if( spNode->nodeName == XML_ON_RESETCONTROL )
         {
            // get the control name
            _bstr_t bstrControl = GetAttr(spNode, XML_ON_RESETCONTROL_ATTR_NAME);

            if( !Utils.Controls().ResetControl(bstrControl) )
            {
               trace(DCM_ERROR) 
						<< _T("[") << strEvent.GetString()
						<< _T("] Failed to reset control '") << (LPCTSTR)bstrControl
						<< _T("'.") << endl;
               _ASSERT(FALSE);
            }
            else
            {
               trace(DCM_INFO) 
						<< _T("[") << strEvent.GetString()
						<< _T("] ResetControl '") << (LPCTSTR)bstrControl
						<< _T("' was issued.") << endl;
            }	
         }
      }
      return TRUE;
   }
   catch(_com_error & cex )
   {
      trace(DCM_ERROR) 
			<< _T("Failed to execute ") << strEvent.GetString()
			<< _T(". Error [0x") << std::hex << cex.Error() << std::dec
			<< _T("]: ") << cex.ErrorMessage()
			<< _T(".") << endl;
      return FALSE;
   }

   return TRUE;
}
#endif

