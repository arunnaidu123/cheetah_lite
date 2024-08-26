import os
import pytest

#@pytest.fixture(scope="class")
#def simple_cpp():
#    l = os.listdir("tests/cpp/global_ns")
#    return [i for i in l if i[-4:]==".cpp"]

@pytest.fixture
def stress_cpp():
    l = os.listdir("tests/cpp/parser_stress_ns")
    return [i for i in l if i[-4:]==".cpp"]
