// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/Include/XError.h 1     6/23/08 4:55p Sa250050 $
#pragma once
/****************************************************************************/
/*                                                                          */
/* Include File Name:         XERROR.h									    */
/*                                                                          */
/* Include Title:             This file contains the class descriptions for */
/*                            the MSD Error class object.  This object is   */
/*                            thrown to report any errors in the Electronic */
/*                            Grocery Store.                                */
/*                                                                          */
/* Status:                    Release 1.0                                   */
/*                                                                          */
/* Notes:                                                                   */
/*   - Dependencies:          <insert dependencies here>                    */
/*   - Restrictions:          <insert restrictions here>                    */
/*                                                                          */
/* Function:                  <explanation of use of these defines>         */
/*                                                                          */
/****************************************************************************/
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/Include/XError.h $
 * 
 * 1     6/23/08 4:55p Sa250050
 * FastLane 4.0 E5.3 Build 291 - Core Source
 * 
 * 10    6/24/05 1:44p Dm185016
 * Added default value for setSeverity()
 * 
 * 9     6/14/05 8:56a Dm185016
 * Corrected locationAtIndex() method for exception tracing.
 * 
 * 6     3/24/05 7:34p Dm185016
 * UNICODE
 * 
 * 5     3/23/05 1:39p Tp151000
 * Per David M. to fixed build problem
 * 
 * 4     2/28/05 6:03p Dm185016
 * Unicode fix
 * 
 * 3     2/28/05 5:23p Dm185016
 * Unicode problems
 * 
 * 2     2/28/05 3:36p Dm185016
 * lint
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 10    1/18/05 4:04p Dm185016
* UNICODE support
* 
* 9     10/07/04 9:44a Dm185016
* Fixed compile error with SCOTAPP - ambiguous IErrorInfo class.
* 
* 8     10/04/04 7:02p Dm185016
* Moved to namespace utils.
* 
* 6     5/18/04 7:21p Dm185016
* lint
* 
* 5     4/20/04 4:46p Dm185016
* Displayed code in hex
* 
* 4     2/10/04 11:54a Dm185016
* IException has been made into a derivative of std::exception.
* 
* 3     1/23/04 3:41p Dm185016
* DLL Support
* 
* 2     1/16/04 9:32a Dm185016
* Cleaned up the code
* 
* 1     1/06/04 9:00a Dm185016
* First Version
*/

//lint -emacro( *, ITHROWSYSTEMERROR )
//lint -emacro(534, ITHROWNEW)
//lint -emacro(534, ITHROW)
//lint -esym(534, IException::addLocation)
/*lint -save -e1763 */

#include "XString.h"
#include "Utils.h"
#include <stdexcept>

namespace utils
{

class UTILS_API IExceptionLocation 
{
public:

    /*------------------------------- Constructors -------------------------------*/
    IExceptionLocation ( const _TCHAR*   fileName = 0,
                        const _TCHAR*   functionName = 0,
                        unsigned long lineNumber = 0 );

    IString toString() const;

    /*-------------------------------- Attributes --------------------------------*/
    inline const _TCHAR
    *fileName     ( ) const { return pClFile; }
    inline const _TCHAR
    *functionName ( ) const{ return pClFunction; };

    inline unsigned long
    lineNumber ( ) const{ return ulClLineNo; };

private:
    /*--------------------------------- Private ----------------------------------*/
    const _TCHAR
    *pClFile,
    *pClFunction;
    unsigned long
    ulClLineNo;
}; // IExceptionLocation


class UTILS_API IErrorInfo
{

public:

    virtual ~IErrorInfo ( ) {};

    virtual const _TCHAR *text ( ) const = 0;

    virtual HRESULT errorId ( ) const = 0;

    virtual IErrorInfo* copy( ) const = 0;

protected:

    IErrorInfo ( ) {};
    IErrorInfo ( const IErrorInfo&  ) {};

private:
/*------------------------------- Constructors -------------------------------*/

