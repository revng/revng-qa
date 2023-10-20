# Output of the runner

As mentioned in the [README.md](./README.md), the goal of the testing suite is to analyze the outputs of the runner binary.
This document outlines the structure of said output.

```mermaid
graph TB
  linkStyle default stroke: #888
  classDef black fill: #fff, stroke: #000, stroke-width: 2px
  classDef red fill: #fff, stroke: #f55, stroke-width: 2px
  classDef blue fill: #fff, stroke: #55f, stroke-width: 2px

  yaml("stdout (yaml)")
  TargetArchitecture(TargetArchitecture)
  IsLittleEndian(IsLittleEndian)
  Iterations(Iterations)
  Function("Function (name)")
  Iteration("Iteration (index)")
  StateBeforeTheCall(StateBeforeTheCall)
  StateAfterTheCall(StateAfterTheCall)
  StateAfterTheReturn(StateAfterTheReturn)
  Registers(Registers)
  Stack("Stack (bytes only)")
  Type(Type)
  Bytes(Bytes)
  Name(Name)
  Value(Value)
  class yaml,TargetArchitecture,IsLittleEndian,Iterations,Function,Iteration,StateBeforeTheCall,StateAfterTheCall,StateAfterTheReturn,Registers,Stack,Type,Bytes,Name,Value black

  Arguments(Arguments)
  class Arguments red

  ExpectedReturnValue(ExpectedReturnValue)
  ReturnValue(ReturnValue)
  ReturnValueAddress(ReturnValueAddress)
  class ExpectedReturnValue,ReturnValue,ReturnValueAddress blue

  yaml --> TargetArchitecture
  yaml --> IsLittleEndian
  yaml --> Iterations

  Iterations --> Function
  Iterations --> Iteration
  Iterations --> Arguments
  linkStyle 5 stroke: #f55
  Iterations --> ExpectedReturnValue
  Iterations --> ReturnValue
  Iterations --> ReturnValueAddress
  linkStyle 6,7,8 stroke: #55f

  Arguments --> Type
  Arguments --> Bytes

  ReturnValueAddress --> Name
  ReturnValueAddress --> Value
  ReturnValueAddress --> Bytes

  ReturnValue --> Type
  ReturnValue --> Bytes

  ExpectedReturnValue --> Type
  ExpectedReturnValue --> Bytes

  Iterations --> StateBeforeTheCall --> Registers
  Iterations --> StateAfterTheCall --> Registers
  Iterations --> StateAfterTheReturn --> Registers

  Registers --> Name
  Registers --> Value
  Registers --> Bytes

  StateBeforeTheCall --> Stack
  StateAfterTheCall --> Stack
  StateAfterTheReturn --> Stack

  Stack --> Bytes
```

<details>
  <summary>(graphviz)</summary>

```graphviz
strict digraph {
    "stdout (yaml)"
    "stdout (yaml)" -> "TargetArchitecture"
    "stdout (yaml)" -> "IsLittleEndian"
    "stdout (yaml)" -> "Iterations"

    "Arguments" [color=red]
    "ExpectedReturnValue" [color=blue]
    "ReturnValue" [color=blue]
    "ReturnValueAddress" [color=blue]
    "Iterations" -> "Function (name)"
    "Iterations" -> "Iteration (index)"
    "Iterations" -> "Arguments" [color=red]
    "Iterations" -> "ExpectedReturnValue" [color=blue]
    "Iterations" -> "ReturnValue" [color=blue]
    "Iterations" -> "ReturnValueAddress" [color=blue]

    "Arguments" -> "Type"
    "Arguments" -> "Bytes"

    "ReturnValueAddress" -> "Name"
    "ReturnValueAddress" -> "Value"
    "ReturnValueAddress" -> "Bytes"

    "ReturnValue" -> "Type"
    "ReturnValue" -> "Bytes"

    "ExpectedReturnValue" -> "Type"
    "ExpectedReturnValue" -> "Bytes"

    "Iterations" -> "StateBeforeTheCall" -> "Registers"
    "Iterations" -> "StateAfterTheCall" -> "Registers"
    "Iterations" -> "StateAfterTheReturn" -> "Registers"

    "Registers" -> "Name"
    "Registers" -> "Value"
    "Registers" -> "Bytes"

    "StateBeforeTheCall" -> "Stack (bytes only)"
    "StateAfterTheCall" -> "Stack (bytes only)"
    "StateAfterTheReturn" -> "Stack (bytes only)"
}
```

</details>

Legend:
* red arrow - this link is only present in argument tests.
* blue arrow - this link is only present in return value tests.

To summarize, each iteration contains three "state" points extracted:
* Right before the call: at the address of the call instruction.
* Right after the call: at the address of the first instruction of the function body.
* Right after the return: at the address of the instruction following the call instruction.

Each of those state points lists all the architecture specific registers in the format of `Name-Value-Bytes`.
It also contains the configured (see [here](./config/README.md#commonyml)) number of top stack bytes starting from the current value of the stack pointer as a raw `Bytes` array.

On top of that, it also provides the values of the arguments OR return values (depending on the test type, see [here](./config/functions.yml)) of the functions generated in `functions.inc` (see [here](./templates/README.md#functionsinc)).

For argument functions, only a serialized type (carried over from the original [config](./config/README.md#functionsyml)) and an array of raw bytes of the value are provided
Which should be equivalent to the output of a function like this:
```cpp
void function(type_0 arg_0, type_1 arg_1, {{ and so on }}) {
  puts("- Type: type_0\n  Bytes: [{{ arg_0 bytes }}]");
  puts("- Type: type_1\n  Bytes: [{{ arg_1 bytes }}]");
  // and so on for each remaining argument.
}
```

For return value function, on top of the value setup function (callsite) emitted, its address (to detect the cases when the value is returned as a pointer) as well as the original value that was returned are output.
Which should be equivalent to the output of a set of functions like this:
```cpp
{{ return_value_type }} callee_function(void) {
  {{ return_value_type }} return_value;

  return_value = {{ copy_random_data_in }};
  puts("ExpectedReturnValue:\n-Type: {{ return_value_type }}\n  Bytes: [{{ bytes copied in }}]");

  return return_value;
}

void caller_function(void) {
  {{ return_value_type }} return_value = callee_function();
  printf("ReturnValueAddress:\n- Name: {{ return_value_type }}\n  Value: 0x%x\n  Bytes: [{{ pointer bytes }}]\n", &return_value);
  puts("ReturnValue:\n- Type: {{ return_value_type }}\n  Bytes: [{{ return value bytes }}]");
}
```
