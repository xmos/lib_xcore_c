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
includes ``master``/``slave`` transactions. For example, a block of xC could use a
``master`` transaction to send a block of words syhchronised only at the beginning
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

.. TODO: example of driving clock from input port

.. Driving clock

Ready signals
~~~~~~~~~~~~~

Configuring ports to use ready signals should be done using the ``port_configure_``
functions provided in ``port_configure.h``. All the basic functions needed to
implement this functionality is provided, but the order of configuring a port
as a strobed or handshaken port is critical and therefore best done using these
wrapper functions.

For example, to create a data port which is controlled by a strobe then the
following code sequence could be used::

  port p = port_enable(XS1_PORT_4A);
  port p_ready = port_enable(XS1_PORT_1A);
  clock clk = clock_enable(XS1_CLKBLK_1);
  clock_start(clk);

  port_configure_in_strobed_slave(p, p_ready, clk);

After this, any data received on the port ``p`` will only be available when the
valid signal (strobe on ``PORT_1A``) is high.

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

The library provides the ability to re-create the equivalent of the xC ``select``
statement.

Example
~~~~~~~

As an example, take a function which receives data from two channels and handles
whichever one is ready.

The function needs to have an *enum* containing an entry per resource that is
part of the ``select``::

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

    while (1) {
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

In xC a ``select`` can have a ``default`` case which is executed if no events
have triggered. This library provides the user with the ability to do this by
using the ``event_select_no_wait()`` function. For example, the above example
could be changed to add to the ``enum`` a default value::

  typedef enum {
    EVENT_CHAN_C = EVENT_ENUM_BASE,
    EVENT_CHAN_D,
    EVENT_DEFAULT
  } event_choice_t;

And then to test for events but perform some background task if there is no data
available on either channel::

    while (1) {
      event_choice_t choice = event_select_no_wait(EVENT_DEFAULT);
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
        case EVENT_DEFAULT: {
          // Run background task
          ...
          break;
        }
      }
    }

The argument that is passed to ``event_select_no_wait()`` is the value that will
be returned if no events are ready.

Event handling functions
~~~~~~~~~~~~~~~~~~~~~~~~

This library also supports the ability to install event handling functions. This
allows the user to write code where events are not all handled within one
``switch`` statement. It makes it possible to write libraries which are completely
self-contained.

For example, if the user writes a library to perform a real-time task based on
a timer event the library initialisation would install a handler::

  void lib_init(void *data)
  {
    timer tmr = timer_alloc();
    int time = timer_get_time(tmr);
    event_setup_timer_function(tmr, timer_handler_func, data, time + period);
  }

This code allocates a hardware timer and then gets the current time before
registering an event handler. The call to ``event_setup_timer_function()`` takes
four arguments.

  1. The timer to configure

  2. The handler function to call when events are triggered by the timer

  3. A ``void*`` which is user data that is passed to the handler

  4. The time at which the next event should fire

*Note*: There are similar functions for ports (``event_setup_port_function()``)
and channel ends (``event_setup_chanend_function()``).

The handler function is passed the resource triggering the event and the user
data registered with that resource::

  void timer_handler_func(resource r, void *data);

A table is used to register the user data with an event and this table has a
default size of 20 entries. In order to change the number of resources that can
have event handler functions registered then the define ``EVENT_MAX_HANDLER_FUNCTIONS``
can be changed at compile time.

The main event handling function then needs to change in order to use this style
of event handling function. The ``event_clear_all()`` function should not be
called, otherwise the timer event will be disabled. Instead, the function should
now clear any events it enables::

  void handle_events(chanend c, chanend d)
  {
    // Setup the channels to generate events
    event_setup_chanend(c, EVENT_CHAN_C);
    event_setup_chanend(d, EVENT_CHAN_D);

    // Handle events using event_select() / event_select_no_wait()
    ...

    // Disable events local to this function
    event_clear_chanend(c);
    event_clear_chanend(d);
  }

