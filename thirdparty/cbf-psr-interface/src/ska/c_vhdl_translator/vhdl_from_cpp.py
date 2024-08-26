"""
Translate C++ structs into a VHDL records.
"""

from .data_types import VhdlStruct, VhdlScalar, VhdlArray

STD_LOGIC_VECTOR = "STD_LOGIC_VECTOR({item_type}-1 DOWNTO 0)"


def vhdl_struct_from_cpp_struct(cpp_struct):
    """
    Convert a CppStruct into a VhdlStruct
    (custom representations used inside this package)
    """
    # As far as I know, only arrays need any noteworthy transformation.
    # The infrastructure allows for this understanding to be refined later if needed.
    vhdl_items = []
    # process arrays (we write a typedef for arrays in VHDL)
    for i in cpp_struct.item_list:
        d = i._asdict()  # convert to dictionary
        if isinstance(d["item_type"], int):
            d["item_type"] = STD_LOGIC_VECTOR.format(**d)

        if "len_array" in d:
            # use the struct name in the typedef to avoid risk of collisions
            d["array_type_name"] = "t_{}_{}_arr".format(
                cpp_struct.struct_name, i.item_name
            )
            vhdl_items.append(VhdlArray(**d))
        else:
            vhdl_items.append(VhdlScalar(**d))
    return VhdlStruct(cpp_struct.struct_name, vhdl_items)


def vhdl_from_cpp(cpp):
    """ Convert a list of CppStructs into a list of VhdlStructs """
    vhdl = []
    for cpp_struct in cpp:
        vhdl.append(vhdl_struct_from_cpp_struct(cpp_struct))
    return vhdl
