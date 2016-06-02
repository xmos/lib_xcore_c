xCORE C Library
===============

Overview
--------

A library providing a native C implementation of xCORE hardware features. This is
effectively a bare-metal programming environment for using the xCORE. It assumes
very good understanding of how the xCORE architecture works as it does not provide
the usual protection that xC does to prevent incorrect use of the hardware.

Features
........

 * Support for channel and streaming channels. This includes full interoperability
   with xC channels and the ability to write custom channel protocols
 * Support for ports and clock blocks
 * Support for timers
 * Support for events so that xC 'select' functionality can be implemented
 * Support for hardware locks

Software version and dependencies
.................................

.. libdeps::
