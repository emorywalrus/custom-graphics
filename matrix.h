#pragma once

#include <array>
#include <format>
#include <functional>
#include <sstream>
#include <iomanip>
#include <cstring>

const double pi = 3.14159265358979323846264338327950288419716939937510;

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

    void foreach(function<void(T)> f) const {
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
    matrix operator*(const N& n) const {
        return matrix([&](int x, int y) {
            return (*this)[x][y] * n;
        });
    }

    matrix<height, width, T> transpose() {
        matrix<height, width, T> ret;
        loop([&](int x, int y) {
            ret[y][x] = (*this)[x][y];
        });
        return ret;
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
                if (y == height - 1) ret << "]]\n";
                else ret << "] \n";
            } else ret << " ";
        });
        return ret.str();
    }

    void print() {
        cout << (string)(*this) << endl;
    }
};

template<int width = 4, int height = 4, Number T = float>
matrix<width, height, T> xRotationMatrix(T angle) {
    matrix<width, height, T> ret;
    ret[0][0] = 1;
    ret[1][1] = cos(angle);
    ret[2][1] = -sin(angle);
    ret[1][2] = sin(angle);
    ret[2][2] = cos(angle);
    ret[3][3] = 1;
    return ret;
}

template<int width = 4, int height = 4, Number T = float>
matrix<width, height, T> yRotationMatrix(T angle) {
    matrix<width, height, T> ret;
    ret[0][0] = cos(angle);
    ret[2][0] = sin(angle);
    ret[0][2] = -sin(angle);
    ret[1][1] = 1;
    ret[2][2] = cos(angle);
    ret[3][3] = 1;
    return ret;
}

template<int width = 4, int height = 4, Number T = float>
matrix<width, height, T> zRotationMatrix(T angle) {
    matrix<width, height, T> ret;
    ret[0][0] = cos(angle);
    ret[1][0] = -sin(angle);
    ret[0][1] = sin(angle);
    ret[1][1] = cos(angle);
    ret[2][2] = 1;
    ret[3][3] = 1;
    return ret;
}

template<int length, Number T>
struct vec : public matrix<1, length, T> {
    T& x_ = (*this)[0];
    T& y_ = (*this)[1];
    T& z_ = (*this)[2];
    T& w_ = (*this)[3];

    vec() : matrix<1, length, T>() {}

    vec(const vec& other) : matrix<1, length, T>(other) {}

    vec(const matrix<1, length, T>& other) : matrix<1, length, T>(other) {}

    vec(T x, T y, T z = 0, T w = 1) : vec() {
        x_ = x;
        y_ = y;
        z_ = z;
        w_ = w;
    }

    vec cross(const vec& other) const {
        vec ret;

        T& a = this->x_;
        T& b = this->y_;
        T& c = this->z_;
        T& d = other.x_;
        T& e = other.y_;
        T& f = other.z_;

        ret.x_ = b * f - c * e;
        ret.y_ = c * d - a * f;
        ret.z_ = a * e - b * d;

        return ret;
    }

    T dot(const vec& other) {
        T sum = 0;
        loop([&](int x, int y) {
            sum += (*this)[x][y] * other[x][y];
        });
        return sum;
    }


    vec(function<void(int i)> f) : matrix<1, length, T>([&](int x, int y) { f(y); }) {}

    T& operator[](int i) {
        return this->data()[0][i];
    }
};

// defaults
template<int width = 4, int height = 4, Number T = float>
matrix() -> matrix<width, height, T>;

template<int width = 4, int height = 4, Number T = float, typename F>
matrix(F) -> matrix<width, height, T>;

template<int length = 4, Number T = float>
vec() -> vec<length, T>;

template<int length = 4, Number T = float>
vec(T ...) -> vec<length, T>;