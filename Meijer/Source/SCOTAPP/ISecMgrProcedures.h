#ifndef _ISECMGRPROCEDURES_H
#define _ISECMGRPROCEDURES_H


class ISecMgrProcedures
{
public:

    ISecMgrProcedures() {}
    virtual ~ISecMgrProcedures() {}

    virtual long AppControl(long lCookie, LPCTSTR bstrParms, BSTR* bstrOutParms) = 0;
    //virtual void VoidItem(long lCookie, LPCTSTR bstrParms) = 0;

};

#endif // _ISECMGRPROCEDURES_H