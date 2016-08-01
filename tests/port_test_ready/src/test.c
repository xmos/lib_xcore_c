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
  port_alloc(&p, XS1_PORT_1A);
  port_set_buffered(p);
  port_set_transfer_width(p, 32);

  port p_ready;
  port_alloc(&p_ready, XS1_PORT_1B);
  clock clk;
  clock_alloc(&clk, XS1_CLKBLK_1);
  clock_start(clk);

  port_configure_out_strobed_master(p, p_ready, clk, 0);

  chan_input_word(c); // Wait for ack

  port_output(p, 0xfeedbeef);

  timer tmr = timer_alloc();
  timer_delay(tmr, 1000);
  timer_free(tmr);

  port_output(p, 0x12345678);

  chan_input_word(c); // Wait for ack

  port_free(p);
  port_free(p_ready);
  clock_free(clk);

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
  port_alloc(&p, XS1_PORT_1C);
  port_set_buffered(p);
  port_set_transfer_width(p, 32);

  port p_ready;
  port_alloc(&p_ready, XS1_PORT_1D);
  clock clk;
  clock_alloc(&clk, XS1_CLKBLK_2);
  clock_start(clk);

  port_configure_in_strobed_slave(p, p_ready, clk);

  chan_output_word(c, 0); // Send ack

  unsigned int x = port_input(p);
  if (x != 0xfeedbeef) {
    debug_printf("Error %x received instead of 0xfeedbeef\n", x);
  }

  x = port_input(p);
  if (x != 0x12345678) {
    debug_printf("Error %x received instead of 0x12345678\n", x);
  }

  chan_output_word(c, 0); // Send ack

  port_free(p);
  port_free(p_ready);
  clock_free(clk);

  // Get information about the tile/core running the server for debug messages
  unsigned tile_id = get_local_tile_id();
  unsigned core_id = get_logical_core_id();
  debug_printf("%x:%d: input done\n", tile_id, core_id);
}

