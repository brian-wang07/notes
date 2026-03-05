```cpp
int f(int a, int b) {
	if (a && b) {return 1;}
	return 0;
}
```
Note that the ast will preserve the logical comparison, as it stays faithful to the original description. However, the IR form: 
`-O0`
![[Pasted image 20260305103754.png|619]]
`-03`
![[Pasted image 20260305103833.png]]
In `-O0`, Noce how there is no `and` operation; the IR will first check the truth value of `a`, and if it is false, it will jump to 12, else 8. then, it will check the truth value of `b`, and if it is false, jump to 12 else 11. Since if a&&b is true iff a is true and b is true, there is no need to an additional and operator. In `-O3`, the compiler will ignore the short circuiting as the function is pure. 
O0:                                                                            03:
![[short_circuit_O0.png|334]]![[short_circuit_O3.png]]
