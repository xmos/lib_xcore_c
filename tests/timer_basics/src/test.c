// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"
#include "xassert.h"

void fast(chanend c)
{
  timer tmr;
  timer_alloc(&tmr);
  xassert(tmr);

  // Wait a short time, so that this core is ahead
  timer_delay(tmr, 100);
  debug_printf("Fast started\n");

  // Get a time a long way in the future that both cores will then wait until
  int time;
  timer_get_time(tmr, &time);
  time += 10000;
  s_chan_out_word(c, time);

  timer_wait_until(tmr, time, &time);

  // Delay a little to guarantee the order of prints
  timer_delay(tmr, 10);
  debug_printf("Fast done\n");

  // Clean up
  timer_free(&tmr);
  xassert(!tmr);

  // Disconnect the channels
  s_chan_out_ct_end(c);
}

void slow(chanend c)
{
  timer tmr;
  timer_alloc(&tmr);
  xassert(tmr);

  // Wait a long time, so that this core is behind
  timer_delay(tmr, 2000);
  debug_printf("Slow started\n");

  // Get the time from the other core to wait until
  int time;
  s_chan_in_word(c, &time);

  timer_wait_until(tmr, time, &time);

  // Print immediately
  debug_printf("Slow done\n");

  // Clean up
  timer_free(&tmr);
  xassert(!tmr);

  // Consume the disconnect token
  s_chan_check_ct_end(c);
}

