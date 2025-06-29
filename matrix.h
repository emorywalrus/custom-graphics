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

template <int width, int height, Number T>
struct matrix : public array<array<T, height>, width> {
    matrix() : array<array<T, height>, width>() {}

    matrix(const matrix& other) : matrix() {
        loop([&](int x, int y) {
            (*this)[x][y] = other[x][y];
        });
    }

    matrix(function<T()> f) : matrix() {
        loop([&](int x, int y) {
            (*this)[x][y] = f();
        });
    }

    matrix(T(*f)()) : matrix(function<T()>(f)) {}

    matrix(function<T(int x, int y)> f) : matrix() {
        loop([&](int x, int y) {
            (*this)[x][y] = f(x, y);
        });
    }

    matrix copy() {
        return matrix(*this);
    }

    void loop(function<void(int x, int y)> f) const {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                f(x, y);
            }
        }
    }

    void loop(function<void()> f) const {
        loop([&](int x, int y) {
            f();
        });
    }

    void loop(function<void(T)> f) const {
        loop([&](int x, int y) {
            f((*this)[x][y]);
        });
    }

    matrix operator+(const matrix& other) const {
        return matrix([&](int x, int y) {
            return (*this)[x][y] + other[x][y];
        });
    }

    template<int other_width, int other_height, Number T>
    matrix operator*(const matrix<other_width, other_height, T>& other) const {
        return matrix<other_width, height, T>([&](int x, int y) {
            float ret = 0;
            for (int i = 0; i < width; ++i) {
                ret += (*this)[i][y] * other[x][i];
            }
            return ret;
        });
    }

    template<Number N>
    matrix operator*(const N& n) const {
        return matrix([&](int x, int y) {
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

template<int length, Number T = float>
struct vec : public matrix<1, length, T> {
    const int length_ = length;
    T dot(const vec& other) {
        static_assert(length_ == other.length_);
        T sum = 0;
        return other.loop([&](T n) {
            sum += n;
        });
        return sum;
    }
};

template <Number T = float>
struct vec3 : public vec<3, T> {
    vec3(function<void(int i)> f) : matrix<1, 3, T>([](int x, int y) { f(y); }) {}
    vec3 cross(const vec3& other) const {

    }
};

// defaults
template<int width = 4, int height = 4, Number T = float>
matrix() -> matrix<width, height, T>;

template<int width = 4, int height = 4, Number T = float, typename F>
matrix(F) -> matrix<width, height, T>;