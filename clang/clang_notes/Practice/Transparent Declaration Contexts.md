There are many examples of [[Declaration Contexts|declaration contexts]] that do not create their own scope. Thus, declarations within the context can "leak" out of the enclosed scope. For example,
```cpp
enum Color {
	Red,
	Green,
	Blue
};
```
Here, `Red`, `Green`, and `Blue` are defined within `Color`'s context, thus iterating over the declarations within the context will yield the variants. However, we are able to define variants without having to scope into `Color`, e.g.
```cpp
Color c = Red; //NOT Color c = Color::Red, as the enum does not create its own scope.
```

Some other examples include `extern` linkage, anonymous unions and structs, and inline namespaces.
```cpp
extern "C" {
	void f(int);
	void g(int);
}

struct LookupTable {
	bool IsVector;
	union { //anonymous
		std::vector<Item> *Vector;
		std::set<Item> *Set;
	};
};

namespace mylib {
	inline namespace debug {
		class X;	
	}
}

f(); //in global scope, dont need to scope into "C"
g();

LookupTable LT;
LT.Vector = 0; //finds Vector in anonymous union

mylib::X *xp; //mylib::X referes to mylib::debug::X
```