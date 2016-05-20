.. include:: ../../../README.rst

Usage
-----

All functions can be accessed via the ``xcore_c.h`` header::

  #include "xcore_c.h"

You will also have to add ``lib_xcore_c`` to the ``USED_MODULES`` field of
your application Makefile.

Using timers
............

The library provides support for xCORE hardware timers. They are allocated
using::

  timer tmr = timer_alloc();

A timer can then be read to get the current time by doing::

  int time = timer_get_time(tmr);

There are two functions provided to delay using a timer. The first waits for a
specified time::

  // The times are in 10ns, so 100000 timer ticks is 1ms
  timer_wait_until(tmr, time + 100000); // Wait for (time + 1ms)

The second delays for a period of time in 100MHz timer ticks::

  timer_delay(tmr, 100000); // Delay for 1ms

When the timer is no longer required it can be released to be used by other cores
by calling::

  timer_free(tmr);

Using channels
..............

Local channels
~~~~~~~~~~~~~~

Local channel connections on a tile are fully supported by the library. A channel
connection is created using::

  chanend c1, c2;
  chan_alloc(&c1, &c2);

Data can then be sent and received using::

  chan_output_word(c1, 1);
  chan_output_byte(c1, 2);

with a corresponding block of code on another core to consume the data::

  int i = chan_input_word(c2);
  char j = chan_input_byte(c2);

When the channel is finished with then it is closed and the resources released
using::

  chan_free(c1, c2);

Inter-tile channels
~~~~~~~~~~~~~~~~~~~

The use of inter-tile channels is supported by the library. However, the only way
to create inter-tile channels is to use a top-level main() written in xC. Without
the top-level main there is no way to automatically communicate the tile ID of
multiple tiles within a system.

A basic top-level main would look like::

  #include <platform.h>
  #include "application.h"

  int main()
  {
    chan c;
    par {
      on tile[0]: {
        application_0(c);
      }
      on tile[1]: {
        application_1(c);
      }
    }
    return 0;
  }

This uses xC to do all of the thread assignment and connecting of the initial
channel. After that, the applications can use more channels on each tile and
have enough information to know how to connect to each other.

