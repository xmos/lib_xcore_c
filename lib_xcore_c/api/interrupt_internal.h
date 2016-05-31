// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __interrupt_internal_h__
#define __interrupt_internal_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include "interrupt_defines.h"
#include "resource.h"

#ifndef INTERRUPT_MAX_HANDLER_FUNCTIONS
/**
 * The default maximum number of interrupt handler functions. This needs to be
 * a compile-time constant as space needs to be allocated for the handler data.
 */
#define INTERRUPT_MAX_HANDLER_FUNCTIONS 20
#endif

/** The typedef for all function interrupt handlers.
 *
 * The ID of the resource that triggered the interrupt is passed to the handler along
 * with the user data registered with that resource.
 */
typedef void (*interrupt_handler)(resource, void*);

/** The structure to register interrupt function handlers
 */
typedef struct interrupt_handler_function_state {
  resource owner;
  interrupt_handler handler;
  void *data;
} interrupt_handler_function_state;

/* Compile time check to ensure the SIZEOF_INTERRUPT_HANDLER_FUNCTION_STATE
 * define is always set correctly
 */
_Static_assert(((SIZEOF_INTERRUPT_HANDLER_FUNCTION_STATE * sizeof(int)) ==
                sizeof(interrupt_handler_function_state)),
               "SIZEOF_INTERRUPT_HANDLER_FUNCTION_STATE define is incorrect");

/** Register an interrupt handler for a resource.
 *
 *  Find a free registry entry and register the function and data for the given
 *  resource.
 *
 *  \param r       The resource to register the handler for
 *  \param handler The function to handle the interrupts
 *  \param data    The value to be passed to the interrupt handler function
 *  \returns       The index allocated for this resource or -1 if the register is full
 */
int interrupt_register_function(resource r, interrupt_handler handler, void *data);

/** Deregister an interrupt handler for a resource
 *
 *  Determine whether there is an interrupt handler function registered for a given
 *  resource. If so, remove the entry. If there is none registered then simply
 *  ignore it.
 *
 *  \param r  The resource to de-register
 */
void interrupt_deregister_function(resource r);

#if defined(__XS2A__)
#define INTERRUPT_ENUM_BASE 0
#else
// On XS1 the environment vectors (EVs) are only 16-bit and bit 16 will be set
// to 1 as it is expected to be used as a memory address.
#define INTERRUPT_ENUM_BASE 0x10000
#endif

/** Enable interrupts on a resource to be handled by a function.
 *
 *  This is a shared function to be used by enable_interrupts_chanend_function(),
 *  enable_interrupts_port_function() and enable_interrupts_timer_function().
 *
 *  \param r       The resource to enable interrupts for
 *  \param handler The function to handle the interrupts
 *  \param data    The value to be passed to the interrupt handler function
 */
inline void interrupt_setup_resource_function(resource r,
                                              interrupt_handler handler,
                                              void *data)
{
  int value = interrupt_register_function(r, handler, data);
  // Set the interrupt vector
  asm volatile("ldap r11, __interrupt_function_wrapper" : : : /* clobbers */ "r11");
  asm volatile("setv res[%0], r11" : : "r" (r));

  // Don't call resource_set_ev() function as that requires the value to have
  // bit 16 set
  asm volatile("add r11, %0, 0" : : "r" (value) : /* clobbers */ "r11");
  asm volatile("setev res[%0], r11" : : "r" (r));

  asm volatile("setc res[%0], 0xA" : : "r" (r)); // Raise interrupts instead of events
  interrupt_enable(r);
}

/** Basic setup function for timer.
 *
 *  Function shared by interrupt_setup_timer() and
 *  interrupt_setup_timer_function().
 *
 *  \param t     The timer to enable interrupts for
 *  \param time  The time at which the timer should trigger an event. The
 *               default timer ticks are at a 10ns resolution.
 */
inline void interrupt_setup_timer_common(timer t, int time)
{
  // Set the condition to be AFTER
  asm volatile("setc res[%0], 0x9" :: "r" (t));

  // Set the time at which the interrupt should fire
  asm volatile("setd res[%0], %1" :: "r" (t), "r" (time));
}

#endif // __XC__

#endif // __interrupt_internal_h__
