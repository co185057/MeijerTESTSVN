#ifndef _IMSGOBJECT_H
#define _IMSGOBJECT_H

class IMsgObject
{
public:
    IMsgObject() {}
    virtual ~IMsgObject() {}

    //virtual void PostDmEventToApplication(long nDeviceClass,
    //                                      long nDeviceId,
    //                                      const CString& sData) = 0;

    //virtual long PostSecMgr(const int nEventType, const CString &csParms) = 0;

    /*virtual long PostDM(const int  Msg,
                const int  Class,
                int  DeviceId,
                const long lCodeOrLength,
                const void * vDPtr,
                const long lDirectIONumber = 0,
                const long lDirectIONumData = 0) =0;*/

    //virtual HWND GetHWnd(void) const = 0;
};

#endif // _IMSGOBJECT_H
