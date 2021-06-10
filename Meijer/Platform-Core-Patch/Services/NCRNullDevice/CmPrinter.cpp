#include "stdafx.h"
#include "ncrnulldevice.h"
#include "PrinterStation.h"
#include <oposptr.hi>
#include <NCRptr.h>
#include "NullService.h"
#include "NullPrinter.h"
#include "PrinterRequests.h"

const char cCR  = 0x0D;
const char cLF  = 0x0A;
const char cFF  = 0x0C;
const char cSP  = 0x20;
const char cESC = 0x1B;
const CString g_sCutFormat = _T("<Cut:%d>\r\n");

// These constants _must_ match the values in the ESC "!#C" escape sequence.
#define CS_SHSW     1
#define CS_SHDW     2
#define CS_DHSW     3
#define CS_DHDW     4

const long CMUpdate_FontType    = 0x0001;
const long CMUpdate_HorizScale  = 0x0010;
const long CMUpdate_VertScale   = 0x0020;
const long CMUpdate_Scale       = CMUpdate_HorizScale | CMUpdate_VertScale;
const long CMUpdate_Bold        = 0x0100;
const long CMUpdate_Underline   = 0x0200;
const long CMUpdate_Reverse     = 0x0400;
const long CMUpdate_AltColor    = 0x0800;

long CPrinterStation::ParsePrintData(    // Return OPOS_SUCCESS, OPOS_E_ILLEGAL, OPOS_E_FAILURE.
    const CString& sSrc )               // In > Source string.