    IErrorInfo &operator= ( const IErrorInfo&);
}; // IErrorInfo

}

#pragma warning( push )
#pragma warning( disable : 4275 )

using namespace utils;

#define MAX_LOC     5

class UTILS_API IException
    : public std::exception
{
public:

    enum Severity { unrecoverable, recoverable };

    /*------------------------------- Constructors -------------------------------*/
    IException ( const _TCHAR*     errorText,
                HRESULT           errorId = 0,
                Severity          severity = IException::unrecoverable );
    IException ( const IException& exception );
    IException();
    IException
        &operator= ( const IException& exc );

    virtual
    ~IException ( );



    /*------------------------------ Exception Text ------------------------------*/
    const _TCHAR
    *text ( unsigned long indexFromTop = 0 ) const;


    virtual const char *what( ) const throw( );
    //IException
    // &appendText  ( const _TCHAR*   errorText ),
    // &setText     ( const _TCHAR*   errorText );

    //unsigned long
    //  textCount     ( ) const;

    /*---------------------------- Exception Severity ----------------------------*/
    IException
    &setSeverity ( Severity      severity = unrecoverable ) { exsevCl = severity; return *this; }

    inline virtual int
    isRecoverable ( ) const { return (exsevCl == recoverable); }

    /*----------------------------- Error Identifier -----------------------------*/
    inline IException
    &setErrorId  ( HRESULT errId ) { m_hrClErrorId = errId; return *this; }

    inline HRESULT
    errorId       ( ) const { return m_hrClErrorId; }

    inline utils::IErrorInfo *
    errorInfo       ( ) const { return m_pErrorInfo; }

    void setErrorInfo (const utils::IErrorInfo *pErrorInfo );

    /*---------------------------- Exception Location ----------------------------*/
    virtual IException
    &addLocation      ( const IExceptionLocation& location );

    inline unsigned long
    locationCount ( ) const { return ulexlocClCount; }

    const IExceptionLocation
    &locationAtIndex ( unsigned long ) const;

    /*------------------------------ Exception Type ------------------------------*/
    virtual const _TCHAR
    *name ( ) const { return _T("<No Name Available>"); }

    /*---------------------------- Exception Logging -----------------------------*/
    //static IException::TraceFn
    //  *setTraceFunction ( IException::TraceFn& traceFunction );

    //virtual IException
    //&logExceptionData ( );

    /*------------------------- Application Termination --------------------------*/
    //virtual void
    //  terminate ( ) { throw(new xmsg("terminate() called for some reason")); }

    /*------------------------------ Throw Support -------------------------------*/
    static void
    assertParameter ( const _TCHAR*        exceptionText,
                        IExceptionLocation location );

private:

    /*----------------------------- Hidden Functions -----------------------------*/
    void _assign ( const IException& exc );

    /*--------------------------------- Private ----------------------------------*/
    //friend class IException::TraceFn;

    Severity            exsevCl;
    HRESULT             m_hrClErrorId;
    IExceptionLocation  exlocClArray[MAX_LOC];
    unsigned long       ulexlocClCount,
                        ulClTxtLvlCount;

    StringType          m_sErrorText;
    mutable std::string         m_sAErrorText;

    utils::IErrorInfo  *m_pErrorInfo;

}; // IException


class UTILS_API ISystemErrorInfo : public utils::IErrorInfo
{
   
public:
    /*------------------------------- Constructors -------------------------------*/
    ISystemErrorInfo ( HRESULT systemErrorId,
                        const _TCHAR* systemFunctionName = 0 );
    ISystemErrorInfo ( const _TCHAR* systemFunctionName = 0 );

    virtual
    ~ISystemErrorInfo ( );

    /*---------------------------- Error Information -----------------------------*/
    inline virtual
        operator const _TCHAR* ( ) const { return m_sErrorText.c_str(); }

    virtual const _TCHAR
    *text ( ) const { return m_sErrorText.c_str(); }

