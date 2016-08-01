// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_channel_h__
#define __xcore_c_channel_h__

#include "xcore_c_channel_streaming.h"

#if !defined(__XC__) || defined(__DOXYGEN__)

/** Helper type for passing around both ends of a channel.
*/
typedef streaming_channel channel;

/** Create a channel.
 *  The chan-ends must be accessed on the same tile.
 *
 *  \param c channel variable holding the two initialised chanends
 */
inline void chan_alloc(channel *c)
{
  s_chan_alloc(c);
}

/** Disconnect and deallocate a channel.
 *
 *  \param c  channel to free
 */
inline void chan_free(channel c)
{
  // Not implemented in terms of s_chan_free() as we are already synced.
  chanend_free(c.left);
  chanend_free(c.right);
}

#endif // __XC__

/** Output a word over a channel-end.
 *
 *  \param c    The channel-end
 *
 *  \param data The word to be output
 */
inline void chan_out_word(chanend c, int data)
{
  s_chan_out_ct_end(c);
  s_chan_check_ct_end(c);
  s_chan_out_word(c, data);
  s_chan_out_ct_end(c);
  s_chan_check_ct_end(c);
}

/** Output a byte over a channel-end.
 *
 *  \param c    The channel-end
 *
 *  \param data The byte to be output
 */
inline void chan_out_byte(chanend c, char data)
{
  s_chan_out_ct_end(c);
  s_chan_check_ct_end(c);
  s_chan_out_byte(c, data);
  s_chan_out_ct_end(c);
  s_chan_check_ct_end(c);
}

/** Output a block of data over a channel-end.
 *
 *  \param c    The streaming channel-end
 *
 *  \param buf  A pointer to the buffer containing the data to send
 *
 *  \param n    The number of words to send
 */
inline void chan_out_buf_word(chanend c, int buf[], int n)
{
  s_chan_out_ct_end(c);
  s_chan_check_ct_end(c);
  s_chan_out_buf_word(c, buf, n);
  s_chan_out_ct_end(c);
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
inline void chan_out_buf_byte(chanend c, char buf[], int n)
{
  s_chan_out_ct_end(c);
  s_chan_check_ct_end(c);
  s_chan_out_buf_byte(c, buf, n);
  s_chan_out_ct_end(c);
  s_chan_check_ct_end(c);
}

/** Input a word from a channel.
 *
 *  \param c    The channel-end
 *
 *  \returns    The inputted word
 */
inline int chan_in_word(chanend c)
{
  int data;
  s_chan_out_ct_end(c);
  s_chan_check_ct_end(c);
  data = s_chan_in_word(c);
  s_chan_out_ct_end(c);
  s_chan_check_ct_end(c);
  return data;
}

/** Input a byte from a channel.
 *
 *  \param c    The channel-end
 *
 *  \returns    The inputted byte
 */
inline char chan_in_byte(chanend c)
{
  char data;
  s_chan_out_ct_end(c);
  s_chan_check_ct_end(c);
  data = s_chan_in_byte(c);
  s_chan_out_ct_end(c);
  s_chan_check_ct_end(c);
  return data;
}

/** Input a block of data from a channel-end.
 *
 *  \param c    The channel-end
 *
 *  \param buf  A pointer to the memory region to fill
 *
 *  \param n    The number of words to receive
 */
inline void chan_in_buf_word(chanend c, int buf[], int n)
{
  s_chan_out_ct_end(c);
  s_chan_check_ct_end(c);
  s_chan_in_buf_word(c, buf, n);
  s_chan_out_ct_end(c);
  s_chan_check_ct_end(c);
}

/** Input a block of data from a channel-end.
 *
 *  \param c    The channel-end
 *
 *  \param buf  A pointer to the memory region to fill
 *
 *  \param n    The number of bytes to receive
 */
inline void chan_in_buf_byte(chanend c, char buf[], int n)
{
  s_chan_out_ct_end(c);
  s_chan_check_ct_end(c);
  s_chan_in_buf_byte(c, buf, n);
  s_chan_out_ct_end(c);
  s_chan_check_ct_end(c);
}

#endif // __xcore_c_channel_h__
