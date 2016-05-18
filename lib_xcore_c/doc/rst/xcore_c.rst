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

|newpage|

Using a timer
.............

.. doxygenfunction:: timer_alloc

.. doxygenfunction:: timer_free

.. doxygenfunction:: timer_get_time

.. doxygenfunction:: timer_delay

|newpage|

TODO the rest

|appendix|

Known Issues
------------

No known issues.

.. include:: ../../../CHANGELOG.rst

