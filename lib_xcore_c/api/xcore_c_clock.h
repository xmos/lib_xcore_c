// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_clock_h__
#define __xcore_c_clock_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <xccompat.h>
#include <xs1.h>

/** Enable a clock
 *
 *  This function enables a specified clock block and returns a clock
 *  variable denoting the clock.
 *
 *  \param clock_id   The id of the clock to enable
 */
inline clock clock_enable(int clock_id)
{
  asm volatile("setc res[%0], 8" :: "r" (clock_id));
  return (clock)clock_id;
}

/** Disable a clock
 *
 *  \param clk  The clock to disable
 */
inline void clock_disable(clock clk)
{
  asm volatile("setc res[%0], 0" :: "r" (clk));
}

/** Start a clock
 *
 *  This function starts a clock running.
 *
 *  \param clk  The clock to start running
 */
inline void clock_start(clock clk)
{
  asm volatile("setc res[%0], 0xf" :: "r" (clk));
}

/** Stop a clock
 *
 *  This function waits until the clock is low and then pauses a clock.
 *
 *  \param clk  The clock to stop
 */
void clock_stop(clock clk)
{
  asm volatile("setc res[%0], 0x7" :: "r" (clk));
}

/** Configure a clock's source to a 1-bit port
 *
 *  A clock can be a 1-bit port, the reference clock or the xCORE clock. Note
 *  that if the xCORE clock is used then a non-zero divide must be used for
 *  ports to function correctly.
 *
 *  \param clk  The clock to configure
 *
 *  \param p  The 1-bit port to set as the clock input. Attempting to set a
 *            port which is not 1-bit as the input will cause an exception.
 */
void clock_set_source_port(clock clk, port p)
{
  asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (p));
}

/** Configure a clock's source to be the 100MHz reference clock
 *
 *  \param clk  The clock to configure
 */
void clock_set_source_clk_ref(clock clk)
{
  asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (0x1));
}

/** Configure a clock's source to be the xCORE clock
 *
 *  \param clk  The clock to configure
 */
void clock_set_source_clk_xcore(clock clk)
{
  asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (0x101));
}

#endif // __XC__

#endif // __xcore_c_clock_h__
