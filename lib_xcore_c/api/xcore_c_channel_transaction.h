// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_channel_transaction_h__
#define __xcore_c_channel_transaction_h__

#if !defined(__XC__) || defined(__DOXYGEN__)

#include "xcore_c_channel_streaming.h"
//#include "trycatch.h"

/** Start a transaction (master).
 *
 *  This initiates a *transaction* on a channel.
 *
 *  A transacting_chanend is used to temporarily open a transaction
 *  route through a channel.
 *  During the transaction, you can use transaction channel operations for
 *  increased efficiency.
 *  You can create a transacting chanend from a normal *chanend* using
 *  chan_init_transaction_master() and chan_init_transaction_slave().
 *
 *  This called must be matched by a call to chan_init_transaction_slave()
 *  on the other end of the channel.
 *
 *  A transaction must be closed with chan_complete_transaction().
 *
 *  \param c    chan-end to initialize the transaction on. chanend is invalidated
 *
 *  \param tc   the intialized master transacting_chanend
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *c or *tc argument.
 */
inline unsigned chan_init_transaction_master(chanend *c, transacting_chanend *tc)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                _s_chan_out_ct_end(*c); \
                                tc->last_out = 0; \
                                tc->c = *c; \
                                *c = 0; \
                              } while (0) );
}

/** Start a transaction (slave).
 *
 *  This initiates a *transaction* on a channel.
 *
 *  This called must be matched by a call to chan_init_transaction_master()
 *  on the other end of the channel.
 *
 *  A transaction must be closed with chan_complete_transaction().
 *
 *  The original *chanend* must not be used until the transaction is closed.
 *
 *  \param c    chan-end to initialize the transaction on. chanend is invalidated
 *
 *  \param tc   the intialized slave transacting_chanend
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or does not contain CT_END token.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *c or *tc argument.
 */
inline unsigned chan_init_transaction_slave(chanend *c, transacting_chanend *tc)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                _s_chan_check_ct_end(*c); \
                                tc->last_out = 1; \
                                tc->c = *c; \
                                *c = 0; \
                              } while (0) );
}

/** Complete a transaction.
 *
 *  This function completes a transaction. After this call the route between the
 *  two ends of the channel is freed allowing other channels to use the
 *  communication network.
 *
 *  Whilst the transacting_chanend is now invalid, the channel remains allocated,
 *  awaiting another transaction or deallocation.
 *
 *  This call must be accompanied by a call to chan_complete_transaction() on
 *  the other side of the channel.
 *
 *  \param tc  Transacting chan-end to close. transacting_chanend is invalidated
 *
 *  \param c   The original chan-end.
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *c or *tc argument.
 */
inline unsigned chan_complete_transaction(transacting_chanend *tc, chanend *c)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                if (tc->last_out) { \
                                  _s_chan_out_ct_end(tc->c); \
                                  _s_chan_check_ct_end(tc->c); \
                                } else { \
                                  _s_chan_check_ct_end(tc->c); \
                                  _s_chan_out_ct_end(tc->c); \
                                } \
                                *c = tc->c; \
                                tc->c = 0; \
                              } while (0) );
}

/** Output a word over a transacting chan-end.
 *
 *  \param tc   Transacting chan-end
 *
 *  \param data Word to be output
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *tc argument.
 */
inline unsigned t_chan_out_word(transacting_chanend *tc, int data)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                _t_chan_change_to_output(tc); \
                                _s_chan_out_word(tc->c, data); \
                              } while (0) );
}

/** Output an byte over a transacting chan-end.
 *
 *  \param tc   Transacting chan-end
 *
 *  \param data Byte to be output
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *tc argument.
 */
inline unsigned t_chan_out_byte(transacting_chanend *tc, char data)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                _t_chan_change_to_output(tc); \
                                _s_chan_out_byte(tc->c, data); \
                              } while (0) );
}

/** Output a block of data over a transacting chan-end.
 *
 *  \param c    Transacting chan-end
 *
 *  \param buf  Pointer to the buffer containing the data to send
 *
 *  \param n    Number of words to send
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *tc or buf[] argument.
 */
inline unsigned t_chan_out_buf_word(transacting_chanend *tc, int buf[], int n)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                _t_chan_change_to_output(tc); \
                                for (int i = 0; i < n; i++) { \
                                  _s_chan_out_word(tc->c, buf[i]); \
                                } \
                              } while (0) );
}

/** Output a block of data over a transacting chan-end.
 *
 *  \param c    Transacting chan-end
 *
 *  \param buf  Pointer to the buffer containing the data to send
 *
 *  \param n    Number of bytes to send
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *tc or buf[] argument.
 */
inline unsigned t_chan_out_buf_byte(transacting_chanend *tc, char buf[], int n)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                _t_chan_change_to_output(tc); \
                                for (int i = 0; i < n; i++) { \
                                  _s_chan_out_byte(tc->c, buf[i]); \
                                } \
                              } while (0) );
}

/** Input a word from a transacting chan-end.
 *
 *  \param tc   Transacting chan-end
 *
 *  \param data Inputted integer
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *tc or *data argument.
 */
inline unsigned t_chan_in_word(transacting_chanend *tc, int *data)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                _t_chan_change_to_input(tc); \
                                *data = _s_chan_in_word(tc->c); \
                              } while (0) );
}

/** Input a byte from a transacting chan-end.
 *
 *  \param tc   Transacting chan-end
 *
 *  \param data Inputted byte
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *tc or *data argument.
 */
inline unsigned t_chan_in_byte(transacting_chanend *tc, char *data)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                _t_chan_change_to_input(tc); \
                                *data = _s_chan_in_byte(tc->c); \
                              } while (0) );
}

/** Input a block of data from a transacting chan-end.
 *
 *  \param tc   Transacting chan-end
 *
 *  \param buf  Pointer to the memory region to fill
 *
 *  \param n    The number of words to receive
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *tc or buf[] argument.
 */
inline unsigned t_chan_in_buf_word(transacting_chanend *tc, int buf[], int n)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                _t_chan_change_to_input(tc); \
                                for (int i = 0; i < n; i++) { \
                                  buf[i] = _s_chan_in_word(tc->c); \
                                } \
                              } while (0) );
}

/** Input a block of data from a transacting chan-end.
 *
 *  \param tc   Transacting chan-end
 *
 *  \param buf  Pointer to the memory region to fill
 *
 *  \param n    The number of bytes to receive
 *
 *  \return     XS1_ET_NONE (or exception type if policy is XCORE_C_NO_EXCEPTION).
 *
 *  \exception  ET_LINK_ERROR         chan-end destination is not set.
 *  \exception  ET_ILLEGAL_RESOURCE   not an allocated chan-end,
 *                                    or channel handshaking corrupted.
 *  \exception  ET_RESOURCE_DEP       another core is actively using the chan-end.
 *  \exception  ET_LOAD_STORE         invalid *tc or buf[] argument.
 */
inline unsigned t_chan_in_buf_byte(transacting_chanend *tc, char buf[], int n)
{
  RETURN_COND_TRYCATCH_ERROR( do { \
                                _t_chan_change_to_input(tc); \
                                for (int i = 0; i < n; i++) { \
                                  buf[i] = _s_chan_in_byte(tc->c); \
                                } \
                              } while (0) );
}

#endif // __XC__

#endif // __xcore_c_channel_transaction_h__
