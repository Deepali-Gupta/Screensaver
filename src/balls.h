//LIBRARIES INCLUDED
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <unistd.h>

#define sides 50 //SIDES OF POLYGON

class Ball
{
	//The ball parameters
	int id;
	int state; //if pressed
	float px, py, pz;
	float vx, vy, vz;
	int red, green, blue;	//original color of balls
	int r2,g2,b2;	//changed colors on collision
	float rad;	
	float X[sides+1][sides+1],Y[sides+1][sides+1],Z[sides+1][sides+1];
	
		public:
	//Ball functions
	Ball();	//Constructor

	void draw_ball();
	int get_id();
	
	//GETTERS
	float get_px();	
	float get_py();
	float get_pz();
	float get_vx();	
	float get_vy();
	float get_vz();
	int get_r();	
	int get_b();	
	int get_g();
	int get_r2();	
	int get_b2();	
	int get_g2();	
	float get_rad();
	
	//SETTERS
	void set_id(int d);
	void set_px(float PX);
	void set_py(float PY);
	void set_pz(float PZ);	
	void set_vx(float VX);	
	void set_vy(float VY);
	void set_vz(float VZ);
	void set_r(int r);	
	void set_b(int b);	
	void set_g(int g);	
	void set_r2(int r);	
	void set_b2(int b);
	void set_g2(int g);
	void set_rad(int r);
	
	void BallPress(int x,int y);
	void BallRelease(int x,int y);
	int BallClickTest(int x,int y) ;
	void TheBallCallback();
};
