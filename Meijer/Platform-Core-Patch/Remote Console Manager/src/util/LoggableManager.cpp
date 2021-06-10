#include "StdAfx.h"
#include "LoggableManager.h"
#include "Registry.h"


// static CLoggableImpl *pNoLoggingLoggable;
//static CLoggableImpl *pAlwaysLoggingLoggable;
//
//CLoggableMap CLoggableManager::m_mapLevels;


bool CLoggableImpl::IsDebug() const
{
    return m_bDebug;
}


bool CLoggableImpl::IsTracing(int _level) const
{
    return _level <= m_nLevel;
}


bool CLoggableImpl::IsTracing() const
{
    return GetTraceObject().isCaptureActive( m_dwMask ) != 0;
}


CLoggableImpl::CLoggableImpl(int _level)
    : m_bDebug(_level == LEVEL_DEBUG)
    , m_nLevel(_level)
    , m_dwMask(0)
{
}


CLoggableImpl::~CLoggableImpl()
{
}


CLoggableManager::CLoggableManager(HTRACE hTrace)
    : CBaseObject(hTrace)
    , m_alwaysLogging(LEVEL_DEBUG)
{
}


CLoggableManager::~CLoggableManager()
{
    try
    {
        CLoggableIterator iter = m_mapLevels.begin();
        for (;iter != m_mapLevels.end(); iter++)
            delete iter->second;
    }
    catch (const exception &exc)
        { ITRACE_EXCEPTION(exc); }
    catch (...)
        { ITRACE_DOT_ERROR(); }
}


ILoggable & CLoggableManager::getLoggable(const _TCHAR *pName)
{
    CLoggableImpl *pLoggable = &m_alwaysLogging;

    StringType loggableName = _T("Log.Level.");
    if (pName != NULL)
        loggableName.append(pName);

    CLoggableIterator iter = m_mapLevels.end();
    if (!m_mapLevels.empty())
        iter = m_mapLevels.find(loggableName.c_str());
    if (iter == m_mapLevels.end())
    {
        _TCHAR *pLevel = _tgetenv(loggableName.c_str());
        //if (pLevel == NULL)
        //{
        //    CRegistry<long> registry;

        //    pLoggable = new CLoggableImpl( registry.GetRegistryValue(HKEY_LOG_LEVEL, loggableName.c_str(), LEVEL_DEBUG) );

        //    m_mapLevels[loggableName.c_str()] = pLoggable;
        //}
        if (pLevel != NULL)
        {
            // int level = atoi(pLevel);

            pLoggable = new CLoggableImpl(LEVEL_DEBUG);

            m_mapLevels[loggableName.c_str()] = pLoggable;
        }
    }
    else
        pLoggable = dynamic_cast<CLoggableImpl*>(iter->second);

    pLoggable->SetTraceObject(GetTraceHandle());

//  ITRACE_DEVELOP("Returning loggable for:  " + IString(loggableName.c_str()));
    return *pLoggable;
}


ILoggable & CLoggableManager::getLoggable(DWORD dwMask)
{
    CLoggableImpl *pLoggable = &m_alwaysLogging;

    StringType loggableName = _T("DC");
    loggableName += (const _TCHAR *)IString(dwMask).d2x();

    CLoggableIterator iter = m_mapLevels.end();
    if (!m_mapLevels.empty())
        iter = m_mapLevels.find(loggableName.c_str());
    if (iter == m_mapLevels.end())
    {
        bool bActive = IsTraceEnabled() 
                            ? (GetTraceObject().isCaptureActive(dwMask) != 0) 
                            : true;
        pLoggable = new CLoggableImpl(bActive ? LEVEL_DEBUG : LEVEL_NONE);
        pLoggable->m_dwMask = dwMask;

        m_mapLevels[loggableName.c_str()] = pLoggable;
    }
    else
        pLoggable = dynamic_cast<CLoggableImpl *>(iter->second);

    pLoggable->SetTraceObject(GetTraceHandle());

//  ITRACE_DEVELOP("Returning loggable for:  " + IString(loggableName.c_str()));
    return *pLoggable;
}