//    CPrintData* pDest )                 // Out> Ptr to destination. Must be empty.
   {
    // Get some pointers and data for later use.
    // Data to convert modes into horizontal and vertical scaling.
    static int anHorizScale[] =         // Columns per character for each mode.
        { 1, 2, 1, 2 };                 //   CS_SHSW, CS_SHDW, CS_DHSW, CS_DHDW
    static int anVertScale[] =          // Rows per character for each mode.
        { 1, 1, 2, 2 };                 //   CS_SHSW, CS_SHDW, CS_DHSW, CS_DHDW

    // At the beginning of each print request:
    //  - Reset the character characteristics that restart at each print request.
    m_nUpdates &=               // Only remember changes to
        CMUpdate_FontType;      //   font type.
    m_nHorizScale = 1;          // Single wide.
    m_nVertScale  = 1;          // Single high.
    m_bBold       = FALSE;      // Bold off.
    m_nUnderline  = 0;          // Underline off.
    m_bReverse    = FALSE;      // Reverse off.
    m_bNotSHSW    = FALSE;      // SH/SW.
    m_bAlternateColor = FALSE;  // Set color to the default
    //  - Start parsing into the left section.
    m_psLCR = &m_sLCRLeft;
    //  - Initialize the validation variables.

    // Define variables to track validation status.
    long RCThis;
    long nValIndex;

    // Initialize source and destination info.
    int     nLen = sSrc.GetLength();    // Source chars to process.
    LPCTSTR pSrc = sSrc;                // Current position w/in source.

    // Variables to handle an embedded logo.
    //   (Recursion not allowed -- didn't want to take chance of logo sequence
    //    within a logo.)
    BOOL    bInLogo = FALSE;
    int     nLenOrig;
    LPCTSTR pSrcOrig;

    // Allocate the first line of data.
    CString*    psDestLine = new CString;


    /////////////////////////////////////////////////////////////////////////

    while ( nLen )
    {
        switch ( *pSrc )    // Next character
        {
        //***************************************************************
        // Carriage return.
        //   If followed by LF, do nothing.
        //   If not followed by LF, then print and try to stay on same line.

        //***************************************************************
        // Line feed.
        //   Print the buffer contents (if any) and advance to next line.

        //***************************************************************
        case cCR:
        case cLF:
            m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Process CR/LF - %s"), (LPCTSTR)(*psDestLine) );
            ProcessEOL( psDestLine );
            break;
        // Escape.  Must be of format
        //      ESC "|" number lowercaseletter lowercaseletter2 uppercaseletter
        //   where number, lowercaseletter, and lowercaseletter2 are optional.
        case cESC:
            {
               m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Process ESC - %s"), (LPCTSTR)(*psDestLine) );
                int nNumber = -1;               // Value of numeric component.
                char cLowLetter = cSP;          // Value of lowercaseletter component.
                char cLowLetter2 = cSP;         // Value of lowercaseletter2 component.
                int nIndex = 2;                 // Current index into the substring pSrc.

                // If not inside a logo, update validation index to the start of the
                //   escape. (If in a logo, then will leave at start of logo sequence
                //   until after logo is done.)
                if ( ! bInLogo )
                    nValIndex = pSrc - (LPCTSTR) sSrc; // Index of the escape.

                // Parse the escape sequence:

                if ( nLen < 3 ||                // If not at least 3 chars long, or
                    pSrc[1] != '|' )            //   doesn't start with x1B '|':
                {
                    goto ProcessDefault;        // Go treat like normal character.
                }

                if ( isdigit( pSrc[nIndex]) )   // If 3rd char is digit:
                {
                    nNumber = 0;
                    while (nIndex < nLen && isdigit( pSrc[nIndex]) )// Convert the number until
                        nNumber = nNumber*10 + pSrc[nIndex++]-'0';  // non-digit or end of data.
                }

                if ( islower( pSrc[nIndex]) )   // If next char is lowercase letter:
                {
                    cLowLetter = pSrc[nIndex];  //   Keep it and
                    nIndex++;                   //   advance index.
                }

                if ( islower( pSrc[nIndex]) )   // If next char is lowercase letter:
                {
                    cLowLetter2 = pSrc[nIndex]; //   Keep it and
                    nIndex++;                   //   advance index.
                }

                if ( nIndex >= nLen ||          // If at end of data or
                    !isupper( pSrc[nIndex]) )   //   char after optional fields is not [A-Z]:
                {
                    goto ProcessDefault;        //     and go treat like normal character.
                                                //     Then will processing '|' as non-escape char.
                }

                pSrc += nIndex;                 // Increment the pointer to the uppercase char.
                nLen -= nIndex;                 // Decrement length.

                // The escape sequence is in the proper format.

                // Now we'll try to interpret it.
                switch ( *pSrc )    // Escape UpperCharacter
                {

                //-------------------------------------------------------
                // (1) ESC "|" # "P"        Perform full or partial cut.
                // (2) ESC "|" # "fP"       Feed to cut bar and perform cut.
                // (3) ESC "|" # "sP"       Feed to cut bar, perform cut, and stamp.
                case 'P':
                    {
                        // If printable characters in the buffer and the app didn't
                        //   do a line feed: Do it for them.
                        if ( m_bPrintBuffer )
						{
                            ProcessEOL( psDestLine );
						}

                        // Reset local result code.
                        RCThis = OPOS_SUCCESS;

                        // If no # is defined, then we are suppose to perform a a Full Cut.
                        // So if default nNumber is found, set it to 100%
                        // MLB - 10/28/97 TAR # 68874
                        if ( nNumber == -1)
                            nNumber = 100;

                        // Check for invalid cases:
                        if ( m_nStation != PTR_S_RECEIPT ||                     // Not receipt.
                             nNumber > 100 )                        // % too big.
                        {
                            RCThis = OPOS_E_FAILURE;
                        }
                        // Check for case (1):
                        else if ( cLowLetter == cSP )
                        {
                            // Add the paper cut sequence.
                           CString sCut;
                           sCut.Format( g_sCutFormat, nNumber );
                           *psDestLine += sCut;
                        }
                        // Check for case (2):
                        else if ( cLowLetter == 'f' && cLowLetter2 == ' ' )
                        {
                            // Add the paper feed and cut sequences.
                           CString sFeed;
                           for ( int nIndex = 0; nIndex < m_nLinesToPaperCut; nIndex++)
                              sFeed +=  _T("\r\n");
                           CString sCut;
                           sCut.Format( g_sCutFormat, nNumber );
                           *psDestLine += sFeed + sCut;
                           }
                        // Check for case (3):
                        else if ( cLowLetter == 's' && cLowLetter2 == ' ' &&
                                  m_bCapStamp )
                        {
                            // Add the feed, paper cut, and stamp sequences.
                           CString sFeed;
                           for ( int nIndex = 0; nIndex < m_nLinesToPaperCut; nIndex++)
                              sFeed +=  _T("\r\n");
                           *psDestLine += sFeed;
							      if ( m_bCapPapercut )
                              {
                              CString sCut;
                              sCut.Format( g_sCutFormat, nNumber );
                              *psDestLine += sCut;
                              }
                           if ( m_bCapStamp != FALSE )
   							      *psDestLine += _T("<Stamp>\r\n");;
                        }
                        // Anything else...
                        else
                        {
                            RCThis = OPOS_E_FAILURE;
                        }

                        // Update the result code.
                        if ( RCThis )
                        {
                        }

                        // Go process next char.
                        break;
                    }

                //-------------------------------------------------------
                // ESC "|1B"    Print bitmap 1.
                // ESC "|2B"    Print bitmap 2.
				// ESC "|nB"    Print bitmap n.
                case 'B':
                    {
                        m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Process B(Bitmap) - %s"), (LPCTSTR)(*psDestLine) );
                        // If printable characters in the buffer and the app didn't
                        //   do a line feed: Do it for them.
                        if ( m_bPrintBuffer )
						         {
                            ProcessEOL( psDestLine );
						         }

                        // Check for invalid cases:
                        if ( cLowLetter != cSP ||                   // Lowercase letter.
                           /*  ( nNumber != 1 &&                      // Not bitmap 1
                               nNumber != 2 ) || */                   //   or 2.
                             m_nStation == PTR_S_JOURNAL                        // Journal station.
                           )
                        {
                        // This is an error
                        }

                        // Bitmap pointer is ok.
                        // Add it to the output data.
                        else
                        {
                           long lResult = PrintRequestEx( *psDestLine );
                           psDestLine->Empty();
                           if ( lResult != OPOS_SUCCESS ) 
                              return lResult;   // Stop now. 
                           lResult = m_dibBitmaps[ nNumber - 1 ].Print( this );
                           if ( lResult != OPOS_SUCCESS ) 
                              return lResult;   // Stop now. 
                        }
                        break;
                    }

                //-------------------------------------------------------
                // ESC "|tL"    Add top logo.
                // ESC "|bL"    Add bottom logo.
				// ESC "|sL"    Add receipt stamp.
                case 'L':
                    {
                        // Check for invalid cases:
                        m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Process L(LOGO) - %s"), (LPCTSTR)(*psDestLine) );
                        if ( cLowLetter2 != cSP ||                  // Lowercase letter #2.
                             bInLogo ||                             // Already in a log.
                             ( cLowLetter != 't' &&                 // Not 't', 'b' or 's'.
                               cLowLetter != 'b' &&
							   cLowLetter != 's' )
                           )
                        {
                        }
                        else if ( cLowLetter == 's' )
                        {
                            if ( m_bCapStamp != FALSE )
                                *psDestLine += _T("<Stamp>");;
						      }
                        else
                        {
                            // Get pointer to logo string.
                            CString* pLogo = &m_sLogoTop;
                            if (cLowLetter == 'b')
                                pLogo = &m_sLogoBottom;

                            // If empty, then ignore, else set to continue with the logo.
                            if ( ! pLogo->IsEmpty() )
                            {
                                // Set variables:
                                bInLogo = TRUE;         // Inside a logo.
                                nLenOrig = nLen - 1;    // Save main length and pointer,
                                pSrcOrig = pSrc + 1;    //   adjusted past the 'L'.

                                nLen = pLogo->GetLength(); // Set logo length.
                                pSrc = *pLogo;          // Set logo pointer.

                                // Continue way back at the "while ( nLen )".
                                continue;
                            }
                        }
                        break;
                    }

                //-------------------------------------------------------
                // (1) ESC "|" # "lF"       Feed lines.
                // (2) ESC "|" # "uF"       Feed map mode units.
                // (3) ESC "|" # "rF"       Reverse feed lines.
                case 'F':
                    {
                        m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Process F(FEED) - %s"), (LPCTSTR)(*psDestLine) );
                        // If printable characters in the buffer and the app didn't
                        //   do a line feed: Do it for them.
                        if ( m_bPrintBuffer )
						{
                            ProcessEOL( psDestLine );
						}

                        // Reset local result code.
                        RCThis = OPOS_SUCCESS;

                        // If no number specified, set default of 1.
                        if ( nNumber < 0 )
                            nNumber = 1;

                        // Check for invalid cases:
                        if ( cLowLetter2 != cSP )                   // Lowercase letter #2.
                        {
                            RCThis = OPOS_E_FAILURE;
                        }
                        // Check for case (1):
                        else if ( cLowLetter == 'l' )
                        {
                            // Add the feed line sequence.
                           CString sFeed;
                           for ( int nIndex = 0; nIndex < nNumber; nIndex++)
                              sFeed +=  _T("\r\n");
                           *psDestLine += sFeed;
                        }
                        // Check for case (2):
                        else if ( cLowLetter == 'u' )
                        {
                            // Calculate feed amount and add the feed unit sequence.
#if 0
                        long nDots = ConvertFromMapMode(
                                CStnMetrics::ConvertHeight, nNumber );
#endif
                            ASSERT( FALSE );
                            // *psDestLine += pPtrEsc->EscStnFeedDots( nDots, m_nStation );
                        }
                        // Check for case (3):
                        else if ( cLowLetter == 'r' )
                        {
                            CString s;
                            s.Format( _T("RevFeed:%d>"), nNumber  );
                            // Add the reverse feed line sequence.
                            *psDestLine += s;
                        }
                        // Anything else...
                        else
                        {
                            RCThis = OPOS_E_FAILURE;
                        }

                        // Update the result code.
                        if ( RCThis )
                        {
                        }

                        // Go process next char.
                        break;
                    }

                //-------------------------------------------------------
                // ESC "|" ... "C"
                case 'C':
                    {
                        m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Process C(Size) - %s"), (LPCTSTR)(*psDestLine) );
                        switch ( cLowLetter )
                        {
                        // ESC "|1C"    Single high/single wide.
                        // ESC "|2C"    Single high/double wide.
                        // ESC "|3C"    Double high/single wide.
                        // ESC "|4C"    Double high/double wide.
                        case ' ':
                            {
                            ProcessXHXW:
                                // Validate the number and set fail flag if not supported.
                                if ( nNumber < CS_SHSW ||
                                     nNumber > CS_DHDW ||
                                     nNumber == CS_SHDW && ! m_bCapDblWide ||
                                     nNumber == CS_DHSW && ! m_bCapDblHigh ||
                                     nNumber == CS_DHDW && ! m_bCapDblWideDblHigh )
                                {
                                    // If DHDW not supported but SHDW is, then change,
                                    // else drop back to SHSW.
                                    if ( nNumber == CS_DHDW && m_bCapDblWide )
                                    {
                                        nNumber = CS_SHDW;
                                    }
                                    else
                                    {
                                        nNumber = CS_SHSW;
                                    }
                                }

                                // If different than current setting, process.
                                int nHorizScale = anHorizScale[ nNumber - 1 ];
                                int nVertScale  = anVertScale [ nNumber - 1 ];
                                if ( m_nHorizScale != nHorizScale ||
                                     m_nVertScale  != nVertScale )
                                {
                                    // Update character scaling and set flag.
                                    m_nHorizScale = nHorizScale;
                                    m_nVertScale  = nVertScale ;
                                    m_nUpdates |= CMUpdate_Scale;
                                }
                                break;
                            }

                        // ESC "|" # "hC"   Scale horizontally
                        case 'h':
                            {
                                m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Process h(Scale)- %s"), (LPCTSTR)(*psDestLine) );
                                // Check for invalid cases:
                                if ( cLowLetter2 != cSP ||          // Lowercase letter #2.
                                     nNumber <= 0 )                 // Number zero or absent.
                                {
                                    break;
                                }
                                // If > max, then coerce to max.
                                long nMax = m_cMaxHorizScale;
                                if ( nNumber > nMax )
                                {
                                    nNumber = nMax;
                                }
                                // If <= 2 and vertical <= 2, then translate from current to new,
                                //   and go use the individual capabilities.
                                char naXlat[] =
                                {   CS_SHSW, CS_DHSW, // To SW.
                                    CS_SHDW, CS_DHDW  // To DW.
                                };
                                if ( nNumber <= 2 && m_nVertScale <= 2 )
                                {
                                    nNumber = naXlat[ (nNumber-1)*2 + m_nVertScale-1 ];
                                    goto ProcessXHXW;   // Join common code above.
                                }
                                // Else set the new scaling.
                                m_nHorizScale = nNumber;
                                m_nUpdates |= CMUpdate_HorizScale;
                                break;
                            }

                        // ESC "|" # "vC"   Scale vertically
                        case 'v':
                            {
                                 m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Process v(Scale) - %s"), (LPCTSTR)(*psDestLine) );
                                // Check for invalid cases:
                                if ( cLowLetter2 != cSP ||          // Lowercase letter #2.
                                     nNumber <= 0 )                 // Number zero or absent.
                                {
                                    break;
                                }
                                // If > max, then coerce to max.
                                long nMax = m_cMaxVertScale;
                                if ( nNumber > nMax )
                                {
                                    nNumber = nMax;
                                }
                                // If <= 2 and horizontal <= 2, then translate from current to new,
                                //   and go use the individual capabilities.
                                // Translate from current to new.
                                char naXlat[] =
                                {   CS_SHSW, CS_SHDW, // To SH.
                                    CS_DHSW, CS_DHDW  // To DH.
                                };
                                if ( nNumber <= 2 && m_nHorizScale <= 2 )
                                {
                                    nNumber = naXlat[ (nNumber-1)*2 + m_nHorizScale-1 ];
                                    goto ProcessXHXW;   // Join common code above.
                                }
                                // Else set the new scaling.
                                m_nVertScale = nNumber;
                                m_nUpdates |= CMUpdate_VertScale;
                                break;
                            }

                        // ESC "|bC"    Bold print on.
                        case 'b':
                            {
                                m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Process BOLD - %s"), (LPCTSTR)(*psDestLine) );
                                // Check for invalid cases:
                                if ( cLowLetter2 != cSP ||              // Lowercase letter #2.
                                     nNumber >= 0 ||                    // Number specified.
                                     !m_bCapBold )       // No capability.
                                {
                                }

                                // Else set on.
                                else if ( ! m_bBold )
                                {
                                    m_bBold = TRUE;
                                    m_nUpdates |= CMUpdate_Bold;
                                }

                                break;
                            }

                        // ESC "|uC"    Underline print on.
                        case 'u':
                            {
                              m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Process UNDER - %s"), (LPCTSTR)(*psDestLine) );
                                // Check for invalid cases:
                                if ( cLowLetter2 != cSP ||              // Lowercase letter #2.
                                     ! m_bCapUnderline || // No capability.
                                     nNumber == 0 )                     // Number is zero.
                                {
                                }

                                // Process the underline.
                                else
                                {
                                    // If number not specified, default to 1.
                                    if ( nNumber < 0 )
                                        nNumber = 1;

                                    // If number too big, set to max; else use it.
                                    if ( nNumber > m_cMaxUnderline )
                                    {
                                        nNumber = m_cMaxUnderline;
                                    }

                                    m_nUnderline = nNumber;
                                    m_nUpdates |= CMUpdate_Underline;
                                }

                                break;
                            }

                        case 'r':
                            {
                                m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Process r(REVERSE) - %s"), (LPCTSTR)(*psDestLine) );
                                if ( cLowLetter2 == 'v' )
                                {
                                    // Reverse video

                                    // Check for invalid cases:
                                    if ( nNumber >= 0 ||                // Number specified.
                                         ! m_cReverseVideo ) // No capability.
                                    {
                                    }
                                    else if ( ! m_bReverse )
                                    {
                                        m_bReverse = TRUE;
                                        m_nUpdates |= CMUpdate_Reverse;
                                    }
                                }
                                else if ( cLowLetter2 == cSP )
                                {
                                    // Use alternate color

                                    // Check for invalid cases:
                                    if ( nNumber >= 0 ||                // Number specified.
                                         ! m_bCap2Color ) // No capability.
                                    {
                                    }
                                    else if ( ! m_bAlternateColor )
                                    {
                                        m_bAlternateColor = TRUE;
                                        m_nUpdates |= CMUpdate_AltColor;
                                    }
                                }
                                break;
                            }

                        // Other -- error.
                        default:
                            {
                                m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Process \"default\" - %s"), (LPCTSTR)(*psDestLine) );
                                break;
                            }
                        }
                        // Go process next character after escape sequence.
                        break;
                    }

                //-------------------------------------------------------
                // ESC "|" lowletter "A"
                case 'A':
                    {
                        m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Process A - %s"), (LPCTSTR)(*psDestLine) );
                        // Check for invalid cases:
                        if ( cLowLetter2 != cSP ||                  // Lowercase letter #2.
                             nNumber >= 0 )                         // Number specified.
                        {
                        }

                        else switch ( cLowLetter )
                        {
                        // ESC "|cA"    Center following characters.
                        case 'c':
                            {
                                if ( m_psLCR != &m_sLCRCenter )
                                {
                                    if ( m_bNotSHSW )
                                    {
                                    *m_psLCR += GetModeString( GMS_SHSW );
                                        m_nUpdates |= CMUpdate_Scale;
                                    }
                                    // If some characters in the left section, increment
                                    //   column count to ensure a space between the sections.
                                    if ( m_sLCRLeft.GetCols() != 0 )
                                        m_nColumns++;
                                    m_psLCR = &m_sLCRCenter;
                                }
                                break;
                            }

                        // ESC "|rA"    Right justify following characters.
                        case 'r':
                            {
                                m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Process r(JUSTIFY) - %s"), (LPCTSTR)(*psDestLine) );
                                if ( m_psLCR != &m_sLCRRight)
                                {
                                    if ( m_bNotSHSW )
                                    {
                                    ASSERT( FALSE );
                                        *m_psLCR += GetModeString( GMS_SHSW );
                                        m_nUpdates |= CMUpdate_Scale;
                                    }
                                    // If some characters in the previous section, increment
                                    //   column count to ensure a space between the sections.
                                    if ( m_psLCR->GetCols() != 0 )
                                        m_nColumns++;
                                    m_psLCR = &m_sLCRRight;
                                }
                                break;
                            }

                        // Other -- error.
                        default:
                            {
                                m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Process \"default\" - %s"), (LPCTSTR)(*psDestLine) );
                                break;
                            }
                        }

                        break;
                    }

                //-------------------------------------------------------
                // ESC "|N"     Set normal characteristics.
                case 'N':
                    {
                       m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Process N(NORMAL) - %s"), (LPCTSTR)(*psDestLine) );
                        *m_psLCR += GetModeString( GMS_Normal );
                        m_psLCR = &m_sLCRLeft;
                        // Go process next char.
                        break;
                    }

                //-------------------------------------------------------
                // Other -- error.
                default:
                    {
                        break;
                    }

                }           // end: ( *pSrc )    // Escape UpperCharacter
                break;
            }

        //***************************************************************
        // If not a special character:
        ProcessDefault:
        default:
            {
                // If printable character (not control character or space),
                //   then increment character count.
                // If not enough space in the line for this character...
                if ( m_nColumns + m_nHorizScale > m_nLineChars )
                {
                    // Special case to try to keep right section together:
                    //   If we were in the right section, and some chars are in
                    //   the left or center section, then print the left+center,
                    //   and place all of the right section as the start of the
                    //   next line.
                    if ( m_psLCR == &m_sLCRRight &&
                         m_sLCRLeft.GetCols() + m_sLCRCenter.GetCols() != 0 )
                    {
                        // Remember the contents of the right section, and clear it.
                        CLCRString sLCRRight = m_sLCRRight;
                        m_sLCRRight.Empty();
                        // Print the left and center, and reinit parse data.
                        ProcessEOL( psDestLine );
                        // Restore the right section, and set column count to its columns.
                        m_sLCRRight = sLCRRight;
                        m_nColumns = m_sLCRRight.GetCols();
                    }
                    // Any other combination: Just print the line.
                    //   (Note that we allow the centered portion to be off-center
                    //    and/or continued onto the next line. Did the special case
                    //    for right mostly due to greater likelihood that it may
                    //    contain currency data that shouldn't be split up.
                    else
                    {
                        ProcessEOL( psDestLine );
                    }
                }

                // If need to change character characteristics, add sequence.
                if ( m_nUpdates )
                {
                    *m_psLCR += GetModeString( GMS_Update );
                }

                // Add character to the section.
                *m_psLCR += *pSrc;
                // Increment the section count and the line column count.
                m_psLCR->IncrCols( m_nHorizScale );
                m_nColumns += m_nHorizScale;
                // Set flag to say data is on current line.
                m_bPrintBuffer = TRUE;
                break;
            }
        }           // end: switch ( *pSrc )    // Next character

        // When done with character or sequence:
        nLen--;                         // Decrement length.
        pSrc++;                         // Increment pointer.

        // If length now zero, but logo is in progress,
        //   update to continue with the main string.
        if ( nLen == 0 && bInLogo )
        {
            bInLogo  = FALSE;           // No longer in logo.
            nLen = nLenOrig;            // Restore main string length.
            pSrc = pSrcOrig;            // Restore main string pointer.
        }
    }

    /////////////////////////////////////////////////////////////////////////

    // Restore mode to normal at end of request.
   *m_psLCR += GetModeString( GMS_Normal );

    // If printable characters left and registry says to auto append a line feed,
    //   output the data.
    if ( m_bPrintBuffer && m_bAutoLineFeed )
       {
       ProcessEOL( psDestLine );
       }

    // If destination line is empty, delete it, else add it to the print data list.
    if ( psDestLine->IsEmpty() )
    {
        delete psDestLine;
    }
    else
    {
        return PrintRequestEx( *psDestLine );

		// Tally number of lines and characters in this buffer
		// NOTE: The next lines need the tally IDs for the 3 stations to
		//       be consecutive in the order of JRN, REC, SLP.
    }

    // Set return value.
    return OPOS_SUCCESS;
}


