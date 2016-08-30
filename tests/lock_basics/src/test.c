// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"
#include "xassert.h"

void core0(chanend c)
{
  hwtimer_t tmr;
  timer_alloc(&tmr);
  xassert(tmr);
  lock l = 0;
  lock_alloc(&l);
  xassert(l);
  chan_out_word(c, l);

  lock_acquire(l);
  debug_printf("Core0 owns the lock\n");
  timer_delay(tmr, 100);
  debug_printf("Core0 done work\n");
  lock_release(l);

  // Wait for core1 to have finished with the lock before freeing it
  int dummy;
  chan_in_word(c, &dummy);

  lock_free(&l);
  xassert(!l);
  timer_free(&tmr);
  xassert(!tmr);
}

void core1(chanend c)
{
  lock l;
  chan_in_word(c, &l);
  debug_printf("Core1 try acquire\n");
  lock_acquire(l);
  debug_printf("Core1 owns the lock\n");
  lock_release(l);

  // Signal done
  chan_out_word(c, 0);
}

