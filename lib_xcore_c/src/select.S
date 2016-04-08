// Copyright (c) 2016, XMOS Ltd, All rights reserved

.text

// Make this global so that it can be used as the default event vector
.globl __event_target

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

// This code will cope with being called in single or dual issue. Hence the extra
// nops.
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

.issue_mode dual
.align 4
event_select_no_wait:
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

