///////////////////////////////////////////////////////////////////////////
///
/// \file SingleInstance.h
/// \brief interface for the CSingleInstance class
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_SINGLEINSTANCE_H__INCLUDED_)
#define AFX_SINGLEINSTANCE_H__INCLUDED_

// Forward Declaration Required - can't include headers, circular referenace
#include <afxtempl.h>
#include "traceSystem.h"

//////////////////////////////////////////////////////////////////////
///
/// CSafeArrayVariant is some common class so that we can store all singleton pointers 
///	 in a container. In TBUninitialize or some other suitable place of destruction, 
///	 we will "delete" all these pointers.
///	 Since the destructor is virtual, then we can simply "delete" pointers of this 
///	 class and they will destroy all the way down to the most derived class.
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////
class CBaseSingleInstance
{
public:
	//////////////////////////////////////////////////////////////////////
	///
	/// IsAutoCleanup, auto cleanup flag
	///
	///	@return bool, true if auto cleanup
	//////////////////////////////////////////////////////////////////////
	bool IsAutoCleanup()
	{
		return m_bAutoCleanup;
	}

	//////////////////////////////////////////////////////////////////////
	///
	/// SetVoidPtr, set the void pointer
	///
	///	@param void * - pointer to the object
	//////////////////////////////////////////////////////////////////////
  void SetVoidPtr(void* i_p)
  {
     m_pVoidPtr= i_p;
  }

  //////////////////////////////////////////////////////////////////////
	///
	/// GetVoidPtr, get the void pointer
	///
	///	@return void * - void pointer
	//////////////////////////////////////////////////////////////////////
	void* GetVoidPtr()
  {
     return m_pVoidPtr;
  }

	//////////////////////////////////////////////////////////////////////
	///
	/// CleanupAll, cleanup all the object with the void pointer
	///
	//////////////////////////////////////////////////////////////////////
	static CleanupAll()
	{
		 int nCount= m_aSingletonArray.GetSize();
		 for(int i=0; i< nCount; i++)
		 {
			 CBaseSingleInstance* pObject= m_aSingletonArray[i];
          void* pVoid= pObject->GetVoidPtr();
			 pObject->~CBaseSingleInstance();
          delete (pVoid);
		 }
		 m_aSingletonArray.RemoveAll();
	 }

	//////////////////////////////////////////////////////////////////////
	///
	/// Destructor
	///
	//////////////////////////////////////////////////////////////////////
  virtual ~CBaseSingleInstance()
  {
  }

protected:
  void* m_pVoidPtr; ///< void pointer

	static CArray<CBaseSingleInstance*, CBaseSingleInstance*> m_aSingletonArray; ///< template

	//////////////////////////////////////////////////////////////////////
	///
	/// Constructor
	///
	//////////////////////////////////////////////////////////////////////
	CBaseSingleInstance()
	{
		m_bAutoCleanup= true;
	}

	bool m_bAutoCleanup;	///< auto clean up flag

};


template<class T>
class CSingleInstance   : public CBaseSingleInstance
{

public:
   static T* instance()
	{ 
      if (!m_pSingleInstance)
		{
         m_pSingleInstance = new T;
			if(((CBaseSingleInstance*)m_pSingleInstance)->IsAutoCleanup())
			{
				CBaseSingleInstance* pBase= (CBaseSingleInstance*)m_pSingleInstance;

            m_pSingleInstance->SetVoidPtr((void*)m_pSingleInstance);

            m_aSingletonArray.Add(m_pSingleInstance);
            tbtrace(TM_MODULE_ID | TRACE_MASK_INFO, _T("Singleton"),__LINE__, _T("Singleton Allocated %d"), (int)m_pSingleInstance);
			}
		}
		return m_pSingleInstance; 
	}

protected:
	//////////////////////////////////////////////////////////////////////
	///
	/// Constructor
	///
	//////////////////////////////////////////////////////////////////////
	CSingleInstance();

	//////////////////////////////////////////////////////////////////////
	///
	/// Destructor
	///
	//////////////////////////////////////////////////////////////////////
	virtual ~CSingleInstance();


	// hide copy and assignment
	//////////////////////////////////////////////////////////////////////
	///
	/// Hide copy const
	///
	//////////////////////////////////////////////////////////////////////
	CSingleInstance(const CSingleInstance&);

	//////////////////////////////////////////////////////////////////////
	///
	/// Hide assignment
	///
	//////////////////////////////////////////////////////////////////////
	CSingleInstance& operator = (const CSingleInstance &);
private:
   static T *m_pSingleInstance; 


};

template <class T>
CSingleInstance<T>::CSingleInstance()
{
}

template <class T>
CSingleInstance<T>::~CSingleInstance()
{
   m_pSingleInstance= NULL; 
}

template<class T>
T* CSingleInstance<T>::m_pSingleInstance=NULL;

#endif // !defined(AFX_SINGLEINSTANCE_H__6FE61490_7CA7_4777_815C_C2D90C5D29E3__INCLUDED_)
