// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

// Timer period which allows enough time for the debug_printfs to complete
static const int period = 10000;

int remaining_chan_c_reads = 5;
int remaining_chan_d_reads = 5;

int global_chan_c_val = -1;
int global_chan_d_val = -1;
int global_timer_val = -1;

int test_complete = 0;

// For XS1 support all values passed to the interrupt setup function must have bit 16 set
typedef enum {
  INTERRUPT_CHAN_C = INTERRUPT_ENUM_BASE,
  INTERRUPT_CHAN_D
} interrupt_choice_t;

void channel_interrupt_handler(resource r, void* data)
{
  // interrupt_mask_all();
  switch ((interrupt_choice_t)data) {
    case INTERRUPT_CHAN_C: {
      int x = chan_input_word(r);
      remaining_chan_c_reads--;
      // debug_printf("Received %d on channel c\n", x);
      global_chan_c_val = x;
      break;
    }
    case INTERRUPT_CHAN_D: {
      int x = chan_input_word(r);
      remaining_chan_d_reads--;
      // debug_printf("Received %d on channel d\n", x);
      global_chan_d_val = x;
      break;
    }
  }
  // interrupt_unmask_all();
}

void test_unmasked_config(chanend c, chanend d)
{
  interrupt_unmask_all();
  // Setup the channels to generate interrupts
  interrupt_setup_chanend_function(c, channel_interrupt_handler,
                                   (void *)INTERRUPT_CHAN_C);
  interrupt_setup_chanend_function(d, channel_interrupt_handler,
                                   (void *)INTERRUPT_CHAN_D);
}

void test_masked_config(chanend c, chanend d)
{
  interrupt_mask_all();
  // Setup the channels to generate interrupts
  interrupt_setup_chanend_function(c, channel_interrupt_handler,
                                   (void *)INTERRUPT_CHAN_C);
  interrupt_setup_chanend_function(d, channel_interrupt_handler,
                                   (void *)INTERRUPT_CHAN_D);
  interrupt_unmask_all();
}

void test_disable_interrupts(chanend c, chanend d)
{
  interrupt_mask_all();
  interrupt_clear_chanend(c);
  interrupt_clear_chanend(d);
}

void test_interrupt_function(chanend c, chanend d)
{
  remaining_chan_c_reads = remaining_chan_d_reads = 5;
  test_unmasked_config(c, d);
  // Wait for interrupts
  while (remaining_chan_c_reads && remaining_chan_d_reads);
  test_disable_interrupts(c, d);

  remaining_chan_c_reads = remaining_chan_d_reads = 5;
  test_masked_config(c, d);
  // Wait for interrupts
  while (remaining_chan_c_reads && remaining_chan_d_reads);
  test_disable_interrupts(c, d);
}

/*
 * Interrupt handler function. Passed the timer that triggered the interrupt and
 * user data associated with that resource. This simply updates the timer to
 * fire after the period again.
 */
void handle_timer(resource r, void *data)
{
  timer t = (timer)r;

  int time = timer_get_time(t);
  // debug_printf("Timer interrupt data 0x%x\n", (int)data);
  global_timer_val = (int)data;
  interrupt_change_timer_time(t, time + period);
}

void test(chanend c, chanend d)
{
  // Test enabling a timer outside of the interrupt loop with a function to handle
  // the interrupts.
  timer t = timer_alloc();

  // Test 1: Run the test function with the timer enabled
  int time = timer_get_time(t);
  interrupt_setup_timer_function(t, handle_timer, (void*)0xfeedbeef, time + period);
  test_interrupt_function(c, d);

  // Test 2: Run the test function again with the timer disabled
  interrupt_clear_timer(t);
  test_interrupt_function(c, d);

  // Test 3: Run the test function again with the timer enabled
  time = timer_get_time(t);
  interrupt_setup_timer_function(t, handle_timer, (void*)0xfeedbeef, time + period);
  test_interrupt_function(c, d);

  test_complete = 1;
}

void printer()
{
  int local_chan_c_val = -1;
  int local_chan_d_val = -1;
  int local_timer_val = -1;

  while (!test_complete)
  {
    if (local_chan_c_val != global_chan_c_val)
    {
      local_chan_c_val = global_chan_c_val;
      debug_printf("Received %d on channel c\n", local_chan_c_val);
    }
    if (local_chan_d_val != global_chan_d_val)
    {
      local_chan_d_val = global_chan_d_val;
      debug_printf("Received %d on channel d\n", local_chan_d_val);
    }
    if (local_timer_val != global_timer_val)
    {
      debug_printf("Timer interrupt data 0x%x\n", local_timer_val);
      global_timer_val = -1;
    }
  }
}

void c_printer()
{
  int local_chan_c_val = -1;

  while (!test_complete)
  {
    if (local_chan_c_val != global_chan_c_val)
    {
      local_chan_c_val = global_chan_c_val;
      debug_printf("Received %d on channel c\n", local_chan_c_val);
    }
  }
}

void d_printer()
{
  int local_chan_d_val = -1;

  while (!test_complete)
  {
    if (local_chan_d_val != global_chan_d_val)
    {
      local_chan_d_val = global_chan_d_val;
      debug_printf("Received %d on channel d\n", local_chan_d_val);
    }
  }
}

void t_printer()
{
  int local_timer_val = -1;

  while (!test_complete)
  {
    if (local_timer_val != global_timer_val)
    {
      local_timer_val = global_timer_val;
      debug_printf("Timer interrupt data 0x%x\n", local_timer_val);
    }
  }
}
