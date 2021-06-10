// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/SCOTAPP/KVParmList.cpp 4     1/05/10 8:39p Cu185008 $
/*
 * @(#)KeyValueParser.cpp    $Revision: 4 $ $Date: 1/05/10 8:39p $
 *
 * Copyright 2004, NCR Corporation.
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/SCOTAPP/KVParmList.cpp $
 * 
 * 4     1/05/10 8:39p Cu185008
 * Migrating Core to G2.
 * 
 * 18    11/27/07 5:37p Lp185019
 * SR629 (TAR 334800) - Resolve CISP Issue: RCM is tracing password by
 * default
 * 
 * 17    6/28/05 5:44p Dm185016
 * TAR 304xxx - Problem with commas in DESCRIPTION=.
 * 
 * 16    6/28/05 2:16p Dm185016
 * 
 * 15    5/31/05 1:26p Dm185016
 * Added encoding functionality.
 * 
 * 13    4/25/05 7:05p Dm185016
 * TAR 299068
 * 
 * 11    3/31/05 2:26p Dm185016
 * TAR 297138
 * 
 * 10    3/28/05 5:12p Dm185016
 * TAR 294050
 * 
 * 9     3/24/05 7:30p Dm185016
 * UNICODE
 *
 * 8     3/16/05 6:20p Dm185016
 * Fixed problem with key=
 *
 * 7     3/11/05 11:22a Dm185016
 * TAR 294050.
 *
 * 6     3/01/05 4:36p Dm185016
 *
 * 5     3/01/05 1:21p Dm185016
 * VS6 incompatibilities.
 *
 * 4     3/01/05 10:15a Dm185016
 * Fix for VS6 incompatibility.
 *
 * 3     2/28/05 3:30p Dm185016
 * lint
*/

//lint -esym(528, operator==*)

#include "stdafx.h"
#include "KVParmList.h"
#if !defined(_SCOT) &&  !defined(_SCOTSSF_)
    #include "xerror.h"
    #include "LoggableManager.h"
    #include <atlenc.h>
#else
    #include "StringComparator.h"
#endif
_kv_parser::CEncoderMap _kv_parser::m_encoders;

unsigned char _kv_parser::defaultTranslation[] =
{
    eof,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    ' ',0,String,0, 0,0,'&',0, 0,0,0,0, 0,'-','.',0,                                                        // '"'
    Number,Number,Number,Number, Number,Number,Number,Number, Number,Number,0,ValueSeparator, 0,0,0,0,      // '0'-'9'
    Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,Alpha,     // '@'-'O'
    Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,'[', Escape,']',0,Alpha,                // 'P'-'Z','_'
    0,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,Alpha,         // 'a'-'o'
    Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,0, 0,0,0,0,                         // 'p'-'z'
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
};


#ifdef _UNICODE
#define _TCHAR_MASK 0x0000FFFF
#else
#define _TCHAR_MASK 0x000000FF
#endif
StringType CTextEncoder::ToString(const StringType &sText)
{
    StringType sRetString = sText;
    return sRetString;
}


StringType CMaskedEncoder::ToString(const StringType & /* sText */ )
{
    return _T("****");
}


_kv_parser::_kv_parser(const _TCHAR *psSource,
                    _TCHAR cDelim,
                    _TCHAR cValueSeparator,
                    _TCHAR cSeparator,
                    _TCHAR cRecordSeparator)
    : m_psCurrent(const_cast<_TCHAR *>(psSource))
    , m_psSource(psSource)
    , m_nSrcLength(psSource == NULL ? 0 : _tcslen(psSource))
    , m_psBegin(NULL)
    , m_psEnd(NULL)
    , m_cDelim(cDelim)
    , m_cSeparator(cSeparator)
    , m_cValueSeparator(cValueSeparator)
    , m_cRecordSeparator(cRecordSeparator)
    , m_sKey(_T(""))
    , m_eCurrentValueType(Alpha)
{
    Init();
}


