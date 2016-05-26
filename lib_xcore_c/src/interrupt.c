// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "interrupt.h"

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
