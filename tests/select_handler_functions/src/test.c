// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

// Timer period which allows enough time for the debug_printfs to complete
static const int period = 10000;

// For XS1 support all values passed to the event_setup function must have bit 16 set
typedef enum {
  EVENT_CHAN_C = ENUM_ID_BASE,
  EVENT_CHAN_D
} event_choice_t;

void test_select_function(chanend c, chanend d)
{
  // Setup the channels to generate events
  chanend_setup_select(c, EVENT_CHAN_C);
  chanend_setup_select(d, EVENT_CHAN_D);
  chanend_enable_trigger(c);
  chanend_enable_trigger(d);

  for (int count = 0; count < 10; count++) {
    event_choice_t choice = select_wait();
    switch (choice) {
      case EVENT_CHAN_C: {
        // Read value and clear event
        uint32_t x;
        chan_in_word(c, &x);
        debug_printf("Received %d on channel c\n", x);
        break;
      }
      case EVENT_CHAN_D: {
        // Read value and clear event
        uint32_t x;
        chan_in_word(d, &x);
        debug_printf("Received %d on channel d\n", x);
        break;
      }
    }
  }
  chanend_disable_trigger(c);
  chanend_disable_trigger(d);
}

typedef struct {
  hwtimer_t t;
  int value;
} hanlde_hwtimer_data;
/*
 * Event handler function. Passed the timer that triggered the event and user
 * data associated with that resource. This simply updates the timer to fire
 * after the period again.
 */
DEFINE_SELECT_CALLBACK(handle_timer, _data)
{
  hanlde_hwtimer_data *data = (hanlde_hwtimer_data*)_data;

  uint32_t time;
  hwtimer_get_time(data->t, &time);
  debug_printf("Timer event data 0x%x\n", data->value);
  hwtimer_change_trigger_time(data->t, time + period);
}

void test(chanend c, chanend d)
{
  // Test enabling a timer outside of the event loop with a function to handle
  // the events.
  hwtimer_t t;
  hwtimer_alloc(&t);
  static hanlde_hwtimer_data data;
  data.t = t;
  data.value = 0xfeedbeef;

  // Test 1: Run the test function with the timer enabled
  uint32_t time;
  hwtimer_get_time(t, &time);
  hwtimer_setup_select_callback(t, time + period, (void*)&data, SELECT_CALLBACK(handle_timer));
  hwtimer_enable_trigger(t);
  test_select_function(c, d);
  hwtimer_disable_trigger(t);

  // Test 2: Run the test function again with the timer disabled
  test_select_function(c, d);

  // Test 3: Run the test function again with the timer enabled
  hwtimer_get_time(t, &time);
  hwtimer_change_trigger_time(t, time + period);
  hwtimer_enable_trigger(t);
  test_select_function(c, d);
  hwtimer_disable_trigger(t);

  hwtimer_free(&t);
}
