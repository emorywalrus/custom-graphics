#include <iostream>
#include "matrix.h"

void clear() {
    std::cout << "\033[2J\033[1;1H";
}

int main(int argc, char** argv) {
    clear();
    vec t1(0, 0, 1);
    t1.print();
    vec t2 = yRotationMatrix(pi/2) * t1;
    t2.print();
    return 0;
}