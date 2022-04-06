#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

# Generate the test sources
if(IN_REVNG_QA)
  add_custom_target(
    generated_abi_artifact_sources
    COMMAND
      "python3"
      "${CMAKE_SOURCE_DIR}/scripts/abi_artifact_generator/generate.py"
      "${CMAKE_SOURCE_DIR}/scripts/abi_artifact_generator/templates/"
      "${CMAKE_SOURCE_DIR}/scripts/abi_artifact_generator/config/"
      "${CMAKE_BINARY_DIR}/include/revng-qa/generated/")
elseif("${CMAKE_PROJECT_NAME}" STREQUAL "revng")
  # Define a target that does nothing
  add_custom_target(generated_abi_artifact_sources)
else()
  # If you run into problems with `generated_abi_artifact_sources` target
  # in projects depending on revng, please report those.
endif()

# Define a macro to simplify multiple architecture artifact creation
macro(register_abi_artifacts ABI ARCHITECTURE)

  register_artifact_category("abi_test_function_library_${ABI}" "ON")
  register_artifact(
    "abi_test_function_library_${ABI}" "abi_test_function_library"
    "${ARCHITECTURE}" "source/abi_test_function_library.c")
  register_artifact_run("abi_test_function_library_${ABI}"
                        "abi_test_function_library" "default" "nope")

  register_artifact_category("describe_abi_test_functions_${ABI}" "ON")
  register_artifact(
    "describe_abi_test_functions_${ABI}" "describe_abi_test_functions"
    "${ARCHITECTURE}" "source/describe_functions.c")
  register_artifact_run("describe_abi_test_functions_${ABI}"
                        "describe_abi_test_functions" "default" "nope")

endmacro()

# Register all the artifacts
register_abi_artifacts("SystemV_x86_64" "x86_64")
# TODO: implement more of these
