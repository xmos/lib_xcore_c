// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_port_h__
#define __xcore_c_port_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <stdint.h>
#include "xcore_c_port_impl.h"
#include "xcore_c_exception_impl.h"

/** Allocates a port.
 *
 *  The port_id must be one of XS1_PORT_1A .. XS1_PORT_32A as specified on the
 *  datasheet and in the xs1.h include file. Either this function or
 *  port_alloc_buffered() must be called once for each variable of type
 *  ``port`` before use. port_free() must be called afterwards.
 *
 *  \param p         Port variable representing the initialised port
 *  \param port_id   Value that identifies which port to drive
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *p argument.
 */
inline unsigned port_alloc(port *p, int port_id)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setc res[%0], 8" :: "r" (port_id)); \
                                *p = port_id; \
                              } while (0) );
}

/** Allocates a port to buffer and serialise/deserialise data.
 *
 *  The port_id must be one of XS1_PORT_1A .. XS1_PORT_32A as specified on the
 *  datasheet and in the xs1.h include file. Either this function or port_alloc()
 *  must be called once for each variable of type ``port`` before use.
 *  port_free() must be called afterwards.
 *
 *  \param p               Port variable representing the initialised port
 *  \param port_id         Value that identifies which port to drive
 *  \param transfer_width  Number of bits to serialise; must be 1, 4, 8, or 32.
 *                         The number of bits must be >= to the physical port
 *                         width.
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port,
 *                                    or is not legal width for the port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *p argument.
 */
inline unsigned port_alloc_buffered(port *p, int port_id, int transfer_width)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setc res[%0], 8" :: "r" (port_id)); \
                                asm volatile("setc res[%0], 0x200f" :: "r" (port_id)); \
                                asm volatile("settw res[%0], %1" :: "r" (port_id), "r" (transfer_width)); \
                                *p = port_id; \
                              } while (0) );
}

/** Deallocate a port
 *
 *  \param p    The port to be freed
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *p argument.
 */
inline unsigned port_free(port *p)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setc res[%0], 0" :: "r" (*p)); \
                                *p = 0; \
                              } while (0) );
}

