// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_timer.h"

extern inline void timer_alloc(timer *t);
extern inline void timer_free(timer t);
extern inline int timer_get_time(timer t);
extern timer timer_when_timerafter(timer tmr, int time);
extern void wait_until(timer tmr, int time);
extern inline void delay(int period);
extern inline void timer_delay(timer tmr, int period);
extern inline int timer_wait_until(timer tmr, int time);
