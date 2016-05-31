// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_event.h"

extern void event_disable(resource r);
extern void event_enable(resource r);
extern void event_clear_all(void);
extern void event_setup_timer(timer t, unsigned value, int time);
extern void event_setup_timer_function(timer t, event_handler handler,
                                       void *data, int time);
extern void event_clear_timer(timer t);
extern void event_change_timer_time(timer t, int time);
extern void event_setup_chanend(chanend c, unsigned value);
extern void event_setup_chanend_function(chanend c, event_handler handler,
                                         void *data);
extern void event_clear_chanend(chanend c);
extern void event_setup_port(port p, unsigned value);
extern void event_setup_port_function(port p, event_handler handler,
                                      void *data);
extern void event_clear_port(port p);
extern void event_change_port_condition(port p, port_condition cond,
                                        unsigned data);
extern void event_change_port_time(port p, int16_t count);

extern void event_setup_resource(resource r, unsigned value);
extern void event_setup_resource_function(resource r, event_handler handler,
                                          void *data);
extern void event_setup_timer_common(timer t, int time);

/*
 * State to store registered event handler state
 */
event_handler_function_state event_handler_state[EVENT_MAX_HANDLER_FUNCTIONS];

int event_register_function(resource r, event_handler handler, void *data)
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
