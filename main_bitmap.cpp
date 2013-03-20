#include <stdio.h>
#include <math.h>
#include <time.h>
#include "imageIO.h"
#include "Def.h"

#define DIM 768
#define sphere_num 4
#define reflectcof 0.75

Sphere s[sphere_num];
Light l,l_2;
Point eye_pos = {375, 300, 1000};

bool sphere_intersect(Point str, Vector dir, int sphereno,float &t){
 //Compute A, B and C coefficients
    //a = d.d
	float a = dir*dir;	
	//b = 2*(o-c).d
	Vector o_c = str - s[sphereno].pos;
	float b = 2*o_c*dir;
	//c = (o-c).(o-c)-r^2
	float c = o_c*o_c - pow(s[sphereno].radius,2);

    //Find discriminant
    float disc = b * b - 4 * a * c;
    
    // if discriminant is negative there are no real roots, so return 
    // false as ray misses sphere
    if (disc < 0)
        return false;

    // compute q as described above
    float distSqrt = sqrtf(disc);
    float q;
    if (b < 0)
        q = (-b - distSqrt)/2.0;
    else
        q = (-b + distSqrt)/2.0;

    // compute t0 and t1
    float t0 = q / a;
    float t1 = c / q;

    // make sure t0 is smaller than t1
    if (t0 > t1){
        // if t0 is bigger than t1 swap them around
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }

    // if t1 is less than zero, the object is in the ray's negative direction
    // and consequently the ray misses the sphere
    if (t1 < 0)
        return false;

    // if t0 is less than zero, the intersection point is at t1
    if (t0 < 0)
        t = t1;  
    // else the intersection point is at t0
    else   
        t = t0;
    
   return true;
}

void display(){
	 ColorImage image;
    //int x,y;
    Pixel Pixel={0,0,0};
	image.init(DIM,DIM); // Initialize the image
	for (int i=0; i < DIM; ++i){
		for (int j=0; j < DIM; ++j){
			int offset = i + j * DIM;
			int level = 0;
			float coef = 1.0;
			
			Color color = {0.15, 0.15, 0.15};		//background ambient
			//Color shadow_color ={0,0,0};
			Point p = {i, j, 0};
			Ray r;
			r.start = eye_pos;
			r.dir = p - eye_pos; //ray的向量
			normalize(r.dir);
			
			int startSphere = -1;
			
			do{
				int i_sphere = -1;
				float tTable[sphere_num];
				
				// Finding the closest intersection 
				float tmin =100000;
				for ( int k = 0; k < sphere_num; ++k){ 
					tTable[k] = -1;
					if ( k != startSphere && sphere_intersect(r.start,r.dir,k,tTable[k]) ){
						if(tTable[k] < tmin){
							tmin = tTable[k];
							i_sphere = k;
						}
					}
				}
				startSphere = i_sphere;

				// Intersection Exsist 
				if (i_sphere >= 0){
					Ray shadowRay,H;
					shadowRay.start = r.start + tTable[i_sphere]*r.dir;
					H.start=0.5*(r.start+l.pos);
					H.dir=H.start-shadowRay.start;
					shadowRay.dir = l.pos - shadowRay.start;
					normalize(shadowRay.dir);normalize(H.dir);
					Vector norm = shadowRay.start - s[i_sphere].pos;
					normalize(norm);
				
					bool inShadow = false;
					// Shadow ray intersection 
					float u;
					for ( int k = 0; k < sphere_num; ++k){
						if (k!=startSphere && k!=i_sphere && sphere_intersect(shadowRay.start,shadowRay.dir,k,u) ){
							inShadow = true;
							color = coef * color * s[i_sphere].color * l.color;
							//color = 0.2*coef * color * shadow_color;
							break;
						}
					}
						
					if ( !inShadow ){
						//float power = 1 - (sqrt( pow(p.x-s[i_sphere].pos.x,2)+pow(p.y-s[i_sphere].pos.y,2) )/s[i_sphere].radius);
						float power = (shadowRay.dir * norm) / (length(shadowRay.dir) * length(norm)) ;
						float specular_power =(H.dir * norm) / (length(H.dir) * length(norm)) ;
						if(power <0)
							power = 0;
						/*if(specular_power <0)
							specular_power = 0;*/
						color += coef *pow(power,1) * s[i_sphere].color * l.color;
						color += pow(specular_power,10) * s[i_sphere].color * l.color;
						//color += coef * power *  l.color;
					}
					
					// reflected light 
					coef *= reflectcof;
					float cosn = 2*(r.dir*norm);
				
					r.start = shadowRay.start;
					r.dir = r.dir - cosn*norm;
					normalize(r.dir);
					coef=coef*0.3;
					
				}
				else{
					break;
				}
				++level;
			} while( level<2 );

			Pixel.R =(int) (color.red * 255);
           Pixel.G = (int) (color.green * 255);
           Pixel.B = (int)(color.blue *  255);
		   image.writePixel(i,j,Pixel); 
		}
	}
	image.outputPPM("Result.ppm");
}

int main(int argc, char **argv)
{
	
	Point lpos = {500,300, 250};
	
	l.pos = lpos;
	
	Color lcol = {1.0,1.0,1.0};
	l.color = lcol;
	
	/*
	Point spos[sphere_num] = { {380,340,-100},{480,200,-10},{200,300,-35},{350,500,-100},{130,150,-130} };//球的位置
	float srad[sphere_num] = {60,60,50,100,50};//球的半徑
	Color sphere_rgb[sphere_num] = { {0.5,0,0},{0,0,0.6},{0,0.5,0},{0.6,0,0.4},{0,0.5,0.6} };//球的顏色
	*/
	Point spos[sphere_num] = { {200,300,80},{200,300,300},{300,200,330},{300,200,0} };//球的位置
	float srad[sphere_num] = {60,20,20,20};//球的半徑
	Color sphere_rgb[sphere_num] = { {0.5,0,0},{0,0,0.6},{0,0.5,0.6},{0.5,0.5,0} };//球的顏色
	for(int i=0; i<sphere_num; ++i){
		s[i].pos = spos[i];
		s[i].radius = srad[i];
		s[i].color = sphere_rgb[i];
	}

	
	
	clock_t start, end;
	start = clock();
	display();
	
	end = clock();
	printf("Time Elapsed : %lf Seconds.\n",double(end - start)/CLOCKS_PER_SEC);
	
	return 0;
}