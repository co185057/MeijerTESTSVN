//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  Library.h
//
// TITLE: General purpose classes/tools library declarations.
//
// Classes provided: PointerStack
//                   PointerQueue
//
// AUTHOR: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LIBRARY
#define _LIBRARY

//

//=======================================================================
//
// POINTER STACK
//
// A PointerStack is a template class implemented as a LIFO linked
// list of pointers of the type specified by the class template
// (instantiation.)  The type specified must be a pointer type.  For
// example, to declare a stack of string pointers, one might define
//  PointerStack<char*> stringStack;
//
// Public interface:
//
//  push: Pass a pointer to the object to push on the stack.
//  pop: 0 is returned if the stack is empty, otherwise a pointer to
//       the object at the top of the stack is returned.  The object
//       is not deleted.
//  isEmpty: Returns bool false if there are any pointers on the
//           stack, true otherwise.
//  getCount: Returns int value of number of pointers on the stack.
//  removeAll: Removes all pointers.  The objects pointed to are not
//             deleted.  No return value.
//  deleteAll: Removes all pointers and deletes the objects pointed to.
//             No return value.
//
//=======================================================================
template<class Type>
class PointerStack
{
public:
  // inline methods

  void  push(Type pElement)
	{ theStack.AddHead(pElement); }
	
  Type  pop()
	{ if (theStack.IsEmpty())
      return 0;
    return theStack.RemoveHead();
	}
	
  bool  isEmpty()
  { // IsEmpty returns BOOL, not bool, so convert
    return (theStack.IsEmpty() ? true : false);
  }
	
  int   getCount()
	{ return theStack.GetCount(); }
	
  void  removeAll()
	{ theStack.RemoveAll(); }

  void  deleteAll()
  {
    Type pElement;
    while ( ! theStack.IsEmpty() )
    {
      pElement = theStack.RemoveHead();
      delete pElement;
    }
	theStack.RemoveAll();
  }
	
  Type & PeekHead()
  {
	POSITION pos;

	pos = theStack.GetHeadPosition();
	ASSERT(pos != NULL); // We want to assert, not supposed to call if empty
	return  (Type)theStack.GetAt( pos );
  }
private:
  // Use composition rather than inheritance so as to disallow public
  // access to all the other CTypedPtrList and CPtrList methods, since
  // that interface would not necessarily be considered valid for a
  // stack.
  CTypedPtrList<CPtrList,Type> theStack;
	
};

//=====================================================================
//
// POINTER QUEUE
//
// A PointerQueue is a template class implemented as a LIFO linked
// list of pointers of the type specified by the class template
// (instantiation.)  The type specified must a pointer type.  For
// example, to declare a queue of string pointers, one might define
//  PointerQueue<char*> stateQueue;
//
// Public interface:
//
//  add: Pass a pointer to the object to add to the tail of the queue.
//  remove: 0 is returned if the queue is empty, otherwise a pointer
//          to the object at the head of the queue is returned.  The
//          object is not deleted.
//  isEmpty: Returns bool false if there are any pointers in the
//           queue, true otherwise.
//  getCount: Returns int value of number of pointers in the queue.
//  removeAll: Removes all pointers.  The objects pointed to are not
//             deleted.  No return value.
//  deleteAll: Removes all pointers and deletes the objects pointed to.
//             No return value.
//
//=====================================================================

template<class Type>
class PointerQueue
{
public:
  // inline methods
  void  add(Type pElement)
	{ theQueue.AddTail(pElement); }
	
  Type  remove()
	{ if (theQueue.IsEmpty())
	    return 0;
    return theQueue.RemoveHead();
	}
	
  bool  isEmpty()
	{ // IsEmpty returns BOOL, not bool, so convert
    return (theQueue.IsEmpty() ? true : false);
  }
	
  int   getCount()
	{ return theQueue.GetCount(); }
	
  void  removeAll()
	{ theQueue.RemoveAll(); }

  void deleteAll()
  {
    Type pElement;
    while ( ! theStack.IsEmpty() )
    {
      pElement = theStack.RemoveHead();
      delete pElement;
    }
  }

	
private:
  // Use composition rather than inheritance so as to disallow public
  // access to all the other CTypedPtrList and CPtrList methods, since
  // that interface would not necessarily be considered valid for a
  // queue.
  CTypedPtrList<CPtrList,Type> theQueue;
};

#endif