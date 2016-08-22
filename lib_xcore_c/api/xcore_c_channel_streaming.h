// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_channel_streaming_h__
#define __xcore_c_channel_streaming_h__

#include "xcore_c_chanend.h"
#include "xcore_c_chan_impl.h"
#include "xcore_c_exception_impl.h"

#if !defined(__XC__) || defined(__DOXYGEN__)

/** Helper type for passing around both ends of a streaming channel.
*/
typedef struct streaming_channel {
  streaming_chanend left;
  streaming_chanend right;
} streaming_channel;

/** Allocate a streaming_channel.
 *
 *  This function allocates two hardware chan-ends and joins them.
 *  If there are not enough chan-ends available the function returns a
 *  streaming_channel with both fields set to 0.
 *  When the streaming_channel is no longer required, s_chan_free() must be
 *  called to deallocate it.
 *  N.B. The chan-ends must be accessed on the same tile.
 *
 *  \param c    streaming_channel variable holding the two initialised and
 *              joined chan-ends or 0s.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LOAD_STORE         invalid *c argument.
 */
inline xcore_c_error s_chan_alloc(streaming_channel *c)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                if ((c->left = _chanend_alloc())) {
                                  if ((c->right = _chanend_alloc())) {
                                    // exception safe calls to _chanend_set_dest()
                                    _chanend_set_dest(c->left, c->right);
                                    _chanend_set_dest(c->right, c->left);
                                  }
                                  else {
                                    _chanend_free(c->left);
                                    c->left = 0;
                                    c->right = 0;
                                  }
                                }
                                else {
                                  c->left = 0;
                                  c->right = 0;
                                }
                              } while (0) );
}

/** Deallocate a streaming_channel.
 *
 *  This function frees the two hardware chan-ends.
 *
 *  \param c    streaming_channel to free.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         a chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated channel,
 *                                    or an input/output is pending.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the channel.
 *  \exception  ET_LOAD_STORE         invalid *c argument.
 */
inline xcore_c_error s_chan_free(streaming_channel *c)
{
  RETURN_EXCEPTION_OR_ERROR(  do {
                                _s_chan_out_ct_end(c->left); \
                                _s_chan_out_ct_end(c->right); \
                                _s_chan_check_ct_end(c->left); \
                                _s_chan_check_ct_end(c->right); \
                                _chanend_free(c->left); \
                                c->left = 0; \
                                _chanend_free(c->right); \
                                c->right = 0; \
                              } while (0) );
}

#endif // __XC__

/** Output a word over a streaming_channel.
 *
 *  \param c    The streaming chan-end
 *
 *  \param data The word to be output
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline xcore_c_error s_chan_out_word(streaming_chanend c, int data)
{
  RETURN_EXCEPTION_OR_ERROR( _s_chan_out_word(c, data) );
}

/** Output an byte over a streaming_channel.
 *
 *  \param c    The streaming chan-end
 *
 *  \param data The byte to be output
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline xcore_c_error s_chan_out_byte(streaming_chanend c, char data)
{
  RETURN_EXCEPTION_OR_ERROR( _s_chan_out_byte(c, data) );
}

/** Output a block of data over a streaming_channel.
 *
 *  \param c    The streaming chan-end
 *
 *  \param buf  A pointer to the buffer containing the data to send
 *
 *  \param n    The number of words to send
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid buf[] argument.
 */
inline xcore_c_error s_chan_out_buf_word(streaming_chanend c, int buf[], int n)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                for (int i = 0; i < n; i++) { \
                                  _s_chan_out_word(c, buf[i]); \
                                } \
                              } while (0) );
}

/** Output a block of data over a streaming_channel.
 *
 *  \param c    The streaming chan-end
 *
 *  \param buf  A pointer to the buffer containing the data to send
 *
 *  \param n    The number of bytes to send
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid buf[] argument.
 */
inline xcore_c_error s_chan_out_buf_byte(streaming_chanend c, char buf[], int n)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                for (int i = 0; i < n; i++) { \
                                  _s_chan_out_byte(c, buf[i]); \
                                } \
                              } \
                              while (0) );
}

