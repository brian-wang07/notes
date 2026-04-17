template<class T> 
T add(T a, T b) {
    return a + b;
}

int g() {
    return add<int>(1, 2);
}
