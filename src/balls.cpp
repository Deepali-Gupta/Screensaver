//LIBRARIES INCLUDED
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <ctime>
#include <unistd.h>

//HEADER FILES INCLUDED
#include "balls.h"
#include "main.h"
#include "buttons.h"
#include "display.h"

#define sides 50

extern Ball b[COUNT];
extern Mouse TheMouse;

float Ball:: get_px()
{
	return px;
}
float Ball:: get_py()
{
	return py;
}
float Ball:: get_pz()
{
	return pz;
}
float Ball:: get_vx()
{
	return vx;
	}
float Ball:: get_vy()
{
	return vy;
}
float Ball:: get_vz()
{
	return vz;
}
int Ball:: get_r()
{
	return red;
}
int Ball:: get_b()
{
	return blue;
}
int Ball:: get_g()
{
	return green;
}
int Ball:: get_r2()
{
	return r2;
}
int Ball:: get_b2()
{
	return b2;
}
int Ball:: get_g2()
{
	return g2;
}
float Ball:: get_rad()
{
	return rad;
}
void Ball:: set_px(float PX)
{
	px=PX;
}
void Ball:: set_pz(float PZ)
{
	pz=PZ;
}
void Ball:: set_py(float PY)
{
	py=PY;
}
void Ball:: set_vx(float VX)
{
	vx=VX;
}
void Ball:: set_vy(float VY)
{
	vy=VY;
}
void Ball:: set_vz(float VZ)
{
	vz=VZ;
}
void Ball:: set_r(int r)
{
	red=r;
}
void Ball:: set_b(int b)
{
	blue=b;
}
void Ball:: set_g(int g)
{
	green=g;
}
void Ball:: set_r2(int r)
{
	r2=r;
}
void Ball:: set_b2(int b)
{
	b2=b;
}
void Ball:: set_g2(int g)
{
	g2=g;
}
void Ball::set_rad(int r)
{
	rad = r;
}
int Ball::get_id()
{
	return id;
}
void Ball::set_id(int d)
{
	id=d;
}

/*
 * \brief	The Ball Class Constructor Definition, initialises the variables and ball surface	
 */
Ball::Ball()
{
	#ifdef DEBUG
		printf("\nBall# %d created\n",this->id);
	#endif
	//Initialize the ball variables randomly
	px=rand() *3.0* ((float)width) /4* RAND_MAX + ((float)(-3)*width/8);
	py=rand() *3.0* ((float)height) /4* RAND_MAX + ((float)(-3)*height/8);
	pz=rand() *3.0* ((float)length) /4* RAND_MAX + ((float)(-3)*length/8);
	vx=rand() * ((float)2*speed) / RAND_MAX + ((float)(-1)*speed);
	vy=rand() * ((float)2*speed) / RAND_MAX + ((float)(-1)*speed);
	vz=rand() * ((float)2*speed) / RAND_MAX + ((float)(-1)*speed);
	red=rand() * ((float)255) / RAND_MAX ;
	green=rand() * ((float)255) / RAND_MAX;
	blue=rand() * ((float)255) / RAND_MAX;
	r2=red;
	g2=green;
	b2=blue;
	state=0;
	rad = Radius; 

   // Initialize ball surface
   for (int i = 0; i <= sides; i++)
   {
      float angle1 = 2 * i * M_PI / sides;
      for (int j = 0; j <= sides; j++)
      {
         float angle2 = j * M_PI / sides;
         X[i][j] = cos(angle1) * sin(angle2);
         Y[i][j] = sin(angle1) * sin(angle2);
		 Z[i][j] = cos(angle2);  
      }
   }
}

/*
 * \brief	Draws the 3D ball
 */

void Ball::draw_ball()
{
   // Draw the surface
   for (int i = 0; i < sides; i++)
   {
      for (int j = 0; j < sides; j++)
      {
		  glBegin(GL_POLYGON);
		  glColor3ub(r2, g2, b2);
			glNormal3f(X[i][j], Y[i][j], Z[i][j]);
			glVertex3f(px+rad*X[i][j],py+rad*Y[i][j], pz+Radius*Z[i][j]);

			glNormal3f(X[i][j + 1], Y[i][j + 1], Z[i][j + 1]);
			glVertex3f(px+rad*X[i][j + 1],py+rad*Y[i][j + 1], pz+rad*Z[i][j + 1]);

			glNormal3f(X[i + 1][j + 1], Y[i + 1][j + 1], Z[i + 1][j + 1]);
			glVertex3f(px+rad*X[i + 1][j + 1],py+rad*Y[i + 1][j + 1], pz+rad*Z[i + 1][j + 1]);

			glNormal3f(X[i + 1][j], Y[i + 1][j], Z[i + 1][j]);
			glVertex3f(px+rad*X[i + 1][j],py+rad*Y[i + 1][j], pz+rad*Z[i + 1][j]);
         glEnd();
      }
   }
}

void Ball::TheBallCallback()
{
	#ifdef DEBUG
		printf("\nBall# %d clicked\n",this->id);
	#endif
	ballno=id;	
}

/*
 * \brief	Checks if Mouse is over Ball
 * \param	x	-	The Mouse x Coordinate
 * \parma	x	-	The Mouse y Coordinate
 */
int Ball::BallClickTest(int x,int y) 
{
	if(this) 
	{
		/*
		 *	If clicked within ball area, then return true
		 */
		 x=x-width/2;	//synchronize the mouse and opengl coordinate system
		 y=(-1)*y+height/2;
		 
	     if( x > px-Radius  && x < px+Radius &&  y > py-Radius  && y < py+Radius ) 
	     {
				return 1;
		 }
	}
	return 0;	
}

/*
 * \brief	Detects if ball is pressed and changes the state of the all	
 * \param	x	-	The Mouse x Coordinate
 * \parma	x	-	The Mouse y Coordinate
 */

void Ball::BallPress(int x,int y)
{	
	if(this)
	{
		/*
		 *	if the mouse click was within the ball's client area, 
		 *	set the state to true.
		 */
		if( BallClickTest(x,y) )
		{
			state = 1;
		}
	}
}

/*
 * \brief	Detects if ball is Released and restores the state of the ball
 * \param	x	-	The Mouse x Coordinate
 * \parma	x	-	The Mouse y Coordinate
 */

void Ball:: BallRelease(int x,int y)
{
	if(this) 
	{
		/*
		 *	If the mouse button was pressed within the ball area
		 *	as well as being released on the ball.....
		 */
		if( BallClickTest(TheMouse.xpress,TheMouse.ypress) && 
			BallClickTest(x,y) )
		{
			/*
			 *	Then if a callback function has been set, call it.
			 */
				TheBallCallback();			
		}
		/*
		 *	Set state back to zero.
		 */
		state = 0;
	}
}

	
