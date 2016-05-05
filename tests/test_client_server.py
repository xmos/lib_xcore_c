#!/usr/bin/env python
import xmostest
import os

def runtest():
    resources = xmostest.request_resource("xsim")

    binary = 'client_server/bin/client_server.xe'

    tester = xmostest.ComparisonTester(open('client_server.expect'),
                                     'lib_xcore_c', 'xcore_c_tests',
                                     'client_server')

    tester.set_min_testlevel("smoke")

    xmostest.run_on_simulator(resources['xsim'], binary,
                              simthreads=[],
                              tester=tester,
                              simargs=['--xscope', '-offline xscope.xmt'])
