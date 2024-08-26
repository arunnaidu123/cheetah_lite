"""
End to end tests of converting c++ to vhdl
"""
import os
import pytest
import difflib

from ska.c_vhdl_translator import translate

def cpp_files(path):
    l = os.listdir(path)
    return [path + i for i in l if os.path.splitext(i)[1]==".cpp"]

def fn_output(cpp_filename, package=None):
    name = os.path.splitext(os.path.basename(cpp_filename))[0]
    if package:
        name += "-" + package
    return "tests/output/" + name + ".vhd"

def fn_expected_output(cpp_filename, package=None):
    name = os.path.splitext(os.path.basename(cpp_filename))[0]
    if package:
        name += "-" + package
    return "tests/expected_output/" + name + ".vhd"

def translate_compare(infile, namespace=None, package=None):
    """ run the test for a given input file and namespace, return True if success """
    fn_out = fn_output(infile, package) # file name of our test output
    fn_exp = fn_expected_output(infile, package) # file name of expected output
    print("comparing:", fn_out, fn_exp)
    with open(infile, "r") as cpp_file:
        with open(fn_out, "w") as outfile:
            translate([cpp_file], outfile, namespace, package)
        with open(fn_out, "r") as result_file, \
          open(fn_exp, "r") as expected_result_file:
            result = result_file.read()
            expected_result = expected_result_file.read()
            retval = result == expected_result
            if not retval: #show difference, if any
                for line in difflib.context_diff(result, expected_result):
                    print(line)
            return retval

class TestGlobalNamespace:
    """ Tests that run on global C++ namespace """
    @pytest.mark.parametrize("infile", cpp_files("tests/cpp/global_ns/"))
    def test_gns(self, infile):
        """Simple input test - one input cpp / one output vhd"""
        assert translate_compare(infile)

class TestNamespace:
    """ Tests that run on a specified C++ namespace """
    @pytest.mark.parametrize("infile", cpp_files("tests/cpp/parser_stress_ns/"))
    def test_parser_stress(self, infile):
        """Test namespace parser_stress"""
        assert translate_compare(infile, "parser_stress")

class TestPackage:
    """ Test the package label output """
    @pytest.mark.parametrize("infile", cpp_files("tests/cpp/global_ns/"))
    def test_gns(self, infile):
        """Global namespace tests, with a package wrapper"""
        assert translate_compare(infile, None, "test_pkg_name")
    @pytest.mark.parametrize("infile", cpp_files("tests/cpp/parser_stress_ns/"))
    def test_parser_stress(self, infile):
        """Test namespace parser_stress, with a package wrapper"""
        assert translate_compare(infile, "parser_stress", "test_pkg_name")

