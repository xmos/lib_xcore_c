// Copyright (c) 2016, XMOS Ltd, All rights reserved

.text

// Make these global so that it can be used as the default event vector
.globl __event_target
.globl __event_function_wrapper

.globl event_select
.globl event_select.nstackwords
.globl event_select.maxthreads
.globl event_select.maxtimers
.globl event_select.maxchanends
.globl event_select.maxsync
.type  event_select, @function
.linkset event_select.locnoside, 1
.linkset event_select.locnochandec, 1
.set event_select.nstackwords, 0
.set event_select.maxthreads, 0
.set event_select.maxtimers, 0
.set event_select.maxchanends, 0
.set event_select.maxsync, 0

// This code will cope with being called in single or dual issue.
// Hence the extra nops.
.align 4
event_select:
  waiteu
  nop
__event_target:
  get r11, ed
  nop
  add r0, r11, 0
  retsp 0


.globl event_select_no_wait
.globl event_select_no_wait.nstackwords
.globl event_select_no_wait.maxthreads
.globl event_select_no_wait.maxtimers
.globl event_select_no_wait.maxchanends
.globl event_select_no_wait.maxsync
.type  event_select_no_wait, @function
.linkset event_select_no_wait.locnoside, 1
.linkset event_select_no_wait.locnochandec, 1
.set event_select_no_wait.nstackwords, 0
.set event_select_no_wait.maxthreads, 0
.set event_select_no_wait.maxtimers, 0
.set event_select_no_wait.maxchanends, 0
.set event_select_no_wait.maxsync, 0

#if defined(__XS2A__)

.issue_mode single
.align 4
event_select_no_wait:
  nop
  entsp 0x0           // Enter single issue
  setsr 0x1           // Enable events
  nop                 // Allow channel events to fire
  clrsr 0x1           // Disable events
  retsp 0x0

#else

.align 4
event_select_no_wait:
  setsr 0x1           // Enable events
  clrsr 0x1           // Disable events
  retsp 0x0

#endif


.globl event_select_ordered
.globl event_select_ordered.nstackwords
.globl event_select_ordered.maxthreads
.globl event_select_ordered.maxtimers
.globl event_select_ordered.maxchanends
.globl event_select_ordered.maxsync
.type  event_select_ordered, @function
.linkset event_select_ordered.locnoside, 1
.linkset event_select_ordered.locnochandec, 1
.set event_select_ordered.nstackwords, 0
.set event_select_ordered.maxthreads, 0
.set event_select_ordered.maxtimers, 0
.set event_select_ordered.maxchanends, 0
.set event_select_ordered.maxsync, 0

#if defined(__XS2A__)
.issue_mode single
#endif

.align 4
event_select_ordered:

#if defined(__XS2A__)
  nop
  entsp 0x0           // Enter single issue
#endif

  clre                // Clear all previously enabled resources
  setsr 0x1           // Enable events on core
  ldc r2, 0           // Resource list index

  // Enable each individual resource in the list in order
event_select_ordered_loop:
  ldw r3, r0[r2]
  bf r3, event_select_ordered_enable_done
  eeu res[r3]
  add r2, r2, 1
  bu event_select_ordered_loop

event_select_ordered_enable_done:
  waiteu


.globl event_select_ordered_no_wait
.globl event_select_ordered_no_wait.nstackwords
.globl event_select_ordered_no_wait.maxthreads
.globl event_select_ordered_no_wait.maxtimers
.globl event_select_ordered_no_wait.maxchanends
.globl event_select_ordered_no_wait.maxsync
.type  event_select_ordered_no_wait, @function
.linkset event_select_ordered_no_wait.locnoside, 1
.linkset event_select_ordered_no_wait.locnochandec, 1
.set event_select_ordered_no_wait.nstackwords, 0
.set event_select_ordered_no_wait.maxthreads, 0
.set event_select_ordered_no_wait.maxtimers, 0
.set event_select_ordered_no_wait.maxchanends, 0
.set event_select_ordered_no_wait.maxsync, 0

#if defined(__XS2A__)
.issue_mode single
#endif

.align 4
event_select_ordered_no_wait:

#if defined(__XS2A__)
  nop
  entsp 0x0           // Enter single issue
#endif

  clre                // Clear all previously enabled resources
  setsr 0x1           // Enable events on core
  ldc r2, 0           // Resource list index

  // Enable each individual resource in the list in order
event_select_ordered_no_wait_loop:
  ldw r3, r0[r2]
  bf r3, event_select_ordered_enable_no_wait_done
  eeu res[r3]
  add r2, r2, 1
  bu event_select_ordered_no_wait_loop

event_select_ordered_enable_no_wait_done:
  clrsr 0x1           // Disable events on the core
  add r0, r1, 0       // Move the no_wait_value to the return value
  retsp 0
