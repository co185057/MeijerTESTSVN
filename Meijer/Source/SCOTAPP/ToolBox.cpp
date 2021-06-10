#include "StdAfx.h"

#include "ToolBox.h"

#include "Common.h"

#include "ICustomerInfo.h"
#include "IReporting.h"
#include "ISecMgrProcedures.h"
#include "IRAProcedures.h"// TODO:
#include "IPSProcedures.h"// TODO:
#include "IDMProcedures.h"// TODO:
#include "IMsgObject.h"
#include "IConfigObject.h" // TODO:
#include "IItemObject.h"
//#include "IApplicationModel.h"

//#include "ApplicationModel.h"


CToolBox::CToolBox() //:
    //m_pAppModel(NULL)
{
} 

CToolBox::~CToolBox()
{
    CToolBox::UnInitialize();
}

void CToolBox::Initialize()
{    
}

void CToolBox::UnInitialize(void) throw()
{
    try
    {

        //delete m_pAppModel;
        //m_pAppModel = NULL;
    }
    catch(...)
    {
    }
}

ICustomerInfo & CToolBox::GetCustomerInfo(void)
{
    return custInfo;
}

IReporting & CToolBox::GetReporting(void)
{
    return rp;
}

ISecMgrProcedures & CToolBox::GetSecMgrProcedures(void)
{
    return SecMgr;
}

IRAProcedures & CToolBox::GetRAProcedures(void)
{
    return ra;
}

IPSProcedures & CToolBox::GetPSProcedures(void)
{
    return ps;
}

IDMProcedures & CToolBox::GetDMProcedures(void)
{
    return dm;
}

IMsgObject & CToolBox::GetMsgObject(void)
{
    return mo;
}

IConfigObject & CToolBox::GetConfigObject(void)
{
    return co;
}

IItemObject & CToolBox::GetItemObject(void)
{
    return io;
}

/*IApplicationModel & CToolBox::GetApplicationModel(void)
{
    if(! m_pAppModel)
    {
        m_pAppModel = new CApplicationModel();
    }

    return *m_pAppModel;
}

void CToolBox::SetApplicationModel(IApplicationModel *pAppModel)
{
    if(m_pAppModel != pAppModel)
    {
        delete m_pAppModel;
        m_pAppModel = pAppModel;
    }
}*/