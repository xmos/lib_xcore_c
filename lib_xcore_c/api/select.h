// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __select_h__
#define __select_h__

#include <stdint.h>
#include <assert.h>
#include <xccompat.h>
#include "port.h"

typedef unsigned resource;

/*
 * TODO: ordered select
 */

/** Clear all pending events.
 *
 *  This function should be called before starting to configure events for
 *  a new event loop to ensure that no other events will be triggered by
 *  events set up by other code.
 */
inline void event_clear_all()
{
  asm volatile("clre");
}

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

/** Enable events on a resource.
 *
 *  This is a shared function to be used by enable_events_chanend(),
 *  enable_events_port() and enable_events_timer().
 *
 *  \param r      The resource to enable events for
 *  \param value The value to be returned by event_select()/event_select_no_wait()
 *               when the timer event is triggered.
 */
inline void event_setup_resource(resource r, unsigned value)
{
  // Set the event vector
  asm volatile("ldap r11, __event_target");
  asm volatile("setv res[%0], r11" :: "r" (r));

#if !defined(__XS2A__)
  if ((value >> 16) != 0x1) {
  	__assert(__FILE__, __LINE__,
   	 "On XS1 bit 16 will always be set in the value returned from an event");
  }
#endif

  // Store the data to be returned on event
  asm volatile("add r11, %0, 0" :: "r" (value));
  asm volatile("setev res[%0], r11" :: "r" (r));

  // Enable the events
  asm volatile("eeu res[%0]" :: "r" (r));
}

/** Setup events on a timer.
 *
 *  This function configures a timer to trigger events when data is ready. By
 *  default a timer is ready instantly. event_change_timer_time() should be used
 *  to cause the timer to wait for a time to be met.
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

  // Set the condition to be AFTER
  asm volatile("setc res[%0], 0x9" :: "r" (t));

  // Set the time at which the event should fire
  asm volatile("setd res[%0], %1" :: "r" (t), "r" (time));
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
 *  \param time  The port counter value at which the port will capture data and
 *               trigger an event.
 */
inline void event_change_port_time(port p, uint16_t time)
{
  asm volatile("setpt res[%0], %1" :: "r" (p), "r" (time));
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
 *  \param no_wait_value  The value to return if no event is triggered.
 *
 *  \returns  The value registered with the resource or the no_wait_value passed
 *            in if no event fired.
 */
unsigned event_select_no_wait(unsigned no_wait_value);

#endif // __select_h__
