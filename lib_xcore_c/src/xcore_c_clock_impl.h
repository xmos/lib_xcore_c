// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_clock_impl_h__
#define __xcore_c_clock_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#include <xccompat.h>

inline void _clock_set_ready_src(clock clk, port ready_source)
{
  asm volatile("setrdy res[%0], %1" :: "r" (clk), "r" (ready_source));
}

#endif // __xcore_c_clock_impl_h__
