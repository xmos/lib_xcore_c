// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_resource_impl_h__
#define __xcore_c_resource_impl_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#include <stdint.h>
#include "xassert.h"

typedef uint32_t resource; // should this be exposed and should it be a union?

typedef void(*callback_function)(void);
typedef callback_function select_callback;
typedef callback_function interrupt_callback;

extern void _select_non_callback(void);  // Implemented in xcore_c_select.S

inline void _resource_enable_trigger(resource r)
{
  asm volatile("eeu res[%0]" :: "r" (r));
}

inline void _resource_disable_trigger(resource r)
{
  asm volatile("edu res[%0]" :: "r" (r));
}

extern void _resource_setup_callback(resource r, void *data, callback_function func, int type);

inline void _resource_setup_interrupt_callback(resource r, void *data, interrupt_callback intrpt)
{
  _resource_setup_callback(r, data, intrpt, 0xA);  // Raise interrupts instead of events
}

inline void _resource_setup_select_callback(resource r, void *data, select_callback callback)
{
  _resource_setup_callback(r, data, callback, 0x2);  // Raise events instead of interrupts
}

inline void _resource_setup_select(resource r, uint32_t value)
{
  _resource_setup_select_callback(r, (void*)value, _select_non_callback);
}

#endif // __XC__

#endif // __xcore_c_resource_impl_h__
