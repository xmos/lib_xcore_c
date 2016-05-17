#!/usr/bin/env python
import xmostest
import os

def run(arch):
    # Derive test name from the python script name with out leading "test_" or
    # the trailing ".py"
    test_name = os.path.basename(__file__)[5:-3]

    resources = xmostest.request_resource("xsim")

    binary = '{}/bin/{}/{}_{}.xe'.format(test_name, arch, test_name, arch)

    tester = xmostest.ComparisonTester(open('{}.expect'.format(test_name)),
                                     'lib_xcore_c', 'xcore_c_tests',
                                     "{}_{}".format(test_name, arch), regexp=True)

    tester.set_min_testlevel("smoke")

    xmostest.run_on_simulator(resources['xsim'], binary,
                              simthreads=[],
                              tester=tester,
                              simargs=['--xscope', '-offline xscope.xmt'],
                              loopback=[{'from': 'tile[0]:XS1_PORT_1A',
                                         'to': 'tile[0]:XS1_PORT_1B'},
                                        {'from': 'tile[1]:XS1_PORT_1A',
                                         'to': 'tile[1]:XS1_PORT_1B'}])

def runtest():
    run("XS1")
    run("XS2")