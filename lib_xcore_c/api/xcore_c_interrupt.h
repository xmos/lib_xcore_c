// Copyright (c) 2016, XMOS Ltd, All rights reserved

#ifndef __xcore_c_interrupt_h__
#define __xcore_c_interrupt_h__

#include <stdint.h>
#include "xcore_c_interrupt_impl.h"
#include "xcore_c_resource_impl.h"
#include "xcore_c_error_codes.h"

/** Define a function that allows interrpts to occur within its scope
 *
 *  This macro will define two functions for you:
 *  1) an ordinary function that may be called directly
 *     Its signature will be '<ret> <root_function>(<...>);'
 *  2) a function that will also reserve space for and set up a stack
 *     for handling interrupts.
 *     The function name is accessed using the INTERRUPT_PERMITTED() macro
 *
 *  You would normally use this macro on the definition of the root function
 *  which will be called in a par statement.
 *  The interrupt stack (kernel stack) is created on the core's stack with the
 *  ksp and sp being modified as necessary. When the functions exits, neither the
 *  kernel stack nor ksp is valid.
 *
 *  The kernel stack allocated has enough space for the interrupt_callback
 *  function (+callees) in the given 'group'. The use of the 'group' identifier
 *  allows a kernel stack to be no larger than that required by its greediest member.
 *
 *  N.B. The kernel stack is not re-entrant so kernel mode must not be masked
 *  from within an interrupt_callback.
*
 *  The user may specify a larger kernel stack by defining XCORE_C_KSTACK_WORDS.
 *
 *  Example usage:
 *    DEFINE_INTERRUPT_PERMITTED(groupA, int, rootfunc, chanend c, int i)
 *    {
 *      // This is the body of 'int rootfunc(chanend c, int i)'
 *    }
 *
 *  \Param group            this is the group of interrupt_callback function
 *                          that may be safely enabled - see DEFINE_INTERRUPT_CALLBACK()
 *  \Param ret              the return type of the ordinary function
 *  \Param root_function    the name of the ordinary function
 *  \Param ...              the arguments of the ordinary function
 */
#define DEFINE_INTERRUPT_PERMITTED(group, ret, root_function, ...) \
        _DEFINE_INTERRUPT_PERMITTED(group, ret, root_function, __VA_ARGS__)

/** Declare an interrupt permitting function
 *
 *  Use this macro when you require a declaration of your interrupt permitting function types
 *
 *  Example usage:
 *    // In another file:
 *    //   DEFINE_INTERRUPT_PERMITTED(groupA, int, rootfunc, chanend c, int i)
 *    //   DEFINE_INTERRUPT_PERMITTED(groupB, void, anotherfunc, void)
 *    DECLARE_INTERRUPT_PERMITTED(int, rootfunc, chanend c, int i);
 *    DECLARE_INTERRUPT_PERMITTED(void, anotherfunc, void);
 *    ...
 *      par {
 *          int ret = INTERRUPT_PERMITTED(rootfunc)(c,i);  // enough kstack for groupA interrupts.
 *          INTERRUPT_PERMITTED(anotherfunc)();  // enough kstack for groupB interrupts.
 *
 *  \Param root_function    this is the name of the ordinary function
 */
#define DECLARE_INTERRUPT_PERMITTED(ret, root_function, ...) \
        _DECLARE_INTERRUPT_PERMITTED(ret, root_function, __VA_ARGS__)

/** The name of the defined interrupt permitting function
 *
 *  Use this macro for retriving the name of the declared interrupt function.
 *  This is the name used to invoke the function.
 *
 *  \return     the name of the defined interrupt permitting function
 */
#define INTERRUPT_PERMITTED(root_function) _INTERRUPT_PERMITTED(root_function)

#ifndef XCORE_C_KSTACK_WORDS
/** Specify the minimum kernel stack size the interrupt permitting function should create.
 *
 *  The user may specify a minimum kstack size by setting the XCORE_C_KSTACK_WORDS
 * define in their Makefile.
 *  This should be done when the kstack is being used by more than interrupt_callback functions.
 */ 
#define XCORE_C_KSTACK_WORDS 0
#endif

/** Mask all interrupts on this logical core.
 *
 *  Prevent any enabled <resource>_setup_interrupt_callback() functions
 *  from triggering.
 *  This has no effect on <resource>_setup_select_callback() triggering.
 *  They can be restored by using interrupt_unmask_all().
 *
 *  \return     error_none
 */
inline xcore_c_error interrupt_mask_all(void)
{
  asm volatile("clrsr 0x2");
  return error_none;
}

/** Unmask all interrupts on this logical core.
 *
 *  Allow any <resource>_setup_interrupt_callback() functions to trigger.
 *  They can be suppressed by using interrupt_mask_all().
 *
 *  \return     error_none
 */
inline xcore_c_error interrupt_unmask_all(void)
{
  asm volatile("setsr 0x2");
  return error_none;
}

/** Define an interrupt handling function
 *
 *  This macro will define two functions for you:
 *  1) an ordinary function that may be called directly
 *     Its signature will be 'void <intrpt>(void* <data>);'
 *  2) an interrupt_callback function for passing to a <resource>_setup_interrupt_callback function.
 *     The interrupt_callback function name is accessed using the INTERRUPT_CALLBACK() macro
 *
 *  N.B. The kernel stack is not re-entrant so kernel mode must not be masked
 *  from within an interrupt_callback.
 *
 *  Example usage:
 *    DEFINE_INTERRUPT_CALLBACK(groupA, myfunc, arg)
 *    {
 *      // This is the body of 'void myfunc(void* arg)'
 *    }
 *
 *  \Param group    the group of interrupt_callback function we belong to
 *                  see DEFINE_INTERRUPT_PERMITTED()
 *  \Param intrpt   this is the name of the ordinary function
 *  \Param data     the name to use for the *void argument
 */
#define DEFINE_INTERRUPT_CALLBACK(group, intrpt, data) \
        _DEFINE_INTERRUPT_CALLBACK(group, intrpt, data)

/** Declare an interrupt handling function
 *
 *  Use this macro when you require a declaration of your interrupt function types
 *
 *  Example usage:
 *    DECLARE_INTERRUPT_CALLBACK(myfunc, arg);
 *    chanend_setup_interrupt_callback(c, 0 , INTERRUPT_CALLBACK(myfunc));
 *
 *  \Param intrpt   this is the name of the ordinary function
 *  \Param data     the name to use for the *void argument
 */
#define DECLARE_INTERRUPT_CALLBACK(intrpt, data) \
        _DECLARE_INTERRUPT_CALLBACK(intrpt, data)

/** The name of the defined 'interrupt_callback' function
 *
 *  Use this macro for retriving the name of the declared interrupt callback function.
 *  This is the name that is passed to <resource>_setup_interrupt_callback() for registration.
 *
 *  \return     the name of the defined interrupt_callback function
 */
#define INTERRUPT_CALLBACK(intrpt) _INTERRUPT_CALLBACK(intrpt)

#endif // __xcore_c_interrupt_h__