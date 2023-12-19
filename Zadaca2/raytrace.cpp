#include <cmath>
#include <vector>
#include <fstream>
#include <algorithm>
#include "geometry.h"
#include "ray.h"
#include "objects.h"
#include "light.h"

using namespace std;

typedef vector<Vec3f> Image;
typedef vector<Object*> Objects;
typedef vector<Light*> Lights;

// Function to save the image to a .ppm file
void save_image(const Image &image, const int width, const int height, const string path)
{
    ofstream ofs;
    ofs.open(path, ofstream::binary);

    // PPM format header
    ofs << "P6\n" << width << " " << height << "\n255\n";

    // Writing pixel data
    for (int i = 0; i < width * height; ++i)
    {
        ofs << (char)(255 * min(max(image[i][0], 0.f), 1.f));
        ofs << (char)(255 * min(max(image[i][1], 0.f), 1.f));
        ofs << (char)(255 * min(max(image[i][2], 0.f), 1.f));
    }

    // Close the file
    ofs.close();
}

// Function to check if a ray intersects with any object in the scene
bool scene_intersect(const Ray &ray, const Objects &objs, Material &hit_material, Vec3f &hit_point, Vec3f &hit_normal)
{
    float best_dist = numeric_limits<float>::max();
    float dist = numeric_limits<float>::max();

    Vec3f normal;

    for (auto obj : objs)
    {
        if (obj->ray_intersect(ray, dist, normal) && dist < best_dist)
        {
            best_dist = dist;
            hit_material = obj->material;
            hit_normal = normal;
            hit_point = ray.origin + ray.direction * dist;
        }
    }

    return best_dist < 1000;
}

// Function to compute the color of a ray by tracing it through the scene
Vec3f cast_ray(const Ray &ray, const Objects &objs, const Lights &lights, const float &depth)
{
    int maxDepth = 5;
    Vec3f hit_normal;
    Vec3f hit_point;
    Material hit_material;

    if (!scene_intersect(ray, objs, hit_material, hit_point, hit_normal) || depth > maxDepth)
    {
        return Vec3f(0.8, 0.8, 1); // Background color
    }
    else
    {
        // ... (unchanged code)

        return hit_color = (hit_color + cast_ray(Ray(hit_point + hit_normal * 0.001, reflected_ray), objs, lights, depth + 1) * hit_material.reflex_coef) * hit_material.opacity + cast_ray(Ray(hit_point + hit_normal * 0.001, refracted_ray), objs, lights, depth + 1) * (1 - hit_material.opacity);
    }
}

// Function to generate a ray from the origin to a pixel in the image
Ray ray_to_pixel(Vec3f origin, int i, int j, int width, int height)
{
    Ray ray = Ray();
    ray.origin = origin;

    float fov = 1.855; // 106.26° in radians
    float tg = tan(fov / 2.);

    float x = (-1 + 2 * (i + 0.5) / (float)width) * tg;
    float y = -(-1 + 2 * (j + 0.5) / (float)height);
    float z = -1;

    ray.direction = Vec3f(x, y, z).normalize();
    return ray;
}

// Function to render the image by tracing rays through each pixel
void draw_image(Objects objs, Lights lights)
{
    const int width = 1024;
    const int height = 768;

    Image img(width * height);

    Vec3f origin = Vec3f(0, 0, 0);

    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            Ray ray = ray_to_pixel(origin, i, j, width, height);
            Vec3f color = cast_ray(ray, objs, lights, 0);
            img[i + j * width] = color;
        }
    }

    save_image(img, width, height, "./render.ppm");
}

int main()
{
Material M(Vec3f(1, 0, 0), 1);
    M.specular_coef = 1;
    M.phong_exp = 50;
    
    Material M1(Vec3f(0.5, 0.5, 0), 0.5);
    M1.specular_coef = 1;
    M1.phong_exp = 1000;
    
    Material M2(Vec3f(0, 0, 1), 0.5);
    M2.specular_coef = 1;
    M2.phong_exp = 200;
    
    Material grey(Vec3f(0.5, 0.5, 0.5), 1);


    Cuboid plane(Vec3f(-10, -5, -15), Vec3f(10, -4.5, 9), grey);
    Sphere s1(Vec3f(-1.5, -3, -12), 1.5, M1);
    Sphere s2(Vec3f(3, -4, -11), 0.5, M);
    Cuboid c1(Vec3f(7, 0,  -15), Vec3f(10, -7,  -10), M1);
    Cuboid c2(Vec3f(-7, 0,  -15), Vec3f(-10, -7,  -10), M2);
    Objects O = { &plane, &s1, &s2, &c1, &c2 };
    

    Light l1 = Light(Vec3f(-20, 20, 10), 2000);
    Light l2 = Light(Vec3f(10, 30, 60), 5000);
    Lights lights = { &l1, &l2 };
    
    draw_image(O, lights);
    return 0;
}