_kv_parser::_kv_parser(
                    _TCHAR cDelim,
                    _TCHAR cValueSeparator,
                    _TCHAR cSeparator,
                    _TCHAR cRecordSeparator)
    : m_psCurrent(NULL)
    , m_psSource(NULL)
    , m_nSrcLength(0)
    , m_psBegin(NULL)
    , m_psEnd(NULL)
    , m_cDelim(cDelim)
    , m_cSeparator(cSeparator)
    , m_cValueSeparator(cValueSeparator)
    , m_cRecordSeparator(cRecordSeparator)
{
    Init();
}


void _kv_parser::Init()
{
    m_nState = 1;
    m_bError = false;

    memcpy(m_translation, defaultTranslation, 256);
    m_translation[m_cDelim]           = Delimeter;
    m_translation[m_cSeparator]       = Separator;
    m_translation[m_cValueSeparator]  = ValueSeparator;
    m_translation[m_cRecordSeparator] = RecordSeparator;

    if (m_psCurrent != NULL)
        m_psEnd         = m_psCurrent + _tcslen(m_psCurrent);
    else
        m_psEnd         = NULL;

    if (m_encoders.empty())
        InitEncoders();

    Next();
}
static CTextEncoder textEncoder;
static CMaskedEncoder maskedEncoder;
void _kv_parser::InitEncoders()
{
    m_encoders[TEXT_ENCODING]      = &textEncoder;
    m_encoders[MASKED_ENCODING]    = &maskedEncoder;
}


_kv_parser::~_kv_parser()
{
    ClearValues();
}


void _kv_parser::ClearValues() throw()
{
    try
    {
        m_vValues.clear();
    }
    catch(...){}
}


void _kv_parser::SetSource(const _TCHAR *psSource)
{
    ClearValues();

    m_psSource = psSource;
    m_psCurrent = const_cast<_TCHAR *>(psSource);

    Init();
}


void _kv_parser::SetDelim(_TCHAR cDelim)
{
    m_translation[m_cDelim]   = 0;
    m_cDelim                = cDelim;
    m_translation[m_cDelim]   = Delimeter;
}


void _kv_parser::SetValueSeparator(_TCHAR cValueSeparator)
{
    m_translation[m_cValueSeparator]  = 0;
    m_cValueSeparator               = cValueSeparator;
    if (m_cValueSeparator != 0)
        m_translation[m_cValueSeparator]  = ValueSeparator;
}


void _kv_parser::SetKeySeparator(_TCHAR cSeparator)
{
    m_translation[m_cSeparator]   = 0;
    m_cSeparator                = cSeparator;
    m_translation[m_cSeparator]   = Separator;
}


void _kv_parser::SetRecordSeparator(_TCHAR cRecordSeparator)
{
    m_translation[m_cRecordSeparator] = 0;
    m_cRecordSeparator              = cRecordSeparator;
    if (m_cRecordSeparator != 0)
        m_translation[m_cRecordSeparator] = RecordSeparator;
}


const _TCHAR *_kv_parser::Key() const
{
    if (m_sKey.size() == 0)
        return NULL;

    return m_sKey.c_str();
}


const _TCHAR *_kv_parser::Value(int nIndex) const
{
    if (m_vValues.size() == 0)
        return NULL;

    unsigned int unIndex = (unsigned int)nIndex;

    if (unIndex >= (unsigned)m_vValues.size())
        return NULL;

    if (m_vValues[unIndex].sValue.size() == 0)
        return NULL;

    return m_vValues[unIndex].sValue.c_str();
}
const _TCHAR *_kv_parser::Encoding(int nIndex) const
{
    if (m_vValues.size() == 0)
        return NULL;

    unsigned int unIndex = (unsigned int)nIndex;

    if (unIndex >= (unsigned)m_vValues.size())
        return NULL;

    if (m_vValues[unIndex].sEncoding.size() == 0)
        return NULL;

    return m_vValues[unIndex].sEncoding.c_str();
}


