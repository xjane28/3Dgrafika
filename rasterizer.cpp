#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include "tgaimage.h"
using namespace std;

// dimenzije slike
const int width  = 512;
const int height = 512;

// definirajmo boje
const TGAColor green = TGAColor(0, 255, 0, 1);
const TGAColor red   = TGAColor(255, 0, 0, 1);
const TGAColor blue  = TGAColor(0, 0, 255, 1);
               

//racun
float funk(float xa,float ya,float xc,float yc,float x,float y){
    return((ya-yc)*x+(xc-xa)*y+xa*yc-xc*ya);
}



//funkcija iscrtava trokut kroz zadane pixele
void draw_triangle_2d(float x0, float y0, float x1, float y1, float x2, float y2 ,TGAImage &image,TGAColor color)
{   
    int najx = ceil (max(max(x0,x1),x2));
    int najmx = floor (min(min(x0,x1),x2));
    int najy = ceil (max(max(y0,y1),y2));
    int najmy = floor(min(min(y0,y1),y2));
   
    float alfa,beta,gama;
    for (int j =najmy; j<=najy ; j++){
        for (int i= najmx;i<=najx;i++){
            
            alfa=funk(x1,y1,x2,y2,i,j)/funk(x1,y1,x2,y2,x0,y0); //fbc
            beta=funk(x2,y2,x0,y0,i,j)/funk(x2,y2,x0,y0,x1,y1);  //fca
            gama=funk(x0,y0,x1,y1,i,j)/funk(x0,y0,x1,y1,x2,y2);  //fab
               
            
            if ((alfa>=0 && beta>=0 && gama>=0) && (alfa<=1 &&beta<=1 &&gama<=1) ) { 
                
                if(( alfa>0 ||((funk(x1,y1,x2,y2,x0,y0))*(funk(x1,y1,x2,y2,-1,-1))) >=0)
                 &&( beta>0 || ((funk(x2,y2,x0,y0,x1,y1))*(funk(x2,y2,x0,y0,-1,-1))) >=0)
                 &&( gama>0 || (funk(x0,y0,x1,y1,x2,y2))*(funk(x0,y0,x1,y1,-1,-1))) >=0){
                        image.set(j, i, color);
                }  
            }
        }
    } 
}



void draw_triangle_2d_gouraurd (float x0, float y0, float x1, float y1, float x2, float y2 ,TGAImage &image,TGAColor c0,TGAColor c1,TGAColor c2){
    int najx = ceil (max(max(x0,x1),x2));
    int najmx = floor (min(min(x0,x1),x2));
    int najy = ceil (max(max(y0,y1),y2));
    int najmy = floor(min(min(y0,y1),y2));
   
    float alfa,beta,gama;
    TGAColor color;
    for (int j =najmy; j<=najy ; j++){
        for (int i= najmx;i<=najx;i++){
            
            alfa=funk(x1,y1,x2,y2,i,j)/funk(x1,y1,x2,y2,x0,y0); //fbc
            beta=funk(x2,y2,x0,y0,i,j)/funk(x2,y2,x0,y0,x1,y1);  //fca
            gama=funk(x0,y0,x1,y1,i,j)/funk(x0,y0,x1,y1,x2,y2);  //fab
               
            
            if ((alfa>=0 && beta>=0 && gama>=0) && (alfa<=1 &&beta<=1 &&gama<=1) ) { 
                
                if(( alfa>0 ||((funk(x1,y1,x2,y2,x0,y0))*(funk(x1,y1,x2,y2,-1,-1))) >=0)
                 &&( beta>0 || ((funk(x2,y2,x0,y0,x1,y1))*(funk(x2,y2,x0,y0,-1,-1))) >=0)
                 &&( gama>0 || (funk(x0,y0,x1,y1,x2,y2))*(funk(x0,y0,x1,y1,-1,-1))) >=0){
                        color.b=alfa*c0.b+beta*c1.b+gama*c2.b;
                        color.g=alfa*c0.g+beta*c1.g+gama*c2.g;
                        color.r=alfa*c0.r+beta*c1.r+gama*c2.r;
                        image.set(j, i, color);
                }  
            }
        }
    } 
}



int main()
{
    // definiraj sliku
    TGAImage image(width, height, TGAImage::RGB);
    
    // nacrtaj trokut 
    draw_triangle_2d(10, 110, 180, 150, 50,240,image, red);
    draw_triangle_2d(300, 200, 400, 150, 350,240,image, blue);
    draw_triangle_2d_gouraurd(300, 200, 200, 100, 100,300,image, blue,red,green);
    
    // spremi sliku 
    image.flip_vertically();
    image.write_tga_file("triangle.tga");
}