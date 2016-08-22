// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_port_impl.h"
extern port _port_alloc(int port_id);
extern void _port_set_buffered(port p);
extern void _port_set_clock(port p, clock clk);
extern void _port_set_mode_data_port(port p);
extern void _port_set_mode_ready_port(port p);
extern void _port_set_ready_src(port p, port ready_source);
extern void _port_set_master(port p);
extern void _port_set_slave(port p);
extern void _port_set_ready_strobed(port p);
extern void _port_set_ready_handshake(port p);
extern void _port_output(port p, int data);
extern int _port_input(port p);
extern void _port_set_condition(port p, port_condition c);
extern void _port_set_conditional_data(port p, int d);
extern void _port_clear_buffer(port p);

#include "xcore_c_port.h"
extern xcore_c_error port_alloc(port *p, int port_id);
extern xcore_c_error port_alloc_buffered(port *p, int port_id, int transfer_width);
extern xcore_c_error port_free(port *p);
extern xcore_c_error port_set_transfer_width(port p, int transfer_width);
extern xcore_c_error port_set_unbuffered(port p);
extern xcore_c_error port_set_buffered(port p);
extern xcore_c_error port_set_clock(port p, clock clk);
extern xcore_c_error port_set_mode_data_port(port p);
extern xcore_c_error port_set_mode_ready_port(port p);
extern xcore_c_error port_set_mode_clock_port(port p);
extern xcore_c_error port_set_ready_src(port p, port ready_source);
extern xcore_c_error port_set_invert(port p);
extern xcore_c_error port_set_no_invert(port p);
extern xcore_c_error port_set_sample_delay(port p);
extern xcore_c_error port_set_no_sample_delay(port p);
extern xcore_c_error port_set_master(port p);
extern xcore_c_error port_set_slave(port p);
extern xcore_c_error port_set_no_ready(port p);
extern xcore_c_error port_set_ready_strobed(port p);
extern xcore_c_error port_set_ready_handshake(port p);
extern xcore_c_error port_output(port p, int data);
extern xcore_c_error port_output_at_time(port p, int16_t t, int data);
extern xcore_c_error port_output_shift_right(port p, int *data);
extern xcore_c_error port_output_shift_right_at_time(port p, int16_t t, int *data);
extern xcore_c_error port_peek(port p, int *data);
extern xcore_c_error port_input(port p, int *data);
extern xcore_c_error port_input_when_pinseq(port p, int value, int *data);
extern xcore_c_error port_input_when_pinsneq(port p, int value, int *data);
extern xcore_c_error port_input_at_time(port p, int16_t t, int *data);
extern xcore_c_error port_input_shift_right(port p, int *data);
extern xcore_c_error port_input_shift_right_when_pinseq(port p, int value, int *data);
extern xcore_c_error port_input_shift_right_when_pinsneq(port p, int value, int *data);
extern xcore_c_error port_input_shift_right_at_time(port p, int16_t t, int *data);
extern xcore_c_error port_get_timestamp(port p, int16_t *ts);
extern xcore_c_error port_set_condition(port p, port_condition c);
extern xcore_c_error port_set_conditional_data(port p, int d);
extern xcore_c_error port_change_condition(port p, port_condition cond, unsigned data);
extern xcore_c_error port_clear_condition(port p);
extern xcore_c_error port_set_time_condition(port p, int16_t count);
extern xcore_c_error port_clear_time_condition(port p);
extern xcore_c_error port_clear_buffer(port p);
extern xcore_c_error port_endin(port p, int *num);
extern xcore_c_error port_force_input(port p, int *num, int *data);
extern xcore_c_error port_setup_select(port p, unsigned enum_id);
extern xcore_c_error port_setup_select_callback(port p, void *data, select_callback func);
extern xcore_c_error port_setup_interrupt_callback(port p, void *data, interrupt_callback func);
extern xcore_c_error port_enable_trigger(port p);
extern xcore_c_error port_disable_trigger(port p);

#include "xcore_c_port_configure.h"
extern xcore_c_error port_configure_in_handshake(port p, port readyin, port readyout, clock clk);
extern xcore_c_error port_configure_out_handshake(port p, port readyin, port readyout, clock clk, unsigned initial);
extern xcore_c_error port_configure_in_strobed_master(port p, port readyout, const clock clk);
extern xcore_c_error port_configure_out_strobed_master(port p, port readyout, const clock clk, unsigned initial);
extern xcore_c_error port_configure_in_strobed_slave(port p, port readyin, clock clk);
extern xcore_c_error port_configure_out_strobed_slave(port p, port readyin, clock clk, unsigned initial);
