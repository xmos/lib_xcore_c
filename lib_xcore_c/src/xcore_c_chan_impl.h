// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_chan_impl_h__
#define __xcore_c_chan_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#include <stdint.h>
#include <xccompat.h>
#ifdef __DOXYGEN__
// Copy typedefs from xccompat.h for use by doxygen
/**
 * chanend is an opaque type for use in C/C++ code.
 * It enables a XC function prototyped as taking a parameter of type chanend to
 * be called from C and vice versa.
 *
 *  Users must not access its raw underlying type.
 */
typedef unsigned chanend;

/**
 * streaming_chanend_t is an opaque type for use in C/C++ code.
 * It enables a XC function prototyped as taking a parameter of type
 * streaming_chanend_t to be called from C and vice versa.
 *
 *  Users must not access its raw underlying type.
 */
typedef unsigned streaming_chanend_t;
#endif
#include <xs1.h>

#if !defined(__XC__) || defined(__DOXYGEN__)
/**
 * transacting_chanend is an opaque type.
 *
 *  Users must not access its raw underlying type.
 */
typedef struct transacting_chanend {
  chanend c;
  int last_out;
} transacting_chanend;

inline chanend _chanend_alloc(void)
{
  chanend c;
  asm("getr %0, 2" : "=r" (c));
  return c;
}

inline void _chanend_free(chanend c)
{
  asm volatile("freer res[%0]" :: "r" (c));
}

inline void _chanend_set_dest(chanend c, chanend dst)
{
  asm("setd res[%0], %1" :: "r" (c), "r" (dst));
}
#endif // __XC__

inline void _s_chan_out_word(streaming_chanend_t c, int data)
{
  asm volatile("out res[%0], %1" :: "r" (c), "r" (data));
}

inline void _s_chan_out_byte(streaming_chanend_t c, char data)
{
  asm volatile("outt res[%0], %1" :: "r" (c), "r" (data));
}

inline int _s_chan_in_word(streaming_chanend_t c)
{
  int data;
  asm volatile("in %0, res[%1]" : "=r" (data): "r" (c));
  return data;
}

inline char _s_chan_in_byte(streaming_chanend_t c)
{
  char data;
  asm volatile("int %0, res[%1]" : "=r" (data): "r" (c));
  return data;
}

inline void _s_chan_out_ct(streaming_chanend_t c, int ct)
{
  asm volatile("outct res[%0], %1" :: "r" (c), "r" (ct));
}

inline void _s_chan_out_ct_end(streaming_chanend_t c)
{
  _s_chan_out_ct(c, XS1_CT_END);
}

inline void _s_chan_check_ct(streaming_chanend_t c, int ct)
{
  asm volatile("chkct res[%0], %1" :: "r" (c), "r" (ct));
}

inline void _s_chan_check_ct_end(streaming_chanend_t c)
{
  _s_chan_check_ct(c, XS1_CT_END);
}

#if !defined(__XC__) || defined(__DOXYGEN__)
// Manage direction changes.
// As specified in the Tools Development Guide, the last_out state is managed
// to control when CT_END tokens are sent or expected.
inline void _t_chan_change_to_input(transacting_chanend *tc)
{
  if (tc->last_out) {
    _s_chan_out_ct_end(tc->c);
    tc->last_out = 0;
  }
}
inline void _t_chan_change_to_output(transacting_chanend *tc)
{
  if (!tc->last_out) {
    _s_chan_check_ct_end(tc->c);
    tc->last_out = 1;
  }
}
#endif // __XC__

#endif // __xcore_c_chan_impl_h__

