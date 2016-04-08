// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

// For XS1 support all values passed to the event_setup function must have bit 16 set
typedef enum {
  CHAN_C = 0x10000,
  CHAN_D
} chan_event_result_t;

void channel_example(chanend c, chanend d)
{
  event_clear_all();

  // Setup the resources for eventing
  event_setup_chanend(c, CHAN_C);
  event_setup_chanend(d, CHAN_D);

  for (int count = 0; count < 10; count++) {
    chan_event_result_t choice = event_select();
    switch (choice) {
      case CHAN_C: {
        int x = chan_input_word(c);
        debug_printf("Received %d on channel c\n", x);
        break;
      }
      case CHAN_D: {
        int x = chan_input_word(d);
        debug_printf("Received %d on channel d\n", x);
        break;
      }
    }
  }
}

// For XS1 support all values passed to the event_setup function must have bit 16 set
typedef enum {
  CHAN_E = 0x10000,
  CHAN_F
} chan_event_fair_result_t;

void channel_example_fair(chanend e, chanend f)
{
  event_clear_all();

  // Setup the resources for eventing
  event_setup_chanend(e, CHAN_E);
  event_setup_chanend(f, CHAN_F);

  for (int count = 0; count < 10; count++) {
    chan_event_fair_result_t choice = event_select();
    switch (choice) {
      case CHAN_E: {
        int x = chan_input_word(e);
        event_disable(e);
        event_enable(f);
        debug_printf("Received %d on channel e\n", x);
        break;
      }
      case CHAN_F: {
        int x = chan_input_word(f);
        event_disable(f);
        event_enable(e);
        debug_printf("Received %d on channel f\n", x);
        break;
      }
    }
  }
}

// For XS1 support all values passed to the event_setup function must have bit 16 set
typedef enum {
  TIMER = 0x10000,
  PORT_P
} port_event_result_t;

void port_example(port p, port q)
{
  static const int period = 5000;
  event_clear_all();

  timer t = timer_alloc();
  int time = timer_get_time(t);
  time += period;

  int q_value = 0;
  port_output(q, q_value);

  // Setup the resources for eventing
  event_setup_timer(t, TIMER, time);
  event_setup_port(p, PORT_P);

  event_change_port_condition(p, PORT_COND_PINSEQ, 0x1);

  for (int count = 0; count < 10; count++) {
    port_event_result_t choice = event_select();
    switch (choice) {
      case TIMER: {
        int now = timer_get_time(t);
        time += period;
        event_change_timer_time(t, time);

        // Toggle the port value
        q_value = !q_value;
        port_output(q, q_value);

        debug_printf("Timer event at time %d\n", now);
        break;
      }

      case PORT_P: {
        int x = port_input(p);
        event_change_port_condition(p, PORT_COND_PINSNEQ, x);

        debug_printf("Port event got %d\n", x);
        break;
      }
    }
  }
}
