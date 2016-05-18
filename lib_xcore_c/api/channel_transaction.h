// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __channel_transaction_h__
#define __channel_transaction_h__

#if defined(__XC__) || defined(__DOXYGEN__)
#define __tc_get_reference_member(x,m) x.m
#else
#define __tc_get_reference_member(x,m) x->m
#endif

#if !defined(__XC__) || defined(__DOXYGEN__)

#include <xs1.h>

/** Type that denotes a transacting channel-end. A transaction temporarily
 *  opens a route to the other side of the channel. During the transaction,
 *  you can use transaction channel operations for increased efficiency.
 *
 *  You can create a transacting chanend from a normal chanend using
 *  chan_init_transaction_master() and chan_init_transaction_slave().
 */
typedef struct transacting_chanend_s {
  chanend c;
  int last_out;
} transacting_chanend;

/** Start a transaction (master).
 *
 *  This initiates a *transaction* on a channel. A transaction temporarily
 *  opens a route to the other side of the channel. During the transaction,
 *  you can use transaction channel operations for increased efficiency.
 *
 *  This called must be matched by a call to chan_init_transaction_slave()
 *  on the other end of the channel.
 *
 *  A transaction should be closed with chan_complete_transaction().
 *
 * \param c   Channel-end to initialize the transaction on
 *
 * \returns   Version of the chanend to use within the transaction
 */
inline transacting_chanend chan_init_transaction_master(chanend c)
{
  transacting_chanend tc;
  tc.c = c;
  tc.last_out = 0;
  s_chan_output_ct((streaming_chanend) c, XS1_CT_END);
  return tc;
}

/** Start a transaction (slave).
 *
 *  This initiates a *transaction* on a channel. A transaction temporarily
 *  opens a route to the other side of the channel. During the transaction,
 *  you can use transaction channel operations for increased efficiency.
 *
 *  This called must be matched by a call to chan_init_transaction_master()
 *  on the other end of the channel.
 *
 *  A transaction should be closed with chan_complete_transaction().
 *
 * \param c   Channel-end to initialize the transaction on
 *
 * \returns   Version of the channel-end to use within the transaction
 */
inline transacting_chanend chan_init_transaction_slave(chanend c)
{
  transacting_chanend tc;
  tc.c = c;
  tc.last_out = 1;
  s_chan_check_ct((streaming_chanend) c, XS1_CT_END);
  return tc;
}

/** Complete a transaction.
 *
 * This function completes a transaction. After this call the route between the
 * two ends of the channel is freed allowing other channels to use the
 * communication network.
 *
 * This call must be accompanied by a call to chan_complete_transaction() on
 * the other side of the channel.
 *
 * \param tc  Transacting channel-end to close the transaction on
 *
 * \returns   Non-transacting version of the channel-end
 */
inline chanend chan_complete_transaction(REFERENCE_PARAM(transacting_chanend, tc))
{
  if (__tc_get_reference_member(tc,last_out)) {
    s_chan_output_ct((streaming_chanend)__tc_get_reference_member(tc,c), XS1_CT_END);
    s_chan_check_ct((streaming_chanend)__tc_get_reference_member(tc,c), XS1_CT_END);
  } else {
    s_chan_check_ct((streaming_chanend)__tc_get_reference_member(tc,c), XS1_CT_END);
    s_chan_output_ct((streaming_chanend)__tc_get_reference_member(tc,c), XS1_CT_END);
  }
  return __tc_get_reference_member(tc,c);
}

/** Manage direction changes.
 *
 * As specified in the Tools Development Guide, the last_out state is managed
 * to control when CT_END tokens are sent or expected
 */
#define __t_chan_change_to_input(tc) \
  do { \
    if (__tc_get_reference_member(tc,last_out)) { \
      __tc_get_reference_member(tc,last_out) = 0; \
      s_chan_output_ct(__tc_get_reference_member(tc,c), XS1_CT_END); \
    } \
  } while (0)

