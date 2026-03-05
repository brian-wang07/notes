- Every declaration exists within a given context.
- SCOPE IS NOT THE SAME AS CONTEXT!!!
```cpp
void f() {
	int x = 1;
	
	{
		int y = 2;	
	}	

}
```
Here there are two scopes: the function and inner block. However, `x` and `y` are both declared in the same `f()` context.
`DeclContext`-derived nodes include: 
- `TranslationUnitDecl`
- `NamespaceDecl`
- `RecordDecl` (class, struct, and union context)
- `FunctionDecl`
- etc.
There are some facilities that are common for all Declaration contexts.
- Source-centric vs semantic-centric view of declarations: Source-centric describes the physical location of the context in the code, whereas sematic-centric gives the logical location. Consider:
	```cpp
	namespace A {
		class X {
			void f();
		};
	}
	
	void A::X::f() {...}
	```
	Here, the declaration of `f` is done in the namespace `A` while the definition appears outside. Thus, the source-centric view will say that the declaration context of `f` is in global namespace. However, semantically, `f` is declared as a member function of `X` so it is in the `X` context. While semantics-centric is useful for future stages with AST traversal as it ensures that member definitions outside of the class stay related to the class, source-centric is more useful for reproducing the source structure for tasks such as error reporting, as it shows where in the source the definition appears. These two views are kept synchronized during AST construction through semantic analysis.
- Storage of declarations within a context: every context can contain some amount of declarations. For example, a `RecordDecl` contains various member fields, functions, etc. All of these are stored in a single `DeclContext` instance and the `DeclContext` can be iterated over. Provides a source centric view of the context.
- Lookup of declarations within a context: `DeclContext` allows for efficient lookups for names within the context. For example, if we are looking for `N::f`, we can use `DeclContext::lookup`. This is implemented with either an array or hash table, depending on the number of declarations in the context. Provides a semantic centric view of the context.
- Ownership of declarations: `DeclContext` owns all declarations within the context, and is thus responsible for managing their memory.
Consider:
```cpp
void f(int x, int y, int z = 1);
inline void f(int x, int y, int z) {/*...*/}
```
Here, the source centric context would include both the declaration and definition of `f`, but the semantics centric will have a single `f` declaration with multiple redeclarations. Expanding on this,
```cpp
void g();
void g(int);
```
`DeclContext::lookup` will return a range of iterators that contain all functions with the name `g`. Note that these are two different functions, but since they share the same name `lookup` will return both.
There are also lexical contexts and semantics contexts, associating with source and semantics centric views respectively.
```cpp
class X {
public:
	void f(int x);
};

void X::f(int x) {/*...*/}
```
Here, the lexical context is `TranslationalUnitDecl` (Global), whereas the semantics is `RecordDecl`.
