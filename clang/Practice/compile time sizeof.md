```cpp
int side_effect();
int g() {
	int x = sizeof(side_effect());
	return x;
}
```
-ast-dump:
![[Pasted image 20260305101050.png]]
Importantly, notice that there is no call to `side_effect()`. the `non_odr_use_unevaluated` tells us that clang does not evaluate the value of `side_effect()`, and there is no call made to the function.
