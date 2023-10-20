# The runtime abi testing suite

There's only one real source of truth about ABI we can reliably trust - the binary that results when a compiler emits it with the said ABI.
This statement encapsulates the effort behind this testing suite: we let a compiler do its job, and then use some clever tricks to extract as much information as we can from running the said, carefully constructed, binary.
Then, we compare our internal function prototype representation against said information to ensure that no matter how we modify function prototypes, we never contradict the way the compiler emitted the function picked for the testing.

Sadly, that means that the binary needs to be crafted in a special way, with some pieces being heavily architecture dependent, which is why rely on jinja2 to generate most of the sources passed to the compiler.

For an overview of the configuration one needs to provide to add a new architecture, or a new function, see [config docs](./config/).
For an overview of the templates used for source generation, as well as the way they interact, see [template docs](./templates/).
For an overview of the entire testing suite in generate, see [end-to-end example](./end_to_end.md).
