// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __code_h__
#define __code_h__

#include <xccompat.h>
#include <xs1.h>
#include "xcore_c.h"

#if __XC__
extern "C" {
#endif

void get_addresses(chanend c, unsigned *local_server, unsigned *remote_server);
void chanend_server(chanend c);
void chanend_client(chanend local_server, chanend remote_server);

#if __XC__
}
#endif

#endif // __code_h__