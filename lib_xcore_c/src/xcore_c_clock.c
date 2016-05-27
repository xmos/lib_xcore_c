// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_clock.h"

extern clock clock_enable(int clock_id);
extern void clock_disable(clock clk);
extern void clock_start(clock clk);
extern void clock_stop(clock clk);
extern void clock_set_source_port(clock clk, port p);
extern void clock_set_source_clk_ref(clock clk);
extern void clock_set_source_clk_xcore(clock clk);
extern void clock_set_divide(clock clk, uint8_t divide);
extern void clock_set_ready_src(clock clk, port ready_source);
