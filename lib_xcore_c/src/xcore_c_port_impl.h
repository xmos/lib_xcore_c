// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_port_impl_h__
#define __xcore_c_port_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#include <stdint.h>
#include <xccompat.h>
#include <xs1.h>

inline void _port_setc(port p, uint32_t c)
{
  asm volatile("setc res[%0], %1" :: "r" (p), "r" (c));
}

inline void _port_set_transfer_width(port p, int width)
{
  asm volatile("settw res[%0], %1" :: "r" (p), "r" (width));
}

inline port _port_alloc(int port_id)
{
  _port_setc(port_id, XS1_SETC_INUSE_ON);
  return port_id;
}

inline void _port_reset(port p)
{
  _port_setc(p, XS1_SETC_INUSE_ON);
}

inline void _port_free(port p)
{
  _port_setc(p, XS1_SETC_INUSE_OFF);
}

inline void _port_set_buffered(port p)
{
  _port_setc(p, XS1_SETC_BUF_BUFFERS);
}

inline void _port_set_unbuffered(port p)
{
  _port_setc(p, XS1_SETC_BUF_NOBUFFERS);
}

inline void _port_set_clock(port p, clock clk)
{
  asm volatile("setclk res[%0], %1" :: "r" (p), "r" (clk));
}

inline void _port_set_inout_data(port p)
{
  _port_setc(p, XS1_SETC_PORT_DATAPORT);
}

inline void _port_set_out_clock(port p)
{
  _port_setc(p, XS1_SETC_PORT_CLOCKPORT);
}

inline void _port_set_out_ready(port p, port ready_source)
{
  _port_setc(p, XS1_SETC_PORT_READYPORT);
  asm volatile("setrdy res[%0], %1" :: "r" (p), "r" (ready_source));
}

inline void _port_set_invert(port p)
{
  _port_setc(p, XS1_SETC_INV_INVERT);
}

inline void _port_set_no_invert(port p)
{
  _port_setc(p, XS1_SETC_INV_NOINVERT);
}

inline void _port_set_sample_falling_edge(port p)
{
  _port_setc(p, XS1_SETC_SDELAY_SDELAY);
}

inline void _port_set_sample_rising_edge(port p)
{
  _port_setc(p, XS1_SETC_SDELAY_NOSDELAY);
}

inline void _port_set_master(port p)
{
  _port_setc(p, XS1_SETC_MS_MASTER);
}

inline void _port_set_slave(port p)
{
  _port_setc(p, XS1_SETC_MS_SLAVE);
}

inline void _port_set_no_ready(port p)
{
  _port_setc(p, XS1_SETC_RDY_NOREADY);
}

inline void _port_set_ready_strobed(port p)
{
  _port_setc(p, XS1_SETC_RDY_STROBED);
}

inline void _port_set_ready_handshake(port p)
{
  _port_setc(p, XS1_SETC_RDY_HANDSHAKE);
}

inline int16_t _port_get_trigger_time(port p)
{
  int16_t ts;
  asm volatile("getts %0, res[%1]" : "=r" (ts) : "r" (p));
  return ts;
}

inline void _port_set_trigger_time(port p, int16_t t)
{
  asm volatile("setpt res[%0], %1" :: "r" (p), "r" (t));
}

inline void _port_clear_trigger_time(port p)
{
  asm volatile("clrpt res[%0]" :: "r" (p));
}

inline void _port_set_trigger_in_equal(port p, int d)
{
  _port_setc(p, XS1_SETC_COND_EQ);
  asm volatile("setd res[%0], %1" :: "r" (p), "r" (d));
}

inline void _port_set_trigger_in_not_equal(port p, int d)
{
  _port_setc(p, XS1_SETC_COND_NEQ);
  asm volatile("setd res[%0], %1" :: "r" (p), "r" (d));
}

inline void _port_clear_trigger_in(port p)
{
  _port_setc(p, XS1_SETC_COND_NONE);
}

inline int _port_peek(port p)
{
  int data;
  asm volatile("peek %0, res[%1]" : "=r" (data): "r" (p));
  return data;
}

inline void _port_out(port p, int data)
{
  asm volatile("out res[%0], %1" :: "r" (p), "r" (data));
}

inline int _port_in(port p)
{
  int data;
  asm volatile("in %0, res[%1]" : "=r" (data): "r" (p));
  return data;
}

inline void _port_out_shift_right(port p, int *data)
{
  // We read-write data
  asm volatile("outshr res[%1], %0" : "+r" (*data) : "r" (p));
}

inline void _port_in_shift_right(port p, int *data)
{
  // We read-write data
  asm volatile("inshr %0, res[%1]" : "+r" (*data) : "r" (p));
}

inline int _port_endin(port p)
{
  int num;
  asm volatile("endin %0, res[%1]" : "=r" (num) : "r" (p));
  return num;
}

inline void _port_clear_buffer(port p)
{
  _port_setc(p, XS1_SETC_RUN_CLRBUF);
}

#endif // __xcore_c_port_impl_h__
