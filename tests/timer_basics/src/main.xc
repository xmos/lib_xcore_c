// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include <xs1.h>

void fast(chanend c);
void slow(chanend c);

int main()
{
  chan c;
  par {
    fast(c);
    slow(c);
  }
  return 0;
}
