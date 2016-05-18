// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __resource_h__
#define __resource_h__

#include <stdint.h>
#include <xccompat.h>
#include <assert.h>

/** The generic resource type
 *
 */
typedef unsigned resource;

/** Set the Environment Vector (EV) of a resource.
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
 *  \param r     The resource to set the EV for
 *  \param value The value to be set in the EV register.
 */
inline void resource_set_ev(resource r, uint32_t value)
{
#if !defined(__XS2A__)
  if ((value >> 16) != 0x1) {
    __assert(__FILE__, __LINE__,
     "On XS1 bit 16 will always be set in the value returned from an event");
  }
#endif

  asm volatile("add r11, %0, 0" : : "r" (value) : /* clobbers */ "r11");
  asm volatile("setev res[%0], r11" : : "r" (r));
}

#endif // __resource_h__
