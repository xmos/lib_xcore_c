// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include <xs1.h>

void port_test_output(chanend c);
void port_test_input(chanend c);

int main()
{
  chan c0, c1;
  par {
    on tile[0] : port_test_output(c0);
    on tile[0] : port_test_input(c0);
    on tile[1] : port_test_output(c1);
    on tile[1] : port_test_input(c1);
  }
  return 0;
}
