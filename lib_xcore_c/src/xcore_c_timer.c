// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_timer_impl.h"
extern hwtimer_t _timer_alloc(void);
extern void _timer_free(hwtimer_t t);
extern void _timer_get_time(hwtimer_t t, uint32_t *now);
extern void _timer_change_trigger_time(hwtimer_t t, uint32_t time);
extern void _timer_set_trigger_time(hwtimer_t t, uint32_t time);
extern void _timer_clear_trigger_time(hwtimer_t t);

#include "xcore_c_timer.h"
extern xcore_c_error_t timer_alloc(hwtimer_t *t);
extern xcore_c_error_t timer_free(hwtimer_t *t);
extern xcore_c_error_t timer_get_time(hwtimer_t t, uint32_t *now);
extern xcore_c_error_t timer_set_trigger_time(hwtimer_t t, uint32_t time);
extern xcore_c_error_t timer_clear_trigger_time(hwtimer_t t);
extern xcore_c_error_t timer_change_trigger_time(hwtimer_t t, uint32_t time);
extern xcore_c_error_t timer_wait_until(hwtimer_t t, uint32_t until, uint32_t *now);
extern xcore_c_error_t timer_delay(hwtimer_t t, uint32_t period);

extern xcore_c_error_t timer_setup_select(hwtimer_t t, uint32_t time, uint32_t enum_id);
extern xcore_c_error_t timer_setup_select_callback(hwtimer_t t, uint32_t time, void *data, select_callback_t callback);
extern xcore_c_error_t timer_setup_interrupt_callback(hwtimer_t t, uint32_t time, void *data, interrupt_callback_t intrpt);
extern xcore_c_error_t timer_enable_trigger(hwtimer_t t);
extern xcore_c_error_t timer_disable_trigger(hwtimer_t t);
