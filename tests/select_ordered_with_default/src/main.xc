// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include <xs1.h>

extern const int events_per_resource;

void channel_first(chanend c, chanend d);
void timer_first(chanend c, chanend d);

/*
 * Create a channel tester which receives data from two other cores
 * simultaneously
 */
int main()
{
  chan c, d;
  par {
    channel_first(c, d);

    for (int i = 0; i < events_per_resource; i++)
    {
      // Provide test data
      c <: 555 + i;
    }

    for (int i = 0; i < events_per_resource; i++)
    {
      // Provide test data
      d <: 333 + i;
    }
  }
  par {
    timer_first(c, d);

    for (int i = 0; i < events_per_resource; i++)
    {
      // Provide test data
      c <: 555 + i;
    }

    for (int i = 0; i < events_per_resource; i++)
    {
      // Provide test data
      d <: 333 + i;
    }
  }
  return 0;
}
