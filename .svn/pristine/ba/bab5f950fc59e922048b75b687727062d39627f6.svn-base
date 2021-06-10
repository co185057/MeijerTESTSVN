#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/CPPTokenizer.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)CPPTokenizer.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/CPPTokenizer.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     3/24/05 7:29p Dm185016
 * UNICODE
 * 
 * 2     2/28/05 3:30p Dm185016
 * lint
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 6     10/04/04 7:00p Dm185016
* Trace object is no longer shared.
* 
* 4     7/13/04 3:14p Dm185016
* Added missing keywords
* 
* 3     7/06/04 5:33p Dm185016
* 
* 2     5/22/04 11:17a Dm185016
* lint + removed trace
* 
* 1     2/10/04 11:52a Dm185016
* New object to parse expressions
* 
* 1     1/23/04 3:39p Dm185016
* Key,Value parser for SecurityManager API
*/

//lint --e{1759}        By design, the postfix increment returns a reference

#include <vector>
#include "Utils.h"
#include "Loggable.h"
#include <map>
#include "StringComparator.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

using namespace std;


typedef map<const _TCHAR *, long, StringComparator> CTypeMap;
typedef CTypeMap::iterator CTypeMapIterator;

///<summary>This class is used to tokenize a string</summary>
class UTILS_API CCPPTokenizer
    : public CBaseObject
{
    enum States
    {
        DONE = -1
    };


    enum CCharTypes 
    {
        Alpha=1,
        Digit,
        Other,
        eol,

        eof=-1
    };

    typedef enum CCharTypes CValueTypes;

    typedef struct
    {
        CCharTypes  eValueType;
        StringType  sValue;
    } CValue;
    typedef std::vector<CValue> CValueVector;
    typedef CValueVector::iterator CValueIterator;

public:
    
    enum CTokenTypes 
    {
        None=0,
        Identifier=1,
        Integer,
        FloatNumber,
        String,
        Character,
        Lt,                     // <
        LtEq,                   // <=
        Gt,                     // >
        GtEq,                   // >=
        ShiftLeft,              // <<
        ShiftLeftAssign,        // <<=
        ShiftRight,             // >>
        ShiftRightAssign,       // >>=
        LtGt_NotEq,             // <>
        LogicalNot,             // !
        NotEq,                  // !=
        Divide,                 // /
        DivideAssign,           // /=
        LineEndComment,         // // ...
        DocComment,             // /// ...
        Comment,                // /* ... */
        ExclusiveOr,            // ^
        ExclusiveOrAssign,      // ^=
        Multiply,               // *
        MultiplyAssign,         // *=
        LeftBrace,              // {
        RightBrace,             // }
        Conditional,            // ?
        Comma,                  // ,
        Subtract,               // -
        SubtractAssign,         // -=
        Decrement,              // --
        Indirection,            // ->
        PtrToMemberPtr,         // ->*
        Modulo,                 // %
        ModuloAssign,           // %=
        BitwiseAnd,             // &
        BitwiseAndAssign,       // &=
        LogicalAnd,             // &&
        LeftBracket,            // [
        RightBracket,           // ]
        Assignment,             // =
        Equals,                 // ==
        BitwiseOr,              // |
        BitwiseOrAssign,        // |=
        LogicalOr,              // ||
        LeftParen,              // (
        RightParen,             // )
        Semicolon,              // ;
        Add,                    // +
        AddAssign,              // +=
        Increment,              // ++
        Colon,                  // :
        ScopeResolution,        // ::
        Complement,             // ~
        MemberSelection,        // .
        PtrToMemberObject,      // .*
        Preprocessor,           // #
        WhiteSpace,             // ' ' or \t
        Continuation,           // \ <CRLF>
        EndOfLine,

        Asm,
        Auto,
        Bool,
        Break,
        Case,
        Catch,
        Char,
        Class,
        Const,
        Const_cast,
        Continue,
        Default,
        Delete,
        Do,
        Double,
        Dynamic_cast,
        Else,
        Enum,
        Explicit,
        Export,
        Extern,
        False,
        Float,
        For,
        Friend,
        Goto,
        If,
        Inline,
        Int,
        Long,
        Mutable,
        Namespace,
        New,
        Operator,
        Pascal,
        Private,
        Protected,
        Public,
        Register,
        Reinterpret_cast,
        Return,  
        Short,
        Signed,
        Sizeof,
        Static,
        Static_cast,
        Struct,
        Switch,
        Template, 
        This,
        Throw,
        True,
        Try,
        Typedef,
        Typeid,
        Typename,
        Union,
        Unsigned,
        Using,
        Virtual,
        Void,
        Volatile,
        While,
        Wchar_t,

        MaxCppToken,

        EndOfFile     = -1
    };

private:
        
    typedef map<StringType, CTokenTypes, StringComparator> CKeyMap;

public:

    ///<summary>Default Constructor</summary>
    CCPPTokenizer();

    ///<summary>Constructor taking the string to parse as its sole argument</summary>
    ///<param name="sSource">The string source to parse</param>
    CCPPTokenizer(const _TCHAR *sSource);

    ///<summary>Default Destructor</summary>
    ~CCPPTokenizer();

    ///<summary>Sets the source string to be parsed</summary>
    ///<param name="sSource">The string source to parse</param>
    void SetSource(const _TCHAR *sSource);

    ///<summary>Advances the internal interator to the next token in the source string</summary>
    void Next();

    ///<summary>Advances the internal interator to the next token in the source string</summary>
    CCPPTokenizer &operator ++(int x);

    ///<summary>Returns the current token string</summary>
    const _TCHAR *Token() const;

    ///<summary>Returns the type of the current token string</summary>
    CTokenTypes GetTokenType() const;

    ///<summary>Returns the type of the current token string as a string</summary>
    StringType GetTokenTypeAsString() const;

    ///<summary>Returns the current line count</summary>
    int GetLineNo() const;

    ///<summary>Returns the type </summary>
    ///<param name="sSource">The type as a string</param>
    static long Translate(const _TCHAR *) throw();
    
    ///<summary>Initializes the keys mapping</summary>
    static void InitKeys() throw();

private:

    _TCHAR NextChar();

    ILoggable &loggable();

    void State1(int nCharType);
    void State2(int nCharType);
    void State3(int nCharType);
    void State4(int nCharType);
    void State5(int nCharType);
    void State6(int nCharType);
    void State7(int nCharType);
    void State8(int nCharType);
    void State9(int nCharType);
    void State10(int nCharType);
    void State11(int nCharType);
    void State12(int nCharType);
    void State13(int nCharType);
    void State14(int nCharType);
    void State15(int nCharType);
    void State16(int nCharType);
    void State17(int nCharType);
    void State18(int nCharType);
    void State19(int nCharType);
    void State20(int nCharType);
    void State21(int nCharType);
//  void State22(int nCharType);
    void State23(int nCharType);
    void State24(int nCharType);
    void State25(int nCharType);
    void State26(int nCharType);
    void State27(int nCharType);
    void State28(int nCharType);
    void State29(int nCharType);
    void State30(int nCharType);
    void State31(int nCharType);
    void State32(int nCharType);

    void Init();
    void SaveToken(CTokenTypes, bool bPushback=false);
    void SaveEolToken(CTokenTypes);

    void SkipWhitespace();
    
    ///<summary>The string to be parsed</summary>
    const _TCHAR * m_psSource;

    _TCHAR * m_psCurrent;
    _TCHAR * m_psBegin;
    _TCHAR * m_psEnd;

    int m_nCharType;
    StringType m_sToken;
    CTokenTypes m_eTokenType;

    _TCHAR translation[256];

    static CKeyMap keys;

    int m_nState;
    int m_nLineNo;

    bool m_bError;

    static _TCHAR defaultTranslation[256];

    ILoggable *m_pLoggable;
};
#pragma warning( pop )
