// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_lock_h__
#define __xcore_c_lock_h__

#include "xcore_c_impl.h"

/** Type that denotes a hardware lock which provide a mutex function.
 *
 */
typedef int lock;

#if !defined(__XC__) || defined(__DOXYGEN__)

/** Allocates a lock.
 *
 *  This function allocates a hardware lock.
 *  If there are no locks availble the function returns 0.
 *  When the lock is no longer required, lock_free() must be called
 *  to deallocate it.
 *
 *  \param l    lock variable representing the initialised lock
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LOAD_STORE         invalid *l argument.
 */
inline unsigned lock_alloc(lock *l)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                asm volatile("getr %0, 5" : "=r" (*l)); \
                              } while (0) );
}

/** Deallocate a lock.
 *
 *  This function frees the hardware lock.
 *  The lock must be released prior to calling this function.
 *
 *  \param l    The lock to be freed
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated lock,
 *                                    or the lock has not been released.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the lock.
 *  \exception  ET_LOAD_STORE         invalid *l argument.
 */
inline unsigned lock_free(lock *l)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                asm volatile("freer res[%0]" :: "r" (*l)); \
                                *l = 0; \
                              } while (0) );
}

/** Acquire a lock.
 *
 *  Only one core at a time can acquire a lock. This provides a hardware mutex
 *  which have very low overheads. If another thread has already acquired this
 *  lock then this function will pause until the lock is released and this core
 *  becomes the owner.
 *
 *  \param l    The lock to acquire
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated lock.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the lock.
 */
inline unsigned lock_acquire(lock l)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                int dummy; \
                                asm volatile("in %0, res[%1]" : "=r" (dummy): "r" (l)); \
                              } while (0) );
}

/** Release a lock.
 *
 *  This releases the lock and allocates the next owner from the list of waiting
 *  cores in round-robin manner.
 *  *Note*: there are no checks that the core releasing the lock is the current
 *  owner.
 *
 *  \param l    The lock to use release
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated lock.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the lock.
 */
inline unsigned lock_release(lock l)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                asm volatile("out res[%0], %0" :: "r" (l)); \
                              } while (0) );
}

#endif // __XC__

#endif // __xcore_c_lock_h__
