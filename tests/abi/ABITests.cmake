#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

macro(register_abi_artifacts ABI ARCHITECTURE)

  register_artifact_category("abi_test_function_library_${ABI}" "ON")
  register_artifact(
    "abi_test_function_library_${ABI}" "abi_test_function_library"
    "${ARCHITECTURE}" "abi_test_functions.c")
  register_artifact_run("abi_test_function_library_${ABI}"
                        "abi_test_function_library" "default" "nope")

  register_artifact_category("describe_abi_test_functions_${ABI}" "ON")
  register_artifact(
    "describe_abi_test_functions_${ABI}" "describe_abi_test_functions"
    "${ARCHITECTURE}" "describe_abi_test_functions.c")
  register_artifact_run("describe_abi_test_functions_${ABI}"
                        "describe_abi_test_functions" "default" "nope")

endmacro()

register_abi_artifacts("SystemV_x86_64" "x86_64")
register_abi_artifacts("SystemV_x86" "x86")
# ...
register_abi_artifacts("Microsoft_x86_stdcall" "x86")
# ...
