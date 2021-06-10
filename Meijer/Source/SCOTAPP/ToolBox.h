#ifndef _CTOOLBOX_H
#define _CTOOLBOX_H

#include "IToolBox.h"

/**
 * \class CToolBox.
 * \brief Implementation of IToolBox interface.
 */
class CToolBox : public IToolBox
{
public:
    friend class IToolBox;

    virtual ICustomerInfo & GetCustomerInfo(void);

    virtual IReporting & GetReporting(void);

    virtual ISecMgrProcedures & GetSecMgrProcedures(void);

    virtual IRAProcedures & GetRAProcedures(void);

    virtual IPSProcedures & GetPSProcedures(void);

    virtual IDMProcedures & GetDMProcedures(void);

    virtual IMsgObject & GetMsgObject(void);

    virtual IConfigObject & GetConfigObject(void);

    virtual IItemObject & GetItemObject(void);

    //virtual IApplicationModel & GetApplicationModel(void);

    //virtual void SetApplicationModel(IApplicationModel *pAppModel);

protected:
    CToolBox();
    virtual ~CToolBox();

    virtual void Initialize();
    virtual void UnInitialize(void) throw();

private:

    // Prevent copying.
    //lint -esym(1526, CToolBox::CToolBox)
    CToolBox(const CToolBox &rhs);

    //lint -esym(1526, CToolBox::operator=)
    CToolBox & operator=(const CToolBox &rhs);

    //IApplicationModel *m_pAppModel;
};

#endif // _CTOOLBOX_H
