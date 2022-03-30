foreach(CONFIGURATION IN LISTS CONFIGURATIONS)
  get_tool(gcc "${CONFIGURATION}" COMPILER)

  # Detect -no-pie
  execute_process(COMMAND ${COMPILER} -c /dev/null -o /dev/null -no-pie
    RESULT_VARIABLE EXIT_CODE
    OUTPUT_QUIET
    ERROR_QUIET)
  if(EXIT_CODE STREQUAL "0")
    set("CFLAGS_${CONFIGURATION}" ${CFLAGS_${CONFIGURATION}} -no-pie)
  endif()

  # Cross-compilers always go for static
  if(NOT CONFIGURATION IN_LIST NATIVE_CONFIGURATIONS)
    set("CFLAGS_${CONFIGURATION}" ${CFLAGS_${CONFIGURATION}} -static)
  endif()

endforeach()

# ARM binaries have the text segment too low
set(CFLAGS_arm ${CFLAGS_arm} -Wl,-Ttext-segment=0x20000)

# static_native is like dynamic but with -static
set(CFLAGS_static_native ${CFLAGS_dynamic_native} -static)

set(CFLAGS_CATEGORY_tests_analysis -nostdlib -O2 -fno-stack-protector -fomit-frame-pointer -fno-reorder-functions -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-stack-check -fno-optimize-sibling-calls -fno-inline-functions -fno-inline-small-functions -fno-align-functions -fno-optimize-sibling-calls)
set(CFLAGS_CATEGORY_tests_analysis_StackAnalysis -nostdlib)
set(CFLAGS_CATEGORY_tests_runtime -std=c99 -fno-pic -fno-pie -ggdb3 -fno-stack-protector)
set(CFLAGS_CATEGORY_tests_analysis_Decompilation -fno-inline -O2)
set(CFLAGS_CATEGORY_tests_analysis_PromoteStackPointer -no-pie -fno-unroll-loops -fno-inline -O1 -fno-stack-protector)

set(CFLAGS_CATEGORY_abi_test_function_library_SystemV_x86_64 ${CFLAGS_CATEGORY_tests_runtime} -DABIDEF=)
set(CFLAGS_CATEGORY_abi_test_function_library_SystemV_x86 ${CFLAGS_CATEGORY_tests_runtime} -DABIDEF=)
# ...
set(CFLAGS_CATEGORY_abi_test_function_library_Microsoft_x86_stdcall ${CFLAGS_CATEGORY_tests_runtime} -DABIDEF=__stdcall)
# ...

set(CFLAGS_CATEGORY_describe_abi_test_functions_COMMON -O2 -fno-stack-protector -fomit-frame-pointer -fno-reorder-functions -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-stack-check -fno-optimize-sibling-calls -fno-inline-functions -fno-inline-small-functions -fno-align-functions -fno-optimize-sibling-calls -Wl,--gc-sections -ffunction-sections)
set(CFLAGS_CATEGORY_describe_abi_test_functions_SystemV_x86_64 ${CFLAGS_CATEGORY_describe_abi_test_functions_COMMON} -DABIDEF=)
set(CFLAGS_CATEGORY_describe_abi_test_functions_SystemV_x86 ${CFLAGS_CATEGORY_describe_abi_test_functions_COMMON} -DABIDEF=)
# ...
set(CFLAGS_CATEGORY_describe_abi_test_functions_Microsoft_x86_stdcall ${CFLAGS_CATEGORY_describe_abi_test_functions_COMMON} -DABIDEF=__stdcall)
# ...

macro(artifact_handler CATEGORY INPUT_FILE CONFIGURATION OUTPUT TARGET_NAME)
  get_tool(gcc "${CONFIGURATION}" COMPILER)
  set(COMMAND_TO_RUN ${COMPILER} ${CFLAGS} ${CFLAGS_${CONFIGURATION}} ${CFLAGS_CATEGORY_${CATEGORY}} -D_GNU_SOURCE -DTARGET_${CONFIGURATION} ${INPUT_FILE} -o ${OUTPUT})
endmacro()

foreach(CONFIG IN LISTS CONFIGURATIONS)
  if(NOT CONFIGURATION IN_LIST NATIVE_CONFIGURATIONS)
    register_derived_artifact_execution_prefix("compiled" "${CONFIGURATION}" "qemu-${CONFIGURATION}")
  endif()
endforeach()
register_derived_artifact("sources" "compiled" "" "PROGRAM")

macro(artifact_handler CATEGORY INPUT_FILE CONFIGURATION OUTPUT TARGET_NAME)

  if(ARTIFACT_CATEGORY_${CATEGORY}_EXECUTABLE)
    set(EXECUTOR "")
    if(NOT CONFIGURATION IN_LIST NATIVE_CONFIGURATIONS)
      set(EXECUTOR "qemu-${CONFIGURATION}")
    endif()

    set(COMMAND_TO_RUN mkdir -p ${OUTPUT})
    foreach(RUN IN LISTS ARTIFACT_RUNS_${ARTIFACT_CATEGORY}__${ARTIFACT})
      set(COMMAND_TO_RUN ${COMMAND_TO_RUN} COMMAND sh -c "${EXECUTOR} ${INPUT_FILE} ${ARTIFACT_RUNS_${ARTIFACT_CATEGORY}__${ARTIFACT}__${RUN}} > ${OUTPUT}/${RUN}.stdout")
    endforeach()
  endif()

endmacro()
register_derived_artifact("compiled" "compiled-run" "" "DIRECTORY")
