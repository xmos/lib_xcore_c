// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __channel_internal_h__
#define __channel_internal_h__

inline void s_chan_output_ct_end(streaming_chanend c)
{
  asm("outct res[%0],1"::"r"(c));
}

inline void s_chan_check_ct_end(streaming_chanend c)
{
  asm("chkct res[%0],1"::"r"(c));
}

#endif // __channel_internal_h__
