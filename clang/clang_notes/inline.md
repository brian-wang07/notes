The `inline` [[Type Qualifier|type qualifier]] has two very different meanings.
1. Consider:
	```cpp
	//foo.h
	
	int add(int a, int b) {
		return a + b;//bad!
	}
	```
	This is usually illegal if this header is included in two different .cpp files:
	```
	a.cpp -> add()
	b.cpp -> add() 
	```
	Multiple declaration linker error.
	by adding the `inline` keyword, different .cpp files can define the function and the linker will merge them to the same implementation.
	```cpp
	inline add(int a, int b) {
		return a + b; //ok	
	}	
	```
2. (historic, mostly depricated): originally, inline would tell the compiler that it can replace function calls with the function body.
	```cpp
	inline int square(int x) {
		return x * x;
	}
	```
	Here, every invocation of `square(x)` will replace it with `x * x`. 
	However, modern compilers perform this optimization automatically.