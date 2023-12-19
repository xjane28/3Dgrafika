#pragma once
#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>

// Main template parameterized by dimension and type
template <size_t DIM, typename T>
struct Vec {
    // Constructor
    Vec() {
        for (size_t i = DIM; i--; data_[i] = T());
    }

    // Access operators
    T& operator[](const size_t i) {
        assert(i < DIM);
        return data_[i];
    }

    const T& operator[](const size_t i) const {
        assert(i < DIM);
        return data_[i];
    }

private:
    T data_[DIM];
};

// Type specialization based on float or int and dimension size
typedef Vec<2, float> Vec2f;
typedef Vec<3, float> Vec3f;
typedef Vec<3, int> Vec3d;
typedef Vec<4, float> Vec4f;

template <typename T>
struct Vec<2, T> {
    // Constructors
    Vec() : x(T()), y(T()) {}
    Vec(T X, T Y) : x(X), y(Y) {}

    template <class U>
    Vec<2, T>(const Vec<2, U> &v);

    // Access operators
    T& operator[](const size_t i) {
        assert(i < 2);
        return i <= 0 ? x : y;
    }

    const T& operator[](const size_t i) const {
        assert(i < 2);
        return i <= 0 ? x : y;
    }

    // Components
    T x, y;
};

template <typename T>
struct Vec<3, T> {
    // Constructors
    Vec() : x(T()), y(T()), z(T()) {}
    Vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

    // Access operators
    T& operator[](const size_t i) {
        assert(i < 3);
        return i <= 0 ? x : (1 == i ? y : z);
    }

    const T& operator[](const size_t i) const {
        assert(i < 3);
        return i <= 0 ? x : (1 == i ? y : z);
    }

    // Normalization
    float norm() {
        return std::sqrt(x * x + y * y + z * z);
    }

    float norm() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vec<3, T>& normalize(T l = 1) {
        *this = (*this) * (l / norm());
        return *this;
    }

    // Components
    T x, y, z;
};

template <typename T>
struct Vec<4, T> {
    // Constructors
    Vec() : x(T()), y(T()), z(T()), w(T()) {}
    Vec(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}

    // Access operators
    T& operator[](const size_t i) {
        assert(i < 4);
        return i <= 0 ? x : (1 == i ? y : (2 == i ? z : w));
    }

    const T& operator[](const size_t i) const {
        assert(i < 4);
        return i <= 0 ? x : (1 == i ? y : (2 == i ? z : w));
    }

    // Components
    T x, y, z, w;
};

// Scalar product implementation
template <size_t DIM, typename T>
T operator*(const Vec<DIM, T> &lhs, const Vec<DIM, T> &rhs) {
    T ret = T();
    for (size_t i = DIM; i--; ret += lhs[i] * rhs[i]);
    return ret;
}

// Vector addition implementation
template <size_t DIM, typename T>
Vec<DIM, T> operator+(Vec<DIM, T> lhs, const Vec<DIM, T> &rhs) {
    for (size_t i = DIM; i--; lhs[i] += rhs[i]);
    return lhs;
}

// Vector subtraction implementation
template <size_t DIM, typename T>
Vec<DIM, T> operator-(Vec<DIM, T> lhs, const Vec<DIM, T> &rhs) {
    for (size_t i = DIM; i--; lhs[i] -= rhs[i]);
    return lhs;
}

// Component-wise product implementation
template <size_t DIM, typename T, typename U>
Vec<DIM, T> operator*(const Vec<DIM, T> &lhs, const U &rhs) {
    Vec<DIM, T> ret;
    for (size_t i = DIM; i--; ret[i] = lhs[i] * rhs);
    return ret;
}

// Negation implementation
template <size_t DIM, typename T>
Vec<DIM, T> operator-(const Vec<DIM, T> &lhs) {
    return lhs * T(-1);
}

// Vector product implementation
template <typename T>
Vec<3, T> cross(Vec<3, T> v1, Vec<3, T> v2) {
    return Vec<3, T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

// Determinant for 3x3 matrices
template <typename T>
float determinant(const Vec<3, T> &v1, const Vec<3, T> &v2, const Vec<3, T> &v3) {
    return (v1.x * (v2.y * v3.z - v2.z * v3.y) - v2.x * (v1.y * v3.z - v1.z * v3.y) + v3.x * (v1.y * v2.z - v1.z * v2.y));
}

// Output vector
template <size_t DIM, typename T>
std::ostream &operator<<(std::ostream &out, const Vec<DIM, T> &v) {
    for (unsigned int i = 0; i < DIM; i++) {
        out << v[i] << " ";
    }
    return out;
}
