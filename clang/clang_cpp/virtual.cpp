struct A {
	virtual int f() {
		return 1;	
	}	
};

int g(A* a) {
	return a->f();
}
