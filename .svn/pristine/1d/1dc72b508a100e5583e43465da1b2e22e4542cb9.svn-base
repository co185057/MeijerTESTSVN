#pragma once
// $Header: /Fastlane/4.0 Patch B/Component Utilities/src/util/Singleton.h 1     1/26/05 2:32p Dm185016 $
/*
 * @(#)Singleton.h    $Revision: 1 $ $Date: 1/26/05 2:32p $
 *
 * Copyright 2004 by NCR Corporation,
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
* $Log: /Fastlane/4.0 Patch B/Component Utilities/src/util/Singleton.h $
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 7     6/30/04 3:46p Dm185016
* Fix for derived Singleton classes.
* 
* 6     5/18/04 7:20p Dm185016
* lint
* 
* 5     4/12/04 3:32p Dm185016
* Suppressed warning
* 
* 4     3/19/04 3:33p Dm185016
* Memory leak fix
* 
* 3     1/23/04 3:43p Dm185016
* Added hdr
*/

// Forward Declaration Required - can't include headers, circular referenace
////

typedef void (*PSINGLETON_RELEASE)(void);

#pragma warning( push )
#pragma warning( disable : 4311 )

template<class T>
class CSingleton
{

public:
   static T* instance()
	{ 
        if (!m_pSingleton)
        {
            m_pSingleton = new T;
//          ITRACE_DEVELOP(QUOTE(T) _T("  instance() allocated:  ") + IString((long)m_pSingleton).d2x());
		}
		return m_pSingleton; 
	}

    static void Release()
    {
//      IMODTRACE_DEVELOP();

//      ITRACE_DEVELOP(QUOTE(T) _T("  Release() released:  ") + IString((long)m_pSingleton).d2x());
//      ITRACE_DUMP(QUOTE(T), (const _TCHAR *)m_pSingleton, sizeof(T));

        delete m_pSingleton;

        m_pSingleton = NULL;
    }

protected:
	CSingleton();
	virtual ~CSingleton();

    static void SetInstance(T *pSingleton)
	{ 
        m_pSingleton = pSingleton;
	}

private:
    static T *m_pSingleton; 

   // hide copy and assignment
	CSingleton(const CSingleton&);
	CSingleton& operator = (const CSingleton &);

};

template <class T>
CSingleton<T>::CSingleton()
{
}

template <class T>
CSingleton<T>::~CSingleton()
{
    m_pSingleton= NULL; 
}

template<class T>
T* CSingleton<T>::m_pSingleton=NULL;

#pragma warning( pop )
