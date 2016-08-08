// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_clock.h"

extern unsigned clock_alloc(clock *clk, int clock_id);
extern unsigned clock_free(clock *clk);
extern unsigned clock_start(clock clk);
extern unsigned clock_stop(clock clk);
extern unsigned clock_set_source_port(clock clk, port p);
extern unsigned clock_set_source_clk_ref(clock clk);
extern unsigned clock_set_source_clk_xcore(clock clk);
extern unsigned clock_set_divide(clock clk, uint8_t divide);
extern unsigned clock_set_ready_src(clock clk, port ready_source);
