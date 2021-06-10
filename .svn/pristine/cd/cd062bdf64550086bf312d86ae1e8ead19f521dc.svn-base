#ifndef _CSTRING_TOOL_H
#define _CSTRING_TOOL_H

#include "tstring.h"

#include <vector>
#include <sstream>

using std::vector;

class CStringTool  
{
public:
    CStringTool();
    virtual ~CStringTool();

    static void Split(const tstring &sToSplit, 
                      TCHAR cToken, vector<tstring>& vResult);

    /**
     * Pad text with spaces so that the text is center-aligned within
     * a given width.
     * \param[in] theString String to center.
     * \param[in] nWidth The maximum string width.
     * \return Centered string.
     */
    static tstring CenterAlign(const tstring &theString, unsigned int nWidth);

    template <typename T>
    static tstring ToString(const T &tValue)
    {
        tstringstream theStream;
        theStream << tValue;
        return theStream.str();
    }

    static tstring ToDecimal(const long nNum, 
                             const unsigned int nNumDecimalPlaces);


    static const TCHAR SEMI_COLON;
};

#endif // _CSTRING_TOOL_H
