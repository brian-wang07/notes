```cpp
template<class T> 
T add(T a, T b) {
	return a + b;
}

int g() {
	return add<int>(1, 2);
}
```
AST:
![[Pasted image 20260305184821.png]]
Each unique call of `add` will implicitly create a name-mangled version of `add`.
