// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

void port_test_output(chanend c)
{
  port p = port_enable(XS1_PORT_1A);

  port_output(p, 0);
  chan_input_word(c); // Wait for ack
  port_output(p, 1);
  chan_input_word(c); // Wait for ack
  port_output(p, 0);
  chan_input_word(c); // Wait for ack
  port_output(p, 1);
  chan_input_word(c); // Wait for ack
  port_output(p, 0);
  chan_input_word(c); // Wait for ack

  port_disable(p);

  // Get information about the tile/core running the server for debug messages
  unsigned tile_id = get_local_tile_id();
  unsigned core_id = get_logical_core_id();
  debug_printf("%x:%d: output done\n", tile_id, core_id);
}

/*
 * Test inputting from a port. Waits for the value to change to the desired value.
 */
void port_test_input(chanend c)
{
  port p = port_enable(XS1_PORT_1B);

  port_input(p);
  chan_output_word(c, 0); // Send ack

  if (port_input_when_pinseq(p, 1) != 1) {
    debug_printf("Error\n");
  }
  chan_output_word(c, 0); // Send ack

  if (port_input_when_pinsneq(p, 1) != 0) {
    debug_printf("Error\n");
  }
  chan_output_word(c, 0); // Send ack

  if (port_input_when_pinsneq(p, 0) != 1) {
    debug_printf("Error\n");
  }
  chan_output_word(c, 0); // Send ack

  if (port_input_when_pinseq(p, 0) != 0) {
    debug_printf("Error\n");
  }
  chan_output_word(c, 0); // Send ack

  port_disable(p);

  // Get information about the tile/core running the server for debug messages
  unsigned tile_id = get_local_tile_id();
  unsigned core_id = get_logical_core_id();
  debug_printf("%x:%d: input done\n", tile_id, core_id);
}