#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

import argparse
import dataclasses
import re
import sys
from dataclasses import dataclass


def accept_arguments():
    parser = argparse.ArgumentParser(
        description="A quick disassembly analyzer that emits some addresses as a C header. "
        + "It expects a text file containing the list of symbols to extract as the first argument, "
        + "the GNU `objdump` output as stdin (will eventually be extended to support `dumpbin` "
        + "as well). It outputs a header to a stdout.",
    )

    parser.add_argument(
        "list_of_symbols_to_extract",
        type=str,
        help="the file listing the interesting symbols",
    )

    return parser.parse_args()


def has_objdump_header(data):
    return bool(re.search(b"^\n[^:]+:\s+file format elf.+\n", data))


def has_dumpbin_header(data):
    return bool(re.search(b"^\nDump of file ", data))


def list_symbols(data):
    result = {}
    for match in re.finditer(
        b"\n0+([a-fA-F0-9]+)\s+"  # address (without leading 0s)
        + b"(\w+)\s+"  # symbol type (local, global, etc)
        + b"(?:(\w+)\s+|)"  # optional extra flags (weak, debug, etc)
        + b"([\*\.\w]+)\s+"  # section symbol belongs to
        + b"([a-fA-F0-9]+)\s+"  # either alignment or size of the symbol
        + b"([\w\._-]+)+",  # name of the symbol
        data,
    ):
        result[match[6].decode("utf-8")] = match[1].decode("utf-8")

    return result


def find_section(name, data):
    match = re.search(
        b"\n\s+\d+\s+"  # index
        + bytes(name, "utf-8")  # name
        + b"\s+([a-fA-F0-9]+)\s+"  # size
        + b"([a-fA-F0-9]+)\s+"  # vma
        + b"([a-fA-F0-9]+)\s+"  # lma
        + b"([a-fA-F0-9]+)\s+"  # offset within the file
        + b"\d+\*\*\d+\s*",  # (uncaptured) alignment
        data,
    )
    if match:
        return (
            match[1].decode("utf-8"),  # size
            match[2].decode("utf-8"),  # vma
            match[4].decode("utf-8"),  # offset
        )
    else:
        raise Exception("unable to find a section: '" + name + "'")


def find_callsites(name, data):
    result = []
    for match in re.finditer(
        b"\n\s+0*([a-fA-F0-9]+):"  # address (without leading 0s)
        + b"[^<\n]*<"  # any number of non-`<` characters followed by `<`
        + bytes(name, "utf-8")  # name
        + b">\n"  # `>` character and a new line
        + b"\s+0*([a-fA-F0-9]+)",  # address of the next instruction
        data,
    ):
        result.append((match[1].decode("utf-8"), match[2].decode("utf-8")))

    return result


@dataclass
class Section:
    name: str = ""
    size: str = ""
    vma: str = ""
    file_offset: str = ""


@dataclass
class CallSite:
    before: str = ""
    after: str = ""


@dataclass
class Function:
    address: str = ""
    entry_point: str = ""
    callsites: list[CallSite] = dataclasses.field(default_factory=list[CallSite])


class Gathered:
    def __init__(self):
        self.sections: list[Section] = []
        self.memory: dict[str, str] = {}
        self.functions: dict[str, Function] = {}

    def emit(self):
        longest_section_name = 0
        longest_symbol_name = 0
        longest_function_name = 0
        maximum_callsite_count = 0

        for section in self.sections:
            if len(section.name) > longest_section_name:
                longest_section_name = len(section.name)
        for variable in self.memory:
            if len(variable) > longest_symbol_name:
                longest_symbol_name = len(variable)
        for name, function in self.functions.items():
            if len(name) > longest_function_name:
                longest_function_name = len(name)
            if len(function.callsites) > maximum_callsite_count:
                maximum_callsite_count = len(function.callsites)

        print(
            """
#include <stdint.h>

struct section {
  char name["""
            + str(longest_section_name + 1)
            + """];
  uint64_t size, vma, file_offset;
};
struct memory {
  char name["""
            + str(longest_symbol_name + 1)
            + """];
  uint64_t address;
};
struct callsite {
  uint64_t before, after;
};
struct function {
  char name["""
            + str(longest_function_name + 1)
            + """];
  uint64_t address;
  uint64_t entry_point;
  uint64_t callsite_count;
  struct callsite callsites["""
            + str(maximum_callsite_count)
            + """];
};

const const struct {
  struct section sections["""
            + str(len(self.sections))
            + """];
  struct memory variables["""
            + str(len(self.memory))
            + """];
  struct function functions["""
            + str(len(self.functions))
            + """];
} input = {
  .sections = { """,
            end="",
        )

        for section in self.sections:
            print(
                """{
    .name = \""""
                + section.name
                + """\",
    .size = 0x"""
                + section.size
                + """,
    .vma = 0x"""
                + section.vma
                + """,
    .file_offset = 0x"""
                + section.file_offset
                + """
  }, """,
                end="",
            )

        print(
            """},
  .variables = { """,
            end="",
        )

        for name, address in self.memory.items():
            print(
                """{
    .name = \""""
                + name
                + """\",
    .address = 0x"""
                + address
                + """
  }, """,
                end="",
            )

        print(
            """},
  .functions = { """,
            end="",
        )

        for name, function in self.functions.items():
            print(
                """{
    .name = \""""
                + name
                + """\",
    .address = 0x"""
                + function.address
                + """,
    .entry_point = 0x"""
                + function.entry_point
                + """,
    .callsite_count = """
                + str(len(function.callsites))
                + """,
    .callsites = { """,
                end="",
            )

            for callsite in function.callsites:
                print(
                    """{
      .before = 0x"""
                    + callsite.before
                    + """,
      .after = 0x"""
                    + callsite.after
                    + """
    } """,
                    end="",
                )

            print(
                """}
  }, """,
                end="",
            )

        print(
            """},
};"""
        )


def main():
    arguments = accept_arguments()

    result = Gathered()
    disassembly = sys.stdin.buffer.read()
    if has_objdump_header(disassembly):
        size, vma, file_offset = find_section(".text", disassembly)
        result.sections.append(Section(".text", size, vma, file_offset))

        list_of_symbols_to_extract = arguments.list_of_symbols_to_extract.split()
        for name, address in list_symbols(disassembly).items():
            if name in list_of_symbols_to_extract:
                assert name not in result.memory
                result.memory[name] = address
            else:
                match = re.match("^(test|setup)_([\w_]+)\s*$", name)
                if match:
                    if match[2] not in result.functions:
                        result.functions[match[2]] = Function()
                    if match[1] == "test":
                        assert result.functions[match[2]].address == ""
                        result.functions[match[2]].address = address
                    elif match[1] == "setup":
                        assert result.functions[match[2]].entry_point == ""
                        result.functions[match[2]].entry_point = address

        assert len(list_of_symbols_to_extract) == len(result.memory)
        assert len(result.functions) > 0

        for name, function in result.functions.items():
            assert function.address != ""
            assert function.entry_point != ""

            for before, after in find_callsites("test_" + name, disassembly):
                function.callsites.append(CallSite(before, after))

    elif has_dumpbin_header(disassembly):
        raise Exception("TODO")

    else:
        raise Exception("Invalid input file")

    result.emit()


if __name__ == "__main__":
    sys.exit(main())
