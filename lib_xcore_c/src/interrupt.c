// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "interrupt.h"

extern void interrupt_disable(resource r);
extern void interrupt_enable(resource r);
extern void interrupt_mask_all(void);
extern void interrupt_unmask_all(void);
extern void interrupt_setup_timer_function(timer t, interrupt_handler handler,
                                           void *data, int time);
extern void interrupt_clear_timer(timer t);
extern void interrupt_change_timer_time(timer t, int time);
extern void interrupt_setup_chanend_function(chanend c,
                                             interrupt_handler handler,
                                             void *data);
extern void interrupt_clear_chanend(chanend c);
extern void interrupt_setup_port_function(port p, interrupt_handler handler,
                                          void *data);
extern void interrupt_clear_port(port p);
extern void interrupt_change_port_condition(port p,
                                            port_condition cond,
                                            unsigned data);
extern void interrupt_change_port_time(port p, int16_t count);

extern void interrupt_setup_resource_function(resource r,
                                              interrupt_handler handler,
                                              void *data);
extern void interrupt_setup_timer_common(timer t, int time);

/*
 * State to store registered interrupt handler state
 */
interrupt_handler_function_state interrupt_handler_state[INTERRUPT_MAX_HANDLER_FUNCTIONS];

int interrupt_register_function(resource r, interrupt_handler handler, void *data)
{
  for (int i = 0; i < INTERRUPT_MAX_HANDLER_FUNCTIONS; ++i)
  {
    if (interrupt_handler_state[i].owner == 0) {
      interrupt_handler_state[i].owner = r;
      interrupt_handler_state[i].handler = handler;
      interrupt_handler_state[i].data = data;
      return i;
    }
  }

  return -1;
}

/** Deregister an interrupt handler for a resource
 *
 *  Determine whether there is an interrupt handler function registered for a
 *  given resource. If so, remove the entry. If there is none registered then
 *  simply ignore it.
 *
 *  \param r  The resource to de-register
 */
void interrupt_deregister_function(resource r)
{
  for (int i = 0; i < INTERRUPT_MAX_HANDLER_FUNCTIONS; ++i)
  {
    if (interrupt_handler_state[i].owner == r) {
      interrupt_handler_state[i].owner = 0;
    }
  }
}
