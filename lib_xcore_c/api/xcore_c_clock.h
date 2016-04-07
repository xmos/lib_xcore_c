// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xio_clock_h__
#define __xio_clock_h__

#ifndef __XC__

#include <xccompat.h>
#include <xs1.h>

/** Enable a clock.
 *
 *  This function enables a specified clock block and returns a clock
 *  variable denoting the clock.
 *
 *  \param clock_id   The id of the clock to enable
 */
inline clock clock_enable(int clock_id) {
  asm volatile("setc res[%0], 8" :: "r" (clock_id));
  return (clock)clock_id;
}

/** Disable a clock
 *
 *  \param clk  The clock to disable
 */
inline void clock_disable(clock clk) {
  asm volatile("setc res[%0], 0" :: "r" (clk));
}

/** Start a clock
 *
 *  This function starts a clock running.
 *
 *  \param clk  The clock to start running
 */
inline void clock_start(clock clk) {
  asm volatile("setc res[%0], 0xf" :: "r" (clk));
}

/** Stop a clock
 *
 *  This function waits until the clock is low and then pauses a clock.
 *
 *  \param clk  The clock to stop
 */
void clock_stop(clock clk) {
  asm volatile("setc res[%0], 0x7" :: "r" (clk));
}

#endif // __XC__

#endif // __xio_clock_h__
