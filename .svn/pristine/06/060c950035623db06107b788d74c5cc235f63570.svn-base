#ifndef __CLASS_PTR_H__
#define __CLASS_PTR_H__


// TTypePtr & TClassPtr: Reference savvy managed pointer templates.
//
// Author E. Kobres
//
// Copyright 1997 NCR Corporation.
//
// Based on the HeapPtr managed pointer template class from:
//
// C++ FAQs - Marshal P. Cline and Greg A.Lomow
//
// Revision History
//
// Date       By   Comment
// ---------  ---  ------------------------------------------------------------
// 30 Dec 97  ECK  Initial Version



// TTypePtr is a reference-counted auto-deleting pointer class.
//
// TTypePtr is for use with built-in types.  It is not suitable for pointers to
// C++ classes or structures because, like built-in type pointers, it does not 
// support the arrow operator.  (Use TTypePtr for classes and structures, it 
// supports the arrow operator.)
//
// See the comments for each method for more information.

template <class TYPE>
class TTypePtr 
   {
   public:

// Standard ctor.  Optionally, specify the pointer to manage.
// We now own the managed poiner, so the reference count starts at 1.
   TTypePtr(TYPE* ptr=NULL) 
      : m_ptr(ptr), 
      m_pref(ptr == NULL ? NULL : new long(1)){}

// Dtor of derived calls Deallocate(), which may or may not delete the managed
// object.

   virtual ~TTypePtr()
      {
      Deallocate();
      }

// Assignment operator for pointers of TYPE.
// Since this is assigned from a TYPE * pointer, we must assume that we
// are the initial owners of the pointer.  (Otherwise the assignement
// would have been handled by operator=(const TTypePtr<TYPE>& ptr))
// Therefore we must allocate a brand new reference pointer for this.

   virtual TTypePtr<TYPE>& operator= (TYPE* ptr)
      {
      Deallocate();
      if ( ptr != NULL )
         m_pref = new long(1);
      m_ptr = ptr;
      return *this;
      }

// Assignment operator for other managed pointers.
// First we must give up any pointers we are currently managing.
// Next we assign our members to point to the members of the source.
// Finally, we increment the reference count.
// Note that this is safe since the source must either be an operand in the
// statement which causes the assignment, or a temporary object constructed
// by the compiler.  In either case, the source cannot be destroyed during 
// the copy unless the user has made a bogus (binary) copy of the safe pointer
// object and destroyed it.  If the reference count ends up at one, we are
// trying to copy an invalid or improperly managed pointer, so assert.

   virtual TTypePtr<TYPE>& operator=(const TTypePtr<TYPE>& ptr)
      {
      ASSERT( ptr.m_ptr == NULL || ptr.m_pref != NULL );  
      ASSERT( ptr.m_ptr != NULL || ptr.m_pref == NULL );  
      // Increment source first, so if we are in an a=a case, 
      // then Deallocate does nothing!
      if ( ptr.m_pref != NULL )
         VERIFY(::InterlockedIncrement(ptr.m_pref) != 1);
      Deallocate();
      m_pref = ptr.m_pref;
      m_ptr = ptr.m_ptr;
      ASSERT( m_ptr == NULL || m_pref != NULL );  
      ASSERT( m_ptr != NULL || m_pref == NULL );  
      return *this;
      }

// Copy constructor for other managed pointers.
// We are not managing anything since we start as dust.
// First we assign our members to the members of the source on the init list.
// Finally, we increment the reference count.
// Note that this is safe since the source must either be an operand in the
// statement which causes the assignment, or a temporary object constructed
// by the compiler.  In either case, the source cannot be destroyed during 
// the copy unless the user has made a bogus (binary) copy of the safe pointer
// object and destroyed it.  If the reference count ends up at one, we are
// trying to copy an invalid or improperly managed pointer, so verify as such.

   TTypePtr(const TTypePtr<TYPE>& ptr)	
      : m_pref(ptr.m_pref) , 
      m_ptr(ptr.m_ptr)
      {
      ASSERT( m_ptr == NULL || m_pref != NULL );  
      ASSERT( m_ptr != NULL || m_pref == NULL );  
      if ( m_pref != NULL )
         VERIFY(::InterlockedIncrement(m_pref) != 1);
      }

// Mutators - These members change 'this'

// This member will remove the managed pointer from management and
// return the original pointer unmanaged.  This member will return NULL if
// there are other references to this managed pointer.  (Serious No-no!) 

   virtual TYPE* RelinquishOwnership()
      {
      TYPE* old = NULL; 
      ASSERT( m_ptr == NULL || m_pref != NULL );  
      ASSERT( m_ptr != NULL || m_pref == NULL );  
      if ( m_pref != NULL
         && ::InterlockedDecrement( m_pref ) == 0 )
         {
         old = m_ptr; 
         delete m_pref;
         }
      m_ptr  = NULL; 
      m_pref = NULL;  
      return old;
      }

// Inspectors - These members do not change this, they are constant.

// Used for "pointer arithmetic".
   virtual TYPE* operator+(size_t stOffset) const {ASSERT(m_ptr != NULL);return (m_ptr + stOffset);}

// size_t does not work as a pointer addition operand in all cases, so we also 
// support DWORD.
   virtual TYPE* operator+(DWORD dwOffset) const {ASSERT(m_ptr != NULL);return (m_ptr + dwOffset);}

// Array access.
   virtual TYPE& operator[](size_t stOffset) const {ASSERT(m_ptr != NULL);return *(m_ptr + stOffset);}

// size_t does not work as an array argument in all cases, so we also support 
// DWORD.
   virtual TYPE& operator[](DWORD dwOffset) const {ASSERT(m_ptr != NULL);return *(m_ptr + dwOffset);}
// Dereference
   virtual TYPE& operator*() const {ASSERT(m_ptr != NULL);return *m_ptr;}

// Pointer. (since we are supposed to look like a pointer, derived classes
// which manage class or struct pointer should support the -> operator.)
// Arrow is not valid for c primitive types, however, so we leave it out of
// this base class.
// It looks like this:

// TYPE* operator->() const {ASSERT(m_ptr != NULL);return m_ptr;}


// Cast operator to TYPE.
// Warning!  The TYPE * casting operator can get you into big trouble
// if you assume that the pointer returned is valid beyond the 
// scope of the "owning" safe pointer object!  Its use should be
// limited to rigid APIs which require a TYPE * and do not store it.
// Also, making a copy will get you into trouble if you cast to a TYPE *
// and the assign to another managed pointer object.  (This will result in a 
// "double delete".)

//   virtual operator const TYPE* const () const {return m_ptr;}
   virtual operator TYPE* () const {return m_ptr;}

   protected:

// This method embodies the destruction of the actual entity we are managing.
// Derived classes should implement their pointer deletion logic here.
   virtual void DestroyPointer()
      {
      delete m_ptr;
      }

// Deallocate the pointer we are managing.  This may or may not mean we 
// need to delete the storage pointed to by our pointer.  We only delete if
// we are the last user.  

   virtual BOOL Deallocate()
      {
      BOOL bDeleted = FALSE;
      ASSERT( m_ptr == NULL || m_pref != NULL );  
      ASSERT( m_ptr != NULL || m_pref == NULL );  
      if (m_pref != NULL)			// Check for zombification.
         {
         if(::InterlockedDecrement(m_pref) == 0)
            {
            DestroyPointer();    // We are the last user, delete target.
            delete m_pref;       // We are the last user, delete ref count.
            bDeleted = TRUE;     // Notify caller we were the last referent.
            }
         m_pref = NULL;          // Clean up for the next assignment.
         m_ptr = NULL;			
         }
      return bDeleted;
      }	

   protected:
   TYPE *m_ptr;                  // Pointer we are managing.
   long *m_pref;                 // Pointer to a reference counter.
   };


