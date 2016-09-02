// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_chanend_h__
#define __xcore_c_chanend_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include "xcore_c_chan_impl.h"
#include "xcore_c_resource_impl.h"
#include "xcore_c_exception_impl.h"

/** Allocate a single chan-end.
 *
 *  This function allocates a hardware channel end.
 *  If there are no channel ends available the function returns 0.
 *  When the channel end is no longer required, chanend_free() must be called
 *  to deallocate it.
 *
 *  \param c    chan-end ID or zero if none are available.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LOAD_STORE         invalid ''\*c'' argument.
 */
inline xcore_c_error_t chanend_alloc(chanend* c)
{
  RETURN_EXCEPTION_OR_ERROR( *c = _chanend_alloc() );
}

/** Deallocate a single channel-end.
 *
 *  This function frees the hardware chan-end.
 *  The last transfer on the chan-end must have been a CT_END token.
 *
 *  \param c    chan-end to free.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    an input/output is pending,
 *                                    or it has not received/sent a CT_END token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid ''\*c'' argument.
 */
inline xcore_c_error_t chanend_free(chanend *c)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
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
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
*/
inline xcore_c_error_t chanend_set_dest(chanend c, chanend dst)
{
  RETURN_EXCEPTION_OR_ERROR( _chanend_set_dest(c, dst) );
}

/** Setup select events on a chan-end.
 *
 *  Configures a chan-end to trigger select events when data is ready.
 *  It is used in combination with select_wait() et al functions,
 *  returning the enum_id when the event is triggered.
 *
 *  Once the event is setup you need to call chanend_enable_trigger() to enable it.
 *
 *  \param c        The chan-end to setup the select event on
 *  \param enum_id  The value to be returned by select_wait() et al when the
 *                  chan-end event is triggered.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in enum_id.
*/
inline xcore_c_error_t chanend_setup_select(chanend c, uint32_t enum_id)
{
  RETURN_EXCEPTION_OR_ERROR( _resource_setup_select(c, enum_id) );
}

/** Setup select events on a chan-end where the events are handled by a function.
 *
 *  Same as chanend_setup_select() except that a callback function is used
 *  rather than the event being passed back to the select_wait() et al functions.
 *
 *  Once the event is setup you need to call chanend_enable_trigger() to enable it.
 *
 *  \param c      The chan-end to setup the select event on
 *  \param data   The value to be passed to the select_callback_t function
 *  \param func   The select_callback_t function to handle the event
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in enum_id.
 */
inline xcore_c_error_t chanend_setup_select_callback(chanend c, void *data, select_callback_t func)
{
  RETURN_EXCEPTION_OR_ERROR( _resource_setup_select_callback(c, data, func) );
}

/** Setup interrupt events on a chan-end .
 *
 *  Once the event is setup you need to call chanend_enable_trigger() to enable it.
 *
 *  \param c     The chan-end to setup the events on
 *  \param data  The value to be passed to the interrupt_callback_t function
 *  \param func  The interrupt_callback_t function to handle events
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in enum_id.
 */
inline xcore_c_error_t chanend_setup_interrupt_callback(chanend c, void *data,
                                                 interrupt_callback_t func)
{
  RETURN_EXCEPTION_OR_ERROR( _resource_setup_interrupt_callback(c, data, func) );
}

/** Enable select & interrupt events on a chan-end.
 *
 *  Prior to enabling, chanend_setup_select(), chanend_setup_select_callback() or
 *  chanend_setup_interrupt_callback() must have been called.
 *  Events can be temporarily disabled and re-enabled using chanend_disable_trigger()
 *  and chanend_enable_trigger().
 *  When the event fires, the value must be read from the chan-end to clear the event.
 *
 *  \param c    The chan-end to enable events on
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline xcore_c_error_t chanend_enable_trigger(chanend c)
{
  RETURN_EXCEPTION_OR_ERROR( _resource_enable_trigger(c) );
}

/** Disable select & interrupt events for a given chan-end.
 *
 *  This function prevents any further events being triggered by a given chan-end.
 *
 *  \param c    The chan-end to disable events on
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline xcore_c_error_t chanend_disable_trigger(chanend c)
{
  RETURN_EXCEPTION_OR_ERROR( _resource_disable_trigger(c) );
}

#endif // __XC__

#endif // __xcore_c_chanend_h__
