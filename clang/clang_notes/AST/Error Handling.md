Error recovery strategies:
- FixIt is used to suggest corrections for when Clang is confident about the fix.
- Each AST node has an invalid flag, and the invalid node is preserved in the AST.
- Invalid nodes are only dropped when absolutely needed.
Consider:
```cpp
int NoArg();
void test(int abc) {
	NoArg(abc); //error! mismatched function args
}
```
Without the recovery AST, clang would historically just drop the function invocation + it's children's nodes.
```
|-FunctionDecl <line:1:1, col:11> NoArg 'int ()'
	|-FunctionDecl <line:2:1, line:4:1> test 'void (int)'
	|-ParmVarDecl <col:11, col:15> col:15 used abc 'int'
		 |-CompoundStmt <col:20, line:4:1>
```
With Recovery AST, extra `RecoveryExpr` nodes will be added.
```
|-FunctionDecl <line:1:1, col:11> NoArg 'int ()'
	|-FunctionDecl <line:2:1, line:4:1> test 'void (int)'
	|-ParmVarDecl <col:11, col:15> used abc 'int'
		|-CompoundStmt <col:20, line:4:1>
			|-RecoveryExpr <line:3:3, col:12> 'int' contains-errors
			    |-UnresolvedLookupExpr <col:3> '<overloaded function type>' lvalue (ADL) = 'NoArg'
			      |-DeclRefExpr <col:9> 'int' lvalue ParmVar 'abc' 'int'
```
Currently, RecoveryExpr is the only recovery node in clang. However, it's type is unclear. due to this, typechecking on some constructs may be difficult, such as in `return 42 + unknownfunction()`. In this scenario, `unknownfunction()` will have some type `DependentTy`, and this suppresses type-based analysis, similar to templates. In cases where the type can be easily inferred, this type will be used for `RecoveryExpr`.