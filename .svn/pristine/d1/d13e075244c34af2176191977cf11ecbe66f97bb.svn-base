//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    ScanInfo.h
//
// TITLE:   ScanInfo data object
//
// AUTHOR:  TDonnan
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// ScanInfo
//////////////////////////////////////////////////////////////////////////////////////////////////
// new CRD126194

#pragma once
#include "XmlSerializer.h"

class CScanInfo
{
public:

    CScanInfo() :
        m_bstrButtonData        (_T("")),
        m_nBarcodeType          (0),
        m_nBarcodeLength        (0),
        m_bstrBarcodeData       (_T("")),
        m_nBarcodeLabelLen      (0),
        m_bstrBarcodeLabel      (_T("")) {}

    CScanInfo(
        _bstr_t a_bstrButtonData,
        int     a_nBarcodeType,
        int     a_nBarcodeLength,
        _bstr_t a_bstrBarcodeData,
        int     a_nBarcodeLabelLen,
        _bstr_t a_bstrBarcodeLabel ) :
            m_bstrButtonData        (   a_bstrButtonData        ),
            m_nBarcodeType          (   a_nBarcodeType          ),
            m_nBarcodeLength        (   a_nBarcodeLength        ),
            m_bstrBarcodeData       (   a_bstrBarcodeData       ),
            m_nBarcodeLabelLen      (   a_nBarcodeLabelLen      ),
            m_bstrBarcodeLabel      (   a_bstrBarcodeLabel      ){}

    virtual ~CScanInfo(){};

    CScanInfo(const CScanInfo & a_oRhs )
    {
        Assign(a_oRhs);
    }

    const CScanInfo & operator = (const CScanInfo & a_oRhs)
    {
        m_bstrButtonData    = a_oRhs.m_bstrButtonData;
        m_nBarcodeType      = a_oRhs.m_nBarcodeType;
        m_nBarcodeLength    = a_oRhs.m_nBarcodeLength;
        m_bstrBarcodeData   = a_oRhs.m_bstrBarcodeData;
        m_nBarcodeLabelLen  = a_oRhs.m_nBarcodeLabelLen;
        m_bstrBarcodeLabel  = a_oRhs.m_bstrBarcodeLabel;
        return *this;
    }

    const CScanInfo & Assign(const CScanInfo & a_oRhs)
    {
        m_bstrButtonData    = a_oRhs.m_bstrButtonData;
        m_nBarcodeType      = a_oRhs.m_nBarcodeType;
        m_nBarcodeLength    = a_oRhs.m_nBarcodeLength;
        m_bstrBarcodeData   = a_oRhs.m_bstrBarcodeData;
        m_nBarcodeLabelLen  = a_oRhs.m_nBarcodeLabelLen;
        m_bstrBarcodeLabel  = a_oRhs.m_bstrBarcodeLabel;
        return *this;
    }

    BOOL IsEqual( const CScanInfo & a_oRhs ) const
    {
        BOOL bIsEqual = TRUE;

        if ( _tcscmp( ButtonData(), a_oRhs.ButtonData() )  != 0 )
        {
            bIsEqual = FALSE;
        }
        else if ( BarcodeType() != a_oRhs.BarcodeType() )
        {
            bIsEqual = FALSE;
        }
        else if ( BarcodeLength() != a_oRhs.BarcodeLength() )
        {
            bIsEqual = FALSE;
        }
        else if ( _tcscmp( BarcodeData(), a_oRhs.BarcodeData() ) != 0 )
        {
            bIsEqual = FALSE;
        }
        else if ( BarcodeLabelLen() != a_oRhs.BarcodeLabelLen() )
        {
            bIsEqual = FALSE;
        }
        else if ( _tcscmp( BarcodeLabel(), a_oRhs.BarcodeLabel() ) != 0 )
        {
            bIsEqual = FALSE;
        }
        return bIsEqual;
    }

public:

