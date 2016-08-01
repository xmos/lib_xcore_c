// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

const int events_per_resource = 5;

// For XS1 support all values passed to the event_setup function must have bit 16 set
typedef enum {
  EVENT_CHAN_C = EVENT_ENUM_BASE,
  EVENT_CHAN_D,
  EVENT_TIMER,
  EVENT_NONE
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
  int default_event_count = 0;

  // Expect N events for each of the resources used (and one lot for the default)
  int count = 0;
  while (count < events_per_resource * 4) {
    event_choice_t choice = event_select_ordered_no_wait(ids, EVENT_NONE);
    switch (choice) {
      case EVENT_CHAN_C: {
        // Read value and clear event
        int x = chan_in_word(c);
        debug_printf("Received %d on channel c\n", x);
        count += 1;
        break;
      }
      case EVENT_CHAN_D: {
        // Read value and clear event
        int x = chan_in_word(d);
        debug_printf("Received %d on channel d\n", x);
        count += 1;
        break;
      }
      case EVENT_TIMER: {
        // Read value and clear event
        debug_printf("Timer event %d\n", timer_event_count);
        time = timer_get_time(tmr);
        timer_event_count++;
        if (timer_event_count >= events_per_resource) {
          // Allow the other resources to have time to finish
          time += 10000000;
        }
        else {
          // Give enough time for the default to fire
          time += 1000;
        }
        event_change_timer_time(tmr, time);
        count += 1;
        break;
      }
      case EVENT_NONE: {
        if (default_event_count < events_per_resource) {
          debug_printf("Default\n");
          count += 1;
          default_event_count += 1;
        }
        break;
      }
      default: {
        debug_printf("Unexpected event!!\n");
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

