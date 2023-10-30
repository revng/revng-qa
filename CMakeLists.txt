cmake_minimum_required(VERSION 3.15.0)

project(revng-qa NONE)

install(PROGRAMS revng-test-configure boolparser.py common.sh
        DESTINATION libexec/revng)

install(
  DIRECTORY share/
  DESTINATION share/
  USE_SOURCE_PERMISSIONS)

add_custom_target(
  generated_abi_artifact_sources ALL
  COMMAND
    "python3" "${CMAKE_SOURCE_DIR}/scripts/abi_artifact_generator/generate.py"
    "${CMAKE_SOURCE_DIR}/scripts/abi_artifact_generator/templates/"
    "${CMAKE_SOURCE_DIR}/scripts/abi_artifact_generator/config/"
    "${CMAKE_BINARY_DIR}/abi_artifact_generated/")

install(DIRECTORY "${CMAKE_BINARY_DIR}/abi_artifact_generated/"
        DESTINATION "share/revng/test/tests/abi/")
