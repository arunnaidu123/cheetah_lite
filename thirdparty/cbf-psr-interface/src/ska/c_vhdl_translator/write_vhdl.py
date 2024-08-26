"""
Translate C++ structs into a VHDL records.
"""

VHDL_HEADER = "LIBRARY ieee;\nUSE ieee.std_logic_1164.all;\n\n"
PKG_HEADER = "PACKAGE {pkg_name} IS\n\n"
TYPE_ARRAY = "TYPE {array_type_name} IS ARRAY(0 TO {len_array}-1) OF {item_type};\n"
REC_HEADER = "TYPE {struct_name} IS RECORD\n"
REC_SCALAR = "\t{item_name} : {item_type};\n"
REC_ARRAY = "\t{item_name} : {array_type_name};\n"
REC_FOOTER = "END RECORD;\n"
PKG_FOOTER = "END {pkg_name};\n"


def write_vhdl(vhdl, f, package=None):
    """
    Write VHDL file from list of VhdlStructs.
    Arguments:
        vhdl - list of VhdlStructs
        f - file object to write to
    """
    ###
    ### There is a possible optimization here -
    ### to search for duplicate array typedefs...
    ### this is awkward to do so I won't until we decide its needed.
    ###

    print("Writing VHDL to {}".format(f.name))
    f.write(VHDL_HEADER)
    if package:
        f.write(PKG_HEADER.format(pkg_name=package))

    # for each struct in list of structs
    for vs in vhdl:
        # write type definitions for arrays
        for a in [_ for _ in vs.item_list if "len_array" in _._fields]:
            f.write(TYPE_ARRAY.format(**a._asdict()))

        # write record header
        f.write(REC_HEADER.format(**vs._asdict()))
        # write record contents (one line per item, in order as per C++)
        for i in vs.item_list:
            if "len_array" in i._fields:
                f.write(REC_ARRAY.format(**i._asdict()))
            else:
                f.write(REC_SCALAR.format(**i._asdict()))
        f.write(REC_FOOTER)  # record footer
        f.write("\n")
        # don't print progress if user is sending VHDL to stdout
        if f.name != "<stdout>":
            print("\t{}".format(vs.struct_name))
    if package:
        f.write(PKG_FOOTER.format(pkg_name=package))
