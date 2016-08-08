// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_lock.h"

extern unsigned lock_alloc(lock *l);
extern unsigned lock_free(lock *l);
extern unsigned lock_acquire(lock l);
extern unsigned lock_release(lock l);
