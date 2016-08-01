// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_event.h"

extern void event_disable(resource r);
extern void event_enable(resource r);
extern void event_clear_all(void);
extern void event_setup_timer(timer t, int time, unsigned value);
extern void event_clear_timer(timer t);
extern void event_change_timer_time(timer t, int time);
extern void event_setup_chanend(chanend c, unsigned value);
extern void event_clear_chanend(chanend c);
extern void event_setup_port(port p, unsigned value);
extern void event_clear_port(port p);
extern void event_change_port_condition(port p, port_condition cond, unsigned data);
extern void event_change_port_time(port p, int16_t count);

extern void event_setup_resource(resource r, unsigned value);
extern void event_setup_timer_common(timer t, int time);
