#ifndef _IRAPROCEDURES_H
#define _IRAPROCEDURES_H

/**
 * \interface IRAProcedures
 */
class IRAProcedures
{
public:
    IRAProcedures() {}
    virtual ~IRAProcedures() {}

    virtual void RequestEnableAssistMode(bool) = 0;
};

#endif // _IRAPROCEDURES_H
