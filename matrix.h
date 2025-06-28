#pragma once

#include <array>
#include <format>
#include <functional>
#include <sstream>
#include <iomanip>


using namespace std;

template <int width, int height>
struct matrix : array<array<float, height>, width> {
    matrix() : array<array<float, height>, width>() {}

    void loop(function<void(int, int)> f) const {
        for (int y = 0; y < width; ++y) {
            for (int x = 0; x < width; ++x) {
                f(x, y);
            }
        }
    }

    matrix operator+(const matrix& other) const {
        matrix ret;
        loop()
    }

    operator string() const {
        stringstream ret;
        loop([&](int x, int y) {
            if (x == 0) {
                if (y == 0) ret << "[[";
                else ret << " [";
            }
            ret << setprecision(3) << setw(8) << (*this)[x][y];
            if (x == width - 1) {
                if (y == width - 1) ret << "]]\n";
                else ret << "] \n";
            } else ret << " ";
        });
        return ret.str();
    }

    void print() {
        cout << (string)(*this) << endl;
    }
};