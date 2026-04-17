the `volatile` [[Type Qualifier|type qualifier]] tells the compiler that the value of an object may change outside of the program's control. for example,
```cpp
while (x == 0) {
/*...*/
}
```
Without `volatile`, the compiler may optimize (depending on the optimization level) this to 
```cpp
if (x==0) {
	while (1) //dont need to compare x for every iteration
}
```
However, if `x` changes externally, this would fail. `volatile` acts as barriers in optimization, telling the compiler to not touch them. 
```cpp
volatile int x = 0;
while (x == 0) {
/*...*/ //the compiler will not touch this, and will perform the x==0 check every iteration.
}
```
Example:
```cpp
//example 1:
static int v = 1; internal linkage
int f() {
	int x = v;
	v = x + 1;
	return v;
}

//example 2:
static volatile int v = 1;
int f() {
	int x = v;
	v = x + 1;
	return v;
}
```
with O3 optimization:
Example 1 (no volatile):
![[no_volatile.png]]
Example 2:
![[volatile.png]]