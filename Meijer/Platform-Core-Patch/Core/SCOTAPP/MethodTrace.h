// MethodTrace.h: Mechanism for tracing entry/exit from method.
//
//////////////////////////////////////////////////////////////////////

#ifndef __METHOD_TRACE__
#define __METHOD_TRACE__

#define TRACE_METHOD(modname) MethodTrace _mm(modname);
class MethodTrace
{
public:

    MethodTrace(const _TCHAR *modName) 
	{ 
		m_modName = modName; 
		trace(L6, _T(">>> %s"), m_modName);
	}

    virtual ~MethodTrace()
	{ 
		trace(L6, _T("<<< %s"), m_modName);
	}

private:

    MethodTrace(const MethodTrace &);
    MethodTrace &operator =(const MethodTrace &); // Hidden assignment operator

    CString m_modName;

};

#endif
