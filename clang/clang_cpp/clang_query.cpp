#include <vector>
#include <string>

int square(int x) {
    return x * x;
}

struct Foo {
    int value;

    Foo(int v) : value(v) {}

    int get() const {
        return value;
    }
};

int main() {
    int a = 3;
    int b = square(a);

    Foo f(10);
    int c = f.get();

    std::vector<int> nums = {1, 2, 3};
    nums.push_back(c);

    if (c > 5) {
        return nums[0];
    }

    return 0;
}
