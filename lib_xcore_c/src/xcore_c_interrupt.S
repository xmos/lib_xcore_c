// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include "xcore_c_interrupt_defines.h"

.text

// Make these global so that it can be used as the default event vector
.globl __interrupt_function_wrapper

.globl __interrupt_function_wrapper
.globl __interrupt_function_wrapper.nstackwords
.globl __interrupt_function_wrapper.maxthreads
.globl __interrupt_function_wrapper.maxtimers
.globl __interrupt_function_wrapper.maxchanends
.globl __interrupt_function_wrapper.maxsync
.type  __interrupt_function_wrapper, @function
.linkset __interrupt_function_wrapper.locnoside, 1
.linkset __interrupt_function_wrapper.locnochandec, 1
.set __interrupt_function_wrapper.nstackwords, 0
.set __interrupt_function_wrapper.maxthreads, 0
.set __interrupt_function_wrapper.maxtimers, 0
.set __interrupt_function_wrapper.maxchanends, 0
.set __interrupt_function_wrapper.maxsync, 0

// This code will cope with being called in single or dual issue.
// Hence the extra nops.
.align 4
__interrupt_function_wrapper:
  nop
  entsp 6 // Keep stack double word aligned

  // Store the caller-save registers to the stack
  stw r0, sp[1]
  stw r1, sp[2]
  stw r2, sp[3]
  stw r3, sp[4]
  stw r11, sp[5]

  // The index is stored in the ED. It needs the top bits cleared on XS1
  get r11, ed
#ifndef __XS2A__
  // XS1 always makes the environment vector a memory address so need to clear
  // bit 16 and above
  mkmsk r3, 16
  and r3, r11, r3
#else
  add r3, r11, 0
#endif
  ldap r11, interrupt_handler_state
  // Calculate offset into interrupt_handler_state table
  ldc r2, SIZEOF_INTERRUPT_HANDLER_FUNCTION_STATE
  mul r3, r3, r2
  ldaw r0, r11[r3]
  // Load the handler function address
  ldw r2, r0[1]
  // Load the user data (void *)
  ldw r1, r0[2]
  // Load the resource ID
  ldw r0, r0[0]
  // Call handler
  bla r2

  // Restore the caller-save registers
  ldw r0, sp[1]
  ldw r1, sp[2]
  ldw r2, sp[3]
  ldw r3, sp[4]
  ldw r11, sp[5]

  // Restore link register
  ldw lr, sp[6]
  // Restore the stack
  ldaw sp, sp[6]
  // Return and re-enable interrupts
  kret
