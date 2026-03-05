static volatile int v = 1;
int f() {
	int x = v;
	v = x + 1;
    return v;
}

