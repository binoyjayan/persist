
## Persistent Memory Programming

### PMDK references

- [pmdk](http://pmem.io/pmdk)
- [libpmemobj](http://pmem.io/pmdk/libpmemobj)
- [libpmemobj examples](https://github.com/pmem/pmdk/tree/master/src/examples/libpmemobj)

### Examples

- e01 : Simplest example
- e02 : Transactional API
- e03 : Nested transactions
- e04 : Typed pointers instead of PMEMoids
- e05 : Transactional dynamic allocation
- e06 : Atomic dynamic memory allocation

### PMKV

- [KVTree](https://github.com/pmem/pmemkv/blob/master/ENGINES.md#kvtree2)
- [PMKV](https://github.com/pmem/pmemkv)

### Block allocator using pmdk

- Writes to persistent memory - whenever there are changes made to leaves
- Recover - read all leaves in the beginning and insert everything keep everything in memory
- Use libpmem -persist call and not libpmemobj
- Do not use transactions whenever possible
- Undo locks for writes across page boundaries


