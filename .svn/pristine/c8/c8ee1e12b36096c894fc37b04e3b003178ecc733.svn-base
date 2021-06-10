#ifndef _SSF_LIBRARY_HELPER_H
#define _SSF_LIBRARY_HELPER_H

class CWnd;

/**
 * \class CLibraryHelper
 * \brief Helper class for loading / freeing dynamic-linked libraries.
 */
class CLibraryHelper
{
public:
    CLibraryHelper();
    virtual ~CLibraryHelper();

    virtual HWND FindWindow(LPCTSTR lpszClasName, LPCTSTR lpszWindowName);

    virtual HMODULE LoadLibrary(LPCTSTR lpFileName);
    virtual BOOL FreeLibrary(HMODULE hModule);
    virtual FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName);
};

#endif // _SSF_LIBRARY_HELPER_H
