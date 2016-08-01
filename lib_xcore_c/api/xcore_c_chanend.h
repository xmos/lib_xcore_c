// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_chanend_h__
#define __xcore_c_chanend_h__

#include <xccompat.h>

#if !defined(__XC__) || defined(__DOXYGEN__)

/** Allocate a single channel-end.
 *
 *  \param c   Channel-end
 */
inline void chanend_alloc(chanend *c)
{
  asm("getr %0, 2" : "=r" (*c));
}

/** Free a single channel-end.
 *
 *  \param c   Channel-end to be freed
 */
inline void chanend_free(chanend c)
{
  if (c) {
    asm volatile("freer res[%0]" :: "r" (c));
  }
}

/** Set the destination of a chanend
 *
 *  \param c     Channel-end
 *
 *  \param dst   Destination channel-end
 */
inline void chanend_set_dest(chanend c, chanend dst)
{
  asm("setd res[%0], %1" :: "r" (c), "r" (dst));
}
#endif // __XC__

#endif // __xcore_c_chanend_h__
