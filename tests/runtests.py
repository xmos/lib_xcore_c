#!/usr/bin/env python
import xmostest

if __name__ == "__main__":
    xmostest.init()

    xmostest.register_group("lib_xcore_c",
                            "xcore_c_tests",
                            "Tests of the xCORE C library",
    """ Tests of the xCORE C library.
    """)

    xmostest.runtests()

    xmostest.finish()
