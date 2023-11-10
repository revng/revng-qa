# Output of the runner

As mentioned in the [README.md](./README.md), the goal of the testing suite is to analyze the outputs of the runner binary.
This document outlines the structure of said output.

```mermaid
graph TB
graph TB
  linkStyle default stroke: #888
  classDef black fill: #fff, stroke: #000, stroke-width: 2px
  classDef grey fill: #fff, stroke: #ccc, stroke-width: 2px
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
  Stack("Stack")
  Type(Type)
  Bytes(Bytes)
  Name(Name)
  Value(Value)
  class yaml,TargetArchitecture,IsLittleEndian,Iterations,Function,Iteration,StateBeforeTheCall,StateAfterTheCall,StateAfterTheReturn,Registers,Stack,Type,Bytes,Name,Value black

  Arguments(Arguments)
  class Arguments red

  ReturnValues(ReturnValues)
  class ReturnValues blue

  Type(Type)
  Address(Address)
  AddressBytes(AddressBytes)
  ExpectedBytes(ExpectedBytes)
  FoundBytes(FoundBytes)
  class Type,Address,AddressBytes,ExpectedBytes,FoundBytes black
  
  String(String)
  Integer(Integer)
  ByteArray(ByteArray)
  class String,Integer,ByteArray grey

  yaml --> TargetArchitecture
  yaml --> IsLittleEndian
  yaml --> Iterations

  Iterations --> Function
  Iterations --> Iteration
  Iterations --> Arguments
  linkStyle 5 stroke: #f55
  Iterations --> ReturnValues
  linkStyle 6 stroke: #55f

  ReturnValues --> Type
  ReturnValues --> Address
  ReturnValues --> AddressBytes
  ReturnValues --> ExpectedBytes
  ReturnValues --> FoundBytes

  Arguments --> Type
  Arguments --> Address
  Arguments --> AddressBytes
  Arguments --> ExpectedBytes
  Arguments --> FoundBytes

  Type --> String
  Address --> Integer
  AddressBytes --> ByteArray
  ExpectedBytes --> ByteArray
  FoundBytes --> ByteArray

  Iterations --> StateBeforeTheCall --> Registers
  Iterations --> StateAfterTheCall --> Registers
  Iterations --> StateAfterTheReturn --> Registers
  StateBeforeTheCall --> Stack
  StateAfterTheCall --> Stack
  StateAfterTheReturn --> Stack

  Registers --> Name
  Registers --> Value
  Registers --> Bytes

  Name --> String
  Value --> Integer
  Bytes --> ByteArray
  Stack --> ByteArray
```

Legend:
* red - this link is only present in argument tests.
* blue - this link is only present in return value tests.
* grey - contextual "type" of the relevant data.

To summarize, each iteration contains three "state" points extracted:
* Right before the call: at the address of the call instruction.
* Right after the call: at the address of the first instruction of the function body.
* Right after the return: at the address of the instruction following the call instruction.

Each of those state points lists all the architecture specific registers in the format of `Name-Value-Bytes`.
It also contains the configured (see [here](./config/README.md#commonyml)) number of top stack bytes starting from the current value of the stack pointer as a raw `Bytes` array.

On top of that, it also provides the values of the arguments OR return values (depending on the test type, see [here](./config/functions.yml)) of the functions generated in `functions.inc` (see [here](./templates/README.md#functionsinc)).

For argument functions, the output is equivalent to something like this:
```cpp
void test_function(type_0 arg_0, type_1 arg_1, {{ and so on }}) {
  printf("- Type: %s\n", TYPE_TO_STRING(type_0));
  printf("  Address: 0x%x\n", TO_HEX_STRING(&arg_0));
  printf("  AddressBytes: [ %s ]\n", TO_BYTE_STRING(&arg_0));
  printf("  ExpectedBytes: [ %s ]\n", EXPECTED_BYTES(arg_0));
  printf("  FoundBytes: [ %s ]\n", FOUND_BYTES(arg_0));

  printf("- Type: %s\n", TYPE_TO_STRING(type_1));
  printf("  Address: 0x%x\n", TO_HEX_STRING(&arg_1));
  printf("  AddressBytes: [ %s ]\n", TO_BYTE_STRING(&arg_1));
  printf("  ExpectedBytes: [ %s ]\n", EXPECTED_BYTES(arg_1));
  printf("  FoundBytes: [ %s ]\n", FOUND_BYTES(arg_1));

  // and so on for each remaining argument.
}
```

For return value function, it's very similar:
```cpp
return_value_type test_function(void) {
  return_value_type return_value;

  return_value = fill_with_deterministic_but_random_data();

  return return_value;
}

void setup_function(void) {
  return_value_type return_value = test_function();
  printf("- Type: %s\n", TYPE_TO_STRING(return_value_type));
  printf("  Address: 0x%x\n", TO_HEX_STRING(&return_value));
  printf("  AddressBytes: [ %s ]\n", TO_BYTE_STRING(&return_value));
  printf("  ExpectedBytes: [ %s ]\n", EXPECTED_BYTES(return_value));
  printf("  FoundBytes: [ %s ]\n", FOUND_BYTES(return_value));
}
```
