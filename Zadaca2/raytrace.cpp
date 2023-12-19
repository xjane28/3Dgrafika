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


// function that prints the image as a .ppm file 
void save_image(const Image &image, const int width, const int height, const string path)
{
    
    ofstream ofs;
    ofs.open(path, ofstream::binary);

    
    // ppm format 
    ofs << "P6\n" << width << " " << height << "\n255\n";

    
    // pixel printing
    for (int i = 0; i < width * height; ++i)
    {
        
        ofs << (char)(255 * min(max(image[i][0], 0.f), 1.f));
        ofs << (char)(255 * min(max(image[i][1], 0.f), 1.f));
        ofs << (char)(255 * min(max(image[i][2], 0.f), 1.f));
        
    }

    
    // closing the file
    ofs.close(); 
    
}


// function that checks if a ray intersects the objects
bool scene_intersect(const Ray &ray, const Objects &objs, Material &hit_material, Vec3f &hit_point, Vec3f &hit_normal)
{
    
    float best_dist = numeric_limits<float>::max();
    float dist = numeric_limits<float>::max();

    
    Vec3f normal;

    
    for (auto obj : objs)
        
    {
        
        if (obj->ray_intersect(ray, dist, normal) && dist < best_dist)
        {
            best_dist = dist;             // distace from the affected sphere
            hit_material = obj->material; // material of the affected sphere 
            hit_normal = normal;          // normal of the affected object
            hit_point = ray.origin + ray.direction * dist; // affected dot
        }
        
    }

    
    return best_dist < 1000;
    
}


// function that returns color
Vec3f cast_ray(const Ray &ray, const Objects &objs, const Lights &lights, const float &depth)
{
    
    int maxDepth = 5;
    Vec3f hit_normal;
    Vec3f hit_point;
    Material hit_material;

    
    if (!scene_intersect(ray, objs, hit_material, hit_point, hit_normal) || depth > maxDepth)
    {
        
        return Vec3f(0.6, 0.7, 1); // return bacground color
        
    }
        
    else
        
    {
        
        float diffuse_light_intensity = 0;
        float specular_light_intensity = 0;

        
        for (auto light : lights) 
        {   
            
            Vec3f light_dir = (light->position - hit_point).normalize();
            float light_dist = (light->position - hit_point).norm();

            
            /*
            
             SHADOWS
            
             idea: - recursively call scene_intersect from Object to Light
                   - if something is between the light and the Object, then we ignore that light
                
            */

            Material shadow_hit_material;
            Vec3f shadow_hit_normal;
            Vec3f shadow_hit_point;
            
            Vec3f shadow_origin;

            
            if (light_dir * hit_normal < 0) // dot product is less than 0 if vector orientations are opposite 
            {        
                
                shadow_origin = hit_point - hit_normal * 0.001;
                
            }
                
            else
                
            {
                
                shadow_origin = hit_point + hit_normal * 0.001;
                
            }

            
            Ray shadow_ray(shadow_origin, light_dir);

            
            // check if the ray shadow_ray intersects the object
            if (scene_intersect(shadow_ray, objs, shadow_hit_material, shadow_hit_point, shadow_hit_normal))
            {
                
                // ray intersects an object
                // we still need to check if that object blocks the light
                // i.e. is it between hit_point and light->position

                
                float dist = (shadow_hit_point - hit_point).norm();

                
                if (dist < light_dist)
                {
                    // the object blocks the light, we skip this iteration
                    continue;
                }
                
            }

            
            // I / r^2
            float dist_factor = light->intensity / (light_dist * light_dist);

            
            // diffuse shading (Lambert model)
            diffuse_light_intensity +=  hit_material.diffuse_coef * dist_factor * max(0.f, hit_normal * light_dir);

            
            // Blinn - Phong shading
            Vec3f view_dir = (ray.origin - hit_point).normalize();

            
            Vec3f half_vector = (view_dir + light_dir).normalize();

            
            specular_light_intensity += hit_material.specular_coef * (dist_factor) * pow(max(0.f, hit_normal * half_vector), hit_material.phong_exp);
        }

        
        Vec3f reflected_ray = ray.direction - hit_normal * (2 * (ray.direction * hit_normal));

        
        float cosi = hit_normal * ray.direction;

        
        Vec3f refracted_ray = (ray.direction * hit_material.refract_coef - hit_normal * (-cosi + hit_material.refract_coef * cosi));

        
        Vec3f diffuse_color = hit_material.diffuse_color * diffuse_light_intensity;

        
        Vec3f hit_color = diffuse_color + Vec3f(1, 1, 1) * specular_light_intensity;

        
        return hit_color = (hit_color + cast_ray(Ray(hit_point + hit_normal * 0.001, reflected_ray), objs, lights, depth + 1) * hit_material.reflex_coef) * hit_material.opacity + cast_ray(Ray(hit_point + hit_normal * 0.001, refracted_ray), objs, lights, depth + 1) * (1 - hit_material.opacity);
    
    }
}


// function that creates a ray from the point origin
// which passes through pixel (i, j) in the image
// (formula from lecture 3)


Ray ray_to_pixel(Vec3f origin, int i, int j, int width, int height)
{
    
    Ray ray = Ray();
    ray.origin = origin;

    
    float fov = 1.855; // 106.26° u radijanima
    float tg = tan(fov / 2.);

    
    float x =  (-1 + 2 * (i + 0.5) / (float)width) * tg;
    float y = -(-1 + 2 * (j + 0.5) / (float)height);
    float z = -1;

    
    ray.direction = Vec3f(x, y, z).normalize();
    return ray;
    
}


void draw_image(Objects objs, Lights lights)
{
    
    // image dimension
    const int width = 1024;
    const int height = 768;

    
    Image img(width * height);

    
    // ray origin
    Vec3f origin = Vec3f(0, 0, 0);

    
    // image drawing
    for (int j = 0; j < height; ++j)
    {
        
        for (int i = 0; i < width; ++i)
        {
            
            Ray ray = ray_to_pixel(origin, i, j, width, height);
            Vec3f color = cast_ray(ray, objs, lights, 0);
            img[i + j * width] = color;
            
        }
    }
    
    // save the image to a disk
    save_image(img, width, height, "./render.ppm");
    
}

int main()
{
    
    Material M(Vec3f(1, 0, 0), 1);
    M.specular_coef = 1;
    M.phong_exp = 50;

    
    Material M1(Vec3f(0.6, 0.6, 0), 0.6);
    M1.specular_coef = 1;
    M1.phong_exp = 1000;

    
    Material M2(Vec3f(0, 0, 1), 0.6);
    M2.specular_coef = 1;
    M2.phong_exp = 200;

    
    Material grey(Vec3f(0.6, 0.6, 0.6), 1);

    

    Cuboid plane(Vec3f(-13, -7, -17), Vec3f(11, -4.5, 10), grey);
    Sphere s1(Vec3f(-2.5, -3.5, -11), 1.5, M1);
    Sphere s2(Vec3f(5, -3, -9), 0.5, M);
    Cuboid c1(Vec3f(5, 2,  -15), Vec3f(13, -5,  -18), M1);
    Cuboid c2(Vec3f(-4, 3,  -12), Vec3f(-7, -4,  -7), M2);
    Objects O = { &plane, &s1, &s2, &c1, &c2 };


    Light l1 = Light(Vec3f(-30, 10, 20), 2000);
    Light l2 = Light(Vec3f(10, 40, 70), 5000);
    Lights lights = { &l1, &l2 };

    
    draw_image(O, lights);

    
    return 0;
}
