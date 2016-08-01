// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_channel_streaming_h__
#define __xcore_c_channel_streaming_h__

#include "xcore_c_chanend.h"

/** Type that denotes a stremaing channel-end. Streaming communication
 *  channels comprise two connected streaming channel-ends and can be
 *  created using s_chan_alloc(). Streaming channels must be deinitialised
 *  using s_chan_free().
 */
typedef chanend streaming_chanend;

#include "xcore_c_channel_internal.h"

#if !defined(__XC__) || defined(__DOXYGEN__)

/** Helper type for passing around both ends of a streaming channel.
*/
typedef struct streaming_channel {
  streaming_chanend left;
  streaming_chanend right;
} streaming_channel;

/** Create a streaming channel.
 * The chan-ends must be accessed on the same tile.
 *
 *  \param c streaming_channel variable holding the two initialised chanends
 */
inline void s_chan_alloc(streaming_channel *c)
{
  chanend_alloc(&c->left);
  chanend_alloc(&c->right);
  chanend_set_dest(c->left, c->right);
  chanend_set_dest(c->right, c->left);
}

/** Disconnect and deallocate a streaming channel.
 *
 *  \param c  streaming_channel to free
 */
inline void s_chan_free(streaming_channel c)
{
  s_chan_output_ct_end(c.left);
  s_chan_output_ct_end(c.right);
  s_chan_check_ct_end(c.left);
  s_chan_check_ct_end(c.right);
  chanend_free(c.left);
  chanend_free(c.right);
}

#endif // __XC__

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

#endif // __xcore_c_channel_streaming_h__
