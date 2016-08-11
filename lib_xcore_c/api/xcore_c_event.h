// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_event_h__
#define __xcore_c_event_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include "xcore_c_event_impl.h"
#include "xcore_c_exception_impl.h"

#include "xcore_c_port.h"

/** Starting value to use for enumerating values to be registerd with
 *  event_setup_xxx() functions and returned by event_select() etal.
 *
 *  On XS1 the environment vectors (EVs) are only 16-bit and bit 16 will be set
 *  to 1 as it is expected to be used as a memory address.
*/
#if defined(__XS2A__)
#define EVENT_ENUM_BASE 0
#else
#define EVENT_ENUM_BASE 0x10000
#endif

/** Disable all events.
 *
 *  This function is called before starting to configure events for
 *  a new event loop.
 *  This will ensure that no events set up by other code will be triggered.
 *
 *  \return     XS1_ET_NONE
 */
inline unsigned event_disable_all(void)
{
  asm volatile("clre");
  return XS1_ET_NONE;
}

/** Setup events on a timer.
 *
 *  This function configures a timer to trigger events when the timer has reached
 *  the specified time. This function configures a timer to be used within the
 *  context of an event handling function and so is passed the value to return
 *  when the event is triggered.
 *
 *  Once the event is setup you need to call event_enable_timer() to enable it.
 *
 *  \param t     The timer to setup events on
 *  \param time  The time at which the timer will trigger an event. The default
 *               timer ticks are at a 10ns resolution.
 *  \param value The value to be returned by event_select()/event_select_no_wait()
 *               when the timer event is triggered.
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in value.
 */
inline unsigned event_setup_timer(timer t, int time, unsigned value)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _event_setup_resource(t, value); \
                                  /* Set the condition to be AFTER */ \
                                  asm volatile("setc res[%0], 0x9" :: "r" (t)); \
                                  /* Set the time at which the event will fire */ \
                                  asm volatile("setd res[%0], %1" :: "r" (t), "r" (time)); \
                              } while (0) );
}

/** Enable events on a timer.
 *
 *  Enable a timer to trigger events. They can be disabled using
 *  event_disable_timer().
 *
 *  Prior to enabling, event_setup_timer() must have been called.
 *
 *  \param t    The timer to enable events on
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
inline unsigned event_enable_timer(timer t)
{
  RETURN_EXCEPTION_OR_ERROR( _event_enable(t) );
}

/** Disable events for a given timer.
 *
 *  This function prevents any further events being triggered by a given timer.
 *
 *  \param t    The timer to disable events on
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
inline unsigned event_disable_timer(timer t)
{
  RETURN_EXCEPTION_OR_ERROR( _event_disable(t) );
}

/** Change the time at which a timer event will fire.
 *
 *  This function modifies the time at which the timer event will be triggered.
 *  It is used after an event caused by a timer configured using
 *  event_setup_timer() has happened.
 *
 *  \param t     The timer to change the time for
 *  \param time  The time at which the timer will trigger an event. The default
 *               timer ticks are at a 10ns resolution.
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid timer.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the timer.
 */
inline unsigned event_change_timer(timer t, int time)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("setd res[%0], %1" :: "r" (t), "r" (time)) );
}

/** Setup events on a chan-end.
 *
 *  This function configures a chan-end to trigger events when data is ready.
 *
 *  Once the event is setup you need to call event_enable_chanend() to enable it.
 *
 *  \param c     The chan-end to enable events on
 *  \param value The value to be returned by event_select()/event_select_no_wait()
 *               when the chan-end event is triggered.
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in value.
*/
inline unsigned event_setup_chanend(chanend c, unsigned value)
{
  RETURN_EXCEPTION_OR_ERROR( _event_setup_resource(c, value) );
}

/** Enable events on a chan-end.
 *
 *  Enable a chan-end to trigger events. They can be disabled using
 *  event_disable_chanend().
 *
 *  Prior to enabling, event_setup_chanend() must have been called.
 *
 *  \param c    The chan-end to enable events on
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline unsigned event_enable_chanend(chanend c)
{
  RETURN_EXCEPTION_OR_ERROR( _event_enable(c) );
}

/** Disable events for a given chan-end.
 *
 *  This function prevents any further events being triggered by a given chan-end.
 *
 *  \param c    The chan-end to disable events on
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline unsigned event_disable_chanend(chanend c)
{
  RETURN_EXCEPTION_OR_ERROR( _event_disable(c) );
}

/** Setup events on a port.
 *
 *  This function configures a port to trigger events when ready. By default a
 *  port will trigger when there is data available. The trigger event can be
 *  changed using the event_change_port_condition() function.
 *
 *  Once the event is setup you need to call event_enable_port() to enable it.
 *
 *  \param p     The port to enable events on
 *  \param value The value to be returned by event_select()/event_select_no_wait()
 *               when the port event is triggered.
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 *  \exception  ET_ECALL              when xassert enabled, on XS1 bit 16 not set in value.
 */