int _kv_parser::GetValueCount() const
{
    return (int)m_vValues.size();
}


_kv_parser::CCharTypes _kv_parser::GetValueType(int nIndex) const
{
    if (m_vValues.size() == 0)
        return eof;

    unsigned unIndex = (unsigned)nIndex;

    if (unIndex >= (unsigned)m_vValues.size())
        return eof;

    return m_vValues[unIndex].eValueType;
}


bool _kv_parser::NextRecord()
{
    bool bTemp = m_bNewRecordDetected;
    m_bNewRecordDetected = false;
    return bTemp;
}


unsigned _kv_parser::GetTranslatedValue(unsigned c) const
{
    if (c >= 256)
        return Other;

    unsigned nxc = m_translation[(unsigned)c];

    // If it is not one of the special characters that we need, then
    if (nxc == 0)
    {
        nxc = Other;
    }

    return nxc;
}


void _kv_parser::Next()
{
    //lint --e{713}
    if (m_psCurrent == NULL)
        return;

    m_sKey                  = _T("");
    m_nState                = 1;
    m_bNewRecordDetected    = false;
    m_psBegin               = m_psCurrent;

    //m_sTemp.clear();
    m_sTemp                 = _T("");
    m_sString               = _T("");
    m_sEncoding             = _T("");

    ClearValues();

    if (m_psBegin >= m_psEnd)
        return;

    _TCHAR c;
    for ( ;m_nState != DONE; m_psCurrent++)
    {
        c = (_TCHAR)*m_psCurrent;
        m_nCharType = GetTranslatedValue(_TCHAR_MASK & c);

        switch (m_nState)
        {
            case 1  :  State1(c, m_nCharType); break;
            case 2  :  State2(c, m_nCharType); break;
            case 3  :  State3(c, m_nCharType); break;
            case 4  :  State4(c, m_nCharType); break;
            case 5  :  State5(c, m_nCharType); break;
            case 6  :  State6(c, m_nCharType); break;
            case 7  :  State7(c, m_nCharType); break;
            case 8  :  State8(c, m_nCharType); break;
            case 9  :  State9(c, m_nCharType); break;
            case 10 :  State10(c, m_nCharType); break;
            case 11 :  State11(c, m_nCharType); break;
            case 12 :  State12(c, m_nCharType); break;
            case 13 :  State13(c, m_nCharType); break;
            case 14 :  State14(c, m_nCharType); break;
            default : break;
        }
    }
}


void _kv_parser::State1(_TCHAR c, int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case Alpha :
            m_sTemp += c;
            m_nState = 2;
            break;
        default :
            m_bError = true;
            m_nState = DONE;
            break;
    }
}


void _kv_parser::State2(_TCHAR c, int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case Alpha :
        case Number :
        case '.' :
        case '-' :
            m_sTemp += c;
            break;
        case Delimeter :
            SaveKey();
            m_nState = 3;
            break;
        case '[' :
            SaveKey();
            m_nState = 12;
            break;
        default :
            m_bError = true;
            m_nState = DONE;
            break;
    }
}


void _kv_parser::State3(_TCHAR c, int nCharType)
{
//  IMODTRACE_DEVELOP();
    m_eCurrentValueType = (CCharTypes)nCharType;
    switch (nCharType)
    {
        case String :
            m_sString += c;
            m_nState = 6;
            break;
        case Separator :
            SaveValue();
            m_nState = DONE;
            break;
        case '&' :
            m_nState = 9;
            break;
        case eof : 
            m_nState = DONE;
            break;
        case Alpha :
        case Number :
        case Other :
        default :
            m_sTemp += c;
            m_nState = 4;
            break;
    }
}