/** Change the transfer width of a port.
 *
 *  Note that a port must have been set to buffered before calling this function.
 *
 *  \param p               The port to change the transfer width of
 *  \param transfer_width  Number of bits to serialise; must be 1, 4, 8, or 32.
 *                         The number of bits must be >= to the physical port
 *                         width.
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port,
 *                                    or is not legal width for the port,
 *                                    or the port is unbuffered.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_transfer_width(port p, int transfer_width)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("settw res[%0], %1" :: "r" (p), "r" (transfer_width)) );
}

/** Sets a port to be unbuffered.
 *
 *  Configures a port into unbuffered mode. Note that before this is called, a
 *  a port needs to have its transfer width equal to the port width and be
 *  configured as a master port.
 *
 *  \param p    The port to set as unbuffered
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_unbuffered(port p)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("setc res[%0], 0x2007" :: "r" (p)) );
}

/** Sets a port to be buffered.
 *
 *  Configures a port into buffered mode where it can automatically serialise or
 *  deserialise data.
 *
 *  \param p    The port to set as buffered
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_buffered(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_buffered(p) );
}

/** Set the clock controlling a port.
 *
 *  This function connects a port to a clock.
 *
 *  \param p      Port to set its clock
 *
 *  \param clk    Clock to attach the port to
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port, clock,
 *                                    or clock is running.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_clock(port p, clock clk)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_clock(p, clk) );
}

/** Set a port drive out the data value.
 *
 *  \param p      Port to change the mode of
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_mode_data_port(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_mode_data_port(p) );
}

/** Set a port drive out the ready signal of another port.
 *
 *  This function configures the port to drive the ready signal of another port
 *  instead of its own data values. The source of the ready input is configured
 *  using the port_set_ready_input() function.
 *
 *  \param p      Port to change the mode of
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_mode_ready_port(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_mode_ready_port(p) );
}

/** Set a port drive out the clock signal.
 *
 *  This function configures the port to drive the clock signal instead of its
 *  own data values. The clock signal that is driven out is configured using the
 *  port_set_clock() function.
 *
 *  \param p      Port to change the mode of
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_mode_clock_port(port p)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("setc res[%0], 0x500f" :: "r" (p)) );
}

/** Set the ready input of a port.
 *
 *  This function configures the ready input of one port to be driven by the
 *  ready output of another port. The port then must be configured using
 *  port_set_mode_ready_port() to get it to drive out this ready signal.
 *
 *  \param p             Port to change the ready input of. This must be a 1-bit
 *                       port or this function will trap.
 *
 *  \param ready_source  The port whose ready signal is being used
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port,
 *                                    or p not a one bit port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_ready_src(port p, port ready_source)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_ready_src(p, ready_source) );
}

/** Set the port to invert its data.
 *
 *  This function configures a port to invert the data on the pin. This can be
 *  reverted by calling port_set_no_invert().
 *
 *  \param p   Port to set its data to be inverted. This must be a 1-bit port
 *             or a trap will be raised.
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_invert(port p)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("setc res[%0], 0x600f" :: "r" (p)) );
}

/** Set the port to not invert its data.
 *
 *  This function configures a port to not invert the data on the pin.
 *
 *  \param p   Port to set the data to not be inverted. This must be a 1-bit port
 *             or a trap will be raised.
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_no_invert(port p)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("setc res[%0], 0x6007" :: "r" (p)) );
}

/** Set the port to sample on the falling edge.
 *
 *  The default is for a port to sample data on the rising edge of the clock.
 *  This function changes the port to sample on the falling edge instead.
 *  This change can be reverted by calling port_set_no_sample_delay().
 *
 *  \param p   Port to change to sample on the falling edge.
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_sample_delay(port p)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("setc res[%0], 0x400f" :: "r" (p)) );
}

/** Set the port to sample on the rising edge.
 *
 *  This function restores a port to sampling data on the rising edge of the clock.
 *
 *  \param p   Port to restore to sampling on the rising edge
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_no_sample_delay(port p)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("setc res[%0], 0x4007" :: "r" (p)) );
}

/** Set the port to master mode (default state).
 *
 *  This function configures a port to be a master. This is only relevant when
 *  using ready signals (port_set_ready_strobed() / port_set_ready_handshake()).
 *
 *  It is highly recommended to use the ``port_configure_*`` functions to put a
 *  port into its desired mode as the order of operations is critical.
 *
 *  \param p   Port to set as a master
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_master(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_master(p) );
}

/** Set the port to slave mode.
 *
 *  This function configures a port to be a master. This is only relevant when
 *  using a ready strobe (port_set_ready_strobed())
 *
 *  *Note*: the port must be set to use a ready strobe, otherwise this function
 *          will raise an exception.
 *
 *  It is highly recommended to use the ``port_configure_*`` functions to put a
 *  port into its desired mode as the order of operations is critical.
 *
 *  \param p   Port to set as a slave
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_slave(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_slave(p) );
}

/** Set the port to use no ready signals (default state).
 *
 *  This function changes a port to not use ready signals. A port can be configured
 *  to use strobes or handshaking signals using port_set_ready_strobed() or
 *  port_set_ready_handshake().
 *
 *  *Note*: the port must be a ``master`` port otherwise this function will raise
 *          an exception.
 *
 *  It is highly recommended to use the ``port_configure_*`` functions to put a
 *  port into its desired mode as the order of operations is critical.
 *
 *  \param p   Port to change to not use ready signals
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_no_ready(port p)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("setc res[%0], 0x3007" :: "r" (p)) );
}

/** Set the port to use a single strobe.
 *
 *  This function changes a port to not use ready signals. A port can be configured
 *  to use strobes or handshaking signals using port_set_ready_strobed() or
 *  port_set_ready_handshake().
 *
 *  *Note*: the port must be a buffered port otherwise this function will raise
 *          an exception.
 *
 *  It is highly recommended to use the ``port_configure_*`` functions to put a
 *  port into its desired mode as the order of operations is critical.
 *
 *  \param p   Port to change to not use ready signals
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_ready_strobed(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_ready_strobed(p) );
}

/** Set the port to be fully handshaken.
 *
 *  This function changes a port to use both a ready input and drive a ready
 *  output in order to control when data is sampled or written.
 *
 *  *Note*: the port must be a master buffered port otherwise this function will
 *          raise an exception.
 *
 *  It is highly recommended to use the ``port_configure_*`` functions to put a
 *  port into its desired mode as the order of operations is critical.
 *
 *  \param p   Port to change to not use ready signals
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_ready_handshake(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_set_ready_handshake(p) );
}

/** Outputs a value onto a port.
 *
 *  In the case of an unbuffered port, the value will be driven on the pins on
 *  the next clock cycle. In the case of a buffered port, the data will be
 *  stored in the buffer, and be serialised onto the output pins.
 *
 *  \param p      Port to output to
 *
 *  \param data   Value to output
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_output(port p, int data)
{
  RETURN_EXCEPTION_OR_ERROR( _port_output(p, data) );
}

/** Outputs a value onto a port at a specified port counter value.
 *
 *  In the case of an unbuffered port, the value will be driven on the pins when
 *  on the clock cycle that moves the port counter to the specified time. In
 *  the case of a buffered port, the data will be stored in the buffer, and
 *  be serialised onto the output pins at the point that the time is reached.
 *
 *  \param p      Port to output to
 *
 *  \param data   Value to output
 *
 *  \param t      The port counter value at which the output will occur
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_output_at_time(port p, int16_t t, int data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setpt res[%0], %1" :: "r" (p), "r" (t)); \
                                asm volatile("out res[%0], %1" :: "r" (p), "r" (data)); \
                              } while (0) );
}

/** Outputs a value onto a port and shifts the output data.
 *
 *  In the case of an unbuffered port, the value will be driven on the pins on
 *  the next clock cycle. In the case of a buffered port, the data will be stored
 *  in the buffer, and be serialised onto the output pins.
 *
 *  \param p      Port to output to
 *
 *  \param data   data is shifted right by the transfer width of the port,
 *                with the bits shifting out onto the port.
 *                The remaining shifted bits are returned in data.
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *data argument.
 */