inline unsigned event_setup_port(port p, unsigned value)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                _event_setup_resource(p, value); \
                                /* Set the default condition to be when there is data available */ \
                                asm volatile("setc res[%0], %1" :: "r" (p), "r" (PORT_COND_FULL)); \
                              } while (0) );
}

/** Enable events on a port.
 *
 *  Enable a port to trigger events. They can be disabled using
 *  event_disable_port().
 *
 *  Prior to enabling, event_setup_port() must have been called.
 *
 *  \param p    The port to enable events on
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 */
inline unsigned event_enable_port(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _event_enable(p) );
}

/** Disable events for a given port.
 *
 *  This function prevents any further events being triggered by a given port.
 *
 *  \param p    The port to disable events on
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 */
inline unsigned event_disable_port(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _event_disable(p) );
}

/** Change the condition that triggers events on a port.
 *
 *  A port can wait for the data in the port to be equal or not equal to a
 *  specified value. After a port detects the value it resets to a default value
 *  of triggering when full.
 *
 *  \param p     The port to enable events on
 *  \param cond  The condition which the port is waiting for
 *  \param data  The data value that is being compared. The data has no effect
 *               when the condition is being set to PORT_COND_FULL.
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 */
inline unsigned event_change_port_condition(port p, port_condition cond, unsigned data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setc res[%0], %1" :: "r" (p), "r" (cond)); \
                                asm volatile("setd res[%0], %1" :: "r" (p), "r" (data)); \
                              } while (0) );
}

/** Set the time at which the port will input data and trigger an event.
 *
 *  A port can wait for the data in the port to be equal or not equal to a
 *  specified value.
 *  port will trigger when there is data available. The trigger event can be
 *  changed using the event_change_port_condition() function.
 *
 *  \param p     The port to enable events on
 *  \param count The port counter value at which the port will capture data and
 *               trigger an event
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 */
inline unsigned event_change_port_time(port p, int16_t count)
{
  //return port_set_time_condition(p, count);
  port_set_time_condition(p, count);
  return 0; /// TODO *****************************************
}

/** Wait for an event to fire.
 *
 *  This function waits for an event to trigger and then returns the value the
 *  user has registered with the resource that triggered the event.
 *
 *  \returns  The value registered with the resource when events were enabled
 */
unsigned event_select(void);

/** Check whether any events are ready, otherwise return.
 *
 *  This function tests for an event to being ready. If there is one ready then
 *  it returns the value the user has registered with the resource that
 *  triggered the event. If no events are ready then returns the no_wait_value
 *  passed in by the user.
 *
 *  \param no_wait_value  The value to return if no event is triggered
 *
 *  \returns  The value registered with the resource which triggered an event
 *            or the no_wait_value passed in if no event fired
 */
unsigned event_select_no_wait(unsigned no_wait_value);

/** Wait for an event from a list of resources using an ordered enable sequence
 *
 *  This function:
 *    - starts by clearing all events that have been configured
 *    - setting events on the core to be enabled
 *    - enabling events on each resource in turn so that there is a defined
 *      order in which events will be taken
 *
 *  \param ids  Null-terminated list of resources to enable events on
 *
 *  \returns    The value registered with the resource which triggers an event
 *
 *  \exception  ET_LOAD_STORE         invalid ids[] argument.
 */
unsigned event_select_ordered(resource ids[]);

/** Wait for an event from a list of resources using an ordered enable sequence
 *
 *  This function does the same as event_select_ordered, but will return the
 *  no_wait_value if no event fires by the end of the enabling sequence.
 *
 *  \param ids            Null-terminated list of resources to enable events on
 *  \param no_wait_value  The value to return if no event is triggered
 *
 *  \returns  The value registered with the resource which triggered an event
 *            or the no_wait_value passed in if no event fired
 *
 *  \exception  ET_LOAD_STORE         invalid ids[] argument.
 */
unsigned event_select_ordered_no_wait(resource ids[], unsigned no_wait_value);

#endif // __XC__

#endif // __xcore_c_event_h__
