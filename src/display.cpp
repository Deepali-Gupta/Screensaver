//LIBRARIES INCLUDED
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <unistd.h>

//HEADER FILES INCLUDED
#include "display.h"
#include "main.h"
#include "balls.h"
#include "buttons.h"

#define sides 50

int ballno;

extern Ball b[COUNT];
extern Mouse TheMouse;

Button PauseButton = {-width/2+100,height/2-70, 100,25, 0,0, "Pause/Play", ThePauseCallback };
Button IncButton = {-width/2+250,height/2-70, 100,25, 0,0, "Speed+", TheIncCallback };
Button DecButton = {-width/2+400,height/2-70, 100,25, 0,0, "Speed-", TheDecCallback };
Button PlusButton = {-width/2+550,height/2-70, 100,25, 0,0, "Size+", ThePlusCallback };
Button MinusButton = {-width/2+700,height/2-70, 100,25, 0,0, "Size-", TheMinusCallback };
Button GravityButton = {-width/2+850,height/2-70, 100,25, 0,0, "Gravity On/Off", TheGravCallback };

/*
 * \brief	Initialises the graphic properties
 * \param	Ka	-	Ambient material property
 * \param	Kd	-	Diffuse material property
 * \param	Ks	-	Specular material property
 */

void init_material(float Ka,float Kd, float Ks, float Kp, float Mr, float Mg, float Mb)
{
   // Material variables
   float ambient[] = { Ka * Mr, Ka * Mg, Ka * Mb, 1.0 };
   float diffuse[] = { Kd * Mr, Kd * Mg, Kd * Mb, 1.0 };
   float specular[] = { Ks * Mr, Ks * Mg, Ks * Mb, 1.0 };

   // Initialize material
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Kp);
}

/*
 * \brief	Initialize light source
 */
 
void init_light(int light_source, float Lx, float Ly, float Lz, float Lr, float Lg, float Lb)
{
   // Light variables
   float light_position[] = { Lx, Ly, Lz, 0.0 };
   float light_color[] = { Lr, Lg, Lb, 1.0 };

   // Initialize light source
   glEnable(GL_LIGHTING);
   glEnable(light_source);
   glLightfv(light_source, GL_POSITION, light_position);
   glLightfv(light_source, GL_AMBIENT, light_color);
   glLightfv(light_source, GL_DIFFUSE, light_color);
   glLightfv(light_source, GL_SPECULAR, light_color);
   glLightf(light_source, GL_CONSTANT_ATTENUATION, 1.0);
   glLightf(light_source, GL_LINEAR_ATTENUATION, 0.0);
   glLightf(light_source, GL_QUADRATIC_ATTENUATION, 0.0);
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

/*
 * \brief	initialises the screen
 */
 
void init()
{
	#ifdef DEBUG
		printf("\nScreen Initialized\n");
	#endif
      // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-width/2, width/2, -height/2, height/2, -length/2, length/2);
   glEnable(GL_DEPTH_TEST);

   // Initialize smooth shading
   glShadeModel(GL_SMOOTH);
   init_light(GL_LIGHT0, 0, 1, 1, 1, 1, 1);
   init_light(GL_LIGHT1, 1, 0, 1, 1, 1, 1);
   init_light(GL_LIGHT2, 1, 1, 0, 1, 1, 1);   
}

void DrawCube()
{
    glMatrixMode(GL_MODELVIEW);
	glBegin(GL_QUADS);
	glColor3f( 0.15f, 0.15f, 0.15f);        
    glVertex3f((3*width/4-Radius)/2, (3*height/4-Radius)/2, (-3)*length/8);   
    glVertex3f((3*width/4-Radius)/2, ((-3)*height/4+Radius)/2, (-3)*length/8);
    glVertex3f(((-3)*width/4+Radius)/2, ((-3)*height/4+Radius)/2, (-3)*length/8);
    glVertex3f(((-3)*width/4+Radius)/2, (3*height/4-Radius)/2, (-3)*length/8); 
       

	glColor3f( 1.0f, 1.0f, 0.0f);
	glVertex3f((3*width/4-Radius)/2, (3*height/4-Radius)/2, -3*length/8);
	glVertex3f((3*width/4-Radius)/2, ((-3)*height/4+Radius)/2, -3*length/8);
	glVertex3f(3*width/8, (-1)*3*height/8, 3*length/8);
	glVertex3f(3*width/8, 3*height/8, 3*length/8);

	glColor3f( 0.0f, 1.0f, 0.0f);
	glVertex3f((3*width/4-Radius)/2, (3*height/4-Radius)/2, -3*length/8);
	glVertex3f(3*width/8, 3*height/8, 3*length/8);
	glVertex3f((-1)*3*width/8, 3*height/8, 3*length/8);
	glVertex3f(((-3)*width/4+Radius)/2, (3*height/4-Radius)/2, -3*length/8);

	glColor3f( 1.0f, 0.5f, 0.0f);
	glVertex3f(((-3)*width/4+Radius)/2, (3*height/4-Radius)/2, -3*length/8);
	glVertex3f((-1)*3*width/8, 3*height/8, 3*length/8);
	glVertex3f((-1)*3*width/8, (-1)*3*height/8, 3*length/8);
	glVertex3f(((-3)*width/4+Radius)/2, ((-3)*height/4+Radius)/2, -3*length/8);

	glColor3f( 1.0f, 0.5f, 0.5f);
	glVertex3f((-1)*3*width/8, (-1)*3*height/8, 3*length/8);
	glVertex3f(((-3)*width/4+Radius)/2, ((-3)*height/4+Radius)/2, -3*length/8);
	glVertex3f((3*width/4-Radius)/2, ((-3)*height/4+Radius)/2, -3*length/8);
	glVertex3f(3*width/8, (-1)*3*height/8, 3*length/8);
	glEnd();  
}

/*
 * \brief	The Glut Dispaly Function
 */
 
void display()
{   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity(); 

	DrawCube();
	Draw2D();
	init_material(0.2, 0.4, 0.4, 100 * (0.5), 0.2, 0.5, 0.3);
    for (int i = 0; i < COUNT; i++)
    {
		init_material(0.2, 0.4, 0.4, 100 * (0.5), (float)b[i].get_r2()/255, (float)b[i].get_g2()/255, (float)b[i].get_b2()/255);
		b[i].draw_ball();
    }
	
	glutSwapBuffers();
}

/*
 * \brief	Draws the buttons
 */
 
void Draw2D()
{
	ButtonDraw(&PauseButton);
	ButtonDraw(&IncButton);
	ButtonDraw(&DecButton);
	ButtonDraw(&PlusButton);
	ButtonDraw(&MinusButton);
	ButtonDraw(&GravityButton);
}
