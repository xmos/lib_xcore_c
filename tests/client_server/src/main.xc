// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <platform.h>
#include "code.h"

int main()
{
  chan c;
  par {
    on tile[0]: {
    unsigned tile0_local_server, tile0_remote_server;
      get_addresses(c, &tile0_local_server, &tile0_remote_server);
    par {
      chanend_server(c);
      unsafe {
        chanend_client((chanend)tile0_local_server, (chanend)tile0_remote_server);
      }
    }
  }
    on tile[1]: {
      unsigned tile1_local_server, tile1_remote_server;
        get_addresses(c, &tile1_local_server, &tile1_remote_server);
      par {
        chanend_server(c);
        unsafe {
          chanend_client((chanend)tile1_local_server, (chanend)tile1_remote_server);
        }
      }
    }
  }
  return 0;
}
