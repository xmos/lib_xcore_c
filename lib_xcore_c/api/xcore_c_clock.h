// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_clock_h__
#define __xcore_c_clock_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
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
inline void clock_stop(clock clk)
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
inline void clock_set_source_port(clock clk, port p)
{
  asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (p));
}

/** Configure a clock's source to be the 100MHz reference clock
 *
 *  \param clk  The clock to configure
 */
inline void clock_set_source_clk_ref(clock clk)
{
  asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (0x1));
}

/** Configure a clock's source to be the xCORE clock.
 *
 *  *Note*: When using the xCORE clock as the clock input a divide of > 0 should
 *  be used for the ports to function correclty.
 *
 *  \param clk  The clock to configure
 */
inline void clock_set_source_clk_xcore(clock clk)
{
  asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (0x101));
}

/** Configure the divider for a clock.
 *
 *  A clock can divide its input signal by an integer value which this function
 *  specifies. The XS2 architecture supports dividing the signal from a 1-bit
 *  port while the XS1 architecture will raise a trap if a non-zero divide is
 *  used with a 1-bit port input.
 *
 *  If the clock has been started then this will raise an exception.
 *
 *  If the divide is 0 then the value signal will be passed through the clock.
 *  If the value is non-zero then the clock output will be divided by 2*divide.
 *
 *  \param clk    The clock to configure
 *
 *  \param divide The divide value
 */
inline void clock_set_divide(clock clk, uint8_t divide)
{
  asm volatile("setd res[%0], %1" :: "r" (clk), "r" (divide));
}

#endif // __XC__

#endif // __xcore_c_clock_h__
