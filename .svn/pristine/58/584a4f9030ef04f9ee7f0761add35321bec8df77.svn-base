#ifndef _CDCAP_METHOD_TRACE_H
#define _CDCAP_METHOD_TRACE_H

#define DCTRACE_METHOD       0x00000010 // Trace entry/exit of method
#define DCTRACE_METHOD_EXT   0x00000020 // Extensive trace entry/exit of method.
#define DCTRACE_NORMAL       0x00000040

#define DCAP_API_TRACE(module) CDCAPMethodTrace _xscopeTrace(module)
#define DCAP_API_TRACE_EXT(module) \
    CDCAPMethodTrace _xscopeTrace(module, DCTRACE_METHOD_EXT)

/**
 * \brief Simple class to trace entry/exit of a method.  This is similar
 *        to the existing MethodTrace class but it uses CmDataCapture instead
 *        of TraceObject.
 *
 * \note  Added for diagnostic tracing of Dr. Watson that can occur if there
 *        is a device error at startup.  See RFC 445940
 */
class CmDataCapture;
class CDCAPMethodTrace
{
public:
    /**
     * Trace entry and exit of a method using data capture.
     * \param[in] szModuleName Name of the method to be traced.
     */
    CDCAPMethodTrace(LPCTSTR szModuleName, long level=DCTRACE_METHOD);
    virtual ~CDCAPMethodTrace();

    /**
     * Set the data capture object to use for tracing.
     * \param[in] dc Pointer to initialized, ready-to-go CmDataCapture object.
     */
    static void SetDC(CmDataCapture *dc);

protected: 
    // Add protected copy ctor and assignment operator to prevent copies.
    CDCAPMethodTrace(const CDCAPMethodTrace &rhs);
    CDCAPMethodTrace & operator=(const CDCAPMethodTrace &rhs);

    static CmDataCapture *m_spDC;

    CString m_csModule;
    long m_lLevel;
};

#endif // _CDCAP_METHOD_TRACE_H
