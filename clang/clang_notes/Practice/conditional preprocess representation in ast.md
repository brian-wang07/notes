```cpp
//example 1:
#if defined(FLAG)
int f(){return 1;}
#else
int f(){return 2;}
#endif

//example 2;
#define FLAG 1
#if defined(FLAG)
int f() {return 1;}
#else
int f() {return 2;}
#endif
#undef FLAG
```
Example 1:
```
# 1 "conditional_preprocessing.cpp"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 537 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "conditional_preprocessing.cpp" 2



int f(){return 2;}
```
Example 2:
```
# 1 "conditional_preprocessing.cpp"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 537 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "conditional_preprocessing.cpp" 2


int f(){return 1;}
```
note that `#ifdef` and `#if defined` are functionally equivalent, but `#if defined` can be used in expressions (such as `#if defined X && Y`). Notice that clang will resolve which definition of `f()` to use in preprocessing.