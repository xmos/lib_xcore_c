// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c.h"
#include "code.h"
#include "debug_print.h"
#include "xassert.h"

#define SHUT_DOWN 0

void get_addresses(chanend c, unsigned *local_server, unsigned *remote_server)
{
  streaming_chanend_t sc = s_chanend_convert(c);
  unsigned tile_id = get_local_tile_id();
  unsigned core_id = get_logical_core_id();

  *local_server = sc;

  // Exchange channel IDs with the other end of a channel
  s_chan_out_word(sc, sc);
  s_chan_out_ct(sc, XS1_CT_END);
  s_chan_in_word(sc, (uint32_t*)remote_server);
  s_chan_check_ct(sc, XS1_CT_END);
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
  streaming_chanend_t sc = s_chanend_convert(c);
  // Get information about the tile/core running the server for debug messages
  unsigned tile_id = get_local_tile_id();
  unsigned core_id = get_logical_core_id();

  while (1) {
    streaming_chanend_t sender;
    s_chan_in_word(sc, (uint32_t*)&sender);
    uint32_t command;
    s_chan_in_word(sc, &command);
    s_chan_check_ct(sc, XS1_CT_END);

    debug_printf("%x:%d: received %d from %x\n", tile_id, core_id, command, sender);

    // Send a response (simply invert the data)
    s_chanend_set_dest(sc, sender);
    s_chan_out_word(sc, ~command);
    s_chan_out_ct(sc, XS1_CT_END);

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
  streaming_chanend_t sc = 0;
  s_chanend_alloc(&sc);

  s_chanend_set_dest(sc, s_chanend_convert(dst));
  s_chan_out_word(sc, sc);
  s_chan_out_word(sc, command);
  s_chan_out_ct(sc, XS1_CT_END);

  uint32_t response;
  s_chan_in_word(sc, &response);
  s_chan_check_ct(sc, XS1_CT_END);

  s_chanend_free(&sc);

  if (command == ~response) {
    debug_printf("Send cmd %d to %x response ok\n", command, dst);
  }
  else {
    debug_printf("Send cmd %d to %x response not ok\n", command, dst);
  }
  return response;
}

/*
 * A test client, communicate with the local server, remote server and then
 * request the local server shuts down.
 */
void chanend_client(streaming_chanend_t local_server, streaming_chanend_t remote_server)
{
  send_command(local_server, 1);
  send_command(remote_server, 2);
  send_command(local_server, SHUT_DOWN);
}

