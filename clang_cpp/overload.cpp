int f(int);
double f(double);
auto g() {
	auto x = f(3);
	auto y = f(3.0);
}
