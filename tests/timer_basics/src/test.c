// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"
#include "xassert.h"

void fast(chanend c)
{
  streaming_chanend_t sc = s_chanend_convert(c);
  hwtimer_t tmr;
  hwtimer_alloc(&tmr);
  xassert(tmr);

  // Wait a short time, so that this core is ahead
  hwtimer_delay(tmr, 100);
  debug_printf("Fast started\n");

  // Get a time a long way in the future that both cores will then wait until
  uint32_t time;
  hwtimer_get_time(tmr, &time);
  time += 10000;
  s_chan_out_word(sc, time);

  hwtimer_wait_until(tmr, time, &time);

  // Delay a little to guarantee the order of prints
  hwtimer_delay(tmr, 10);
  debug_printf("Fast done\n");

  // Clean up
  hwtimer_free(&tmr);
  xassert(!tmr);

  // Disconnect the channels
  s_chan_out_ct_end(sc);
}

void slow(chanend c)
{
  streaming_chanend_t sc = s_chanend_convert(c);
  hwtimer_t tmr;
  hwtimer_alloc(&tmr);
  xassert(tmr);

  // Wait a long time, so that this core is behind
  hwtimer_delay(tmr, 2000);
  debug_printf("Slow started\n");

  // Get the time from the other core to wait until
  uint32_t time;
  s_chan_in_word(sc, &time);

  hwtimer_wait_until(tmr, time, &time);

  // Print immediately
  debug_printf("Slow done\n");

  // Clean up
  hwtimer_free(&tmr);
  xassert(!tmr);

  // Consume the disconnect token
  s_chan_check_ct_end(sc);
}

