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


def yaml_files_in(directory):
    """Returns the YAML files in `directory`, in a deterministic order.

    The directory is expected to only contain YAML and Markdown files.
    Anything else is a hard error.
    """

    result = []
    for name in sorted(os.listdir(str(directory))):
        path = os.path.join(str(directory), name)
        if os.path.isdir(path):
            continue

        extension = os.path.splitext(name)[1]
        if extension == ".yml":
            result.append(path)

        elif extension == ".md":
            continue  # ignore documentation

        else:
            raise Exception("`" + path + "` is neither a YAML nor a Markdown file")

    return result


def load_function_list(directory):
    """Loads every YAML file in `directory` and merges them together.

    The files are merged per top-level key. For example, the `argument_tests`
    in *all* the files form a single `argument_tests` map. The same holds for
    the other keys. Defining the same entry in two files is an error.
    """

    paths = yaml_files_in(directory)
    if not paths:
        raise Exception("no YAML files found in `" + str(directory) + "`")

    result = {}

    # Keep track of the entries we already loaded so we can point conflicting
    # tests out.
    key_origins = {}
    entry_origins = {}

    for path in paths:
        with open(path, "r") as file:
            document = yaml.safe_load(file)

        if document is None:
            continue

        if not isinstance(document, dict):
            raise Exception("`" + path + "` does not contain a map")

        for key, value in document.items():
            if key not in result:
                result[key] = value
                key_origins[key] = path
                if isinstance(value, dict):
                    for entry in value:
                        entry_origins[(key, entry)] = path
                continue

            if not isinstance(result[key], dict) or not isinstance(value, dict):
                raise Exception(
                    "`"
                    + key
                    + "` is defined in both `"
                    + key_origins[key]
                    + "` and `"
                    + path
                    + "`, and it is not "
                    "a map, so the two definitions cannot be merged."
                )

            for entry, entry_value in value.items():
                if entry in result[key]:
                    raise Exception(
                        "`"
                        + key
                        + "."
                        + entry
                        + "` is defined in both `"
                        + entry_origins[(key, entry)]
                        + "` and `"
                        + path
                        + "`, which would generate two functions with the "
                        "same name."
                    )
                result[key][entry] = entry_value
                entry_origins[(key, entry)] = path

    return result


def load_configs(arguments):
    config_directory = str(arguments.config_directory)

    # The configuration directory is expected to *only* contain YAML files
    # and the documentation. Anything else is a hard error.
    yaml_files = yaml_files_in(config_directory)
    if len(yaml_files) != 2:
        raise Exception(
            "Expected exactly two configuration files: `architectures.yml` and `common.yml`, instead found: "
            + str(yaml_files)
        )

    with open(os.path.join(config_directory, "architectures.yml"), "r") as file:
        architectures = yaml.safe_load(file)
    with open(os.path.join(config_directory, "common.yml"), "r") as file:
        config = yaml.safe_load(file)

    functions = load_function_list(os.path.join(config_directory, "functions"))

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


def gpr_type(gpr_size: str):
    if gpr_size == 1:
        return "uint8_t"
    elif gpr_size == 2:
        return "uint16_t"
    elif gpr_size == 4:
        return "uint32_t"
    elif gpr_size == 8:
        return "uint64_t"
    else:
        raise Exception("unsupported register size")


def all_the_argument_functions(functions):
    result = dict(functions["argument_tests"])
    result.update(stack_argument_tests(functions))
    return result


def stack_argument_tests(functions):
    prefix_type = functions["stack_argument_test_prefix_type"]
    prefix_count = functions["stack_argument_test_prefix_count"]
    prefix = [prefix_type] * prefix_count

    result = {}
    for name, arguments in functions["stack_argument_tests"].items():
        if name in functions["argument_tests"]:
            raise Exception(
                "`" + name + "` is present in both `argument_tests` and "
                "`stack_argument_tests`, which would generate two functions "
                "with the same name."
            )
        result[name] = prefix + list(arguments)

    return result


