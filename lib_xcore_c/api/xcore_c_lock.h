// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_lock_h__
#define __xcore_c_lock_h__

#include <xccompat.h>

/** Type that denotes a hardware lock which provide a mutex function.
 *
 */
typedef int lock;

#if !defined(__XC__) || defined(__DOXYGEN__)

/** Allocates a lock.
 *
 *  This function allocates a hardware lock. If there are no locks availble,
 *  then the function will return NULL. When the lock is no longer required,
 *  lock_free() should be called to free the lock.
 *
 *  \returns   The lock or NULL if there are no locks available.
 */
inline lock lock_alloc(void)
{
  lock l;
  asm volatile("getr %0, 5" : "=r" (l));
  return l;
}

/** Deallocate a lock.
 *
 *  This function frees a lock. If the lock is currently owned by a core
 *  (has been acquired but not released) then this function will trap.
 *
 *  \param l  The lock to be freed
 */
inline void lock_free(lock l)
{
  asm volatile("freer res[%0]" :: "r" (l));
}

/** Acquire a lock.
 *
 *  Only one core at a time can acquire a lock. This provides a hardware mutex
 *  which have very low overheads. If another thread has already acquired this
 *  lock then this function will pause until the lock is released and this core
 *  becomes the owner.
 *
 *  \param l    The lock to acquire
 */
inline void lock_acquire(lock l)
{
  int i;
  asm volatile("in %0, res[%1]" : "=r" (i): "r" (l));
}

/** Release a lock.
 *
 *  This releases the lock and allocates the next owner from the list of waiting
 *  cores in round-robin manner.
 *  *Note*: there are no checks that the core releasing the lock is the current
 *  owner.
 *
 *  \param l  The lock to use release
 */
inline void lock_release(lock l)
{
  asm volatile("out res[%0], %0" :: "r" (l));
}

#endif // __XC__

#endif // __xcore_c_lock_h__