void _kv_parser::State4(_TCHAR c, int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case Alpha :
        case Number :
        case Other :
        case '.' :
        case ' ' :
            m_sTemp += c;
            break;
        case ValueSeparator :
            SaveValue();
            m_nState = 5;
            break;
        case '&' :
            m_nState = 9;
            break;
        case Separator :
            SaveValue();
            m_nState = DONE;
            break;
        case RecordSeparator :
            SaveValue();
            m_nState = DONE;
            m_bNewRecordDetected = true;
            break;
        case eof :
            SaveValue();
            m_nState = DONE;
            break;
        default :
            m_sTemp += c;
            break;
    }
}


void _kv_parser::State5(_TCHAR c, int nCharType)
{
//  IMODTRACE_DEVELOP();
    m_eCurrentValueType = (CCharTypes)nCharType;
    switch (nCharType)
    {
        case Alpha :
        case Number :
        case Other :
            m_sTemp += c;
            m_nState = 4;
            break;
        case String :
            m_sTemp += c;
            m_nState = 6;
            break;
        default :
            m_nState = DONE;
            m_bError = true;
            break;
    }
}


void _kv_parser::State6(_TCHAR c, int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case Escape :
            m_sString += c;
            m_nState = 7;
            break;
        case String :
            m_sString += c;
            m_nState = 8;
            break;
        case eof :
            m_bError = true;
            m_nState = DONE;
            break;
        default :
            m_sString += c;
            break;
    }
}


void _kv_parser::State7(_TCHAR c, int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case String :
            m_sString += c;
            m_nState = 6;
            break;
        case eof : 
            m_bError = true;
            m_nState = DONE;
            break;
        default :
            m_sString += c;
            m_nState = 6;
            break;
    }
}


void _kv_parser::State8(_TCHAR c, int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case Separator :
            m_sTemp = m_sString;
            SaveValue();
            m_nState = DONE;
            break;
        case ValueSeparator :
            m_sTemp = m_sString;
            SaveValue();
            m_nState = 5;
            break;
        case eof :
            m_sTemp = m_sString;
            SaveValue();
            m_nState = DONE;
            break;
        default :
            m_sTemp += m_sString;
            m_sTemp += c;
            m_sString = _T("");
            m_nState = 3;
            break;
    }
}


void _kv_parser::State9(_TCHAR c, int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case Alpha :
        case Number :
            m_sEntity += c;
            m_nState = 11;
            break;
        case _T('#') :
            m_nState = 10;
            break;
        case Separator :
            if (SaveEntity())
                m_nState = 4;
            else
                m_nState = DONE;
            break;
        default :
            if (!SaveEntity())
            {
                m_nState = 4;
                m_sTemp += c;
            }
            break;
    }
}


void _kv_parser::State10(_TCHAR c, int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case Alpha :
        case Number :
            m_sEntity += c;
            break;
        case Separator :
            SaveNumericEntity();
            m_nState = 4;
            break;
        default :
            m_bError = true;
            m_nState = DONE;
            break;
    }
}


void _kv_parser::State11(_TCHAR c, int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case Alpha :
        case Number :
            m_sEntity += c;
            break;
        case Separator :
            if (SaveEntity())
                m_nState = 4;
            else
                m_nState = DONE;
            break;
        default :
            if (!SaveEntity())
            {
                m_nState = 4;
                m_sTemp += c;
            }
            break;
    }
}


void _kv_parser::State12(_TCHAR c, int nCharType)
{
    switch (nCharType)
    {
        case Alpha :
            m_sEncoding += c;
            m_nState = 13;
            break;
        default :
            m_bError = true;
            m_nState = DONE;
            break;
    }
}


void _kv_parser::State13(_TCHAR c, int nCharType)
{
    switch (nCharType)
    {
        case Alpha :
        case Number :
        case '.' :
        case '-' :
            m_sEncoding += c;
            break;
        case ']' :
            SaveEncoding();
            m_nState = 14;
            break;
        default :
            m_bError = true;
            m_nState = DONE;
            break;
    }
}


