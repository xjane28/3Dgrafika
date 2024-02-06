#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include "tgaimage.h"
using namespace std;


typedef float Vec3[3]; 
typedef float Vec2[2];

// dimenzije slike
const int width  = 512;
const int height = 512;

// definirajmo boje
const TGAColor green = TGAColor(0, 255, 0, 1);
const TGAColor red   = TGAColor(255, 0, 0, 1);
const TGAColor blue  = TGAColor(0, 0, 255, 1);
               

//racun 
float rubnafunk(const Vec3 &a, const Vec3 &b, const Vec3 &c) { 
    return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]); 
} 

//funkcija iscrtava trokut kroz zadane pixele
void draw_triangle(float x0, float y0,float z0, float x1, float y1,float z1, float x2, float y2,float z2 ,TGAImage &image,TGAColor color)
{   
    Vec3 a = {x0,y0,z0};
    Vec3 b = {x1,y1,z1};
    Vec3 c = {x2, y2,z2};

     // podijeli kord sa z
    a[0]/=a[2]; a[1]/=a[2];
    b[0]/=b[2]; b[1]/=b[2];
    c[0]/=c[2]; c[1]/=c[2];
     
    //tranformacija 
    
    a[0] = (1 + a[0]) * 0.5 * width; a[1]= (1 + a[1]) * 0.5 * height; 
    b[0] = (1 + b[0]) * 0.5 * width; b[1]= (1 + b[1]) * 0.5 * height; 
    c[0] = (1 + c[0]) * 0.5 * width; c[1]= (1 + c[1]) * 0.5 * height; 
    
    // 1/z
    a[2] = 1 /a[2];  b[2]= 1 /b[2]; c[2] = 1 /c[2];

    for (int j =0; j<height ; j++){
        for (int i= 0;i<width;i++){
            Vec3 p = {((float)i+0.5f),(height-(float)j+0.5f),0};
            float alfa = rubnafunk(c,b,p); 
            float beta =rubnafunk(a,c,p);
            float gama = rubnafunk(b,a,p); 
            if((alfa >= 0 && beta >= 0 && gama >= 0) ) { 
                image.set(j,i,color);
   
            }
        }
    }
}


