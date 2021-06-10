#include "StdAfx.h"
#include "DCAPMethodTrace.h"
#include "CmDataCapture.h"
#include <assert.h>

CmDataCapture *CDCAPMethodTrace::m_spDC = NULL;

CDCAPMethodTrace::CDCAPMethodTrace(LPCTSTR szModuleName, long level) : 
    m_lLevel(level)
{
    assert(szModuleName != NULL); 
    if(szModuleName == NULL || m_spDC == NULL)
    {
        return;
    }

    m_csModule = szModuleName;
    m_spDC->DCPrintf(m_lLevel, _T("++%s"), (LPCTSTR)m_csModule); 
}

CDCAPMethodTrace::~CDCAPMethodTrace()
{
    if(m_spDC == NULL)
    {
        return;
    }

    try
    {
        m_spDC->DCPrintf(m_lLevel, _T("--%s"), (LPCTSTR)m_csModule); 
    }
    catch(...)
    {
    }
}

void CDCAPMethodTrace::SetDC(CmDataCapture *dc)
{
    m_spDC = dc;
}

CDCAPMethodTrace::CDCAPMethodTrace(const CDCAPMethodTrace &rhs) : 
    m_lLevel(0)
{
    *this = rhs;
}

CDCAPMethodTrace & CDCAPMethodTrace::operator=(const CDCAPMethodTrace &rhs)
{
    if(this == &rhs)
    {
        return *this;
    }

    m_csModule = rhs.m_csModule;
    m_lLevel = rhs.m_lLevel;

    return *this;
}
