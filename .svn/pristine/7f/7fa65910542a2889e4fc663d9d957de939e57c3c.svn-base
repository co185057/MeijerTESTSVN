#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/RuleSet.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)RuleSet.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
 *
 * Copyright 2005 by NCR Corporation,
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/RuleSet.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     3/28/05 5:08p Dm185016
 * Lint
 * 
 * 4     3/24/05 11:37a Dm185016
 * If conditional (<if> or <select>) was first in the rulelist, then the
 * expression was not evaluated and the NullAction was executed instead of
 * the attached rulelist.
 * 
 * 3     3/11/05 1:35p Dm185016
 * <select> support
 * 
 * 2     2/28/05 3:53p Dm185016
 * lint
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 2     1/21/05 4:45p Dm185016
* Fixed memory leak.
* 
* 1     1/12/05 6:24p Dm185016
* Select support.
*/

//lint --e{1732, 1733}
//lint --e{1759}            By design, the postfix increment operator returns a reference
//lint --e{423}             No memory leak here

#include <stack>
#include "SMConstants.h"
#include "Rule.h"
#include "statem.h"
#include "ModifiableRule.h"

using namespace std;

namespace STATE_MACHINE_NAMESPACE
{


//
//  Select statements and its associated When clauses are arranged in the following manner:
//  
//  CRulesList
//        |  
//        V             0                  1                  2
//  +-----------+       +----------+       +----------+       +----------+
// 0|  When     |------>|  Action  |------>|  Action  |------>|  Action  |---------+
//  +-----------+       +----------+       +----------+       +----------+         |
//        |                                                                        |
//        |                                                                        |
//        V             0                                                          |
//  +-----------+       +----------+                                               |
// 1|  When     |------>|  Action  |-----------------------------------------------+ 
//  +-----------+       +----------+                                               |
//        |                                                                        |
//        |                                                                        |
//        V            0                   1                                       |
//  +-----------+       +----------+       +----------+                            |
// 2|  When     |------>|  Action  |------>|  Action  |----------------------------+
//  +-----------+       +----------+       +----------+                            |
//        |                                                                        |
//        |                                                                        |
//        V                                                                        |
//  +-----------+                                                                  |
// 3|  Select   |<-----------------------------------------------------------------+
//  +-----------+
//
// The virtical chain on the left represents the original CRulesList.  Each side chain
// is also a CRulesList.  The side chain is only executed if the expression for the When
// clause returns a true.  Once the side chain completely executes, then the next action 
// to be executed will be the select at 3.
//
class STATEM_API CRuleSet
{
public:

    typedef struct __node
    {
        int first;                      // Index of the next action in the chain
        CRulesList *second;             // The CRulesList currently being executed
        int third;                      // When execution resumes, this is the return
                                        // index into the chain.

        __node()
            : first(0)                  // always start at the first in the chain
            , second(NULL)              // assume the action has no side chain
            , third(-1)                 // If a side chain is being executed, then this
                                        //      will be the next index after return 
                                        //      from executing side chain
        {}

    } _node;


    typedef std::stack<_node *> CRulesStack;


    /// <summary>Constructor for this iterator.  Takes a collection of rules.</summary>
    /// <param name="pRules">Ptr to the rules collection</param>
    CRuleSet(CRulesList *pRules)
        : m_pnode(new _node)
    {
        node().second   = pRules;
        m_rulesStack.push(m_pnode);

        Next(false);
    }


    /// <summary>Destructor for this iterator.</summary>
    ~CRuleSet()
    {
        try
        {
            _node *pnode;
            while (!m_rulesStack.empty())
            {
                pnode = m_rulesStack.top();
                m_rulesStack.pop();

                delete pnode;
            }
        }
        catch (...) { }

        m_pnode = NULL;
    }

    /// <summary>Returns a reference to the next rule in the collection.</summary>
    inline IRule *operator->()
    {
        _ASSERTE(!complete());
        if (complete())
            return NULL;

        IRule *pRule = rules().at(node().first);

        return pRule;
    }
    

    /// <summary>Returns a reference to the next rule in the collection.</summary>
    inline IRule *operator*()
    {
        _ASSERTE(!complete());
        if (complete())
            return NULL;

        return rules().at(node().first);
    }
    

    /// <summary>Advances the iterator to the next rule in the collection.</summary>
    inline CRuleSet & operator ++(int)
    {
        Next();

        return *this;
    }
    

    /// <summary>Prefix operator ++ is used to get the next value using 
    /// the same key as the previous usage</summary>
    inline CRuleSet & operator ++()
    {
        Next();

        return *this;
    }


    /// <summary>Returns true if we have reached the end of the current rules list.</summary>
    inline bool complete() const
    {
        return (const_node().first >= (int)rules().size());
    }

protected:    

    const CRulesList &rules() const { return *const_node().second; }
    _node &node() { return *m_pnode; }
    const _node &const_node() const { return *m_pnode; }

    
    inline void NextRule(bool bIncrement = true)
    {
        if (bIncrement)
            node().first++;

        while (complete())
        {
            if (m_rulesStack.size() <= 1)
               return;

            delete m_pnode;

            m_rulesStack.pop();

            m_pnode = m_rulesStack.top();

            int nIndex = node().third;
            if (nIndex == -1)
                node().first++;
            else
                node().first = nIndex;
        }
    }

    
    inline void Next(bool bIncrement = true)
    {
        NextRule(bIncrement);

        IRule *pRule = NULL;
        while (!complete())
        {
            pRule = operator*();
            if (pRule->IsActive())
            {
                if (pRule->IsRulesListActive())
                {
                    node().third    = dynamic_cast<IModifiableRule *>(pRule)->GetNextRuleIndex();

                    m_pnode = new _node;        //lint !e672    No memory leak.  stack owns all nodes
                    node().second   = &pRule->GetRulesList();
                    m_rulesStack.push(m_pnode);

                    NextRule(false);
                }
                else
                    return;
            }
            else
                NextRule();
        }
    }

    _node *m_pnode;

    CRulesStack m_rulesStack;

private:

    CRuleSet();

};

}
