// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "debug_print.h"
#include "xcore_c.h"

void test();

/*
 * Compile time test of calling the lib_xcore_c functions at optimisation levels
 * that will not result in inlining.
 */
int main()
{
  asm volatile (""::"r"(&test));
  debug_printf("Pass\n");
  return 0;
}
