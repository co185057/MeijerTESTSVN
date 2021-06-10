#ifndef _ITOOLBOX_H
#define _ITOOLBOX_H

class ICustomerInfo;
class IReporting;
class ISecMgrProcedures;
class IRAProcedures;
class IPSProcedures;
class IDMProcedures;
class IMsgObject;
class IConfigObject;
class IItemObject;

//class IApplicationModel;

/**
 * \interface IToolBox.
 * \brief Singleton that provides access to global helper classes.
 */
class IToolBox
{
public:
    static IToolBox & GetInstance(void);

    /**
     * Allow a particular tool box implementation to be specified.
     * Mostly used for testing.
     * \param[in] toolBox Implementation of IToolBox to use.
     */
    static void SetInstance(IToolBox &toolBox);

    static void Destroy(void);

    /**
     * \return A ICustomerInfo implementation to use for interacting with
     *         the Personalization.
     */
    virtual ICustomerInfo & GetCustomerInfo(void) = 0;

    /**
     * \return A IReporting implementation to use for interacting with
     *         the Reporting.
     */
    virtual IReporting & GetReporting(void) = 0;

    /**
     * \return A ISecMgrProcedures implementation to use for interacting with
     *         the security manager.
     */
    virtual ISecMgrProcedures & GetSecMgrProcedures(void) = 0;

    /**
     * \return A IRAProcedures implementation to use for interacting with
     *         the remote attendant station.
     */
    virtual IRAProcedures & GetRAProcedures(void) = 0;

    /**
     * \return A IPSProcedures implementation to use for interacting with
     *         the UI.
     */
    virtual IPSProcedures & GetPSProcedures(void) = 0;

   /**
     * \return A IDMProcedures implementation to use for interacting with
     *         the device.
     */
    virtual IDMProcedures & GetDMProcedures(void) = 0;



    /**
     * \return A MsgObject implementation to use for interacting with
     *         the msg object.
     */
    virtual IMsgObject & GetMsgObject(void) = 0;

    /**
     * \return A ConfigObject implementation to use for interacting with
     *         the config object.
     */
    virtual IConfigObject & GetConfigObject(void) = 0;

    /**
     * \return An ItemObject implementation to use for interacting with
     *         the intem object.
     */
    virtual IItemObject & GetItemObject(void) = 0;

    /**
     * \return An ApplicationModel implementation
     */
    //virtual IApplicationModel & GetApplicationModel(void) = 0;

    /**
     * Set the implementation of IApplicationModel to be used by the toolbox.
     * \param[in] pAppModel Pointer to implementation to be used.
     */
    //virtual void SetApplicationModel(IApplicationModel *pAppModel) = 0;

protected:
    IToolBox() {}
    virtual ~IToolBox(){}

	virtual void Initialize() = 0;
    virtual void UnInitialize(void) = 0;

private:
    static IToolBox *m_spToolBox;
};

#endif // _ITOOLBOX_H
