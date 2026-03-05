Constant folding is a compiler optimization that evaluates expressions, or "folds" expressions at compile time when possible. As one of the [[Design Philosophy|Design Philosophies]] of clang is to stay faithful to the source code, constant folding becomes a tree walk to handle various cases.
The ISO C/C++ Standards define what an integer constant expression (ICE) with a very specific set of requirements. There are many "gray area" cases that are, by the standard, considered semantically incorrect but may still be accepted by the compiler. An example includes:
```cpp
int x = 5;
int a[x]; //VLA: array size initialization must be an ICE
```
Clang is very pedantic about these, however the code will still be accepted unless the `-pedantic-errors`  flag is enabled. As such, expressions as the one above with VLAs must be folded in order to check for semantic validity. 