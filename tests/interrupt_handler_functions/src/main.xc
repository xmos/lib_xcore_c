// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include <xs1.h>
#include "debug_print.h"

void test(chanend c, chanend d);

static const int num_tests = 3;

/*
 * Create a channel tester which receives data from two other cores
 * simultaneously
 */
int main()
{
  chan c, d;
  par {
    test(c, d);

    // Provide test data
    for (int test = 0; test < num_tests; ++test) {
      for (int i = 0; i < 10; i++) {
        c <: 555*test + i;
        delay_ticks(5000 * i);
      }
    }

    // Provide test data
    for (int test = 0; test < num_tests; ++test) {
      for (int i = 0; i < 10; i++) {
        d <: 333*test + i;
        delay_ticks(5000 * i);
      }
    }
  }
  return 0;
}
