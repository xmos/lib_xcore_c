// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_lock.h"

extern xcore_c_error lock_alloc(lock *l);
extern xcore_c_error lock_free(lock *l);
extern xcore_c_error lock_acquire(lock l);
extern xcore_c_error lock_release(lock l);
