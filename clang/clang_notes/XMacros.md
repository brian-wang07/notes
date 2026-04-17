- XMacros are a common syntax pattern used throughout the CLang codebase. They help avoid writing duplicate code for enum variants, lists, and switch statements.
Consider:
```cpp
//naive
enum Color {
	Red,
	Green,
	Blue
}

const char* ColorNames[] = {
	"Red",
	"Green",
	"Blue"
}
```
The variants Red, Green, and Blue must be repeated in both the enum ```Color```as well as ```ColorNames[]```.
Solution:
```cpp
//colors.def
X(Red)
X(Green)
X(Blue)
```
a seperate ```colors.def``` file is created that stores all of the variants.
```cpp
enum Color {
	#define X(name) name,
	#include "colors.def",
	#undef X		
};
```
Line by line:
```#define X(name) name```: Preprocessing macro. tells compiler to replace X(name) with just name.
#```include "colors.def"```: C++ include statements are naive; the compiler will paste the contents of colors.def. Thus, this expands to:
```cpp
enum Color {
	#define X(name) name,
	X(Red)
	X(Green)
	X(Blue)
	#undef X
}
```
which then gets resolved into 
```cpp
enum Color {
	Red,
	Green,
	Blue	
}
```
We can also create a string table in a similar manner:
```cpp
const char* ColorNames[] = {
	#define X(name) #name,
	#include "colors.def"
	#undef X
}
```
Here, ```#name``` will replace each X(Name) instance with "name", wrapping it in quotations and converting it to a string literal. you can also do ##name, which will paste the string inline. thus, something like ```#define KEYWORD(X, Y) kw_##X``` with a definition such as ```KEYWORD(if, KEYALL)```will be replaced with kw_if.
With a switch statement:
```cpp
const char* toString(Color c) {
	switch(c) {
#define X(name) case name: return #name;
#include "colors.def"
#undef X	
	}
	return "Unknown";
}
```
This expands to:
```cpp
const char* toString(Color c) {
	switch(c) {
		case Red: return "Red";
		case Green: return "Green";
		case Blue: return "Blue";
	}
	return "Unknown";
}
```
In CLang, this is used in areas such as clang/lib/Basic/TokenKinds.cpp:
```cpp
//Token name string table
static const char* const TokNames[] = {
#define TOK(X) #X,
#define KEYWORD(X, Y) #X,
#include "clang/Basic/TokenKinds.def"
	nullptr; //sentinel
}
```
in TokenKinds.def:
```
TOK(unknown)
TOK(eof)
TOK(EOD)
...
KEYWORD(auto, KEYALL)
KEYWORD(break, KEYALL)
...
```
Thus TokNames gets expanded to:
```cpp
static const char* const TokNames[] = {
	"unknown",
	"eof",
	"eod",
	...
	"auto",
	break"
	...
}
```