inline unsigned port_output_shift_right(port p, int *data)
{
  /* The data is a read-write operand */
  RETURN_EXCEPTION_OR_ERROR( asm volatile("outshr res[%0], %1" : "+r" (*data) : "r" (p)) );
}

/** Outputs a value onto a port at a specified time and shifts the output data.
 *
 *  In the case of an unbuffered port, the value will be driven on the pins when
 *  on the clock cycle that moves the port counter to the specified time. In
 *  the case of a buffered port, the data will be stored in the buffer, and
 *  be serialised onto the output pins at the point that the time is reached.
 *
 *  \param p      Port to output to
 *  \param t      The time of the output
 *  \param data   data is shifted right by the transfer width of the port,
 *                with the bits shifting out onto the port.
 *                The remaining shifted bits are returned in data.
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *data argument.
 */
inline unsigned port_output_shift_right_at_time(port p, int16_t t, int *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setpt res[%0], %1" :: "r" (p), "r" (t)); \
                                /* The data is a read-write operand */ \
                                asm volatile("outshr res[%0], %1" : "+r" (*data) : "r" (p)); \
                              } while (0) );
}

/** Peek at the value on a a port.
 *
 *  Peeking a port returns the current value on the pins of a port, regardless
 *  of whether the port is an output or input and without affecting its direciton.
 *
 *  \param p    Port to be peeked
 *  \param data The current value on the pins
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_LOAD_STORE         invalid *data argument.
 */
inline unsigned port_peek(port p, int *data)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("peek %0, res[%1]" : "=r" (*data): "r" (p)) );
}

/** Input a value from a port.
 *
 *  In the case of an unbuffered port, the data will be whatever is on the input
 *  pins. In the case of a buffered port, this function will wait until the buffer
 *  is filled up with deserialised data.
 *
 *  \param p    Port to input from
 *  \param data The inputted data
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *data argument.
 */
inline unsigned port_input(port p, int *data)
{
  RETURN_EXCEPTION_OR_ERROR( *data = _port_input(p) );
}

/** Input data from a port when its pins match a specific value.
 *
 *  In the case of an unbuffered port, the data inputted
 *  be identical to the value. In the case of a buffered port, an input
 *  will wait until the value appears on the pins and then return that
 *  value and some previous values that have been deserialised.
 *
 *  \param p      Port to input from
 *  \param value  The value to match against the pins
 *  \param data   The inputted data
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *data argument.
 */
inline unsigned port_input_when_pinseq(port p, int value, int *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setd res[%0], %1" :: "r" (p), "r" (value)); \
                                asm volatile("setc res[%0], 0x11" :: "r" (p)); \
                                asm volatile("in %0, res[%1]" : "=r" (*data): "r" (p)); \
                              } while (0) );
}

/** Input data from a port when its pins do not
 *  match a specific value.
 *
 *  In the case of an unbuffered port, the inputted data
 *  will be the non-matching pin values. In the case of a buffered port,
 *  this macro will wait until a non matching value appears on the pins, and
 *  then return that value and previous values that have been deserialised.
 *
 *  \param p      Port to input from
 *  \param value  The value to match against the pins
 *  \param data   The inputted data
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *data argument.
 */
