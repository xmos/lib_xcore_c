// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_event_impl_h__
#define __xcore_c_event_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#include <stdint.h>
#include "xassert.h"
#include "xcore_c_timer.h"

/** The generic resource type */
typedef uint32_t resource;

inline void _event_enable(resource r)
{
  asm volatile("eeu res[%0]" :: "r" (r));
}

inline void _event_disable(resource r)
{
  asm volatile("edu res[%0]" :: "r" (r));
}

/** Enable events on a resource.
 *
 *  This is a shared function to be used by enable_events_chanend(),
 *  enable_events_port() and enable_events_timer().
 *
 *  The value is stored in the EV register.
 *
 *  The EV for a resource is a piece of state associated with each resource. The
 *  value of the EV is placed in the Exception Data (ED) register when an event
 *  is taken.
 *
 *   - On the XS1 architecture it stores 16-bits and has the most significant
 *  16-bits of the VECTOR_BASE register ORed with its value that is placed in
 *  the ED register when an event is taken.
 *   - On the XS2 architecture the EV register is 32-bits wide so does not have
 *  anything combined with it.
 *
 *  \param r     The resource to enable events for
 *
 *  \param value The value to be returned by event_select()/event_select_no_wait()
 *               when the timer event is triggered.
 */
inline void _event_setup_resource(resource r, uint32_t value)
{
#if !defined(__XS2A__)
  xassert( (value >> 16) == 0x1 && msg("On XS1 bit 16 will always be set in the value returned from an event"));
#endif
  // Set the event vector
  asm volatile("ldap r11, __event_target" : : : /* clobbers */ "r11");
  asm volatile("setv res[%0], r11" : : "r" (r));
  asm volatile("add r11, %0, 0" : : "r" (value) : /* clobbers */ "r11");
  asm volatile("setev res[%0], r11" : : "r" (r));
}

#endif // __xcore_c_event_impl_h__

