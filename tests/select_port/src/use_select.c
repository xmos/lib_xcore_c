// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

// For XS1 support all values passed to the event_setup function must have bit 16 set
typedef enum {
  EVENT_TIMER = ENUM_ID_BASE,
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
  select_disable_trigger_all();

  port p;
  port_alloc(&p, port_1A);
  port q;
  port_alloc(&q, port_1B);

  hwtimer_t t;
  timer_alloc(&t);
  int time;
  timer_get_time(t, &time);
  time += period;

  int q_value = 0;
  port_out(q, q_value);

  // Setup the resources for eventing
  timer_setup_select(t, time, EVENT_TIMER);
  timer_enable_trigger(t);
  port_setup_select(p, EVENT_PORT_P);
  port_set_trigger_in_equal(p, 0x1);
  port_enable_trigger(p);

  for (int count = 0; count < 10; count++) {
    port_event_result_t choice = select_wait();
    switch (choice) {
      case EVENT_TIMER: {
        // Read the timer to clear the event
        int dummy;
        timer_get_time(t, &dummy);

        // Set up the next timer event
        time += period;
        timer_change_trigger_time(t, time);

        // Toggle the port value
        q_value = !q_value;
        port_out(q, q_value);

        debug_printf("Timer event, drive %d\n", q_value);
        break;
      }

      case EVENT_PORT_P: {
        // Read the port to clear the event
        int x;
        port_in(p, &x);
        port_set_trigger_in_not_equal(p, x);

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
