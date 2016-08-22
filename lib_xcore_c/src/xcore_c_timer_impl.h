// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_timer_impl_h__
#define __xcore_c_timer_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <xccompat.h>

inline timer _timer_alloc(void)
{
  timer t;
  asm volatile("getr %0, 1" : "=r" (t));
  return t;
}

inline void _timer_free(timer t)
{
  asm volatile("freer res[%0]" :: "r" (t));
}

inline void _timer_get_time(timer t, int *now)
{
  asm volatile("in %0, res[%1]" : "=r" (*now): "r" (t));
}

inline void _timer_change_trigger_time(timer t, int time)
{
  asm volatile("setd res[%0], %1" :: "r" (t), "r" (time));
}

inline void _timer_setup_trigger(timer t, int time)
{
  // Set the condition to be AFTER
  asm volatile("setc res[%0], 0x9" :: "r" (t));
  // Set the time at which an event or get will occur
  _timer_change_trigger_time(t, time);
}

inline void _timer_clear_trigger(timer t)
{
  // Clear the condition
  asm volatile("setc res[%0], 0x1" :: "r" (t));
  // timer_get_time() will respond immediately
}

#endif // __XC__

#endif // __xcore_c_timer_impl_h__