inline unsigned port_input_when_pinsneq(port p, int value, int *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setd res[%0], %1" :: "r" (p), "r" (value)); \
                                asm volatile("setc res[%0], 0x19" :: "r" (p)); \
                                asm volatile("in %0, res[%1]" : "=r" (*data): "r" (p)); \
                              } while (0) );
}

/** Input data from a port when its counter is at a specific time.
 *
 *  In the case of an unbuffered port, the data will be inputted when the
 *  counter reaches time ``t``. In the case of a buffered port, an input
 *  will wait until the given time and then will start capturing data,
 *  returning a value when the buffer is full.
 *
 *  \param p      Port to input from
 *  \param t      The time to do input
 *  \param data   The inputted data
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *data argument.
 */
inline unsigned port_input_at_time(port p, int16_t t, int *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setpt res[%0], %1" :: "r" (p), "r" (t)); \
                                asm volatile("in %0, res[%1]" : "=r" (*data): "r" (p)); \
                              } while (0) );
}

/** Input a value from a port and shift the data.
 *
 *  In the case of an unbuffered port, the data will be whatever is on the input
 *  pins. In the case of a buffered port, this function will wait until the
 *  buffer is filled up with deserialised data.
 *
 *  \param p      Port to input from
 *
 *  \param data   The input data shifted right by the transfer width
 *                of the port
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *data argument.
 */
inline unsigned port_input_shift_right(port p, int *data)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("inshr %0, res[%1]" : "=r" (*data) : "r" (p)) );
}

/** Input data from a port when its pins match a specific value  and shift the data.
 *
 *  In the case of an unbuffered port, the data inputted
 *  be identical to the value. In the case of a buffered port, an input
 *  will wait until the value appears on the pins and then return that
 *  value and some previous values that have been deserialised.
 *
 *  \param p      Port to input from
 *
 *  \param value  The value to match against the pins
 *
 *  \param data  The input data shifted right by the transfer width
 *                of the port
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *data argument.
 */
inline unsigned port_input_shift_right_when_pinseq(port p, int value, int *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setd res[%0], %1" :: "r" (p), "r" (value)); \
                                asm volatile("setc res[%0], 0x11" :: "r" (p)); \
                                asm volatile("inshr %0, res[%1]" : "=r" (*data) : "r" (p)); \
                              } while (0) );
}

/** Input data from a port when its pins do not
 *  match a specific value  and shift the data.
 *
 *  In the case of an unbuffered port, the inputted data
 *  will be the non-matching pin values. In the case of a buffered port,
 *  this macro will wait until a non matching value appears on the pins, and
 *  then return that value and previous values that have been deserialised.
 *
 *  \param p      Port to input from
 *
 *  \param value  The value to match against the pins
 *
 *  \param data   The input data shifted right by the transfer width
 *                of the port
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *data argument.
 */
inline unsigned port_input_shift_right_when_pinsneq(port p, int value, int *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setd res[%0], %1" :: "r" (p), "r" (value)); \
                                asm volatile("setc res[%0], 0x19" :: "r" (p)); \
                                asm volatile("inshr %0, res[%1]" : "=r" (*data) : "r" (p)); \
                              } while (0) );
}

/** Input data from a port when its counter is at a specific time  and shift the data.
 *
 *  In the case of an unbuffered port, the data will be inputted when the
 *  counter reaches time ``t``. In the case of a buffered port, an input
 *  will wait until the given time and then will start capturing data,
 *  returning a value when the buffer is full.
 *
 *  \param p      Port to input from
 *
 *  \param t      The time to do input
 *
 *  \param data   The input data shifted right by the transfer width
 *                of the port
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *data argument.
 */
inline unsigned port_input_shift_right_at_time(port p, int16_t t, int *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("setpt res[%0], %1" :: "r" (p), "r" (t)); \
                                asm volatile("setc res[%0], 0x11" :: "r" (p)); \
                                asm volatile("inshr %0, res[%1]" : "=r" (*data) : "r" (p)); \
                              } while (0) );
}

/** Get the timestamp of the last operation on a port.
 *
 *  This function gets the timestamp of the last input or output operation
 *  on a port.
 *
 *  \param p    The port to get the timestamp from
 *
 *  \param data The timestamp of the last operation
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *ts argument.
 */
inline unsigned port_get_timestamp(port p, int16_t *ts)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("getts %0, res[%1]" : "=r" (*ts) : "r" (p)) );
}

