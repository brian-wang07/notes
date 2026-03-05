```cpp
//example 1:
struct A {
	virtual int f() {
		return 1;	
	}	
};

int g(A* a) {
	return a->f();
}

//example 2:
struct A {
	int f() {
		return 1;	
	}	
};

int g(A* a) {
	return a->f();
}
```

![[Pasted image 20260305125035.png]]
Note that the AST for virtual vs non virtual is nearly the exact same; the only difference is that the member function `f` wluld be missing the `virtual` flag. This is because the AST is not responsible for vtable creation; it only checks for semantic accuracy. For example,
```cpp
struct A { virtual int f(); } ;
struct B : A {int f() override; };
```
The compiler only needs to check if `f` exists as a member function of `A`, and nothing else. however, in codegen:
-O0:
Virtual:
![[Pasted image 20260305125826.png]]
Normal member:
caller: ![[no_virtual_struct.png]]
callee:![[no_virtual_call.png]]
In the non virtual case, there are two cfgs generated; one for the struct, another for `g()`. Notice that `g()` directly invokes the function (%4). however, for the virtual case, it calls only a pointer (%7). Since it is indirectly mentioned, there is no cfg generated for this method.



