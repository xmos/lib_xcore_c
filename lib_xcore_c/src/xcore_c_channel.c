// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_impl.h"
extern void _chanend_free(chanend c);
extern void _chanend_set_dest(chanend c, chanend dst);
extern void _s_chan_out_word(streaming_chanend c, int data);
extern void _s_chan_out_byte(streaming_chanend c, char data);
extern int _s_chan_in_word(streaming_chanend c);
extern char _s_chan_in_byte(streaming_chanend c);
extern void _s_chan_out_ct_end(streaming_chanend c);
extern void _s_chan_check_ct_end(streaming_chanend c);
extern void _t_chan_change_to_input(transacting_chanend *tc);
extern void _t_chan_change_to_output(transacting_chanend *tc);

#include "xcore_c_chanend.h"
extern chanend chanend_alloc(void);
extern unsigned chanend_free(chanend *c);
extern unsigned chanend_set_dest(chanend c, chanend dst);

#include "xcore_c_channel_streaming.h"
extern streaming_channel s_chan_alloc(void);
extern unsigned s_chan_free(streaming_channel *c);
extern unsigned s_chan_out_word(streaming_chanend c, int data);
extern unsigned s_chan_out_byte(streaming_chanend c, char data);
extern unsigned s_chan_out_buf_word(streaming_chanend c, int buf[], int n);
extern unsigned s_chan_out_buf_byte(streaming_chanend c, char buf[], int n);
extern unsigned s_chan_in_word(streaming_chanend c, int *data);
extern unsigned s_chan_in_byte(streaming_chanend c, char *data);
extern unsigned s_chan_in_buf_word(streaming_chanend c, int buf[], int n);
extern unsigned s_chan_in_buf_byte(streaming_chanend c, char buf[], int n);
extern unsigned s_chan_out_ct(streaming_chanend c, int ct);
extern unsigned s_chan_out_ct_end(streaming_chanend c);
extern unsigned s_chan_check_ct(streaming_chanend c, int ct);
extern unsigned s_chan_check_ct_end(streaming_chanend c);

#include "xcore_c_channel.h"
extern channel chan_alloc(void);
extern unsigned chan_free(channel *c);
extern unsigned chan_out_word(chanend c, int data);
extern unsigned chan_out_byte(chanend c, char data);
extern unsigned chan_out_buf_word(chanend c, int buf[], int n);
extern unsigned chan_out_buf_byte(chanend c, char buf[], int n);
extern unsigned  chan_in_word(chanend c, int *data);
extern unsigned chan_in_byte(chanend c, char *data);
extern unsigned chan_in_buf_word(chanend c, int buf[], int n);
extern unsigned chan_in_buf_byte(chanend c, char buf[], int n);

#include "xcore_c_channel_transaction.h"
extern unsigned chan_init_transaction_master(chanend *c, transacting_chanend *tc);
extern unsigned chan_init_transaction_slave(chanend *c, transacting_chanend *tc);
extern unsigned chan_complete_transaction(transacting_chanend *tc, chanend *c);
extern unsigned t_chan_out_word(transacting_chanend *c, int data);
extern unsigned t_chan_out_byte(transacting_chanend *c, char data);
extern unsigned t_chan_out_buf_word(transacting_chanend *c, int buf[], int n);
extern unsigned t_chan_out_buf_byte(transacting_chanend *c, char buf[], int n);
extern unsigned t_chan_in_word(transacting_chanend *c, int *data);
extern unsigned t_chan_in_byte(transacting_chanend *c, char *data);
extern unsigned t_chan_in_buf_word(transacting_chanend *c, int buf[], int n);
extern unsigned t_chan_in_buf_byte(transacting_chanend *c, char buf[], int n);
