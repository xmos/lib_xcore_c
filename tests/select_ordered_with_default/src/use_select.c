// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

const int events_per_resource = 5;

// For XS1 support all values passed to the event_setup function must have bit 16 set
typedef enum {
  EVENT_CHAN_C = ENUM_ID_BASE,
  EVENT_CHAN_D,
  EVENT_TIMER,
  EVENT_NONE
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
  int default_event_count = 0;

  // Expect N events for each of the resources used (and one lot for the default)
  int count = 0;
  while (count < events_per_resource * 4) {
    event_choice_t choice = select_no_wait_ordered(EVENT_NONE, ids);
    switch (choice) {
      case EVENT_CHAN_C: {
        // Read value to clear event
        uint32_t x;
        chan_in_word(c, &x);
        debug_printf("Received %d on channel c\n", x);
        count += 1;
        break;
      }
      case EVENT_CHAN_D: {
        // Read value to clear event
        uint32_t x;
        chan_in_word(d, &x);
        debug_printf("Received %d on channel d\n", x);
        count += 1;
        break;
      }
      case EVENT_TIMER: {
        // Read value to clear event
        debug_printf("Timer event %d\n", timer_event_count);
        timer_get_time(tmr, &time);
        timer_event_count++;
        if (timer_event_count >= events_per_resource) {
          // Allow the other resources to have time to finish
          time += 10000000;
        }
        else {
          // Give enough time for the default to fire
          time += 1000;
        }
        timer_change_trigger_time(tmr, time);
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

