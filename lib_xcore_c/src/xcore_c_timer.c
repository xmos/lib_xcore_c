// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_timer_impl.h"
extern timer _timer_alloc(void);
extern void _timer_free(timer t);
extern void _timer_get_time(timer t, int *now);
extern void _timer_change_trigger_time(timer t, int time);
extern void _timer_set_trigger_time(timer t, int time);
extern void _timer_clear_trigger_time(timer t);

#include "xcore_c_timer.h"
extern xcore_c_error timer_alloc(timer *t);
extern xcore_c_error timer_free(timer *t);
extern xcore_c_error timer_get_time(timer t, int *now);
extern xcore_c_error timer_set_trigger_time(timer t, int time);
extern xcore_c_error timer_clear_trigger_time(timer t);
extern xcore_c_error timer_change_trigger_time(timer t, int time);
extern xcore_c_error timer_wait_until(timer t, int until, int *now);
extern xcore_c_error timer_delay(timer t, int period);

extern xcore_c_error timer_setup_select(timer t, int time, unsigned enum_id);
extern xcore_c_error timer_setup_select_callback(timer t, int time, void *data, select_callback callback);
extern xcore_c_error timer_setup_interrupt_callback(timer t, int time, void *data, interrupt_callback intrpt);
extern xcore_c_error timer_enable_trigger(timer t);
extern xcore_c_error timer_disable_trigger(timer t);
