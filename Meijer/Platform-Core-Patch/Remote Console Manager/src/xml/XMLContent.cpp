#include "StdAfx.h"
#include "XMLContent.h"

CXMLContent::CXMLContent(void)
    : m_pLoggable(NULL)
{
}

CXMLContent::~CXMLContent(void)
{
}


ILoggable &CXMLContent::loggable()
{ 
    if (m_pLoggable == NULL)
        m_pLoggable = &Trace::getLoggable(GetTraceHandle(), eDCXml);

    return *m_pLoggable; 
}
