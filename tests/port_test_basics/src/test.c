// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

void port_test_output(chanend c)
{
  port p;
  port_alloc(&p, XS1_PORT_1A);

  int dummy;
  port_output(p, 0);
  chan_in_word(c, &dummy); // Wait for ack
  port_output(p, 1);
  chan_in_word(c, &dummy); // Wait for ack
  port_output(p, 0);
  chan_in_word(c, &dummy); // Wait for ack
  port_output(p, 1);
  chan_in_word(c, &dummy); // Wait for ack
  port_output(p, 0);
  chan_in_word(c, &dummy); // Wait for ack

  port_free(&p);

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
  port p;
  port_alloc(&p, XS1_PORT_1B);

  int input;
  port_input(p, &input);
  chan_out_word(c, 0); // Send ack

  port_input_when_pinseq(p, 1, &input);
  if (input != 1) {
    debug_printf("Error\n");
  }
  chan_out_word(c, 0); // Send ack

  port_input_when_pinsneq(p, 1, &input);
  if (input != 0) {
    debug_printf("Error\n");
  }
  chan_out_word(c, 0); // Send ack

  port_input_when_pinsneq(p, 0, &input);
  if (input != 1) {
    debug_printf("Error\n");
  }
  chan_out_word(c, 0); // Send ack

  port_input_when_pinseq(p, 0, &input);
  if (input != 0) {
    debug_printf("Error\n");
  }
  chan_out_word(c, 0); // Send ack

  port_free(&p);

  // Get information about the tile/core running the server for debug messages
  unsigned tile_id = get_local_tile_id();
  unsigned core_id = get_logical_core_id();
  debug_printf("%x:%d: input done\n", tile_id, core_id);
}

