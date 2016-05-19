// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

/*
 * Time the number of reference clock ticks that it takes to drive a number of
 * words out of a 1-bit port. The 1-bit port has been configured as 32-bit
 * buffered so that it is much slower than the instruction rate.
 *
 * The clock is started at the beginning and stopped at the end so that it can
 * be re-configured by the tests.
 */
static void time_port_rate(timer tmr, port p, clock c)
{
  int num_writes = 100;

  clock_start(c);

  port_output(p, 0); // Pre-fill the transfer reg
  port_output(p, 0); // Start running after a clock edge has moved data from the
                     // transfer register
  int start_time = timer_get_time(tmr);
  for (int i = 0; i < num_writes; ++i) {
    port_output(p, i);
  }
  int end_time = timer_get_time(tmr);

  clock_stop(c);

  debug_printf("%d ref ticks per output\n", (end_time - start_time) / num_writes);
}

/*
 * Test the timing using the reference clock as the clock source.
 */
static void test_reference_clock(timer tmr, port p, clock c)
{
  clock_set_source_clk_ref(c);
  clock_set_divide(c, 0);
  time_port_rate(tmr, p, c);
}

/*
 * Test the timing using the xCORE clock as the clock source.
 */
static void test_xcore_clock(timer tmr, port p, clock c)
{
  clock_set_source_clk_xcore(c);
  clock_set_divide(c, 1); // Non-zero divide required for port logic to function correctly
  time_port_rate(tmr, p, c);
}

/*
 * Test the timing using a port as the clock source for a clock block.
 *
 * Use a second clock block to drive a port and then use that port as the input
 * for the first clock block.
 */
static void test_port_clock(timer tmr, port p, clock c)
{
  clock divided_c = clock_enable(XS1_CLKBLK_2);
  clock_set_divide(divided_c, 2);

  // Enable a port to use as a clock source
  port p_clk_src = port_enable(XS1_PORT_1B);
  port_set_clock(p_clk_src, divided_c);
  port_set_mode_clock_port(p_clk_src);

  clock_start(divided_c);

  clock_set_divide(c, 0);
  clock_set_source_port(c, p_clk_src);

  time_port_rate(tmr, p, c);

  clock_stop(divided_c);
  clock_disable(divided_c);
  port_disable(p_clk_src);
}

/*
 * Master test - allocate resources, run the tests and then clean up
 */
void test_clock_sources()
{
  timer tmr = timer_alloc();

  // Allocate the clock to control the port
  clock c = clock_enable(XS1_CLKBLK_1);

  // Enable a port to use as a clock source
  port p = port_enable(XS1_PORT_1A);
  port_set_buffered(p);
  port_set_transfer_width(p, 32);
  port_set_clock(p, c);

  test_reference_clock(tmr, p, c);
  test_xcore_clock(tmr, p, c);
  test_port_clock(tmr, p, c);

  port_disable(p);
  clock_disable(c);
  timer_free(tmr);
}
