// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c.h"
#include "debug_print.h"

/*
 * Compile time test of including the lib_xcore_c header from xC which must have
 * been successful if an executable is produced
 */
int main()
{
  debug_printf("Pass\n");
  return 0;
}
