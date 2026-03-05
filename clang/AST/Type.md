Types are accessed through the ASTContext class. 
- Often times implicitly created, e.g. `int a = 1; int b = 2; int c = a + b;` will implicitly create two `int` types when initializing `c`.
- Note that [[Type Qualifier|type qualifiers]] such as `const` and `volatile` are not captured by `types`. 
- all types are immutable.
Each instance of the type class contains a canonical type pointer. for simple types with no `typedef` this point will point to itself (e.g. `int, *int, **int`). for types that have a `typedef` definition, it will point to their structurally equivalent type.
