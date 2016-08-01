// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_event_internal_h__
#define __xcore_c_event_internal_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include <assert.h>
#include <xccompat.h>
#include "xcore_c_resource.h"
#include "xcore_c_port.h"

#if defined(__XS2A__)
#define EVENT_ENUM_BASE 0
#else
// On XS1 the environment vectors (EVs) are only 16-bit and bit 16 will be set
// to 1 as it is expected to be used as a memory address.
#define EVENT_ENUM_BASE 0x10000
#endif

/** Enable events on a resource.
 *
 *  This is a shared function to be used by enable_events_chanend(),
 *  enable_events_port() and enable_events_timer().
 *
 *  \param r     The resource to enable events for
 *  \param value The value to be returned by event_select()/event_select_no_wait()
 *               when the timer event is triggered.
 */
inline void event_setup_resource(resource r, unsigned value)
{
  // Set the event vector
  asm volatile("ldap r11, __event_target" : : : /* clobbers */ "r11");
  asm volatile("setv res[%0], r11" : : "r" (r));

  resource_set_ev(r, value);
  event_enable(r);
}

/** Basic setup function for timer.
 *
 *  Function shared by event_setup_timer() and event_setup_timer_function().
 *
 *  \param t     The timer to enable events for
 *  \param time  The time at which the timer will trigger an event. The
 *               default timer ticks are at a 10ns resolution.
 */
inline void event_setup_timer_common(timer t, int time)
{
  // Set the condition to be AFTER
  asm volatile("setc res[%0], 0x9" :: "r" (t));

  // Set the time at which the event will fire
  asm volatile("setd res[%0], %1" :: "r" (t), "r" (time));
}

#endif // __XC__

#endif // __xcore_c_event_internal_h__
