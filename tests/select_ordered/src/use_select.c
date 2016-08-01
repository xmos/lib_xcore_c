// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

const int events_per_resource = 5;

// For XS1 support all values passed to the event_setup function must have bit 16 set
typedef enum {
  EVENT_CHAN_C = EVENT_ENUM_BASE,
  EVENT_CHAN_D,
  EVENT_TIMER
} event_choice_t;

void test(chanend c, chanend d, timer tmr, resource ids[])
{
  int time = timer_get_time(tmr);
  time += 10;

  // Setup the channels to generate events
  event_setup_chanend(c, EVENT_CHAN_C);
  event_setup_chanend(d, EVENT_CHAN_D);
  event_setup_timer(tmr, EVENT_TIMER, time);

  int timer_event_count = 0;

  // Expect N events for each of the resources used
  for (int count = 0; count < events_per_resource * 3; count++) {
    event_choice_t choice = event_select_ordered(ids);
    switch (choice) {
      case EVENT_CHAN_C: {
        // Read value and clear event
        int x = chan_in_word(c);
        debug_printf("Received %d on channel c\n", x);
        break;
      }
      case EVENT_CHAN_D: {
        // Read value and clear event
        int x = chan_in_word(d);
        debug_printf("Received %d on channel d\n", x);
        break;
      }
      case EVENT_TIMER: {
        // Read value and clear event
        time = timer_get_time(tmr);
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
        event_change_timer_time(tmr, time);
        break;
      }
    }
  }
}

void channel_first(chanend c, chanend d)
{
  // No need to clear all events first as the event_select_ordered will do that
  debug_printf("Running with order [c, d, timer]\n");

  timer tmr;
  timer_alloc(&tmr);

  resource ids[4] = {c, d, tmr, 0};
  test(c, d, tmr, ids);

  timer_free(tmr);
}

void timer_first(chanend c, chanend d)
{
  // No need to clear all events first as the event_select_ordered will do that
  debug_printf("Running with order [timer, d, c]\n");

  timer tmr;
  timer_alloc(&tmr);

  resource ids[4] = {tmr, d, c, 0};
  test(c, d, tmr, ids);

  timer_free(tmr);
}

