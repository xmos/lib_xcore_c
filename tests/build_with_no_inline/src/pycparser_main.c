// Copyright (c) 2016, XMOS Ltd, All rights reserved

/* The following defines are a fix for unsupported compiler extensions and
 * language features in pycparser
 */
#define __attribute__(x)
#define asm(...)
#define volatile(...)
#define _Static_assert(...)

#include "xcore_c.h"

void test();

/*
 * Compile time test of calling the lib_xcore_c functions at optimisation levels
 * that will not result in inlining.
 */
int main()
{
  test();
  return 0;
}
