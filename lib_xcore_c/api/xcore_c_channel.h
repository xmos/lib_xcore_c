// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_channel_h__
#define __xcore_c_channel_h__

#include <xccompat.h>

/** Type that denotes a stremaing channel-end. Streaming communication
 *  channels comprise two connected streaming channel-ends and can be
 *  created using s_chan_alloc(). Streaming channels must be deinitialised
 *  using s_chan_free().
 */
typedef chanend streaming_chanend;

#include "xcore_c_channel_internal.h"

#if !defined(__XC__) || defined(__DOXYGEN__)
/** Allocate a single channel-end.
 *
 *  \param c   Channel-end
 */
inline void chanend_alloc(chanend *c)
{
  asm("getr %0, 2" : "=r" (*c));
}

/** Free a single channel-end.
 *
 *  \param c   Channel-end to be freed
 */
inline void chanend_free(chanend c)
{
  if (c) {
    asm volatile("freer res[%0]" :: "r" (c));
  }
}

/** Set the destination of a chanend
 *
 *  \param c     Channel-end
 *
 *  \param dst   Destination channel-end
 */
inline void chanend_set_dest(chanend c, chanend dst)
{
  asm("setd res[%0], %1" :: "r" (c), "r" (dst));
}

/** Create a channel between a pair of
 *  channel-ends. The channel-end variables must reside on the same core.
 *
 *  \param c1   Channel-end of one side
 *
 *  \param c2   Channel-end of the other side
 */
inline void chan_alloc(chanend *c1, chanend *c2)
{
  chanend_alloc(c1);
  chanend_alloc(c2);
  chanend_set_dest(*c1, *c2);
  chanend_set_dest(*c2, *c1);
}

/** Create a streaming channel between a pair of streaming
 *  channel-ends. The channel-end variables must reside on the same core.
 *
 *  \param c1   Channel-end of one side
 *
 *  \param c2   Channel-end of the other side
 */
inline void s_chan_alloc(streaming_chanend *c1, streaming_chanend *c2)
{
  chan_alloc(c1, c2);
}

/** Disconnect and deallocate a channel between a pair of channel-ends.
 *
 *  \param c1   Channel-end of one side
 *
 *  \param c2   Channel-end of the other side
 */
inline void chan_free(chanend c1, chanend c2)
{
  asm volatile("freer res[%0]" :: "r" (c1));
  asm volatile("freer res[%0]" :: "r" (c2));
}

/** Disconnect and deallocate a streaming channel between a
 *  pair of streaming channel-ends.
 *
 *  \param c1   Channel-end of one side
 *  \param c2   Channel-end of the other side
 */
inline void s_chan_free(streaming_chanend c1, streaming_chanend c2)
{
  s_chan_output_ct_end(c1);
  s_chan_output_ct_end(c2);
  s_chan_check_ct_end(c1);
  s_chan_check_ct_end(c1);
  asm volatile("freer res[%0]" :: "r" (c1));
  asm volatile("freer res[%0]" :: "r" (c2));
}
#endif

/** Output a control token onto a streaming channel-end.
 *
 *  \param c    The streaming channel-end
 *
 *  \param ct   Control token to be output. Legal control tokens that can be
 *              used are 0 or any value in the range 3..191 inclusive.
 */
inline void s_chan_output_ct(streaming_chanend c, int ct)
{
  asm volatile("outct res[%0], %1" :: "r" (c), "r" (ct));
}

/** Check that a specific control token is available on a streaming channel-end.
 *
 *  This function blocks until a token is available on the streaming channel. If
 *  the available token is a control token and has the value ``ct``, then the
 *  token is input and discarded. Otherwise an exception is raised.
 *
 *  \param c    The streaming channel-end
 *
 *  \param ct   Control token that is expected on the streaming channel
 */
inline void s_chan_check_ct(streaming_chanend c, int ct)
{
  asm volatile("chkct res[%0], %1" :: "r" (c), "r" (ct));
}

/** Output a word over a streaming channel-end.
 *
 *  \param c    The streaming channel-end
 *
 *  \param data The word to be output
 */
inline void s_chan_output_word(streaming_chanend c, int data)
{
  asm volatile("out res[%0], %1" :: "r" (c), "r" (data));
}

/** Output an byte over a streaming channel-end.
 *
 *  \param c    The streaming channel-end
 *
 *  \param data The byte to be output
 */
