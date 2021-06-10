#include "StdAfx.h"
#include "IToolBox.h"

#ifdef _CPPUNIT
	#include "FakeToolBox.h"
#else
	#include "ToolBox.h"
#endif

IToolBox *IToolBox ::m_spToolBox = NULL;

IToolBox & IToolBox ::GetInstance(void)
{
    if(! m_spToolBox)
    {
#ifdef _CPPUNIT
		m_spToolBox = new CFakeToolBox();
#else
		m_spToolBox = new CToolBox();
#endif        
        m_spToolBox->Initialize();
    }

    return *m_spToolBox;
}

void IToolBox ::SetInstance(IToolBox &toolBox)
{
    if(m_spToolBox)
    {
        Destroy();
    }

    m_spToolBox = &toolBox;
}

void IToolBox::Destroy(void)
{
    if(! m_spToolBox)
    {
        return;     // Nothing to do!
    }

    m_spToolBox->UnInitialize();
    delete m_spToolBox; 
    m_spToolBox = NULL;
}
