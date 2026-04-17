```cpp
int f(int);
double f(double);
auto g() {
	auto x = f(3);
	auto y = f(3.0);
}
```
![[Pasted image 20260305123844.png]]
The `CallExpr` node chooses the correct implementation of f, thus no ambiguity. Note that there is no name mangling done at this stage, to stay faithful to the source.