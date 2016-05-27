#!/usr/bin/env python
import xmostest
import os
import os.path
import re
from pycparser import c_parser, c_ast, parse_file

file_top = """\
#include "xcore_c.h"

void test()
{
"""

file_bottom = """\
}
"""

class LibraryFunctionExtractor(c_ast.NodeVisitor):
    def __init__(self):
        self.function_call_blocks = []

    def get_function_call_blocks(self, node):
        self.visit(node)
        return self.function_call_blocks

    def visit_FuncDef(self, node):
        current_path = '{}'.format(node.decl.coord)
        if re.match(os.path.join('..','lib_xcore_c','api','')+'\w+\.h\:\d+',
                    current_path) and '_internal.h' not in current_path:
            args = []
            function_params = []
            for params in node.decl.type.args.params:
                # Assign parameter name
                pname = params.name ###ParamList/Decl
                if pname is None:
                    pname = ''
                # Parameter is a pointer type of some kind
                if type(params.type) is c_ast.PtrDecl:
                    # Parameter is a pointer to a pointer type - double indirection
                    if type(params.type.type) is c_ast.PtrDecl:
                        ptype = params.type.type.type.type.names ###Decl/PtrDecl/PtrDecl/TypeDecl/IdentifierType
                    # There is no double indirection
                    else:
                        # Parameter is a pointer to a function type
                        if type(params.type.type) is c_ast.FuncDecl:
                            pname = str(params.type.type.type.type.names) + ' (*' ###Decl/PtrDecl/TypeDecl/IdentifierType
                            pname = pname + params.type.type.type.declname + ')' ###Decl/PtrDecl/FuncDecl/TypeDecl
                            ptype = ''
                            for subparams in params.type.type.args.params: ###Decl/PtrDecl/FuncDecl/ParamList
                                ptype = ptype + str(subparams.type.type.type.names) ###Typename/PtrDecl/TypeDecl/IdentifierType
                        # Parameter is a pointer type - single indirection
                        else:
                            ptype = params.type.type.type.names ###Decl/PtrDecl/TypeDecl/IdentifierType
                            ptype.append('*')
                # Parameter is a variable
                elif type(params.type.type) is c_ast.IdentifierType:
                    ptype = params.type.type.names
                    if ''.join(ptype) == 'void':
                        ptype = ''
                # Parameter is an array
                elif type(params.type) is c_ast.ArrayDecl:
                    ptype = params.type.type.type.names
                    pname = pname + '[5]'

                args.append('{} {}; '.format(' '.join(ptype), pname))
                function_params.append('{}, '.format(pname.rstrip('[5]')))

            function_call = '{}({});\n'.format(node.decl.name, ''.join(function_params).rstrip(', '))
            func_block = '  {{\n    {}\n    {}  }}\n'.format(''.join(args).rstrip(', '), function_call)
            self.function_call_blocks.append(func_block)

def generate_test_app():
    test_app_dir = os.path.join('build_with_no_inline', 'src')
    c_file = os.path.join(test_app_dir, 'pycparser_main.c')
    ast = parse_file(c_file,
                     use_cpp=True,
                     cpp_path=os.path.join(os.environ.get('XMOS_TOOL_PATH'),
                                           'bin', 'xcc'),
                     cpp_args=['-E',
                               '{}{}'.format('-I',
                                             os.path.join('..','lib_xcore_c','api'))
                              ]
                    )

    functions = LibraryFunctionExtractor()
    function_call_blocks = functions.get_function_call_blocks(ast)

    with open(os.path.join(test_app_dir, 'test.c'), "w") as test_c_file:
        test_c_file.writelines(file_top)
        test_c_file.writelines(function_call_blocks)
        test_c_file.writelines(file_bottom)

def run(arch):
    test_name = os.path.splitext(os.path.basename(__file__).replace("test_", "", 1))[0]

    resources = xmostest.request_resource("xsim")

    binary = '{}/bin/{}/{}_{}.xe'.format(test_name, arch, test_name, arch)

    tester = xmostest.ComparisonTester(open('{}.expect'.format(test_name)),
                                     'lib_xcore_c', 'xcore_c_tests',
                                     "{}_{}".format(test_name, arch))

    tester.set_min_testlevel("smoke")

    xmostest.run_on_simulator(resources['xsim'], binary,
                              simthreads=[],
                              tester=tester,
                              simargs=['--xscope', '-offline xscope.xmt'])

def runtest():
    generate_test_app()
    run("XS1")
    run("XS2")
