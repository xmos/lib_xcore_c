#!/usr/bin/env python
import xmostest
import os

def runtest():
    test_name = "select_port" 

    resources = xmostest.request_resource("xsim")

    binary = '{}/bin/{}.xe'.format(test_name, test_name)

    tester = xmostest.ComparisonTester(open('{}.expect'.format(test_name)),
                                     'lib_xcore_c', 'xcore_c_tests',
                                     test_name)

    tester.set_min_testlevel("smoke")

    xmostest.run_on_simulator(resources['xsim'], binary,
                              simthreads=[],
                              tester=tester,
                              simargs=['--xscope', '-offline xscope.xmt'],
                              loopback=[{'from': 'tile[0]:XS1_PORT_1A',
                                         'to': 'tile[0]:XS1_PORT_1B'}])
