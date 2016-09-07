// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include <xs1.h>
#include "debug_print.h"

void test(unsigned numTimers);

int main()
{
  par {
    test(4);
    test(3);
    test(2); // 4+3+2 = 9 timers
#ifdef __XS2A__
    test(1); // 4+3+2+1 = 10 timers.
#else
    // 4+3+2 = 9 timers + 1 for xscope.
#endif
  }
  debug_printf("success\n");
  return 0;
}
