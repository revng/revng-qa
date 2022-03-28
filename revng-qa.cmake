if("${CMAKE_PROJECT_NAME}" STREQUAL "revng-qa")
  set(IN_REVNG_QA On)

  macro(include_and_install PATH)
    include("${PATH}")
    get_filename_component(PATH_DIR "${PATH}" DIRECTORY)
    install(FILES "${PATH}" DESTINATION "share/revng/qa/cmake/${PATH_DIR}")
  endmacro()

  install(FILES "revng-qa.cmake" DESTINATION "share/revng/qa/cmake/")

else()
  set(IN_REVNG_QA Off)

  macro(include_and_install PATH)
    include("${CMAKE_INSTALL_PREFIX}/share/revng/qa/cmake/${PATH}")
  endmacro()
endif()

include_and_install(macros.cmake)
include_and_install(configurations.cmake)
include_and_install(tests/abi/ABITests.cmake)
include_and_install(tests/runtime/RuntimeTests.cmake)
include_and_install(tests/analysis/AnalysisTests.cmake)

if(NOT IN_REVNG_QA)
  # Include all the derived artifacts definitions
  file(GLOB DERIVED_ARTIFACTS_CMAKE "${CMAKE_INSTALL_PREFIX}/share/revng/qa/cmake/derived-artifacts/*.cmake")
  foreach(DERIVED_ARTIFACT_CMAKE ${DERIVED_ARTIFACTS_CMAKE})
    include("${DERIVED_ARTIFACT_CMAKE}")
  endforeach()
endif()
