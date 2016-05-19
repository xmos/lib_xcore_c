// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __timer_h__
#define __timer_h__

#include <xccompat.h>

#if !defined(__XC__) || defined(__DOXYGEN__)

/** Allocates and initialise a timer.
 *
 *  This macro is to be called once on every variable of the type ``timer``.
 *  If there are no timers availble, then the function will return NULL.
 *  When the timer is no longer required, timer_free() should be called
 *  to free the timer.
 *
 *  \returns   The initialized timer
 */
inline timer timer_alloc(void)
{
  timer t;
  asm volatile("getr %0, 1" : "=r" (t));
  return t;
}

/** Deallocate a timer.
 *
 *  This function is to be called once on every variable of the type ``timer``.
 *
 *  \param t  The timer to be freed
 */
inline void timer_free(timer t)
{
  asm volatile("freer res[%0]" :: "r" (t));
}

/** Get the current time from the timer.
 *
 *  \param t    The timer on which to input
 *
 *  \returns    The time value (a 32-bit value)
 */
inline int timer_get_time(timer t)
{
  int i;
  asm volatile("in %0, res[%1]" : "=r" (i): "r" (t));
  return i;
}

/** Pause until after a specified time.
 *
 *  \param tmr  The timer to use for timing
 *
 *  \param time The time to wait until
 *
 */
inline int timer_wait_until(timer tmr, int time)
{
  asm volatile("setd res[%0], %1" :: "r" (tmr), "r" (time));
  asm volatile("setc res[%0], 0x9" :: "r" (tmr));
  int i = timer_get_time(tmr);

  // Clear the condition so that a timer_get_time() will work by default
  asm volatile("setc res[%0], 0x1" :: "r" (tmr));
  return i;
}

/** Delay for a specified time using a specific timer.

 *  This function pauses until the time is
 *  reached.
 *
 *  \param tmr       The timer resource to use
 *
 *  \param period    The amount of time to wait (in reference time ticks,
 *                   usually 10ns steps)
 */
inline void timer_delay(timer tmr, int period)
{
  int time = timer_get_time(tmr);
  time += period;
  timer_wait_until(tmr, time);
}

#endif // __XC__

#endif // __timer_h__
