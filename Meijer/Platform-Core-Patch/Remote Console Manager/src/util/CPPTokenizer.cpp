// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/CPPTokenizer.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)CPPTokenizer.cpp    $Revision: 1 $ $Date: 6/20/08 1:13p $
 *
 * Copyright 2004, NCR Corporation.
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with msi.
 */
//nolint --e{ 545 }
//lint --e{ 613, 1550 }     Null pointer references not possible

#include "stdafx.h"
#include "CPPTokenizer.h"


CCPPTokenizer::CKeyMap CCPPTokenizer::keys;


_TCHAR CCPPTokenizer::defaultTranslation[] = 
{
    eof,0,0,0, 0,0,0,0, 0,0,eol,0, 0,eol,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,'!','"','#', Alpha,'%','&','\'', '(',')','*','+', ',','-','.','/',                                                                // '"'
    Digit,Digit,Digit,Digit, Digit,Digit,Digit,Digit, Digit,Digit,':',';', '<','=','>','?',                   // '0'-'9'
    Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,Alpha,     // '@'-'O'
    Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,'[', '\\',']','^',Alpha,                // 'P'-'Z','_'
    0,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,Alpha,         // 'a'-'o'
    Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,Alpha, Alpha,Alpha,Alpha,'{', '|','}','~',0,                         // 'p'-'z'
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
};


CCPPTokenizer::CCPPTokenizer(const _TCHAR *psSource)
    : m_psSource(psSource)
    , m_psCurrent(const_cast<_TCHAR *>(psSource))
//  , m_psCurrent(m_psSource)
    , m_nState(1)
    , m_nLineNo(1)
    , m_bError(false)
    , m_pLoggable(NULL)
{
    Init();
}


CCPPTokenizer::CCPPTokenizer()
    : m_psSource(NULL)
    , m_psCurrent(NULL)
//  , m_psCurrent(m_psSource)
    , m_nState(1)
    , m_nLineNo(1)
    , m_bError(false)
    , m_pLoggable(NULL)
{
    Init();
}


void CCPPTokenizer::Init()
{
    memcpy(translation, defaultTranslation, 256);

    InitKeys();

    Next();
}


void CCPPTokenizer::InitKeys() throw()
{
    if (keys.empty())
    {
        keys[_T("asm")]     = Asm;
        keys[_T("auto")]     = Auto;
        keys[_T("bool")]     = Bool;
        keys[_T("break")]     = Break;
        keys[_T("case")]     = Case;
        keys[_T("catch")]     = Catch;
        keys[_T("char")]     = Char;
        keys[_T("class")]     = Class;
        keys[_T("const")]     = Const;
        keys[_T("const_cast")]     = Const_cast;
        keys[_T("continue")]     = Continue;
        keys[_T("default")]     = Default;
        keys[_T("delete")]     = Delete;
        keys[_T("do")]     = Do;
        keys[_T("double")]     = Double;
        keys[_T("dynamic_cast")]     = Dynamic_cast;
        keys[_T("else")]     = Else;
        keys[_T("enum")]     = Enum;
        keys[_T("explicit")]     = Explicit;
        keys[_T("export")]     = Export;
        keys[_T("extern")]     = Extern;
        keys[_T("false")]     = False;
        keys[_T("float")]     = Float;
        keys[_T("for")]     = For;
        keys[_T("friend")]     = Friend;
        keys[_T("goto")]     = Goto;
        keys[_T("if")]     = If;
        keys[_T("inline")]     = Inline;
        keys[_T("int")]     = Int;
        keys[_T("long")]     = Long;
        keys[_T("mutable")]     = Mutable;
        keys[_T("namespace")]     = Namespace;
        keys[_T("new")]     = New;
        keys[_T("operator")]     = Operator;
        keys[_T("pascal")]     = Pascal;
        keys[_T("private")]     = Private;
        keys[_T("protected")]     = Protected;
        keys[_T("public")]     = Public;
        keys[_T("register")]     = Register;
        keys[_T("reinterpret_cast")]     = Reinterpret_cast;
        keys[_T("return")]     = Return;  
        keys[_T("short")]     = Short;
        keys[_T("signed")]     = Signed;
        keys[_T("sizeof")]     = Sizeof;
        keys[_T("static")]     = Static;
        keys[_T("static_cast")]     = Static_cast;
        keys[_T("struct")]     = Struct;
        keys[_T("switch")]     = Switch;
        keys[_T("template")]     = Template; 
        keys[_T("this")]     = This;
        keys[_T("throw")]     = Throw;
        keys[_T("true")]     = True;
        keys[_T("try")]     = Try;
        keys[_T("typedef")]     = Typedef;
        keys[_T("typeid")]     = Typeid;
        keys[_T("typename")]     = Typename;
        keys[_T("union")]     = Union;
        keys[_T("unsigned")]     = Unsigned;
        keys[_T("using")]     = Using;
        keys[_T("virtual")]     = Virtual;
        keys[_T("void")]     = Void;
        keys[_T("volatile")]     = Volatile;
        keys[_T("while")]     = While;
        keys[_T("wchar_t")]     = Wchar_t;
    }
}


