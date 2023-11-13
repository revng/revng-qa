#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

import argparse
import dataclasses
import re
import sys
from abc import ABC, abstractmethod
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

    parser.add_argument(
        "section_dump", type=str, help="the file listing sections present in the binary"
    )

    parser.add_argument(
        "symbol_dump", type=str, help="the file listing symbols present in the binary"
    )

    parser.add_argument(
        "disassembly_dump",
        type=str,
        help="the file containing the outputs of a disassembler for the binary",
    )

    return parser.parse_args()


class DisassemblyParser(ABC):
    def __init__(self, sections, symbols, disassembly):
        self.sections = sections
        self.symbols = symbols
        self.disassembly = disassembly

    @abstractmethod
    def list_symbols(self):
        raise Exception("Use one of the children.")

    @abstractmethod
    def find_section(self, name):
        raise Exception("Use one of the children.")

    @abstractmethod
    def find_callsites(self, name):
        raise Exception("Use one of the children.")


class ObjdumpOutputParser(DisassemblyParser):
    def __init__(self, *args):
        super().__init__(*args)

    def list_symbols(self):
        result = {}
        for match in re.finditer(
            b"\n0+([a-fA-F0-9]+)\s+"  # address (without leading 0s)
            + b"(\w+)\s+"  # symbol type (local, global, etc)
            + b"(?:(\w+)\s+|)"  # optional extra flags (weak, debug, etc)
            + b"([\*\.\w]+)\s+"  # section symbol belongs to
            + b"([a-fA-F0-9]+)\s+"  # either alignment or size of the symbol
            + b"([\w\._-]+)+",  # name of the symbol
            self.symbols,
        ):
            result[match[6].decode("utf-8")] = match[1].decode("utf-8")

        return result

    def find_section(self, name):
        match = re.search(
            b"\n\s+\d+\s+"  # index
            + bytes(name, "utf-8")  # name
            + b"\s+([a-fA-F0-9]+)\s+"  # size
            + b"([a-fA-F0-9]+)\s+"  # vma
            + b"([a-fA-F0-9]+)\s+"  # lma
            + b"([a-fA-F0-9]+)\s+"  # offset within the file
            + b"\d+\*\*\d+\s*",  # (uncaptured) alignment
            self.sections,
        )
        if match:
            return Section(
                name,
                match[1].decode("utf-8"),  # size
                match[2].decode("utf-8"),  # vma
                match[4].decode("utf-8"),  # offset
            )
        else:
            raise Exception("unable to find a section: '" + name + "'")

    def find_callsites(self, name):
        result = []
        for match in re.finditer(
            b"\n\s+0*([a-fA-F0-9]+):"  # address (without leading 0s)
            + b"[^<\n]*<"  # any number of non-`<` characters followed by `<`
            + bytes(name, "utf-8")  # name
            + b">\n"  # `>` character and a new line
            + b"\s+0*([a-fA-F0-9]+)",  # address of the next instruction
            self.disassembly,
        ):
            result.append((match[1].decode("utf-8"), match[2].decode("utf-8")))

        return result


def select_parser(sections, symbols, disassembly):
    if re.match(b"^\n[^:]+:\s+file format elf.+\n", sections):
        return ObjdumpOutputParser(sections, symbols, disassembly)
    else:
        raise Exception("Unsupported input file format")


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

    with open(arguments.section_dump, "rb") as sections:
        with open(arguments.symbol_dump, "rb") as symbols:
            with open(arguments.disassembly_dump, "rb") as disassembly:
                parser = select_parser(sections.read(), symbols.read(), disassembly.read())

    result = Gathered()
    result.sections.append(parser.find_section(".text"))

    list_of_symbols_to_extract = arguments.list_of_symbols_to_extract.split()
    for name, address in parser.list_symbols().items():
        match = re.match("^(?:_|@|)([\w_]+)(?:_\d+|@+\d+|)\s*$", name)
        if match:
            if match[1] in list_of_symbols_to_extract:
                assert match[1] not in result.memory
                result.memory[match[1]] = address
            else:
                match = re.match("^(test|setup)_([\w_]+)$", match[1])
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

        for before, after in parser.find_callsites("test_" + name):
            function.callsites.append(CallSite(before, after))

        assert len(function.callsites) == 1

    result.emit()


if __name__ == "__main__":
    sys.exit(main())
