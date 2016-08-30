// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_exception_impl_h__
#define __xcore_c_exception_impl_h__

// This file contains private implementation details and is not part of the API.
// The contents may vary between releases.

#include "trycatch.h"
#include "xcore_c_error_codes.h"

#ifndef XCORE_C_NO_EXCEPTION
/** The exception policy for the library.
 *
 *  If the user respects the resource type parameter of the library,
 *  checks for zero on allocation, passes in valid pointers and
 *  does not access the same resource on multiple logical cores
 *  the library will not throw an exception.
 *  Exceptions are seen as user error.
 *
 *  The default exception policy for the library is to
 *  throw exceptions.
 *  Setting the library to no_exceptions will increase the code size
 *  and run time of the resultant binary.
 *  
 *  XCORE_C_NO_EXCEPTION may be set by the user to a boolean constant or expression.
 */
#define XCORE_C_NO_EXCEPTION 0
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
  return error_none

#endif // __xcore_c_exception_impl_h__