    virtual HRESULT
    errorId ( ) const{ return m_hrClErrorId; }

    virtual utils::IErrorInfo* copy( ) const { return new ISystemErrorInfo(*this); }

    inline virtual BOOL
    isAvailable ( ) const { return true; }


private:
    ISystemErrorInfo ( const ISystemErrorInfo& errorInformation );

    ISystemErrorInfo &operator= ( const ISystemErrorInfo& errorInformation );

    /*--------------------------------- Private ----------------------------------*/
    BOOL bClError;
    HRESULT m_hrClErrorId;
    StringType m_sErrorText;
}; // ISystemErrorInfo


class UTILS_API ISocketErrorInfo : public utils::IErrorInfo
{
public:
    /*------------------------------- Constructors -------------------------------*/
        ISocketErrorInfo ( HRESULT socketErrorId,
                            const _TCHAR* socketFunctionName = 0 );
        ISocketErrorInfo ( const _TCHAR* socketFunctionName = 0 );

    virtual
        ~ISocketErrorInfo ( );

    /*---------------------------- Error Information -----------------------------*/
    inline virtual
        operator const _TCHAR* ( ) const { return m_sErrorText.c_str(); }

    inline virtual const _TCHAR
        *text ( ) const { return m_sErrorText.c_str();}

    inline virtual HRESULT
        errorId ( ) const{ return m_hrClErrorId; }

    virtual utils::IErrorInfo* copy( ) const { return new ISocketErrorInfo(*this); }

    inline virtual bool
        isAvailable ( ) const { return true; }

    protected:

	    const _TCHAR *lookupError(HRESULT) const;

    private:
        ISocketErrorInfo ( const ISocketErrorInfo& errorInformation );

        ISocketErrorInfo
         &operator= ( const ISocketErrorInfo& errorInformation );

        /*--------------------------------- Private ----------------------------------*/
        bool
          bClError;
        HRESULT
          m_hrClErrorId;
        StringType m_sErrorText;
}; // ISocketErrorInfo


class UTILS_API ICommunicationErrorInfo : public utils::IErrorInfo
{

public:
    /*------------------------------- Constructors -------------------------------*/
        ICommunicationErrorInfo ( const COMSTAT &comstat,
                                DWORD comErrors,
                                const _TCHAR* commFunctionName = 0 );

    virtual
        ~ICommunicationErrorInfo ( );

    /*---------------------------- Error Information -----------------------------*/
    inline virtual
        operator const _TCHAR* ( ) const { return m_sErrorText.c_str(); }

    inline virtual const _TCHAR
        *text ( ) const { return m_sErrorText.c_str(); }

    inline virtual HRESULT
        errorId ( ) const{ return (HRESULT)comError; }

    virtual utils::IErrorInfo* copy( ) const { return new ICommunicationErrorInfo(*this); }

    inline virtual COMSTAT
        deviceStatus( ) const{ return comstat; }

    inline virtual bool
		isBreakCondition( ) const{ return (comError & CE_BREAK ? true : false); }

    inline virtual bool
        isFramingError( ) const{ return (comError & CE_FRAME ? true : false); }

    inline virtual bool
        isIOError( ) const{ return (comError & CE_IOE ? true : false); }

    inline virtual bool
        isModeError( ) const{ return (comError & CE_MODE ? true : false); }

    inline virtual bool
        isBufferOverrun( ) const{ return (comError & CE_OVERRUN ? true : false); }

    inline virtual bool
        isReceiveBufferFull( ) const{ return (comError & CE_RXOVER ? true : false); }

    inline virtual bool
        isParityError( ) const{ return (comError & CE_RXPARITY ? true : false); }

    inline virtual bool
        isTransmitBufferFull( ) const{ return (comError & CE_TXFULL ? true : false); }

    inline virtual bool
        isAvailable ( ) const { return true; }

protected:

