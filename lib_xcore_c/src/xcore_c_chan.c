// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_chan_impl.h"
extern chanend _chanend_alloc(void);
extern void _chanend_free(chanend c);
extern void _chanend_set_dest(chanend c, chanend dst);
extern void _s_chan_out_word(streaming_chanend c, int data);
extern void _s_chan_out_byte(streaming_chanend c, char data);
extern int _s_chan_in_word(streaming_chanend c);
extern char _s_chan_in_byte(streaming_chanend c);
extern void _s_chan_out_ct(streaming_chanend c, int ct);
extern void _s_chan_out_ct_end(streaming_chanend c);
extern void _s_chan_check_ct(streaming_chanend c, int ct);
extern void _s_chan_check_ct_end(streaming_chanend c);
extern void _t_chan_change_to_input(transacting_chanend *tc);
extern void _t_chan_change_to_output(transacting_chanend *tc);

#include "xcore_c_chanend.h"
extern xcore_c_error chanend_alloc(chanend *c);
extern xcore_c_error chanend_free(chanend *c);
extern xcore_c_error chanend_set_dest(chanend c, chanend dst);
extern xcore_c_error chanend_setup_select(chanend c, unsigned enum_id);
extern xcore_c_error chanend_setup_select_callback(chanend c, void *data, select_callback func);
extern xcore_c_error chanend_setup_interrupt_callback(chanend c, void *data, interrupt_callback intrpt);
extern xcore_c_error chanend_enable_trigger(chanend c);
extern xcore_c_error chanend_disable_trigger(chanend c);

#include "xcore_c_channel_streaming.h"
extern xcore_c_error s_chan_alloc(streaming_channel *c);
extern xcore_c_error s_chan_free(streaming_channel *c);
extern xcore_c_error s_chan_out_word(streaming_chanend c, int data);
extern xcore_c_error s_chan_out_byte(streaming_chanend c, char data);
extern xcore_c_error s_chan_out_buf_word(streaming_chanend c, int buf[], int n);
extern xcore_c_error s_chan_out_buf_byte(streaming_chanend c, char buf[], int n);
extern xcore_c_error s_chan_in_word(streaming_chanend c, int *data);
extern xcore_c_error s_chan_in_byte(streaming_chanend c, char *data);
extern xcore_c_error s_chan_in_buf_word(streaming_chanend c, int buf[], int n);
extern xcore_c_error s_chan_in_buf_byte(streaming_chanend c, char buf[], int n);
extern xcore_c_error s_chan_out_ct(streaming_chanend c, int ct);
extern xcore_c_error s_chan_out_ct_end(streaming_chanend c);
extern xcore_c_error s_chan_check_ct(streaming_chanend c, int ct);
extern xcore_c_error s_chan_check_ct_end(streaming_chanend c);

#include "xcore_c_channel.h"
extern xcore_c_error chan_alloc(channel *c);
extern xcore_c_error chan_free(channel *c);
extern xcore_c_error chan_out_word(chanend c, int data);
extern xcore_c_error chan_out_byte(chanend c, char data);
extern xcore_c_error chan_out_buf_word(chanend c, int buf[], int n);
extern xcore_c_error chan_out_buf_byte(chanend c, char buf[], int n);
extern xcore_c_error chan_in_word(chanend c, int *data);
extern xcore_c_error chan_in_byte(chanend c, char *data);
extern xcore_c_error chan_in_buf_word(chanend c, int buf[], int n);
extern xcore_c_error chan_in_buf_byte(chanend c, char buf[], int n);

#include "xcore_c_channel_transaction.h"
extern xcore_c_error chan_init_transaction_master(chanend *c, transacting_chanend *tc);
extern xcore_c_error chan_init_transaction_slave(chanend *c, transacting_chanend *tc);
extern xcore_c_error chan_complete_transaction(transacting_chanend *tc, chanend *c);
extern xcore_c_error t_chan_out_word(transacting_chanend *c, int data);
extern xcore_c_error t_chan_out_byte(transacting_chanend *c, char data);
extern xcore_c_error t_chan_out_buf_word(transacting_chanend *c, int buf[], int n);
extern xcore_c_error t_chan_out_buf_byte(transacting_chanend *c, char buf[], int n);
extern xcore_c_error t_chan_in_word(transacting_chanend *c, int *data);
extern xcore_c_error t_chan_in_byte(transacting_chanend *c, char *data);
extern xcore_c_error t_chan_in_buf_word(transacting_chanend *c, int buf[], int n);
extern xcore_c_error t_chan_in_buf_byte(transacting_chanend *c, char buf[], int n);
