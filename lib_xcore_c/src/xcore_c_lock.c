// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_lock.h"

extern void lock_alloc(lock *l);
extern void lock_free(lock l);
extern void lock_acquire(lock l);
extern void lock_release(lock l);