void _kv_parser::State14(_TCHAR, int nCharType)
{
    switch (nCharType)
    {
        case Delimeter :
            m_nState = 3;
            break;
        default :
            m_bError = true;
            m_nState = DONE;
            break;
    }
}

void _kv_parser::SaveKey()
{
    m_sKey = m_sTemp;

    //m_sTemp.clear();
    m_sTemp                 = _T("");
}
void _kv_parser::SaveEncoding()
{
    CEncoderMap::iterator iter = m_encoders.find(m_sEncoding);
}


void _kv_parser::SaveValue()
{
    //lint --e{774}     Boolean message is incorrect
    _ASSERTE(m_psCurrent!=NULL);

    CValue value;
    value.eValueType    = m_eCurrentValueType;
    value.sValue        = m_sTemp;
    value.sEncoding     = m_sEncoding;

    m_vValues.push_back(value);

    m_psBegin=m_psCurrent+1;

    //m_sTemp.clear();
    m_sTemp                 = _T("");
}


bool _kv_parser::SaveEntity()
{
    bool bRetValue = true;

    if (m_sEntity == ENTITY_SEMI)
        m_sTemp += ';';
    else if (m_sEntity == ENTITY_EQ)
        m_sTemp += '=';
    else if (m_sEntity == ENTITY_AMP)
        m_sTemp += '&';
    else if (m_sEntity == ENTITY_COMMA)
        m_sTemp += ',';
    else if (m_sEntity == ENTITY_LINEFEED)
        m_sTemp += '\n';

    // Not a valid entity, so treat it as text
    else
    {
        m_sTemp += '&';
        m_sTemp += m_sEntity;
        bRetValue = false;
    }

    //m_sEntity.clear();
    m_sEntity                 = _T("");

    return bRetValue;
}


void _kv_parser::SaveNumericEntity()
{
    //TODO:  Need to implement numeric entities
    m_sTemp += '0';

    //m_sEntity.clear();
    m_sEntity                 = _T("");
}


CKeyValueParmList::CKeyValueParmList(const _TCHAR *psSource,
                    _TCHAR cDelim,
                    _TCHAR cValueSeparator,
                    _TCHAR cSeparator,
                    _TCHAR cRecordSeparator)
    : m_psSource(psSource)
    , m_nSrcLength(psSource == NULL ? 0 : _tcslen(psSource))
    , m_bNeedSeparator(psSource != NULL)
    , m_iterEnd(NULL, 0)
{
    if (m_nSrcLength != 0)
        m_sValue = psSource;
    SetDelim(cDelim);
    SetKeySeparator(cSeparator);
    SetValueSeparator(cValueSeparator);
    SetRecordSeparator(cRecordSeparator);
}


CKeyValueParmList::CKeyValueParmList(
                    _TCHAR cDelim,
                    _TCHAR cValueSeparator,
                    _TCHAR cSeparator,
                    _TCHAR cRecordSeparator)
    : m_psSource(NULL)
    , m_nSrcLength(0)
    , m_sValue(_T(""))
    , m_bNeedSeparator(false)
    , m_iterEnd(NULL, 0)
{
    SetDelim(cDelim);
    SetKeySeparator(cSeparator);
    SetValueSeparator(cValueSeparator);
    SetRecordSeparator(cRecordSeparator);
}




