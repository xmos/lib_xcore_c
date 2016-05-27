// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "port.h"

extern void port_set_transfer_width(port p, int transfer_width);
extern port port_enable(int port_id);
extern port port_enable_buffered(int port_id, int transfer_width);
extern void port_disable(port p);
extern void port_set_transfer_width(port p, int transfer_width);
extern void port_set_unbuffered(port p);
extern void port_set_buffered(port p);
extern void port_set_clock(port p, clock clk);
extern void port_set_mode_data_port(port p);
extern void port_set_mode_ready_port(port p);
extern void port_set_mode_clock_port(port p);
extern void port_set_ready_src(port p, port ready_source);
extern void port_set_invert(port p);
extern void port_set_no_invert(port p);
extern void port_set_sample_delay(port p);
extern void port_set_no_sample_delay(port p);
extern void port_set_master(port p);
extern void port_set_slave(port p);
extern void port_set_no_ready(port p);
extern void port_set_ready_strobed(port p);
extern void port_set_ready_handshake(port p);
extern void port_output(port p, int data);
extern void port_output_at_time(port p, int data, int16_t t);
extern int port_output_shift_right(port p, int data);
extern int port_output_shift_right_at_time(port p, int data, int16_t t);
extern int port_peek(port p);
extern int port_input(port p);
extern int port_input_when_pinseq(port p, int value);
extern int port_input_when_pinsneq(port p, int value);
extern int port_input_at_time(port p, int16_t t);
extern int port_input_shift_right(port p);
extern int port_input_shift_right_when_pinseq(port p, int value);
extern int port_input_shift_right_when_pinsneq(port p, int value);
extern int port_input_shift_right_at_time(port p, int16_t t);
extern int16_t port_get_timestamp(port p);
extern void port_set_condition(port p, port_condition c);
extern void port_set_conditional_data(port p, int d);
extern void port_clear_condition(port p);
extern void port_set_time_condition(port p, int16_t count);
extern void port_clear_time_condition(port p);
extern void port_clear_buffer(port p);
extern int port_endin(port p);
extern int port_force_input(port p, int *n);
