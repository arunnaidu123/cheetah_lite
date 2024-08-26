"""
Translate C++ structs into a VHDL records.
"""

import argparse

from . import read_cpp
from . import vhdl_from_cpp
from . import write_vhdl


def translate(infiles, outfile, namespace=None, package=None):
    """
    translate c++ file into vhdl file
    infiles - list of TextIOWrappers to read from
    outfile - TextIOWrapper to write to
    namespace - str - optional namespace to read
    """

    cpp = read_cpp(infiles, namespace)
    vhdl = vhdl_from_cpp(cpp)
    write_vhdl(vhdl, outfile, package)


def main():
    """
    do command line argument parsing then call translate
    """
    # command-line argument parsing
    parser = argparse.ArgumentParser(description="C++ struct to VHDL translator")
    parser.add_argument("infile", nargs="+", type=argparse.FileType("r"))
    parser.add_argument(
        "--namespace", nargs="?", default=None, help="namespace to select from C++"
    )
    parser.add_argument(
        "--package", nargs="?", default=None, help="package name to write in VHDL"
    )
    parser.add_argument("outfile", nargs=1, type=argparse.FileType("w"))
    args = parser.parse_args()

    translate(
        args.infile, args.outfile[0], namespace=args.namespace, package=args.package
    )


if __name__ == "__main__":
    main()
