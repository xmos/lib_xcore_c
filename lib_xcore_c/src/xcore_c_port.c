// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_port.h"

extern unsigned port_alloc(port *p, int port_id);
extern unsigned port_alloc_buffered(port *p, int port_id, int transfer_width);
extern unsigned port_free(port *p);
extern unsigned port_set_transfer_width(port p, int transfer_width);
extern unsigned port_set_unbuffered(port p);
extern unsigned port_set_buffered(port p);
extern unsigned port_set_clock(port p, clock clk);
extern unsigned port_set_mode_data_port(port p);
extern unsigned port_set_mode_ready_port(port p);
extern unsigned port_set_mode_clock_port(port p);
extern unsigned port_set_ready_src(port p, port ready_source);
extern unsigned port_set_invert(port p);
extern unsigned port_set_no_invert(port p);
extern unsigned port_set_sample_delay(port p);
extern unsigned port_set_no_sample_delay(port p);
extern unsigned port_set_master(port p);
extern unsigned port_set_slave(port p);
extern unsigned port_set_no_ready(port p);
extern unsigned port_set_ready_strobed(port p);
extern unsigned port_set_ready_handshake(port p);
extern unsigned port_output(port p, int data);
extern unsigned port_output_at_time(port p, int16_t t, int data);
extern unsigned port_output_shift_right(port p, int *data);
extern unsigned port_output_shift_right_at_time(port p, int16_t t, int *data);
extern unsigned port_peek(port p, int *data);
extern unsigned port_input(port p, int *data);
extern unsigned port_input_when_pinseq(port p, int value, int *data);
extern unsigned port_input_when_pinsneq(port p, int value, int *data);
extern unsigned port_input_at_time(port p, int16_t t, int *data);
extern unsigned port_input_shift_right(port p, int *data);
extern unsigned port_input_shift_right_when_pinseq(port p, int value, int *data);
extern unsigned port_input_shift_right_when_pinsneq(port p, int value, int *data);
extern unsigned port_input_shift_right_at_time(port p, int16_t t, int *data);
extern unsigned port_get_timestamp(port p, int16_t *ts);
extern unsigned port_set_condition(port p, port_condition c);
extern unsigned port_set_conditional_data(port p, int d);
extern unsigned port_clear_condition(port p);
extern unsigned port_set_time_condition(port p, int16_t count);
extern unsigned port_clear_time_condition(port p);
extern unsigned port_clear_buffer(port p);
extern unsigned port_endin(port p, int *num);
extern unsigned port_force_input(port p, int *num, int *data);
