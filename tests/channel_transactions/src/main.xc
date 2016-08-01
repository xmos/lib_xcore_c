// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include <xs1.h>
#include <stddef.h>
#include "debug_print.h"

// Provided in test.c
extern const size_t array_len;

void test_int(chanend c);
void test_char(chanend c);
extern "C" {
  void print_array_int(const char *prefix, int data[], size_t num_words);
  void print_array_char(const char *prefix, char data[], size_t num_bytes);
}

transaction in_array_int(chanend c, int data[], size_t size)
{
  for (size_t i = 0; i < size; i++) {
    c :> data[i];
  }
}

transaction out_array_int(chanend c, int data[], size_t size)
{
  for (size_t i = 0; i < size; i++) {
    c <: data[i];
  }
}

void xc_test_int(chanend c)
{
  int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  master out_array_int(c, data, array_len);
  slave in_array_int(c, data, array_len);

  print_array_int("XC received: ", data, array_len);
}

void xc_test_char(chanend c)
{
  char data[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

  slave {
    for (size_t i = 0; i < array_len; i++)
    {
      c <: data[i];
    }
  }

  master {
    for (size_t i = 0; i < array_len; i++)
    {
      c :> data[i];
    }
  }
  print_array_char("XC received: ", data, array_len);
}

int main()
{
  chan c, d;
  par {
    test_int(c);
    xc_test_int(c);
  }

  par {
    test_char(d);
    xc_test_char(d);
  }
  return 0;
}
