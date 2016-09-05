// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <xs1.h>
#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

/*
 * Configure PORT_1A as data with PORT_1B as the valid signal. Ensure that the
 * receiver is ready using a channel end. Send a word of data, delay for a while
 * and send another word to ensure the valid signals are functioning.
 */
void port_test_output(chanend c)
{
  port p;
  port_alloc(&p, port_1A);
  port_set_buffered(p);
  port_set_transfer_width(p, 32);

  port p_ready;
  port_alloc(&p_ready, port_1B);
  clock clk;
  clock_alloc(&clk, clock_1);
  clock_start(clk);

  port_protocol_out_strobed_master(p, p_ready, clk, 0);

  int dummy;
  chan_in_word(c, &dummy); // Wait for ack

  port_out(p, 0xfeedbeef);

  hwtimer_t tmr;
  hwtimer_alloc(&tmr);
  hwtimer_delay(tmr, 1000);
  hwtimer_free(&tmr);

  port_out(p, 0x12345678);

  chan_in_word(c, &dummy); // Wait for ack

  port_free(&p);
  port_free(&p_ready);
  clock_free(&clk);

  // Get information about the tile/core running the server for debug messages
  unsigned tile_id = get_local_tile_id();
  unsigned core_id = get_logical_core_id();
  debug_printf("%x:%d: output done\n", tile_id, core_id);
}

/*
 * Configure PORT_1C as data with PORT_1D as the valid signal. Test receiving
 * two different words of data and check they are the correct values.
 */
void port_test_input(chanend c)
{
  port p;
  port_alloc(&p, port_1C);
  port_set_buffered(p);
  port_set_transfer_width(p, 32);

  port p_ready;
  port_alloc(&p_ready, port_1D);
  clock clk;
  clock_alloc(&clk, clock_2);
  clock_start(clk);

  port_protocol_in_strobed_slave(p, p_ready, clk);

  chan_out_word(c, 0); // Send ack

  unsigned int x;
  port_in(p, &x);
  if (x != 0xfeedbeef) {
    debug_printf("Error %x received instead of 0xfeedbeef\n", x);
  }

  port_in(p, &x);
  if (x != 0x12345678) {
    debug_printf("Error %x received instead of 0x12345678\n", x);
  }

  chan_out_word(c, 0); // Send ack

  port_free(&p);
  port_free(&p_ready);
  clock_free(&clk);

  // Get information about the tile/core running the server for debug messages
  unsigned tile_id = get_local_tile_id();
  unsigned core_id = get_logical_core_id();
  debug_printf("%x:%d: input done\n", tile_id, core_id);
}

