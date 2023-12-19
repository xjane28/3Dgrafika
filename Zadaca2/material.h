#pragma once
#include "geometry.h"

struct Material
{  
    Vec3f diffuse_color;
    
    float diffuse_coef = 0.8;     // Modified diffuse coefficient
    float specular_coef = 0.1;    // Modified specular coefficient
    float phong_exp = 1.5;        // Modified Phong exponent
    float reflex_coef = 0.3;      // Modified reflection coefficient
    float refract_coef = 0.8;     // Modified refraction coefficient
    float opacity;
    
    Material(const Vec3f &color, const float opacity) : diffuse_color(color), opacity(opacity) {}
    Material() : diffuse_color(Vec3f(0, 0, 0)), opacity(1) {}
};
