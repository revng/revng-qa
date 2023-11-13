#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

import argparse
import os
import sys

import jinja2
import yaml


def accept_arguments():
    parser = argparse.ArgumentParser(
        description="A code generator to help making abi test artifacts"
    )

    parser.add_argument(
        "template_directory",
        type=str,
        help="Specifies the directory where the templates are located",
    )
    parser.add_argument(
        "config_directory",
        type=str,
        help="Specifies the directory where the configs are located",
    )
    parser.add_argument(
        "output_directory",
        type=str,
        help="Specifies the directory where to output the results to",
    )

    return parser.parse_args()


def load_configs(arguments):
    with open(str(arguments.config_directory) + "/architectures.yml", "r") as file:
        architectures = yaml.safe_load(file)
    with open(str(arguments.config_directory) + "/common.yml", "r") as file:
        config = yaml.safe_load(file)
    with open(str(arguments.config_directory) + "/functions.yml", "r") as file:
        functions = yaml.safe_load(file)
    return (architectures, config, functions)


def setup_jinja_environment(arguments):
    jinja_loader = jinja2.FileSystemLoader(str(arguments.template_directory))
    return jinja2.Environment(
        block_start_string="/*{",
        block_end_string="}*/",
        variable_start_string="/*(",
        variable_end_string=")*/",
        comment_start_string="/*#",
        comment_end_string="#*/",
        loader=jinja_loader,
    )


def render_helper(env, template_name: str, result_path: str, variable_dictionary={}):
    jinja_template = env.get_template(template_name + ".jinja_template")

    os.makedirs(os.path.dirname(result_path), exist_ok=True)
    with open(result_path, "w") as file:
        file.write(jinja_template.render(variable_dictionary))


def register_type(register_size: str):
    if register_size == 1:
        return "uint8_t"
    elif register_size == 2:
        return "uint16_t"
    elif register_size == 4:
        return "uint32_t"
    elif register_size == 8:
        return "uint64_t"
    else:
        raise Exception("unsupported register size")


def intel_register_type(register_size: str):
    if register_size == 1:
        return "byte"
    elif register_size == 2:
        return "word"
    elif register_size == 4:
        return "dword"
    elif register_size == 8:
        return "qword"
    else:
        raise Exception("unsupported register size")


def load_a_register(architecture, location, offset, register):
    local_dictionary = {
        "location": location,
        "offset": offset,
        "register": register,
    }

    jinja_template = jinja2.Environment().from_string(architecture["templates"]["load_a_register"])
    return jinja_template.render(local_dictionary)


def setup_registers(architecture):
    result = architecture["comment"] + " fill the registers with random data\n"
    register_count = len(architecture["register_list"])
    for index, register in enumerate(reversed(architecture["register_list"])):
        location = "expected_state"
        offset = (register_count - index - 1) * architecture["register_size"]
        current = load_a_register(architecture, location, offset, register)
        result = result + current + "\n"
    return result + "\n"


def setup_stack(architecture, config):
    register_size = architecture["register_size"]
    last_offset = (len(architecture["register_list"]) - 1) * register_size
    first_offset = last_offset + config["stack_byte_count"]

    result = architecture["comment"] + " fill the stack with random data\n"

    # Some architectures only support pushing registers onto the stack in pairs.
    # To work around that limitation, this introduces the `load_two_registers`
    # option for the templates to utilize: it lets two registers to be loaded
    # at once, but has the limitation of using up twice the space - hence we
    # need to limit the pushes, which is what the `flip_flop` flag is for:
    # the registers are only pushed when it's set, which happens on every second
    # iteration.
    flip_flop = True
    for offset in range(first_offset, last_offset, -register_size):
        stack_helper_dictionary = {
            "register": architecture["register_list"][0],
            "second_register": architecture["register_list"][1],
            "load_a_register": load_a_register(
                architecture,
                "expected_state",
                offset,
                architecture["register_list"][0],
            ),
        }
        if flip_flop:
            stack_helper_dictionary["load_two_registers"] = (
                load_a_register(
                    architecture,
                    "expected_state",
                    offset,
                    architecture["register_list"][0],
                )
                + "\n"
                + load_a_register(
                    architecture,
                    "expected_state",
                    offset - register_size,
                    architecture["register_list"][1],
                )
            )
        else:
            stack_helper_dictionary["load_two_registers"] = ""
        jinja_template = jinja2.Environment().from_string(
            architecture["templates"]["push_to_stack"]
        )
        result = result + jinja_template.render(stack_helper_dictionary) + "\n"

        flip_flop = not flip_flop

    return result + "\n"


def save_return_address(architecture):
    local_dictionary = {
        "location": "saved_return_address",
        "register": architecture["register_list"][0],
    }

    jinja_template = jinja2.Environment().from_string(
        architecture["comment"]
        + " save the return address\n"
        + architecture["templates"]["save_return_address"]
        + "\n\n"
    )
    return jinja_template.render(local_dictionary)


def restore_return_address(architecture, config):
    local_dictionary = {
        "location": "saved_return_address",
        "register": architecture["register_list"][0],
    }

    jinja_template = jinja2.Environment().from_string(
        architecture["comment"]
        + " restore the return address\n"
        + architecture["templates"]["restore_return_address"]
        + "\n\n"
    )
    return jinja_template.render(local_dictionary)