inline void s_chan_output_byte(streaming_chanend c, char data)
{
  asm volatile("outt res[%0], %1" :: "r" (c), "r" (data));
}

/** Output a block of data over a streaming channel-end.
 *
 *  \param c    The streaming channel-end
 *
 *  \param buf  A pointer to the buffer containing the data to send
 *
 *  \param n    The number of bytes to send
 */
inline void s_chan_output_block(streaming_chanend c, char buf[], int n)
{
  // Note we could do this more efficiently depending on the size of n
  // and the alignment of buf
  for (int i = 0; i < n; i++) {
    s_chan_output_byte(c, buf[i]);
  }
}

/** Input a word from a streaming channel-end.
 *
 *  \param c    The streaming channel-end
 *
 *  \returns    The inputted integer
 */
inline int s_chan_input_word(streaming_chanend c)
{
  int data;
  asm volatile("in %0, res[%1]" : "=r" (data): "r" (c));
  return data;
}

/** Input a byte from a streaming channel-end.
 *
 *  \param c    The streaming channel-end
 *
 *  \returns    The inputted byte
 */
inline char s_chan_input_byte(streaming_chanend c)
{
  char data;
  asm volatile("int %0, res[%1]" : "=r" (data): "r" (c));
  return data;
}

/** Input a block of data from a streaming channel-end.
 *
 *  \param c    The streaming channel-end
 *
 *  \param buf  A pointer to the memory region to fill
 *
 *  \param n    The number of bytes to receive
 */
inline void s_chan_input_block(streaming_chanend c, char buf[], int n)
{
  // Note we could do this more efficiently depending on the size of n
  // and the alignment of buf
  for (int i = 0; i < n; i++) {
    buf[i] = s_chan_input_byte(c);
  }
}

/** Output a word over a channel-end.
 *
 *  \param c    The channel-end
 *
 *  \param data The word to be output
 */
inline void chan_output_word(chanend c, int data)
{
  s_chan_output_ct_end(c);
  s_chan_check_ct_end(c);
  s_chan_output_word(c, data);
  s_chan_output_ct_end(c);
  s_chan_check_ct_end(c);
}

/** Output a byte over a channel-end.
 *
 *  \param c    The channel-end
 *
 *  \param data The byte to be output
 */
inline void chan_output_byte(chanend c, char data)
{
  s_chan_output_ct_end(c);
  s_chan_check_ct_end(c);
  s_chan_output_byte(c, data);
  s_chan_output_ct_end(c);
  s_chan_check_ct_end(c);
}

/** Output a block of data over a channel-end.
 *
 *  \param c    The streaming channel-end
 *
 *  \param buf  A pointer to the buffer containing the data to send
 *
 *  \param n    The number of bytes to send
 */
inline void chan_output_block(chanend c, char buf[], int n)
{
  s_chan_output_ct_end(c);
  s_chan_check_ct_end(c);
  s_chan_output_block(c, buf, n);
  s_chan_output_ct_end(c);
  s_chan_check_ct_end(c);
}

/** Input a word from a channel.
 *
 *  \param c    The channel-end
 *
 *  \returns    The inputted word
 */
inline int chan_input_word(chanend c)
{
  int data;
  s_chan_output_ct_end(c);
  s_chan_check_ct_end(c);
  data = s_chan_input_word(c);
  s_chan_output_ct_end(c);
  s_chan_check_ct_end(c);
  return data;
}

/** Input a byte from a channel.
 *
 *  \param c    The channel-end
 *
 *  \returns    The inputted byte
 */
inline int chan_input_byte(chanend c)
{
  int data;
  s_chan_output_ct_end(c);
  s_chan_check_ct_end(c);
  data = s_chan_input_word(c);
  s_chan_output_ct_end(c);
  s_chan_check_ct_end(c);
  return data;
}

/** Input a block of data from a channel-end.
 *
 *  \param c    The channel-end
 *
 *  \param buf  A pointer to the memory region to fill
 *
 *  \param n    The number of bytes to receive
 */
inline void chan_input_block(chanend c, char buf[], int n)
{
  s_chan_output_ct_end(c);
  s_chan_check_ct_end(c);
  s_chan_input_block(c, buf, n);
  s_chan_output_ct_end(c);
  s_chan_check_ct_end(c);
}

#include "xcore_c_channel_transaction.h"

#endif // __xcore_c_channel_h__
