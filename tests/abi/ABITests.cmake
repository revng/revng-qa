#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

macro(register_abi_artifacts ABI ARCHITECTURE)

register_artifact_category("reference_abi_binary_${ABI}" "ON")
register_artifact("reference_abi_binary_${ABI}" "reference_binary" "${ARCHITECTURE}" "reference_binary.c")
register_artifact_run("reference_abi_binary_${ABI}" "reference_binary" "default" "nope")

register_artifact_category("runtime_abi_analysis_${ABI}" "ON")
register_artifact("runtime_abi_analysis_${ABI}" "analyzed_binary" "${ARCHITECTURE}" "runtime_abi_analysis.c")
register_artifact_run("runtime_abi_analysis_${ABI}" "analyzed_binary" "default" "nope")

endmacro()

register_abi_artifacts("SystemV_x86_64" "x86_64")
register_abi_artifacts("SystemV_x86" "x86")
# ...
register_abi_artifacts("Microsoft_x86_stdcall" "x86")
# ...
