#include "StdAfx.h"
#include "StringConverter.h"
#include <comutil.h>

CStringConverter::CStringConverter()
{
}

CStringConverter::~CStringConverter()
{
}

string CStringConverter::AsString(const tstring &sToConvert) const
{
    const _bstr_t bstrConvert(sToConvert.c_str());

    return (LPCSTR)bstrConvert;
}
