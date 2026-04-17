int side_effect();
int g() {
	int x = sizeof(side_effect());
	return x;
}