CKeyValueParmList::CKeyValueParmList(const CKeyValueParmList &theOther)
    : m_psSource(theOther.m_psSource)       //lint !e1554   Want copy of original ptr.  We do not own it.
    , m_nSrcLength(theOther.m_nSrcLength)
    , m_sValue(theOther.m_sValue)
    , m_bNeedSeparator(theOther.m_bNeedSeparator)
    , m_iterEnd(NULL, 0)
{
    SetDelim(theOther.m_cDelim);
    SetKeySeparator(theOther.m_cSeparator);
    SetValueSeparator(theOther.m_cValueSeparator);
    SetRecordSeparator(theOther.m_cRecordSeparator);
}
//
//
//CKeyValueParmList& CKeyValueParmList::operator=( const CKeyValueParmList&theOther )
//{
//
//    if (&theOther == this)
//        return *this;
//
//    m_psSource          = theOther.m_psSource;
//    m_nSrcLength        = theOther.m_nSrcLength;
//    m_iterBegin         = theOther.m_iterBegin;
//    m_iterEnd           = theOther.m_iterEnd;
//    m_bNeedSeparator    = theOther.m_bNeedSeparator;
//    m_sValue            = theOther.m_sValue;
//
//    SetDelim(theOther.m_cDelim);
//    SetKeySeparator(theOther.m_cSeparator);
//    SetValueSeparator(theOther.m_cValueSeparator);
//    SetRecordSeparator(theOther.m_cRecordSeparator);
//
//    return *this;
//}


CKeyValueParmList::~CKeyValueParmList()
{
}


//lint -esym(1762, CKeyValueParmList::SetSource)
void CKeyValueParmList::SetSource(const _TCHAR *)
{
    // TODO:  Need to implement this method
}


void CKeyValueParmList::SetDelim(_TCHAR cDelim)
{
    m_cDelim = cDelim;
}


void CKeyValueParmList::SetValueSeparator(_TCHAR cValueSeparator)
{
    m_cValueSeparator = cValueSeparator;
}


void CKeyValueParmList::SetKeySeparator(_TCHAR cSeparator)
{
    m_cSeparator = cSeparator;
}


void CKeyValueParmList::SetRecordSeparator(_TCHAR cRecordSeparator)
{
    m_cRecordSeparator = cRecordSeparator;
}
CKeyValueParmList::iterator CKeyValueParmList::begin() 
{ 
    return CKeyValueParmList::iterator(m_sValue.c_str()
                                     , m_cDelim
                                     , m_cValueSeparator
                                     , m_cSeparator
                                     , m_cRecordSeparator); 
}


StringType CKeyValueParmList::EscapeText(const StringType &text)
{
    StringType retValue;
    _TCHAR c;
    for (unsigned i=0; i<text.size(); i++)
    {
        c = text[i];
        switch(c)
        {
            case ';' :
                retValue += _T("&") ENTITY_SEMI _T(";");
                break;
            case '=' :
                retValue += _T("&") ENTITY_EQ _T(";");
                break;
            case '&' :
                retValue += _T("&") ENTITY_AMP _T(";");
                break;
            case ',' :
                retValue += _T("&") ENTITY_COMMA _T(";");
                break;
            case 0x0a :
                retValue += _T("&") ENTITY_LINEFEED _T(";");
                break;
            default :
                retValue += c;
                break;
        }
    }

    return retValue;
}


StringType CKeyValueParmList::EscapeValue(const StringType &text)
{
    StringType retValue;
    _TCHAR c;
    for (unsigned i=0; i<text.size(); i++)
    {
        c = text[i];
        switch(c)
        {
            case 0x0a :
                retValue += _T("&") ENTITY_LINEFEED _T(";");
                break;
            default :
                retValue += c;
                break;
        }
    }

    return retValue;
}


StringType CKeyValueParmList::EscapeValueSeparator(const StringType &text)
{
    StringType retValue;
    _TCHAR c;
    for (unsigned i=0; i<text.size(); i++)
    {
        c = text[i];
        switch(c)
        {
            case _T(',') :
                retValue += _T("&") ENTITY_COMMA _T(";");
                break;
            default :
                retValue += c;
                break;
        }
    }

    return retValue;
}


void CKeyValueParmList::NewKVPair(const _TCHAR *pKey, int nValue, const StringType & sEncoding)
{
    _TCHAR buffer[20];
    _itot( nValue, buffer, 10 );

    NewKVPair(pKey, buffer, sEncoding.c_str());
}


