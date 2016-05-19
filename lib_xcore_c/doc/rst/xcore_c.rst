.. include:: ../../../README.rst

Usage
-----

All functions can be accessed via the ``xcore_c.h`` header::

  #include "xcore_c.h"

You will also have to add ``lib_xcore_c`` to the ``USED_MODULES`` field of
your application Makefile.

Using timers
............

TODO

Using channels
..............

TODO

Using ports and clock blocks
............................

TODO

Using hardware locks
....................

TODO

Using events
............

The library provides the ability to re-create the equivalent of the XC *select* statement.

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

.. doxygenfunction:: t_chan_check_ct

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

