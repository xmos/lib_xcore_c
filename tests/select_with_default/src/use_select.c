// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

// For XS1 support all values passed to the event_setup function must have bit 16 set
typedef enum {
  EVENT_CHAN_C = EVENT_ENUM_BASE,
  EVENT_CHAN_D,
  EVENT_NONE
} event_choice_t;

void channel_example(chanend c, chanend d)
{
  event_disable_all();

  // Setup the channels to generate events
  event_setup_chanend(c, EVENT_CHAN_C);
  event_enable_chanend(c);
  event_setup_chanend(d, EVENT_CHAN_D);
  event_enable_chanend(d);

  int default_printed = 0;
  int count = 0;
  while (count < 10) {
    event_choice_t choice = event_select_no_wait(EVENT_NONE);
    switch (choice) {
      case EVENT_CHAN_C: {
        // Read value to clear event
        int x;
        chan_in_word(c, &x);
        debug_printf("Received %d on channel c\n", x);
        count += 1;
        default_printed = 0;
        break;
      }
      case EVENT_CHAN_D: {
        // Read value to clear event
        int x;
        chan_in_word(d, &x);
        debug_printf("Received %d on channel d\n", x);
        count += 1;
        default_printed = 0;
        break;
      }
      case EVENT_NONE: {
        if (!default_printed) {
          // Only print default taken once to reduce affect of compiler changes
          // on test output
          debug_printf("Default taken\n");
          default_printed = 1;
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
