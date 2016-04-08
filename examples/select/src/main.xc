// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include <xs1.h>

void channel_example(chanend c, chanend d);
void channel_example_fair(chanend c, chanend d);
void port_example(port p, port q);

port p = XS1_PORT_1A;
port q = XS1_PORT_1B;

int main()
{
  chan c, d, e, f;
  par {
    channel_example(c, d);
    channel_example_fair(e, f);
    port_example(p, q);

    for (int i = 0; i < 5; i++) {
      d <: 333 + i;
    }

    for (int i = 0; i < 5; i++) {
      c <: 555 + i;
    }

    for (int i = 0; i < 5; i++) {
      e <: 777 + i;
    }

    for (int i = 0; i < 5; i++) {
      f <: 999 + i;
    }
  }
  return 0;
}
