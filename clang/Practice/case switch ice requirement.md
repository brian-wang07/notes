```cpp
int f(int a) {
	switch (a) {
		case 3+4: return 1;
		default: return 0;	
	}
}
```
AST:
![[Pasted image 20260305100756.png]]
Notice how in the `CaseStmt`, we have both a `ConstantExpr` and the original 3 + 4. This is because clang will fold all integer constant expressions, but will also carry through the original, source accurate representation, following the [[Design Philosophy|it's design philosophy]].