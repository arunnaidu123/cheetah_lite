"""
Read C++ file, extract struct definitions into internal data structure.
"""

import re

from pygccxml import declarations
from pygccxml import parser
from pygccxml import utils

from .data_types import CppStruct, CppScalar, CppArray


def bits_from_ctype(t):
    """ Take a C++ type object and return an integer number of how many bits it uses """
    b = int(t.byte_size)
    if b:
        return b * 8
    else:
        # if byte_size doesn't work, extract the numeric portion of the type name
        # (e.g. uint16_t => 16 bits)
        # regex looks for [:...][u]int<numbers>_t and extracts the numeric part
        rx = re.compile(":*u?int([0-9]+)_t")
        n = rx.match(t.decl_string)
        if n:
            return int(n.groups()[0])
        errmsg = "Can't determine number of bits for {}".format(t.decl_string)
        raise Exception(errmsg)
    return 0


def read_cpp(infiles, namespace):
    """
    Parse C++ input and extract information about struct definitions.
    Arguments:
        infiles - a list of file objects
    Returns a list of CppStructs.
    """
    generator_path, generator_name = utils.find_xml_generator()
    xml_generator_config = parser.xml_generator_configuration_t(
        xml_generator_path=generator_path, xml_generator=generator_name
    )

    # it seems strange to extract the filename from the already-open file,
    # but argparse has much better error handling than the cpp parser...
    filenames = [f.name for f in infiles]
    dec = parser.parse(filenames, xml_generator_config)
    gns = declarations.get_global_namespace(dec)
    if namespace:
        ns = gns.namespace(namespace)
    else:
        ns = gns
    cpp = []  # list of CppStructs to return

    # list of class objects representing structs
    # (filter out compiler-generated with double-undscore prefix)
    cpp_structs = [
        d
        for d in ns.decls()
        if isinstance(d, declarations.class_declaration.class_t)
        and d.class_type == "struct"
        and d.name[:2] != "__"
    ]
    # string names of structs (note: without namespace)
    cpp_struct_names = [n.name for n in cpp_structs]
    for d in cpp_structs:
        items = []  # list of variables within this struct
        for m in d.get_members():
            if isinstance(m, declarations.variable.variable_t):
                # the parser gives us array sizes as a number
                # would be nicer if we got the variable name too...
                # is it an array?
                if isinstance(m.decl_type, declarations.cpptypes.array_t):
                    # strip namespace from type
                    #
                    # WARNING - this might create collisions in pathological cases
                    # if req'd, could replace :: with _ and use the whole name
                    # - also change cpp_struct_names generator in this case
                    #
                    m_type = str(m.decl_type.base).split("::")[-1]
                    if m_type in cpp_struct_names:
                        items.append(CppArray(m.name, m_type, m.decl_type.size))
                    else:
                        bits = bits_from_ctype(m.decl_type.base)
                        items.append(CppArray(m.name, bits, m.decl_type.size))
                else:
                    # assume scalar
                    m_type = str(m.decl_type).split("::")[-1]
                    if m_type in cpp_struct_names:
                        items.append(CppScalar(m.name, m_type))
                    else:
                        # m.bits only works if code explicity sets a bit-width
                        # (i.e. for bit field variables)
                        bits = m.bits if m.bits else bits_from_ctype(m.decl_type)
                        items.append(CppScalar(m.name, bits))
            else:
                pass
        # we should have collected all items within this struct now
        cpp.append(CppStruct(d.name, items))
    return cpp
