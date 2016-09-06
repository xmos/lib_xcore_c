// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

// For XS1 support all values passed to the event_setup function must have bit 16 set
typedef enum {
  EVENT_CHAN_C = ENUM_ID_BASE,
  EVENT_CHAN_D
} event_choice_t;

/*
 * Test receiving from two channels fairly. When data is received on one, it
 * disables events on itself and re-enables events on the other channel.
 */
void channel_example_fair(chanend c, chanend d)
{
  select_disable_trigger_all();

  // Setup the channels to generate events
  chanend_setup_select(c, EVENT_CHAN_C);
  chanend_enable_trigger(c);
  chanend_setup_select(d, EVENT_CHAN_D);
  chanend_enable_trigger(d);

  for (int count = 0; count < 10; count++) {
    event_choice_t choice = select_wait();
    switch (choice) {
      case EVENT_CHAN_C: {
        // Read value to clear event and toggle enabled event
        uint32_t x;
        chan_in_word(c, &x);
        chanend_disable_trigger(c);
        chanend_enable_trigger(d);
        debug_printf("Received %d on channel c\n", x);
        break;
      }
      case EVENT_CHAN_D: {
        // Read value to clear event and toggle enabled event
        uint32_t x;
        chan_in_word(d, &x);
        chanend_disable_trigger(d);
        chanend_enable_trigger(c);
        debug_printf("Received %d on channel d\n", x);
        break;
      }
    }
  }
}