void CKeyValueParmList::NewKVPair(const _TCHAR *pcszKey, const _TCHAR *pcszValue, const _TCHAR * pcszEncoding)
{
    if (m_bNeedSeparator)
    {
        m_sValue += m_cSeparator;
    }

    m_sValue += pcszKey;
    if (pcszEncoding != NULL)
    {
        m_sValue += _T('[');
        m_sValue += pcszEncoding;
        m_sValue += _T(']');
    }
    m_sValue += m_cDelim;
	m_sValue += pcszValue;
    m_bNeedSeparator = true;
}


void CKeyValueParmList::NewValue(const _TCHAR *pValue)
{
    NewValue(StringType(pValue));
}


void CKeyValueParmList::NewValue(const StringType &value)
{
    m_sValue += m_cValueSeparator;
    m_sValue += value;
}


void CKeyValueParmList::NewRecord(const _TCHAR *pRecord)
{
    if (pRecord != NULL)
    {
        if (!m_sValue.empty())
            m_sValue += m_cRecordSeparator;
        m_sValue += pRecord;

        m_bNeedSeparator = true;
    }
    else
    {
        m_sValue += m_cRecordSeparator;

        m_bNeedSeparator = false;
    }
}


bool CKeyValueParmList::empty() const
    { return m_sValue.empty(); }


void CKeyValueParmList::Append(const CKeyValueParmList &other)
{
    if (m_bNeedSeparator)
    {
        m_sValue += m_cSeparator;
    }

    m_sValue += (const _TCHAR *)other;
    m_bNeedSeparator = true;
}


void CKeyValueParmList::Clear()
{
    m_sValue = _T("");
    m_bNeedSeparator = false;
}


CKeyValueParmList::operator const _TCHAR * ( ) const
{
    return m_sValue.c_str();
}


StringType CKeyValueParmList::ToString()
{
    StringType sRetValue;

    iterator iter = begin();
    if (iter != end())
    {
        sRetValue += iter.ToString();
        iter++;
        for (;iter != end(); iter++)
        {
            sRetValue += m_cSeparator;
            sRetValue += iter.ToString();
        }
    }

    return sRetValue;
}


size_t CKeyValueParmList::size( ) const
{
    return m_sValue.size();
}


/*
StringType base64Codec::encode(const _TCHAR *pszPlainText, size_t len)
{
    StringType sResult;

    CT2A ascii(pszPlainText);

    size_t length = len * sizeof(_TCHAR);

    int b64Length = (int)(length * sizeof(_TCHAR) * 1.5);
    IStringBuffer buffer(b64Length);

    if (TRUE == Base64Encode((const BYTE *)pszPlainText, (int)length, (LPSTR)buffer, &b64Length))
        sResult = (const _TCHAR *)buffer;

    return sResult;
}


StringType base64Codec::decode(const char *pszCipher, size_t length)
{
    int b64Length = (int)length;
    IStringBuffer buffer((ULONG)length);
    StringType sResult;

    if (TRUE == Base64Decode(pszCipher, (int)length, (BYTE *)(LPSTR)buffer, &b64Length))
        //sResult = buffer.operator const unsigned short *();
        sResult = buffer.operator const _TCHAR *();

    return sResult;
}
*/
//
//
//_base64Codec &base64Codec::instance()
//{
//    if (pBase64Codec == NULL)
//        pBase64Codec = new base64Codec;
//
//    return *pBase64Codec;
//}
//
//
//_base64Codec *base64Codec::pBase64Codec = NULL;


StringType hexCodec::encode(const _TCHAR *pszPlainText, size_t length)
{
    StringType sResult(pszPlainText, length);

    return sResult;
}


StringType hexCodec::decode(const _TCHAR *pszCipher, size_t length)
{
    StringType sResult(pszCipher, length);

    return sResult;
}
//
//
//_hexCodec &hexCodec::instance()
//{
//    if (pHexCodec == NULL)
//        pHexCodec = new hexCodec;
//
//    return *pHexCodec;
//}
//
//
//_hexCodec *hexCodec::pHexCodec = NULL;
