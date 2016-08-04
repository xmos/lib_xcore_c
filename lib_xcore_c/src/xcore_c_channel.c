// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <xccompat.h>
#include "xcore_c_chanend.h"
#include "xcore_c_channel.h"
#include "xcore_c_channel_streaming.h"
#include "xcore_c_channel_transaction.h"

extern void chanend_alloc(chanend *c);
extern void chanend_free(chanend c);
extern void chanend_set_dest(chanend c, chanend dst);

extern void chan_alloc(channel *c);
extern void chan_free(channel c);
extern void chan_out_word(chanend c, int data);
extern void chan_out_byte(chanend c, char data);
extern void chan_out_buf_word(chanend c, int buf[], int n);
extern void chan_out_buf_byte(chanend c, char buf[], int n);
extern int  chan_in_word(chanend c);
extern char chan_in_byte(chanend c);
extern void chan_in_buf_word(chanend c, int buf[], int n);
extern void chan_in_buf_byte(chanend c, char buf[], int n);

extern void s_chan_alloc(streaming_channel *c);
extern void s_chan_free(streaming_channel c);
extern void s_chan_out_word(streaming_chanend c, int data);
extern void s_chan_out_byte(streaming_chanend c, char data);
extern void s_chan_out_buf_word(streaming_chanend c, int buf[], int n);
extern void s_chan_out_buf_byte(streaming_chanend c, char buf[], int n);
extern int  s_chan_in_word(streaming_chanend c);
extern char s_chan_in_byte(streaming_chanend c);
extern void s_chan_in_buf_word(streaming_chanend c, int buf[], int n);
extern void s_chan_in_buf_byte(streaming_chanend c, char buf[], int n);
extern void s_chan_out_ct(streaming_chanend c, int ct);
extern void s_chan_check_ct(streaming_chanend c, int ct);
extern void s_chan_out_ct_end(streaming_chanend c);
extern void s_chan_check_ct_end(streaming_chanend c);

extern transacting_chanend chan_init_transaction_master(chanend c);
extern transacting_chanend chan_init_transaction_slave(chanend c);
extern chanend chan_complete_transaction(REFERENCE_PARAM(transacting_chanend, c));
extern void t_chan_out_word(REFERENCE_PARAM(transacting_chanend, c), int data);
extern void t_chan_out_byte(REFERENCE_PARAM(transacting_chanend, c), char data);
extern void t_chan_out_buf_word(REFERENCE_PARAM(transacting_chanend, c), int buf[], int n);
extern void t_chan_out_buf_byte(REFERENCE_PARAM(transacting_chanend, c), char buf[], int n);
extern int t_chan_in_word(REFERENCE_PARAM(transacting_chanend, c));
extern char t_chan_in_byte(REFERENCE_PARAM(transacting_chanend, c));
extern void t_chan_in_buf_word(REFERENCE_PARAM(transacting_chanend, c), int buf[], int n);
extern void t_chan_in_buf_byte(REFERENCE_PARAM(transacting_chanend, c), char buf[], int n);

