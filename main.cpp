#include <iostream>
#include "matrix.h"

void clear() {
    std::cout << "\033[2J\033[1;1H";
}

int main(int argc, char** argv) {
    clear();
    matrix t1;
    t1.print();
    return 0;
}