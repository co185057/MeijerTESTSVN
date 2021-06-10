/****************************************************************************
                       
Author: 
   Paul M. Parks (pp230000) 
   paul@parkscomputing.com

Purpose: 
   Replacement definitions for standard C++ library classes that will 
   work for ASCII and wide-character builds.

Comments: 

Change history:

*****************************************************************************/

#ifndef TSTRING_H
#define TSTRING_H

#pragma warning(disable: 4018 4100 4245 4511 4512 4663)

#include <tchar.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

namespace NCR
{
   namespace Util
   {
      #ifdef _UNICODE
      static std::wistream& tcin = std::wcin;
      static std::wostream& tcout = std::wcout;
      static std::wostream& tcerr = std::wcerr;
      static std::wostream& tclog = std::wclog;
      #else
      static std::istream& tcin = std::cin;
      static std::ostream& tcout = std::cout;
      static std::ostream& tcerr = std::cerr; 
      static std::ostream& tclog = std::clog; 
      #endif

      typedef std::basic_ios<TCHAR, std::char_traits<TCHAR> > tios;
      typedef std::basic_streambuf<TCHAR, std::char_traits<TCHAR> > tstreambuf;
      typedef std::basic_istream<TCHAR, std::char_traits<TCHAR> > tistream;
      typedef std::basic_ostream<TCHAR, std::char_traits<TCHAR> > tostream;
      typedef std::basic_iostream<TCHAR, std::char_traits<TCHAR> > tiostream;
      typedef std::basic_stringbuf<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > tstringbuf;
      typedef std::basic_istringstream<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > tistringstream;
      typedef std::basic_ostringstream<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > tostringstream;
      typedef std::basic_stringstream<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > tstringstream;
      typedef std::basic_filebuf<TCHAR, std::char_traits<TCHAR> > tfilebuf;
      typedef std::basic_ifstream<TCHAR, std::char_traits<TCHAR> > tifstream;
      typedef std::basic_ofstream<TCHAR, std::char_traits<TCHAR> > tofstream;
      typedef std::basic_fstream<TCHAR, std::char_traits<TCHAR> > tfstream;

      typedef std::basic_string<TCHAR> tstring;

      // A vector of TCHARs masquerading as a character array
      typedef std::vector<TCHAR> char_vec; 
   }
}


#endif // #ifndef TSTRING_H
