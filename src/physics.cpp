//HEADER FILES INCLUDED
#include "physics.h"
#include "main.h"

/*
 * \brief	Takes the data of two cooliding and returns array of updated ball data
 * \param	Pxi	-	x coordinate of ball i
 * \param	Pyi	-	y coordinate of ball i	
 * \param	Pzi	-	z coordinate of ball i
 * \param	Vxi	-	velocity in x direction of ball i
 * \param	Vyi	-	velocity in y direction of ball i
 * \param	Vzi	-	velocity in z direction of ball i
 * \param	Pxi	-	x coordinate of ball k
 * \param	Pyi	-	y coordinate of ball k	
 * \param	Pzi	-	z coordinate of ball k
 * \param	Vxi	-	velocity in x direction of ball k
 * \param	Vyi	-	velocity in y direction of ball k
 * \param	Vzi	-	velocity in z direction of ball k
 */
 
float* velocities(float Pxi, float Pyi,float Pzi, float Vxi, float Vyi, float Vzi, float Pxj, float Pyj, float Pzj, float Vxj, float Vyj, float Vzj)
{
	float dot_x = (Vxi-Vxj)*(Pxi-Pxj) + (Vyi-Vyj)*(Pyi-Pyj) + (Vzi-Vzj)*(Pzi-Pzj) ;
	float norm = (Pxi-Pxj)*(Pxi-Pxj) + (Pyi-Pyj)*(Pyi-Pyj) + (Pzi-Pzj)*(Pzi-Pzj) ;
	float val = dot_x/norm ;
	float axial_x = Pxi-Pxj;
	float axial_y = Pyi-Pyj;
	float axial_z = Pzi-Pzj;
	float vxi = Vxi - (val*axial_x) ;
	float vyi = Vyi - (val*axial_y) ;
	float vzi = Vzi - (val*axial_z) ;
	float vxj = Vxi+Vxj-vxi;
	float vyj = Vyi+Vyj-vyi;
	float vzj = Vzi+Vzj-vzi;
	float *arr;
	arr = new float[6];
	arr[0] = vxi;
	arr[1] = vyi;
	arr[2] = vzi;
	arr[3] = vxj;
	arr[4] = vyj;
	arr[5] = vzj;
	return arr;
}

/*
 * \brief	Takes the data of two balls, checks if they are colliding and returns true
 * \param	Pxi	-	x coordinate of ball i
 * \param	Pyi	-	y coordinate of ball i	
 * \param	Pzi	-	z coordinate of ball i
 * \param	Pxi	-	x coordinate of ball j
 * \param	Pyi	-	y coordinate of ball j
 * \param	Pzi	-	z coordinate of ball j
 */

bool overlap(float Pxi, float Pyi, float Pzi, float Pxj, float Pyj, float Pzj)
{
    float d = ((Pxi-Pxj)*(Pxi-Pxj) + (Pyi-Pyj)*(Pyi-Pyj) + (Pzi-Pzj)*(Pzi-Pzj)) ;
    if (d<=(4*Radius*Radius)) return true;
    return false;
}

/*
 * \brief	Takes the data of a ball and checks it's collision with the walls of the cube, returns array of updated data
 * \param	Pxi	-	x coordinate of ball i
 * \param	Pyi	-	y coordinate of ball i	
 * \param	Pzi	-	z coordinate of ball i
 * \param	Vxi	-	velocity in x direction of ball i
 * \param	Vyi	-	velocity in y direction of ball i
 * \param	Vzi	-	velocity in z direction of ball i
 */

float* wall(float Pxi, float Pyi, float Pzi, float Vxi, float Vyi, float Vzi)
{
	bool flag=false;
	if (Pxi > 3*width/8 - Radius) 
    {
		Pxi = 3*width/8 - Radius; Vxi *=(-1);
		flag=true;
	}  
    if (Pyi > 3*height/8 - Radius) 
    {
		Pyi = 3*height/8 - Radius; Vyi *= (-1); 
		flag=true;
	}
    if (Pzi > 3*length/8 - Radius) 
    {
		Pzi = 3*length/8 - Radius; Vzi *=(-1);
		flag=true;
	}
    if (Pxi < (-3)*width/8 + Radius) 
    {
		Pxi = (-3)*width/8 + Radius; Vxi *=(-1);
		flag=true;
	}
    if (Pyi < (-3)*height/8 + Radius) 
    {
		Pyi = (-3)*height/8 + Radius; Vyi *=(-1); 
		flag=true;
	}
	if (Pzi < (-3)*length/8 + Radius) 
    {
		Pzi = (-3)*length/8 + Radius; Vzi *=(-1);
		flag=true;
	}
	float* arr;
	arr = new float[7];
	arr[0]=Pxi;
	arr[1]=Pyi;
	arr[2]=Pzi;
	arr[3]=Vxi;	
	arr[4]=Vyi;
	arr[5]=Vzi;
	
	if(flag)
		arr[6]=1;
	else
		arr[6]=0;
	return arr;
}


