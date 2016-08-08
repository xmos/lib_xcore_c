// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"
#include "xassert.h"

const size_t array_len = 10;

void print_array_int(const char *prefix, int data[], size_t num_words)
{
  debug_printf("%s", prefix);
  for (size_t i = 0; i < num_words; i++) {
    debug_printf("%d ", data[i]);
  }
  debug_printf("\n");
}

void print_array_char(const char *prefix, char data[], size_t num_bytes)
{
  debug_printf("%s", prefix);
  for (size_t i = 0; i < num_bytes; i++) {
    debug_printf("%c ", data[i]);
  }
  debug_printf("\n");
}

void test_int(chanend c)
{
  // First receive the data as a slave transaction
  int data[array_len];
  transacting_chanend tc;
  chan_init_transaction_slave(&c, &tc);
  xassert(!c && tc.c);
  for (size_t i = 0; i < array_len; i++) {
    t_chan_in_word(&tc, &data[i]);
  }
  chan_complete_transaction(&tc, &c);
  xassert(c && !tc.c);

  print_array_int("C received: ", data, array_len);

  // Change the data
  for (size_t i = 0; i < array_len; i++) {
    data[i] += 10;
  }

  // Send it back
  chan_init_transaction_master(&c, &tc);
  xassert(!c && tc.c);
  for (size_t i = 0; i < array_len; i++) {
    t_chan_out_word(&tc, data[i]);
  }
  chan_complete_transaction(&tc, &c);
  xassert(c && !tc.c);
}

void test_char(chanend c)
{
  // First receive the data as a slave transaction
  char data[array_len];
  transacting_chanend tc;
  chan_init_transaction_master(&c, &tc);
  t_chan_in_buf_byte(&tc, data, array_len);
  chan_complete_transaction(&tc, &c);

  print_array_char("C received: ", data, array_len);

  // Change the data
  for (size_t i = 0; i < array_len; i++) {
    data[i] += 10;
  }

  // Send it back
  chan_init_transaction_slave(&c, &tc);
  t_chan_out_buf_byte(&tc, data, array_len);
  chan_complete_transaction(&tc, &c);
}