CCPPTokenizer::~CCPPTokenizer()
{
}


int CCPPTokenizer::GetLineNo() const 
{ 
    return m_nLineNo; 
}


void CCPPTokenizer::SetSource(const _TCHAR *psSource)
{
    m_psSource = psSource;
    m_psCurrent = const_cast<_TCHAR *>(psSource);

    Init();
}


_TCHAR CCPPTokenizer::NextChar()
{
    _TCHAR c = *m_psCurrent;

    if(c >= 256)
    {
        m_nCharType = Other;
    }
    else
    {
        m_nCharType = translation[c];
        switch (m_nCharType)
        {
            case 0 :
                m_nCharType = Other;
                break;
            case eol :
                m_nLineNo++;
                break;
            case eof :
                c = eof;
                break;
            default :
                break;
        }
    }

    return c;
}


void CCPPTokenizer::Next()
{
//  IMODTRACE_DEVELOP();

    if (m_psSource == NULL)
        return;
    if (m_psCurrent == NULL)
        return;

    m_sToken.clear();
    m_eTokenType=None;
    m_nState        = 1;

    m_psBegin       = m_psCurrent;
    _TCHAR c;
    for ( ;m_nState != DONE; m_psCurrent++)
    {
        if (m_nState == 1)
            SkipWhitespace();

        c = NextChar();

        //if (loggable().IsDebug())
        //    ITRACE_DEVELOP(_T("c='") + IString(c) +
        //                   _T("'(") + IString((long)c) + _T(").  nCharType=") 
        //                   + IString(m_nCharType) + 
        //                   _T(".  m_nState=") + IString(m_nState));
        switch (m_nState)
        {
            case 1 :  State1(m_nCharType); break;
            case 2 :  State2(m_nCharType); break;
            case 3 :  State3(m_nCharType); break;
            case 4 :  State4(m_nCharType); break;
            case 5 :  State5(m_nCharType); break;
            case 6 :  State6(m_nCharType); break;
            case 7 :  State7(m_nCharType); break;
            case 8 :  State8(m_nCharType); break;
            case 9 :  State9(m_nCharType); break;
            case 10 :  State10(m_nCharType); break;
            case 11 :  State11(m_nCharType); break;
            case 12 :  State12(m_nCharType); break;
            case 13 :  State13(m_nCharType); break;
            case 14 :  State14(m_nCharType); break;
            case 15 :  State15(m_nCharType); break;
            case 16 :  State16(m_nCharType); break;
            case 17 :  State17(m_nCharType); break;
            case 18 :  State18(m_nCharType); break;
            case 19 :  State19(m_nCharType); break;
            case 20 :  State20(m_nCharType); break;
            case 21 :  State21(m_nCharType); break;
//          case 22 :  State22(m_nCharType); break;
            case 23 :  State23(m_nCharType); break;
            case 24 :  State24(m_nCharType); break;
            case 25 :  State25(m_nCharType); break;
            case 26 :  State26(m_nCharType); break;
            case 27 :  State27(m_nCharType); break;
            case 28 :  State28(m_nCharType); break;
            case 29 :  State29(m_nCharType); break;
            case 30 :  State30(m_nCharType); break;
            case 31 :  State31(m_nCharType); break;
            case 32 :  State32(m_nCharType); break;
            default :
                _ASSERTE(false); // lint -e506 -e774 -e717
        }

        if (m_nState != DONE)
            m_sToken += c;
    }

    if ( m_eTokenType == None )
        m_sToken = _T("");
}