// Use TClassPtr for pointers to classes or structures.

template <class TYPE>
class TClassPtr : public TTypePtr<TYPE>
   {
   public:
   TClassPtr(TYPE* ptr=NULL) : TTypePtr<TYPE>(ptr) {}
   virtual ~TClassPtr() {}

// Only pointers to class and struct types support the "->"  operator.
   TYPE* operator->() const {ASSERT(m_ptr != NULL);return m_ptr;}
   };

// Use TSyncPtr to provide safe multi-threaded access to TClassPtr and TTypePtr
// objects 
// Normally, only objects that are accessed on multiple threads need to use
// this class. Local copies, pass by value can be TClassPtr or TTypePtr objects
// which are more efficient because they are unsynchronized.

template <class TYPE>
class TSyncPtr 
   {
   public:
      TSyncPtr( void) { }
      TSyncPtr(TYPE& ptr ) 
         : m_cpObject( ptr ) { }
      operator TYPE() const
         {
         CSingleLock csAccess( &m_csAccess, TRUE );
         return m_cpObject;
         }
      TYPE& operator=(const TYPE& ptr)
         {
         CSingleLock csAccess( &m_csAccess, TRUE );
         m_cpObject = ptr;
         return m_cpObject;
         }
   private:
      TYPE                       m_cpObject;
      mutable CCriticalSection   m_csAccess;

   };


#endif


