// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_exception_impl_h__
#define __xcore_c_exception_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#include "trycatch.h"
#include "xcore_c_error_codes.h"

#ifndef XCORE_C_NO_EXCEPTION
/**
 * The exception policy for the library.
 * This is a boolean constant or expression.
 */
#define XCORE_C_NO_EXCEPTION 1
#endif

#define RETURN_EXCEPTION_OR_ERROR(expr) \
  if (XCORE_C_NO_EXCEPTION) { \
    exception_t e; \
    TRY { \
      expr ; \
    } \
    CATCH(e) { \
      return e.type; \
    } \
  } \
  else { \
    expr ; \
  } \
  return XS1_ET_NONE

#endif // __xcore_c_exception_impl_h__

