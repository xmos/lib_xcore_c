// Copyright (c) 2016, XMOS Ltd, All rights reserved

#include <stdio.h>
#include "xcore_c.h"
#include "debug_print.h"

// Timer period which allows enough time for the debug_printfs to complete
static const int period = 10000;

volatile int remaining_chan_c_reads = 30;
volatile int remaining_chan_d_reads = 30;

// For XS1 support all values passed to the interrupt setup function must have bit 16 set
typedef enum {
  INTERRUPT_CHAN_C = INTERRUPT_ENUM_BASE,
  INTERRUPT_CHAN_D
} interrupt_choice_t;

void channel_interrupt_handler(resource r, void* data)
{
  switch ((interrupt_choice_t)data) {
    case INTERRUPT_CHAN_C: {
      int x = chan_input_word(r);
      remaining_chan_c_reads--;
      debug_printf("Received %d on channel c\n", x);
      break;
    }
    case INTERRUPT_CHAN_D: {
      int x = chan_input_word(r);
      remaining_chan_d_reads--;
      debug_printf("Received %d on channel d\n", x);
      break;
    }
  }
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

void test_interrupt_function(chanend c, chanend d, int n)
{
  test_unmasked_config(c, d);
  // Wait for some interrupts, c and d might not have raised the same number
  while (remaining_chan_c_reads > (n + 5) && remaining_chan_d_reads > (n + 5));
  test_disable_interrupts(c, d);

  test_masked_config(c, d);
  // Wait for all of the remaining interrupts
  while (!(remaining_chan_c_reads <= n && remaining_chan_d_reads <= n));
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
  debug_printf("Timer interrupt data 0x%x\n", (int)data);
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
  test_interrupt_function(c, d, 20);

  // Test 2: Run the test function again with the timer disabled
  interrupt_clear_timer(t);
  test_interrupt_function(c, d, 10);

  // Test 3: Run the test function again with the timer enabled
  time = timer_get_time(t);
  interrupt_setup_timer_function(t, handle_timer, (void*)0xfeedbeef, time + period);
  test_interrupt_function(c, d, 0);
}