A new channel-end can be allocated using::

  void application_0(chanend c)
  {
    chanend new_c = chanend_alloc();

And a new connection established by passing this new channel-end over the
existing link, receiving the destination link on the other tile and connecting
the two. So, both applications can do::

    chan_output_word(c, new_c);
    chanend dest = chan_input_word(c);
    chanend_set_dest(new_c, dest);

Streaming channels
~~~~~~~~~~~~~~~~~~

Streaming channels can be used in a similar manner to standard channel-ends. A
streaming channel is created using::

  streaming_chanend c1, c2;
  s_chan_alloc(&c1, &c2);

Data can then be sent and received using::

  s_chan_output_word(c1, 1);
  s_chan_output_byte(c1, 2);

with a corresponding block of code on another core to consume the data::

  int i = s_chan_input_word(c2);
  char j = s_chan_input_byte(c2);

When the channel is finished with then it is closed and the resources released
using::

  s_chan_free(c1, c2);

Channel transactions
~~~~~~~~~~~~~~~~~~~~

The library has functions to support interacting with xC channel ends. This
includes master/slave transactions. For example, a block of xC could use a
master transaction to send a block of words syhchronised only at the beginning
and end::

  int data[10] = {...}
  master {
    for (int i = 0; i < 10; i++) {
      c <: data[i];
    }
  }

The C code to receive this data is of the form::

  transacting_chanend tc = chan_init_transaction_slave(c);
  int data[10];
  for (int i = 0; i < 10; i++) {
    data[i] = t_chan_input_word(tc);
  }
  chan_complete_transaction(tc);

There are additional functions to send and receive both bytes and blocks of data.

Using ports and clock blocks
............................

The use of ports and clock blocks is fully supported in the library. This section
of the document gives a brief example of how to use the library. For complete
documentation of the functionality supported please see the API section.

Example
~~~~~~~

This example will show how to use the library to configure a clock block and
port. The first thing to do is to configure the clock block. For example, if using
clock block 1 to be clocked from a divided version of the reference clock::

  clock c = clock_enable(XS1_CLKBLK_1);
  clock_set_source_clk_ref(c);
  clock_set_divide(c, 1); // Configure to 50MHz (100Mhz / 2*1)

The port to be used can then be enabled, configured and connected to the clock::

  port p = port_enable(XS1_PORT_1A);
  port_set_clock(p, c);

Starting the clock will reset the port counters on all connected ports. This is
generally best done after all ports have been connected so that their counters
will be synchronised::

  clock_start(c);

The port can now be used to output or input data::

  port_output(p, 1);
  port_output(p, 0);
  ...

In order to clean up, both the port and clock block should be disabled::

  clock_disable(c);
  port_disable(c);

Handshaking
~~~~~~~~~~~

TODO

Using hardware locks
....................

The library provides support for xCORE hardware locks. They are allocated
using::

  lock l = lock_alloc();

To enter a mutex region the lock is then acquired::

  lock_acquire(l);

After this function completes it is safe to use shared state that must only be
used by one core at a time.

To leave the mutex region the lock is released::

  lock_release(l);

The lock resource is released using::

  lock_free(l);

Using events
............

The library provides the ability to re-create the equivalent of the xC *select*
statement.

Example
~~~~~~~

As an example, take a function which receives data from two channels and handles
whichever one is ready.

The function needs to have an *enum* containing an entry per resource that is
part of the *select*::

  typedef enum {
    EVENT_CHAN_C = EVENT_ENUM_BASE,
    EVENT_CHAN_D
  } event_choice_t;

The function then starts by clearing all existing events enable on resources
owned by this core to ensure that they cannot trigger events::

  void channel_example(chanend c, chanend d)
  {
    event_clear_all();

The resources are each configured to trigger events and return a value from the
*enum* specified above::

    event_setup_chanend(c, EVENT_CHAN_C);
    event_setup_chanend(d, EVENT_CHAN_D);

And then the rest of the function can simply use the ``event_select()`` function
to wait for events to be triggered by either resource::

    for (int count = 0; count < 10; count++) {
      event_choice_t choice = event_select();
      switch (choice) {
        case EVENT_CHAN_C: {
          // Read value and clear event
          int x = chan_input_word(c);
          ...
          break;
        }
        case EVENT_CHAN_D: {
          // Read value and clear event
          int x = chan_input_word(d);
          ...
          break;
        }
      }
    }

Event handling with a default
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TODO

Event handling functions
~~~~~~~~~~~~~~~~~~~~~~~~

TODO

The default statement
~~~~~~~~~~~~~~~~~~~~~

TODO

Using interrupts
................

TODO

Trap handlers
.............

TODO

API
---

Supporting types
................

.. doxygentypedef:: resource

.. doxygentypedef:: lock

.. doxygentypedef:: streaming_chanend

.. doxygentypedef:: transacting_chanend

.. doxygentypedef:: event_handler

.. doxygenenum:: port_condition

|newpage|

Timers
......

.. doxygenfunction:: timer_alloc

.. doxygenfunction:: timer_free

.. doxygenfunction:: timer_get_time

.. doxygenfunction:: timer_delay

|newpage|

Channels
........

.. doxygenfunction:: chanend_alloc

.. doxygenfunction:: chanend_free

.. doxygenfunction:: chanend_set_dest

.. doxygenfunction:: chan_alloc

.. doxygenfunction:: chan_free

.. doxygenfunction:: chan_output_word

.. doxygenfunction:: chan_output_byte

.. doxygenfunction:: chan_output_block

.. doxygenfunction:: chan_input_word

.. doxygenfunction:: chan_input_byte

.. doxygenfunction:: chan_input_block

|newpage|

Streaming channels
..................

.. doxygenfunction:: s_chan_alloc

.. doxygenfunction:: s_chan_free

.. doxygenfunction:: s_chan_output_ct

.. doxygenfunction:: s_chan_output_ct_end

.. doxygenfunction:: s_chan_check_ct

.. doxygenfunction:: s_chan_check_ct_end

.. doxygenfunction:: s_chan_output_word

.. doxygenfunction:: s_chan_output_byte

.. doxygenfunction:: s_chan_output_block

.. doxygenfunction:: s_chan_input_word

.. doxygenfunction:: s_chan_input_byte

.. doxygenfunction:: s_chan_input_block

|newpage|

Channels with transactions
..........................

.. doxygenfunction:: chan_init_transaction_master

.. doxygenfunction:: chan_init_transaction_slave

.. doxygenfunction:: chan_complete_transaction

.. doxygenfunction:: t_chan_output_word

.. doxygenfunction:: t_chan_output_byte

.. doxygenfunction:: t_chan_output_block

.. doxygenfunction:: t_chan_input_word

.. doxygenfunction:: t_chan_input_byte

.. doxygenfunction:: t_chan_input_block

|newpage|

Ports
.....

.. doxygenfunction:: port_set_buffered

.. doxygenfunction:: port_set_transfer_width

.. doxygenfunction:: port_enable

.. doxygenfunction:: port_enable_buffered

.. doxygenfunction:: port_disable

.. doxygenfunction:: port_set_transfer_width

.. doxygenfunction:: port_set_unbuffered

.. doxygenfunction:: port_set_buffered

.. doxygenfunction:: port_set_clock

.. doxygenfunction:: port_set_mode_data_port

.. doxygenfunction:: port_set_mode_ready_port

.. doxygenfunction:: port_set_mode_clock_port

.. doxygenfunction:: port_set_ready_input

.. doxygenfunction:: port_output

.. doxygenfunction:: port_output_at_time

.. doxygenfunction:: port_output_shift_right

.. doxygenfunction:: port_output_shift_right_at_time

.. doxygenfunction:: port_peek

.. doxygenfunction:: port_input

.. doxygenfunction:: port_input_when_pinseq

.. doxygenfunction:: port_input_when_pinsneq

.. doxygenfunction:: port_input_at_time

.. doxygenfunction:: port_input_shift_right

.. doxygenfunction:: port_input_shift_right_when_pinseq

.. doxygenfunction:: port_input_shift_right_when_pinsneq

.. doxygenfunction:: port_input_shift_right_at_time

.. doxygenfunction:: port_get_timestamp

.. doxygenfunction:: port_set_condition

.. doxygenfunction:: port_set_conditional_data

.. doxygenfunction:: port_clear_condition

.. doxygenfunction:: port_set_time_condition

.. doxygenfunction:: port_clear_time_condition

.. doxygenfunction:: port_clear_buffer

.. doxygenfunction:: port_endin

.. doxygenfunction:: port_force_input

|newpage|

Clock blocks
............

.. doxygenfunction:: clock_enable

.. doxygenfunction:: clock_disable

.. doxygenfunction:: clock_start

.. doxygenfunction:: clock_stop

.. doxygenfunction:: clock_set_source_port

.. doxygenfunction:: clock_set_source_clk_ref

.. doxygenfunction:: clock_set_source_clk_xcore

.. doxygenfunction:: clock_set_divide

|newpage|

Locks
.....

.. doxygenfunction:: lock_alloc

.. doxygenfunction:: lock_free

.. doxygenfunction:: lock_acquire

.. doxygenfunction:: lock_release

|newpage|

Events
......

.. doxygenfunction:: event_disable

.. doxygenfunction:: event_enable

.. doxygenfunction:: event_clear_all

.. doxygenfunction:: event_setup_timer

.. doxygenfunction:: event_setup_timer_function

.. doxygenfunction:: event_clear_timer

.. doxygenfunction:: event_change_timer_time

.. doxygenfunction:: event_setup_chanend

.. doxygenfunction:: event_setup_chanend_function

.. doxygenfunction:: event_clear_chanend

.. doxygenfunction:: event_setup_port

.. doxygenfunction:: event_setup_port_function

.. doxygenfunction:: event_clear_port

.. doxygenfunction:: event_change_port_condition

.. doxygenfunction:: event_change_port_time

.. doxygenfunction:: event_select

.. doxygenfunction:: event_select_no_wait

.. doxygenfunction:: event_select_ordered

.. doxygenfunction:: event_select_ordered_no_wait

|newpage|

Interrupts
..........

TODO

|newpage|

Trap handlers
.............

TODO

|appendix|

Known Issues
------------

No known issues.

.. include:: ../../../CHANGELOG.rst

