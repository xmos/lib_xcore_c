// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c.h"
#include "code.h"
#include "debug_print.h"

#define SHUT_DOWN 0

void get_addresses(chanend c, unsigned *local_server, unsigned *remote_server)
{
  unsigned tile_id = get_local_tile_id();
  unsigned core_id = get_logical_core_id();

  *local_server = c;

  // Exchange channel IDs with the other end of a channel
  s_chan_output_word(c, c);
  s_chan_output_ct(c, XS1_CT_END);
  *remote_server = s_chan_input_word(c);
  s_chan_check_ct(c, XS1_CT_END);
  debug_printf("%x:%d: get_addresses: local %x, remote: %x\n",
    tile_id, core_id, *local_server, *remote_server);
}

/*
 * A server that receives data on a channel end. The first word of data is the
 * return address, the second word is data that it does a bitwise inversion
 * and returns before closing the connection.
 */
void chanend_server(chanend c)
{
  // Get information about the tile/core running the server for debug messages
  unsigned tile_id = get_local_tile_id();
  unsigned core_id = get_logical_core_id();

  while(1) {
    chanend sender = s_chan_input_word(c);
    int command = s_chan_input_word(c);
    s_chan_check_ct(c, XS1_CT_END);

    debug_printf("%x:%d: received %d from %x\n", tile_id, core_id, command, sender);

    // Send a response (simply invert the data)
    chanend_set_dest(c, sender);
    s_chan_output_word(c, ~command);
    s_chan_output_ct(c, XS1_CT_END);

    if (command == SHUT_DOWN) {
      debug_printf("%x:%d: shutting down\n", tile_id, core_id);
      break;
    }
  }
}

/*
 * Send a command to a server. Send the channel address to return a response to.
 * Read the response and display it.
 */
int send_command(chanend dst, int command)
{
  chanend c;
  chanend_alloc(&c);
  chanend_set_dest(c, dst);
  s_chan_output_word(c, c);
  s_chan_output_word(c, command);
  s_chan_output_ct(c, XS1_CT_END);

  int response = s_chan_input_word(c);
  s_chan_check_ct(c, XS1_CT_END);

  chanend_free(c);
  if (command == ~response) {
    debug_printf("Send cmd %d to %x response ok\n", command, dst);
  } else {
    debug_printf("Send cmd %d to %x response not ok\n", command, dst);
  }
  return response;
}

/*
 * A test client, communicate with the local server, remote server and then
 * request the local server shuts down.
 */
void chanend_client(chanend local_server, chanend remote_server)
{
  send_command(local_server, 1);
  send_command(remote_server, 2);
  send_command(local_server, SHUT_DOWN);
}
