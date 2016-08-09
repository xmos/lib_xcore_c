// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

// For XS1 support all values passed to the event_setup function must have bit 16 set
typedef enum {
  EVENT_TIMER = EVENT_ENUM_BASE,
  EVENT_PORT_P
} port_event_result_t;

/*
 * Test selecting off a port. Uses a timer to change the value on an output port
 * which is looped back to an input port. The input port is set up to event
 * whenever the value changes.
 */
void port_example()
{
  static const int period = 5000;
  event_disable_all();

  port p;
  port_alloc(&p, XS1_PORT_1A);
  port q;
  port_alloc(&q, XS1_PORT_1B);

  timer t;
  timer_alloc(&t);
  int time;
  timer_get_time(t, &time);
  time += period;

  int q_value = 0;
  port_output(q, q_value);

  // Setup the resources for eventing
  event_setup_timer(t, time, EVENT_TIMER);
  event_enable_timer(t);
  event_setup_port(p, EVENT_PORT_P);
  event_change_port_condition(p, PORT_COND_PINSEQ, 0x1);
  event_enable_port(p);

  for (int count = 0; count < 10; count++) {
    port_event_result_t choice = event_select();
    switch (choice) {
      case EVENT_TIMER: {
        // Read the timer to clear the event
        int dummy;
        timer_get_time(t, &dummy);

        // Set up the next timer event
        time += period;
        event_change_timer(t, time);

        // Toggle the port value
        q_value = !q_value;
        port_output(q, q_value);

        debug_printf("Timer event, drive %d\n", q_value);
        break;
      }

      case EVENT_PORT_P: {
        // Read the port to clear the event
        int x;
        port_input(p, &x);
        event_change_port_condition(p, PORT_COND_PINSNEQ, x);

        debug_printf("Port event got %d\n", x);
        break;
      }
    }
  }

  // Release the resources
  timer_free(&t);
  port_free(&q);
  port_free(&p);
}
