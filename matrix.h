#pragma once

#include <array>
#include <format>
#include <functional>
#include <sstream>
#include <iomanip>
#include <cstring>


using namespace std;

template<typename T>
concept Number = integral<T> || floating_point<T>;

template <int width = 4, int height = 4, typename T = float>
class matrix : array<array<T, height>, width> {
    public:
    matrix() : array<array<T, height>, width>() {}

    matrix(function<T()> f) : array<array<T, height>, width>() {
        loop([&](int x, int y) {
            (*this)[x][y] = f();
        });
    }

    matrix(const matrix& other) : array<array<T, height>, width>() {
        loop([&](int x, int y) {
            (*this)[x][y] = other[x][y];
        });
    }

    matrix(function<T(int x, int y)> f) : array<array<T, height>, width>() {
        loop([&](int x, int y) {
            (*this)[x][y] = f(x, y);
        });
    }

    matrix<width, height, T> copy() {
        return matrix<width, height, T>(*this);
    }

    void loop(function<void(int x, int y)> f) const {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                f(x, y);
            }
        }
    }

    void loop(function<void()> f) const {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                f();
            }
        }
    }

    matrix<width, height, T> operator+(const matrix& other) const {
        return matrix([&](int x, int y) {
            return (*this)[x][y] + other[x][y];
        });
    }

    template<int other_width, int other_height, typename T>
    matrix<other_width, height, T> operator*(const matrix<other_width, other_height, T>& other) const {
        return matrix<other_width, height, T>([&](int x, int y) {
            float ret = 0;
            for (int i = 0; i < width; ++i) {
                ret += (*this)[i][y] * other[x][i];
            }
            return ret;
        });
    }

    template<Number N>
    matrix<width, height, T> operator*(const N& n) const {
        return matrix<width, height, T>([&](int x, int y) {
            return (*this)[x][y] * n;
        });
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