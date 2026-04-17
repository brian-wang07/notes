Namespaces are one of the few declarations that can be defined multiple times. Semantically, each definition of a namespace gets merged into one. Thus, semantically, the two examples below are represented identically:
```cpp
//Example 1:
namespace N {
	void f();
}

namespace N {
	void f(int);
}


//Example 2:
namespace N {
	void f();
	void f(int);
}
```
Source-centric, however, will show that in example one there are two seperate declarations of N each with one function definition. In example two, it will show a single declaration of N that defines 2 functions.