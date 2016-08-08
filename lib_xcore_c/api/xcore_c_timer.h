// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_timer_h__
#define __xcore_c_timer_h__

#include "xcore_c_impl.h"

#if !defined(__XC__) || defined(__DOXYGEN__)

/** Allocates and initialise a timer.
 *
 *  This function allocates a hardware timer.
 *  If there are no timers availble, then the function will return 0.
 *  This macro is to be called once on every variable of the type ``timer``.
 *  When the timer is no longer required, timer_free() must be called
 *  to deallocate it.
 *
 *  \param t    Timer variable representing the initialised timer
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LOAD_STORE         invalid *t argument.
 */
inline unsigned timer_alloc(timer *t)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                asm volatile("getr %0, 1" : "=r" (*t)); \
                              } while (0) );
}

/** Deallocate a timer.
 *
 *  This function frees the hardware timer.
 *
 *  \param t    The timer to be freed
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 *  \exception  ET_LOAD_STORE         invalid *t argument.
 */
inline unsigned timer_free(timer *t)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                asm volatile("freer res[%0]" :: "r" (*t)); \
                                *t = 0; \
                              } while (0) );
}

/** Get the current time from the timer.
 *
 *  \param t    The timer on which to input
 *
 *  \param now  The time value (a 32-bit value)
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 *  \exception  ET_LOAD_STORE         invalid *now argument.
 */
inline unsigned timer_get_time(timer t, int *now)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                asm volatile("in %0, res[%1]" : "=r" (*now): "r" (t)); \
                              } while (0) );
}

/** Wait until after a specified time.
 *
 *  \param t  The timer to use for timing
 *
 *  \param until The time to wait until
 *
 *  \param now  The time we actually waited until
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 *  \exception  ET_LOAD_STORE         invalid *now argument.
 */
inline unsigned timer_wait_until(timer t, int until, int *now)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                asm volatile("setd res[%0], %1" :: "r" (t), "r" (until)); \
                                asm volatile("setc res[%0], 0x9" :: "r" (t)); \
                                asm volatile("in %0, res[%1]" : "=r" (*now): "r" (t)); \
                                /* Clear the condition so that a timer_get_time() will work by default */ \
                                asm volatile("setc res[%0], 0x1" :: "r" (t)); \
                              } while (0) );
}

/** Delay for a specified time using a specific timer.
 *
 *  \param t    The timer resource to use
 *
 *  \param period The amount of time to wait (in reference time ticks, usually 10ns steps)
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 *  \exception  ET_LOAD_STORE         invalid *now argument.
 */
inline unsigned timer_delay(timer t, int period)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                int start; \
                                asm volatile("in %0, res[%1]" : "=r" (start): "r" (t)); \
                                int until = start + period; \
                                asm volatile("setd res[%0], %1" :: "r" (t), "r" (until)); \
                                asm volatile("setc res[%0], 0x9" :: "r" (t)); \
                                int dummy; \
                                asm volatile("in %0, res[%1]" : "=r" (dummy): "r" (t)); \
                                /* Clear the condition so that a timer_get_time() will work by default */ \
                                asm volatile("setc res[%0], 0x1" :: "r" (t)); \
                              } while (0) );
}

#endif // __XC__

#endif // __xcore_c_timer_h__
