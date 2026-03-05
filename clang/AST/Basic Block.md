Each basic block is defined as the maximal sequence of statements with a single entry point a single exit point, such that control enters at the beginning and leaves at the end. Note that the control can branch at the end. For example,
```
x = 1
y = 2
if (x < y) goto L1
x = y + 1
goto L2
L1: y = x + 2
L2: return y
```
In the above code snippet, the basic blocks would be defined as:
```
B1:
1. x = 1
2. y = 2
3. if (x < y) goto L1
   
B2:
4. x = y + 1
5. goto L2
   
B3:
6. y = x + 2

B4:
7. return y
```

Each [[CFG]] instance also contains an entry block `CFG::getEntry()` which has no incoming edges, and an exit block `CFG::getExit()` which has no outgoing edges. Note that these blocks contain no statements. The CFG can be pretty-printed to stderr using `clang -cc1 -analyze -analyzer-checker=debug.DumpCFG file_name.cpp`.