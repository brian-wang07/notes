- What is the difference between undefined, unspecified, and implementation-defined behaviour? Give an example of each
implementation defined: behaviour can vary depending on implementation (architecture, abi, compiler, etc.)
	examples: char being signed/unsigned, exact type of std::size_t, sizeof(int), number of bits in a byte, etc.
unspecified: behaviour of the program varies depending on implementation, and doesnt necessarily have to be documented/consistent within the implementation.
	example: order of evaluation: int x = f() + g(); does f or g get evaluated first?
undefined behaviour: formally, there are no restrictions on the behaviour.
	examples: data races, signed int overflow, indexing oob.

- What is ODR (One Definition Rule) and why does it exist?
ODR: Within a given translation unit, there may only exist one *definition* of an object. 
	***There may exist multiple declarations, but each declaration can only have one definition.***
	Prevents linker errors for inconsistent definitions.

- Explain two-phase name lookup in templates. Why is it necessary?
Describes the process in which templates are instantiated. At template definition, all syntactic and semantic analysis for non-dependent names are resolved (analysis that can be done regardless of the generic type). On invocation, the template parameter is replaced with the correct type and dependent names are resolved
	example: if a template has a generic type T and calls sqrt(T), then the first lookup will not check for if sqrt(T) is valid; this is done when the function is invoked and T is replaced with a concrete type.

- What is value category? Explain lvalue, rvalue, prvalue, xvalue, glvalue
 Value categories are on expressions, not on individual types/objects.
 Two main ways they get defined: 
	 Does the expression have identity? -> If the expression has a physical location in memory that persists past the expression itself
	 Can it be moved from? -> If it is safe to steal resources from the expression
	Lvalue: An expression that designates an object/function that has a stable identity; You can get the address of it safely and it's lifetime extends past the expression itself. 
		examples: names of variables, dereference, array index, function/member calls (on lvalues), etc
	prvalue (pure r value): an expression that has no identity nor location in memory, but has a value
		examples: literals, built in arithmetic, `nullptr`, `true`, `static_cast<T>(x)`, 
	xvalue (eXpiring value): An lvalue whos location is expiring, thus is safe to steal resources from.
		examples: `std::move(x)`, `static_cast<T&&>(x)`, `T&& f()`, array index on rvalue
	glvalue (generalized lvalue): the union of lvalue $\bigcup$ xvalue. useful for definitions such as: glvalues of array types can decay to a pointer to the first element, since both lvalues and xvalues behave this way.
	rvalue: the union of prvalue $\bigcup$ xvalue. Any rvalue is safe to move from
	Materialization: from C++17+, there is an operation known as temporary materialization; it is the only conversion from an prvalue to glvalue; when a prvalue is needed, it gets implicitly converted into an xvalue. 
```cpp
struct S { int m; };
int i = S().m;//member access expects a glvalue. S() prvalue is implicitly converted to an xvalue.
```

- What happens during overload resolution? Walk me through how the compiler picks a function
consider
```cpp
void f(int x);
void f(double x);
void f(int x, int y);

f(3.0f);
```
1. Name lookup: the compiler finds all valid declarations of f within context and creates a candidate set: here, it would be all three declarations of f.
2. Viable function filtering: Candidate functions are valid if they satisfy: 
	1. correct argument count, accounting for default and variadic args
	2. there exists some sequence of implicit conversions from each argument type to the corresponding parameter type.
	Here, since the invocation of f has one parameter, `f(int x, int y)` is eliminated. however, since there exists conversions from float -> int (narrowing conversion) and float -> double (floating-point promotion), there are still two potential candidates.
3. best viable function: each implicit conversion sequence (ICS) is ranked as follows:
	1. Exact match: no conversion is needed, or a trivial identity/lvalue->rvalue conversion
	2. Promotion: float->double, char->int, bool->int, or any conversion that widens the type to their natural size
	3. Standard: float->int (narrowing), int->double, pointer conversions, derived to base, etc.
	4. User defined conversion via conversion operator or `operator T()`
	5. Ellipses: variadic
	The function that has the lowest rank wins. in this case, the promotion (rank 2) wins over the standard conversion (rank 3).
4. Tie breaking: If there are still multiple candidates, tie breaking rules take place:
	1. A non-template function beats a function with template specialization
	2. A more specialized template (think: more strict trait bounds) beats a less specialized one
	3. Else: ambiguity error
Note: `=delete` functions are still candidates, and do not fall back.
```cpp
h(int x)=delete;
h(double x);

h(5); //will select h(int), then errors use of deleted function.
```

- What is SFINAE and how does it interact with overload resolution?
SFINAE: Substitution Failure Is Not An Error
Consider:
```cpp
template <typename T>
typename T::value_type sum(T a, T b, typename T::value_type acc) {
	std::cout << "value_type version called" << std::endl;
	while (a != b) {
		acc += *b++;
	}	
	return acc;
}

template <typename T>
T sum(T *a, T *b, T acc) {
	std::cout << "T version called" << std::endl;
	while (a != b) {
		acc += *b++;
	}	
	return acc;	
}

int main() {
	std::vector<int> a {1, 2, 3, 4};
	auto res1 = sum(a.begin(), a.end(), 0)
	auto res2 = sum(a.data(), a.data() + 4, 0)
}
```
In this case, we have an overload of the sum function, one that expects a container and asserts via T::valuetype, and another that expects a pointer. When we invoke sum via res1, the function gets matched with both versions; however, even though `*a.begin()` should raise an error, it doesn't; just because the realization of a template type fails does not mean that the function itself fails, **as long as there is another valid overload**. 
SFINAE is also way template errors are so verbose; the compiler will ignore errors on the substitution, and thus the error gets propagated and the stack trace becomes very long

- How do concepts improve on SFINAE?
SFINAE was never originally a feature; it was a subtle bug that people adapted for better generic capabilities for templates. Thus, the main intent doesnt lie in the the type system but through tools such as`std::enable_if` which have boilerplate. As well, diagnostics are unclear (the compiler describes substitution failure, not constraint violation). 
```cpp
template<typename T>
concept Integral = std::is_integral_v<T>;
```
the `requires` keyword (somewhat) similar to rust trait bounds:
```cpp
template<typename T>
concept Addable = requires(T a, T b) {
	{a + b} -> std::same_as<T> //a + b must exist, and it's return type is the same as T.
};
```
Diagnostics improve with concepts as well:
```cpp
//define a type T, and this functionis only valid if T is an integral type. if it is, it will instantiate as an int type with a default value of 0
template<typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
void process(T x);

process(3.14);
```
The error this will give is that the substitution fails; however, it doesnt say what type was missing nor why the constraint exists. With concepts,
```cpp
template<Integral T> 
void process(T x);

process(3.14);
```
Here, the error will say that the constraint was not satisfied, and that `double` does not satisfy `Integral`, and that `std::is_integral_v<double>` evaluated to false. Concepts also give subsumption: concept A subsumes concept B if A's constraints logically imply B's. This allows for overloads with less `std::enable_if`'s and less ambiguity errors, since the concept that is more constrained will get priority.
```cpp
template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
concept SignedIntegral = Integral<T> && std::is_signed_v<T>;

template<Integral T>
void f(T x);

template<SignedIntegral T>
void f(T y);

f(-5);
```
If this was to be done with SFINAE, there is ambiguity, requiring multiple `std::enable_if`. However, since signed_integral subsumes integral, it wins with no ambiguity. Note that templates can also be used in normal function declarations:
```cpp
void f(Integral auto x);
```

- What is argument-dependent lookup (ADL) and when can it cause surprising behaviour?