void CCPPTokenizer::SkipWhitespace()
{
    _ASSERTE(m_psCurrent!=NULL);
    _TCHAR c = *m_psCurrent;
    bool done = false;
    _TCHAR crlf = 0;
    while (!done)
    {
        switch (c)
        {
            case ' ' :
            case '\t' :
                m_psCurrent++;
                c = *m_psCurrent;
                break;
            case 13 :
            case 10 :
                if (crlf ==  0)
                {
                    crlf = c;
                    m_psCurrent++;
                    c = *m_psCurrent;
                }
                else if (crlf == c)
                {
                    m_psCurrent--;
                    done = true;
                }
                else
                    done = true;
                break;
            default:
                done = true;
        }
    }

    m_psBegin = m_psCurrent;
}


void CCPPTokenizer::State1(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '<' : 
            m_nState = 2;
            break;
        case '>' : 
            m_nState = 3;
            break;
        case '!' : 
            m_nState = 4;
            break;
        case '/' : 
            m_nState = 5;
            break;
        case '^' : 
            m_nState = 8;
            break;
        case '*' : 
            m_nState = 9;
            break;
        case '_' :
        case Alpha :
            m_nState = 10;
            break;
        case Digit :
            m_nState = 11;
            break;
        case '{' : 
            SaveToken(LeftBrace);
            break;
        case '}' : 
            SaveToken(RightBrace);
            break;
        case '?' : 
            SaveToken(Conditional);
            break;
        case ',' : 
            SaveToken(Comma);
            break;
        case '[' : 
            SaveToken(LeftBracket);
            break;
        case ']' : 
            SaveToken(RightBracket);
            break;
        case '(' : 
            SaveToken(LeftParen);
            break;
        case ')' : 
            SaveToken(RightParen);
            break;
        case ';' : 
            SaveToken(Semicolon);
            break;
        case '~' : 
            SaveToken(Complement);
            break;
        case '#' : 
            SaveToken(Preprocessor);
            break;
        case eol : 
            m_nState = 7;
            break;
        case '-' : 
            m_nState = 13;
            break;
        case '%' : 
            m_nState = 20;
            break;
        case '&' : 
            m_nState = 14;
            break;
        case '=' : 
            m_nState = 17;
            break;
        case '|' : 
            m_nState = 18;
            break;
        case '+' : 
            m_nState = 19;
            break;
        case ':' : 
            m_nState = 21;
            break;
        case '.' : 
            m_nState = 24;
            break;
        case '"' : 
            m_nState = 25;
            break;
        case '\'' : 
            m_nState = 27;
            break;
        case '\\' : 
            m_nState = 32;
            break;
        case eof : 
            m_nState = DONE;
            break;
        default :
            m_bError = true;
            m_nState = DONE;
            break;
    }
}


void CCPPTokenizer::State2(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '=' : 
            SaveToken(LtEq);
            break;
        case '>' : 
            SaveToken(LtGt_NotEq);
            break;
        case '<' :
            m_nState = 16;
            break;
        default :
            SaveToken(Lt, true);
            break;
    }
}


void CCPPTokenizer::State3(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '>' : 
            m_nState = 15;
            break;
        case '=' : 
            SaveToken(GtEq);
            break;
        default :
            SaveToken(Gt, true);
            break;
    }
}


void CCPPTokenizer::State4(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '=' : 
            SaveToken(NotEq);
            break;
        default :
            SaveToken(LogicalNot, true);
            break;
    }
}


void CCPPTokenizer::State5(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '=' : 
            SaveToken(DivideAssign);
            break;
        case '/' : 
            m_nState = 6;
            break;
        case '*' : 
            m_nState = 30;
            break;
        default :
            SaveToken(Divide, true);
            break;
    }
}


void CCPPTokenizer::State6(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '/' :
            SaveEolToken(DocComment);
            break;
        default :
            SaveEolToken(LineEndComment);
            break;
    }
}


void CCPPTokenizer::State7(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case eol : 
            SaveToken(EndOfLine);
            break;
        default :
            SaveToken(EndOfLine, true);
            break;
    }
}


void CCPPTokenizer::State8(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '=' : 
            SaveToken(ExclusiveOrAssign);
            break;
        default :
            SaveToken(ExclusiveOr, true);
            break;
    }
}


