cmake_minimum_required(VERSION 3.15.0)

project(revng-qa NONE)

install(PROGRAMS revng-test-configure boolparser.py
  DESTINATION libexec/revng)

install(DIRECTORY share/ DESTINATION share/)

add_custom_target(
  generated_abi_artifact_sources ALL
  COMMAND
  "python3"
  "${CMAKE_SOURCE_DIR}/scripts/abi_artifact_generator/generate.py"
  "${CMAKE_SOURCE_DIR}/scripts/abi_artifact_generator/templates/"
  "${CMAKE_SOURCE_DIR}/scripts/abi_artifact_generator/config/"
  "${CMAKE_BINARY_DIR}/abi_artifact_generated/")

install(FILES
  "${CMAKE_BINARY_DIR}/abi_artifact_generated/functions.h"
  "${CMAKE_BINARY_DIR}/abi_artifact_generated/functions.inc"
  DESTINATION "share/revng/test/tests/abi/source")

install(FILES
  "${CMAKE_BINARY_DIR}/abi_artifact_generated/x86_64/describe_functions.inc"
  DESTINATION "share/revng/test/tests/abi/source/x86_64")

install(FILES
  "${CMAKE_BINARY_DIR}/abi_artifact_generated/arm/describe_functions.inc"
  DESTINATION "share/revng/test/tests/abi/source/arm")
