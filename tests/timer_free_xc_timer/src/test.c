// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c.h"
#include "xassert.h"

/////////////////////////////////////////////////////
// N.B. if we 'print' xscope will require a hwtimer
/////////////////////////////////////////////////////

#define MAX_TIMERS 4

void test(unsigned numTimers)
{
  timer_free_xc_timer();

  xassert(numTimers <= MAX_TIMERS);
  hwtimer_t tmr[MAX_TIMERS];
  for (unsigned i = 0; i < MAX_TIMERS; ++i) {
    if (i < numTimers) {
      timer_alloc(&tmr[i]);
      xassert(tmr[i]);
    }
    timer_delay(tmr[0], 10); // Allow lagging cores to release.
  }

  timer_delay(tmr[0], 100); // Delay so all cores have completed allocation.

  // There are no more timers to allocate.
  hwtimer_t tmr_fail;
  timer_alloc(&tmr_fail);
  xassert(!tmr_fail);  // Failed to allocate.

  exception_t e;
  TRY {
    timer_realloc_xc_timer();
  }
  CATCH(e);
  xassert(e.type == XS1_ET_ECALL); // Failed to reallocate too.

  timer_delay(tmr[0], 100); // Delay so all cores have attempted allocation.

  for (unsigned i = 0; i < MAX_TIMERS; ++i) {
    if (i < numTimers) {
      timer_free(&tmr[i]);
      xassert(!tmr[i]);
    }
  }

  timer_realloc_xc_timer();
}

