#
# This file is distributed under the MIT License. See LICENSE.md for details.
#

register_artifact_category("tests_runtime" "ON")

register_artifact("tests_runtime" "calc" "" "calc.c")
register_artifact_run("tests_runtime" "calc" "literal" "12")
register_artifact_run("tests_runtime" "calc" "sum" "'(+ 4 5)'")
register_artifact_run("tests_runtime" "calc" "multiplication" "'(* 5 6)'")

register_artifact("tests_runtime" "function-call" "" "function-call.c")
register_artifact_run("tests_runtime" "function-call" "default" "nope")

register_artifact("tests_runtime" "floating-point" "" "floating-point.c")
register_artifact_run("tests_runtime" "floating-point" "default" "nope")

register_artifact("tests_runtime" "syscall" "" "syscall.c")
register_artifact_run("tests_runtime" "syscall" "default" "nope")

register_artifact("tests_runtime" "global" "" "global.c")
register_artifact_run("tests_runtime" "global" "default" "nope")

register_artifact("tests_runtime" "printf" "" "printf.c")
register_artifact_run("tests_runtime" "printf" "one" "one")
register_artifact_run("tests_runtime" "printf" "two" "one two")
register_artifact_run("tests_runtime" "printf" "three" "one two three")