/** Type representing port conditions.
 *
 */
typedef enum port_condition {
  PORT_COND_FULL    =  0x1, //< Condition that holds when port contains data ready to be input
  PORT_COND_PINSEQ  = 0x11, //< Condition that holds when the pins of the port are equal to the port conditional data
  PORT_COND_PINSNEQ = 0x19  //< Condition that holds when the pins of the port are equal to the port conditional data
} port_condition;

/** Set the condition on a port.
 *
 *  This sets the condition on a port. This condition will apply to all
 *  future inputs or outputs on
 *  a buffered port until the condition is set again.
 *  On a non-buffered port the condition will only hold for the next input or
 *  output after which the condition will be reset.
 *
 *  \param p  The port to set the condition on
 *
 *  \param c  The condition
 *
 *  \sa port_set_conditional_data
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_condition(port p, port_condition c)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("setc res[%0], %1" :: "r" (p), "r" (c)) );
}

/** Set the conditional data on a port.
 *
 *  This sets the conditional data on a port.
 *  This is used in conjunction with port_set_condition().
 *
 *  \param p  The port to set the conditional data on
 *
 *  \param d  The conditional data
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_set_conditional_data(port p, int d)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("setd res[%0], %1" :: "r" (p), "r" (d)) );
}

/** Clear the condition on a port.
 *
 *  This function clears any condition on the port so the next input or
 *  output will happen unconditionally. This function does not clear
 *  any time condition on the port.
 *
 *  \param p  The port to clear the condition on
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_clear_condition(port p)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("setc res[%0], 0x1" :: "r" (p)) );
}

/** Set the time condition on a port
 *
 *  This function sets the time condition on the next input or output on
 *  a port.
 *
 *  \param p      The port to set the condition on
 *
 *  \param count  The port counter value
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the port.
 */
inline unsigned port_set_time_condition(port p, int16_t count)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("setpt res[%0], %0" :: "r" (p), "r" (count)) );
}

/** Clear the time condition on a port.
 *
 *  This function clears any time condition on the port so the next input or
 *  output will happen unconditionally. This function does not clear
 *  any non-time condition on the port.
 *
 *  \param p the port to clear the time condition on
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_clear_time_condition(port p)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("clrpt res[%0]" :: "r" (p)) );
}

/** Clears the buffer used by a port.
 *
 *  Any data sampled by the port which has not been input by the processor is
 *  discarded. Any data output by the processor which has not been driven by the
 *  port is discarded. If the port is in the process of serialising output, it
 *  is interrupted immediately. If a pending output would have caused a change
 *  in direction of the port then that change of direction does not take place.
 *  If the port is driving a value on its pins when this function is called then
 *  it continues to drive the value until an output statement changes the value
 *  driven.
 *
 *  \param p  The port whose buffer is to be cleared
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 */
inline unsigned port_clear_buffer(port p)
{
  RETURN_EXCEPTION_OR_ERROR( _port_clear_buffer(p) );
}

/** Ends the current input on a buffered port.
 *
 *  The number of bits sampled by the port but not yet input by the processor is
 *  returned. This count includes both data in the transfer register and data in
 *  the shift register used for deserialisation.
 *  Subsequent inputs on the port return transfer-width bits of data
 *  until there is less than one transfer-width bits of data remaining.
 *  Any remaining data can be read with one further input, which
 *  returns transfer-width bits of data with the remaining buffered data
 *  in the most significant bits of this value.
 *
 *  \param p     The port to end the current input on
 *  \param num  The number of bits of data remaining
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *num argument.
 */
inline unsigned port_endin(port p, int *num)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("endin %0, res[%1]" : "=r" (*num) : "r" (p)) );
}

/** Force an input on a buffered port.
 *
 *  The function will perform an input on a buffered port even if the buffer is
 *  only partially full.
 *
 *  \param p    The port to do the input on
 *  \param num  A variable to be filled with number of bits inputted
 *  \param data The inputted data
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not a valid port.
 *  \exception  ET_RESOURCE_DEP       another core is actively changing the port.
 *  \exception  ET_LOAD_STORE         invalid *num or *data argument.
 */
inline unsigned port_force_input(port p, int *num, int *data)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                asm volatile("endin %0, res[%1]" : "=r" (*num) : "r" (p)); \
                                asm volatile("in %0, res[%1]" : "=r" (*data): "r" (p)); \
                              } while (0) );
}

#endif // __XC__

#endif // __xcore_c_port_h__
