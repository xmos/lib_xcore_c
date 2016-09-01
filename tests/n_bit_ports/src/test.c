// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

void port_test_output(chanend c)
{
  port p;
  port_alloc(&p, port_4A);

  int dummy;
  chan_in_word(c, &dummy); // Wait for ack

  for (int i = 0; i < 16; ++i) {
    port_out(p, i);
    chan_in_word(c, &dummy); // Wait for ack
  }

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
  port_alloc(&p, port_4C);

  int input;
  port_in(p, &input);
  chan_out_word(c, 0); // Send ack

  for (int i = 0; i < 16; ++i) {
    int ii;
    port_in_when_pinseq(p, PORT_UNBUFFERED, i, &input);
    if (input != i) {
      debug_printf("Error\n");
    }
    chan_out_word(c, 0); // Send ack
  }

  port_free(&p);

  // Get information about the tile/core running the server for debug messages
  unsigned tile_id = get_local_tile_id();
  unsigned core_id = get_logical_core_id();
  debug_printf("%x:%d: input done\n", tile_id, core_id);
}

