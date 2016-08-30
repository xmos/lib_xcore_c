// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_timer_impl.h"
extern hwtimer_t _timer_alloc(void);
extern void _timer_free(hwtimer_t t);
extern void _timer_get_time(hwtimer_t t, int *now);
extern void _timer_change_trigger_time(hwtimer_t t, int time);
extern void _timer_set_trigger_time(hwtimer_t t, int time);
extern void _timer_clear_trigger_time(hwtimer_t t);

#include "xcore_c_timer.h"
extern xcore_c_error timer_alloc(hwtimer_t *t);
extern xcore_c_error timer_free(hwtimer_t *t);
extern xcore_c_error timer_get_time(hwtimer_t t, int *now);
extern xcore_c_error timer_set_trigger_time(hwtimer_t t, int time);
extern xcore_c_error timer_clear_trigger_time(hwtimer_t t);
extern xcore_c_error timer_change_trigger_time(hwtimer_t t, int time);
extern xcore_c_error timer_wait_until(hwtimer_t t, int until, int *now);
extern xcore_c_error timer_delay(hwtimer_t t, int period);

extern xcore_c_error timer_setup_select(hwtimer_t t, int time, unsigned enum_id);
extern xcore_c_error timer_setup_select_callback(hwtimer_t t, int time, void *data, select_callback callback);
extern xcore_c_error timer_setup_interrupt_callback(hwtimer_t t, int time, void *data, interrupt_callback intrpt);
extern xcore_c_error timer_enable_trigger(hwtimer_t t);
extern xcore_c_error timer_disable_trigger(hwtimer_t t);
