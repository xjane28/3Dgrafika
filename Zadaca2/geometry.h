#pragma once
#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>


// main template with dimension and type as parameters 
template <size_t DIM, typename T> struct vec // definiraj vektor
{
    
    // constructor
    vec() { for (size_t i=DIM; i--; data_[i] = T()); }
    
    // access operator
    T& operator[](const size_t i)             {    assert(i<DIM); return data_[i]; }
    const T& operator[](const size_t i) const {    assert(i<DIM); return data_[i]; }
    
private:
    T data_[DIM];
    
};


// type specialization depending on float or int and on dimension size 
typedef vec <2, float> Vec2f;
typedef vec <3, float> Vec3f;
typedef vec <3, int  > Vec3d;
typedef vec <4, float> Vec4f;


template <typename T> struct vec<2,T> 
{
    // constructors
    vec() : x(T()), y(T()) {}
    vec(T X, T Y) : x(X), y(Y) {}
    
    
    template <class U> vec<2,T>(const vec<2,U> &v);
    
    
    // access operators
    T& operator[](const size_t i)       { assert(i<2); return i<=0 ? x : y; }
    const T& operator[](const size_t i) const { assert(i<2); return i<=0 ? x : y; }
    
    
    // components
    T x,y;
    
};

template <typename T> struct vec<3,T> 
{
    
    // constructors
    vec() : x(T()), y(T()), z(T()) {}
    vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
    
    
    // access operators
    T& operator[]      (const size_t i)       { assert(i<3); return i<=0 ? x : (1==i ? y : z); }
    const T& operator[](const size_t i) const { assert(i<3); return i<=0 ? x : (1==i ? y : z); }


    // normalization
    float norm()                     { return std::sqrt(x*x+y*y+z*z); }    
    float norm()  const              { return std::sqrt(x*x+y*y+z*z); }    
    vec<3,T> & normalize(T l=1) { *this = (*this)*(l/norm()); return *this; }
    
    
    // components
    T x,y,z;
    
};

template <typename T> struct vec<4,T> 
{
    
    // constructors
    vec() : x(T()), y(T()), z(T()), w(T()) {}
    vec(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}
    
    
    // operator pristupa
    T& operator[]      (const size_t i)       { assert(i<4); return i<=0 ? x : (1==i ? y : (2==i ? z : w)); }
    const T& operator[](const size_t i) const { assert(i<4); return i<=0 ? x : (1==i ? y : (2==i ? z : w)); }


    // components
    T x,y,z,w;
    
};


// dot product implementation
template<size_t DIM,typename T> T operator*(const vec<DIM,T>& lhs, const vec<DIM,T>& rhs)
{
    T ret = T();
    for (size_t i=DIM; i--; ret+=lhs[i]*rhs[i]);
    return ret;
}


// vector addition implementation
template<size_t DIM,typename T>vec<DIM,T> operator+(vec<DIM,T> lhs, const vec<DIM,T>& rhs)
{
    for (size_t i=DIM; i--; lhs[i]+=rhs[i]);
    return lhs;
}


// vector subtraction implementation rhs
template<size_t DIM,typename T>vec<DIM,T> operator-(vec<DIM,T> lhs, const vec<DIM,T>& rhs)
{
    for (size_t i=DIM; i--; lhs[i]-=rhs[i]);
    return lhs;
}


// product implementation by components
template<size_t DIM,typename T,typename U> vec<DIM,T> operator*(const vec<DIM,T> &lhs, const U& rhs) 
{
    vec<DIM,T> ret;
    for (size_t i=DIM; i--; ret[i]=lhs[i]*rhs);
    return ret;
}


// vector subtraction implementation lhs
template<size_t DIM,typename T> vec<DIM,T> operator-(const vec<DIM,T> &lhs) 
{
    return lhs*T(-1);
}


// vector product (vektorski)
template <typename T> vec<3,T> cross(vec<3,T> v1, vec<3,T> v2)
{
    return vec<3,T>(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}


// 3x3 matrix determinant
template <typename T> float determinant(const vec<3,T> &v1,const vec<3,T> &v2, const vec<3,T> &v3)
{
    return (v1.x * (v2.y*v3.z-v2.z*v3.y ) - v2.x * (v1.y*v3.z-v1.z*v3.y ) + v3.x * (v1.y*v2.z-v1.z*v2.y ));
}


// printing the vector
template <size_t DIM, typename T> std::ostream& operator<<(std::ostream& out, const vec<DIM,T>& v)
{
    
    for(unsigned int i=0; i<DIM; i++) 
    {
        out << v[i] << " " ;
    }
    
    
    return out ;
}
