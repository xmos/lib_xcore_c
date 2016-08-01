// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_port_configure.h"

extern void port_configure_in_handshake(port p, port readyin, port readyout,
                                        clock clk);
extern void port_configure_out_handshake(port p, port readyin, port readyout,
                                         clock clk, unsigned initial);
extern void port_configure_in_strobed_master(port p, port readyout,
                                             const clock clk);
extern void port_configure_out_strobed_master(port p, port readyout,
                                              const clock clk, unsigned initial);
extern void port_configure_in_strobed_slave(port p, port readyin, clock clk);
extern void port_configure_out_strobed_slave(port p, port readyin,
                                             clock clk, unsigned initial);
