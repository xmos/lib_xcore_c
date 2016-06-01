// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_channel_internal_h__
#define __xcore_c_channel_internal_h__

/** Send an *end* control token over a streaming channel.
 *
 *  The *end* control token closes the streaming data connection.
 *
 * \param c   Streaming channel-end on which to send the *end* control token
 */
inline void s_chan_output_ct_end(streaming_chanend c)
{
  asm("outct res[%0],1"::"r"(c));
}

/** Receive an *end* control token on a streaming channel.
 *
 *  This function will consume an *end* control token from a streaming
 *  channel-end. It will trap if the data on the channel-end is not an *end*
 *  control token.
 *
 * \param c   Streaming channel-end on which to receive the *end* control token
 */
inline void s_chan_check_ct_end(streaming_chanend c)
{
  asm("chkct res[%0],1"::"r"(c));
}

#endif // __xcore_c_channel_internal_h__
