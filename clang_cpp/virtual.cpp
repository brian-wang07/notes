struct A {
	int f() {
		return 1;	
	}	
};

int g(A* a) {
	return a->f();
}