void CPrinterStation::ProcessEOL( 
   CString* psDestLine, 
   CString* psTermination /* = NULL */ )
   {
   int nSpacing;
   int nColumns = 0;

   // If current section not in SHSW, force back to it.
   if ( m_bNotSHSW )
      *m_psLCR += GetModeString( GMS_SHSW );

   // If some padding is needed at the left, then add some space characters.
   if ( m_bPrintBuffer && m_nLeftPadChars )
      *psDestLine += CString( cSP, m_nLeftPadChars );

   //-----------------------------------------------------------------------
   // If some chars in the left section...
   if ( m_sLCRLeft.GetCols() )
      {
      // Add characters to the destination, and increment column count.
      *psDestLine += m_sLCRLeft;
      nColumns = m_sLCRLeft.GetCols();
      }

   // ...........................................................
   // If no chars, but some data (perhaps a mode change)...
   else if ( ! m_sLCRLeft.IsEmpty() )
      {
      // Add non-printables to the destination.
      *psDestLine += m_sLCRLeft;
      }

   //-----------------------------------------------------------------------
   // If some chars in the center section...
   if ( m_sLCRCenter.GetCols() )
      {
      // Calculate the spacing necessary to center the data.
      //   (If odd, the data is one column to the left of center.)
      nSpacing = ( m_nLineChars - m_sLCRCenter.GetCols() ) / 2 - nColumns;
      // If the spacing is zero or negative:
      if ( nSpacing <= 0 )
         {
         // Use one if left section present -- assures a little space.
         // Use zero if no left section (this occurs if the center section is
         //   the full line width).
         nSpacing = nColumns ? 1 : 0;
         }
      // If the spacing is positive, and there is a right section:
      else if ( m_sLCRRight.GetCols() )
         {
         // Calculate the available blank space, which is the line width less
         //   the sum of (Left, Center, Right, and 1 space between center and right).
         int nAvailSpace = m_nLineChars - ( nColumns + m_sLCRCenter.GetCols() + 1 + m_sLCRRight.GetCols() );
         // If can't center and still fit the right section in,
         //   then shift center section to the left enough to fit it.
         if ( nSpacing > nAvailSpace )
            nSpacing = nAvailSpace;
         }
      // Add spacing and characters to the destination, and increment column count.
      *psDestLine = *psDestLine + CString( cSP, nSpacing ) + m_sLCRCenter;
      nColumns = nColumns + nSpacing + m_sLCRCenter.GetCols();
      }
   // ...........................................................
   // If no chars, but some data (perhaps a mode change)...
   else if ( ! m_sLCRCenter.IsEmpty() )
      {
      // Add non-printables to the destination.
      *psDestLine += m_sLCRCenter;
      }

   //-----------------------------------------------------------------------
   // If some chars in the right section...
   if ( m_sLCRRight.GetCols())
      {
      // Calculate the spacing necessary to right justify the data.
      nSpacing = m_nLineChars - m_sLCRRight.GetCols() - nColumns;
      // (Will only be negative if the app changed the "line chars" while a
      //  partial line was already buffered.)
      if ( nSpacing < 0 )
         nSpacing = 0;

      // Add spacing and characters to the destination.
      *psDestLine = *psDestLine + CString( cSP, nSpacing ) + m_sLCRRight;
      }
   // ...........................................................
   // If no chars, but some data (perhaps a mode change)...
   else if ( ! m_sLCRRight.IsEmpty() )
      {
      // Add non-printables to the destination.
      *psDestLine += m_sLCRRight;
      }

   //-----------------------------------------------------------------------
   // If termination string passed, use it. Else add a terminating line feed.
   if ( psTermination )
      *psDestLine += *psTermination;
   else
      *psDestLine += _T("\r\n");

   // Reinitialize the parse data.
    m_sLCRLeft.Empty();
    m_sLCRCenter.Empty();
    m_sLCRRight.Empty();

    m_bPrintBuffer = FALSE;
    m_nColumns = 0;

   // If not in SHSW mode, then set flags to set proper mode on next character.
   if ( m_bNotSHSW )
      m_nUpdates |= CMUpdate_Scale;
   }


