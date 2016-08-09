// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_clock_h__
#define __xcore_c_clock_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include "xcore_c_exception_impl.h"

/** Allocates a clock
 *
 *  This function enables a specified clock block and returns a clock
 *  variable denoting the clock.
 *
 *  \param clk        Clock variable representing the initialised clock
 *
 *  \param clock_id   The id of the clock to allocate
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 *  \exception  ET_LOAD_STORE         invalid *clk argument.
 */
inline unsigned clock_alloc(clock *clk, int clock_id)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setc res[%0], 8" :: "r" (clock_id)); \
                                *clk = clock_id; \
                              } while (0) );
}

/** Deallocate a clock
 *
 *  \param clk  The clock to be freed
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 *  \exception  ET_LOAD_STORE         invalid *clk argument.
 */
inline unsigned clock_free(clock *clk)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setc res[%0], 0" :: "r" (*clk)); \
                                *clk = 0; \
                              } while (0) );
}

/** Start a clock
 *
 *  This function starts a clock running.
 *
 *  \param clk  The clock to start running
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 */
inline unsigned clock_start(clock clk)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setc res[%0], 0xf" :: "r" (clk)); \
                              } while (0) );
}

/** Stop a clock
 *
 *  This function waits until the clock is low and then pauses a clock.
 *
 *  \param clk  The clock to stop
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 */
inline unsigned clock_stop(clock clk)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setc res[%0], 0x7" :: "r" (clk)); \
                              } while (0) );
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
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock or port,
 *                                    or the clock is running.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 */
inline unsigned clock_set_source_port(clock clk, port p)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (p)); \
                              } while (0) );
}

/** Configure a clock's source to be the 100MHz reference clock
 *
 *  \param clk  The clock to configure
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock,
 *                                    or the clock is running.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 */
inline unsigned clock_set_source_clk_ref(clock clk)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (0x1)); \
                              } while (0) );
}

/** Configure a clock's source to be the xCORE clock.
 *
 *  *Note*: When using the xCORE clock as the clock input a divide of > 0 must
 *  be used for the ports to function correclty.
 *
 *  \param clk  The clock to configure
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock,
 *                                    or the clock is running.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 */
inline unsigned clock_set_source_clk_xcore(clock clk)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setclk res[%0], %1" :: "r" (clk), "r" (0x101)); \
                              } while (0) );
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
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock,
 *                                    or the clock is running.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 */
inline unsigned clock_set_divide(clock clk, uint8_t divide)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setd res[%0], %1" :: "r" (clk), "r" (divide)); \
                              } while (0) );
}

/** Sets a clock to use a 1-bit port for the ready-in signal.
 *
 *  If the port is not a 1-bit port then an exception is raised. The ready-in
 *  port controls when data is sampled from the pins.
 *
 *  \param clk          The clock to configure.
 *
 *  \param ready_source The 1-bit port to use for the ready-in signal.
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid clock.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the clock.
 */
inline unsigned clock_set_ready_src(clock clk, port ready_source)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setrdy res[%0], %1" :: "r" (clk), "r" (ready_source)); \
                              } while (0) );
}

#endif // __XC__

#endif // __xcore_c_clock_h__