	const _TCHAR *lookupError(const _TCHAR *pFunctionName) const;

private:
    ICommunicationErrorInfo ( );
    ICommunicationErrorInfo ( const ICommunicationErrorInfo& errorInformation );

    ICommunicationErrorInfo
        &operator= ( const ICommunicationErrorInfo& errorInformation );

    /*--------------------------------- Private ----------------------------------*/
    BOOL
        bClError;
    COMSTAT
        comstat;
    DWORD
        comError;
    StringType m_sErrorText;
}; // ICommunicationErrorInfo


#if defined ( INO_FUNCTION_NAMES )
  #define IEXCEPTION_LOCATION() \
                IExceptionLocation(_T(__FILE__), _T(""), __LINE__)
#elif defined ( __FUNCTION__ )
  #define IEXCEPTION_LOCATION()  \
                IExceptionLocation(_T(__FILE__), _T(__FUNCTION__) , __LINE__)
#else
  #define IEXCEPTION_LOCATION()   \
                IExceptionLocation(_T(__FILE__), _T(""), __LINE__)
#endif

#define ITHROWNEW(type, msg)\
{\
    type exc(msg);\
 	exc.addLocation(IEXCEPTION_LOCATION());\
 	throw(exc);\
}

#define ITHROW(EXCEPTION)\
{\
    IException exc = EXCEPTION;\
 	exc.addLocation(IEXCEPTION_LOCATION());\
 	throw(exc);\
}

#define IRETHROW(exc)\
{\
     exc.addLocation(IEXCEPTION_LOCATION());\
     throw;\
}


//#define ALLOCSYSTEMERROR(id, func)
#define ITHROWSYSTEMERROR(id, func, sev)\
{\
    DWORD dw = id;\
    ISystemError exc(Trace::formatLastError(QUOTE(func), dw), dw);\
    exc.setSeverity(sev)\
     	.addLocation(IEXCEPTION_LOCATION());\
	throw(exc);\
}

//#define ALLOCSYSTEMERROR(id, func)
#define ITHROWSEHERROR(code, info, sev)\
{\
    ISehError exc(Trace::formatLastSeh(code, info));\
    exc.setSeverity(sev)\
        .addLocation(IEXCEPTION_LOCATION());\
	throw(exc);\
}


#define IEXCLASSDECLARE(child,parent) class UTILS_API child : public parent {\
public:\
  child(const _TCHAR* a, HRESULT b = 0);\
  virtual const _TCHAR* name() const;\
  virtual ~child();\
  child(const child &);\
private:\
  child &operator = ( const child & );\
  child();\
}


#define IEXCLASSDECLARE_API(child,parent,linkage) class linkage child : public parent {\
public:\
  child(const _TCHAR* a, HRESULT b = S_OK);\
  virtual const _TCHAR* name() const;\
  virtual ~child();\
  child(const child &);\
private:\
  child &operator = ( const child & );\
  child();\
}


#define IEXCLASSIMPLEMENT(child,parent)\
  child :: child(const _TCHAR* a, HRESULT b)\
    : parent((const _TCHAR *)(IString(a) + _T(".  Code=") + IString(b).d2x()), b)\
        { }\
  const _TCHAR* child :: name() const\
    {\
     return (  _T(#child) );\
    }\
  child :: ~child() {;}\
  child :: child(const child & a) : parent(a) {;}


IEXCLASSDECLARE(IInvalidRequest,IException);
IEXCLASSDECLARE(IEmptyException,IException);
IEXCLASSDECLARE(ISocketError,IException);
IEXCLASSDECLARE(ISystemError,IException);
IEXCLASSDECLARE(IAccessError,IException);
IEXCLASSDECLARE(IInvalidParameter,IException);
IEXCLASSDECLARE(IOutOfMemory,IException);
IEXCLASSDECLARE(ISehError,IException);
IEXCLASSDECLARE(IOError,IException);

#pragma warning( pop )

//lint -restore