#define __t_chan_change_to_output(tc) \
  do { \
    if (!__tc_get_reference_member(tc,last_out)) { \
      __tc_get_reference_member(tc,last_out) = 1; \
      s_chan_check_ct(__tc_get_reference_member(tc,c), XS1_CT_END); \
    } \
  } while (0)

/** Check that a specific control token is available on a
 * transacting channel-end. This function blocks until a token is available on
 * the transacting channel. If the available token is a control token and
 * has the value ``ct``, then the token is input and discarded. Otherwise
 * an exception is raised.
 *
 * \param tc   Transacting channel-end
 *
 * \param ct   Control token that is expected on the transacting channel
 */
inline void t_chan_check_ct(REFERENCE_PARAM(transacting_chanend, tc), int ct)
{
  __t_chan_change_to_input(tc);
  asm volatile("chkct res[%0],%1" :: "r" (__tc_get_reference_member(tc,c)), "r" (ct));
}

/** Output a word over a transacting channel-end.
 *
 * \param tc   Transacting channel-end
 *
 * \param data Word to be output
 */
inline void t_chan_output_word(REFERENCE_PARAM(transacting_chanend, tc), int data)
{
  __t_chan_change_to_output(tc);
  asm volatile("out res[%0],%1" :: "r" (__tc_get_reference_member(tc,c)), "r" (data));
}

/** Output an byte over a transacting channel-end.
 *
 * \param tc   Transacting channel-end
 *
 * \param data Byte to be output
 */
inline void t_chan_output_byte(REFERENCE_PARAM(transacting_chanend, tc), char data)
{
  __t_chan_change_to_output(tc);
  asm volatile("outt res[%0],%1" :: "r" (__tc_get_reference_member(tc,c)), "r" (data));
}

/** Output a block of data over a transacting channel-end.
 *
 * \param c    Transacting channel-end
 *
 * \param buf  Pointer to the buffer containing the data to send
 *
 * \param n    Number of bytes to send
 */
inline void t_chan_output_block(REFERENCE_PARAM(transacting_chanend, tc), char buf[], int n)
{
  // Note we could do this more efficiently depending on the size of n
  // and the alignment of buf
  __t_chan_change_to_output(tc);
  for (int i = 0; i < n; i++) {
    s_chan_output_byte(__tc_get_reference_member(tc,c), buf[i]);
  }
}

/** Input a word from a transacting channel-end.
 *
 * \param tc   Transacting channel-end
 *
 * \returns    Inputted integer
 */
inline int t_chan_input_word(REFERENCE_PARAM(transacting_chanend, tc))
{
  int data;
  __t_chan_change_to_input(tc);
  asm volatile("in %0,res[%1]" : "=r" (data): "r" (__tc_get_reference_member(tc,c)));
  return data;
}

/** Input a byte from a transacting channel-end.
 *
 * \param tc   Transacting channel-end
 *
 * \returns    Inputted byte
 */
inline char t_chan_input_byte(REFERENCE_PARAM(transacting_chanend, tc))
{
  char data;
  __t_chan_change_to_input(tc);
  asm volatile("int %0,res[%1]" : "=r" (data): "r" (__tc_get_reference_member(tc,c)));
  return data;
}

/** Input a block of data from a transacting channel-end.
 *
 * \param tc   Transacting channel-end
 *
 * \param buf  Pointer to the memory region to fill
 *
 * \param n    The number of bytes to receive
 */
inline void t_chan_input_block(REFERENCE_PARAM(transacting_chanend, tc), char buf[], int n)
{
  // Note we could do this more efficiently depending on the size of n
  // and the alignment of buf
  __t_chan_change_to_input(tc);
  for (int i = 0; i < n; i++) {
    buf[i] = s_chan_input_byte(__tc_get_reference_member(tc,c));
  }
}

#endif // __XC__

#endif // __channel_transaction_h__
