#include <iostream>
#include "matrix.h"

void clear() {
    std::cout << "\033[2J\033[1;1H";
}

int main(int argc, char** argv) {
    clear();
    matrix t1([&](int x, int y) -> float { return x; });
    matrix t2([&](int x, int y) -> float { return y; });
    t1.print();
    t2.print();
    matrix t3 = t2 * t1;
    t3.print();
    return 0;
}