void CCPPTokenizer::State9(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '=' : 
            SaveToken(MultiplyAssign);
            break;
        default :
            SaveToken(Multiply, true);
            break;
    }
}


void CCPPTokenizer::State10(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case Alpha : 
        case Digit : 
        case '@' : 
        case '_' : 
            break;
        default :
            SaveToken(Identifier, true);
            break;
    }
}


void CCPPTokenizer::State11(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case Digit : 
            break;
        case '.' : 
            m_nState = 12;
            break;
        default :
            SaveToken(Integer, true);
            break;
    }
}


void CCPPTokenizer::State12(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case Digit : 
            break;
        default :
            SaveToken(Float, true);
            break;
    }
}


void CCPPTokenizer::State13(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case Digit : 
            m_nState = 11;
            break;
        case '-' : 
            SaveToken(Decrement);
            break;
        case '=' : 
            SaveToken(SubtractAssign);
            break;
        case '>' : 
            m_nState = 23;
            break;
        default :
            SaveToken(Subtract, true);
            break;
    }
}


void CCPPTokenizer::State14(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '&' : 
            SaveToken(LogicalAnd);
            break;
        case '=' : 
            SaveToken(BitwiseAndAssign);
            break;
        default :
            SaveToken(BitwiseAnd, true);
            break;
    }
}


void CCPPTokenizer::State15(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '=' : 
            SaveToken(ShiftRightAssign);
            break;
        default :
            SaveToken(ShiftRight, true);
            break;
    }
}


void CCPPTokenizer::State16(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '=' : 
            SaveToken(ShiftLeftAssign);
            break;
        default :
            SaveToken(ShiftLeft, true);
            break;
    }
}


void CCPPTokenizer::State17(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '=' : 
            SaveToken(Equals);
            break;
        default :
            SaveToken(Assignment, true);
            break;
    }
}


void CCPPTokenizer::State18(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '|' : 
            SaveToken(LogicalOr);
            break;
        case '=' : 
            SaveToken(BitwiseOrAssign);
            break;
        default :
            SaveToken(BitwiseOr, true);
            break;
    }
}


void CCPPTokenizer::State19(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '+' : 
            SaveToken(Increment);
            break;
        case '=' : 
            SaveToken(AddAssign);
            break;
        default :
            SaveToken(Add, true);
            break;
    }
}


void CCPPTokenizer::State20(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '=' : 
            SaveToken(ModuloAssign);
            break;
        default :
            SaveToken(Modulo, true);
            break;
    }
}


void CCPPTokenizer::State21(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case ':' : 
            SaveToken(ScopeResolution);
            break;
        default :
            SaveToken(Colon, true);
            break;
    }
}


void CCPPTokenizer::State23(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '*' : 
            SaveToken(PtrToMemberPtr);
            break;
        default :
            SaveToken(Indirection, true);
            break;
    }
}


void CCPPTokenizer::State24(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '*' : 
            SaveToken(PtrToMemberObject);
            break;
        default :
            SaveToken(MemberSelection, true);
            break;
    }
}


void CCPPTokenizer::State25(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '"' : 
            SaveToken(String);
            break;
        case '\\' : 
            m_nState = 26;
            break;
        default :
            break;
    }
}


void CCPPTokenizer::State26(int)
{
//  IMODTRACE_DEVELOP();
    m_nState = 25;
}


void CCPPTokenizer::State27(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '\\' : 
            m_nState = 29;
            break;
        default :
            m_nState = 28;
            break;
    }
}


void CCPPTokenizer::State28(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '\'' : 
            SaveToken(Character);
            break;
        default :
            break;
    }
}


void CCPPTokenizer::State29(int)
{
//  IMODTRACE_DEVELOP();
    m_nState = 28;
}


void CCPPTokenizer::State30(int nCharType)
{
    switch (nCharType)
    {
        case '*' : 
            m_nState = 31;
            break;
        case eof : 
            m_nState = DONE;
            m_bError = true;
            break;
        default :
            break;
    }
}


void CCPPTokenizer::State31(int nCharType)
{
//  IMODTRACE_DEVELOP();
    switch (nCharType)
    {
        case '/' : 
            SaveToken(Comment);
            break;
        default :
            m_nState = 30;
            break;
    }
}


