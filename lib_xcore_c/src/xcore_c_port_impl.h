// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_port_impl_h__
#define __xcore_c_port_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#include <stdint.h>
#include <xccompat.h>

inline void _port_set_buffered(port p)
{
  asm volatile("setc res[%0], 0x200f" :: "r" (p));
}

inline void _port_set_clock(port p, clock clk)
{
  asm volatile("setclk res[%0], %1" :: "r" (p), "r" (clk));
}

inline void _port_set_mode_data_port(port p)
{
  asm volatile("setc res[%0], 0x5007" :: "r" (p));
}

inline void _port_set_mode_ready_port(port p)
{
  asm volatile("setc res[%0], 0x5017" :: "r" (p));
}

inline void _port_set_ready_src(port p, port ready_source)
{
  asm volatile("setrdy res[%0], %1" :: "r" (p), "r" (ready_source));
}

inline void _port_set_master(port p)
{
  asm volatile("setc res[%0], 0x1007" :: "r" (p));
}

inline void _port_set_slave(port p)
{
  asm volatile("setc res[%0], 0x100f" :: "r" (p));
}

inline void _port_set_ready_strobed(port p)
{
  asm volatile("setc res[%0], 0x300f" :: "r" (p));
}

inline void _port_set_ready_handshake(port p)
{
  asm volatile("setc res[%0], 0x3017" :: "r" (p));
}

inline void _port_output(port p, int data)
{
  asm volatile("out res[%0], %1" :: "r" (p), "r" (data));
}

inline int _port_input(port p)
{
  int data;
  asm volatile("in %0, res[%1]" : "=r" (data): "r" (p));
  return data;
}

inline void _port_clear_buffer(port p)
{
  asm volatile("setc res[%0], 0x17" :: "r" (p));
}

#endif // __xcore_c_port_impl_h__