After the ``handle_events()`` function has completed another equivalent function
can be called in which the timer handler will continue to be called periodically.

When the timer events are no longer required then they can be disabled using the
``event_clear_timer()`` function (or equivalent port/chanend functions)::

  event_clear_timer(tmr);

Ordered events
~~~~~~~~~~~~~~

The xCORE hardware has implicit ordering enforced. Ports are highest priority,
then timers, then channels. If there are multiple resources of the same type
that are ready then the resource with the lowest resource ID will be selected.

If the user wants to enforce a different ordering from that provided by the
hardware then they can use ``event_select_ordered()`` (or
``event_select_ordered_no_wait()`` to have a default).

Events are set up in as detailed above and a list is created with all the active
resources. For example, if using two channels (``c``, ``d``) and a timer (``tmr``)
then a null-terminated list can be defined to ensure the channels are handled
before the timer if they are ready::

  resource ids[4] = {c, d, tmr, 0};

And then the core of the event handling loop would be changed to pass this list
of resource IDs to define the order in which events are enabled::

  while (1) {
    event_choice_t choice = event_select_ordered(ids);
    switch (choice) {
    ...
    }
  }

Using interrupts
................

The library provides support for hardware interrupts from xCORE resources.

Interrupts can be raised by resources as an alternative to events, and will be vectored to the provided handler function.

As interrupts can occur at any point during program execution there are certain
requirements which must be adhered to ensure safe operation:

  #. Resources must not have interrupts enabled whilst being configured, or
     the core must have interrupts masked if the resource has already been
     configured to raise interrupts.

  #. The core must have interrupts masked when disabling interrupts for a
     resource.
.. Using trap handlers
.. ...................

.. TODO

API
---

Supporting types
................

.. doxygentypedef:: resource

.. doxygentypedef:: lock

.. doxygentypedef:: streaming_chanend

.. doxygentypedef:: transacting_chanend

.. doxygentypedef:: event_handler

.. doxygentypedef:: interrupt_handler

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

.. doxygenfunction:: port_set_ready_src

.. doxygenfunction:: port_set_invert

.. doxygenfunction:: port_set_no_invert

.. doxygenfunction:: port_set_sample_delay

.. doxygenfunction:: port_set_no_sample_delay

.. doxygenfunction:: port_set_master

.. doxygenfunction:: port_set_slave

.. doxygenfunction:: port_set_no_ready

.. doxygenfunction:: port_set_ready_strobed

.. doxygenfunction:: port_set_ready_handshake

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

Port configuration helpers
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. doxygenfunction:: port_configure_in_handshake

.. doxygenfunction:: port_configure_out_handshake

.. doxygenfunction:: port_configure_in_strobed_master

.. doxygenfunction:: port_configure_out_strobed_master

.. doxygenfunction:: port_configure_in_strobed_slave

.. doxygenfunction:: port_configure_out_strobed_slave

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

.. doxygendefine:: EVENT_MAX_HANDLER_FUNCTIONS

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

.. doxygendefine:: INTERRUPT_MAX_HANDLER_FUNCTIONS

.. doxygenfunction:: interrupt_disable

.. doxygenfunction:: interrupt_enable

.. doxygenfunction:: interrupt_mask_all

.. doxygenfunction:: interrupt_unmask_all

.. doxygenfunction:: interrupt_setup_timer_function

.. doxygenfunction:: interrupt_clear_timer

.. doxygenfunction:: interrupt_change_timer_time

.. doxygenfunction:: interrupt_setup_chanend_function

.. doxygenfunction:: interrupt_clear_chanend

.. doxygenfunction:: interrupt_setup_port_function

.. doxygenfunction:: interrupt_clear_port

.. doxygenfunction:: interrupt_change_port_condition

.. doxygenfunction:: interrupt_change_port_time

|newpage|

.. Trap handlers
.. .............

.. TODO

|appendix|

Known Issues
------------

No known issues.

.. include:: ../../../CHANGELOG.rst

