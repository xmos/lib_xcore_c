// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_chanend_h__
#define __xcore_c_chanend_h__

#include "xcore_c_impl.h"

#if !defined(__XC__) || defined(__DOXYGEN__)

/** Allocate a single chan-end.
 *
 *  This function allocates a hardware channel end.
 *  If there are no channel ends available the function returns 0.
 *  When the channel end is no longer required, chanend_free() must be called
 *  to deallocate it.
 *
 *  \param c    chan-end ID or zero if none are available.
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LOAD_STORE         invalid *c argument.
 */
inline unsigned chanend_alloc(chanend* c)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                *c = _chanend_alloc(); \
                              } while (0) );
}

/** Deallocate a single channel-end.
 *
 *  This function frees the hardware chan-end.
 *  The last transfer on the chan-end must have been a CT_END token.
 *
 *  \param c    chan-end to free.
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    an input/output is pending,
 *                                    or it has not received/sent a CT_END token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *c argument.
 */
inline unsigned chanend_free(chanend *c)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                _chanend_free(*c); \
                                *c = 0; \
                              } while (0) );
}

/** Set the destination of a chan-end
 *
 *  \param c     chan-end to set.
 *
 *  \param dst   Destination chan-end.
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
*/
inline unsigned chanend_set_dest(chanend c, chanend dst)
{
  RETURN_COND_TRYCATCH_ERROR( _chanend_set_dest(c, dst) );
}

#endif // __XC__

#endif // __xcore_c_chanend_h__
