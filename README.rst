DEPRECATED: xCORE C Library
===========================

**This library is no longer supported. A system library with similar intent has been
added to the 15.0.x tools release and should be used in preference to this library.**

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
 * Support for select events so that xC 'select' functionality can be implemented
 * Support for interrupt events
 * Support for hardware locks

Software version and dependencies
.................................

.. libdeps::