def get_generation_notice():
    return """/* This file was autogenerated. DO NOT MODIFY!
 * It is distributed under the MIT License. See LICENSE.md for details.
 */"""


def register_lists(architecture):
    lists = []
    offset = 0
    for entry in architecture:
        count = len(entry["register_list"])
        size = entry["register_size"]
        lists.append(
            {
                "group": entry["group"],
                "register_list": entry["register_list"],
                "count": count,
                "register_size": size,
                "offset": offset,
            }
        )
        offset += count * size

    return {
        "lists": lists,
        # Size of the whole register block, i.e. every list concatenated.
        # The stack snapshot follows immediately after it.
        "total_bytes": offset,
    }


def render_functions(jinja_environment, config, functions, out_dir: str):
    dictionary = {
        "generation_notice": get_generation_notice(),
        "maximum_argument_count": config["maximum_argument_count"],
        "maximum_argument_size": config["maximum_argument_size"],
        "structs": functions["structs"],
        "packed_structs": functions["packed_structs"],
        "unions": functions["unions"],
        "argument_functions": all_the_argument_functions(functions),
        "return_value_functions": functions["return_value_tests"],
    }

    filename = "functions.h"
    result_path = out_dir + "/" + filename
    render_helper(jinja_environment, filename, result_path, dictionary)

    filename = "functions.inc"
    result_path = out_dir + "/" + filename
    render_helper(jinja_environment, filename, result_path, dictionary)


def render_function_description(jinja_env, architectures, config, functions, out_dir):
    for architecture_name, architecture in architectures.items():
        gpr_size = architecture[0]["register_size"]
        dictionary = {
            "generation_notice": get_generation_notice(),
            "stack_byte_count": config["stack_byte_count"],
            "lfsr_seed": config["lfsr_seed"],
            "iteration_count": config["iteration_count"],
            "maximum_argument_count": config["maximum_argument_count"],
            "maximum_argument_size": config["maximum_argument_size"],
            "gpr_type": gpr_type(gpr_size),
            "gpr_size": gpr_size,
            "argument_functions": all_the_argument_functions(functions),
            "return_value_functions": functions["return_value_tests"],
        }

        dictionary["generated_byte_count"] = (
            dictionary["stack_byte_count"] + register_lists(architecture)["total_bytes"]
        )
        assert dictionary["generated_byte_count"] % dictionary["gpr_size"] == 0

        argument_test_count = len(all_the_argument_functions(functions))
        return_value_test_count = len(functions["return_value_tests"])
        dictionary["function_count"] = argument_test_count + return_value_test_count

        filename = "constants.h"
        path = out_dir + "/" + architecture_name + "/" + filename
        render_helper(jinja_env, filename, path, dictionary)

        filename = "decoders.inc"
        path = out_dir + "/" + architecture_name + "/" + filename
        render_helper(jinja_env, filename, path, dictionary)

        filename = "setup.inc"
        path = out_dir + "/" + architecture_name + "/" + filename
        render_helper(jinja_env, filename, path, dictionary)


def render_registers(jinja_env, architectures, out_dir: str):
    lists_per_architecture = {}
    for arch_name, architecture in architectures.items():
        lists_per_architecture[arch_name] = register_lists(architecture)

    dictionary = {
        "generation_notice": get_generation_notice(),
        "architectures": lists_per_architecture,
    }

    filename = "registers.h"
    path = out_dir + "/" + filename
    render_helper(jinja_env, filename, path, dictionary)


def main():
    arguments = accept_arguments()
    architectures, config, functions = load_configs(arguments)

    environment = setup_jinja_environment(arguments)

    out_dir = str(arguments.output_directory)
    render_functions(environment, config, functions, out_dir)
    render_function_description(environment, architectures, config, functions, out_dir)
    render_registers(environment, architectures, out_dir)

    return 0


if __name__ == "__main__":
    sys.exit(main())
