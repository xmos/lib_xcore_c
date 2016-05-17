// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __select_h__
#define __select_h__

#ifndef __XC__

#include "resource.h"
#include "port.h"

/** Disable events from a given resource.
 *
 *  Disable a given resource raising events. They can be enabled using
 *  event_enable().
 *
 *  \param r  The resource to disable events for
 */
inline void event_disable(resource r)
{
  asm volatile("edu res[%0]" :: "r" (r));
}

/** Enable events from a given resource.
 *
 *  Enable a given resource to trigger events. They can be disabled using
 *  event_disable().
 *
 *  \param r  The resource to enable events for
 */
inline void event_enable(resource r)
{
  asm volatile("eeu res[%0]" :: "r" (r));
}

#include "event_internal.h"

/** Clear all pending events.
 *
 *  In order to ensure that no events set up by other code will be triggered
 *  then this function should be called before starting to configure events for
 *  a new event loop.
 */
inline void event_clear_all()
{
  asm volatile("clre");
}

/** Setup events on a timer.
 *
 *  This function configures a timer to trigger events when the timer has reached
 *  the specified time. This function configures a timer to be used within the
 *  context of an event handling function and so is passed the value to return
 *  when the event is triggered.
 *
 *  \param t     The timer to enable events on
 *  \param value The value to be returned by event_select()/event_select_no_wait()
 *               when the timer event is triggered.
 *  \param time  The time at which the timer should trigger an event. The default
 *               timer ticks are at a 10ns resolution.
 */
inline void event_setup_timer(timer t, unsigned value, int time)
{
  event_setup_resource(t, value);
  event_setup_timer_common(t, time);
}

/** Setup events on a timer where the events are handled by a function.
 *
 *  This sets up events in the same way as event_setup_timer() but the events
 *  are designed to be handled by an event handler function which is passed in.
 *
 *  \param t       The timer to enable events on
 *  \param handler The function to handle the events
 *  \param data    The value to be passed to the event handler function
 *  \param time    The time at which the timer should trigger an event. The default
 *                 timer ticks are at a 10ns resolution.
 */
inline void event_setup_timer_function(timer t, event_handler_t handler,
                                       void *data, int time)
{
  event_setup_resource_function(t, handler, data);
  event_setup_timer_common(t, time);
}

/** Clear events for a given timer.
 *
 *  This function prevents any further events being triggered by a given timer.
 *
 *  \param t  The timer to clear events on
 */
inline void event_clear_timer(timer t)
{
  event_deregister_function(t);
  event_disable(t);
}

/** Change the time at which a timer event will fire.
 *
 *  This function modifies the time at which the timer event will be triggered.
 *  It should be used after an event caused by a timer configured using
 *  event_setup_timer() has happened.
 *
 *  \param t     The timer to change the time for
 *  \param time  The time at which the timer should trigger an event. The default
 *               timer ticks are at a 10ns resolution.
 */
inline void event_change_timer_time(timer t, int time)
{
  asm volatile("setd res[%0], %1" :: "r" (t), "r" (time));
}

/** Setup events on a channel end.
 *
 *  This function configures a channel end to trigger events when data is ready.
 *
 *  \param c     The channel end to enable events on
 *  \param value The value to be returned by event_select()/event_select_no_wait()
 *               when the channel end event is triggered.
 */
inline void event_setup_chanend(chanend c, unsigned value)
{
  event_setup_resource(c, value);
}

/** Setup events on a channel end where the events are handled by a function.
 *
 *  Same as event_setup_chanend() except that a handler function is used.
 *
 *  \param c       The channel end to enable events on
 *  \param handler The handler function to handle events
 *  \param data    The value to be passed to the event handler function
 */
inline void event_setup_chanend_function(chanend c, event_handler_t handler,
                                         void *data)
{
  event_setup_resource_function(c, handler, data);
}


/** Clear events for a given chanend.
 *
 *  This function prevents any further events being triggered by a given channel
 *  end.
 *
 *  \param c  The chanend to clear events on
 */
inline void event_clear_chanend(chanend c)
{
  event_deregister_function(c);
  event_disable(c);
}

/** Setup events on a port.
 *
 *  This function configures a port to trigger events when ready. By default a
 *  port will trigger when there is data available. The trigger event change be
 *  changed using the event_change_port_condition() function.
 *
 *  \param p     The port to enable events on
 *  \param value The value to be returned by event_select()/event_select_no_wait()
 *               when the port event is triggered.
 */
inline void event_setup_port(port p, unsigned value)
{
  event_setup_resource(p, value);

  // Set the default condition to be when there is data available
  asm volatile("setc res[%0], %1" :: "r" (p), "r" (PORT_COND_FULL));
}

/** Setup events on a port where the events are handler by a function.
 *
 *  Same as event_setup_port() except that a handler function is used.
 *
 *  \param p       The port to enable events on
 *  \param handler The handler function to handle events
 *  \param data    The value to be passed to the event handler function
 */
inline void event_setup_port_function(port p, event_handler_t handler,
                                      void *data)
{
  event_setup_resource_function(p, handler, data);

  // Set the default condition to be when there is data available
  asm volatile("setc res[%0], %1" :: "r" (p), "r" (PORT_COND_FULL));
}

/** Clear events for a given port.
 *
 *  This function prevents any further events being triggered by a given port.
 *
 *  \param p  The port to clear events on
 */
inline void event_clear_port(port p)
{
  event_deregister_function(p);
  event_disable(p);
}

/** Change the condition that triggers events on a port.
 *
 *  A port can wait for the data in the port to be equal or not equal to a
 *  specified value. After a port detects the value it resets to a default value
 *  of triggering when full.
 *
 *  \param p     The port to enable events on
 *  \param data  The data value that is being compared. The data has no effect
 *               when the condition is being set to PORT_COND_FULL.
 */
inline void event_change_port_condition(port p, port_condition_t cond, unsigned data)
{
  asm volatile("setc res[%0], %1" :: "r" (p), "r" (cond));
  asm volatile("setd res[%0], %1" :: "r" (p), "r" (data));
}

/** Set the time at which the port will input data and trigger an event.
 *
 *  A port can wait for the data in the port to be equal or not equal to a
 *  specified value.
 *  port will trigger when there is data available. The trigger event change be
 *  changed using the event_change_port_condition() function.
 *
 *  \param p     The port to enable events on
 *  \param count The port counter value at which the port will capture data and
 *               trigger an event
 */
inline void event_change_port_time(port p, int16_t count)
{
  port_set_time_condition(p, count);
}

/** Wait for an event to fire.
 *
 *  This function waits for an event to trigger and then returns the value the
 *  user has registered with the resource that triggered the event.
 *
 *  \returns  The value registered with the resource when events were enabled
 */
unsigned event_select();

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
 */
unsigned event_select_ordered_no_wait(resource ids[], unsigned no_wait_value);

#endif // __XC__

#endif // __select_h__