void CCPPTokenizer::State32(int nCharType)
{
//  IMODTRACE_DEVELOP();
    _ASSERTE(m_psCurrent!=NULL);
    if (m_psCurrent == NULL)        //lint !e774 Needed for Production build
        return;

    _TCHAR c1 = *m_psCurrent;
    switch (nCharType)
    {
        case eol : 
            while ((c1 == 10) || (c1 == 13))
            {
                m_psCurrent--;
                c1 = *m_psCurrent;
            }
            SaveToken(Continuation);
            break;
        default :
            m_nState = 30;
            break;
    }
}


void CCPPTokenizer::SaveToken(CTokenTypes eTokenType, bool bPushback)
{
//  IMODTRACE_DEVELOP();

    if (!bPushback)
    {
        m_sToken += *m_psCurrent;
        m_psCurrent++;
    }
    m_psBegin=m_psCurrent;

    if (eTokenType == Identifier)
    {
        CKeyMap::iterator iter = keys.find(m_sToken);

        if (iter != keys.end())
            m_eTokenType = iter->second;
        else
            m_eTokenType = eTokenType;
    }
    else
        m_eTokenType = eTokenType;

    //if (loggable().IsDebug())
    //    ITRACE_DEVELOP(_T("m_sToken=\"") + IString(m_sToken.c_str()) + _T("\""));

    m_nState = DONE;

    m_psCurrent--;
}


void CCPPTokenizer::SaveEolToken(CTokenTypes eTokenType)
{
//  IMODTRACE_DEVELOP();
    _ASSERTE(m_psCurrent!=NULL);

    _TCHAR c = *m_psCurrent;
    if(c>=256)
    {
        m_nCharType = Other;
    }
    else
    {
        m_nCharType = translation[c];
    }
    
    for ( ;(m_nCharType != eol) && (m_nCharType != eof); m_psCurrent++)
    {
        c = *m_psCurrent;
        if(c>=256)
        {
            m_nCharType = Other;
        }
        else
        {
            m_nCharType = translation[c];
        }
    }

    m_psCurrent--;

    unsigned int length = (unsigned int)(m_psCurrent - m_psBegin);

    m_sToken = StringType(m_psBegin).substr(0, length);
    m_psBegin=m_psCurrent;

    m_eTokenType = eTokenType;

    //if (loggable().IsDebug())
    //    ITRACE_DEVELOP(_T("m_sToken=") + IString(m_sToken.c_str()));

    m_nState = DONE;
    m_psCurrent--;
}


CCPPTokenizer &CCPPTokenizer::operator ++(int)
{
    Next();

    return *this;
}


const _TCHAR *CCPPTokenizer::Token() const
{
    if (m_sToken.size() == 0)
        return _T("");

    if (m_eTokenType == EndOfLine)
        return _T("");

    return m_sToken.c_str();
}


CCPPTokenizer::CTokenTypes CCPPTokenizer::GetTokenType() const
{
    return m_eTokenType;
}


ILoggable &CCPPTokenizer::loggable()
{ 
    if (m_pLoggable == NULL)
        m_pLoggable = &Trace::getLoggable(GetTraceHandle(), _T("CPPTokenizer"));

    return *m_pLoggable; 
}