def setup_prologue(architecture, config):
    return (
        save_return_address(architecture)
        + setup_stack(architecture, config)
        + setup_registers(architecture)
    )


def call_a_function(architecture, name: str):
    jinja_template = jinja2.Environment().from_string(architecture["templates"]["call_a_function"])
    return jinja_template.render({"function_name": name}) + "\n\n"


def restore_stack(architecture, config):
    jinja_template = jinja2.Environment().from_string(architecture["templates"]["restore_stack"])
    return jinja_template.render({"stack_size": config["stack_byte_count"]}) + "\n\n"


def return_from_function(architecture, config):
    result = restore_return_address(architecture, config)

    jinja_template = jinja2.Environment().from_string(
        architecture["templates"]["return_from_function"]
    )
    return result + jinja_template.render({"stack_size": config["stack_byte_count"]})


def asm(text: str):
    return "\n  ".join(text.splitlines())


def get_generation_notice():
    return """/* This file was autogenerated. DO NOT MODIFY!
 * It is distributed under the MIT License. See LICENSE.md for details.
 */"""


def get_masm_style_generation_notice():
    return """;; This file was autogenerated. DO NOT MODIFY!
;; It is distributed under the MIT License. See LICENSE.md for details.
"""


def render_functions(jinja_environment, config, functions, out_dir: str):
    dictionary = {
        "generation_notice": get_generation_notice(),
        "maximum_argument_count": config["maximum_argument_count"],
        "maximum_argument_size": config["maximum_argument_size"],
        "structs": functions["structs"],
        "packed_structs": functions["packed_structs"],
        "argument_functions": functions["argument_tests"],
        "return_value_functions": functions["return_value_tests"],
        "supported_primitives": [
            "int8_t",
            "int16_t",
            "int32_t",
            "int64_t",
            "uint8_t",
            "uint16_t",
            "uint32_t",
            "uint64_t",
        ],
    }

    filename = "functions.h"
    result_path = out_dir + "/" + filename
    render_helper(jinja_environment, filename, result_path, dictionary)

    filename = "types.h"
    result_path = out_dir + "/" + filename
    render_helper(jinja_environment, filename, result_path, dictionary)

    filename = "functions.inc"
    result_path = out_dir + "/" + filename
    render_helper(jinja_environment, filename, result_path, dictionary)


def render_function_description(jinja_env, architectures, config, functions, out_dir):
    for architecture_name, architecture in architectures.items():
        dictionary = {
            "generation_notice": get_generation_notice(),
            "architecture_name": architecture_name,
            "stack_byte_count": config["stack_byte_count"],
            "lfsr_seed": config["lfsr_seed"],
            "iteration_count": config["iteration_count"],
            "maximum_argument_count": config["maximum_argument_count"],
            "maximum_argument_size": config["maximum_argument_size"],
            "register_type": register_type(architecture["register_size"]),
            "intel_register_type": intel_register_type(architecture["register_size"]),
            "register_size": architecture["register_size"],
            "register_list": architecture["register_list"],
            "register_count": len(architecture["register_list"]),
            "stack_pointer": architecture["stack_pointer"],
            "argument_functions": functions["argument_tests"],
            "return_value_functions": functions["return_value_tests"],
            "setup_prologue": asm(setup_prologue(architecture, config)),
            "call_a_function": lambda n, a=architecture: asm(call_a_function(a, n)),
            "restore_stack": asm(restore_stack(architecture, config)),
            "return_from_function": asm(return_from_function(architecture, config)),
        }

        combined_registers = dictionary["register_count"] * dictionary["register_size"]
        dictionary["generated_byte_count"] = dictionary["stack_byte_count"] + combined_registers
        assert dictionary["generated_byte_count"] % dictionary["register_size"] == 0

        function_count = len(functions["argument_tests"]) + len(functions["return_value_tests"])
        dictionary["function_count"] = function_count

        filename = "constants.h"
        path = out_dir + "/" + architecture_name + "/" + filename
        render_helper(jinja_env, filename, path, dictionary)

        filename = "decoders.inc"
        path = out_dir + "/" + architecture_name + "/" + filename
        render_helper(jinja_env, filename, path, dictionary)

        filename = "setup_return_values.inc"
        path = out_dir + "/" + architecture_name + "/" + filename
        render_helper(jinja_env, filename, path, dictionary)

        for current_style in architecture["styles"]:
            style_specific_dictionary = dictionary
            style_specific_dictionary["current_style"] = current_style

            if current_style == "msvc":
                style_specific_dictionary["generation_notice"] = get_masm_style_generation_notice()

            filename = "setup_arguments.S"
            path = out_dir + "/" + architecture_name + "/" + current_style + "/" + filename
            render_helper(jinja_env, filename, path, style_specific_dictionary)


def main():
    arguments = accept_arguments()
    architectures, config, functions = load_configs(arguments)

    environment = setup_jinja_environment(arguments)

    out_dir = str(arguments.output_directory)
    render_functions(environment, config, functions, out_dir)
    render_function_description(environment, architectures, config, functions, out_dir)

    return 0


if __name__ == "__main__":
    sys.exit(main())
