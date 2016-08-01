// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include <xs1.h>

void channel_example(chanend c, chanend d);

/*
 * Create a channel tester which receives data from two other cores
 * simultaneously
 */
int main()
{
  chan c, d;
  par {
    channel_example(c, d);

    for (int i = 0; i < 5; i++)
    {
      // Provide test data
      c <: 555 + i;
      delay_ticks(20000);
    }

    for (int i = 0; i < 5; i++)
    {
      // Provide test data
      d <: 333 + i;
      delay_ticks(20000);
    }
  }
  return 0;
}
