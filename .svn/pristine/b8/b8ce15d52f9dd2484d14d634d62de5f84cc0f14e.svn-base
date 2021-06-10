#include "StdAfx.h"
#include "StringTool.h"

const TCHAR CStringTool::SEMI_COLON(_T(';'));

CStringTool::CStringTool()
{
}

CStringTool::~CStringTool()
{
}

void CStringTool::Split(const tstring& sToSplit, 
                        TCHAR cToken, vector<tstring>& vResult)
{
    vResult.clear();
    tstring sBuffer;
    for(size_t i=0; i<sToSplit.length(); i++)
    {
        if(sToSplit[i] == cToken)
        {
            vResult.push_back(sBuffer);
            sBuffer = _T("");
        }
        else
        {
			sBuffer += sToSplit[i];            
        }
    }

    if(!sBuffer.empty())
    {
        if(! vResult.empty())
        {
            vResult.push_back(sBuffer);
        }
    }
}

tstring CStringTool::CenterAlign(const tstring &theString, 
                                 unsigned int nWidth)
{
    tstring sResult;

    if(! theString.empty() && nWidth > theString.length())
    {
        const unsigned int nPadSpaces((nWidth - theString.length()) / 2);
        for(size_t i=0; i<nPadSpaces; i++)
        {
            sResult += _T(' ');
        }

        sResult += theString;
    }
    else
    {
        sResult = theString;
    }
    return sResult;
}

tstring CStringTool::ToDecimal(const long nNum, 
                               const unsigned int nNumDecimalPlaces)
{
    long nFactor(1);
    for(size_t i=0; i<nNumDecimalPlaces; i++)
    {
        nFactor *= 10;
    }

    const long nRaw(abs(nNum));
    const long nNotePortion = nRaw / nFactor;
    const long nCoinPortion = nRaw % nFactor;

    tstring sResult;
    if(nNum < 0)
    {
        sResult += _T('-');
    }

    sResult += ToString(nNotePortion);

    if(nNumDecimalPlaces > 0)
    {
        sResult += _T('.');
        if(nCoinPortion < nFactor/10)
        {
            long nPadding = nCoinPortion > 0 ? nCoinPortion : 1;
            while(nPadding < (nFactor/10))
            {
                nPadding *= 10;
                sResult += _T('0');
            }
        }
        
        sResult += ToString(nCoinPortion);
    }

    return sResult;
}