StringType CCPPTokenizer::GetTokenTypeAsString() const
{
    //lint --e{787}
    switch (m_eTokenType)
    {
        case None:  return _T("None");
        case Identifier:  return _T("Identifier");
        case Integer:  return _T("Integer");
        case FloatNumber:  return _T("FloatNumber");
        case String:  return _T("String");
        case Character:  return _T("Character");
        case Lt:  return _T("Lt");
        case LtEq:  return _T("LtEq");                
        case Gt:  return _T("Gt");              
        case GtEq:  return _T("GtEq");
        case ShiftLeft:  return _T("ShiftLeft");
        case ShiftLeftAssign:  return _T("ShiftLeftAssign");
        case ShiftRight:  return _T("ShiftRight");
        case ShiftRightAssign:  return _T("ShiftRightAssign");
        case LtGt_NotEq:  return _T("LtGt_NotEq");
        case LogicalNot:  return _T("LogicalNot");
        case NotEq:  return _T("NotEq");
        case Divide:  return _T("Divide");
        case DivideAssign:  return _T("DivideAssign");
        case LineEndComment:  return _T("LineEndComment");
        case DocComment:  return _T("DocComment");
        case Comment:  return _T("Comment");
        case ExclusiveOr:  return _T("ExclusiveOr");
        case ExclusiveOrAssign:  return _T("ExclusiveOrAssign");
        case Multiply:  return _T("Multiply");
        case MultiplyAssign:  return _T("MultiplyAssign");
        case LeftBrace:  return _T("LeftBrace");
        case RightBrace:  return _T("RightBrace");
        case Conditional:  return _T("Conditional");
        case Comma:  return _T("Comma");
        case Subtract:  return _T("Subtract");
        case SubtractAssign:  return _T("SubtractAssign");
        case Decrement:  return _T("Decrement");
        case Indirection:  return _T("Indirection");
        case PtrToMemberPtr:  return _T("PtrToMemberPtr");
        case Modulo:  return _T("Modulo");
        case ModuloAssign:  return _T("ModuloAssign");
        case BitwiseAnd:  return _T("BitwiseAnd");      
        case BitwiseAndAssign:  return _T("BitwiseAndAssign");
        case LogicalAnd:  return _T("LogicalAnd");
        case LeftBracket:  return _T("LeftBracket");
        case RightBracket:  return _T("RightBracket");
        case Assignment:  return _T("Assignment");
        case Equals:  return _T("Equals");
        case BitwiseOr:  return _T("BitwiseOr");
        case BitwiseOrAssign:  return _T("BitwiseOrAssign");
        case LogicalOr:  return _T("LogicalOr");
        case LeftParen:  return _T("LeftParen");
        case RightParen:  return _T("RightParen");
        case Semicolon:  return _T("Semicolon");
        case Add:  return _T("Add");
        case AddAssign:  return _T("AddAssign");
        case Increment:  return _T("Increment");
        case Colon:  return _T("Colon");
        case ScopeResolution:  return _T("ScopeResolution");
        case Complement:  return _T("Complement");
        case MemberSelection:  return _T("MemberSelection");
        case PtrToMemberObject:  return _T("PtrToMemberObject");
        case EndOfLine:  return _T("EndOfLine");
        case Preprocessor:  return _T("Preprocessor");
        case WhiteSpace:  return _T("WhiteSpace");
        case Continuation:  return _T("Continuation");

        case Auto: return _T("Auto");
        case Bool: return _T("Bool");
        case Break: return _T("Break");
        case Case: return _T("Case");
        case Catch: return _T("Catch");
        case Char: return _T("Char");
        case Class: return _T("Class");
        case Const: return _T("Const");
        case Continue: return _T("Continue");
        case Default: return _T("Default");
        case Delete: return _T("Delete");
        case Do: return _T("Do");
        case Double: return _T("Double");
        case Else: return _T("Else");
        case Enum: return _T("Enum");
        case Extern: return _T("Extern");
        case Float: return _T("Float");
        case For: return _T("For");
        case Friend: return _T("Friend");
        case Goto: return _T("Goto"); 
        case If: return _T("If");
        case Inline: return _T("Inline");
        case Int: return _T("Int");
        case Long: return _T("Long");
        case New: return _T("New");
        case Operator: return _T("Operator");
        case Pascal: return _T("Pascal");
        case Private: return _T("Private");
        case Protected: return _T("Protected");
        case Public: return _T("Public");
        case Register: return _T("Register");
        case Return: return _T("Return");  
        case Short: return _T("Short");
        case Signed: return _T("Signed");
        case Sizeof: return _T("Sizeof");
        case Static: return _T("Static");
        case Struct: return _T("Struct");
        case Switch: return _T("Switch");
        case Template: return _T("Template");
        case This: return _T("This");
        case Typedef: return _T("Typedef");
        case Union: return _T("Union");
        case Unsigned: return _T("Unsigned");
        case Virtual: return _T("Virtual");
        case Void: return _T("Void)");
        case Volatile: return _T("Volatile");
        case While: return _T("While)");

        case EndOfFile:  return _T("EndOfFile");
    }

    return _T("<Unknown>");
}


long CCPPTokenizer::Translate(const _TCHAR *sType) throw()
{
    CKeyMap::iterator iter = keys.find(sType);
    if (iter != keys.end())
        return iter->second;

    return None;
}