void raw_triangle_tex (float x0, float y0,float z0, float x1, float y1,float z1, float x2, float y2,float z2 ,float u0,float v0,
                        float u1,float v1 ,float u2,float v2 ,TGAImage &image,const char* filepath){
    
    //ucitavanje teksture
    TGAImage texture;
    texture.read_tga_file(filepath);

    Vec3 a = {x0,y0,z0};
    Vec3 b = {x1,y1,z1};
    Vec3 c = {x2, y2,z2};
    Vec2 u = {u0, v0};
    Vec2 v = {u1, v1};
    Vec2 w = {u2,v2};
   
     // podijeli kord sa z
    a[0]/=a[2]; a[1]/=a[2];
    b[0]/=b[2]; b[1]/=b[2];
    c[0]/=c[2]; c[1]/=c[2];
    
    u[0] /= a[2]; u[1] /= a[2]; 
    v[0] /= b[2]; v[1] /= b[2]; 
    w[0] /= c[2]; w[1] /= c[2]; 
    //tranformacija 
    a[0] = (1 + a[0]) * 0.5 * width; a[1]= (1 + a[1]) * 0.5 * height; 
    b[0] = (1 + b[0]) * 0.5 * width; b[1]= (1 + b[1]) * 0.5 * height; 
    c[0] = (1 + c[0]) * 0.5 * width; c[1]= (1 + c[1]) * 0.5 * height; 
 
    // 1/z
    a[2] = 1 /a[2];  b[2]= 1 /b[2]; c[2] = 1 /c[2];

    float povrsina = rubnafunk(a,b,c); 
    TGAColor color;

    for (int j =0; j<height ; j++){
        for (int i= 0;i<width;i++){
            Vec3 p = {((float)i+0.5f),(height-(float)j+0.5f),0};
            float alfa = rubnafunk(c,b,p); 
            float beta =rubnafunk(a,c,p);
            float gama = rubnafunk(b,a,p); 
            if((alfa >= 0 && beta >= 0 && gama >= 0) ) { 
                color=texture.get(i,j);
                image.set(j,i,color);
            }    
        }
    }
}
void draw_triangle_tex_corrected (float x0, float y0,float z0, float x1, float y1,float z1, float x2, float y2,float z2 ,float u0,float v0,
                        float u1,float v1 ,float u2,float v2 ,TGAImage &image,const char* filepath){
        
         
    //ucitavanje teksture
    TGAImage texture;
    texture.read_tga_file(filepath);

    Vec3 a = {x0,y0,z0};
    Vec3 b = {x1,y1,z1};
    Vec3 c = {x2, y2,z2};
    Vec2 u = {u0, v0};
    Vec2 v = {u1, v1};
    Vec2 w = {u2,v2};
   
     // podijeli kord sa z
    a[0]/=a[2]; a[1]/=a[2];
    b[0]/=b[2]; b[1]/=b[2];
    c[0]/=c[2]; c[1]/=c[2];
    
    u[0] /= a[2]; u[1] /= a[2]; 
    v[0] /= b[2]; v[1] /= b[2]; 
    w[0] /= c[2]; w[1] /= c[2]; 
    //tranformacija 
    a[0] = (1 + a[0]) * 0.5 * width; a[1]= (1 + a[1]) * 0.5 * height; 
    b[0] = (1 + b[0]) * 0.5 * width; b[1]= (1 + b[1]) * 0.5 * height; 
    c[0] = (1 + c[0]) * 0.5 * width; c[1]= (1 + c[1]) * 0.5 * height; 
 
    // 1/z
    a[2] = 1 /a[2];  b[2]= 1 /b[2]; c[2] = 1 /c[2];

    float povrsina = rubnafunk(a,b,c); 
    TGAColor color;

    for (int j =0; j<height ; j++){
        for (int i= 0;i<width;i++){
            Vec3 p = {((float)i+0.5f),(height-(float)j+0.5f),0};
            float alfa = rubnafunk(c,b,p); 
            float beta =rubnafunk(a,c,p);
            float gama = rubnafunk(b,a,p); 
            if((alfa >= 0 && beta >= 0 && gama >= 0) ) { 
                float x = (alfa * u[0] + beta * v[0] + gama * w[0])*(texture.get_width()-0.5); 
                float y =(alfa * u[1] + beta * v[1] + gama * w[1])*(texture.get_height()-0.5); 
                 
                 
                 float z = 1/(alfa * a[2] + beta * b[2] + gama * c[2]); 
                 x*=z;y*=z; 

                color=texture.get(x,y);
                image.set(j,i,color);
            }    
        }
    }
}

int main()
{
    // definiraj sliku
    TGAImage image(width, height, TGAImage::RGB);
    
    // nacrtaj trokute
    draw_triangle(-48, -10,  82, 29, -15,  44, 13,  34, 114,image, blue);
    //raw_triangle_tex(-48, -10,  82, 29, -15,  44, 13,  34, 114,0,0,0,1,1,0,image,"text.tga");
    //draw_triangle_tex_corrected(-48, -10,  82, 29, -15,  44, 13,  34, 114,0,0,0,1,1,0,image,"text.tga");
    //raw_triangle_tex(-48, -10,  82, 29, -15,  44, 13,  34, 114,0,0,0,1,1,0,image,"drvosah.tga");
    //draw_triangle_tex_corrected(-48, -10,  82, 29, -15,  44, 13,  34, 114,0,0,0,1,1,0,image,"drvosah.tga");
    // spremi sliku 
    image.flip_vertically();
    image.write_tga_file("triangle.tga");
}