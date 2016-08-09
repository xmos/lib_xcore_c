// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_event_impl.h"
extern void _event_enable(resource r);
extern void _event_disable(resource r);
extern void _event_setup_resource(resource r, uint32_t value);

#include "xcore_c_event.h"
extern unsigned event_disable_all(void);
extern unsigned event_setup_timer(timer t, int time, unsigned value);
extern unsigned event_enable_timer(timer t);
extern unsigned event_disable_timer(timer t);
extern unsigned event_change_timer(timer t, int time);
extern unsigned event_setup_chanend(chanend c, unsigned value);
extern unsigned event_enable_chanend(chanend c);
extern unsigned event_disable_chanend(chanend c);
extern unsigned event_setup_port(port p, unsigned value);
extern unsigned event_enable_port(port p);
extern unsigned event_disable_port(port p);
extern unsigned event_change_port_condition(port p, port_condition cond, unsigned data);
extern unsigned event_change_port_time(port p, int16_t count);

