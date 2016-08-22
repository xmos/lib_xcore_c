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

  timer tmr;
  timer_alloc(&tmr);

A timer can then be read to get the current time by doing::

  int time;
  timer_get_time(tmr, &time);

There are two functions provided to delay using a timer. The first waits for a
specified time::

  // The times are in 10ns, so 100000 timer ticks is 1ms
  int now;
  timer_wait_until(tmr, time + 100000, &now); // Wait for (time + 1ms)

The second delays for a period of time in 100MHz timer ticks::

  timer_delay(tmr, 100000); // Delay for 1ms

When the timer is no longer required it can be released to be used by other cores
by calling::

  timer_free(&tmr);

Using channels
..............

Local channels
~~~~~~~~~~~~~~

Local channel connections on a tile are fully supported by the library. A channel
connection is created using::

  channel c;
  chan_alloc(&c);

Data can then be sent and received using::

  chan_out_word(c.left, 1);
  chan_out_byte(c.left, 2);

with a corresponding block of code on another core to consume the data::

  int i;
  chan_in_word(c.right, &i);
  char j;
  chan_in_byte(c.right, &j);

When the channel is finished with then it is closed and the resources released
using::

  chan_free(&c);

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
    chanend new_c;
    chanend_alloc(&new_c);

And a new connection established by passing this new channel-end over the
existing link, receiving the destination link on the other tile and connecting
the two. So, both applications can do::

    chan_out_word(c, new_c);             // Send my new-chanend to other tile.
    chanend new_dest;
    chan_in_word(c, new_dest);           // Recieve other tile's new-chanend...
    chanend_set_dest(new_c, new_dest);   // ... and connect it to my new-chanend.

When the channel-end is finished with then it is closed and the resources released
using::

  chanend_free(&new_c);

Streaming channels
~~~~~~~~~~~~~~~~~~

Streaming channels can be used in a similar manner to standard channels. A
streaming channel is created using::

  streaming_chanend c;
  s_chan_alloc(&c);

Data can then be sent and received using::

  s_chan_out_word(c.left, 1);
  s_chan_out_byte(c.left, 2);

with a corresponding block of code on another core to consume the data::

  int i;
  s_chan_in_word(c.right, &i);
  char j;
  s_chan_in_byte(c.right, &j);

When the channel is finished with then it is closed and the resources released
using::

  s_chan_free(&c);

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

  // we have a chanend 'c';
  transacting_chanend tc;
  chan_init_transaction_slave(&c, &tc);
  int data[10];
  for (int i = 0; i < 10; i++) {
    t_chan_in_word(tc, &data[i]);
  }
  chan_complete_transaction(&tc, &c);

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

  clock c;
  clock_alloc(&c, XS1_CLKBLK_1);
  clock_set_source_clk_ref(c);
  clock_set_divide(c, 1); // Configure to 50MHz (100Mhz / 2*1)

The port to be used can then be enabled, configured and connected to the clock::

  port p;
  port_alloc(&p, XS1_PORT_1A);
  port_set_clock(p, c);

Starting the clock will reset the port counters on all connected ports. This is
generally best done after all ports have been connected so that their counters
will be synchronised::

  clock_start(c);

The port can now be used to output or input data::

  port_output(p, 1);
  port_output(p, 0);
  ...

In order to clean up, both the port and clock block must be freed::

  clock_free(&c);
  port_free(&p);

.. TODO: example of driving clock from input port

.. Driving clock

Ready signals
~~~~~~~~~~~~~

Configuring ports to use ready signals is done using the ``port_configure_``
functions provided in ``port_configure.h``. All the basic functions needed to
implement this functionality is provided, but the order of configuring a port
as a strobed or handshaken port is critical and therefore best done using these
wrapper functions.

For example, to create a data port which is controlled by a strobe then the
following code sequence could be used::

  port p;
  port_alloc(&p, XS1_PORT_4A);
  port p_ready;
  port_alloc(&p_ready, XS1_PORT_1A);
  clock clk;
  clock_alloc(&clk, XS1_CLKBLK_1);

  port_configure_in_strobed_slave(p, p_ready, clk);
  clock_start(clk);

After this, any data received on the port ``p`` will only be available when the
valid signal (strobe on ``PORT_1A``) is high.

Using hardware locks
....................

The library provides support for xCORE hardware locks. They are allocated
using::

  lock l;
  lock_alloc(&l);

