```cpp
#define STR(x) #x
#define CAT(a,b) a##b
int CAT(my,Var) = 3;
const char *s = STR(hello);
```
macro expansions are done in preprocessing; thus every stage (including and after lexing) will have macros resolved. This can be seen by running using the -E flag, which will stop compilation after preprocessing and dump to stdout. running `clang -E macro_stringification.cpp` yields:
```cpp
# 1 "macro_stringification.cpp"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 537 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "macro_stringification.cpp" 2

int myVar = 3;
const char *s = "hello";
```
`clang -E -P <file_name>.cpp` can be used to suppress the linemarkers (at the top). Since the lexer receives 