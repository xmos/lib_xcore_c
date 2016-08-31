// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_timer_impl_h__
#define __xcore_c_timer_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <hwtimer.h>
#ifdef __DOXYGEN__
// Copy typedefs from hwtimer.h for use by doxygen
/**
 * hwtimer_t is an opaque type.
 *
 *  The hwtimer_t type can be used just like the timer type. It gives a unique
 *  hardware timer to use (as opposed to the default timer in XC which is
 *  allocated based on a shared hardware timer per logical core).
 *
 *  Users must not access its raw underlying type.
 */
typedef unsigned int hwtimer_t;
#endif

inline hwtimer_t _timer_alloc(void)
{
  hwtimer_t t;
  asm volatile("getr %0, 1" : "=r" (t));
  return t;
}

inline void _timer_free(hwtimer_t t)
{
  asm volatile("freer res[%0]" :: "r" (t));
}

inline void _timer_get_time(hwtimer_t t, int *now)
{
  asm volatile("in %0, res[%1]" : "=r" (*now): "r" (t));
}

inline void _timer_change_trigger_time(hwtimer_t t, int time)
{
  asm volatile("setd res[%0], %1" :: "r" (t), "r" (time));
}

inline void _timer_set_trigger_time(hwtimer_t t, int time)
{
  // Set the condition to be AFTER
  asm volatile("setc res[%0], 0x9" :: "r" (t));
  // Set the time at which an event or get will occur
  _timer_change_trigger_time(t, time);
}

inline void _timer_clear_trigger_time(hwtimer_t t)
{
  // Clear the condition
  asm volatile("setc res[%0], 0x1" :: "r" (t));
  // timer_get_time() will respond immediately
}

#endif // __XC__

#endif // __xcore_c_timer_impl_h__
