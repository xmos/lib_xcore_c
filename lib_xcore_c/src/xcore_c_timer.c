// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_timer.h"

extern unsigned timer_alloc(timer *t);
extern unsigned timer_free(timer *t);
extern unsigned timer_get_time(timer t, int *now);
extern unsigned timer_wait_until(timer t, int until, int *now);
extern unsigned timer_delay(timer t, int period);
