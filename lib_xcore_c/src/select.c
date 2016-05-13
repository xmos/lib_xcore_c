// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "select.h"

/*
 * State to store registered event handler state
 */
event_handler_function_state_t event_handler_state[EVENT_MAX_HANDLER_FUNCTIONS];

int event_register_function(resource r, event_handler_t handler, void *data)
{
  for (int i = 0; i < EVENT_MAX_HANDLER_FUNCTIONS; ++i)
  {
  	if (event_handler_state[i].owner == 0) {
  	  event_handler_state[i].owner = r;
  	  event_handler_state[i].handler = handler;
  	  event_handler_state[i].data = data;
  	  return i;
  	}
  }

  return -1;
}

/** Deregister an event handler for a resource
 *
 *  Determine whether there is an event handler function registered for a given
 *  resource. If so, remove the entry. If there is none registered then simply
 *  ignore it.
 *
 *  \param r  The resource to de-register
 */
void event_deregister_function(resource r)
{
  for (int i = 0; i < EVENT_MAX_HANDLER_FUNCTIONS; ++i)
  {
  	if (event_handler_state[i].owner == r) {
  	  event_handler_state[i].owner = 0;
  	}
  }
}
