#ifndef _ISCOTWINAPP_H
#define _ISCOTWINAPP_H


/**
 * \brief Interface for accessing configuration information.
 */
class IScotWinApp
{
public:
    IScotWinApp() {}
    virtual ~IScotWinApp() {}

    virtual CString GetProfileString(LPCTSTR, LPCTSTR, LPCTSTR = NULL) = 0;
    virtual int GetProfileInt(LPCTSTR, LPCTSTR, int) = 0;
    virtual LPCTSTR GetProfileName() = 0;
    virtual void SetProfileName(LPCTSTR) = 0;
};

#endif // _ISCOTWINAPP_H
