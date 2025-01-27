/*=========================================================================

 Program:   Visualization Toolkit
 Module:    vtkSMPThreadLocal.h

 Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
 All rights reserved.
 See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

    This software is distributed WITHOUT ANY WARRANTY; without even
    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkSMPThreadLocal
 * @brief   Thread local storage for VTK objects.
 *
 * A thread local object is one that maintains a copy of an object of the
 * template type for each thread that processes data. vtkSMPThreadLocal
 * creates storage for all threads but the actual objects are created
 * the first time Local() is called. Note that some of the vtkSMPThreadLocal
 * API is not thread safe. It can be safely used in a multi-threaded
 * environment because Local() returns storage specific to a particular
 * thread, which by default will be accessed sequentially. It is also
 * thread-safe to iterate over vtkSMPThreadLocal as long as each thread
 * creates its own iterator and does not change any of the thread local
 * objects.
 *
 * A common design pattern in using a thread local storage object is to
 * write/accumulate data to local object when executing in parallel and
 * then having a sequential code block that iterates over the whole storage
 * using the iterators to do the final accumulation.
 *
 * @warning
 * There is absolutely no guarantee to the order in which the local objects
 * will be stored and hence the order in which they will be traversed when
 * using iterators. You should not even assume that two vtkSMPThreadLocal
 * populated in the same parallel section will be populated in the same
 * order. For example, consider the following
 *
 * @code
 * vtkSMPThreadLocal<int> Foo;
 * vtkSMPThreadLocal<int> Bar;
 * class AFunctor
 * {
 *    void Initialize() const
 *    {
 *        int& foo = Foo.Local();
 *        int& bar = Bar.Local();
 *        foo = random();
 *        bar = foo;
 *    }
 *
 *    void operator()(vtkIdType, vtkIdType) const
 *    {}
 * };
 *
 * AFunctor functor;
 * vtkParalllelUtilities::For(0, 100000, functor);
 *
 * vtkSMPThreadLocal<int>::iterator itr1 = Foo.begin();
 * vtkSMPThreadLocal<int>::iterator itr2 = Bar.begin();
 * while (itr1 != Foo.end())
 * {
 *   assert(*itr1 == *itr2);
 *   ++itr1; ++itr2;
 * }
 * @endcode
 *
 * @warning
 * It is possible and likely that the assert() will fail using the TBB
 * backend. So if you need to store values related to each other and
 * iterate over them together, use a struct or class to group them together
 * and use a thread local of that class.
 *
 * @sa
 * vtkSMPThreadLocalObject
 */

#ifndef vtkSMPThreadLocal_h
#define vtkSMPThreadLocal_h

#include "SMP/Common/vtkSMPThreadLocalAPI.h"

template <typename T>
class vtkSMPThreadLocal
{
public:
  /**
   * Default constructor. Creates a default exemplar.
   */
  vtkSMPThreadLocal()
    : ThreadLocalAPI()
  {
  }

  /**
   * Constructor that allows the specification of an exemplar object
   * which is used when constructing objects when Local() is first called.
   * Note that a copy of the exemplar is created using its copy constructor.
   */
  explicit vtkSMPThreadLocal(const T& exemplar)
    : ThreadLocalAPI(exemplar)
  {
  }

  /**
   * This needs to be called mainly within a threaded execution path.
   * It will create a new object (local to the thread so each thread
   * get their own when calling Local) which is a copy of exemplar as passed
   * to the constructor (or a default object if no exemplar was provided)
   * the first time it is called. After the first time, it will return
   * the same object.
   */
  T& Local() { return this->ThreadLocalAPI.Local(); }

  /**
   * Return the number of thread local objects that have been initialized
   */
  size_t size() { return this->ThreadLocalAPI.size(); }

  /**
   * Subset of the standard iterator API.
   * The most common design pattern is to use iterators in a sequential
   * code block and to use only the thread local objects in parallel
   * code blocks.
   * It is thread safe to iterate over the thread local containers
   * as long as each thread uses its own iterator and does not modify
   * objects in the container.
   */
  typedef typename vtk::detail::smp::vtkSMPThreadLocalAPI<T>::iterator iterator;

  /**
   * Returns a new iterator pointing to the beginning of
   * the local storage container. Thread safe.
   */
  iterator begin() { return this->ThreadLocalAPI.begin(); };

  /**
   * Returns a new iterator pointing to past the end of
   * the local storage container. Thread safe.
   */
  iterator end() { return this->ThreadLocalAPI.end(); }

private:
  vtk::detail::smp::vtkSMPThreadLocalAPI<T> ThreadLocalAPI;

  // disable copying
  vtkSMPThreadLocal(const vtkSMPThreadLocal&) = delete;
  void operator=(const vtkSMPThreadLocal&) = delete;
};

#endif
// VTK-HeaderTest-Exclude: vtkSMPThreadLocal.h
