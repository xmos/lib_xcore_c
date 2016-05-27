// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "lock.h"

extern lock lock_alloc(void);
extern void lock_free(lock l);
extern void lock_acquire(lock l);
extern void lock_release(lock l);