/** Input a word from a streaming_channel.
 *
 *  \param c    The streaming chan-end
 *
 *  \param data The inputted word.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or has pending control token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *data argument.
 */
inline xcore_c_error s_chan_in_word(streaming_chanend c, int *data)
{
  RETURN_EXCEPTION_OR_ERROR( *data = _s_chan_in_word(c) );
}

/** Input a byte from a streaming_channel.
 *
 *  \param c    The streaming chan-end
 *
 *  \param data The inputted byte
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or has pending control token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *data argument.
 */
inline xcore_c_error s_chan_in_byte(streaming_chanend c, char *data)
{
  RETURN_EXCEPTION_OR_ERROR( *data = _s_chan_in_byte(c) );
}

/** Input a block of data from a streaming_channel.
 *
 *  \param c    The streaming chan-end
 *
 *  \param buf  A pointer to the memory region to fill
 *
 *  \param n    The number of words to receive
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or has pending control token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid buf[] argument.
 */
inline xcore_c_error s_chan_in_buf_word(streaming_chanend c, int buf[], int n)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                for (int i = 0; i < n; i++) { \
                                  buf[i] = _s_chan_in_word(c); \
                                } \
                              } while (0) );
}

/** Input a block of data from a streaming_channel.
 *
 *  \param c    The streaming chan-end
 *
 *  \param buf  A pointer to the memory region to fill
 *
 *  \param n    The number of bytes to receive
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or has pending control token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid buf[] argument.
 */
inline xcore_c_error s_chan_in_buf_byte(streaming_chanend c, char buf[], int n)
{
  RETURN_EXCEPTION_OR_ERROR(  do { \
                                for (int i = 0; i < n; i++) { \
                                  buf[i] = _s_chan_in_byte(c); \
                                } \
                              } while (0) );
}

/** Output a control token onto a streaming_channel.
 *
 *  \param c    The streaming chan-end
 *
 *  \param ct   Control token to be output. Legal control tokens that can be
 *              used are 0 or any value in the range 3..191 inclusive.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set
 *                                    or token is reserverd.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline xcore_c_error s_chan_out_ct(streaming_chanend c, int ct)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("outct res[%0], %1" :: "r" (c), "r" (ct)) );
}

/** Output a CT_END control token onto a streaming_channel.
 *
 *  Outputting a CT_END control token informs the communication network and the
 *  other chan-end that the current transaction has completed. Thus freeing the
 *  communication network for other channels to use.
 *
 *  The streaming_channel remains allocated, awaiting another transaction or
 *  deallocation.
 *
 *  \param c    The streaming chan-end
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline xcore_c_error s_chan_out_ct_end(streaming_chanend c)
{
  return s_chan_out_ct(c, XS1_CT_END);
}

/** Check that a specific control token on a streaming_channel.
 *
 *  This function blocks until a token is available on the streaming_channel. If
 *  the available token is a control token and has the value ``ct``, then the
 *  token is input and discarded. Otherwise an exception is raised.
 *
 *  \param c    The streaming chan-end
 *
 *  \param ct   Control token that is expected on the streaming_channel.
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or does not contain expected token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline xcore_c_error s_chan_check_ct(streaming_chanend c, int ct)
{
  RETURN_EXCEPTION_OR_ERROR( asm volatile("chkct res[%0], %1" :: "r" (c), "r" (ct)) );
}

/** Check for a CT_END token on a streaming_channel.
 *
 *  This function blocks until a token is available on the streaming_channel. If
 *  the available token is a CT_END token the token is input and discarded.
 *  Otherwise an exception is raised.
 *
 *  \param c    The streaming chan-end
 *
 *  \return     error_none (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or does not contain CT_END token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 */
inline xcore_c_error s_chan_check_ct_end(streaming_chanend c)
{
  return s_chan_check_ct(c, XS1_CT_END);
}

#endif // __xcore_c_channel_streaming_h__
