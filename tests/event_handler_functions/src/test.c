// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

// Timer period which allows enough time for the debug_printfs to complete
static const int period = 10000;

// For XS1 support all values passed to the event_setup function must have bit 16 set
typedef enum {
  EVENT_CHAN_C = EVENT_ENUM_BASE,
  EVENT_CHAN_D
} event_choice_t;

void test_select_function(chanend c, chanend d)
{
  // Setup the channels to generate events
  event_setup_chanend(c, EVENT_CHAN_C);
  event_setup_chanend(d, EVENT_CHAN_D);

  for (int count = 0; count < 10; count++) {
    event_choice_t choice = event_select();
    switch (choice) {
      case EVENT_CHAN_C: {
        // Read value and clear event
        int x = chan_input_word(c);
        debug_printf("Received %d on channel c\n", x);
        break;
      }
      case EVENT_CHAN_D: {
        // Read value and clear event
        int x = chan_input_word(d);
        debug_printf("Received %d on channel d\n", x);
        break;
      }
    }
  }
  event_clear_chanend(c);
  event_clear_chanend(d);
}

/*
 * Event handler function. Passed the timer that triggered the event and user
 * data associated with that resource. This simply updates the timer to fire
 * after the period again.
 */
void handle_timer(resource r, void *data)
{
  timer t = (timer)r;

  int time = timer_get_time(t);
  debug_printf("Timer event data 0x%x\n", (int)data);
  event_change_timer_time(t, time + period);
}

void test(chanend c, chanend d)
{
  // Test enabling a timer outside of the event loop with a function to handle
  // the events.
  timer t = timer_alloc();

  // Test 1: Run the test function with the timer enabled
  int time = timer_get_time(t);
  event_setup_timer_function(t, handle_timer, (void*)0xfeedbeef, time + period);
  test_select_function(c, d);

  // Test 2: Run the test function again with the timer disabled
  event_clear_timer(t);
  test_select_function(c, d);

  // Test 3: Run the test function again with the timer enabled
  time = timer_get_time(t);
  event_setup_timer_function(t, handle_timer, (void*)0xfeedbeef, time + period);
  test_select_function(c, d);
}
