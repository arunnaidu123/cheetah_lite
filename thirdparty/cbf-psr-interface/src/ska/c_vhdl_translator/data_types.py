"""
Custom Data Types
"""

import collections

CppStruct = collections.namedtuple("CppStruct", ["struct_name", "item_list"])
VhdlStruct = CppStruct

# for CppScalar, item_type is either an integer number of bits, or string name of a custom type
CppScalar = collections.namedtuple("CppScalar", ["item_name", "item_type"])
VhdlScalar = CppScalar  # item_type for VHDL is a string

CppArray = collections.namedtuple("CppArray", ["item_name", "item_type", "len_array"])
VhdlArray = collections.namedtuple(
    "VhdlArray", ["item_name", "item_type", "len_array", "array_type_name"]
)