    _bstr_t ButtonData()        const   { return m_bstrButtonData;  }
    int     BarcodeType()       const   { return m_nBarcodeType;    }
    int     BarcodeLength()     const   { return m_nBarcodeLength;  }
    _bstr_t BarcodeData()       const   { return m_bstrBarcodeData; }
    _bstr_t BarcodeLabel()      const   { return m_bstrBarcodeLabel;}
    int     BarcodeLabelLen()   const   { return m_nBarcodeLabelLen;}

    void ButtonData(const _bstr_t & a_bstrtValue )      { m_bstrButtonData  = a_bstrtValue; }
    void BarcodeType(const int a_nValue )               { m_nBarcodeType    = a_nValue ;    }
    void BarcodeLength(int a_nValue )                   { m_nBarcodeLength  = a_nValue;     }
    void BarcodeData(const _bstr_t &a_bstrtValue )      { m_bstrBarcodeData = a_bstrtValue; }
    void BarcodeLabel(const _bstr_t & a_bstrtValue )    { m_bstrBarcodeLabel= a_bstrtValue; }
    void BarcodeLabelLen(const int a_nValue )           { m_nBarcodeLabelLen= a_nValue;     }


public:

    _bstr_t Serialize() const
    {
        // trace(L6, _T("+CMobileCartAuditParser::CreateStartAuditRequest()"));

        CXmlSerializer  xs;
        _bstr_t         bstrtXml(L"");
        BOOL            bSuccess = FALSE;

        //<ScanInfo>
        //    <ButtonData>%s</ButtonData>
        //    <BarcodeType>%d</BarcodeType>
        //    <BarcodeLength>%d</BarcodeLength>
        //    <BarcodeData>%s</BarcodeData>
        //    <BarcodeLabelLen>%d</BarcodeLabelLen>
        //    <BarcodeLabel>%s</BarcodeLabel>
        //</ScanInfo>

        if ( eXsSuccess == xs.Create() )
        {
            xs.Serialize(_T("ScanInfo"));
            xs.Serialize(_T("ButtonData"),      m_bstrButtonData    );
            xs.Serialize(_T("BarcodeType"),     m_nBarcodeType      );
            xs.Serialize(_T("BarcodeLength"),   m_nBarcodeLength    );
            xs.Serialize(_T("BarcodeData"),     m_bstrBarcodeData   );
            xs.Serialize(_T("BarcodeLabelLen"), m_nBarcodeLabelLen  );
            xs.Serialize(_T("BarcodeLabel"),    m_bstrBarcodeLabel  );

            bstrtXml = xs.GetXml();

            #ifdef _CPPUNIT
                xs.Save(_T("ScanInfo.xml"));
            #endif

            bSuccess = TRUE;
        }

        xs.Close();

        // trace(L6, _T("-CMobileCartAuditParser::CreateStartAuditRequest() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

        return bstrtXml;
    }

    BOOL Deserialize(const _bstr_t & a_bstrtSSCOScanInfoXml)
    {
        BOOL            bSuccess = FALSE;
        CXmlSerializer  xs;
        eXsRetValue     eXsRet = eXsFailure;
        
        if ( eXsSuccess ==  xs.LoadXml( a_bstrtSSCOScanInfoXml ) )
        {
            if ( xs.Deserialize(_T("ScanInfo")) )
            {
                xs.Deserialize(_T("ButtonData"),      m_bstrButtonData    );
                xs.Deserialize(_T("BarcodeType"),     m_nBarcodeType      );
                xs.Deserialize(_T("BarcodeLength"),   m_nBarcodeLength    );
                xs.Deserialize(_T("BarcodeData"),     m_bstrBarcodeData   );
                xs.Deserialize(_T("BarcodeLabelLen"), m_nBarcodeLabelLen  );
                xs.Deserialize(_T("BarcodeLabel"),    m_bstrBarcodeLabel  );

                bSuccess = TRUE;
            }
        }
        return bSuccess;
    }

private:

    _bstr_t     m_bstrButtonData;
    int         m_nBarcodeType;
    int         m_nBarcodeLength;
    _bstr_t     m_bstrBarcodeData;
    int         m_nBarcodeLabelLen;
    _bstr_t     m_bstrBarcodeLabel;
};
