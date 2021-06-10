#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLDocumentWriter.h 1     6/20/08 1:14p Sa250050 $
/*
 * @(#)XMLDocumentWriter.h    $Revision: 1 $ $Date: 6/20/08 1:14p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/xml/XMLDocumentWriter.h $
 * 
 * 1     6/20/08 1:14p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:34p Dm185016
 * Moved to new Repository.
* 
* 1     7/20/04 3:43p Dm185016
*/ 
#include "Utils.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

class UTILS_API CXMLDocumentWriter
{
public:
    CXMLDocumentWriter(void);
    virtual ~CXMLDocumentWriter(void);

    virtual StringType GetXMLOutput() const;

    virtual void StartDocument();
    virtual void EndDocument();
    virtual void StartElement(const StringType &);
    virtual void EndElement(const StringType &);
    virtual void ClearAttributes();
    virtual void Characters(const StringType &);

    virtual void AddAttribute(const StringType &, const StringType &);
    virtual void AddAttribute(const StringType &, const _TCHAR *);
    virtual void AddAttribute(const StringType &, short);
    virtual void AddAttribute(const StringType &, long);
    virtual void AddAttribute(const StringType &, int);
    virtual void AddAttribute(const StringType &, unsigned long);
    virtual void AddAttribute(const StringType &, bool);
    virtual void AddAttribute(const StringType &, float);

    virtual void AddElement(const StringType &, const StringType &);
    virtual void AddElement(const StringType &, bool);
    virtual void AddElement(const StringType &, int);
    virtual void AddElement(const StringType &, long);
    virtual void AddElement(const StringType &, unsigned long);
    virtual void AddElement(const StringType &, short);
    virtual void AddElement(const StringType &, const _TCHAR *);
    virtual void AddElement(const StringType &, float);

protected:

    virtual void AddAdornments();

    inline IMXWriter &contentHandler() { return *pWriter; }

private:

	wchar_t *GetString(const std::string &) const;
	wchar_t *GetString(const std::wstring &) const;

    int attrcount;

    IMXWriter      *pWriter;
    IMXAttributes  *pMXAttrs;

    CComBSTR nullAttrString;

	static size_t nBufferLength;
	static wchar_t *pBuffer;
};
#pragma warning( pop )
