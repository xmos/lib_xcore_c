// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __interrupt_h__
#define __interrupt_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include "resource.h"
#include "port.h"

/** Disable interrupts from a given resource.
 *
 *  Disable a given resource raising interrupts. They can be enabled using
 *  interrupt_enable().
 *
 *  \param r  The resource to disable interrupts for
 */
inline void interrupt_disable(resource r)
{
  asm volatile("edu res[%0]" :: "r" (r));
}

/** Enable interrupts from a given resource.
 *
 *  Enable a given resource to trigger interrupts. They can be disabled using
 *  interrupt_disable().
 *
 *  \param r  The resource to enable interrupts for
 */
inline void interrupt_enable(resource r)
{
  asm volatile("eeu res[%0]" :: "r" (r));
}

/** Mask all interrupts on this core.
 *
 *  Prevent any enabled interrupts from triggering. They can be restored by
 *  using interrupt_unmask_all().
 */
inline void interrupt_mask_all()
{
  asm volatile("clrsr 0x2"); // IEBLE is bit 1
}

/** Unmask all interrupts on this core.
 *
 *  Allow any enabled interrupts to trigger. They can be suppressed by using
 *  interrupt_mask_all().
 */
inline void interrupt_unmask_all()
{

  asm volatile("setsr 0x2"); // IEBLE is bit 1
}

#include "interrupt_internal.h"

/** Setup interrupts on a timer where the interrupts are handled by a function.
 *
 *  This function configures a timer to trigger interrupts when the timer has
 *  reached the specified time. This function configures a timer to be used
 *  within the context of an interrupt handling function and so is passed the
 *  value to return when the interrupt is triggered.
 *
 *  \param t       The timer to enable interrupts on
 *  \param handler The function to handle the interrupts
 *  \param data    The value to be passed to the interrupt handler function
 *  \param time    The time at which the timer should trigger an interrupt. The
 *                 default timer ticks are at a 10ns resolution.
 */
inline void interrupt_setup_timer_function(timer t, interrupt_handler handler,
                                           void *data, int time)
{
  interrupt_setup_resource_function(t, handler, data);
  interrupt_setup_timer_common(t, time);
}

/** Clear interrupts for a given timer.
 *
 *  This function prevents any further interrupts being triggered by a given timer.
 *
 *  Interrupts for the core must first masked before this is called by using
 *  interrupt_mask_all(). Failure to do so will result in an illegal resource
 *  exception being raised.
 *
 *  \param t  The timer to clear interrupts on
 */
inline void interrupt_clear_timer(timer t)
{
  interrupt_deregister_function(t);
  interrupt_disable(t);
}

/** Change the time at which a timer interrupt will fire.
 *
 *  This function modifies the time at which the timer interrupt will be triggered.
 *  It should be used after an interrupt caused by a timer configured using
 *  interrupt_setup_timer() has happened.
 *
 *  \param t     The timer to change the time for
 *  \param time  The time at which the timer should trigger an interrupt. The
 *               default timer ticks are at a 10ns resolution.
 */
inline void interrupt_change_timer_time(timer t, int time)
{
  asm volatile("setd res[%0], %1" :: "r" (t), "r" (time));
}

/** Setup interrupts on a channel end where the interrupts are handled by a
 *  function.
 *
 *  This function configures a channel end to trigger interrupts when data is
 *  ready.
 *
 *  \param c       The channel end to enable interrupts on
 *  \param handler The handler function to handle interrupts
 *  \param data    The value to be passed to the interrupt handler function
 */
inline void interrupt_setup_chanend_function(chanend c,
                                             interrupt_handler handler,
                                             void *data)
{
  interrupt_setup_resource_function(c, handler, data);
}

/** Clear interrupts for a given chanend.
 *
 *  This function prevents any further interrupts being triggered by a given
 *  channel end.
 *
 *  Interrupts for the core must first masked before this is called by using
 *  interrupt_mask_all(). Failure to do so will result in an illegal resource
 *  exception being raised.
 *
 *  \param c  The chanend to clear interrupts on
 */
inline void interrupt_clear_chanend(chanend c)
{
  interrupt_deregister_function(c);
  interrupt_disable(c);
}

/** Setup interrupts on a port where the interrupts are handler by a function.
 *
 *  Same as interrupt_setup_port() except that a handler function is used.
 *
 *  \param p       The port to enable interrupts on
 *  \param handler The handler function to handle interrupts
 *  \param data    The value to be passed to the interrupt handler function
 */
inline void interrupt_setup_port_function(port p, interrupt_handler handler,
                                          void *data)
{
  interrupt_setup_resource_function(p, handler, data);

  // Set the default condition to be when there is data available
  asm volatile("setc res[%0], %1" :: "r" (p), "r" (PORT_COND_FULL));
}

/** Clear interrupts for a given port.
 *
 *  This function prevents any further interrupts being triggered by a given port.
 *
 *  Interrupts for the core must first masked before this is called by using
 *  interrupt_mask_all(). Failure to do so will result in an illegal resource
 *  exception being raised.
 *
 *  \param p  The port to clear interrupts on
 */
inline void interrupt_clear_port(port p)
{
  interrupt_deregister_function(p);
  interrupt_disable(p);
}

/** Change the condition that triggers interrupts on a port.
 *
 *  A port can wait for the data in the port to be equal or not equal to a
 *  specified value. After a port detects the value it resets to a default value
 *  of triggering when full.
 *
 *  \param p     The port to enable interrupts on
 *  \param cond  The condition which the port is waiting for
 *  \param data  The data value that is being compared. The data has no effect
 *               when the condition is being set to PORT_COND_FULL.
 */
inline void interrupt_change_port_condition(port p,
                                            port_condition cond,
                                            unsigned data)
{
  asm volatile("setc res[%0], %1" :: "r" (p), "r" (cond));
  asm volatile("setd res[%0], %1" :: "r" (p), "r" (data));
}

/** Set the time at which the port will input data and trigger an interrupt.
 *
 *  A port can wait for the data in the port to be equal or not equal to a
 *  specified value.
 *  port will trigger when there is data available. The trigger interrupt change
 *  be changed using the interrupt_change_port_condition() function.
 *
 *  \param p     The port to enable interrupts on
 *  \param count The port counter value at which the port will capture data and
 *               trigger an interrupt
 */
inline void interrupt_change_port_time(port p, int16_t count)
{
  port_set_time_condition(p, count);
}

#endif // __XC__

#endif // __interrupt_h__
