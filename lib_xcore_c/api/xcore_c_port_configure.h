// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_port_configure_h__
#define __xcore_c_port_configure_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include "xcore_c_port.h"
#include "xcore_c_clock.h"

/** Configure a port to be a clocked input port in handshake mode.
 *
 *  If the ready-in or ready-out ports are not 1-bit ports, an exception is raised.
 *  The ready-out port is asserted on the falling edge of the clock when the
 *  port's buffer is not full. The port samples its pins on its sampling edge
 *  when both the ready-in and ready-out ports are asserted.
 *
 *  By default the port's sampling edge is the rising edge of clock. This can be
 *  changed by the function port_set_sample_delay().
 *
 *  *Note*: A handshaken port must be buffered, so this function will also make
 *          the port buffered.
 *
 *  \param p         The port to configure
 *
 *  \param readyin   A 1-bit port to use for the ready-in signal
 *
 *  \param readyout  A 1-bit port to use for the ready-out signal
 *
 *  \param clk       The clock used to configure the port
 */
 inline void port_configure_in_handshake(port p, port readyin, port readyout,
                                         clock clk)
{
  port_set_mode_data_port(p);
  port_set_buffered(p);
  /* ensure port is in input mode. */
  port_input(p);

  clock_set_ready_src(clk, readyin);
  port_set_clock(p, clk);
  port_set_ready_handshake(p);
  port_clear_buffer(p);
  port_set_ready_src(readyout, p);
  port_set_mode_ready_port(readyout);
}

/** configures a port to be a clocked output port in handshake mode.
 *
 *  if the ready-in or ready-out ports are not 1-bit ports an exception is
 *  raised. the port drives the initial value on its pins until an
 *  output statement changes the value driven.
 *
 *  the ready-in port is read on the sampling edge of the port. outputs are driven
 *  on the next falling edge of the clock where the previous value read from the
 *  ready-in port was high.
 *
 *  on the falling edge of the clock the ready-out port is driven high
 *  if data is output on that edge, otherwise it is driven low.
 *
 *  by default the port's sampling edge is the rising edge of clock. this can be
 *  changed by the function port_set_sample_delay().
 *
 *  *note*: a handshaken port must be buffered, so this function will also make
 *          the port buffered.
 *
 *  \param p        the port to configure
 *
 *  \param readyin  a 1-bit port to use for the ready-in signal
 *
 *  \param readyout a 1-bit port to use for the ready-out signal
 *
 *  \param clk      the clock used to configure the port
 *
 *  \param initial  the initial value to output on the port
 */
inline void port_configure_out_handshake(port p, port readyin, port readyout,
                                         clock clk, unsigned initial)
{
  port_set_mode_data_port(p);
  port_set_buffered(p);
  /* ensure port is in output mode. */
  port_output(p, initial);

  clock_set_ready_src(clk, readyin);
  port_set_clock(p, clk);
  port_set_ready_handshake(p);
  port_clear_buffer(p);
  port_set_ready_src(readyout, p);
  port_set_mode_ready_port(readyout);
}

/** configures a port to be a clocked input port in strobed master mode.
 *
 *  if the ready-out port is not a 1-bit port, an exception is raised.
 *  the ready-out port is asserted on the falling edge of the clock when the
 *  port's buffer is not full. the port samples its pins on its sampling edge
 *  after the ready-out port is asserted.
 *
 *  by default the port's sampling edge is the rising edge of clock. this can be
 *  changed by the function set_port_sample_delay().
 *
 *  *note*: a strobed port must be buffered, so this function will also make the
 *          port buffered.
 *
 *  \param p         the port to configure
 *
 *  \param readyout  a 1-bit port to use for the ready-out signal
 *
 *  \param clk       the clock used to configure the port
 */
inline void port_configure_in_strobed_master(port p, port readyout,
                                             const clock clk)
{
  port_set_mode_data_port(p);
  port_set_buffered(p);
  /* ensure port is in input mode. */
  port_input(p);

  port_set_clock(p, clk);
  port_set_ready_strobed(p);
  port_set_master(p);
  port_clear_buffer(p);
  port_set_ready_src(readyout, p);
  port_set_mode_ready_port(readyout);
}

/** configures a port to be a clocked output port in strobed master mode.
 *
 *  if the ready-out port is not a 1-bit port, an exception is raised.
 *  the port drives the initial value on its pins until an
 *  output statement changes the value driven. outputs are driven on the next
 *  falling edge of the clock. on the falling edge of the clock the ready-out
 *  port is driven high if data is output on that edge, otherwise it is driven
 *  low.
 *
 *  *note*: a strobed port must be buffered, so this function will also make the
 *          port buffered.
 *
 *  \param p        the port to configure
 *
 *  \param readyout a 1-bit port to use for the ready-out signal
 *
 *  \param clk      the clock used to configure the port
 *
 *  \param initial  the initial value to output on the port
 */
inline void port_configure_out_strobed_master(port p, port readyout,
                                              const clock clk, unsigned initial)
{
  port_set_mode_data_port(p);
  port_set_buffered(p);
  /* ensure port is in output mode. */
  port_output(p, initial);

  port_set_clock(p, clk);
  port_set_ready_strobed(p);
  port_set_master(p);
  port_clear_buffer(p);
  port_set_ready_src(readyout, p);
  port_set_mode_ready_port(readyout);
}

/** configures a port to be a clocked input port in strobed slave mode.
 *
 *  if the ready-in port is not a 1-bit port, an exception is raised.
 *  the port samples its pins on its sampling edge when the ready-in signal is
 *  high. by default the port's sampling edge is the rising edge of clock. this
 *  can be changed by the function set_port_sample_delay().
 *
 *  *note*: a strobed port must be buffered, so this function will also make the
 *          port buffered.
 *
 *  \param p       the port to configure
 *
 *  \param readyin a 1-bit port to use for the ready-in signal
 *
 *  \param clk     the clock used to configure the port
 */
inline void port_configure_in_strobed_slave(port p, port readyin, clock clk)
{
  port_set_mode_data_port(p);
  port_set_buffered(p);
  /* ensure port is in input mode. */
  port_input(p);

  clock_set_ready_src(clk, readyin);
  port_set_clock(p, clk);
  port_set_ready_strobed(p);
  port_set_slave(p);
  port_clear_buffer(p);
}

/** configures a port to be a clocked output port in strobed slave mode.
 *
 *  if the ready-in port is not a 1-bit port, an exception is raised.
 *  the port drives the initial value on its pins until an
 *  output statement changes the value driven. the ready-in port is read on the
 *  port's sampling edge. outputs are driven on the next falling edge
 *  of the clock where the previous value read from the ready-in port is high.
 *  by default the port's sampling edge is the rising edge of clock. this
 *  can be changed by the function set_port_sample_delay().
 *
 *  *note*: a strobed port must be buffered, so this function will also make the
 *          port buffered.
 *
 *  \param p       the port to configure
 *
 *  \param readyin a 1-bit port to use for the ready-in signal
 *
 *  \param clk     the clock used to configure the port
 *
 *  \param initial the initial value to output on the port
 */
inline void port_configure_out_strobed_slave(port p, port readyin,
                                             clock clk, unsigned initial)
{
  port_set_mode_data_port(p);
  port_set_buffered(p);
  /* ensure port is in output mode. */
  port_output(p, initial);

  clock_set_ready_src(clk, readyin);
  port_set_clock(p, clk);
  port_set_ready_strobed(p);
  port_set_slave(p);
}

#endif // __xc__

#endif // __xcore_c_port_configure_h__
