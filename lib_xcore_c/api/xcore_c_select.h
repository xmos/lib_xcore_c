// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_select_h__
#define __xcore_c_select_h__

#if !defined(__XC__) || defined(__DOXYGEN__)
// XC files should use the native operators.

#include <stdint.h>
#include "xcore_c_select_impl.h"
#include "xcore_c_resource_impl.h"
#include "xcore_c_error_codes.h"

/** Starting value to use for the enum_id
 *
 *  The enum_id is passed to the <resource>_setup_select() and returned by
 *  select_wait() et al
 *
 *  On XS1 the environment vectors (EVs) are only 16-bit and bit 16 will be set
 *  to 1 as it is expected to be used as a memory address.
*/
#if defined(__XS2A__)
#define ENUM_ID_BASE 0
#else
#define ENUM_ID_BASE 0x10000
#endif

/** Disable all select events on this logical core.
 *
 *  This function is called before starting to configure select events for
 *  a new event loop.
 *  This will ensure that no events set up by other code will be triggered
 *
 *  N.B. this affect events setup using <resource>_setup_select() and
 *       <resource>_setup_select_callback()
 *       but not <resource>_setup_interrupt_callback()
 *
 *  \return     error_none
 */
inline xcore_c_error select_disable_trigger_all(void)
{
  asm volatile("clre");
  return error_none;
}

/** Wait for a select event to trigger.
 *
 *  This function waits for an event to trigger and then returns the value the
 *  user has registered with the resource that triggered the event.
 *
 *  \returns  The enum_id registered with the resource when events were enabled
 */
unsigned select_wait(void);

/** Check whether any select events have triggered, otherwise return.
 *
 *  This function tests for an event to being ready. If there is one ready then
 *  it returns the enum_id the user has registered with the resource that
 *  triggered the event. If no events are ready then returns the no_wait_id
 *  passed in by the user.
 *  N.B. select_callback events are handled, but are not considered 'select events'
 *
 *  \param no_wait_id  The enum_id to return if no 'select event' is triggered
 *
 *  \returns  The enum_id registered with the resource which triggered an event
 *            or the no_wait_id passed in if no event fired
 */
unsigned select_no_wait(unsigned no_wait_id);

/** Wait for an select event from a list of resources using an ordered enable sequence
 *
 *  This function:
 *    - Starts by clearing all select events that have been configured for this core.
 *      This includes select_callback functions but not interrupt_callback functions.
 *    - Enables select events on each resource in turn so that there is a defined
 *      order in which pending events will be taken
 *  N.B. Enabled select_callback resources will be taken, but will not terminate
 *  the process. A user may wish to place these at the front of the list.
 *
 *  \param ids  Null-terminated list of resources to enable events on
 *
 *  \returns    The enum_id registered with the resource which triggers an event
 *
 *  \exception  ET_LOAD_STORE         invalid ids[] argument.
 */
unsigned select_wait_ordered(resource ids[]);

/** Wait for a select event from a list of resources using an ordered enable sequence
 *
 *  This function does the same as select_wait_ordered, but will return the
 *  no_wait_id if no select event fires by the end of the enabling sequence.
 *  N.B. select_callback events are handled, but are not considered 'select events'
 *
 *  \param no_wait_id  The enum_id to return if no 'select event' is triggered
 *  \param ids         Null-terminated list of resources to enable events on
 *
 *  \returns  The enum_id registered with the resource which triggered an event
 *            or the no_wait_id passed in if no event fired
 *
 *  \exception  ET_LOAD_STORE         invalid ids[] argument.
 */
unsigned select_no_wait_ordered(unsigned no_wait_id, resource ids[]);

/** Define a select callback handling function
 *
 *  This macro will define two functions for you:
 *  1) an ordinary function that may be called directly
 *     Its signature will be 'void <callback>(void* <data>);'
 *  2) an select_callback function for passing to the <resource>_setup_select_callback functions
 *     The select_callback function name is accessed using the SELECT_CALLBACK() macro
 *
 *  Example usage:
 *    DEFINE_SELECT_CALLBACK(myfunc, arg)
 *    {
 *      // This is the body of 'void myfunc(void* arg)'
 *    }
 *
 *  \Param callback this is the name of the ordinary function
 *  \Param data     the name to use for the *void argument
 */
#define DEFINE_SELECT_CALLBACK(callback, data) _DEFINE_SELECT_CALLBACK(callback, data)

/** Declare a select callback handling function
 *
 *  Use this macro when you require a declaration of your select callback function types
 *
 *  Example usage:
 *    DECLARE_SELECT_CALLBACK(myfunc, arg);
 *    chanend_setup_select_callback(c, 0 , SELECT_CALLBACK(myfunc));
 *
 *  \Param callback this is the name of the ordinary function
 *  \Param data     the name to use for the *void argument
 */
#define DECLARE_SELECT_CALLBACK(callback, data) _DECLARE_SELECT_CALLBACK(callback, data)

/** The name of the defined 'select_callback' function
 *
 *  Use this macro for retriving the name of the declared select callback function.
 *  This is the name that is passed to <resource>_setup_select_callback() for registration.
 *
 *  \return     the name of the defined select_callback function
 */
#define SELECT_CALLBACK(callback) _SELECT_CALLBACK(callback)

#endif // __XC__

#endif // __xcore_c_select_h__
