// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include <xs1.h>
#include "debug_print.h"

void test(chanend c, chanend d);
void printer();
void c_printer();
void d_printer();
void t_printer();

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
    printer();
    // c_printer();
    // d_printer();
    // t_printer();

    // Provide test data
    for (int test = 0; test < num_tests; ++test) {
      for (int i = 0; i < 5; i++) {
        c <: 555*test + i;
        delay_ticks(5000 * i);
      }
    }

    // Provide test data
    for (int test = 0; test < num_tests; ++test) {
      for (int i = 0; i < 5; i++) {
        d <: 333*test + i;
        delay_ticks(5000 * i);
      }
    }
  }
  return 0;
}
