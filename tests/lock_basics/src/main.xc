// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include <xs1.h>

void core0(chanend c);
void core1(chanend c);

int main()
{
  chan c;
  par {
    core0(c);
    core1(c);
  }
  return 0;
}
