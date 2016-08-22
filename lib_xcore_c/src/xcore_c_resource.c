// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_resource_impl.h"
extern void _resource_enable_trigger(resource r);
extern void _resource_disable_trigger(resource r);
extern void _resource_setup_interrupt_callback(resource r, void *data, interrupt_callback intrpt);
extern void _resource_setup_select_callback(resource r, void *data, select_callback callback);
extern void _resource_setup_select(resource r, uint32_t value);

void _resource_setup_callback(resource r, void *data, callback_function func, int type)
{
#if !defined(__XS2A__)
  xassert( ((unsigned)data >> 16) == 0x1 && msg("On XS1 bit 16 will always be set in the data returned from an event"));
#endif
  asm volatile("add r11, %0, 0" : : "r" (func) : /* clobbers */ "r11");
  asm volatile("setv res[%0], r11" : : "r" (r));
  asm volatile("add r11, %0, 0" : : "r" (data) : /* clobbers */ "r11");
  asm volatile("setev res[%0], r11" : : "r" (r));
  asm volatile("setc res[%0], %1" : : "r" (r), "r" (type)); // Raise interrupts or events
}
