// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

const int events_per_resource = 5;

// For XS1 support all values passed to the event_setup function must have bit 16 set
typedef enum {
  EVENT_CHAN_C = ENUM_ID_BASE,
  EVENT_CHAN_D,
  EVENT_TIMER
} event_choice_t;

void test(chanend c, chanend d, hwtimer_t tmr, resource_t ids[])
{
  uint32_t time;
  timer_get_time(tmr, &time);
  time += 10;

  // Setup the channels to generate events
  chanend_setup_select(c, EVENT_CHAN_C);
  chanend_enable_trigger(c);
  chanend_setup_select(d, EVENT_CHAN_D);
  chanend_enable_trigger(d);
  timer_setup_select(tmr, time, EVENT_TIMER);
  timer_enable_trigger(tmr);

  int timer_event_count = 0;

  // Expect N events for each of the resources used
  for (int count = 0; count < events_per_resource * 3; count++) {
    event_choice_t choice = select_wait_ordered(ids);
    switch (choice) {
      case EVENT_CHAN_C: {
        // Read value to clear event
        uint32_t x;
        chan_in_word(c, &x);
        debug_printf("Received %d on channel c\n", x);
        break;
      }
      case EVENT_CHAN_D: {
        // Read value to clear event
        uint32_t x;
        chan_in_word(d, &x);
        debug_printf("Received %d on channel d\n", x);
        break;
      }
      case EVENT_TIMER: {
        // Read value to clear event
        timer_get_time(tmr, &time);
        timer_event_count++;
        debug_printf("Timer event %d\n", timer_event_count);
        if (timer_event_count >= events_per_resource) {
          // Allow the other resources to have time
          time += 10000000;
        }
        else {
          // Don't give time for other resources to be ready
          time += 10;
        }
        timer_change_trigger_time(tmr, time);
        break;
      }
    }
  }
}

void channel_first(chanend c, chanend d)
{
  // No need to clear all events first as the select_wait_ordered will do that
  debug_printf("Running with order [c, d, timer]\n");

  hwtimer_t tmr;
  timer_alloc(&tmr);

  resource_t ids[4] = {c, d, tmr, 0};
  test(c, d, tmr, ids);

  timer_free(&tmr);
}

void timer_first(chanend c, chanend d)
{
  // No need to clear all events first as the select_wait_ordered will do that
  debug_printf("Running with order [timer, d, c]\n");

  hwtimer_t tmr;
  timer_alloc(&tmr);

  resource_t ids[4] = {tmr, d, c, 0};
  test(c, d, tmr, ids);

  timer_free(&tmr);
}

