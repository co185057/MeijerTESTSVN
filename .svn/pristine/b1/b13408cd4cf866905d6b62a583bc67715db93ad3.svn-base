// MethodTrace.h: Mechanism for tracing entry/exit from method.
//
//////////////////////////////////////////////////////////////////////

#ifndef __METHOD_TRACE__
#define __METHOD_TRACE__

#define TRACE_METHOD(modname) MethodTrace _mm(modname, 6);
#define TRACE_METHOD_AT_LEVEL(modname, lTraceLevel) MethodTrace _mm(modname, lTraceLevel);
class MethodTrace
{
public:

    MethodTrace(const _TCHAR *modName, long lTraceLevel) 
	{ 
		m_modName = modName; 
        m_lTraceLevel = lTraceLevel;
		trace(TRACE_FIXED_HEADER, m_lTraceLevel, _T(">>> %s"), m_modName);
	}

    virtual ~MethodTrace()
	{ 
		trace(TRACE_FIXED_HEADER, m_lTraceLevel, _T("<<< %s"), m_modName);
	}

private:

    MethodTrace(const MethodTrace &);
    MethodTrace &operator =(const MethodTrace &); // Hidden assignment operator

    CString m_modName;
    long m_lTraceLevel;

};

#endif