To enter a mutex region the lock is then acquired::

  lock_acquire(l);

After this function completes it is safe to use shared state that must only be
used by one core at a time.

To leave the mutex region the lock is released::

  lock_release(l);

The lock resource is released using::

  lock_free(&l);

Using select events
...................

The library provides the ability to re-create the equivalent of the xC ``select``
statement.

Example
~~~~~~~

As an example, take a function which receives data from two channels and handles
whichever one is ready.

The function needs to have an *enum* containing an entry per resource that is
part of the ``select``::

  typedef enum {
    EVENT_CHAN_C = ENUM_ID_BASE,
    EVENT_CHAN_D
  } event_choice_t;

The function then starts by clearing all existing select triggers on resources
owned by this core to ensure that they cannot trigger events::

  void channel_example(chanend c, chanend d)
  {
    select_disable_trigger_all();

The resources are each configured to trigger events and return a value from the
*enum* specified above::

    chanend_setup_select(c, EVENT_CHAN_C);
    chanend_enable_trigger(c);
    chanend_setup_select(d, EVENT_CHAN_D);
    chanend_enable_trigger(d);

And then the rest of the function can simply use the ``select_wait()`` function
to wait for events to be triggered by either resource::

    while (1) {
      event_choice_t choice = select_wait();
      int x;
      switch (choice) {
        case EVENT_CHAN_C: {
          // Read value to clear the trigger
          chan_in_word(c, &x);
          ...
          break;
        }
        case EVENT_CHAN_D: {
          // Read value to clear the trigger
          chan_in_word(d, &x);
          ...
          break;
        }
      }
    }

Select event handling with a default
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In xC a ``select`` can have a ``default`` case which is executed if no events
have triggered. This library provides the user with the ability to do this by
using the ``select_no_wait()`` function. For example, the above example
could be changed to add to the ``enum`` a no-event value::

  typedef enum {
    EVENT_CHAN_C = ENUM_ID_BASE,
    EVENT_CHAN_D,
    EVENT_NONE
  } event_choice_t;

And then to test for triggers but perform some background task if there is no data
available on either channel::

    while (1) {
      event_choice_t choice = select_no_wait(EVENT_NONE);
      int x;
      switch (choice) {
        case EVENT_CHAN_C: {
          // Read value and clear event
          chan_in_word(c, &x);
          ...
          break;
        }
        case EVENT_CHAN_D: {
          // Read value and clear event
          chan_in_word(d, &x);
          ...
          break;
        }
        case EVENT_NONE: {
          // Run background task
          ...
          break;
        }
      }
    }

The argument that is passed to ``select_no_wait()`` is the value that will
be returned if no events are ready.

Select event callback functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This library also supports the ability to install select event callback functions. This
allows the user to write code where events are not all handled within one
``switch`` statement. It makes it possible to write libraries which are completely
self-contained.

For example, if the user writes a library to perform a real-time task based on
a timer event the library initialisation would install a callback::

  void lib_init(void *data)
  {
    timer tmr;
    timer_alloc(&tmr);
    int time;
    timer_get_time(tmr, &time);
    timer_setup_select_callback(tmr, time + period, data, timer_callback_func);
  }

This code allocates a hardware timer and then gets the current time before
registering callback function. The call to ``timer_setup_select_callback()`` takes
four arguments.

  1. The timer to configure
  2. The time at which the next event should fire
  3. A ``void*`` which is user data that is passed to the handler
  4. The callback function to call when events are triggered by the timer

*Note*: There are similar functions for ports (``port_setup_select_callback()``)
and channel ends (``chanend_setup_select_callback()``).

The callback function is passed the user data registered with that resource::

  void timer_callback_func(void *data);

This will usually be the resource's ID so that the callback can access the resource.
If additional information is required, data may be a pointer to a struct::

  typedef struct data_t {timer tmr; int period;} data_t;

  void timer_callback_func(void *data) {
    data_t *d = (data_t *)data;
    int time;
    timer_get_time(d->tmr, &time);
    timer_change_trigger_time(d->tmr, time + d->period);


When using select callback functions, the ``select_disable_trigger_all()`` function
should not be called, otherwise any registered callback functions will be disabled.
Instead, users should now clear any triggers it enables::

  void handle_events(chanend c, chanend d)
  {
    // Setup the channels to generate select events
    chanend_setup_select(c, EVENT_CHAN_C);
    chanend_enable_trigger(c);
    chanend_setup_select(d, EVENT_CHAN_D);
    chanend_enable_trigger(d);

    // Handle select events using select_wait() / select_no_wait()
    ...

    // Disable select events local to this function
    chanend_disable_trigger(c);
    chanend_disable_trigger(d);
    // The chenends keep their setup should you wish to re-enable their triggering.
  }

After the ``handle_events()`` function has completed another equivalent function
can be called in which the timer callback will continue to be called periodically.

When the timer select callbacks are no longer required then they can be disabled
 using the ``timer_disable_trigger()`` function (or equivalent port/chanend functions)::

  timer_disable_trigger(tmr);

Ordered select events
~~~~~~~~~~~~~~~~~~~~~

The xCORE hardware has implicit ordering enforced. Ports are highest priority,
then timers, then channels. If there are multiple resources of the same type
that are ready then the resource with the highest priority (lowest resource ID)
will be selected.

If the user wants to enforce a different ordering from that provided by the
hardware then they can use ``select_wait_ordered()`` (or the no wait equivalent
``select_no_wait_ordered()``).

Events are set up as detailed above and a list is created with all the active
resources. For example, if using two channels (``c``, ``d``) and a timer (``tmr``)
then a null-terminated list can be defined to ensure the channels are handled
before the timer if they are ready::

  resource ids[4] = {c, d, tmr, 0};

And then the core of the select event handling loop would be changed to pass this list
of resource IDs to define the order in which events are enabled::

  while (1) {
    event_choice_t choice = select_wait_ordered(ids);
    switch (choice) {
    ...
    }
  }

Using interrupts
................

The library provides support for hardware interrupts from xCORE resources.

Interrupts can be raised by resources as an alternative to select events, and
will be vectored to the provided callback function.

As interrupts can occur at any point during program execution there are certain
requirements which must be adhered to ensure safe operation:

  #. Resources must not have interrupts enabled whilst being configured, or
     the core must have interrupts masked if the resource has already been
     configured to raise interrupts.

  #. The core must have interrupts masked when disabling interrupts for a
     resource.


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

.. doxygenfunction:: chan_out_word

.. doxygenfunction:: chan_out_byte

.. doxygenfunction:: chan_out_buf_word

.. doxygenfunction:: chan_out_buf_byte

.. doxygenfunction:: chan_in_word

.. doxygenfunction:: chan_in_byte

.. doxygenfunction:: chan_in_buf_word

.. doxygenfunction:: chan_in_buf_byte

|newpage|

Streaming channels
..................

.. doxygenfunction:: s_chan_alloc

.. doxygenfunction:: s_chan_free

.. doxygenfunction:: s_chan_out_ct

.. doxygenfunction:: s_chan_out_ct_end

.. doxygenfunction:: s_chan_check_ct

.. doxygenfunction:: s_chan_check_ct_end

.. doxygenfunction:: s_chan_out_word

.. doxygenfunction:: s_chan_out_byte

.. doxygenfunction:: s_chan_out_buf_word

.. doxygenfunction:: s_chan_out_buf_byte

.. doxygenfunction:: s_chan_in_word

.. doxygenfunction:: s_chan_in_byte

.. doxygenfunction:: s_chan_in_buf_word

.. doxygenfunction:: s_chan_in_buf_byte

|newpage|

Channels with transactions
..........................

.. doxygenfunction:: chan_init_transaction_master

.. doxygenfunction:: chan_init_transaction_slave

.. doxygenfunction:: chan_complete_transaction

.. doxygenfunction:: t_chan_out_word

.. doxygenfunction:: t_chan_out_byte

.. doxygenfunction:: t_chan_out_buf_word

.. doxygenfunction:: t_chan_out_buf_byte

.. doxygenfunction:: t_chan_in_word

.. doxygenfunction:: t_chan_in_byte

.. doxygenfunction:: t_chan_in_buf_word

.. doxygenfunction:: t_chan_in_buf_byte

|newpage|

Ports
.....

.. doxygenfunction:: port_alloc

.. doxygenfunction:: port_alloc_buffered

.. doxygenfunction:: port_free

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

.. doxygenfunction:: clock_alloc

.. doxygenfunction:: clock_free

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

|appendix|

Known Issues
------------

No known issues.

.. include:: ../../../CHANGELOG.rst

